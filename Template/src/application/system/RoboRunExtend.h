/**
 * \file    RoboRunExtend.h
 * \author  gross10
 * \date    2014-05-08
 *
 * \version 1.0
 *  create this file
 *
 * \brief   funncy action state
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef ROBORUNEXTEND_H_
#define ROBORUNEXTEND_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* game configurations */
#define PLAY_FUNNY_TIME             5.0 /*!< [s] */


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initRoboRunExtendState();
extern void runRoboRunExtendState(portTickType*);


#endif /* ROBORUNEXTEND_H_ */
/**
 * @}
 */
