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
volatile uint8_t Mammoth_flag_SeparationDone = 1;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doMammothNode
 * \brief       Moves the separation out if it's save to do so (checks rangefinder)
 * \note        Doesn't wait after the servo value is set. Also doesn't move the servo in again.
 *
 * \param       retry_delay      Delay in ms between retries
 * \param       retry_count_max  Max. number of retries (including first try)
 * \return      separation_done  1 if separation done, 0 if undone
 */
uint8_t moveSeparationOutSavely(uint8_t retry_delay, uint8_t retry_count_max) {

	/* Variable to count number of retries */
	uint8_t i = 0;

//	/* Check the rangefinder */
//	while(Rangefinder_flag_SeAlarmUS) {
//
//		/* Separation (fresco panel) space is blocked, wait or return */
//		if(i < retry_count_max) {
//
//			vTaskDelay(retry_delay / portTICK_RATE_MS);
//			i++;
//		}
//		else {
//
//			/* Max retries reached */
//			return 0;
//		}
//	}

	/* Move the separation all the way out */
	setServo_1(SERVO_POS_FRESCO_OUT);
	return 1;
}


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

	/* Activate rangefinder (needed to check separation space) */
	vTaskResume(xRangefinderTask_Handle);

	/* Check if on the previous run the separation couldn't be done because it was blocked too long */
	if(!Mammoth_flag_SeparationDone) {

		/* Retry it now */
		Mammoth_flag_SeparationDone = moveSeparationOutSavely(SEPARATION_RETRY_DELAY, SEPARATION_MAX_RETRIES);
		/* Don't continue if it's still not possible to move separation out */
		if(!Mammoth_flag_SeparationDone) {

			/* Report status */
			param->node_state = NODE_FINISH_ERROR;
			return;
		}
	}

	/* Move the launcher servo all the way forward, in case it isn't already */
	setServo_2(SERVO_POS_LAUNCHER_LOAD);
	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Make sure the separation is all the way in */
	setServo_1(SERVO_POS_FRESCO_IN);

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

	/* Move the separation out if it's save to do so */
	Mammoth_flag_SeparationDone = moveSeparationOutSavely(SEPARATION_RETRY_DELAY, SEPARATION_MAX_RETRIES);
	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Make sure the separation is all the way in */
	setServo_1(SERVO_POS_FRESCO_IN);

//	/* Wait some time while servo moves */
//	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Suspend rangefinder safely */
	suspendRangefinderTask();

	/* Report status */
	param->node_state = NODE_FINISH_SUCCESS;
}


/**
 * @}
 */


