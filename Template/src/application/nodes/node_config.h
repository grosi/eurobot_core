/**
 * \file    node_config.h
 * \author  gross10
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   config file for the game nodes
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef NODE_CONFIG_H_
#define NODE_CONFIG_H_

/* exported typedef -----------------------------------------------------------*/

/**
 * \brief current robo state
 */
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t angle;
}robo_state_t;


/**
 *  \brief node parameters
 */
typedef struct
{
    uint8_t id;
    uint8_t points;
    float   percent;
    uint8_t time; /*[s]*/
    float   x; /*!<[m]*/
    float   y; /*!<[m]*/
    uint8_t pool_id;
    float   weight;
}node_param_t;


/**
 * \brief node complete data
 */
typedef struct
{
    node_param_t param;
    void (*node_function)(node_param_t*, robo_state_t*, uint8_t*);
}node_t;


/* exported define ------------------------------------------------------------*/
/* general */
#define NODE_QUANTITY       11
#define NODE_POOL_QUANTITY  2

/* nodes states */
#define NODE_BUSY       0
#define NODE_STOP       1
#define NODE_FREE_DRIVE 2

/* nodes pools */
#define NODE_NO_POOL_ID         0
#define NODE_MAMMUT_POOL_ID     1
#define NODE_MAMMUT_POOL_SIZE   6
#define NODE_MAMMUT_POOL_LEVEL  3
#define NODE_FRESCO_POOL_ID     2
#define NODE_FRESCO_POOL_SIZE   2
#define NODE_FRESCO_POOL_LEVEL  1
#define NODE_FIRE_POOL_ID       3
#define NODE_FIRE_POOL_SIZE     0
#define NODE_FIRE_POOL_LEVEL    0


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
extern node_t node_mammut_1;


/* exported function prototypes -----------------------------------------------*/


#endif /* NODE_CONFIG_H_ */

/**
 * @}
 */
