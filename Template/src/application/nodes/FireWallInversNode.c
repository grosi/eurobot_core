/**
 * \file    FireWallInversNode.c
 * \author  gross10
 * \date    2014-05-09
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fire wall node statemaschine
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "../Rangefinder.h"
#include "../CANGatekeeper.h"
#include "NodeConfig.h"
#include "FireWallInversNode.h"
/* lib */
#include "lib/air.h"
#include "lib/servo.h"
#include "lib/sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doFireWallInversNode
 * \brief       Tries to complete the fire node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doFireWallInversNode(node_param_t* param, volatile game_state_t* game_state)
{
	/* local variables */
	/* Variable to set the servo position step by step */
	volatile uint16_t servo_pos;
	/* Copy current game state, so it wont be changed during calculation */
	taskENTER_CRITICAL();
	game_state_t game_state_copy = *game_state;
	taskEXIT_CRITICAL();

//	/* Don't continue if an other robot is in front */
//	if(isRobotInFront(&game_state_copy))
//	{
//		param->node_state = NODE_FINISH_ERROR;
//		return;
//	}

	/* Move the sucker servo down a bit, step by step */
	servo_pos = SERVO_POS_AIR_UP; /* Current position */
	while(servo_pos > (SERVO_POS_AIR_WALL_INVERSE+SERVO_AIR_STEP))
	{
		/* Decrement servo position by step size */
		servo_pos -= SERVO_AIR_STEP;

		/* Check if it's the last step */
		if(servo_pos < SERVO_POS_AIR_WALL_INVERSE)
		{
			/* Set the final servo position without over-rotating */
			setServo_1(SERVO_POS_AIR_WALL_INVERSE);
		}
		else
		{
			/* Set the new servo position */
			setServo_1(servo_pos);
		}
		/* Wait some time while servo moves */
		vTaskDelay(SERVO_AIR_STEP_DELAY / portTICK_RATE_MS);
	}

	/* Drive over fire from NORTH */
	if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
	{
		txGotoXY(param->x, param->y-FIRE_WALL_INVERSE_NODE_DELTA_GO, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
	}
	/* Drive over fire from EAST */
	else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
	{
		txGotoXY(param->x-FIRE_WALL_INVERSE_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
	}
	/* Drive over fire from SOUTH */
	else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
	{
		txGotoXY(param->x, param->y+FIRE_WALL_INVERSE_NODE_DELTA_GO, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
	}
	/* Drive over fire from WEST */
	else
	{
		txGotoXY(param->x+FIRE_WALL_INVERSE_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
	}

// TODO add Rangfinder flag
//    /* Stop driving when Rangefinderflag (5 cm) is set */
//    if()
//    {
//    	txStopDrive();
//    }

	/* Wait while driving */
	vTaskDelay(FIRE_WALL_NODE_DRIVE_DELAY / portTICK_RATE_MS);

	/* activate sucker */
	setAir(AIR_ON);

	/* move sucker down */
	servo_pos = SERVO_POS_AIR_WALL_INVERSE; /* Current position */
	while(servo_pos > (SERVO_POS_AIR_WALL_INVERSE_SUCKER+SERVO_AIR_STEP))
	{
		/* Decrement servo position by step size */
		servo_pos -= SERVO_AIR_STEP;

		/* Check if it's the last step */
		if(servo_pos < SERVO_POS_AIR_WALL_INVERSE_SUCKER)
		{
			/* Set the final servo position without over-rotating */
			setServo_1(SERVO_POS_AIR_WALL_INVERSE_SUCKER);
		}
		else
		{
			/* Set the new servo position */
			setServo_1(servo_pos);
		}
		/* Wait some time while servo moves */
		vTaskDelay(SERVO_AIR_STEP_DELAY / portTICK_RATE_MS);

		/* stop moving the sucker down */
		if(getSensor_Air())
		{
			break;
		}
	}

	/* Move the sucker servo up, step by step */
	while(servo_pos < (SERVO_POS_AIR_UP+SERVO_AIR_STEP))
	{
		/* Decrement servo position by step size */
		servo_pos += SERVO_AIR_STEP;

		/* Check if it's the last step */
		if(servo_pos > SERVO_POS_AIR_UP)
		{
			/* Set the final servo position without over-rotating */
			setServo_1(SERVO_POS_AIR_UP);
		}
		else
		{
			/* Set the new servo position */
			setServo_1(servo_pos);
		}
		/* Wait some time while servo moves */
		vTaskDelay(SERVO_AIR_STEP_DELAY / portTICK_RATE_MS);
	}

	/* Drive 5 cm backwards */
	txGotoXY(param->x, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_BACKWARD);

	/* wait while driving backwards */
	vTaskDelay(FIRE_WALL_NODE_DRIVE_BACK_DELAY / portTICK_RATE_MS);

	// TODO to be defined if we go to the heart of fire or just place it anywhere else
	// TODO activate Rangefinder
	/* drive to the heart of fire in the middle */
	txGotoXY(1500, 600, 180, 100, GOTO_NO_BARRIER, GOTO_DRIVE_FORWARD);

	/* wait while driving to the heart of fire */
	vTaskDelay(FIRE_WALL_NODE_DRIVE_HEART_DELAY / portTICK_RATE_MS);

	/* move sucker down */
	servo_pos = SERVO_POS_AIR_UP; /* Current position */
	while(servo_pos > (SERVO_POS_AIR_PLACE+SERVO_AIR_STEP))
	{
		/* Decrement servo position by step size */
		servo_pos -= SERVO_AIR_STEP;

		/* Check if it's the last step */
		if(servo_pos < SERVO_POS_AIR_PLACE)
		{
			/* Set the final servo position without over-rotating */
			setServo_1(SERVO_POS_AIR_PLACE);
		}
		else
		{
			/* Set the new servo position */
			setServo_1(servo_pos);
		}
		/* Wait some time while servo moves */
		vTaskDelay(SERVO_AIR_STEP_DELAY / portTICK_RATE_MS);
	}

	// TODO to be defined if we go to the heart of fire or just place it anywhere else
	// TODO activate Rangefinder
	/* drive to the heart of fire in the middle */
	txGotoXY(1500, 650, 180, FIRE_WALL_NODE_SPEED, GOTO_NO_BARRIER, GOTO_DRIVE_FORWARD);

	/* wait while driving to the heart of fire */
	vTaskDelay(FIRE_HEART_DRIVE_DELAY / portTICK_RATE_MS);

	/* deactivate sucker, place fire */
	setAir(AIR_OFF);

	/* Drive 5 cm backwards */
	txGotoXY(param->x, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_BACKWARD);

	/* wait while driving backwards */
	vTaskDelay(FIRE_WALL_NODE_DRIVE_BACK_DELAY / portTICK_RATE_MS);

	/* node complete */
	param->node_state = NODE_FINISH_SUCCESS;

	/* Copy current game state back */
	taskENTER_CRITICAL();
	*game_state = game_state_copy;
	taskEXIT_CRITICAL();
}

/**
 * @}
 */
