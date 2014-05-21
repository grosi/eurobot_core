/**
 * \file    NodeMisc.c
 * \author  gross10
 * \date    2014-05-19
 *
 * \version 1.0
 *  create this file
 *
 * \brief   a few node functions
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* standard */
#include <math.h>

/* application */
#include "../AppConfig.h"
#include "../Rangefinder.h"
#include "../CANGatekeeper.h"
#include "NodeConfig.h"
#include "NodeMisc.h"

/* lib */
#include "../../lib/servo.h"
#include "../../lib/sensor.h"
#include "../../lib/air.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define GOTOCONFIRM_QUEUE_LENGTH   1
#define GOTOSTATERESP_QUEUE_LENGTH 1
#define GOTO_STATERESP_DELAY       200

#define DRIVE_ROUTE_DIST_MIN       150   /* Minimum distance in mm for which the drive system calculates a route */
#define DRIVE_BACK_DIST            50    /* Distance in mm the robot travels on a backwards goto command */
#define DRIVE_BACK_TIME            1500  /* Time taken to travel DRIVE_BACK_DIST backwards */
#define DIST_OFFSET                50    /* Safety distance offset in mm */  //TODO remove this or the one below
#define SPEED_OFFSET               25    /* Safety speed offset in mm */  //TODO remove this or the one above

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* CAN */
xQueueHandle qGotoConfirm;
xQueueHandle qGotoStateResp;

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initNodeResources
 * \brief       Function to initialise all the resources needed by the node task
 *
 * \param[in]   None
 * \return      None
 */
void initNodeResources()
{
    /* Initialise fresco/separation servo and set start position */
    initServo_1();
    setServo_1(SERVO_AIR_INIT);
    /* Initialise launcher servo and set start position */
    initServo_2();
    setServo_2(SERVO_NET_INIT);

    /* Initialise fresco sensors */
    initSensor_Fire_Pool_Left();
    initSensor_Air();

    /* Initialise air system */
    initAir();

    /* Create a queue and set CAN listener for GoTo ACK */
    qGotoConfirm = xQueueCreate(GOTOCONFIRM_QUEUE_LENGTH, sizeof(CAN_data_t));
    if(qGotoConfirm != 0) {
        setQueueCANListener(qGotoConfirm, GOTO_CONFIRM);
    }

    /* Create a queue and set CAN listener for GoTo state response */
    qGotoStateResp = xQueueCreate(GOTOSTATERESP_QUEUE_LENGTH, sizeof(CAN_data_t));
    if(qGotoStateResp != 0) {
        setQueueCANListener(qGotoStateResp, GOTO_STATE_RESPONSE);
    }

}


/**
* \fn setNodeConfig2Default
* \brief set all node actors to default.
*/
void setNodeConfig2Default(void)
{
    /* servos */
    setServo_1(SERVO_AIR_INIT);
    //setServo_2(SERVO_NET_INIT);
    setAir(0);
}


/**
 * \fn distance2speed
 * \brief returns a speed according
 *
 * \param[in] distance  absolute distance in mm (without radiuses)
 * \param[in] max_speed maximum speed in percent
 *
 * \retval speed in percent
 */
uint8_t distance2speed(uint16_t distance, uint8_t max_speed)
{
    uint8_t speed = max_speed * distance / RANGEFINDER_THRESHOLD_FW;

    /* Handle max. speed */
    if(speed > max_speed)
    {
        speed = max_speed;
    }
    /* Handle safety offset */
    if(distance <= DIST_OFFSET)
    {
    	speed = 0;
    }

    return speed;
}


/**
 * \fn      checkDrive
 * \brief   safety drive to the next X/Y position and control the route
 *
 * \param[in]  x   x-position [mm]
 * \param[in]  y   y-position [mm]
 * \param[in]  angle
 * \param[in]  speed
 * \param[in]  direction
 * \param[in]  game_state
 *
 * \retval     func_report (FUNC_SUCCESS, FUNC_INCOMPLETE_LIGHT, FUNC_INCOMPLETE_HEAVY or FUNC_ERROR)
 */
func_report_t checkDrive(uint16_t x, uint16_t y, uint16_t angle, uint8_t speed, uint8_t direction, volatile game_state_t* game_state)
{
    CAN_data_t CAN_buffer;
    uint32_t estimated_GoTo_time;
    uint32_t estimated_GoTo_time_history[5] = {0};
    uint8_t estimated_GoTo_time_index = 0;
    func_report_t retval;
    uint8_t CAN_ok;
    int16_t delta_x, delta_y;
    uint16_t distance;
    uint16_t range;
    uint8_t is_in_range = 0; /* is in range flag */

    /* Differentiate between driving backward and forward */
    if(direction == GOTO_DRIVE_FORWARD)
    {
        /* If distance is to small, the drive system doesn't calculate a route and thus doesn't check if an enemy
         * blocks the path. We have to check this here, else we start drive until the rangefinder reports an enemy. */
        taskENTER_CRITICAL();
        delta_x = x - game_state->x;
        delta_y = y - game_state->y;
        taskEXIT_CRITICAL();
        distance = round(sqrt(delta_x*delta_x + delta_y*delta_y));

        if(distance <= DRIVE_ROUTE_DIST_MIN)  /* No route calculation, just driving */
        {
            range = isRobotInRange(game_state, FALSE);
            if((range == 0) || (range >= DRIVE_ROUTE_DIST_MIN + DIST_OFFSET))  /* No enemy in range */
            {
                /* Drive forward */
                if(driveGoto(x, y, angle, speed, direction, GOTO_ROUTE, game_state))
                {
                    /* Wait at least GOTO_STATERESP_DELAY before asking for goto time for the first time,
                     * else we may get the old time */
                    vTaskDelay(GOTO_STATERESP_DELAY / portTICK_RATE_MS);

                    do
                    {
                        /* Ask drive system for GoTo state */
                        txGotoStateRequest();
                        /* Receive the GoTo state response */
                        CAN_ok = xQueueReceive(qGotoStateResp, &CAN_buffer, CAN_WAIT_DELAY / portTICK_RATE_MS);

                        if(CAN_ok == pdFALSE)
                        {
                            retval = FUNC_ERROR;
                            break;
                        }

                        /* Extract time */
                        estimated_GoTo_time = CAN_buffer.state_time;  /* In ms */

                        /* Handle "goto not possible at the moment" message */
                        if(estimated_GoTo_time == GOTO_NOT_POSSIBLE_ATM)
                        {
                            /* Finish node with error,
                             * this way the current node will be retried if it's more attractive again */
                            retval = FUNC_INCOMPLETE_HEAVY;
                            break;
                        }

                        /* Handle "goto blocked at the moment" message */
                        if(estimated_GoTo_time == GOTO_BLOCKED_ATM)
                        {
                            /* Finish node with error,
                             * this way the current node will be retried if it's more attractive again */
                            retval = FUNC_INCOMPLETE_HEAVY;
                            break;
                        }

//                        /* Handle Drive-Node Bug */
//                        if(estimated_GoTo_time == estimated_GoTo_time_history[estimated_GoTo_time_index])
//                        {
//                            estimated_GoTo_time_history[estimated_GoTo_time_index] = estimated_GoTo_time;
//                            estimated_GoTo_time_index++;
//
//                            if(estimated_GoTo_time_index == 5)
//                            {
//                                /* Finish node with error,
//                                 * this way the current node will be retried if it's more attractive again */
//                                retval = FUNC_INCOMPLETE_HEAVY;
//                                break;
//                            }
//                        }
//                        else
//                        {
//                        	//estimated_GoTo_time_index = 0;
//							estimated_GoTo_time_history[estimated_GoTo_time_index] = estimated_GoTo_time;
//                        }

                        vTaskDelay(CAN_CHECK_DELAY/portTICK_RATE_MS);
                    }
                    while(estimated_GoTo_time > 0);

                    retval = FUNC_SUCCESS;
                }
                else
                {
                    retval = FUNC_ERROR;
                }
            }
            else  /* Enemy in range */
            {
                //TODO: Drive slower?

                /* STOPP */
                txStopDrive();

                /* Wait some time so the other tasks get proccess time (e.g. strategy) */
                vTaskDelay(500 / portTICK_RATE_MS);

                /* Don't drive */
                retval = FUNC_INCOMPLETE_HEAVY;
            }
        }
        else /* Route calculated */
        {
            /* Start driving forward */
            if(driveGoto(x, y, angle, speed, direction, GOTO_ROUTE, game_state))
            {
                retval = FUNC_SUCCESS;
            }
            else
            {
                retval = FUNC_ERROR;
            }

            /* check arrive drive for obstacles like enemys */
            if(retval == FUNC_SUCCESS)
            {
                /* Wait at least GOTO_STATERESP_DELAY before asking for goto time for the first time,
                 * else we may get the old time */
                vTaskDelay(GOTO_STATERESP_DELAY / portTICK_RATE_MS);

                do
                {
                    /* Ask drive system for GoTo state */
                    txGotoStateRequest();
                    /* Receive the GoTo state response */
                    CAN_ok = xQueueReceive(qGotoStateResp, &CAN_buffer, CAN_WAIT_DELAY / portTICK_RATE_MS);

                    if(CAN_ok == pdFALSE)
                    {
                        retval = FUNC_ERROR;
                        break;
                    }

                    /* Extract time */
                    estimated_GoTo_time = CAN_buffer.state_time;  /* In ms */

                    /* Handle "goto not possible at the moment" message */
                    if(estimated_GoTo_time == GOTO_NOT_POSSIBLE_ATM)
                    {
                        /* Finish node with error,
                         * this way the current node will be retried if it's more attractive again */
                        retval = FUNC_INCOMPLETE_HEAVY;
                        break;
                    }

                    /* Handle "goto blocked at the moment" message */
                    if(estimated_GoTo_time == GOTO_BLOCKED_ATM)
                    {
                        /* Finish node with error,
                         * this way the current node will be retried if it's more attractive again */
                        retval = FUNC_INCOMPLETE_HEAVY;
                        break;
                    }

                    /* Handle Drive-Node Bug */
//					if(estimated_GoTo_time == estimated_GoTo_time_history[estimated_GoTo_time_index])
//					{
//						estimated_GoTo_time_history[estimated_GoTo_time_index] = estimated_GoTo_time;
//						estimated_GoTo_time_index++;
//
//						if(estimated_GoTo_time_index == 5)
//						{
//							/* Finish node with error,
//							 * this way the current node will be retried if it's more attractive again */
//							retval = FUNC_INCOMPLETE_HEAVY;
//							break;
//						}
//					}
//					else
//					{
//						//estimated_GoTo_time_index = 0;
//						estimated_GoTo_time_history[estimated_GoTo_time_index] = estimated_GoTo_time;
//					}

                    /* Check if an enemy/confederate is within range in front of the robot */
                    range = isRobotInRange(game_state, FALSE);
                    if(range != 0)  /* Enemy in range */
                    {
                        /* set range flag */
                        is_in_range = 1;

                        /* Drive forward with speed relative to the range */
                        uint8_t calc_speed = distance2speed(range, speed);
                        if(calc_speed <= SPEED_OFFSET)
                        {
                            /* STOPP */
                            txStopDrive();


                            //TODO
                            ////////////////////////////////////////////////////////////////////
                            /* make possible to break the node task for funny action */
//							taskENTER_CRITICAL();
							/* Drive backward */
							if(driveGoto(IGNORED_VALUE, IGNORED_VALUE, IGNORED_VALUE, IGNORED_VALUE, GOTO_DRIVE_BACKWARD, GOTO_ROUTE, game_state))
							{
								/* Wait while driving */
								vTaskDelay(DRIVE_BACK_TIME / portTICK_RATE_MS);
							}
							else
							{
								retval = FUNC_ERROR;
							}
//							taskEXIT_CRITICAL();

                            //TODO
							/////////////////////////////////////////////////////////////

                            /* Don't drive */
                            retval = FUNC_INCOMPLETE_LIGHT;
                            break;
                        }
                        else
                        {
							if(driveGoto(x, y, angle, calc_speed, direction, GOTO_NO_ROUTE, game_state))
							{
								retval = FUNC_SUCCESS;
							}
							else
							{
								retval = FUNC_ERROR;
								break;
							}
                        }
                    }
                    else if(is_in_range)
                    {
                        /* reset flag */
                        is_in_range = 0;

                        /* not in range anymore */
                        if(driveGoto(x, y, angle, speed, direction, GOTO_NO_ROUTE, game_state))
                        {
                            retval = FUNC_SUCCESS;
                        }
                        else
                        {
                            retval = FUNC_ERROR;
                            break;
                        }
                    }

                    vTaskDelay(CAN_CHECK_DELAY/portTICK_RATE_MS);

                /* Repeat state request while not at target destination */
                } while(estimated_GoTo_time > 0);
            }
        }
    }
    else /* direction == GOTO_DRIVE_BACKRWARD */
    {
        /* Check if path blocked */
        range = isRobotInRange(game_state, TRUE);
        if((range == 0) || (range >= DRIVE_BACK_DIST + DIST_OFFSET))  /* No enemy in range */
        {
        	/* make possible to break the node task for funny action */
//        	taskENTER_CRITICAL();

            /* Drive backward */
            if(driveGoto(IGNORED_VALUE, IGNORED_VALUE, IGNORED_VALUE, IGNORED_VALUE, direction, GOTO_NO_ROUTE, game_state))
            {
                /* Wait while driving */
                vTaskDelay(DRIVE_BACK_TIME / portTICK_RATE_MS);
                retval = FUNC_SUCCESS;
            }
            else
            {
                retval = FUNC_ERROR;
            }
//            taskEXIT_CRITICAL();
        }
        else  /* Enemy in range */
        {
            /* Don't drive */
            retval = FUNC_INCOMPLETE_LIGHT;
        }
    }

    return retval;
}


/**
 *  \fn     driveGoto
 *  \brief  drive to an x/y position
 *
 *  \param[in]  x   x-position [mm]
 *  \param[in]  y   y-position [mm]
 *  \param[in]  angle
 *  \param[in]  speed
 *  \param[in]  direction
 *  \param[in]  route
 *  \param[in]  game_state
 *
 *  \retval 0   error
 *  \retval 1   success
 */
uint8_t driveGoto(uint16_t x, uint16_t y, uint16_t angle, uint8_t speed, uint8_t direction, uint8_t route, volatile game_state_t* game_state)
{
    uint8_t success = pdFALSE;
    uint8_t goto_retries = 0;
    uint8_t CAN_ok = pdFALSE;
    CAN_data_t CAN_buffer;
    uint16_t barrier;
    taskENTER_CRITICAL();
    barrier = game_state->barrier;
    taskEXIT_CRITICAL();

    /* send and check drive command */
    do
    {
        goto_retries++;

        /* drive to the heart */
        txGotoXY(x, y, angle, speed, barrier, direction, route);

        /* Receive GoTo confirmation */
        CAN_ok = xQueueReceive(qGotoConfirm, &CAN_buffer, CAN_WAIT_DELAY / portTICK_RATE_MS);

    }
    /* Retry if no transmission confirmed received and another retry is allowed */
    while((CAN_ok != pdTRUE) && (goto_retries <= CAN_MAX_RETRIES));

    if(CAN_ok == pdTRUE)
    {
        success = 1;
    }
    else
    {
        success = 0;
    }

    return success;
}


/*
 * \fn      placeSucker
 * \brief   moves the sucker to the right position
 *
 * \param[in]   position    the next position -> have to be deeper than the current one
 */
void placeSucker(uint16_t position)
{
    /* local variable */
    volatile uint16_t servo_pos; /* Variable to set the servo position step by step */

    /* Current position */
    servo_pos = getServo_1();

    if(servo_pos > position)
    {
        /* Move the sucker servo down a bit, step by step */
        while(servo_pos > (position+SERVO_STEP))
        {
            /* Decrement servo position by step size */
            servo_pos -= SERVO_STEP;

            /* Check if it's the last step */
            if(servo_pos < SERVO_POS_DOWN)
            {
                /* Set the final servo position without over-rotating */
                setServo_1(position);
            }
            else
            {
                /* Set the new servo position */
                setServo_1(servo_pos);
            }
            /* Wait some time while servo moves*/
            vTaskDelay(SERVO_STEP_DELAY / portTICK_RATE_MS);
        }
    }
    else
    {
        while(servo_pos < (SERVO_POS_UP+SERVO_STEP))
        {
            /* Decrement servo position by step size */
            servo_pos += SERVO_STEP;

            /* Check if it's the last step */
            if(servo_pos > SERVO_POS_UP)
            {
                /* Set the final servo position without over-rotating */
                setServo_1(SERVO_POS_UP);
            }
            else
            {
                /* Set the new servo position */
                setServo_1(servo_pos);
            }
            /* Wait some time while servo moves */
            vTaskDelay(SERVO_STEP_DELAY / portTICK_RATE_MS);
        }
    }
}

/**
 * @}
 */


