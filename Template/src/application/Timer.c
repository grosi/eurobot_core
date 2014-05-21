/**
 * \file    Timer.h
 * \author  meert1, gross10
 * \date    2014-02-14
 *
 * \version 1.1
 *  task replaced with a sw-timer
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
/* application */
#include "AppConfig.h"
#include "CANGatekeeper.h"
#include "System.h"
#include "Timer.h"
#include "system/RoboRun.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static xTimerHandle xGameTimer;
static volatile uint8_t elapsedTime = 0;


/* Private function prototypes -----------------------------------------------*/
void vGameTimerCallback(xTimerHandle);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initTimerTask
 * \brief       Initialisation of the Timer Task
 *
 * \param[in]   None
 * \return      None
 */
void initGameTimer(void)
{
    /* create sw-timer */
    xGameTimer = xTimerCreate(( signed char * )TIMER_NAME,
                            TIMER_PERIODE / portTICK_RATE_MS,
                            pdTRUE,
                            NULL,
                            vGameTimerCallback);
}


/**
 * \fn          vGameTimerCallback
 * \brief       Watch the 90 sec of the game and stop the robot if the time is up
 *
 * \param[in]   None
 * \return      None
 */
void vGameTimerCallback(xTimerHandle pxTimer)
{
    /* funny action break */
    if(elapsedTime >= PLAY_TIME_FUNNY)
    {
        setFunnyBreak();

        //TODO: always vGameTimerCallback is called or only once?
    }

    /* Check if the time is up */
    if(elapsedTime >= PLAY_TIME_TOTAL - 1)
    {
        /* Stop drive and shut down the actors */
        xTimerStop(xGameTimer,0);
        txStopDrive();

        /* Stop the strategy task */
        SystemStop();
    }
    else
    {
        /* Increase the second timer */
        elapsedTime++;
    }
}


/**
 * \fn          startTimer
 * \brief       starts the countdown-timer
 *
 * \param[in]   None
 * \return      None
 */
inline void startGameTimer(void)
{
    /* Start the timer */
    xTimerStart(xGameTimer,0);
}


/**
 * \fn          stopTimer
 * \brief       stops the countdown-timer
 *
 * \param[in]   None
 * \return      None
 */
inline void stopGameTimer(void)
{
    /* Stop the timer */
    xTimerStop(xGameTimer,0);
}


/**
 * \fn          resetTimer
 * \brief       resets the countdown-timer to 90s
 *
 * \param[in]   None
 * \return      None
 */
inline void resetGameTimer(void)
{
    /* Reset the timer */
    elapsedTime = 0;
}


/**
 * \fn          getRemainingTime
 * \brief       returns the remaining time until 0
 *
 * \param[in]   None
 * \return      remaining time in seconds
 */
uint8_t getRemainingGameTime(void)
{
	uint8_t remaining_time;

    /* Return the remaining time of the game */
	if(elapsedTime >= PLAY_TIME_TOTAL)
	{
		remaining_time = 0;
	}
	else
	{
		remaining_time = PLAY_TIME_TOTAL - elapsedTime;
	}
	return remaining_time;
}

/**
 * @}
 */
