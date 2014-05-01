/**
 * \file    RoboInitialisation.c
 * \author  gross10
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy init sate
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "../CANGatekeeper.h"
#include "../System.h"
#include "RoboSetup.h" /* next state if this one is completed successfully */
#include "RoboError.h" /* next state if this one is completed with errors */
#include "RoboInitialisation.h"

/* lib */
#include "lib/servo.h"
#include "lib/display.h"
#include "lib/sensor.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    DISPLAY_INIT = 0,
    DRIVE_INIT,
    NAVI_INIT,
    EMERGENCY_ACTIVE
}robot_init_t;


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static xQueueHandle qStateResponse;
static robot_init_t state; /* statemachine */

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboInitialisationState
 * \brief   initialisation of the RoboInitialisationState
 *
 * \note    have to be called from the initSystem function
 */
void initRoboInitialisationState()
{
    /* create the queue */
    qStateResponse = xQueueCreate(ROBOINIT_QUEUE_LENGTH,sizeof(CAN_data_t)); /* RX-Message Queue */

    /* set CAN listeners */
    setQueueCANListener(qStateResponse, CHECK_NAVI_RESPONSE);
    setQueueCANListener(qStateResponse, CHECK_DRIVE_RESPONSE);

    /* set first state */
    state = DISPLAY_INIT;
}


/**
 * \fn      setConfigRoboInitialisationState2Default
 * \brief   set all necessary values to default
 */
void setConfigRoboInitialisationState2Emergency()
{
    state = EMERGENCY_ACTIVE;
}


/**
 * \fn      runRoboInitialisationState
 * \brief   run the RoboInitialisationState
 *
 * \param[in]   tick    pointer to systick timer for absolut delays
 * \todo    display init and notstop
 */
void runRoboInitialisationState(portTickType* tick)
{
    /* local variable */
    CAN_data_t can_response;


    /* small statemachine */
    switch(state)
    {
        case DISPLAY_INIT:
            LCD_init((void(*)(long))vTaskDelay);

            state = DRIVE_INIT;

            break;

        case DRIVE_INIT:
            /* request a sign of live from the drive-node */
            txCheckDriveRequest();

            /* wait for max 1s */
            if(xQueueReceive(qStateResponse,&can_response,ROBOINIT_TIMEOUT / portTICK_RATE_MS) == pdTRUE)
            /* drive-node is okay -> goto to navi-node check */
            {
                state = NAVI_INIT;
            }
            /* drive-node is not alive -> goto RoboError state */
            else
            {
                system_state = runRoboErrorState;
            }
            break;

        case NAVI_INIT:
            /* request a sign of live from the drive-node */
            txCheckNaviRequest();

            /* wait for max 1s */
            if(xQueueReceive(qStateResponse,&can_response,ROBOINIT_TIMEOUT / portTICK_RATE_MS) == pdTRUE)
            /* navi-node is okay -> goto to next system-state */
            {
                system_state = runRoboSetupState;
            }
            /* navi-node is not alive -> goto RoboError state */
            else
            {
                system_state = runRoboErrorState;
            }
            break;

        case EMERGENCY_ACTIVE:
            /* wait until the notstop-switch is released */
            if(getSensor_EmergencyStop())
            {
                state = DISPLAY_INIT;
            }
            vTaskDelayUntil(tick, ROBOINIT_TIMEOUT / portTICK_RATE_MS);
            break;
    }
}

/**
 * @}
 */
