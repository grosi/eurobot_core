/**
 * \file    FireNode.h
 * \author  gross10
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fire node statemaschine
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef FIRENODE_H_
#define FIRENODE_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
#define FIRE_APPROACH_DISTANCE      400
#define FIRE1_APPROACH_DISTANCE		240
#define FIRE1_NODE_DELTA_GO			100
#define FIRE_NODE_DELTA_GO          (FIRE_APPROACH_DISTANCE) /*!< [mm] */
#define FIRE_NODE_SPEED             100 /*!< [%] */
#define FIRE_NODE_DRIVE_DELAY       1200
#define FIRENODE_APPROACHDISTANCE   400

/* servos */
#define SERVO_FIRE_POS_UP           1990
#define SERVO_FIRE_POS_DOWN         1180

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFireNode(node_param_t*, volatile game_state_t*);


#endif /* FIRE_NODE_H_ */

/**
 * @}
 */
