/**
 * \file    FireWallNormalNode.h
 * \author  gross10
 * \date    2014-05-09
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fire wall node statemaschine
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef FIREWALLNORMALNODE_H_
#define FIREWALLNORMALNODE_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
//#define FIRE_NODE_DELTA_GO 250 /*!< [mm] */
//#define FIRE_NODE_SPEED 100 /*!< [%] */
//#define FIRE_NODE_BARRIER 0
//#define FIRE_NODE_ACK_DELAY 20
//#define FIRE_NODE_DRIVE_DELAY 500

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFireWallNormalNode(node_param_t*, volatile game_state_t* game_state);


#endif /* FIREWALLNORMALNODE_H_ */

/**
 * @}
 */