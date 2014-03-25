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
#define BINARY_SEMAPHORE_LENGTH	1
#define SEMAPHORE_SET_SIZE 		(BINARY_SEMAPHORE_LENGTH * 2)

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* RTOS */
static xTaskHandle xNodeTask1_Handle = NULL;
static xTaskHandle xNodeTask2_Handle = NULL;
xSemaphoreHandle sSyncRoboRunNodeTask1;
xSemaphoreHandle sSyncRoboRunNodeTask2;
xSemaphoreHandle sSyncNodeTaskxDrive;
xSemaphoreHandle sSyncNodeTaskxServo;
static xQueueSetHandle sSyncRoboRunNodeTaskxSet;
static node_t* node_task_1 = NULL;
static node_t* node_task_2 = NULL;

/* game and strategy */
static const node_t* nodes_red[NODE_QUANTITY] = {&node_mammut_1};
static const node_t* nodes_yellow[NODE_QUANTITY] = {&node_mammut_1};
static node_t* node_game;
volatile static float enemey_position[((int)(PLAYGROUND_HEIGH/ENEMY_GRID_SIZE_Y))][((int)(PLAYGROUND_WIDTH/ENEMY_GRID_SIZE_X))] = {{0.0}};
volatile static robo_state_t robo_state = {0,0,0};
node_t* next_node;
uint8_t start_node;



/* Private function prototypes -----------------------------------------------*/
static void vNodeTask1(void*);
static void vNodeTask2(void*);
static void vTrackEnemy(uint16_t, CAN_data_t*);
static void vMyPosition(uint16_t, CAN_data_t*);

/* Private functions ---------------------------------------------------------*/


/**
 *
 */
void initRoboRunState()
{
    /* create the task */
    xTaskCreate( vNodeTask1, ( signed char * ) SYSTEM_NODE_TASK_1_NAME, SYSTEM_NODE_STACK_SIZE, NULL, SYSTEM_NODE_TASK_PRIORITY, &xNodeTask1_Handle );
    xTaskCreate( vNodeTask2, ( signed char * ) SYSTEM_NODE_TASK_2_NAME, SYSTEM_NODE_STACK_SIZE, NULL, SYSTEM_NODE_TASK_PRIORITY, &xNodeTask2_Handle );

    /* suspend the node task until they will used */
    vTaskSuspend(xNodeTask1_Handle);
    vTaskSuspend(xNodeTask2_Handle);

    /* create sync-semaphore */
    vSemaphoreCreateBinary(sSyncRoboRunNodeTask1); /* for RoboRun <-> node-task sync */
    vSemaphoreCreateBinary(sSyncRoboRunNodeTask2); /* for RoboRun <-> node-task sync */
    vSemaphoreCreateBinary(sSyncNodeTaskxDrive); /* for RoboRun <-> node-task sync */
    vSemaphoreCreateBinary(sSyncNodeTaskxServo); /* for RoboRun <-> node-task sync */

    /* create a sync semaphore set */
    sSyncRoboRunNodeTaskxSet = xQueueCreateSet(SEMAPHORE_SET_SIZE);
    xQueueAddToSet(sSyncRoboRunNodeTask1,sSyncRoboRunNodeTaskxSet);
    xQueueAddToSet(sSyncRoboRunNodeTask2,sSyncRoboRunNodeTaskxSet);

    /* set CAN listeners */
    setFunctionCANListener(vTrackEnemy,ENEMEY_1_POSITION_RESPONSE);
    setFunctionCANListener(vTrackEnemy,ENEMEY_2_POSITION_RESPONSE);
    setFunctionCANListener(vMyPosition,KALMAN_POSITION_RESPONSE);
}


/**
 *
 */
uint8_t setConfigRoboRunState(uint8_t start_node_id, uint8_t teamcolor)
{
	/* locale variables */
	uint8_t node_count;
	uint8_t success = 0;

	/* load correct node-set */
	if(teamcolor == GIP_TEAMCOLOR_YELLOW)
	{
		node_game = nodes_yellow;
	}
	else
	{
		node_game = nodes_red;
	}

	/* search start-node address */
	for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
	{
		if((node_game+node_count)->param.id == start_node_id)
		{
			next_node = (node_game+node_count);
			success = 1;
			break;
		}
	}

	return success;
}


/**
 *
 */
void runRoboRunState(portTickType* tick)
{
    float weight_dec, weight_inc, weight_dest, weight_enemy, weight_src_dest;
    uint8_t remain_time;
    uint8_t node_count;
    node_t* current_node;
    xQueueSetMemberHandle sSyncRoboRunNodeTaskxSetHandle;



    /* node task 1 */
    if(xSemaphoreTake(sSyncRoboRunNodeTask1,0) == pdTRUE)
    {
    	/* load next node */
		node_task_1 = next_node;
		current_node = next_node;

		vTaskResume(xNodeTask1_Handle);
    }
    else
    {
		/* node task 2 */
		if(xSemaphoreTake(sSyncRoboRunNodeTask2,0) == pdTRUE)
		{
			/* load next node */
			node_task_2 = next_node;
			current_node = next_node;

			vTaskResume(xNodeTask2_Handle);
		}
    }

    /* wait 1s or a given semaphore */
    sSyncRoboRunNodeTaskxSetHandle = xQueueSelectFromSet(sSyncRoboRunNodeTaskxSet, 1000 / portTICK_RATE_MS);

    /* check the node result of node-task 1 */
	if(sSyncRoboRunNodeTaskxSetHandle == sSyncRoboRunNodeTask1 && node_task_1 != NULL)
	{

	}

	/* check the node result of node-task 2 */
	if(sSyncRoboRunNodeTaskxSetHandle == sSyncRoboRunNodeTask2 && node_task_2 != NULL)
	{

	}


    /******************/
    /* calc next node */
    /******************/
    remain_time = getRemainingTime();
    weight_dec = remain_time / PLAY_TIME;
    weight_inc = (PLAY_TIME - remain_time) / PLAY_TIME;

    for(node_count = 0; node_count < NODE_QUANTITY; node_count++)
    {
        /* destination weight depends on the current robo-position and the arrive-direction */
        switch((node_game+node_count)->param.arrive)
        {
            case NORTH:
                if(robo_state.y <= (node_game+node_count)->param.y)
                {
                    weight_dec =((node_game+node_count)->param.points/(node_game+node_count)->param.time)
                            * (1/(node_game+node_count)->param.percent);
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
        taskENTER_CRITICAL();
        weight_enemy = enemey_position[(int)((node_game+node_count)->param.y / (ENEMY_GRID_SIZE_Y*1000))]
                                       [(int)((node_game+node_count)->param.y / (ENEMY_GRID_SIZE_Y*1000))];
        taskEXIT_CRITICAL();

        /* source -> destination node distance-time weight */
        weight_src_dest = sqrtf((fabsf(robo_state.x - (node_game+node_count)->param.x) * fabsf(robo_state.x - (node_game+node_count)->param.x)) +
                (fabsf(robo_state.y - (node_game+node_count)->param.y) * fabsf(robo_state.y - (node_game+node_count)->param.y))) / ROBO_AVERAGE_SPEED;
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

        taskENTER_CRITICAL();
        xSemaphoreGive(sSyncRoboRunNodeTask1);
        vTaskSuspend(NULL);
        taskEXIT_CRITICAL();
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

        taskENTER_CRITICAL();
		xSemaphoreGive(sSyncRoboRunNodeTask2);
		vTaskSuspend(NULL);
		taskEXIT_CRITICAL();
    }
}
/**
 * @}
 */
