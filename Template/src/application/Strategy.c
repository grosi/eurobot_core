/**
 * \file    Strategy.h
 * \author  gross10
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Task for the strategy
 *
 * \defgroup strategy Strategy
 * \brief   Strategy task
 * \ingroup app_task
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "app_config.h"
#include "CANGatekeeper.h"
#include "Strategy.h"
#include "nodes/node_config.h"


/* HW-library */
//#include "..\lib\roboBoardInterface.h" /*!< \todo remove if not used */

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static xTaskHandle xNodeTask1_Handle;
static xTaskHandle xNodeTask2_Handle;
static uint8_t state_node_1 = NODE_STOP;
static uint8_t state_node_2 = NODE_STOP;
static node_t* nodes[NODE_QUANTITY] = {&node_mammut_1};
static node_t* node_task_1;
static node_t* node_task_2;
volatile static float enemey_position[((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y))][((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))] = {{0.0}};
static robo_state_t robo_state = {0,0,0};


/* Private function prototypes -----------------------------------------------*/
static void vStrategySchedulerTask(void*);
static void vNodeTask1(void*);
static void vNodeTask2(void*);
static void vTrackEnemy(uint16_t, CAN_data_t*);
static void vMyPosition(uint16_t, CAN_data_t*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initStrategyTask
 * \brief       Initialisation of the strategy task
 *
 * \param[in]   None
 * \return      None
 */
void initStrategyTask(void){

    /* create the task */
    xTaskCreate( vStrategySchedulerTask, ( signed char * ) STRATEGY_SCHEDULER_TASK_NAME, STRATEGY_STACK_SIZE, NULL, STRATEGY_SCHEDULER_TASK_PRIORITY, NULL );
    xTaskCreate( vNodeTask1, ( signed char * ) STRATEGY_NODE_TASK_1_NAME, STRATEGY_STACK_SIZE, NULL, STRATEGY_NODE_TASK_PRIORITY, xNodeTask1_Handle );
    xTaskCreate( vNodeTask2, ( signed char * ) STRATEGY_NODE_TASK_2_NAME, STRATEGY_STACK_SIZE, NULL, STRATEGY_NODE_TASK_PRIORITY, xNodeTask2_Handle );

    /* suspend the node task until they will used */
    vTaskSuspend(xNodeTask1_Handle);
    vTaskSuspend(xNodeTask2_Handle);

    /* set CAN listeners */
    setFunctionCANListener(vTrackEnemy,ENEMEY_1_POSITION_RESPONSE);
    setFunctionCANListener(vTrackEnemy,ENEMEY_2_POSITION_RESPONSE);
    setFunctionCANListener(vMyPosition,KALMAN_POSITION_RESPONSE);

}


/**
 * \fn          vStrategySchedulerTask
 * \brief       Task to handle the strategy
 *
 * \param[in]   None
 * \return      None
 */
static void vStrategySchedulerTask(void* pvParameters )
{
    /* local variables */
    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();


    /* endless */
    for(;;)
    {







        /* wait for 1 second*/
        vTaskDelayUntil( &xLastFlashTime, 1000 / portTICK_RATE_MS);
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
        node_task_1->node_function(&node_task_1->param,&robo_state,&state_node_1);
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
        node_task_2->node_function(&node_task_2->param,&robo_state,&state_node_2);
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
 * @}
 */
