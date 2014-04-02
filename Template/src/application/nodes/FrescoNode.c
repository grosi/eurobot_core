/**
 * \file    FrescoNode.c
 * \author  gross10
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
#include "FrescoNode.h"
#include "NodeConfig.h"
/* lib */
#include "lib/servo.h"
#include "lib/sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Servo */
#define SERVO_POS_FRESCO_IN     (0)   /* Servo position: fresco panel all the way in */ //TODO
#define SERVO_POS_FRESCO_OUT    (100) /* Servo position: fresco panel all the way in */ //TODO
#define SERVO_FRESCO_STEP       (2)   /* Size of single step for the fresco servo */ //TODO
#define SERVO_FRESCO_STEP_DELAY (10)  /* Delay in ms to wait between steps */ //TODO
#define SERVO_MOVING_DELAY      (100) /* Delay in ms to wait while the servo moves the whole way */ //TODO


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
volatile uint8_t frescoOnWall = 0;    /* Set by EXTI (TODO), value of sensor to check if panel touched wall */


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doFrescoNode
 * \brief       Tries to complete the fresco node. Reports status.
 *
 * \param       param      node parameters
 * \param       game_state current robo state
 * \return      None
 */
void doFrescoNode(node_param_t* param, volatile game_state_t* game_state) {

	/* Variable to count number of performed attempts to complete fresco action */
	uint8_t n_fresco_attempts = 0;
	/* Variable to count number of frescos in robot */
	uint8_t n_frescos_present = 0;
	/* Variable to set the servo position step by step */
	uint16_t servo_pos = SERVO_POS_FRESCO_IN;

	do {

		/* Move separation out, step by step, but stop if fresco panel is on the wall */
		while(servo_pos < (SERVO_POS_FRESCO_OUT-SERVO_FRESCO_STEP) && !frescoOnWall) {	//TODO: "<" or ">" and "-" or "+"?

			/* Increment servo position by step size */
			servo_pos += SERVO_FRESCO_STEP;

			/* Check if it's the last step */
			if(servo_pos > SERVO_POS_FRESCO_OUT) {

				/* Set the final servo position without over rotating */
				setServo_1(SERVO_POS_FRESCO_OUT);
			}
			else {

				/* Set the new servo position */
				setServo_1(servo_pos);
			}

			/* Wait some time while servo moves */
			vTaskDelay(SERVO_FRESCO_STEP_DELAY / portTICK_RATE_MS);
		}

		/* Move separation in */
		setServo_1(SERVO_POS_FRESCO_IN);

		/* Wait some time while servo moves */
		vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

		/* Increment number of performed attempts */
		n_fresco_attempts++;

		/* Get number of frescos still in robot */
		n_frescos_present = getSensor_Fresco_1() + getSensor_Fresco_2();

		/* Retry if there are still frescos in the robot and another retry is allowed (max. not reached). */
	} while(n_frescos_present && (n_fresco_attempts <= FRESCO_MAX_RETRIES));

	/* Report status */
	switch(n_frescos_present) {

	case 0:
		param->node_state = NODE_FINISH_SUCCESS;
		break;

	case 1:
		param->node_state = NODE_FINISH_ERROR;
		break;

	default: /* 2 */
		param->node_state = NODE_UNDONE;
		break;
	}
}


/**
 * @}
 */


