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
#define GOTOCONFIRM_QUEUE_LENGTH (1)


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* CAN */
xQueueHandle qGotoConfirm;
/* mammoth nodes configs */
/* node 1 */
node_t node_mammoth_1 =
{
    {
        .id = 1,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 0.62,                          /*!<node x position [m]*/
        .y = 0.49,                          /*!<node y position [m]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 180,                       /*!<node arrive direction */
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
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
	setServo_1(SERVO_POS_LAUNCHER_LOAD);
	/* Initialise fresco sensors */
	initSensor_Fresco_1();
	initSensor_Fresco_Wall();
	/* Set external interrupt for fresco wall sensor */
	initSensorEXTI_Wall();

	/* Create a queue and set CAN listener for GoTo ACK */
    qGotoConfirm = xQueueCreate(GOTOCONFIRM_QUEUE_LENGTH, sizeof(CAN_data_t));
	setQueueCANListener(qGotoConfirm, GOTO_CONFIRM);
}


/**
 * @}
 */


