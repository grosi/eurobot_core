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
#include "NodeConfig.h"
#include "NodeMisc.h"
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

	/* Move the sucker servo down a bit, step by step */
	placeSucker(SERVO_POS_AIR_WALL_INVERSE);

	// TODO curve while driving foreward but there is no curve espected...
	/* Drive over fire from NORTH */
	if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
	{
		checkDrive(param->x, param->y-FIRE_WALL_INVERSE_NODE_DELTA_GO, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
	}
	/* Drive over fire from EAST */
	else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
	{
		checkDrive(param->x - FIRE_WALL_INVERSE_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
	}
	/* Drive over fire from SOUTH */
	else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
	{
		checkDrive(param->x, param->y+FIRE_WALL_INVERSE_NODE_DELTA_GO, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
	}
	/* Drive over fire from WEST */
	else
	{
		checkDrive(param->x+FIRE_WALL_INVERSE_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
	}

	/* activate sucker */
	setAir(AIR_ON);

	/* Move the sucker servo down a bit, step by step */
	servo_pos = getServo_1(); /* Current position */
	while(servo_pos > (SERVO_POS_AIR_WALL_INVERSE_SUCKER + SERVO_STEP) && !getSensor_Air())
	{
		/* Decrement servo position by step size */
		servo_pos -= SERVO_STEP;

		/* Check if it's the last step */
		if(servo_pos < SERVO_WALL_INVERS_POS_DOWN)
		{
			/* Set the final servo position without over-rotating */
			setServo_1(SERVO_WALL_INVERS_POS_DOWN);
		}
		else
		{
			/* Set the new servo position */
			setServo_1(servo_pos);
		}
		/* Wait some time while servo moves  (a bit slower)*/
		vTaskDelay(SERVO_STEP_DELAY*10 / portTICK_RATE_MS);
	}

	/* no fire detected */
	if(getServo_1() <= SERVO_POS_AIR_WALL_INVERSE_SUCKER + SERVO_WALL_INVERS_STEP)
	{
		/* Move sucker down */
		placeSucker(SERVO_POS_UP);

		/* Drive 5 cm backwards */
		while(checkDrive(param->x, param->y, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

		/* Deactivate sucker */
		setAir(AIR_OFF);

		param->node_state = NODE_FINISH_SUCCESS;
		return;
	}

	/* Move the sucker servo up with fire, step by step */
	placeSucker(SERVO_POS_UP);

	/* Drive 5 cm backwards */
	while(checkDrive(param->x, param->y, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

	/* drive before the heart of fire in the middle */
	if(checkDrive(X_HEART_OF_FIRE, Y_HEART_OF_FIRE - Y_APPROACH_HEART_OF_FIRE, HEART_OF_FIRE_ANGLE, HEART_OF_FIRE_DRIVE_SPEED, GOTO_DRIVE_FORWARD, game_state) != FUNC_SUCCESS)
	{
		/* Drive 5 cm backwards if it's not possible to get to the heart of fire */
		while(checkDrive(param->x, param->y, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

		/* place the fire where the robot stopped */
		setAir(AIR_OFF);

		/* Don't continue */
		param->node_state = NODE_FINISH_SUCCESS;
		return;
	}

	/* move sucker down for placing fire on heart */
	placeSucker(SERVO_POS_AIR_PLACE);

	/* drive to the heart of fire place position */
	if(checkDrive(X_HEART_OF_FIRE, Y_HEART_OF_FIRE, HEART_OF_FIRE_ANGLE, HEART_OF_FIRE_DRIVE_SPEED, GOTO_DRIVE_FORWARD, game_state)  != FUNC_SUCCESS)
	{
		/* place the fire where the robot stopped */
		setAir(AIR_OFF);
		/* Don't continue */
		param->node_state = NODE_FINISH_SUCCESS;
		return;

	}

	/* place fire */
	setAir(AIR_OFF);

	/* move sucker up */
	placeSucker(SERVO_WALL_INVERS_POS_UP);

	/* Drive 5 cm backwards */
	while(checkDrive(param->x, param->y, param->angle, FIRE_WALL_INVERS_NODE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

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
