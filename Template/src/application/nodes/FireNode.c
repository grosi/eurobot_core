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
    /* local variables */
    /* Copy current game state, so it wont be changed during calculation */
//    taskENTER_CRITICAL();
//    game_state_t game_state_copy = *game_state;
//    taskEXIT_CRITICAL();

	/* Don't continue if an other robot is in front */
	if(isRobotInRange(game_state, FALSE))
	{
		param->node_state = NODE_FINISH_ERROR;
		return;
	}

	/* reset current barrier flag */
	taskENTER_CRITICAL();
	switch(param->id)
	{
	case 1:
        game_state->barrier &= ~(GOTO_FIRE_1_FORCE | GOTO_FIRE_1 | GOTO_FIRE_2_FORCE | GOTO_FIRE_2);
        break;
	case 2:
	    game_state->barrier &= ~(GOTO_FIRE_3_FORCE | GOTO_FIRE_3 | GOTO_FIRE_4_FORCE | GOTO_FIRE_4);
        break;
	case 3:
	    game_state->barrier &= ~(GOTO_FIRE_5_FORCE | GOTO_FIRE_5 | GOTO_FIRE_6_FORCE | GOTO_FIRE_6);
        break;
	}
	taskEXIT_CRITICAL();


    /* Move the sucker servo down a bit, step by step */
	placeSucker(SERVO_POS_AIR_THIRD_FIRE);

    /* Drive through fire from NORTH */
    if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
    {
        checkDrive(param->x, param->y-FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    	//txGotoXY(param->x, param->y-FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }
    /* Drive through fire from EAST */
    else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
    {
        checkDrive(param->x-FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    	//txGotoXY(param->x-FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }
    /* Drive through fire from SOUTH */
    else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
    {
        checkDrive(param->x, param->y+FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    	//txGotoXY(param->x, param->y+FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }
    /* Drive through fire from WEST */
    else
    {
        checkDrive(param->x+FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    	//txGotoXY(param->x+FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, game_state_copy.barrier, GOTO_DRIVE_FORWARD);
    }

    /* Wait while driving */
	vTaskDelay(FIRE_NODE_DRIVE_DELAY / portTICK_RATE_MS);

	/* Move the sucker servo up, step by step */
	placeSucker(SERVO_POS_AIR_UP);

	/* node complete */
	param->node_state = NODE_FINISH_SUCCESS;

	/* Copy current game state back */
//    taskENTER_CRITICAL();
//    *game_state = game_state_copy;
//    taskEXIT_CRITICAL();
}

/**
 * @}
 */


