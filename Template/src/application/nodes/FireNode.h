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
#define FIRE_NODE_DELTA_GO 300 /*!< [mm] */
#define FIRE_NODE_SPEED 100 /*!< [%] */
#define FIRE_NODE_BARRIER 0
#define FIRE_NODE_ACK_DELAY 20
#define FIRE_NODE_DRIVE_DELAY 500

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFireNode(node_param_t*);


#endif /* FIRE_NODE_H_ */

/**
 * @}
 */
