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
    SENSOR_TEST,
    DRIVE_INIT,
    NAVI_INIT,
    EMERGENCY_ACTIVE_MESSAGE,
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
    state = EMERGENCY_ACTIVE_MESSAGE;
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
    boolean sensor_done = TRUE;
    static uint8_t sensor_error = 0;
    static uint8_t sensor_old;


    /* small statemachine */
    switch(state)
    {
        /* first init state */
        case DISPLAY_INIT:
            LCD_init((void(*)(long))vTaskDelay);

            /* plot message */
            LCD_write_string(MESSAGE_INIT_ROW, MESSAGE_INIT_COLUMN, MESSAGE_INIT, TRUE);
            LCD_write_string(MESSAGE_WAIT_ROW, MESSAGE_WAIT_COLUMN, MESSAGE_WAIT, TRUE);
#ifndef STANDALONE
            state = SENSOR_TEST;
#else
            system_state = runRoboSetupState;
#endif

            break;

        case SENSOR_TEST:


            /* test every sensor input */
            if(getSensor_Fresco_1() != SENSOR_FRESCO_1_INIT)
            {
                /* error message */
                if(!(sensor_error & 1<<0))
                {
                    LCD_write_string(MESSAGE_FRESCO_1_ROW, MESSAGE_FRESCO_1_COLUMN, MESSAGE_FRESCO_1, TRUE);
                    LCD_write_string(MESSAGE_CHECK_ROW, MESSAGE_CHECK_COLUMN, MESSAGE_CHECK, TRUE);
                    sensor_old = sensor_error |= 1<<0;
                }
                sensor_done = FALSE;
            }
            else
            {
                sensor_error &= ~(1<<0);
                if(sensor_old ^ sensor_error)
                {
                    sensor_old = sensor_error = 0;
                }
            }

            if(getSensor_Fresco_2() != SENSOR_FRESCO_2_INIT)
            {
                if(!(sensor_error & 1<<1))
                {
                    LCD_write_string(MESSAGE_FRESCO_2_ROW, MESSAGE_FRESCO_2_COLUMN, MESSAGE_FRESCO_2, TRUE);
                    LCD_write_string(MESSAGE_CHECK_ROW, MESSAGE_CHECK_COLUMN, MESSAGE_CHECK, TRUE);
                    sensor_old = sensor_error |= 1<<1;
                }
                sensor_done = FALSE;
            }
            else
            {
                sensor_error &= ~(1<<1);
                if(sensor_old ^ sensor_error)
                {
                    sensor_old = sensor_error = 0;
                }
            }

            if(getSensor_Fresco_Wall() != SENSOR_WALL_INIT)
            {
                if(!(sensor_error & 1<<2))
                {
                    LCD_write_string(MESSAGE_WALL_ROW, MESSAGE_WALL_COLUMN, MESSAGE_WALL, TRUE);
                    LCD_write_string(MESSAGE_CHECK_ROW, MESSAGE_CHECK_COLUMN, MESSAGE_CHECK, TRUE);
                    sensor_old = sensor_error |= 1<<2;
                }
                sensor_done = FALSE;
            }
            else
            {
                sensor_error &= ~(1<<2);
                if(sensor_old ^ sensor_error)
                {
                    sensor_old = sensor_error = 0;
                }
            }

            if(getSensor_EmergencyStop() != SENSOR_EMERGENCY_INIT)
            {
                if(!(sensor_error & 1<<3))
                {
                    LCD_write_string(MESSAGE_EMERGENCY_ROW, MESSAGE_EMERGENCY_COLUMN, MESSAGE_EMERGENCY, TRUE);
                    LCD_write_string(MESSAGE_CHECK_ROW, MESSAGE_CHECK_COLUMN, MESSAGE_CHECK, TRUE);
                    sensor_old = sensor_error |= 1<<3;
                }
                sensor_done = FALSE;
            }
            else
            {
                sensor_error &= ~(1<<3);
                if(sensor_old ^ sensor_error)
                {
                    sensor_old = sensor_error = 0;
                }
            }

            if(getSensor_Key() != SENSOR_KEY_INIT)
            {
                if(!(sensor_error & 1<<4))
                {
                    LCD_write_string(MESSAGE_KEY_ROW, MESSAGE_KEY_COLUMN, MESSAGE_KEY, TRUE);
                    LCD_write_string(MESSAGE_CHECK_ROW, MESSAGE_CHECK_COLUMN, MESSAGE_CHECK, TRUE);
                    sensor_old = sensor_error |= 1<<4;
                }
                sensor_done = FALSE;
            }
            else
            {
                sensor_error &= ~(1<<4);
                if(sensor_old ^ sensor_error)
                {
                    sensor_old = sensor_error = 0;
                }
            }

            /* every sensor was okay */
            if(sensor_done == TRUE)
            {
                state = DRIVE_INIT;
            }

            /* state delay for sensor debouncing */
            vTaskDelayUntil(tick, SETUP_BUTTON_DELAY / portTICK_RATE_MS);

            break;

        case DRIVE_INIT:
            /* request a sign of live from the drive-node (wait 1s before) */
            vTaskDelay(ROBOINIT_TIMEOUT);
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
                /* error message */
                LCD_write_string(MESSAGE_ERROR_ROW, MESSAGE_ERROR_COLUMN, MESSAGE_ERROR, TRUE);
                LCD_write_string(MESSAGE_RESTART_ROW, MESSAGE_RESTART_COLUMN, MESSAGE_RESTART, TRUE);

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
                state = SENSOR_TEST; /* reset state */
            }
            /* navi-node is not alive -> goto RoboError state */
            else
            {
                /* error message */
                LCD_write_string(MESSAGE_ERROR_ROW, MESSAGE_ERROR_COLUMN, MESSAGE_ERROR, TRUE);
                LCD_write_string(MESSAGE_RESTART_ROW, MESSAGE_RESTART_COLUMN, MESSAGE_RESTART, TRUE);

                system_state = runRoboErrorState;
            }
            break;

        case EMERGENCY_ACTIVE_MESSAGE:
            /* message */
            LCD_write_string(MESSAGE_EMERGENCY_ROW,MESSAGE_EMERGENCY_COLUMN,MESSAGE_EMERGENCY,TRUE);
            LCD_write_string(MESSAGE_CHECK_ROW,MESSAGE_CHECK_COLUMN,MESSAGE_CHECK,TRUE);

            /* send message over CAN for better break-enhancement */
            txEmergencyStop(0);

            state = EMERGENCY_ACTIVE;
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
