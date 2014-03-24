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
/* application */
#include <math.h>
#include "../AppConfig.h"
#include "../CANGatekeeper.h"
#include "../Timer.h"
#include "../System.h"
#include "../nodes/NodeConfig.h"
#include "RoboRun.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* RTOS */
static xTaskHandle xNodeTask1_Handle = NULL;
static xTaskHandle xNodeTask2_Handle = NULL;
static node_t* node_task_1 = NULL;
static node_t* node_task_2 = NULL;

/* game and strategy */
static const node_t* nodes[NODE_QUANTITY] = {&node_mammut_1};
volatile static float enemey_position[((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y))][((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))] = {{0.0}};
volatile static robo_state_t robo_state = {0,0,0};
node_t* next_node;


/* Private function prototypes -----------------------------------------------*/
static void vNodeTask1(void*);
static void vNodeTask2(void*);
static void vTrackEnemy(uint16_t, CAN_data_t*);
static void vMyPosition(uint16_t, CAN_data_t*);

/* Private functions ---------------------------------------------------------*/



void initRoboRunState()
{
    /* create the task */
    xTaskCreate( vNodeTask1, ( signed char * ) SYSTEM_NODE_TASK_1_NAME, SYSTEM_NODE_STACK_SIZE, NULL, SYSTEM_NODE_TASK_PRIORITY, &xNodeTask1_Handle );
    xTaskCreate( vNodeTask2, ( signed char * ) SYSTEM_NODE_TASK_2_NAME, SYSTEM_NODE_STACK_SIZE, NULL, SYSTEM_NODE_TASK_PRIORITY, &xNodeTask2_Handle );

    /* suspend the node task until they will used */
    vTaskSuspend(xNodeTask1_Handle);
    vTaskSuspend(xNodeTask2_Handle);

    /* set CAN listeners */
    setFunctionCANListener(vTrackEnemy,ENEMEY_1_POSITION_RESPONSE);
    setFunctionCANListener(vTrackEnemy,ENEMEY_2_POSITION_RESPONSE);
    setFunctionCANListener(vMyPosition,KALMAN_POSITION_RESPONSE);
}


void runRoboRunState(portTickType* tick)
{
    float weight_dec, weight_inc, weright_dest, weight_enemy, weight_src_dest;
    uint8_t remain_time;
    uint8_t node_count;


    /* node task 1 */
    if(node_task_1 == NULL)
    {
        /* load next node */
        node_task_1 = next_node;
        if(node_task_2 == NULL)
        {
            node_task_1->param.node_state = NODE_BUSY;
            vTaskResume(xNodeTask1_Handle);
        }
        else
        {
            if(node_task_2->param.node_state != NODE_BUSY)
            {
                node_task_1->param.node_state = NODE_BUSY;
                vTaskResume(xNodeTask1_Handle);
            }
        }
    }
    else
    {
        if(node_task_1->param.node_state == NODE_FINISH_SUCCESS || node_task_1->param.node_state == NODE_FINISH_ERROR)
        {
            /* load next node */
            node_task_1 = next_node;

            if(node_task_2 == NULL)
            {
                node_task_1->param.node_state = NODE_BUSY;
                vTaskResume(xNodeTask1_Handle);
            }
            else
            {
                if(node_task_2->param.node_state != NODE_BUSY)
                {
                    node_task_1->param.node_state = NODE_BUSY;
                    vTaskResume(xNodeTask1_Handle);
                }
            }

        }
    }

    /* node task 2 */
    if(node_task_2 == NULL)
    {
        /* load next node */
        node_task_2 = next_node;

        if(node_task_1 == NULL)
        {
            node_task_2->param.node_state = NODE_BUSY;
            vTaskResume(xNodeTask2_Handle);
        }
        else
        {
            if(node_task_1->param.node_state != NODE_BUSY)
            {
                node_task_2->param.node_state = NODE_BUSY;
                vTaskResume(xNodeTask2_Handle);
            }
        }
    }
    else
    {
        if(node_task_2->param.node_state == NODE_FINISH_SUCCESS || node_task_2->param.node_state == NODE_FINISH_ERROR)
        {
            /* load next node */
            node_task_2 = next_node;

            if(node_task_1 == NULL)
            {
                node_task_2->param.node_state = NODE_BUSY;
                vTaskResume(xNodeTask2_Handle);
            }
            else
            {
                if(node_task_1->param.node_state != NODE_BUSY)
                {
                    node_task_2->param.node_state = NODE_BUSY;
                    vTaskResume(xNodeTask2_Handle);
                }
            }

        }
    }

    /* wait for 1s */
    vTaskDelayUntil(tick, ROBORUN_TIMEOUT / portTICK_RATE_MS);


    /******************/
    /* calc next node */
    /******************/
    remain_time = getRemainingTime();
    weight_dec = remain_time / PLAY_TIME;
    weight_inc = (PLAY_TIME - remain_time) / PLAY_TIME;

    for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
    {
        /* destination weight depends on the current robo-position and the arrive-direction */
        switch(nodes[node_count]->param.arrive)
        {
            case NORTH:
                if(robo_state.y <= nodes[node_count]->param.y)
                {
                    weight_dec =(nodes[node_count]->param.points/nodes[node_count]->param.time)
                            * (1/nodes[node_count]->param.percent);
                }
                break;
            case EAST:
                break;
            case SOUTH:
                break;
            case WEST:
                break;
        }

        /* read out the enemy track-position for the node location */
        /* todo evtl. interrupts ausschalten */
        weight_enemy = enemey_position[(int)(nodes[node_count]->param.y / (ENEMY_GRID_SIZE_Y*1000))]
                                       [(int)(nodes[node_count]->param.y / (ENEMY_GRID_SIZE_Y*1000))];

        /* source -> destination node distance-time weight */
        weight_src_dest = sqrtf((fabsf(robo_state.x - nodes[node_count]->param.x) * fabsf(robo_state.x - nodes[node_count]->param.x)) +
                (fabsf(robo_state.y - nodes[node_count]->param.y) * fabsf(robo_state.y - nodes[node_count]->param.y))) / ROBO_AVERAGE_SPEED;
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
    robo_state.x = data->elp_x;
    robo_state.y = data->elp_y;
    robo_state.angle = data->elp_angle;
}


/**
 * \fn          vTrackEnemy
 * \brief       Track the enemys-position in an simple grid array
 *
 * \param[in]   id      CAN message ID
 * \param[in]   data    CAN message data (here, ELP)
 * \return      None
 */
static void vTrackEnemy(uint16_t id, CAN_data_t* data)
{
    /* locale variable */
    uint8_t i;

    /* data check  todo mit Antriebklaeren*/

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
}


/**
 * \fn          vNodeTask1
 * \brief       Task to handle the strategy
 *
 * \param[in]   None
 * \return      None
 */
static void vNodeTask1(void* pvParameters )
{
    /* endless */
    for(;;)
    {
        node_task_1->node_function(&node_task_1->param,&robo_state);
    }
}


/**
 * \fn          vNodeTask2
 * \brief       Task to handle the strategy
 *
 * \param[in]   None
 * \return      None
 */
static void vNodeTask2(void* pvParameters )
{
    /* endless */
    for(;;)
    {
        node_task_2->node_function(&node_task_2->param,&robo_state);
    }
}
/**
 * @}
 */
