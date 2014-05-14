/**
 * \file    MammothNode.c
 * \author  kasen1, gross10
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
#include "../Rangefinder.h"
/* lib */
#include "lib/servo.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Separation */
#define SEPARATION_RETRY_DELAY    100  /* Time in ms to wait before rechecking if the separation is blocked */
#define SEPARATION_MAX_RETRIES    10   /* Number of retries if the seperation is blocked */
#define SERVO_LAUNCHER_STEP       10   /* Size of single step for the launch servo */ //TODO
#define SERVO_LAUNCHER_STEP_DELAY 10   /* Delay in ms to wait between steps */ //TODO
#define SERVO_LAUNCHER_RELIEF     5    /* Servo position delta to relieve servo */


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
void doMammothNode(node_param_t* param, volatile game_state_t* game_state) {

	/* local variables */
	/* Variable to set the servo position step by step */
	volatile uint16_t servo_pos;

	/* Make sure the separation is all the way in */
	setServo_1(SERVO_POS_FRESCO_IN);
	/* Move the launcher servo all the way forward, in case it isn't already */
	setServo_2(SERVO_POS_LAUNCHER_LOAD);
	/* Wait some time while servos move */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Move the launcher servo all the way back to launch the two loaded balls, step by step */
	servo_pos = SERVO_POS_LAUNCHER_LOAD;  /* Current position */
	while(servo_pos > (SERVO_POS_LAUNCHER_LAUNCH+SERVO_LAUNCHER_STEP)) {

		/* Decrement servo position by step size */
		servo_pos -= SERVO_LAUNCHER_STEP;

		/* Check if it's the last step */
		if(servo_pos < SERVO_POS_LAUNCHER_LAUNCH) {

			/* Set the final servo position without over-rotating */
			setServo_2(SERVO_POS_LAUNCHER_LAUNCH);
		}
		else {

			/* Set the new servo position */
			setServo_2(servo_pos);
		}
		/* Wait some time while servo moves */
		vTaskDelay(SERVO_LAUNCHER_STEP_DELAY / portTICK_RATE_MS);
	}

	/* Move the launcher servo all the way forward, step by step */
	servo_pos = SERVO_POS_LAUNCHER_LAUNCH;  /* Current position */
	while(servo_pos < (SERVO_POS_LAUNCHER_LOAD-SERVO_LAUNCHER_STEP)) {
		/* Decrement servo position by step size */
		servo_pos += SERVO_LAUNCHER_STEP;

		/* Check if it's the last step */
		if(servo_pos > SERVO_POS_LAUNCHER_LOAD) {

			/* Set the final servo position without over-rotating */
			setServo_2(SERVO_POS_LAUNCHER_LOAD);
		}
		else {

			/* Set the new servo position */
			setServo_2(servo_pos);
		}
		/* Wait some time while servo moves */
		vTaskDelay(SERVO_LAUNCHER_STEP_DELAY / portTICK_RATE_MS);
	}
	/* Launcher servo relief */
	if(SERVO_POS_LAUNCHER_LOAD > SERVO_POS_LAUNCHER_LAUNCH) {
		setServo_2(SERVO_POS_LAUNCHER_LOAD-SERVO_LAUNCHER_RELIEF);
	}
	else {
		setServo_2(SERVO_POS_LAUNCHER_LOAD+SERVO_LAUNCHER_RELIEF);
	}

	/* Move the separation all the way out */
	setServo_1(SERVO_POS_FRESCO_OUT);
	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Move the separation all the way in */
	setServo_1(SERVO_POS_FRESCO_IN);
	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Report status */
	param->node_state = NODE_FINISH_SUCCESS;
}


/**
 * @}
 */


