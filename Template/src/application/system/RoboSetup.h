/**
 * \file    RoboSetup.h
 * \author  gross10
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy setup state
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef ROBOSETUP_H_
#define ROBOSETUP_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
extern xSemaphoreHandle sELP;

/* exported function prototypes -----------------------------------------------*/
extern void runRoboSetupState(portTickType*);


#endif /* ROBOSETUP_H_ */
/**
 * @}
 */
