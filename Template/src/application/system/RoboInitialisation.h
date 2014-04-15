/**
 * \file    RoboInitialisation.h
 * \author  gross10
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

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initRoboInitialisationState();
extern void setConfigRoboInitialisationState2Emergency();
extern void runRoboInitialisationState(portTickType*);
extern void initNodeResources();

#endif /* ROBOINITIALISATION_H_ */
/**
 * @}
 */
