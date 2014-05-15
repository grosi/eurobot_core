/**
 * \file    RoboRunExtend.c
 * \author  gross10
 * \date    2014-05-08
 *
 * \version 1.0
 *  create this file
 *
 * \brief   funncy action state
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "../Timer.h"
#include "../ELP.h"
#include "../System.h"
#include "../nodes/NodeConfig.h"
#include "RoboInitialisation.h" /* next state if this one is completed successfully */
#include "RoboError.h" /* next state if this one is completed with errors */
#include "RoboRunExtend.h"

/* lib */
#include "lib/servo.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboRunExtendState
 * \brief   init things for this state
 */
void initRoboRunExtendState()
{

}


/**
 * \fn      runRoboRunExtendState
 * \brief   system state for the funny action
 *
 * \param   tick    tick for cycle delays
 */
void runRoboRunExtendState(portTickType* tick)
{
	uint8_t remaining_time = getRemainingGameTime();

    /* check if the 90s are over */
    if(remaining_time == 0)
    {
        setServo_2(SERVO_POS_NET_LAUNCH);
        vTaskDelay(1000/portMAX_DELAY);
        setServo_2(SERVO_POS_NET_LOAD);

        /* re-init the complete system */
        stopGameTimer();
        system_state = runRoboInitialisationState;
    }
    /* wait until the game is finished */
    else
    {
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}


/**
 * @}
 */

