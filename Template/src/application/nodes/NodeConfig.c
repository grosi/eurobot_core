/**
 * \file    NodeConfig.c
 * \author  gross10, kasen1
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   config file for the game nodes
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "../CANGatekeeper.h"
#include "NodeConfig.h"
#include "FireNode.h"
#include "FireWallInversNode.h"
#include "FireWallNormalNode.h"
#include "FirePoolNode.h"
#include "NetNode.h"


/* lib */
#include "../../lib/servo.h"
#include "../../lib/sensor.h"
#include "../../lib/exti_sensor.h"
#include "../../lib/air.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define GOTOCONFIRM_QUEUE_LENGTH   1
#define GOTOSTATERESP_QUEUE_LENGTH 1


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* CAN */
xQueueHandle qGotoConfirm;
xQueueHandle qGotoStateResp;


/* node fire 1 red*/
node_t node_fire_1_red =
{
    {
        .id = 1,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 2 red*/
node_t node_fire_2_red =
{
    {
        .id = 2,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 2600,                          /*!<node x position [mm]*/
        .y = 750,                           /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 90,                        /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 3 red*/
node_t node_fire_3_red =
{
    {
        .id = 3,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 2250,                          /*!<node x position [mm]*/
        .y = 400,                           /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 180,                       /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node wall fire pool red*/
node_t node_fire_pool_red =
{
    {
        .id = 4,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFirePoolNode
};

/* node wall fire normal 1 red*/
node_t node_fire_wall_normal_1_red =
{
    {
        .id = 5,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire normal 2 red*/
node_t node_fire_wall_normal_2_red =
{
    {
        .id = 6,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire invers red*/
node_t node_fire_wall_invers_red =
{
    {
        .id = 7,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallInversNode
};


/* node net 1 red*/
node_t node_net_1_red =
{
    {
        .id = 8,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doNetNode
};

/* node net 2 red*/
node_t node_net_2_red =
{
    {
        .id = 9,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doNetNode
};

/* node net 3 red*/
node_t node_net_3_red =
{
    {
        .id = 10,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doNetNode
};

















/* yellow */
/* node fire 1 yellow*/
node_t node_fire_1_yellow =
{
    {
        .id = 1,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1050,                           /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 180,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 2 yellow*/
node_t node_fire_2_yellow =
{
    {
        .id = 2,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x =  400,                          /*!<node x position [mm]*/
        .y = 750,                           /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 90,                        /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 3 yellow*/
node_t node_fire_3_yellow =
{
    {
        .id = 3,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 750,                           /*!<node x position [mm]*/
        .y = 400,                           /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};


/* node wall fire pool red*/
node_t node_fire_pool_yellow =
{
    {
        .id = 4,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFirePoolNode
};

/* node wall fire normal 1 yellow*/
node_t node_fire_wall_normal_1_yellow =
{
    {
        .id = 5,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire normal 2 yellow*/
node_t node_fire_wall_normal_2_yellow =
{
    {
        .id = 6,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire invers yellow*/
node_t node_fire_wall_invers_yellow =
{
    {
        .id = 7,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallInversNode
};


/* node net 1 yellow*/
node_t node_net_1_yellow =
{
    {
        .id = 8,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doNetNode
};

/* node net 2 yellow*/
node_t node_net_2_yellow =
{
    {
        .id = 9,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doNetNode
};

/* node net 3 yellow*/
node_t node_net_3_yellow =
{
    {
        .id = 10,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doNetNode
};



/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initNodeResources
 * \brief       Function to initialise all the resources needed by the node task
 *
 * \TODO init sensors (incl. exti)
 *
 * \param[in]   None
 * \return      None
 */
void initNodeResources()
{
	/* Initialise fresco/separation servo and set start position */
	initServo_1();
	setServo_1(SERVO_POS_AIR_UP);
	/* Initialise launcher servo and set start position */
	initServo_2();
	setServo_2(SERVO_POS_NET_LOAD);

	/* Initialise fresco sensors */
	initSensor_Fire_Pool();
	initSensor_Air();

	/* Initialise air system */
	//initAir();

	/* Create a queue and set CAN listener for GoTo ACK */
    qGotoConfirm = xQueueCreate(GOTOCONFIRM_QUEUE_LENGTH, sizeof(CAN_data_t));
    if(qGotoConfirm != 0) {
    	setQueueCANListener(qGotoConfirm, GOTO_CONFIRM);
    }
//TODO:
//    else {
//    }

    /* Create a queue and set CAN listener for GoTo state response */
	qGotoStateResp = xQueueCreate(GOTOSTATERESP_QUEUE_LENGTH, sizeof(CAN_data_t));
	if(qGotoStateResp != 0) {
		setQueueCANListener(qGotoStateResp, GOTO_STATE_RESPONSE);
	}
//TODO:
//    else {
//    }
}


/**
 * @}
 */


