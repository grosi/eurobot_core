/**
 * \file    System.c
 * \author  gross10
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Task for the strategy
 *
 * \defgroup strategy Strategy
 * \brief   Strategy task
 * \ingroup app_task
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "AppConfig.h"
#include "system/RoboInitialisation.h"
#include "system/RoboSetup.h"
#include "system/RoboRun.h"
#include "system/RoboError.h"
#include "System.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
void (*system_state)(portTickType*); /* functionpointer to the current system-state */


/* Private function prototypes -----------------------------------------------*/
static void vSystemTask(void*);


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initStrategyTask
 * \brief       Initialisation of the strategy task
 *
 * \param[in]   None
 * \return      None
 */
void initSystemTask(void){

    /* create the task */
    xTaskCreate( vSystemTask, ( signed char * ) SYSTEM_TASK_NAME, SYSTEM_STACK_SIZE, NULL, SYSTEM_TASK_PRIORITY, NULL );

    /* init node resources */
    initNodeResources();

    /* init system states */
    initRoboInitialisationState();
    initRoboRunState();

    /* set start state */
    system_state = runRoboInitialisationState;

}


/**
 * \fn          vSystemTask
 * \brief       Task to handle the strategy
 *
 * \param[in]   None
 * \return      None
 */
static void vSystemTask(void* pvParameters )
{
    /* local variables */
    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* endless */
    for(;;)
    {
        system_state(&xLastFlashTime);
    }
}

/**
 * @}
 */
