/**
 * \file    RoboRun.c
 * \author  gross10
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy run sate
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* standard libraries */
#include <string.h>
#include <math.h>

/* application */
#include "../AppConfig.h"
#include "../CANGatekeeper.h"
#include "../Timer.h"
#include "../ELP.h"
#include "../System.h"
#include "../nodes/NodeConfig.h"
#include "../Rangefinder.h"
#include "RoboSetup.h" /* next state if this one is completed successfully */
#include "RoboError.h" /* next state if this one is completed with errors */
#include "RoboRun.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Node */
#define NODE_POOL_ID_INFO      0
#define NODE_POOL_SIZE_INFO    1
#define NODE_POOL_LEVEL_INFO   2
/* CAN */
#define ROBO_SPEED             100     /* Speed in percent */
#define ROBO_BARRIER_FLAGS     0x0000
#define GOTO_ACK_DELAY         10      /* Delay in ms to wait before checking goto confirmation */
#define GOTO_NACK_MAX_RETRIES  5       /* Number of retries (incl. first try) if there's no confirmation from drive system (uint8_t) */
#define GOTO_STATERESP_TIMEOUT (500 / portTICK_RATE_MS)    /* Timeout in ticks to wait for GoTo state response. (portMAX_DELAY, portTICK_RATE_MS) */
#define MASK_24BIT             0xFFFFFF
#define GOTO_TIME_UNKNOWN      0xFFFFFF


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* RTOS */
static xTaskHandle xNodeTask_Handle = NULL;
xSemaphoreHandle sSyncRoboRunNodeTask; /*!< for RoboRun <-> node-task sync */
static node_t* node_task = NULL; /*!< pointer to the current running node */

/* game and strategy */
static node_t* nodes_red[NODE_QUANTITY] = {&node_mammoth_1,
                                           &node_mammoth_2,
                                           &node_mammoth_3,
                                           &node_mammoth_4,
                                           &node_mammoth_5,
                                           &node_mammoth_6,
                                           &node_fresco_1,
                                           &node_fresco_2,
                                           &node_fire_1_red,
                                           &node_fire_2_red,
                                           &node_fire_3_red}; /*!< node-set for the yellow teamcolor */
static node_t* nodes_yellow[NODE_QUANTITY] = {&node_mammoth_1,
                                              &node_mammoth_2,
                                              &node_mammoth_3,
                                              &node_mammoth_4,
                                              &node_mammoth_5,
                                              &node_mammoth_6,
                                              &node_fresco_1,
                                              &node_fresco_2,
                                              &node_fire_1_yellow,
                                              &node_fire_2_yellow,
                                              &node_fire_3_yellow}; /*!< node-set for the red teamcolor */
static node_t** nodes_game = NULL; /*!< node set of the current game-round */
static node_t* next_node; /*!< pointer to the next node */
static uint8_t remain_nodes; /*!< undone nodes */
static uint8_t enemy_count; /*!< enemy quantity */
static uint8_t node_pools[NODE_POOL_QUANTITY][3] = {{NODE_MAMMOTH_POOL_ID,
                                                     NODE_MAMMOTH_POOL_SIZE,
                                                     NODE_MAMMOTH_POOL_LEVEL},
                                                    {NODE_FRESCO_POOL_ID,
                                                     NODE_FRESCO_POOL_SIZE,
                                                     NODE_FRESCO_POOL_LEVEL}}; /*!< pool settings -> have to set to default values after game round */
volatile static uint16_t enemey_position[20][30] = {{0}}; /*!< enemy-tracking grid TODO*/
//volatile static uint16_t enemey_position[((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y))][((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))] = {{0.0}}; /*!< enemy-tracking grid */
volatile static game_state_t game_state = {0,               /*!< x-position */
                                            0,              /*!< y-position */
                                            0,              /*!< angle */
                                            NODE_NO_ENEMY,  /*!< x-position enemy 1 */
                                            NODE_NO_ENEMY,  /*!< y-position enemy 1 */
                                            NODE_NO_ENEMY,  /*!< x-position enemy 2 */
                                            NODE_NO_ENEMY}; /*!< y-position enemy 2 */


/* Private function prototypes -----------------------------------------------*/
static void vNodeTask(void*);
static void vTrackEnemy(uint16_t, CAN_data_t*);
static void vMyPosition(uint16_t, CAN_data_t*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboRunState
 * \brief   initialisation of the RoboRunState
 */
void initRoboRunState()
{
    /* create the task */
    //xTaskCreate( vNodeTask, ( signed char * ) SYSTEM_NODE_TASK_NAME, SYSTEM_NODE_STACK_SIZE, NULL, SYSTEM_NODE_TASK_PRIORITY, &xNodeTask_Handle );

#ifndef STANDALONE
    /* init node resources */
    initNodeResources();
#endif

    /* suspend the node task until they will used */
    //vTaskSuspend(xNodeTask_Handle);

    /* create sync-semaphore */
    vSemaphoreCreateBinary(sSyncRoboRunNodeTask); /* for RoboRun <-> node-task sync */
    //xSemaphoreTake(sSyncRoboRunNodeTask,0); /* take semaphore here, because the node-task will empty at the beginning */

    /* set CAN listeners */
    setFunctionCANListener(vTrackEnemy,ENEMEY_1_POSITION_RESPONSE);
    setFunctionCANListener(vTrackEnemy,ENEMEY_2_POSITION_RESPONSE);
    setFunctionCANListener(vMyPosition,KALMAN_POSITION_RESPONSE);
}


/**
 * \fn      setConfigRoboRunState
 * \brief   set the necessary configurations for operation
 * \note    have to call before runRoboRunState
 *
 * \param[in]   start_node_id   number of the first node
 * \param[in]   teamcolor       red or yellow
 * \param[in]   enemies         enemy quantity
 *
 * \retval      1   set start configuration was successful
 * \retval      0   set start configuration was not possible
 */
uint8_t setConfigRoboRunState(uint8_t start_node_id, uint8_t teamcolor, uint8_t enemies)
{
    /* local variables */
    uint8_t node_count;
    uint8_t success = 0;

    /* load correct node-set */
    if(teamcolor == GIP_TEAMCOLOR_YELLOW)
    {
        nodes_game = nodes_yellow;
    }
    else
    {
        nodes_game = nodes_red;
    }
    remain_nodes = NODE_QUANTITY; /* set counter to max. */

    /* search start-node address */
    for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
    {
        if(((*nodes_game)+node_count)->param.id == start_node_id)
        {
            next_node = ((*nodes_game)+node_count);
            success = 1;
            break;
        }
    }

    /* set enemy count */
    enemy_count = enemies;

    /* create the node-task */
    xTaskCreate(vNodeTask, ( signed char * ) SYSTEM_NODE_TASK_NAME,
                SYSTEM_NODE_STACK_SIZE, NULL, SYSTEM_NODE_TASK_PRIORITY, &xNodeTask_Handle );
    /* suspend the node task until they will used */
    vTaskSuspend(xNodeTask_Handle);
    /* take semaphore here, because the node-task will empty at the beginning */
    xSemaphoreTake(sSyncRoboRunNodeTask,0);

    return success;
}


/**
 * \fn      setConfigRoboRunState2Default
 * \brief   set all necessary values to default
 */
void setConfigRoboRunState2Default()
{
    /* local variable */
    uint8_t node_count;// y, x;

    /* set all nodes to state UNDONE */
    if(nodes_game != NULL)
    {
        for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
        {
            ((*nodes_game)+node_count)->param.node_state = NODE_UNDONE;
        }
    }

    /* node pools to default */
    node_pools[NODE_MAMMOTH_POOL_ID-1][NODE_POOL_SIZE_INFO] = NODE_MAMMOTH_POOL_SIZE;
    node_pools[NODE_MAMMOTH_POOL_ID-1][NODE_POOL_LEVEL_INFO] = NODE_MAMMOTH_POOL_LEVEL;
    node_pools[NODE_FRESCO_POOL_ID-1][NODE_POOL_SIZE_INFO] = NODE_FRESCO_POOL_SIZE;
    node_pools[NODE_FRESCO_POOL_ID-1][NODE_POOL_LEVEL_INFO] = NODE_FRESCO_POOL_LEVEL;

    /* TODO enemy-field to default */
    memset(enemey_position,0,sizeof(enemey_position[0][0]) * ((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))
            * ((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y)));
//    for(y=0; y < ((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y)); y++)
//    {
//        for(x=0; x < ((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X)); x++)
//        {
//            enemey_position[y][x] =
//        }
//    }
    /* give the node-task semaphore free */
    xSemaphoreGive(sSyncRoboRunNodeTask);

    /* delete task until the next game round */
    if(xNodeTask_Handle != NULL)
    {
        vTaskDelete(xNodeTask_Handle);
    }

    /* stop timers */
    stopGameTimer();
    stopELP();
}


/**
 * \fn      runRoboRunState
 * \brief   most important state of whole system.
 *
 * \param[in]   tick systicks for absolute RTOS delay
 */
void runRoboRunState(portTickType* tick)
{
    /* local variables */
    float weight_dec, weight_inc; /* game overall weights, depends on time */
    float weight_dest; /* costs of the destination node */
    float weight_enemy; /* enemy tracking weight */
    float weight_src_dest; /* way-time weight (estimated) */
    float weight_next_node; /* the weight of the next node */
    uint8_t weight_arrive; /* additional weight for bad arrives */
    uint8_t remain_time;
    uint8_t node_count; /* simple count variable */
    node_t* current_node;


    /* load next node in node task */
    current_node = next_node;
    node_task = next_node;

    vTaskResume(xNodeTask_Handle);

    /* wait until current node is done */
    xSemaphoreTake(sSyncRoboRunNodeTask,portMAX_DELAY);


    /*********************/
    /* check node result */
    /*********************/
    if(current_node->param.node_state == NODE_FINISH_SUCCESS)
    {
        remain_nodes--;

        if(current_node->param.pool_id != NODE_NO_POOL_ID)
        {
            node_pools[current_node->param.pool_id-1][NODE_POOL_SIZE_INFO]--;

            /* all necessary nodes within the pool are done */
            if(node_pools[current_node->param.pool_id-1][NODE_POOL_SIZE_INFO] ==
                    node_pools[current_node->param.pool_id-1][NODE_POOL_LEVEL_INFO])
            {
                remain_nodes -= node_pools[current_node->param.pool_id-1][NODE_POOL_LEVEL_INFO];

                /* set all remaining nodes of the pool to FINISH_SUCCESS */
                for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
                {
                    if(((*nodes_game)+node_count)->param.pool_id == current_node->param.pool_id)
                    {
                        ((*nodes_game)+node_count)->param.node_state = NODE_FINISH_SUCCESS;
                    }
                }
            }
        }
    }
    else
    {
        remain_nodes--;
    }


    /******************/
    /* calc next node */
    /******************/
    remain_time = getRemainingGameTime();
    weight_dec = remain_time / PLAY_TIME;
    weight_inc = (PLAY_TIME - remain_time) / PLAY_TIME;
    weight_next_node = INFINITY;

    for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
    {
        /* calculate the weights only for undone nodes */
        if(((*nodes_game)+node_count)->param.node_state == NODE_UNDONE)
        {
            /* destination weight depends on the current robo-position and the arrive-direction */
            /* NORTH */
            if(((*nodes_game)+node_count)->param.angle >= NODE_NORTH_MIN_ANGLE &&
                    ((*nodes_game)+node_count)->param.angle <= NODE_NORTH_MAX_ANGLE)
            {
                /* opposite arrive */
                if(((*nodes_game)+node_count)->param.y > current_node->param.y)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }else if(((*nodes_game)+node_count)->param.y <= current_node->param.y &&
                        ((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME > current_node->param.y)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }else if(((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME <= current_node->param.y &&
                        (((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME >= current_node->param.x ||
                        ((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME <= current_node->param.x))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }

            /* WEST */
            } else if(((*nodes_game)+node_count)->param.angle >= NODE_WEST_MIN_ANGLE &&
                    ((*nodes_game)+node_count)->param.angle <= NODE_WEST_MAX_ANGLE)
            {
                /* opposite arrive */
                if(((*nodes_game)+node_count)->param.x < current_node->param.x)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }else if(((*nodes_game)+node_count)->param.x >= current_node->param.x &&
                        ((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME < current_node->param.x)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }else if(((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME >= current_node->param.x &&
                        (((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME >= current_node->param.y ||
                        ((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME <= current_node->param.y))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }

            /* SOUTH */
            } else if(((*nodes_game)+node_count)->param.angle >= NODE_SOUTH_MIN_ANGLE &&
                    ((*nodes_game)+node_count)->param.angle <= NODE_SOUTH_MAX_ANGLE)
            {
                /* opposite arrive */
                if(((*nodes_game)+node_count)->param.y < current_node->param.y)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }else if(((*nodes_game)+node_count)->param.y >= current_node->param.y &&
                        ((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME < current_node->param.y)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }else if(((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME >= current_node->param.y &&
                        (((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME >= current_node->param.x ||
                        ((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME <= current_node->param.x))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }

            /* EAST */
            } else
            {
                /* opposite arrive */
                if(((*nodes_game)+node_count)->param.x > current_node->param.x)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }else if(((*nodes_game)+node_count)->param.x <= current_node->param.x &&
                        ((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME > current_node->param.x)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }else if(((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME <= current_node->param.x &&
                        (((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME >= current_node->param.y ||
                        ((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME <= current_node->param.y))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }
            }
//            /* destination weight depends on the current robo-position and the arrive-direction */
//            switch(((*nodes_game)+node_count)->param.arrive)
//            {
//                case NORTH:
//                    /* opposite arrive */
//                    if(((*nodes_game)+node_count)->param.y > current_node->param.y)
//                    {
//                        weight_arrive = NODE_WORST_ARRIVE;
//                    /* too close */
//                    }else if(((*nodes_game)+node_count)->param.y <= current_node->param.y &&
//                            ((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME > current_node->param.y)
//                    {
//                        weight_arrive = NODE_BAD_ARRIVE;
//                    /* not bad, but not perfect as well */
//                    }else if(((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME <= current_node->param.y &&
//                            (((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME >= current_node->param.x ||
//                            ((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME <= current_node->param.x))
//                    {
//                        weight_arrive = NODE_WELL_ARRIVE;
//                    /* best possible arrive */
//                    }else
//                    {
//                        weight_arrive = NODE_PERFECT_ARRIVE;
//                    }
//                    break;
//
//                case EAST:
//                    /* opposite arrive */
//                    if(((*nodes_game)+node_count)->param.x > current_node->param.x)
//                    {
//                        weight_arrive = NODE_WORST_ARRIVE;
//                    /* too close */
//                    }else if(((*nodes_game)+node_count)->param.x <= current_node->param.x &&
//                            ((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME > current_node->param.x)
//                    {
//                        weight_arrive = NODE_BAD_ARRIVE;
//                    /* not bad, but not perfect as well */
//                    }else if(((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME <= current_node->param.x &&
//                            (((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME >= current_node->param.y ||
//                            ((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME <= current_node->param.y))
//                    {
//                        weight_arrive = NODE_WELL_ARRIVE;
//                    /* best possible arrive */
//                    }else
//                    {
//                        weight_arrive = NODE_PERFECT_ARRIVE;
//                    }
//                    break;
//
//                case SOUTH:
//                    /* opposite arrive */
//                    if(((*nodes_game)+node_count)->param.y < current_node->param.y)
//                    {
//                        weight_arrive = NODE_WORST_ARRIVE;
//                    /* too close */
//                    }else if(((*nodes_game)+node_count)->param.y >= current_node->param.y &&
//                            ((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME < current_node->param.y)
//                    {
//                        weight_arrive = NODE_BAD_ARRIVE;
//                    /* not bad, but not perfect as well */
//                    }else if(((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME >= current_node->param.y &&
//                            (((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME >= current_node->param.x ||
//                            ((*nodes_game)+node_count)->param.x + NODE_ARRIVE_FRAME <= current_node->param.x))
//                    {
//                        weight_arrive = NODE_WELL_ARRIVE;
//                    /* best possible arrive */
//                    }else
//                    {
//                        weight_arrive = NODE_PERFECT_ARRIVE;
//                    }
//                    break;
//
//                case WEST:
//                    /* opposite arrive */
//                    if(((*nodes_game)+node_count)->param.x < current_node->param.x)
//                    {
//                        weight_arrive = NODE_WORST_ARRIVE;
//                    /* too close */
//                    }else if(((*nodes_game)+node_count)->param.x >= current_node->param.x &&
//                            ((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME < current_node->param.x)
//                    {
//                        weight_arrive = NODE_BAD_ARRIVE;
//                    /* not bad, but not perfect as well */
//                    }else if(((*nodes_game)+node_count)->param.x - NODE_ARRIVE_FRAME >= current_node->param.x &&
//                            (((*nodes_game)+node_count)->param.y - NODE_ARRIVE_FRAME >= current_node->param.y ||
//                            ((*nodes_game)+node_count)->param.y + NODE_ARRIVE_FRAME <= current_node->param.y))
//                    {
//                        weight_arrive = NODE_WELL_ARRIVE;
//                    /* best possible arrive */
//                    }else
//                    {
//                        weight_arrive = NODE_PERFECT_ARRIVE;
//                    }
//                    break;
//            }
            weight_dest =((((*nodes_game)+node_count)->param.points/((*nodes_game)+node_count)->param.time)
                    * (1/((*nodes_game)+node_count)->param.percent)) * weight_arrive;

            /* read out the enemy track-position for the node location */
            taskENTER_CRITICAL();
            weight_enemy = enemey_position[(int)(((*nodes_game)+node_count)->param.y / (ENEMY_GRID_SIZE_Y*1000))]
                                           [(int)(((*nodes_game)+node_count)->param.y / (ENEMY_GRID_SIZE_Y*1000))];
            taskEXIT_CRITICAL();

            /* source -> destination node distance-time weight */
            weight_src_dest = sqrtf((fabsf(current_node->param.x - ((*nodes_game)+node_count)->param.x) * fabsf(current_node->param.x - ((*nodes_game)+node_count)->param.x)) +
                    (fabsf(current_node->param.y - ((*nodes_game)+node_count)->param.y) * fabsf(current_node->param.y - ((*nodes_game)+node_count)->param.y))) / ROBO_AVERAGE_SPEED;


            /* searching next node */
            if((weight_dest * weight_dec + weight_enemy * weight_dec + weight_src_dest * weight_inc) < weight_next_node)
            {
                next_node = ((*nodes_game)+node_count);
            }
        }
    }


    /* game is finished */
    if(remain_nodes == 0)
    {
    	setConfigRoboRunState2Default();

    	system_state = runRoboSetupState;
    }
}


/**
 * \fn          vMyPosition
 * \brief       save the current robo position
 * \note        read access has to be atomic!
 *
 * \param[in]   id      CAN message ID
 * \param[in]   data    CAN message data (here, ELP)
 * \return      None
 */
static void vMyPosition(uint16_t id, CAN_data_t* data)
{
    game_state.x = data->elp_x;
    game_state.y = data->elp_y;
    game_state.angle = data->elp_angle;
}


/**
 * \fn          vTrackEnemy
 * \brief       Track the enemys-position in an simple grid array
 * \note        read access has to be atomic!
 *
 * \param[in]   id      CAN message ID
 * \param[in]   data    CAN message data (here, ELP)
 * \return      None
 */
static void vTrackEnemy(uint16_t id, CAN_data_t* data)
{
    /* local variable */
    uint8_t i;

    if(enemy_count > 0)
    {
        /* check if the position within the grid and not on the frame */
        if((data->elp_y/(ENEMY_GRID_SIZE_Y*1000)) < (PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y-1) &&
                (data->elp_y/(ENEMY_GRID_SIZE_Y*1000)) > 0 &&
                (data->elp_x/(ENEMY_GRID_SIZE_X*1000)) > 0 &&
                (data->elp_x/(ENEMY_GRID_SIZE_X*1000)) < (PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X-1))
        {
            /* set center weight */
            enemey_position[(int)(data->elp_y/(ENEMY_GRID_SIZE_Y*1000))][(int)(data->elp_x/(ENEMY_GRID_SIZE_X*1000))] += ENEMY_GRID_CENTER_WEIGHT;

            /* set upper frame edge */
            for(i=0; i<3; i++)
            {
                enemey_position[(int)(data->elp_y/(ENEMY_GRID_SIZE_Y*1000)-1)][(int)(data->elp_x/(ENEMY_GRID_SIZE_X*1000)-1+i)] += ENEMY_GRID_FRAME_WEIGHT;
            }

            /* set deeper frame edge */
            for(i=0; i<3; i++)
            {
                enemey_position[(int)(data->elp_y/(ENEMY_GRID_SIZE_Y*1000)+1)][(int)(data->elp_x/(ENEMY_GRID_SIZE_X*1000)-1+i)] += ENEMY_GRID_FRAME_WEIGHT;
            }

            /* set left frame edge */
            enemey_position[(int)(data->elp_y/(ENEMY_GRID_SIZE_Y*1000))][(int)(data->elp_x/(ENEMY_GRID_SIZE_X*1000)-1)] += ENEMY_GRID_FRAME_WEIGHT;

            /* set right frame edge */
            enemey_position[(int)(data->elp_y/(ENEMY_GRID_SIZE_Y*1000))][(int)(data->elp_x/(ENEMY_GRID_SIZE_X*1000)+1)] += ENEMY_GRID_FRAME_WEIGHT;
        }

        /* set game data */
        if(id == ENEMEY_1_POSITION_RESPONSE)
        {
            game_state.enemy_1_x = data->elp_x;
            game_state.enemy_1_y = data->elp_y;
        }
        else
        {
            game_state.enemy_2_x = data->elp_x;
            game_state.enemy_2_y = data->elp_y;
        }

    }
}


/**
 * \fn          gotoNode
 * \brief       Function to give the GoTo command and monitor the rangefinder whle moving.
 *
 * \param[in]   None
 * \return      None
 */
void gotoNode(node_param_t* param, volatile game_state_t* game_state)
{
	/* Activate rangefinder */
	vTaskResume(xRangefinderTask_Handle);

	/* Variable for CAN RX */
	CAN_data_t CAN_buffer;
	uint8_t CAN_ok = pdFALSE;

	/* Variable to store estimated GoTo time received from drive system (24 Bit, 1 Bit ca. 1 ms) */
	uint32_t estimated_GoTo_time;

	uint8_t i=0;
	do {

		i++;

		/* Send GoTo command through CAN to drive system */
		txGotoXY(param->x, param->y, param->angle, ROBO_SPEED, ROBO_BARRIER_FLAGS);

		/* Receive GoTo confirmation */
		CAN_ok = xQueueReceive(qGotoConfirm, &CAN_buffer, GOTO_ACK_DELAY / portTICK_RATE_MS);

	/* Retry if no transmission confirmed received and another retry is allowed */
	} while((CAN_ok != pdTRUE) && (i <= GOTO_NACK_MAX_RETRIES));

	/* If still no GoTo confirmation was received, report error */
	if(CAN_ok != pdTRUE) {

		/* No confirmation was received from drive system! */
		param->node_state = GOTO_CAN_ERROR;
		return;
	}

	do {

		/* Ask drive system for GoTo state */
		txGotoStateRequest();
		/* Receive the GoTo state response */
		CAN_ok = xQueueReceive(qGotoStateResp, &CAN_buffer, GOTO_STATERESP_TIMEOUT);
		/* Check if time out */
		if(CAN_ok != pdTRUE) {

			/* Drive system didn't answer within specified time, report */
			param->node_state =  GOTO_CAN_ERROR;
			return;
		}

		/* Extract time */
		estimated_GoTo_time = CAN_buffer.state_time && MASK_24BIT;  /* In ms */
		if(estimated_GoTo_time == GOTO_TIME_UNKNOWN) {

			/* Set to 1 second -> Retry in 1 second */
			estimated_GoTo_time = 1000;
		}

		/* Try to take semaphore from rangefinder task, use estimated time from drive system as timeout */
		if(estimated_GoTo_time != 0 && xSemaphoreTake(sSyncNodeTask, estimated_GoTo_time / portTICK_RATE_MS) == pdTRUE) {

			/* Semaphore received, this means an obstacle was detected! */

			/* Check rangefinder- and current robot state infos, and deside if a emergency break is needed */
			//TODO (and don't forget to use critical to be sure game_state isn't changed in the meantime --> Issue #27)

			/* Semaphore is always only given by rangefinder task and always only taken by node task */
		}

	/* Repeat while not at target destination */
	} while(estimated_GoTo_time != 0);

	/* Suspend rangefinder safely */
	suspendRangefinderTask();
}


/**
 * \fn          vNodeTask
 * \brief       Task to handle the strategy
 *
 * \param[in]   None
 * \return      None
 */
static void vNodeTask(void* pvParameters )
{
    /* endless */
    for(;;)
    {
    	/* Give goto command */
    	gotoNode(&node_task->param, &game_state);

    	/* Only if there was no CAN error */
    	if(node_task->param.node_state != GOTO_CAN_ERROR) {
    		/* Do node action */
    		node_task->node_function(&node_task->param);
    	}

        taskENTER_CRITICAL();
        xSemaphoreGive(sSyncRoboRunNodeTask);
        vTaskSuspend(NULL);
        taskEXIT_CRITICAL();
    }
}

/**
 * @}
 */
