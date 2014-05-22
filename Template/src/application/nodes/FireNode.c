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
#include "NodeMisc.h"
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
    game_state_t game_state_copy;

    taskENTER_CRITICAL();
	/* reset current barrier flag */
	switch(param->id)
	{
	    case 10:
	        game_state->barrier &= ~(GOTO_FIRE_1_FORCE | GOTO_FIRE_1 | GOTO_FIRE_2_FORCE | GOTO_FIRE_2);
	        break;
	    case 11:
	        game_state->barrier &= ~(GOTO_FIRE_3_FORCE | GOTO_FIRE_3 | GOTO_FIRE_4_FORCE | GOTO_FIRE_4);
            break;
	    case 12:
	        game_state->barrier &= ~(GOTO_FIRE_5_FORCE | GOTO_FIRE_5 | GOTO_FIRE_6_FORCE | GOTO_FIRE_6);
	        break;
	}

	/* Copy current game state, so it wont be changed during calculation */
    game_state_copy = *game_state;
    taskEXIT_CRITICAL();

	/* Move the separation all the way out */
	setServo_1(SERVO_POS_FRESCO_OUT);

	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

    /* Drive through fire from NORTH */
    if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
    {
        checkDrive(param->x, param->y - FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, &game_state_copy);
    }
    /* Drive through fire from EAST */
    else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
    {
        checkDrive(param->x - FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, &game_state_copy);
    }
    /* Drive through fire from SOUTH */
    else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
    {
        checkDrive(param->x, param->y + FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, &game_state_copy);
    }
    /* Drive through fire from WEST */
    else
    {
        checkDrive(param->x + FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, &game_state_copy);
    }

    /* Put seperation in after driving through the fire */
	setServo_1(SERVO_POS_FRESCO_IN);

	/* Wait some time while servo moves */
	vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);

	param->node_state = NODE_FINISH_SUCCESS;
}

/**
 * @}
 */


