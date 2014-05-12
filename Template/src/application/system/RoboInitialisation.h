/**
 * \file    RoboInitialisation.h
 * \author  gross10, kasen1
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy init state
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef ROBOINITIALISATION_H_
#define ROBOINITIALISATION_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* RTOS */
#define ROBOINIT_QUEUE_LENGTH   1
#define ROBOINIT_TIMEOUT        1000 /*!< wait for max. 1s */

/* sensors */
#define SENSOR_EMERGENCY_INIT       1
#define SENSOR_AIR_INIT             0
#define SENSOR_FIRE_POOL_RIGHT_INIT 0
#define SENSOR_FIRE_POOL_LEFT_INIT  0
#define SENSOR_KEY_INIT             0

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initRoboInitialisationState();
extern void setConfigRoboInitialisationState2Emergency();
extern void runRoboInitialisationState(portTickType*);

#endif /* ROBOINITIALISATION_H_ */
/**
 * @}
 */
