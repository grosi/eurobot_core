/**
 * \file    NodeMisc.h
 * \author  gross10
 * \date    2014-05-19
 *
 * \version 1.0
 *  create this file
 *
 * \brief   a few node functions
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef NODEMISC_H_
#define NODEMISC_H_

/* Includes -------------------------------------------------------------------*/
#include "NodeConfig.h"  /* For game_state_t */

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* drive */
#define DRIVE_ROUTE_DIST_MIN        150
/* CAN */
#define CAN_WAIT_DELAY              400
#define CAN_CHECK_DELAY             100
#define CAN_MAX_RETRIES             5

/* servos */
#define SERVO_AIR_INIT              1990
#define SERVO_NET_INIT              1400
#define SERVO_STEP_DELAY            2
#define SERVO_STEP                  2
#define SERVO_POS_DOWN              1100
#define SERVO_POS_UP                1990

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initNodeResources(void);
extern void setNodeConfig2Default(void);
extern uint8_t checkDrive(uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, volatile game_state_t*);
extern uint8_t driveGoto(uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, volatile game_state_t*);
extern void placeSucker(uint16_t);


#endif /* NODEMISC_H_ */

/**
 * @}
 */
