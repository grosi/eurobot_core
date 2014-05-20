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
#include "NodeMisc.h"
//#include "../system/RoboRun.h"
#include "NodeConfig.h"
#include "FrescoNode.h"
/* lib */
#include "lib/servo.h"
#include "lib/sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Servo */
#define FRESCO_SERVO_STEP       5   /* Size of single step for the fresco servo */
#define FRESCO_SERVO_STEP_DELAY 10  /* Delay in ms to wait between steps */
/* Drive */
#define FRESCO_DRIVECHECK_DELAY 10  /* Delay in ms to wait between checking if robot hit wall */

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
void doFrescoNode(node_param_t* param, volatile game_state_t* game_state) {

	/* Drive closer to the wall, so the final goto distance is < 150 mm and
	 * thus no route is calculated (which allows us to drive in the "forbidden" zone near the wall) */
	checkDrive(param->x, param->y + FRESCO_APPROACH_DISTANCE, param->angle, FRESCO_APPROACH_SPEED, GOTO_DRIVE_FORWARD, game_state);

	/* Move panel all the way out */
	setServo_1(SERVO_POS_FRESCO_OUT);

//	/* Wait some time while driving and servo moves */
//	vTaskDelay(FRESCO_APPROACH_DELAY / portTICK_RATE_MS);

	/* Drive all the way to the wall */
	checkDrive(param->x, FRESCO_WALL_POSITION, param->angle, FRESCO_WALL_SPEED, GOTO_DRIVE_FORWARD, game_state);

	volatile uint16_t approach_counter = 0;
	while(!getSensor_Fresco_Wall() && approach_counter < FRESCO_WALL_TIME) {

		/* Wait some time before next check */
		vTaskDelay(FRESCO_DRIVECHECK_DELAY / portTICK_RATE_MS);
		approach_counter += FRESCO_DRIVECHECK_DELAY;
	}

	/* Stop driving further */
	txStopDrive();

	/* Check if we are on the wall, else the space is blocked */
	if(getSensor_Fresco_Wall()) {

		/* Move fresco panel in slowly (step by step) */
		volatile uint16_t servo_pos = SERVO_POS_FRESCO_OUT;  /* Current position */
		while(servo_pos < (SERVO_POS_FRESCO_IN-FRESCO_SERVO_STEP)) {

			/* Increment servo position by step size */
			servo_pos += FRESCO_SERVO_STEP;

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
			vTaskDelay(FRESCO_SERVO_STEP_DELAY / portTICK_RATE_MS);
		}
	}
	else {  /* Not on wall */

		/* Move panel all the way in, we don't need to do that slowly here because we're not pasting frescos */
		setServo_1(SERVO_POS_FRESCO_IN);

	    /* Wait some time while servo moves */
	    vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);
	}

    /* drive backwards (distance defined by drive system, currently 50 mm) */
    checkDrive(IGNORED_VALUE, IGNORED_VALUE, IGNORED_VALUE, FRESCO_WALL_SPEED, GOTO_DRIVE_BACKWARD, game_state);
    /* wait while driving backwards */
    vTaskDelay(2000 / portTICK_RATE_MS);  // 1s is too short, TODO: evaluate why even necessary

	/* Get number of frescos still in robot */
	uint8_t n_frescos_present = getSensor_Fresco_1() + getSensor_Fresco_2();

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


