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
#include "Rangefinder.h"
#include "CANGatekeeper.h"
#include "system/RoboInitialisation.h"
#include "system/RoboSetup.h"
#include "system/RoboRun.h"
#include "system/RoboError.h"
#include "System.h"

/* lib */
#include "../lib/display.h"
#include "../lib/sensor.h"
#include "../lib/exti_sensor.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
void (*system_state)(portTickType*); /* functionpointer to the current system-state */
xSemaphoreHandle sSyncEmergencyStopRoboState; /* semaphore for emergencystop sync */


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

    /* create sync semaphore */
    vSemaphoreCreateBinary(sSyncEmergencyStopRoboState);
    xSemaphoreTake(sSyncEmergencyStopRoboState, 0);

    /* init system states */
    initRoboInitialisationState();
    initRoboSetupState();
    initRoboRunState();
    initRoboErrorState();

#ifndef STANDALONE
    /* init system hw */
    initSensor_EmergencyStop();
    initSensorEXTI_EmergencyStop();
    initSensor_Key();
#endif

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
 * \fn      SystemStop
 * \brief   stop the system if the game-timer runs out
 */
void SystemStop(void)
{
    /* stay in ErrorState if there */
    if(system_state != runRoboErrorState)
    {
        /* set correct state in initialisation state */
        setConfigRoboSetup2Default();
        setConfigRoboRunState2Default();
        setNodeConfig2Default();

        /* hw */
        suspendRangefinderTask();
        txEmergencyStop(0);

        system_state = runRoboInitialisationState;
    }
}


/**
 * \fn      SystemReset
 * \brief   reset the system by emergency stop
 * \note    could call asynchronous from ISR
 */
void SystemReset(void)
{
    /* stay in ErrorState if there */
    if(system_state != runRoboErrorState)
    {
        /* set correct state in initialisation state */
        setConfigRoboInitialisationState2Emergency();
        setConfigRoboSetup2Default();
        setNodeConfig2Default();
        suspendRangefinderTask();

        system_state = runRoboInitialisationState;
    }
}

/**
 * \fn      EmergencyStop_Handler
 * \brief   catch the emergency stop interrupt
 */
void EmergencyStop_Handler(void)
{
    static signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(sSyncEmergencyStopRoboState, &xHigherPriorityTaskWoken);
    SystemReset();

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @}
 */
