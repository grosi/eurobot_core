/**
 * \file    MammothNode.c
 * \author  gross10
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   mammoth node statemaschine
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "NodeConfig.h"
#include "MammothNode.h"
/* lib */
#include "lib/servo.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Servo */
#define SERVO_POS_LAUNCHER_LOAD     (0)   /* Servo position: Launcher all the way front */ //TODO
#define SERVO_POS_LAUNCHER_LAUNCH   (100) /* Servo position: Launcher all the way back */ //TODO


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doMammothNode
 * \brief       Tries to complete the mammoth node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doMammothNode(node_param_t* param) {

	/* Move the launcher servo all the way forward, in case it isn't already */
	setServo_2(SERVO_POS_LAUNCHER_LOAD);

	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Launch three times */
	int i;
	for(i=0; i<3; i++) {

		/* Move the launcher servo all the way back back to launch the two loaded balls */
		setServo_2(SERVO_POS_LAUNCHER_LAUNCH);
		/* Wait some time while servo moves */
		vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

		/* Move the launcher servo all the way forward */
		setServo_2(SERVO_POS_LAUNCHER_LOAD);
		/* Move the separation all the way out */
		setServo_2(SERVO_POS_FRESCO_OUT);  //TODO catch the case in which the area for the panel is blocked by another robot
		/* Wait some time while servos move */
		vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

		/* Move the separation all the way in */
		setServo_2(SERVO_POS_FRESCO_IN);
		/* Wait some time while servos move */
		vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);
	}

	/* Report status */
	param->node_state = NODE_FINISH_SUCCESS;
}


/**
 * @}
 */


