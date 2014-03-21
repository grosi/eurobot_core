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
#include "../app_config.h"
#include "../CANGatekeeper.h"
#include "../Strategy.h"
#include "../nodes/node_config.h"
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
static node_t* nodes[NODE_QUANTITY] = {&node_mammut_1};
volatile static float enemey_position[((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y))][((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))] = {{0.0}};
volatile static robo_state_t robo_state = {0,0,0};


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


void runRoboRunState()
{

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
