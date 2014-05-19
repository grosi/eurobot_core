/**
 * \file    FirePoolNode.c
 * \author  gross10
 * \date    2014-05-09
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fire pool node statemaschine
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
#include "FirePoolNode.h"
/* lib */
#include "lib/air.h"
#include "lib/sensor.h"
#include "lib/servo.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static uint8_t takePool(uint16_t, uint16_t, uint16_t, volatile game_state_t*);


/* Private functions ---------------------------------------------------------*/

/**
 * \fn      takePool
 * \brief   take the pool or the next fire
 *
 * \param[in]   param   node parameters
 * \param[in]   game_state  current game states
 *
 * \retval  0   error
 * \retval  1   success
 */
static uint8_t takePool(uint16_t x, uint16_t y, uint16_t angle, volatile game_state_t* game_state)
{
    /* local variable */
    volatile uint16_t servo_pos; /* Variable to set the servo position step by step */
    volatile uint16_t approach_counter = 0;
    uint8_t pushed = 0;
    uint8_t success;

    /* Drive closer to the pool */
    if(driveGoto(x,y,angle,FIRE_POOL_APPROACH_SPEED,GOTO_DRIVE_FORWARD,game_state))
    {
        while(!getSensor_Fire_Pool_Left() && approach_counter < FIRE_POOL_APPROACH_TIME)
        {
            /* Wait some time before next check */
            vTaskDelay(FIRE_POOL_DRIVECHECK_DELAY / portTICK_RATE_MS);
            approach_counter += FIRE_POOL_DRIVECHECK_DELAY;

            if(getSensor_Fire_Pool_Left())
            {
                pushed = 1;
                break;
            }
        }

        /* Stop driving further */
        txStopDrive();

        /* check pool position */
        if(pushed)
        {
            /* Move the sucker servo down a bit, step by step */
            servo_pos = getServo_1(); /* Current position */
            while(servo_pos > (SERVO_POS_AIR_DOWN+SERVO_AIR_STEP) && !getSensor_Air())
            {
                /* Decrement servo position by step size */
                servo_pos -= SERVO_AIR_STEP;

                /* Check if it's the last step */
                if(servo_pos < SERVO_POS_AIR_DOWN)
                {
                    /* Set the final servo position without over-rotating */
                    setServo_1(SERVO_POS_AIR_DOWN);
                }
                else
                {
                    /* Set the new servo position */
                    setServo_1(servo_pos);
                }
                /* Wait some time while servo moves  (a bit slower)*/
                vTaskDelay(SERVO_AIR_STEP_DELAY*5 / portTICK_RATE_MS);
            }

            setAir(AIR_ON);

            /* pool is at the right position and sucket */
            success = 1;
        }
        else
        {
            /* no pool detected */
            success = 0;
        }
    }
    else
    {
        /*enemy is in front */
        success = 0;
    }

    return success;
}


/**
 * \fn          doFireWallInversNode
 * \brief       Tries to complete the fire node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doFirePoolNode(node_param_t* param, volatile game_state_t* game_state)
{

    /* reset barrier */
    game_state->barrier &= ~(GOTO_FIRE_POOL_1_FORCE | GOTO_FIRE_POOL_2_FORCE);


    /* try to get the fire-pool */
	if(takePool(param->x, param->y - (FIREPOOL_APPROACHDISTANCE + FIRE_POOL_DELTA_GO), param->angle, game_state))
    {
	    /* put the sucker up */
	    placeSucker(SERVO_POS_AIR_UP);

	    /* turn right if Team yellow, left if team red */
	    if(game_state->teamcolor == TEAM_YELLOW)
	    {
	        checkDrive(param->x, param->y - (FIREPOOL_APPROACHDISTANCE + FIRE_POOL_DELTA_GO), ANGLE_TURN_RIGHT, FIRE_POOL_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
	    }
	    else
	    {
	        checkDrive(param->x, param->y - (FIREPOOL_APPROACHDISTANCE + FIRE_POOL_DELTA_GO), ANGLE_TURN_LEFT, FIRE_POOL_NODE_SPEED, GOTO_DRIVE_FORWARD, game_state);
	    }

        /* drive before the heart of fire in the middle */
        if(checkDrive(X_HEART_OF_FIRE, Y_HEART_OF_FIRE - Y_APPROACH_HEART_OF_FIRE, HEART_OF_FIRE_ANGLE, HEART_OF_FIRE_DRIVE_SPEED, GOTO_DRIVE_FORWARD, game_state) != FUNC_SUCCESS)
        {
            // TODO try to drive to the heart of fire in the corner

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
        placeSucker(SERVO_POS_AIR_UP);

        /* Drive 5 cm backwards */
        while(checkDrive(param->x, param->y, param->angle, HEART_OF_FIRE_DRIVE_SPEED, GOTO_DRIVE_BACKWARD, game_state) != FUNC_SUCCESS);

        param->node_state = NODE_FINISH_SUCCESS;

    }
    else
    {
        param->node_state = NODE_FINISH_SUCCESS;
    }
}

/**
 * @}
 */
