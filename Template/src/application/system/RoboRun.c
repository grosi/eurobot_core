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
#include "RoboSetup.h" /* previous state */
#include "RoboRunExtend.h" /* next state if this one is completed successfully */
#include "RoboError.h" /* next state if this one is completed with errors */
#include "RoboRun.h"

/* lib */
#include "lib/display.h"
#include "lib/sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* System */
#define BINARY_SEMAPHORE_LENGTH 1
#define DRIVE_ROUTE_DIST_MIN   15       /* Min. drive distance in cm for which the drive system calculates a route an not just drives */
/* Node */
#define NODE_POOL_ID_INFO      0
#define NODE_POOL_SIZE_INFO    1
#define NODE_POOL_LEVEL_INFO   2
/* CAN */
#define GOTO_DEFAULT_SPEED             100      /* Speed in percent */
#define GOTO_ROBO_BARRIER_FLAGS     0x0000
#define GOTO_DEFAULT_BARRIER_R GOTO_FIRE_3_FORCE|GOTO_FIRE_5_FORCE|GOTO_FIRE_POOL_1_FORCE //GOTO_FIRE_1_FORCE| /* Barrier flags for red team */
#define GOTO_DEFAULT_BARRIER_Y GOTO_FIRE_2_FORCE|GOTO_FIRE_4_FORCE|GOTO_FIRE_6_FORCE|GOTO_FIRE_POOL_2_FORCE /* Barrier flags for yellow team */
#define GOTO_ACK_DELAY         20       /* Delay in ms to wait before checking goto confirmation */
#define GOTO_NACK_MAX_RETRIES  5        /* Number of retries (incl. first try) if there's no confirmation from drive system (uint8_t) */
#define GOTO_STATERESP_DELAY   100      /* Delay in ms to wait for GoTo state response. Drive system needs 400 ms (worst case) */
#define GOTO_STATERESP_TIMEOUT 400      /* Time in ms to wait for the GoTo state response */
#define GOTO_NOT_POSSIBLE_ATM  0xFFFFFF /* Message: It's not possible to go to that position at the moment */
#define GOTO_DEFAULT_TIME      1000     /* Time in ms to use if no time was received from drive system */


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* RTOS */
static xTaskHandle xNodeTask_Handle = NULL;
xSemaphoreHandle sSyncRoboRunNodeTask; /*!< for RoboRun <-> node-task sync */

/* game and strategy -> don't change the order!!!*/
static volatile node_t* nodes_red[NODE_QUANTITY] = {&node_fire_1_red,
                                                    &node_fire_2_red,
                                                    &node_fire_3_red,
                                                    &node_fire_pool_red,
                                                    &node_fire_wall_normal_1_red,
                                                    &node_fire_wall_normal_2_red,
                                                    &node_fire_wall_invers_red,
                                                    &node_net_1_red,
                                                    &node_net_2_red,
                                                    &node_net_3_red}; /*!< node-set for the yellow teamcolor */
static volatile node_t* nodes_yellow[NODE_QUANTITY] = {&node_fire_1_yellow,
                                                       &node_fire_2_yellow,
                                                       &node_fire_3_yellow,
                                                       &node_fire_pool_yellow,
                                                       &node_fire_wall_normal_1_yellow,
                                                       &node_fire_wall_normal_2_yellow,
                                                       &node_fire_wall_invers_yellow,
                                                       &node_net_1_yellow,
                                                       &node_net_2_yellow,
                                                       &node_net_3_yellow}; /*!< node-set for the yellow teamcolor */
static node_t* nodes_game[NODE_QUANTITY];
static node_t* node_task = NULL; /*!< pointer to the current running node */
static node_t* next_node; /*!< pointer to the next node */
static uint8_t remain_nodes; /*!< undone nodes */
static uint8_t node_pools[NODE_POOL_QUANTITY][3] = {{NODE_NET_POOL_ID,
                                                     NODE_NET_POOL_SIZE,
                                                     NODE_NET_POOL_LEVEL}};/*!< pool settings -> have to set to default values after game round */
volatile static uint16_t enemy_position[20][30] = {{0}}; /*!< enemy-tracking grid TODO*/
//volatile static uint16_t enemey_position[((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y))][((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))] = {{0.0}}; /*!< enemy-tracking grid */
volatile static game_state_t game_state = { .x = 0,                      /*!< x-position */
                                            .y = 0,                      /*!< y-position */
                                            .angle = 0,                  /*!< angle */
                                            .teamcolor = TEAM_YELLOW,    /*!< color of our team */
                                            .enemy_count = 0,            /*!< number of enemies */
                                            .enemy_1_x = NODE_NO_ENEMY,  /*!< x-position enemy 1 */
                                            .enemy_1_y = NODE_NO_ENEMY,  /*!< y-position enemy 1 */
                                            .enemy_1_diameter = NODE_NO_ENEMY_DIAMETER, /*!< diameter of enemy 1 [cm] */
                                            .enemy_2_x = NODE_NO_ENEMY,  /*!< x-position enemy 2 */
                                            .enemy_2_y = NODE_NO_ENEMY,  /*!< y-position enemy 2 */
                                            .enemy_2_diameter = NODE_NO_ENEMY_DIAMETER,  /*!< diameter of enemy 2 [cm] */
                                            .confederate_count = 0,      /*!< number of enemies */
                                            .confederate_x = NODE_NO_CONFEDERATE,  /*!< x-position enemy 1 */
                                            .confederate_y = NODE_NO_CONFEDERATE  /*!< y-position enemy 1 */
                                           };


/* Private function prototypes -----------------------------------------------*/
static void vNodeTask(void*);
static void vTrackEnemy(uint16_t, CAN_data_t*);
static void vConfederatePosition(uint16_t, CAN_data_t*);
static void vMyPosition(uint16_t, CAN_data_t*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboRunState
 * \brief   initialisation of the RoboRunState
 */
void initRoboRunState()
{
#ifndef STANDALONE
    /* init node resources */
    initNodeResources();
#endif

    /* create sync-semaphore*/
    vSemaphoreCreateBinary(sSyncRoboRunNodeTask); /* for RoboRun <-> node-task sync */

#ifdef DEBUGGING
    vQueueAddToRegistry(sSyncRoboRunNodeTask, (signed char*) "sSyncRoboRun");
    vQueueAddToRegistry(sSyncEmergencyStopRoboState, (signed char*) "sSyncEmergency");
#endif

    /* set CAN listeners */
    setFunctionCANListener(vTrackEnemy,ENEMEY_1_POSITION_RESPONSE);
    setFunctionCANListener(vTrackEnemy,ENEMEY_2_POSITION_RESPONSE);
    setFunctionCANListener(vConfederatePosition,CONFEDERATE_POSITION_RESPONSE);
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
 * \param[in]   confederate     confederate quantity
 * \param[in]   enemy_size_1    diameter of enemy 1
 * \param[in]   enemy_size_2    diameter of enemy 2
 *
 * \retval      1   set start configuration was successful
 * \retval      0   set start configuration was not possible
 */
uint8_t setConfigRoboRunState(uint8_t start_node_id, uint8_t teamcolor, uint8_t enemies, uint8_t confederate, uint8_t enemy_size_1, uint8_t enemy_size_2)
{
    /* local variables */
    uint8_t node_count;
    uint8_t success = 0;

    /* save color in game_state */
    game_state.teamcolor = teamcolor;

    /* load correct node-set */
    taskENTER_CRITICAL();
    if(teamcolor == GIP_TEAMCOLOR_YELLOW)
    {
        memcpy(&nodes_game,nodes_yellow,sizeof(node_t*[NODE_QUANTITY]));
        game_state.teamcolor = TEAM_YELLOW;
        game_state.barrier = GOTO_DEFAULT_BARRIER_Y;
    }
    else
    {
        memcpy(&nodes_game,nodes_red,sizeof(node_t*[NODE_QUANTITY]));
        game_state.teamcolor = TEAM_RED;
        game_state.barrier = GOTO_DEFAULT_BARRIER_R;
    }
    remain_nodes = NODE_QUANTITY; /* set counter to max. */
    taskEXIT_CRITICAL();

    /* search start-node address */
    for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
    {
        if(nodes_game[node_count]->param.id == start_node_id)
        {
            next_node = nodes_game[node_count];
            success = 1;
            break;
        }
    }

    /* set enemy count */
    game_state.enemy_count = enemies;
    game_state.enemy_1_diameter = enemy_size_1; /* diameter in centimeter */
    game_state.enemy_2_diameter = enemy_size_2; /* diameter in centimeter */

    /* set confederate count */
    game_state.confederate_count = confederate;

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
 * \note    do not call this function from a ISR!
 */
void setConfigRoboRunState2Default()
{
    /* local variable */
    uint8_t node_count;

    /* set all nodes to state UNDONE (except net-nodes -> NODE_FINISH_SUCCESS) */
    if(nodes_game != NULL)
    {
        for(node_count = 0; node_count < NODE_QUANTITY-3; node_count++)
        {
            nodes_game[node_count]->param.node_state = NODE_UNDONE;
            nodes_game[node_count]->param.node_tries = 1;
        }
        for(;node_count < NODE_QUANTITY; node_count++)
        {
            nodes_game[node_count]->param.node_state = NODE_FINISH_SUCCESS;
        }
    }

    /* node pools to default */
    node_pools[NODE_NET_POOL_ID-1][NODE_POOL_SIZE_INFO] = NODE_NET_POOL_SIZE;
    node_pools[NODE_NET_POOL_ID-1][NODE_POOL_LEVEL_INFO] = NODE_NET_POOL_LEVEL;

    taskENTER_CRITICAL(); /* for more safety */

    /* enemy-field to default */
    memset(enemy_position,0,sizeof(enemy_position[0][0]) * ((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))
            * ((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y)));

    /* give the node-task semaphore free */
    xSemaphoreGive(sSyncRoboRunNodeTask);

    /* delete task until the next game round */
    if(xNodeTask_Handle != NULL)
    {
        vTaskDelete(xNodeTask_Handle);
    }

    /* stop timers */
    stopELP();
    stopGameTimer();

    taskEXIT_CRITICAL();
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
    uint8_t weight_arrive = NODE_PERFECT_ARRIVE; /* additional weight for bad arrives */
    uint8_t remain_time;
    uint8_t node_count; /* simple count variable */
    volatile node_t* current_node;
    uint8_t x_index, y_index;
    game_state_t game_state_copy;


    /* load next node in node task */
    taskENTER_CRITICAL();
    current_node = next_node;
    node_task = next_node;
    taskEXIT_CRITICAL();

    vTaskResume(xNodeTask_Handle);

    /* wait until current node is done or an emergencystop was occursed -> queue-set does not work!!!*/
    if(xSemaphoreTake(sSyncRoboRunNodeTask,portMAX_DELAY) || xSemaphoreTake(sSyncEmergencyStopRoboState,portMAX_DELAY))
    {
        /* suspend node task */
        vTaskSuspend(xNodeTask_Handle);

        /* check if the emergency-semaphore was the event */
        if(!getSensor_EmergencyStop())
        {
            setConfigRoboRunState2Default();
            return;
        }
    }

    /* Copy current game state, so it wont be changed during calculation */
    taskENTER_CRITICAL();
    game_state_copy = game_state;
    taskEXIT_CRITICAL();

    /*********************/
    /* check node result */
    /*********************/
    switch(current_node->param.node_state)
    {
        /* the arrival wasn't possible */
        case NODE_UNDONE:
            break;

        case NODE_FINISH_SUCCESS:
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
            break;


        /* Node not complete -> try again later */
        case NODE_FINISH_ERROR:
            current_node->param.node_tries++;
            current_node->param.node_state = NODE_UNDONE;
            break;


        /* no CAN communication */
        case GOTO_CAN_ERROR:
        default:
            /* error message and state change */
            LCD_write_string(MESSAGE_ERROR_ROW, MESSAGE_ERROR_COLUMN, MESSAGE_ERROR, TRUE);
            LCD_write_string(MESSAGE_RESTART_ROW, MESSAGE_RESTART_COLUMN, MESSAGE_RESTART, TRUE);

            system_state = runRoboErrorState;
            return;
    }


    /******************
     * time barrier
     ******************/
    if((getRemainingGameTime() < PLAY_TIME_TOTAL- PLAY_TIME && remain_nodes != 0) || (remain_nodes == 1)) //
    {
        for(node_count = 0; node_count < NODE_QUANTITY-3; node_count++)
        {
            nodes_game[node_count]->param.node_state = NODE_FINISH_SUCCESS;
        }
        for(;node_count < NODE_QUANTITY; node_count++)
        {
            nodes_game[node_count]->param.node_state = NODE_UNDONE;
        }
        remain_nodes = 1;
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
        if(nodes_game[node_count]->param.node_state == NODE_UNDONE)
        {
            /* destination weight depends on the current robo-position and the arrive-direction */
            /* SOUTH */
            if(nodes_game[node_count]->param.angle >= NODE_SOUTH_MIN_ANGLE &&
                    nodes_game[node_count]->param.angle <= NODE_SOUTH_MAX_ANGLE)
            {
                /* opposite arrive */
                if(nodes_game[node_count]->param.y < game_state_copy.y)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }
                else if(nodes_game[node_count]->param.y >= game_state_copy.y &&
                        nodes_game[node_count]->param.y - NODE_ARRIVE_FRAME < game_state_copy.y)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }
                else if(nodes_game[node_count]->param.y - NODE_ARRIVE_FRAME >= game_state_copy.y &&
                        (nodes_game[node_count]->param.x - NODE_ARRIVE_FRAME >= game_state_copy.x ||
                         nodes_game[node_count]->param.x + NODE_ARRIVE_FRAME <= game_state_copy.x))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }

            /* EAST */
            } else if(nodes_game[node_count]->param.angle >= NODE_EAST_MIN_ANGLE &&
                      nodes_game[node_count]->param.angle <= NODE_EAST_MAX_ANGLE)
            {
                /* opposite arrive */
                if(nodes_game[node_count]->param.x > game_state_copy.x)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }else if(nodes_game[node_count]->param.x <= game_state_copy.x &&
                         nodes_game[node_count]->param.x + NODE_ARRIVE_FRAME > game_state_copy.x)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }else if(nodes_game[node_count]->param.x + NODE_ARRIVE_FRAME <= game_state_copy.x &&
                        (nodes_game[node_count]->param.y - NODE_ARRIVE_FRAME >= game_state_copy.y ||
                         nodes_game[node_count]->param.y + NODE_ARRIVE_FRAME <= game_state_copy.y))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }

            /* NORTH */
            } else if(nodes_game[node_count]->param.angle >= NODE_NORTH_MIN_ANGLE &&
                      nodes_game[node_count]->param.angle <= NODE_NORTH_MAX_ANGLE)
            {
                /* opposite arrive */
                if(nodes_game[node_count]->param.y > current_node->param.y)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }
                else if(nodes_game[node_count]->param.y <= game_state_copy.y &&
                         nodes_game[node_count]->param.y + NODE_ARRIVE_FRAME > game_state_copy.y)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }
                else if(nodes_game[node_count]->param.y + NODE_ARRIVE_FRAME <= game_state_copy.y &&
                    (nodes_game[node_count]->param.x - NODE_ARRIVE_FRAME >= game_state_copy.x ||
                     nodes_game[node_count]->param.x + NODE_ARRIVE_FRAME <= game_state_copy.x))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }

            /* WEST */
            } else
            {
                /* opposite arrive */
                if(nodes_game[node_count]->param.x < game_state_copy.x)
                {
                    weight_arrive = NODE_WORST_ARRIVE;
                /* too close */
                }else if(nodes_game[node_count]->param.x >= game_state_copy.x &&
                         nodes_game[node_count]->param.x - NODE_ARRIVE_FRAME < game_state_copy.x)
                {
                    weight_arrive = NODE_BAD_ARRIVE;
                /* not bad, but not perfect as well */
                }else if(nodes_game[node_count]->param.x - NODE_ARRIVE_FRAME >= game_state_copy.x &&
                        (nodes_game[node_count]->param.y - NODE_ARRIVE_FRAME >= game_state_copy.y ||
                         nodes_game[node_count]->param.y + NODE_ARRIVE_FRAME <= game_state_copy.y))
                {
                    weight_arrive = NODE_WELL_ARRIVE;
                /* best possible arrive */
                }else
                {
                    weight_arrive = NODE_PERFECT_ARRIVE;
                }
            }


            /* cost of destination */
            weight_dest = ((nodes_game[node_count]->param.time/nodes_game[node_count]->param.points)
                   * (1/nodes_game[node_count]->param.percent)) * weight_arrive * (nodes_game[node_count]->param.node_tries);

            /* read out the enemy track-position for the node location */
            taskDISABLE_INTERRUPTS();
            y_index = (uint8_t)(nodes_game[node_count]->param.y / ENEMY_GRID_SIZE_Y);
            x_index = (uint8_t)(nodes_game[node_count]->param.y / ENEMY_GRID_SIZE_Y);
            weight_enemy = enemy_position[y_index][x_index];
            taskENABLE_INTERRUPTS();

            /* source -> destination node distance-time weight */
            weight_src_dest = (sqrtf(((game_state_copy.x - nodes_game[node_count]->param.x) * (game_state_copy.x - nodes_game[node_count]->param.x)) +
                    ((game_state_copy.y - nodes_game[node_count]->param.y) * (game_state_copy.y - nodes_game[node_count]->param.y)))/1000) / ROBO_AVERAGE_SPEED;

            /* searching next node */
            if((weight_dest * weight_dec + weight_enemy * weight_dec + weight_src_dest * weight_inc) < weight_next_node)
            {
                weight_next_node = weight_dest * weight_dec + weight_enemy * weight_dec + weight_src_dest * weight_inc;
                next_node = nodes_game[node_count];
            }
        }
    }


    /* game is finished */
    if(remain_nodes == 0)
    {
        SystemStop();
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
 * \fn          vConfederatePosition
 * \brief       save the current confederate position
 * \note        read access has to be atomic!
 *
 * \param[in]   id      CAN message ID
 * \param[in]   data    CAN message data (here, ELP)
 * \return      None
 */
static void vConfederatePosition(uint16_t id, CAN_data_t* data)
{
    if(game_state.confederate_count > 0)
    {
        game_state.confederate_x = data->elp_x;
        game_state.confederate_y = data->elp_y;
    }
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
    uint8_t x_index, y_index;

    /* start tracking only if an enemy exist and game runs */
    if(game_state.enemy_count > 0 )
    {
        if(getRemainingGameTime() < PLAY_TIME_TOTAL)
        {

            y_index = (uint8_t)(data->elp_y/ENEMY_GRID_SIZE_Y);
            x_index = (uint8_t)(data->elp_x/ENEMY_GRID_SIZE_X);

            /* check if the position within the grid and not on the frame */
            if(y_index < (PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y-1) && y_index > 0 &&
                    x_index > 0 && x_index < (PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X-1))
            {
                /* set center weight */
                enemy_position[y_index][x_index] += ENEMY_GRID_CENTER_WEIGHT;

                /* set upper frame edge */
                for(i=0; i<3; i++)
                {
                    enemy_position[y_index-1][x_index-1+i] += ENEMY_GRID_FRAME_WEIGHT;
                }

                /* set deeper frame edge */
                for(i=0; i<3; i++)
                {
                    enemy_position[y_index+1][x_index-1+i] += ENEMY_GRID_FRAME_WEIGHT;
                }

                /* set left frame edge */
                enemy_position[y_index][x_index-1] += ENEMY_GRID_FRAME_WEIGHT;

                /* set right frame edge */
                enemy_position[y_index][x_index+1] += ENEMY_GRID_FRAME_WEIGHT;
            }
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
 * \param[in]   node_param_t* param Node infos
 * \param[in]   game_state_t* game_state Game infos (navi)
 * \return      func_report (FUNC_SUCCESS, FUNC_INCOMPLETE or FUNC_ERROR)
 */
func_report_t gotoNode(node_param_t* param, volatile game_state_t* game_state)
{
	/* Variables for calculating drive distance */
	int16_t delta_x, delta_y;
	uint16_t distance;

    /* Variable for CAN RX */
    CAN_data_t CAN_buffer;
    uint8_t CAN_ok = pdFALSE;

    /* Copy current game state, so it wont be changed during calculation */
    taskENTER_CRITICAL();
    game_state_t game_state_copy = *game_state;
    taskEXIT_CRITICAL();

    /* Variable to store estimated GoTo time received from drive system (24 Bit, 1 Bit ca. 1 ms) */
    uint32_t estimated_GoTo_time = 0;

	/* If distance is to small, the drive system doesn't calculate a route and thus doesn't check if an enemy
	 * blocks the path. We have to check this here, else we start drive until the rangefinder reports an enemy. */
	delta_x = param->x - game_state_copy.x;
	delta_y = param->y - game_state_copy.y;
	distance = round(sqrt(delta_x*delta_x + delta_y*delta_y));
	/* Don't continue if distance is to small for route calculation (+5 cm overhead) and robot in front */
	if(distance <= DRIVE_ROUTE_DIST_MIN + 5 && isRobotInFront(game_state)) {
		return FUNC_INCOMPLETE;
	}

    /* Activate rangefinder */
	vTaskResume(xRangefinderTask_Handle);

	uint8_t i=0;
	do {

		i++;

        txGotoXY(param->x, param->y, param->angle, GOTO_DEFAULT_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);

		/* Receive GoTo confirmation */
		CAN_ok = xQueueReceive(qGotoConfirm, &CAN_buffer, GOTO_ACK_DELAY / portTICK_RATE_MS);

	/* Retry if no transmission confirmed received and another retry is allowed */
	} while((CAN_ok != pdTRUE) && (i <= GOTO_NACK_MAX_RETRIES));

	/* If still no GoTo confirmation was received, report error */
	if(CAN_ok != pdTRUE) {
//TODO
//	    /* Suspend rangefinder safely */
//        suspendRangefinderTask();
//
//		/* No confirmation was received from drive system! */
//		param->node_state = GOTO_CAN_ERROR;
//		return FUNC_ERROR;
	}

	estimated_GoTo_time = 0;
	do {
		/* Wait at least GOTO_STATERESP_DELAY before asking for goto time for the first time,
		 * else we may get the old time */
		if(estimated_GoTo_time < GOTO_STATERESP_DELAY) {
			vTaskDelay(GOTO_STATERESP_DELAY / portTICK_RATE_MS);
		}
		/* Ask drive system for GoTo state */
		txGotoStateRequest();
		/* Receive the GoTo state response */
		CAN_ok = xQueueReceive(qGotoStateResp, &CAN_buffer, GOTO_STATERESP_TIMEOUT / portTICK_RATE_MS);
		/* Check if time out */
		if(CAN_ok != pdTRUE) {

			/* Drive system didn't answer within specified time, use to default time */
			estimated_GoTo_time = GOTO_DEFAULT_TIME;
		}
		else {

			/* Extract time */
			estimated_GoTo_time = CAN_buffer.state_time;  /* In ms */
			/* Handle "goto not possible at the moment" message */
			if(estimated_GoTo_time == GOTO_NOT_POSSIBLE_ATM) {

				/* Suspend rangefinder safely */
				suspendRangefinderTask();

				/* Finish node with error,
				 * this way the current node will be retried if it's more attractive again */
				param->node_state = NODE_FINISH_ERROR;

				return FUNC_INCOMPLETE;
			}
		}

		/* Try to take semaphore from rangefinder task, use estimated time from drive system as timeout */
		if(estimated_GoTo_time != 0 && xSemaphoreTake(sSyncNodeTask, estimated_GoTo_time / portTICK_RATE_MS) == pdTRUE)
		{
			/* Semaphore received, this means an obstacle was detected! */
			
			/* Check if an enemy/confederate is within range in front of the robot */
			if(isRobotInRange(game_state, FALSE)) {

				/* STOPP */
				txStopDrive();

				/* Suspend rangefinder safely */
				suspendRangefinderTask();

				return FUNC_INCOMPLETE;
			}

			/* Semaphore is always only given by rangefinder task and always only taken by node task,
			 * so we don't have to give it back here */
		}

	/* Repeat while not at target destination */
	} while(estimated_GoTo_time != 0);

	/* Suspend rangefinder safely */
	suspendRangefinderTask();

	return FUNC_SUCCESS;
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
    	/* Give goto command and do node if goto was successful */
    	if(gotoNode(&node_task->param, &game_state) == FUNC_SUCCESS) {
    		/* Do node action */
    		node_task->node_function(&node_task->param, &game_state);
    	}

    	/* unblock syste task */
        xSemaphoreGive(sSyncRoboRunNodeTask);
    }
}

/**
 * @}
 */
