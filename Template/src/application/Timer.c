/**
 * \file    Timer.h
 * \author  meert1
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Task to watch the gametime (90s)
 *
 * \defgroup timer Timer
 * \brief   Timer task
 * \ingroup app_task
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* RTOS */
#include <memPoolService.h>         /* Memory pool manager service */

/* application */
#include "AppConfig.h"
#include "CANGatekeeper.h"
#include "Timer.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static uint8_t elapsedTime = 0;

boolean timerEnabled = FALSE;


/* Private function prototypes -----------------------------------------------*/
static void vTimerTask(void*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initTimerTask
 * \brief       Initialisation of the Timer Task
 *
 * \param[in]   None
 * \return      None
 */
void initTimerTask(void){

    /* create the task */
    xTaskCreate( vTimerTask, ( signed char * ) TIMER_TASK_NAME, TIMER_STACK_SIZE, NULL, TIMER_TASK_PRIORITY, NULL );

}

/**
 * \fn          vTimerTask
 * \brief       Watch the 90 sec of the game and stop the robot if the time is up
 *
 * \param[in]   None
 * \return      None
 */
static void vTimerTask(void* pvParameters ) {

    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 / portTICK_RATE_MS);

    for(;;)
    {
        /* Wait until the timer is started */
        while(timerEnabled)
        {
            /* wait */
            vTaskDelayUntil( &xLastFlashTime, 1000 / portTICK_RATE_MS);

            /* Check if the time is up */
            if(elapsedTime == TIMER_STOP_TIME - 1)
            {
                /* Stop drive and shut down the actors */
                txStopDrive();
                /** \todo Stop the strategy task */
            }
            else
            {
                /* Increase the second timer */
                elapsedTime++;
            }
        }

        /* If the Timer isn't started yet wait 10 ms */
        vTaskDelayUntil( &xLastFlashTime, 10 / portTICK_RATE_MS);

    }
}


/**
 * \fn          startTimer
 * \brief       starts the countdown-timer
 *
 * \param[in]   None
 * \return      None
 */
void startTimer(void)
{
    /* Start the timer */
    timerEnabled = TRUE;
}


/**
 * \fn          getRemainingTime
 * \brief       returns the remaining time until 0
 *
 * \param[in]   None
 * \return      remaining time in seconds
 */
uint8_t getRemainingTime(void)
{
    /* Return the remaining time of the game */
    return TIMER_STOP_TIME - elapsedTime;
}

/**
 * @}
 */
