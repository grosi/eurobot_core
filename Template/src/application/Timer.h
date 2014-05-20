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

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef Timer_TASK_H_
#define Timer_TASK_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* Timer-task RTOS configuration */
#define TIMER_NAME                  "Game Timer"

#define TIMER_PERIODE               1000 /*!< Time between to interruptions in ms */
#define TIMER_STOP_TIME             90 /*!< Time in sec to stop */
#define TIMER_FUNNY_TIME            75 /*!< time barrier for funny action*/


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initGameTimer(void);
extern uint8_t getRemainingGameTime(void);
extern inline void startGameTimer(void);
extern inline void stopGameTimer(void);
extern inline void resetGameTimer(void);


#endif /* TIMER_TASK_H_ */

/**
 * @}
 */
