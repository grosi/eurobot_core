/**
 * \file    NetNode.h
 * \author  gross10
 * \date    2014-05-09
 *
 * \version 1.0
 *  create this file
 *
 * \brief   net node statemaschine
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef NETNODE_H_
#define NETNODE_H_

/* includes for prototypes */
#include "NodeConfig.h"

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
#define SERVO_POS_NET_LOAD          1400 /* Servo position: Launcher all the way front */
#define SERVO_POS_NET_LAUNCH        1700 /* Servo position: Launcher all the way back */

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doNetNode(node_param_t*, volatile game_state_t* game_state);


#endif /* FIREWALLINVERSNODE_H_ */

/**
 * @}
 */
