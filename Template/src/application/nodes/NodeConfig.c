/**
 * \file    NodeConfig.c
 * \author  gross10, kasen1
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   config file for the game nodes
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
#include "../CANGatekeeper.h"
#include "../Rangefinder.h"
#include "NodeConfig.h"
#include "FireNode.h"
#include "FireWallInversNode.h"
#include "FireWallNormalNode.h"
#include "FirePoolNode.h"
#include "NetNode.h"


/* lib */
#include "../../lib/servo.h"
#include "../../lib/sensor.h"
#include "../../lib/exti_sensor.h"
#include "../../lib/air.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define GOTOCONFIRM_QUEUE_LENGTH   1
#define GOTOSTATERESP_QUEUE_LENGTH 1


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* CAN */
xQueueHandle qGotoConfirm;
xQueueHandle qGotoStateResp;


/* node fire 1 red*/
node_t node_fire_1_red =
{
    {
        .id = 1,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.05,                    	/*!<percent of the total points [%]*/
        .time = 4,                          	/*!<estimated node time [s]*/
        .x = 900 - FIRE_APPROACH_DISTANCE, 	/*!<node x position [mm]*/
        .y = 1400,                          	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 0,                         	/*!<node arrive direction */
        .node_tries = 1,                   		/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 2 red*/
node_t node_fire_2_red =
{
    {
        .id = 2,                           		/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.05,                    	/*!<percent of the total points [%]*/
        .time = 4,                          	/*!<estimated node time [s]*/
        .x = 400,                           	/*!<node x position [mm]*/
        .y = 900 + FIRE_APPROACH_DISTANCE,	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 270,                        	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 3 red*/
node_t node_fire_3_red =
{
    {
        .id = 3,                           		/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.05,                    	/*!<percent of the total points [%]*/
        .time = 4,                          	/*!<estimated node time [s]*/
        .x = 900 + FIRE_APPROACH_DISTANCE,   /*!<node x position [mm]*/
        .y = 400,                           	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 180,                       	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node wall fire pool red*/
node_t node_fire_pool_red =
{
    {
        .id = 4,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.23,                    /*!<percent of the total points [%]*/
        .time = 30,                         /*!<estimated node time [s]*/
        .x = 1200,                          /*!<node x position [mm]*/
        .y = 1200,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 225,                       /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFirePoolNode
};

/* node wall fire normal 1 red*/
node_t node_fire_wall_normal_1_red =
{
    {
        .id = 5,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 6,                          /*!<estimated node time [s]*/
        .x = 0 + FIREWALL_APPROACHDISTANCE,	/*!<node x position [mm]*/
        .y = 1200,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 180,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire normal 2 red*/
node_t node_fire_wall_normal_2_red =
{
    {
        .id = 6,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 6,                          /*!<estimated node time [s]*/
        .x = 1700,                          /*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACHDISTANCE, /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire invers red*/
node_t node_fire_wall_invers_red =
{
    {
        .id = 7,                            /*!<node id */
        .points = 2,                        /*!<node points */
        .percent = 0.11,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1300,                          /*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACHDISTANCE, /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallInversNode
};


/* node net 1 red*/
node_t node_net_1_red =
{
    {
        .id = 8,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.35,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 400,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 250,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};

/* node net 2 red*/
node_t node_net_2_red =
{
    {
        .id = 9,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.35,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 700,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};

/* node net 3 red*/
node_t node_net_3_red =
{
    {
        .id = 10,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.35,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1000,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 290,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};



/* yellow */
/* node fire 1 yellow*/
node_t node_fire_1_yellow =
{
    {
        .id = 1,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.05,                    	/*!<percent of the total points [%]*/
        .time = 4,                          	/*!<estimated node time [s]*/
        .x = 2100 + FIRE_APPROACH_DISTANCE,  /*!<node x position [mm]*/
        .y = 1400,                          	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 180,                         	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 2 yellow*/
node_t node_fire_2_yellow =
{
    {
        .id = 2,                           		/*!<node id */
        .points = 1,                       	 	/*!<node points */
        .percent = 0.05,                    	/*!<percent of the total points [%]*/
        .time = 4,                          	/*!<estimated node time [s]*/
        .x =  2600,                          	/*!<node x position [mm]*/
        .y = 900 + FIRE_APPROACH_DISTANCE,	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 90,                        	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 3 yellow*/
node_t node_fire_3_yellow =
{
    {
        .id = 3,                           		/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.05,                    	/*!<percent of the total points [%]*/
        .time = 4,                          	/*!<estimated node time [s]*/
        .x = 2100 - FIRE_APPROACH_DISTANCE,	/*!<node x position [mm]*/
        .y = 400,                           	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 0,                       		/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};


/* node wall fire pool red*/
node_t node_fire_pool_yellow =
{
    {
        .id = 4,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.23,                    /*!<percent of the total points [%]*/
        .time = 30,                          /*!<estimated node time [s]*/
        .x = 1800,                          /*!<node x position [mm]*/
        .y = 1200,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 315,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFirePoolNode
};

/* node wall fire normal 1 yellow*/
node_t node_fire_wall_normal_1_yellow =
{
    {
        .id = 5,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.05,                    	/*!<percent of the total points [%]*/
        .time = 6,                          	/*!<estimated node time [s]*/
        .x = 3000 - FIREWALL_APPROACHDISTANCE,  /*!<node x position [mm]*/
        .y = 1200,                          	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 0,                         	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire normal 2 yellow*/
node_t node_fire_wall_normal_2_yellow =
{
    {
        .id = 6,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 6,                          /*!<estimated node time [s]*/
        .x = 1300,                          /*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACHDISTANCE, /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire invers yellow*/
node_t node_fire_wall_invers_yellow =
{
    {
        .id = 7,                            /*!<node id */
        .points = 1,                        /*!<node points */
        .percent = 0.11,                    /*!<percent of the total points [%]*/
        .time = 15,                          /*!<estimated node time [s]*/
        .x = 1700,                          /*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACHDISTANCE, /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doFireWallInversNode
};


/* node net 1 yellow*/
node_t node_net_1_yellow =
{
    {
        .id = 8,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.35,                    /*!<percent of the total points [%]*/
        .time = 30,                          /*!<estimated node time [s]*/
        .x = 2000,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 250,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};

/* node net 2 yellow*/
node_t node_net_2_yellow =
{
    {
        .id = 9,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.35,                    /*!<percent of the total points [%]*/
        .time = 30,                          /*!<estimated node time [s]*/
        .x = 2300,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};

/* node net 3 yellow*/
node_t node_net_3_yellow =
{
    {
        .id = 10,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.35,                    /*!<percent of the total points [%]*/
        .time = 30,                          /*!<estimated node time [s]*/
        .x = 2600,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 290,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};



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
	setServo_1(SERVO_POS_AIR_UP);
	/* Initialise launcher servo and set start position */
	initServo_2();
	setServo_2(SERVO_POS_NET_LOAD);

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
    setServo_1(SERVO_POS_AIR_UP);
    setServo_2(SERVO_POS_NET_LOAD);
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
	if(distance <= 50)  //TODO DIST_OFFSET
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
	uint16_t estimated_GoTo_time;
	func_report_t retval;
	uint8_t CAN_ok;
	int16_t delta_x, delta_y;
	uint16_t distance;

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

		if(distance <= 150) //TODO DRIVE_ROUTE_DIST_MIN /* No route calculation, just driving */
		{
			if(isRobotInRange(game_state, FALSE) < 150 + 50) //TODO DRIVE_ROUTE_DIST_MIN + DIST_OFFSET
			{
				/* Don't drive */
				retval = FUNC_INCOMPLETE_LIGHT;
			}
			else
			{
				uint8_t calc_speed = distance2speed(distance, speed);
				if(calc_speed <= 0)
				{
					/* Don't drive */
					retval = FUNC_INCOMPLETE_LIGHT;
				}
				else
				{
					/* Drive forward with speed relative to the distance */
					if(driveGoto(x, y, angle, calc_speed, direction, game_state))
					{
						retval = FUNC_SUCCESS;
					}
					else
					{
						retval = FUNC_ERROR;
					}
				}
			}

		}
		else /* Route calculated */
		{
			/* Start driving forward */
			if(driveGoto(x, y, angle, speed, direction, game_state))
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
				vTaskDelay(100 / portTICK_RATE_MS);//TODO: GOTO_STATERESP_DELAY / portTICK_RATE_MS);

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
						txStopDrive();  //TODO: Necessary?
						retval = FUNC_INCOMPLETE_HEAVY;
						break;
					}

					/* Check if an enemy/confederate is within range in front of the robot */
					if(isRobotInRange(game_state, FALSE))
					{
						/* STOPP */
						txStopDrive();

						retval = FUNC_INCOMPLETE_LIGHT;
						break;
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
		if(isRobotInRange(game_state, TRUE) < 50 + 50) //TODO DRIVE_BACK_DIST + DIST_OFFSET
		{
			/* Don't drive */
			retval = FUNC_INCOMPLETE_LIGHT;
		}
		else
		{
			/* Drive backward */
			if(driveGoto(x, y, angle, speed, direction, game_state))
			{
				retval = FUNC_SUCCESS;
			}
			else
			{
				retval = FUNC_ERROR;
			}
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
 *  \param[in]  game_state
 *
 *  \retval 0   error
 *  \retval 1   success
 */
uint8_t driveGoto(uint16_t x, uint16_t y, uint16_t angle, uint8_t speed, uint8_t direction, volatile game_state_t* game_state)
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
        txGotoXY(x, y, angle, speed, barrier, direction);

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
        while(servo_pos > (position+SERVO_AIR_STEP))
        {
            /* Decrement servo position by step size */
            servo_pos -= SERVO_AIR_STEP;

            /* Check if it's the last step */
            if(servo_pos < SERVO_POS_AIR_DOWN)
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
            vTaskDelay(SERVO_AIR_STEP_DELAY / portTICK_RATE_MS);
        }
    }
    else
    {
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
}



/**
 * @}
 */


