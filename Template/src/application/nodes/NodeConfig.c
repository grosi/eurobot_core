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
        .time = 2,                          	/*!<estimated node time [s]*/
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
        .time = 2,                          	/*!<estimated node time [s]*/
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
        .time = 2,                          	/*!<estimated node time [s]*/
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 900,                          /*!<node x position [mm]*/
        .y = 900+200,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
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
        .time = 2,                          /*!<estimated node time [s]*/
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
        .time = 2,                          /*!<estimated node time [s]*/
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
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
        .time = 2,                          	/*!<estimated node time [s]*/
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
        .time = 2,                          	/*!<estimated node time [s]*/
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
        .time = 2,                          	/*!<estimated node time [s]*/
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
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
        .time = 2,                          	/*!<estimated node time [s]*/
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
        .time = 2,                          /*!<estimated node time [s]*/
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
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 2700,                         /*!<node arrive direction */
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
        .points = 1,                        /*!<node points */
        .percent = 0.05,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1950,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 270,                         /*!<node arrive direction */
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
 * \retval 0   error
 * \retval 1   success
 */
uint8_t checkDrive(uint16_t x, uint16_t y, uint16_t angle, uint8_t speed, uint8_t direction, volatile game_state_t* game_state)
{
    CAN_data_t CAN_buffer;
    uint16_t estimated_GoTo_time = 0;
    uint8_t success = 1;
    uint8_t CAN_ok;
    uint8_t retries = 0;

    success = driveGoto(x,y,angle,speed,direction,game_state);

    /* check arrive drive for barriers like enemys */
    if(success)
    {
        do
        {
            /* Wait at least GOTO_STATERESP_DELAY before asking for goto time for the first time,
             * else we may get the old time */
            if(estimated_GoTo_time < 20) {
                vTaskDelay(20 / portTICK_RATE_MS);
            }

            /* Ask drive system for GoTo state */
            txGotoStateRequest();
            /* Receive the GoTo state response */
            CAN_ok = xQueueReceive(qGotoStateResp, &CAN_buffer, CAN_WAIT_DELAY / portTICK_RATE_MS);

            if(CAN_ok == pdFALSE)
            {
                success = 0;
                break;
            }

            /* Extract time */
            estimated_GoTo_time = CAN_buffer.state_time;  /* In ms */

            /* Handle "time unknown" message */
            if(estimated_GoTo_time == 0xFFFFFF)
            {
                /* Finish node with error,
                 * this way the current node will be retried if it's more attractive again */
                success = 0;
                break;
            }

            /* Check if an enemy/confederate is within range in front of the robot */
            if(direction == GOTO_DRIVE_FORWARD)
            {
                if(isRobotInRange(game_state, FALSE))
                {
                    /* STOPP */
                    txStopDrive();

                    /* recalculate route */
                    driveGoto(x, y, angle, speed, direction, game_state);
                    retries++;

                    if(retries >= 5)
                    {
                        success = 0;
                        break;
                    }
                }
            }
            else
            {
                //TODO: isRobotInBack()
//                if(isRobotInFront(game_state))
//                {
//                    /* STOPP */
//                    txStopDrive();
//
//                    /* recalculate route */
//                    driveGoto(x, y, angle, speed, direction, game_state);
//                }
            }


            vTaskDelay(CAN_CHECK_DELAY/portTICK_RATE_MS);

        /* Repeat while not at target destination */
        } while(estimated_GoTo_time != 0);
    }

    return success;
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

    /* send and check drive command */
    do
    {
        goto_retries++;

        /* drive to the heart */
        txGotoXY(x, y, angle, speed, game_state->barrier, direction);

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


