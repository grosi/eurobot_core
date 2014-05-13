/**
 * \file    FireWallInversNode.h
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
#ifndef FIREWALLINVERSNODE_H_
#define FIREWALLINVERSNODE_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
#define FIRE_WALL_INVERSE_NODE_DELTA_GO 	100 	/*!< [mm] */
#define FIRE_WALL_NODE_DRIVE_HEART_DELAY	1500
#define FIRE_HEART_DRIVE_DELAY				500
#define SERVO_POS_AIR_WALL_INVERSE			1700
#define SERVO_POS_AIR_WALL_INVERSE_SUCKER	1600
#define SERVO_POS_AIR_PLACE					1200

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFireWallInversNode(node_param_t*, volatile game_state_t*);


#endif /* FIREWALLINVERSNODE_H_ */

/**
 * @}
 */
