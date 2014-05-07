/**
 * \file    FrescoNode.c
 * \author  kasen1, gross10
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fresco node state machine
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
#include "FrescoNode.h"
/* lib */
#include "lib/servo.h"
#include "lib/sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Servo */
#define SERVO_FRESCO_STEP       (5)   /* Size of single step for the fresco servo */
#define SERVO_FRESCO_STEP_DELAY (10)  /* Delay in ms to wait between steps */

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doFrescoNode
 * \brief       Tries to complete the fresco node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doFrescoNode(node_param_t* param) {

	/* Variable to count number of performed attempts to complete fresco action */
	uint8_t n_fresco_attempts = 0;
	/* Variable to count number of frescos in robot */
	uint8_t n_frescos_present = 0;
	/* Variable to set the servo position step by step */
	volatile uint16_t servo_pos;

	/* Drive closer to the wall */
	txGotoXY(param->x, param->y + FRESCO_APPROACH_DISTANCE, param->angle, FRESCO_APPROACH_SPEED, 0x00);
	vTaskDelay(FRESCO_APPROACH_TIME / portTICK_RATE_MS);

	do {

		/* Move fresco panel out, step by step, but stop if on the wall */
		servo_pos = SERVO_POS_FRESCO_IN;  /* Current position */
		while(servo_pos > (SERVO_POS_FRESCO_OUT+SERVO_FRESCO_STEP) && !getSensor_Fresco_Wall()) {

			/* Decrement servo position by step size */
			servo_pos -= SERVO_FRESCO_STEP;

			/* Check if it's the last step */
			if(servo_pos < SERVO_POS_FRESCO_OUT) {

				/* Set the final servo position without over-rotating */
				setServo_1(SERVO_POS_FRESCO_OUT);
				servo_pos = SERVO_POS_FRESCO_OUT;
			}
			else {

				/* Set the new servo position */
				setServo_1(servo_pos);
			}
			/* Wait some time while servo moves */
			vTaskDelay(SERVO_FRESCO_STEP_DELAY / portTICK_RATE_MS);
		}

		/* Move fresco panel in, step by step */
		while(servo_pos < (SERVO_POS_FRESCO_IN-SERVO_FRESCO_STEP)) {

			/* Increment servo position by step size */
			servo_pos += SERVO_FRESCO_STEP;

			/* Check if it's the last step */
			if(servo_pos > SERVO_POS_FRESCO_IN) {

				/* Set the final servo position without over-rotating */
				setServo_1(SERVO_POS_FRESCO_IN);
			}
			else {

				/* Set the new servo position */
				setServo_1(servo_pos);
			}
			/* Wait some time while servo moves */
			vTaskDelay(SERVO_FRESCO_STEP_DELAY / portTICK_RATE_MS);
		}

//			/* Move fresco panel in, not step by step */
//			setServo_1(SERVO_POS_FRESCO_IN);
//			/* Wait some time while servo moves */
//			vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

		/* Increment number of performed attempts */
		n_fresco_attempts++;

		/* Get number of frescos still in robot */
		n_frescos_present = getSensor_Fresco_1() + getSensor_Fresco_2();

	/* Retry if there are still both frescos in the robot and another retry is allowed (max. not reached). */
	} while((n_frescos_present >= 2) && (n_fresco_attempts < FRESCO_MAX_RETRIES));

	/* Report status */
	if(n_frescos_present > 0) {
		param->node_state = NODE_FINISH_ERROR;
	}
	else {
		param->node_state = NODE_FINISH_SUCCESS;
	}
}


/**
 * @}
 */


