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
#define TIMER_STACK_SIZE            configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */

#define TIMER_PERIODE               1000 /*!< Time between to interruptions in ms */
#define TIMER_STOP_TIME             90 /*!< Time in sec to stop */


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initTimerTask(void);
extern uint8_t getRemainingTime(void);
extern inline void startTimer(void);
extern inline void stopTimer(void);
extern inline void resetTimer(void);


#endif /* TIMER_TASK_H_ */

/**
 * @}
 */
