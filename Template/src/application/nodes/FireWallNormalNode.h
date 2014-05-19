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
#define FIRE_WALL_NODE_DELTA_GO 	            110 	/*!< [mm] */
#define SERVO_POS_WALL			                1765	/* Servo Position to drop fire */
#define FIREWALL_APPROACHDISTANCE               300

#define FIRE_WALL_NORMAL_NODE_SPEED             60      /*!< [%] */
#define FIRE_WALL_NORMAL_NODE_BARRIER           0
#define FIRE_WALL_NORMAL_NODE_DRIVE_DELAY       1000
#define FIRE_WALL_NORMAL_NODE_DRIVE_BACK_DELAY  1000

#define SERVO_WALL_NORMAL_STEP_DELAY            2
#define SERVO_WALL_NORMAL_STEP                  2
#define SERVO_WALL_NORMAL_POS_DOWN              1100
#define SERVO_WALL_NORMAL_POS_UP                1990

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFireWallNormalNode(node_param_t*, volatile game_state_t* game_state);


#endif /* FIREWALLNORMALNODE_H_ */

/**
 * @}
 */
