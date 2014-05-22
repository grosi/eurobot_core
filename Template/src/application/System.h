/**
 * \file    System.h
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

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef SYSTEM_H_
#define SYSTEM_H_

/* exported typedef -----------------------------------------------------------*/

/* exported define ------------------------------------------------------------*/
/* Strategy-task RTOS configuration */
#define SYSTEM_TASK_NAME        "System Task"
#define SYSTEM_STACK_SIZE       4*configMINIMAL_STACK_SIZE
#define SYSTEM_TASK_PRIORITY    (configMAX_PRIORITIES - 4UL) /*!< priority 3 */


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
extern void (*system_state)(portTickType*);
extern xSemaphoreHandle sSyncEmergencyStopRoboState;


/* exported function prototypes -----------------------------------------------*/
extern void initSystemTask(void);
extern void SystemStop(void);
extern void SystemReset(void);
extern void EmergencyStop_Handler(void);


#endif /* SYSTEM_H_ */

/**
 * @}
 */
