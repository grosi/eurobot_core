/**
 * \file    Rangefinder.h
 * \author  kasen1
 * \date    2014-01-14
 *
 * \version 1.2
 *  - IR sensors in new arrangement
 *  - Added flag for separation blocked alarm
 * \version 1.1
 *  - Changed implementation of IR detection to external interrupt
 *  - Implemented comparison of last three US measures
 *  - Fixed I2C error, where the bus was always busy after disconnecting a slave
 *  - Software tested (12.03.2014)
 * \version 1.0
 *  Software tested (14.01.2014)
 * \version 0.1
 *  Import from template (14.01.2014)
 *
 * \brief   task for the rangefinder sensors
 *
 * \todo     Unit test for version 1.2 on robot
 *
 * \defgroup rangefinder Rangefinder
 * \brief   Rangefinder task
 * \ingroup hw_task
 *
 * @{
 */
/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef RANGEFINDER_TASK_H_
#define RANGEFINDER_TASK_H_

/* Includes -------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"

/* exported typedef -----------------------------------------------------------*/

/* exported define ------------------------------------------------------------*/
/* Rangefinder-task RTOS configuration */
#define RANGEFINDER_TASK_NAME       "Rangefinder Task"
#define RANGEFINDER_QUEUE_LENGTH    20      /* Size of the message queues */
#define RANGEFINDER_STACK_SIZE      configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define RANGEFINDER_TASK_PRIORITY   (configMAX_PRIORITIES - 1UL) /* priority 5*/

/* Rangefinder settings */ //TODO: Adjust values
#define RANGEFINDER_RANGE           100     /* Maximum detection range in cm (int16_t values from 5 to 1100) */
#define RANGEFINDER_THRESHOLD_FW    30      /* Minimum allowed distance in cm to an obstacle (in front) without alert */
#define RANGEFINDER_THRESHOLD_BW    30      /* Minimum allowed distance in cm to an obstacle (in back) without alert */
#define RANGEFINDER_THRESHOLD_SE    3       /* Minimum allowed space in cm for the separation (fresco panel) without alert */
#define RANGEFINDER_DELAY           100     /* Task delay in ms between measures */

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/
/* Mutex for I2C */
extern xSemaphoreHandle mHwI2C;    /* Created with xSemaphoreCreateMutex() */
extern xSemaphoreHandle sSyncNodeTask;    /* Is given by Rangefinder, when obstacle detected */

/* Alarm flags (read only!), 1 if object detected, 0 if no object detected */
extern volatile uint8_t Rangefinder_flag_FwAlarmIR;     /* Infrared forward alarm */
extern volatile uint8_t Rangefinder_flag_BwAlarmIR;     /* Infrared backward alarm */
extern volatile uint8_t Rangefinder_flag_FwAlarmUS;     /* Ultrasonic forward alarm */
extern volatile uint8_t Rangefinder_flag_BwAlarmUS;     /* Ultrasonic backward alarm */
extern volatile uint8_t Rangefinder_flag_SeAlarmUS;     /* Ultrasonic separation alarm */

/* exported function prototypes -----------------------------------------------*/
extern void initRangefinderTask(void);
extern void IRSensorFwLeft_IT(void);
extern void IRSensorFwRight_IT(void);
extern void IRSensorBwLeft_IT(void);
extern void IRSensorBwRight_IT(void);

#endif /* RANGEFINDER_TASK_H_ */

/**
 * @}
 */
