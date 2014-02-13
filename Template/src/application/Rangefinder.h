/**
 * \file    Rangefinder.h
 * \author  kasen1
 * \date    2014-01-14
 *
 * \version 1.0
 *  Softwared tested (14.01.2014)
 * \version 1.0
 *  import from template (14.01.2014)
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
#define RANGEFINDER_TASK_NAME_IR    "Rangefinder Infrarot Task"
#define RANGEFINDER_TASK_NAME_US    "Rangefinder Ultraschall Task"
#define RANGEFINDER_QUEUE_LENGTH    20      /* Size of the message queues */
#define RANGEFINDER_STACK_SIZE      configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define RANGEFINDER_TASK_PRIORITY   (configMAX_PRIORITIES - 4UL) /* priority 4*/

/* Rangefinder settings */ //TODO: Adjust values
#define RANGEFINDER_RANGE           100     /* Maximum detection range in cm (int16_t values from 5 to 1100) */
#define RANGEFINDER_THRESHOLD_FW    30      /* Minimum allowed distance in cm to an obstacle (in front) without alert*/
#define RANGEFINDER_THRESHOLD_BW    30      /* Minimum allowed distance in cm to an obstacle (in back) without alert*/
#define RANGEFINDER_DELAY           100     /* Task delay in ms between measures */

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/
/* Alarm flags (pleas read only!), TRUE if object detected, FALSE if no object detected */
extern bool RangefinderIR_FwAlarm_flag;     /* Infrared forward alarm */
extern bool RangefinderIR_BwAlarm_flag;     /* Infrared backward alarm */
extern bool RangefinderUS_FwAlarm_flag;     /* Ultrasonic forward alarm */
extern bool RangefinderUS_BwAlarm_flag;     /* Ultrasonic backward alarm */

/* exported function prototypes -----------------------------------------------*/
extern void initRangefinderTasks(void);

#endif /* RANGEFINDER_TASK_H_ */

/**
 * @}
 */
