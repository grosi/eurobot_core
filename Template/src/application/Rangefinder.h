/**
 * \file    Rangefinder.c
 * \author  kasen1
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   task for the rangefinder sensors
 *
 * \defgroup rangefinder Rangefinder
 * \brief   Rangefinder task
 * \ingroup app_task
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef RANGEFINDER_TASK_H_
#define RANGEFINDER_TASK_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* Rangefinder-task RTOS configuration */
#define RANGEFINDER_TASK_NAME           "Rangefinder Task"
#define RANGEFINDER_QUEUE_LENGTH        20 /*!< Size of the message queues */
#define RANGEFINDER_STACK_SIZE          configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define RANGEFINDER_TASK_PRIORITY       (configMAX_PRIORITIES - 4UL) /*!< priority 4*/


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initRangefinderTask(void);


#endif /* RANGEFINDER_TASK_H_ */

/**
 * @}
 */
