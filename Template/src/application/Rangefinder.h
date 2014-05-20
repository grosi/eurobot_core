/**
 * \file    Rangefinder.c
 * \author  kasen1
 * \date    2013-05-18
 *
 * \version 2.1b
 *  - Updated function to compare rangefinder with navigation informations
 *  - Changed all distances from cm to mm
 * \version 2.0b
 *  - Changed to two ultrasonic sensor in front (for the big robot "B52")
 *  - Removed code for IR-Sensors, as they're not used in this robot
 *  - Updated navigation comparing function with option to check backwards
 *  - Version tested (2013-05-18)
 * \version 1.3
 *  - Added function to compare rangefinder with navigation informations
 *  - Releasing semaphore on i2c error for safety reason
 *  - Version tested (2013-05-13)
 * \version 1.2
 *  - IR sensors in new arrangement
 *  - Added flag for separation blocked alarm
 *  - Added option to only use forward rangefinder by defining RANGEFINDER_ONLY_FW
 *  - Task is suspended at beginning, so ultrasonic only running when really used
 *  - Semaphore for synchronising with node task added (not in test protocol, but is tested)
 *  - Version tested (2013-05-01)
 * \version 1.1
 *  - Changed implementation of IR detection to external interrupt
 *  - Implemented comparison of last three US measures
 *  - Fixed I2C error, where the bus was always busy after disconnecting a slave
 *  - Version tested (2013-03-12)
 * \version 1.0
 *  - Implemented first version
 *  - Version tested (2013-01-14)
 * \version 0.1
 *  - Import from template (2013-01-14)
 *
 * \brief    task for the rangefinder sensors and helper functions
 *
 * \defgroup rangefinder Rangefinder
 * \brief    Rangefinder task
 * \ingroup  hw_task
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef RANGEFINDER_TASK_H_
#define RANGEFINDER_TASK_H_

/* Includes -------------------------------------------------------------------*/
#include "AppConfig.h"
#include "nodes/NodeConfig.h"  /* For game_state_t */

/* exported typedef -----------------------------------------------------------*/

/* exported define ------------------------------------------------------------*/
/* Rangefinder module */
#define RANGEFINDER_ONLY_FW                 /* Comment out to use Fw and Bw sensors */
/* Rangefinder-task RTOS configuration */
#define RANGEFINDER_TASK_NAME       "Rangefinder Task"
#define RANGEFINDER_QUEUE_LENGTH    20      /*!< Size of the message queues */
#define RANGEFINDER_STACK_SIZE      configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define RANGEFINDER_TASK_PRIORITY   (configMAX_PRIORITIES - 2UL) /*!< priority 5*/

/* Rangefinder settings */ //TODO: Adjust values
#define RANGEFINDER_RANGE           1000    /*!< Maximum detection range in mm (int16_t values from 43 to 11008) */
#define RANGEFINDER_THRESHOLD_FW    400     /*!< Minimum allowed distance in mm to an obstacle (in front) without alert */
#define RANGEFINDER_THRESHOLD_FI    30      /*!< Minimum allowed space in mm for the fire node without alert */
#define RANGEFINDER_DELAY           100     /*!< Task delay in ms between measures */
#define RANGEFINDER_ANGLE           30      /*!< half of the complete angle range (complete 60 degree) */

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/
/* RTOS */
extern xTaskHandle xRangefinderTask_Handle;
extern xSemaphoreHandle mHwI2C;  /* Mutex for I2C */
extern xSemaphoreHandle sSyncNodeTask;  /* Semaphore for node task synchronisation */

/* Alarm flags (read only!), 1 if object detected, 0 if no object detected */
extern volatile uint8_t Rangefinder_flag_LAlarmUS;     /* Ultrasonic left alarm */
extern volatile uint8_t Rangefinder_flag_RAlarmUS;     /* Ultrasonic right alarm */
extern volatile uint8_t Rangefinder_flag_FiAlarmUS;     /* Ultrasonic fire node alarm */

/* exported function prototypes -----------------------------------------------*/
extern void initRangefinderTask(void);
extern void suspendRangefinderTask(void);
extern uint16_t isRobotInRange(volatile game_state_t*, boolean);

#endif /* RANGEFINDER_TASK_H_ */

/**
 * @}
 */
