/**
 * \file    FireNode.c
 * \author  gross10
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fire node statemaschine
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
#include "FireNode.h"

/* lib */
#include "lib/servo.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doFireNode
 * \brief       Tries to complete the fire node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doFireNode(node_param_t* param, volatile game_state_t* game_state)
{
    /* Copy current game state, so it wont be changed during calculation */
    taskENTER_CRITICAL();
    game_state_t game_state_copy = *game_state;
    taskEXIT_CRITICAL();

	/* Don't continue if an other robot is in front */
	if(isRobotInFront(&game_state_copy)) {

		param->node_state = NODE_FINISH_ERROR;
		return;
	}

	/* reset current barrier flag */
	switch(param->id)
	{
	    case 9:
	        game_state_copy.barrier &= ~(GOTO_FIRE_1_FORCE | GOTO_FIRE_1 | GOTO_FIRE_2_FORCE | GOTO_FIRE_2);
	        break;
	    case 10:
	        game_state_copy.barrier &= ~(GOTO_FIRE_3_FORCE | GOTO_FIRE_3 | GOTO_FIRE_4_FORCE | GOTO_FIRE_4);
            break;
	    case 11:
	        game_state_copy.barrier &= ~(GOTO_FIRE_5_FORCE | GOTO_FIRE_5 | GOTO_FIRE_6_FORCE | GOTO_FIRE_6);
	        break;
	}

	/* Move the separation all the way out */
	setServo_1(SERVO_POS_FRESCO_OUT);

	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

    /* Drive through fire from NORTH */
    if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
    {
        txGotoXY(param->x, param->y-FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }
    /* Drive through fire from EAST */
    else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
    {
        txGotoXY(param->x-FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }
    /* Drive through fire from SOUTH */
    else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
    {
        txGotoXY(param->x, param->y+FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }
    /* Drive through fire from WEST */
    else
    {
        txGotoXY(param->x+FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }

    /* Wait while driving */
	vTaskDelay(FIRE_NODE_DRIVE_DELAY / portTICK_RATE_MS);

    /* Put seperation in after driving through the fire */
	setServo_1(SERVO_POS_FRESCO_IN);

	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	/* Copy current game state back */
	taskENTER_CRITICAL();
	*game_state = game_state_copy;
	taskEXIT_CRITICAL();

	param->node_state = NODE_FINISH_SUCCESS;
}

/**
 * @}
 */


