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
#define FIRE_POOL_APPROACH_FIRST_DISTANCE           230 /* Distance in mm to drive from the fire-pool node point to the pool */
#define FIRE_POOL_APPROACH_SECOND_DISTANCE          100
#define FIRE_POOL_FRONT_X_POSITION                  150
#define FIRE_POOL_FRONT_Y_POSITION                  0
#define FIRE_POOL_APPROACH_SPEED                    40 /* Speed in percent to drive from node point to pool */
#define FIRE_POOL_TRANSIT_SPEED                     80
#define FIRE_POOL_PLACE_SPEED                       100
#define FIRE_POOL_SHIFT_SPEED                       40
#define FIRE_POOL_APPROACH_TIME                     5000 /* Time in ms to wait while driving to pool */ //TODO
#define FIRE_POOL_DRIVECHECK_DELAY                  10
#define FIRE_POOL_MAX_RETRIES                       5
#define FIRE_POOL_ACK_DELAY                         20
/* red */
#define FIRE_POOL_HEARTPOOL_X_POSITION_RED          270
#define FIRE_POOL_HEARTPOOL_Y_POSITION_RED          450
#define FIRE_POOL_HEARTPOOL_FRONT_X_POSITION_RED    FIRE_POOL_FRONT_X_POSITION
#define FIRE_POOL_HEARTPOOL_FRONT_Y_POSITION_RED    FIRE_POOL_FRONT_Y_POSITION
#define FIRE_POOL_HEARTPOOL_APPROACH_FIRST_X_RED    (-FIRE_POOL_APPROACH_FIRST_DISTANCE)
#define FIRE_POOL_HEARTPOOL_APPROACH_FIRST_Y_RED    (-FIRE_POOL_APPROACH_FIRST_DISTANCE)
#define FIRE_POOL_HEARTPOOL_APPROACH_SECOND_X_RED   (-FIRE_POOL_APPROACH_SECOND_DISTANCE)
#define FIRE_POOL_HEARTPOOL_APPROACH_SECOND_Y_RED   0
#define FIRE_POOL_HEARTPOOL_ANGLE_POSITION_RED      180
#define FIRE_POOL_HEART_FRONT_X_POSITION_RED 		450
#define FIRE_POOL_HEART_FRONT_Y_POSITION_RED		200
#define FIRE_POOL_HEART_X_POSITION_RED              330
#define FIRE_POOL_HEART_Y_POSITION_RED              200
#define FIRE_POOL_HEART_ANGLE_POSITION_RED          190
/* yellow */
#define FIRE_POOL_HEARTPOOL_X_POSITION_YELLOW           2730
#define FIRE_POOL_HEARTPOOL_Y_POSITION_YELLOW           450
#define FIRE_POOL_HEARTPOOL_FRONT_X_POSITION_YELLOW     (-FIRE_POOL_FRONT_X_POSITION)
#define FIRE_POOL_HEARTPOOL_FRONT_Y_POSITION_YELLOW     (-FIRE_POOL_FRONT_Y_POSITION)
#define FIRE_POOL_HEARTPOOL_APPROACH_FIRST_X_YELLOW     (FIRE_POOL_APPROACH_FIRST_DISTANCE)
#define FIRE_POOL_HEARTPOOL_APPROACH_FIRST_Y_YELLOW     (FIRE_POOL_APPROACH_FIRST_DISTANCE)
#define FIRE_POOL_HEARTPOOL_APPROACH_SECOND_X_YELLOW    (FIRE_POOL_APPROACH_SECOND_DISTANCE)
#define FIRE_POOL_HEARTPOOL_APPROACH_SECOND_Y_YELLOW    0
#define FIRE_POOL_HEARTPOOL_ANGLE_POSITION_YELLOW       0
#define FIRE_POOL_HEART_FRONT_X_POSITION_YELLOW         2550
#define FIRE_POOL_HEART_FRONT_Y_POSITION_YELLOW         200
#define FIRE_POOL_HEART_X_POSITION_YELLOW               2670
#define FIRE_POOL_HEART_Y_POSITION_YELLOW               200
#define FIRE_POOL_HEART_ANGLE_POSITION_YELLOW           350

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFirePoolNode(node_param_t*, volatile game_state_t*);


#endif /* FIREPOOLNODE_H_ */

/**
 * @}
 */
