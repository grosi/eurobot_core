/**
 * \file    Strategy.h
 * \author  meert1
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

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef Strategy_TASK_H_
#define Strategy_TASK_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* Strategy-task RTOS configuration */
#define STRATEGY_TASK_NAME          "Strategy Task"
#define STRATEGY_QUEUE_LENGTH       20 /*!< Size of the message queues */
#define STRATEGY_STACK_SIZE         configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define STRATEGY_TASK_PRIORITY      (configMAX_PRIORITIES - 5UL) /*!< priority of the receive and transmit task */


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initStrategyTask(void);


#endif /* STRATEGY_TASK_H_ */

/**
 * @}
 */
