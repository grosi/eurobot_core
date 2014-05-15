/**
 * \file    FireWallNormalNode.c
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
#include "FireWallNormalNode.h"
/* lib */
#include "lib/air.h"
#include "lib/servo.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doFireWallNormalNode
 * \brief       Tries to complete the fire node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doFireWallNormalNode(node_param_t* param, volatile game_state_t* game_state)
{
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

    /* Move the sucker servo up, step by step just to make sure*/
	placeSucker(SERVO_POS_AIR_UP);

    /* Drive over fire from NORTH */
    if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
    {
        txGotoXY(param->x, param->y-FIRE_WALL_NODE_DELTA_GO, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
    }
    /* Drive over fire from EAST */
    else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
    {
        txGotoXY(param->x-FIRE_WALL_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
    }
    /* Drive over fire from SOUTH */
    else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
    {
        txGotoXY(param->x, param->y+FIRE_WALL_NODE_DELTA_GO, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
    }
    /* Drive over fire from WEST */
    else
    {
        txGotoXY(param->x+FIRE_WALL_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_FORWARD);
    }

// TODO add Rangfinder flag
//    /* Stop driving when Rangefinderflag (5 cm) is set */
//    if()
//    {
//    	txStopDrive();
//    }

    /* Wait while driving */
	vTaskDelay(FIRE_WALL_NODE_DRIVE_DELAY / portTICK_RATE_MS);

	/* Move the sucker servo down a bit, step by step */
	placeSucker(SERVO_POS_AIR_WALL);

    /* Drive 5 cm backwards and drop the fire */
    txGotoXY(param->x, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_BACKWARD);
    vTaskDelay(FIRE_WALL_NODE_DRIVE_DELAY / portTICK_RATE_MS);
    txGotoXY(param->x, param->y, param->angle, FIRE_WALL_NODE_SPEED, FIRE_WALL_NODE_BARRIER, GOTO_DRIVE_BACKWARD);

    /* wait while driving backwards */
    vTaskDelay(FIRE_WALL_NODE_DRIVE_BACK_DELAY / portTICK_RATE_MS);

    /* Move the sucker servo up, step by step */
    placeSucker(SERVO_POS_AIR_UP);

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
