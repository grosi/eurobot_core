/**
 * \file    DefaultTask.c
 * \author  gross10
 * \date    2013--11-15
 *
 * \version 1.0
 *  create this file
 *
 * \brief   test task with temporary code sequences
 *
 * \defgroup default default_task
 * \brief   test task
 * \ingroup app_task
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef DEFAULTTASK_H_
#define DEFAULTTASK_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* default-task RTOS configuration */
#define DEFAULT_TASK_NAME           "Default Task"
#define TEST_QUEUE_LENGTH           20 /* Size of the message queues */
#define DEFAULT_STACK_SIZE          configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define DEFAULT_TASK_PRIORITY       (configMAX_PRIORITIES - 6UL) /* priority of default task -> 1 = low */



/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initDefaultTask(void);
extern inline void deleteDefaultTask(void);


#endif /* DEFAULTTASK_H_ */

/**
 * @}
 */
