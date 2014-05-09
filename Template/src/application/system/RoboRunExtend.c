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
#include "RoboSetup.h" /* next state if this one is completed successfully */
#include "RoboError.h" /* next state if this one is completed with errors */
#include "RoboRunExtend.h"

/* lib */
#include "lib/display.h"
#include "lib/sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

void initRoboRunExtendState()
{

}

void runRoboRunExtendState(portTickType* tick)
{
    /* check if the 90s are over */
    if(getRemainingGameTime() == 0)
    {
        //TODO Netz
    }
    /* wait until the game is finished */
    else
    {
        vTaskDelay(getRemainingGameTime()*1000);
    }
}


/**
 * @}
 */

