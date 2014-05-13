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
#include "../Rangefinder.h"
#include "../CANGatekeeper.h"
#include "NodeConfig.h"
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
static void releasePool(uint8_t);
static uint8_t takePool(uint16_t, uint16_t, uint16_t, volatile game_state_t*);


/* Private functions ---------------------------------------------------------*/




/**
 * \fn      releasePool
 * \brief   take the pool free
 *
 * \param[in]   air     1:=air still on; 0:=air off
 */
static void releasePool(uint8_t air)
{
    /* local variable */
    volatile uint16_t servo_pos; /* Variable to set the servo position step by step */

    if(air)
    {
        setAir(AIR_OFF);
    }

    /* Move the sucker servo up, step by step */
    servo_pos = getServo_1(); /* Current position */
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
}


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
    uint8_t success;

    /* Drive closer to the pool */
    if(checkDrive(x,y,angle,FIRE_POOL_APPROACH_SPEED,GOTO_DRIVE_FORWARD,game_state))
    {
        while(!getSensor_Fire_Pool_Left() && !getSensor_Fire_Pool_Left() && approach_counter < FIRE_POOL_APPROACH_TIME)
        {
            /* Wait some time before next check */
            vTaskDelay(FIRE_POOL_DRIVECHECK_DELAY / portTICK_RATE_MS);
            approach_counter += FIRE_POOL_DRIVECHECK_DELAY;
        }

        /* Stop driving further */
        txStopDrive();

        /* check pool position */
        if(getSensor_Fire_Pool_Left() && getSensor_Fire_Pool_Right())
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
                vTaskDelay(SERVO_AIR_STEP_DELAY*10 / portTICK_RATE_MS);
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
    /* local variables */
    uint8_t fire_count;
    uint16_t x_pool, y_pool, angle_pool;
    uint16_t x_heart, y_heart, angle_heart;

    /* differ between the tow possible teamcolors */
    if(game_state->teamcolor == TEAM_YELLOW)
    {
        x_pool = FIRE_POOL_HEARTPOOL_X_POSITION_YELLOW;
        y_pool = FIRE_POOL_HEARTPOOL_Y_POSITION_YELLOW;
        angle_pool = FIRE_POOL_HEARTPOOL_ANGLE_POSITION_YELLOW;
        x_heart = FIRE_POOL_HEART_X_POSITION_YELLOW;
        y_heart = FIRE_POOL_HEART_Y_POSITION_YELLOW;
        angle_heart = FIRE_POOL_HEART_ANGLE_POSITION_YELLOW;
    }
    else
    {
        x_pool = FIRE_POOL_HEARTPOOL_X_POSITION_RED;
        y_pool = FIRE_POOL_HEARTPOOL_Y_POSITION_RED;
        angle_pool = FIRE_POOL_HEARTPOOL_ANGLE_POSITION_RED;
        x_heart = FIRE_POOL_HEART_X_POSITION_RED;
        y_heart = FIRE_POOL_HEART_Y_POSITION_RED;
        angle_heart = FIRE_POOL_HEART_ANGLE_POSITION_RED;
    }


    /* try to get the fire-pool */
    if(takePool(param->x, param->y - FIRE_POOL_APPROACH_DISTANCE - FIRE_POOL_APPROACH_OVERHEAD,
            param->angle, game_state))
    {
        /* drive to the heart of fire */
        if(checkDrive(x_pool,y_pool,angle_pool,FIRE_POOL_TRANSIT_SPEED,GOTO_DRIVE_FORWARD,game_state))
        {
            for(fire_count = 0; fire_count < 3; fire_count++)
            {
                /* move the sucker up with fire */
                releasePool(1);

                /* the second fire lies on the wrong side */
                if(fire_count % 2)
                {
                    /* turn a bit */
                    if(checkDrive(x_pool,y_pool,angle_pool,FIRE_POOL_TRANSIT_SPEED,GOTO_DRIVE_FORWARD,game_state)){break;};
                    releasePool(0);
                }
                else
                {
                    /* drive back 5cm*/
                    if(checkDrive(x_pool,y_pool,angle_pool,FIRE_POOL_APPROACH_SPEED,GOTO_DRIVE_BACKWARD,game_state)){break;};
                    /* drive to the front of heart*/
                    if(checkDrive(x_heart-50,y_heart-50,angle_heart,FIRE_POOL_TRANSIT_SPEED,GOTO_DRIVE_FORWARD,game_state)){break;};
                    /* moves sucker down */
                    placeSucker(SERVO_POS_AIR_HEART);
                    /* drive a bit forward (slowly) and shift the other fire back */
                    if(checkDrive(x_heart,y_heart,angle_heart,FIRE_POOL_APPROACH_SPEED,GOTO_DRIVE_FORWARD,game_state)){break;};
                    /* place fire and moves the sucker up */
                    releasePool(0);
                    /* drive a bit backwarts */
                    if(checkDrive(x_heart-50,y_heart-50,angle_heart,FIRE_POOL_APPROACH_SPEED,GOTO_DRIVE_BACKWARD,game_state)){break;};
                }

                if(fire_count < 2)
                {
                    /* drive back to the pool and take the next fire */
                    if(checkDrive(x_pool-50,y_pool-50,angle_pool,FIRE_POOL_TRANSIT_SPEED,GOTO_DRIVE_FORWARD,game_state)){break;};
                    if(takePool(x_pool, y_pool,angle_pool, game_state)){break;};
                }
            }
            releasePool(0); /* be sure that the sucker is up and the air-system off */
            param->node_state = NODE_FINISH_SUCCESS;
        }
        else
        {
            param->node_state = NODE_FINISH_SUCCESS;
            releasePool(0);
        }
    }
    else
    {
        param->node_state = NODE_FINISH_ERROR;
    }
}

/**
 * @}
 */
