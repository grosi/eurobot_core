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
#include "FrescoNode.h"
#include "MammothNode.h"

/* lib */
#include "../../lib/servo.h"
#include "../../lib/sensor.h"
#include "../../lib/exti_sensor.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define GOTOCONFIRM_QUEUE_LENGTH   1
#define GOTOSTATERESP_QUEUE_LENGTH 1


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* CAN */
xQueueHandle qGotoConfirm;
xQueueHandle qGotoStateResp;
/* mammoth nodes configs */
/* node mammoth 1 */
node_t node_mammoth_1 =
{
    {
        .id = 1,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 660,                          /*!<node x position [mm]*/
        .y = 1500,                          /*!<node y position [mm]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
};

/* node mammoth 2 */
node_t node_mammoth_2 =
{
    {
        .id = 2,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 760,                          /*!<node x position [mm]*/
        .y = 1500,                          /*!<node y position [mm]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
};

/* node mammoth 3 */
node_t node_mammoth_3 =
{
    {
        .id = 3,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 860,                          /*!<node x position [mm]*/
        .y = 1500,                           /*!<node y position [mm]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
};

/* node mammoth 4 */
node_t node_mammoth_4 =
{
    {
        .id = 6,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 2140,                          /*!<node x position [mm]*/
        .y = 1510,                           /*!<node y position [mm]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
};

/* node mammoth 5 */
node_t node_mammoth_5 =
{
    {
        .id = 7,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 2240,                          /*!<node x position [mm]*/
        .y = 1500,                           /*!<node y position [mm]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
};

/* node mammoth 6 */
node_t node_mammoth_6 =
{
    {
        .id = 8,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 2340,                          /*!<node x position [mm]*/
        .y = 1500,                           /*!<node y position [mm]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
};

/* node fresco 1 */
node_t node_fresco_1 =
{
    {
        .id = 4,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.28,                    /*!<percent of the total points [%]*/
        .time = 10,                         /*!<estimated node time [s]*/
        .x = 1350,                          /*!<node x position [mm]*/
        .y = 1760,                           /*!<node y position [mm]*/
        .pool_id = NODE_FRESCO_POOL_ID,     /*!<node pool id */
        .angle = 90,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFrescoNode
};

/* node fresco 2 */
node_t node_fresco_2 =
{
    {
        .id = 5,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.28,                    /*!<percent of the total points [%]*/
        .time = 10,                         /*!<estimated node time [s]*/
        .x = 1650,                          /*!<node x position [mm]*/
        .y = 1760,                           /*!<node y position [mm]*/
        .pool_id = NODE_FRESCO_POOL_ID,     /*!<node pool id */
        .angle = 90,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFrescoNode
};

/* node fire 1 red*/
node_t node_fire_1_red =
{
    {
        .id = 9,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 2100,                          /*!<node x position [mm]*/
        .y = 1400,                           /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 2 red*/
node_t node_fire_2_red =
{
    {
        .id = 10,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 2600,                          /*!<node x position [mm]*/
        .y = 900,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 90,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 3 red*/
node_t node_fire_3_red =
{
    {
        .id = 11,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 2100,                          /*!<node x position [mm]*/
        .y = 400,                           /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 180,                         /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 1 yellow*/
node_t node_fire_1_yellow =
{
    {
        .id = 9,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 900,                           /*!<node x position [mm]*/
        .y = 1400,                           /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 0,                         /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 2 yellow*/
node_t node_fire_2_yellow =
{
    {
        .id = 10,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x =  400,                          /*!<node x position [mm]*/
        .y = 900,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 90,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
};

/* node fire 3 yellow*/
node_t node_fire_3_yellow =
{
    {
        .id = 11,                           /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 900,                           /*!<node x position [mm]*/
        .y = 400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 180,                       /*!<node arrive direction */
        .node_tries = 1,                     /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireNode
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
	setServo_1(SERVO_POS_FRESCO_IN);
	/* Initialise launcher servo and set start position */
	initServo_2();
	setServo_2(SERVO_POS_LAUNCHER_LOAD);
	setServo_2(SERVO_POS_LAUNCHER_LOAD-10);
	/* Initialise fresco sensors */
	initSensor_Fresco_1();
	initSensor_Fresco_2();
	initSensor_Fresco_Wall();

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


