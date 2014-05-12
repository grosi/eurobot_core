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
    /* Variable to set the servo position step by step */
    volatile uint16_t servo_pos;
    /* Copy current game state, so it wont be changed during calculation */
    taskENTER_CRITICAL();
    game_state_t game_state_copy = *game_state;
    taskEXIT_CRITICAL();

	/* Don't continue if an other robot is in front */
	if(isRobotInFront(&game_state_copy))
	{
		param->node_state = NODE_FINISH_ERROR;
		return;
	}

	/* reset current barrier flag */
	switch(param->id)
	{
	case 1:
        game_state_copy.barrier &= ~(GOTO_FIRE_1_FORCE | GOTO_FIRE_1 | GOTO_FIRE_2_FORCE | GOTO_FIRE_2);
        break;
	case 2:
        game_state_copy.barrier &= ~(GOTO_FIRE_3_FORCE | GOTO_FIRE_3 | GOTO_FIRE_4_FORCE | GOTO_FIRE_4);
        break;
	case 3:
        game_state_copy.barrier &= ~(GOTO_FIRE_5_FORCE | GOTO_FIRE_5 | GOTO_FIRE_6_FORCE | GOTO_FIRE_6);
        break;
	}

    /* Move the sucker servo down a bit, step by step */
	servo_pos = SERVO_POS_AIR_UP; /* Current position */
    while(servo_pos > (SERVO_POS_AIR_SECOND_FIRE+SERVO_AIR_STEP))
    {
        /* Decrement servo position by step size */
        servo_pos -= SERVO_AIR_STEP;

        /* Check if it's the last step */
        if(servo_pos < SERVO_POS_AIR_SECOND_FIRE)
        {
            /* Set the final servo position without over-rotating */
            setServo_1(SERVO_POS_AIR_SECOND_FIRE);
        }
        else
        {
            /* Set the new servo position */
            setServo_1(servo_pos);
        }
        /* Wait some time while servo moves */
        vTaskDelay(SERVO_AIR_STEP_DELAY / portTICK_RATE_MS);
    }


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

	/* Move the sucker servo up, step by step */
    servo_pos = SERVO_POS_AIR_SECOND_FIRE; /* Current position */
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


