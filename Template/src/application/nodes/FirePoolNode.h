/**
 * \file    FireWallNormalNode.h
 * \author  gross10
 * \date    2014-05-09
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fire pool node statemaschine
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef FIREPOOLNODE_H_
#define FIREPOOLNODE_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
#define FIRE_POOL_APPROACH_DISTANCE                 100 /* Distance in mm to drive from the fire-pool node point to the pool */
#define FIRE_POOL_APPROACH_OVERHEAD                 30 /* Additional Distance in mm to drive to the pool to make sure we are there */
#define FIRE_POOL_APPROACH_SPEED                    30 /* Speed in percent to drive from node point to pool */
#define FIRE_POOL_TRANSIT_SPEED                     100
#define FIRE_POOL_APPROACH_TIME                     5000 /* Time in ms to wait while driving to pool */ //TODO
#define FIRE_POOL_DRIVECHECK_DELAY                  10
#define FIRE_POOL_MAX_RETRIES                       5
#define FIRE_POOL_ACK_DELAY                         20
#define FIRE_POOL_HEARTPOOL_X_POSITION_RED          250
#define FIRE_POOL_HEARTPOOL_Y_POSITION_RED          410
#define FIRE_POOL_HEARTPOOL_ANGLE_POSITION_RED      225
#define FIRE_POOL_HEART_X_POSITION_RED              300
#define FIRE_POOL_HEART_Y_POSITION_RED              180
#define FIRE_POOL_HEART_ANGLE_POSITION_RED          225
#define FIRE_POOL_HEARTPOOL_X_POSITION_YELLOW       2750
#define FIRE_POOL_HEARTPOOL_Y_POSITION_YELLOW       410
#define FIRE_POOL_HEARTPOOL_ANGLE_POSITION_YELLOW   320
#define FIRE_POOL_HEART_X_POSITION_YELLOW           2700
#define FIRE_POOL_HEART_Y_POSITION_YELLOW           180
#define FIRE_POOL_HEART_ANGLE_POSITION_YELLOW       320

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFirePoolNode(node_param_t*, volatile game_state_t*);


#endif /* FIREPOOLNODE_H_ */

/**
 * @}
 */
