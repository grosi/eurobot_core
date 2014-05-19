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
#define FIRE_POOL_APPROACH_DISTANCE           230 /* Distance in mm to drive from the fire-pool node point to the pool */
#define FIRE_POOL_APPROACH_SPEED                    40 /* Speed in percent to drive from node point to pool */
#define FIRE_POOL_TRANSIT_SPEED                     80
#define FIRE_POOL_APPROACH_TIME                     5000 /* Time in ms to wait while driving to pool */ //TODO
#define FIRE_POOL_DRIVECHECK_DELAY                  10

/* red */
#define FIRE_POOL_HEARTPOOL_X_POSITION_RED          1060
#define FIRE_POOL_HEARTPOOL_Y_POSITION_RED          900
#define FIRE_POOL_HEARTPOOL_APPROACH_X_RED    (FIRE_POOL_APPROACH_DISTANCE)

/* yellow */
#define FIRE_POOL_HEARTPOOL_X_POSITION_YELLOW           2730
#define FIRE_POOL_HEARTPOOL_Y_POSITION_YELLOW           1940
#define FIRE_POOL_HEARTPOOL_APPROACH_X_YELLOW     (-FIRE_POOL_APPROACH_DISTANCE)


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFirePoolNode(node_param_t*, volatile game_state_t*);


#endif /* FIREPOOLNODE_H_ */

/**
 * @}
 */
