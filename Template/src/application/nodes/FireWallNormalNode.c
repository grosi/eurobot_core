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
#include "NodeConfig.h"
#include "NodeMisc.h"
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

    /* Move the sucker servo up, step by step just to make sure*/
	placeSucker(SERVO_WALL_NORMAL_POS_UP);

	// TODO handle return value of checkDrive
    /* Drive over fire from NORTH */
    if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
    {
    	checkDrive(param->x, param->y-FIRE_WALL_NODE_DELTA_GO, param->angle, FIRE_WALL_NORMAL_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    }
    /* Drive over fire from EAST */
    else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
    {
        checkDrive(param->x-FIRE_WALL_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_NORMAL_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    }
    /* Drive over fire from SOUTH */
    else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
    {
        checkDrive(param->x, param->y+FIRE_WALL_NODE_DELTA_GO, param->angle, FIRE_WALL_NORMAL_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    }
    /* Drive over fire from WEST */
    else
    {

    	checkDrive(param->x+FIRE_WALL_NODE_DELTA_GO, param->y, param->angle, FIRE_WALL_NORMAL_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
    }

	/* Move the sucker servo down a bit, step by step */
	placeSucker(SERVO_POS_WALL);

    /* Drive 5 cm backwards and drop the fire */
	while(checkDrive(param->x, param->y, param->angle, FIRE_WALL_NORMAL_NODE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

    /* Drive 5 cm backwards and drop the fire */
    while(checkDrive(param->x, param->y, param->angle, FIRE_WALL_NORMAL_NODE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

    /* Drive 5 cm backwards and drop the fire */
	while(checkDrive(param->x, param->y, param->angle, FIRE_WALL_NORMAL_NODE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

    /* Move the sucker servo up, step by step */
    placeSucker(SERVO_WALL_NORMAL_POS_UP);

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
