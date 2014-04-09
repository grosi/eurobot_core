/**
 * \file    NodeConfig.h
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
#ifndef NODECONFIG_H_
#define NODECONFIG_H_

/* exported typedef -----------------------------------------------------------*/

/**
 * \brief current node state
 */
typedef enum
{
    NODE_UNDONE = 0,        /*!< no node is in ready mode -> still undone */
    NODE_FINISH_SUCCESS,    /*!< node is done and the node-task ready for new tasks */
    NODE_FINISH_ERROR,      /*!< node is done with errors and the node-task ready for new tasks -> if the node is in a pool, don't decrement this! */
}node_state_t;


/**
 * \brief the needed arrive direction for a node
 */
typedef enum
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
}node_arrive_t;


/**
 * \brief current robo state
 */
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t angle;
    int16_t enemy_1_x; /*!< -1 = no enemy 1 or error */
    int16_t enemy_1_y; /*!< -1 = no enemy 1 or error */
    int16_t enemy_2_x; /*!< -1 = no enemy 2 or error */
    int16_t enemy_2_y; /*!< -1 = no enemy 2 or error */
}game_state_t;


/**
 *  \brief node parameters
 */
typedef struct
{
    const uint8_t id;
    const uint8_t points;
    const float   percent;
    const uint8_t time; /*[s]*/
    const uint16_t x; /*!<[mm]*/
    const uint16_t y; /*!<[mm]*/
    const uint8_t pool_id;
    const node_arrive_t arrive;  //TODO: Change code to use 'angle' and then remove 'arrive'
    const uint16_t angle; /*10 bit, 1 bit =ca. 1�*/
    node_state_t node_state;
}node_param_t;


/**
 * \brief node complete data
 */
typedef struct
{
    node_param_t param;
    void (*node_function)(node_param_t*);
}node_t;


/* exported define ------------------------------------------------------------*/
/* general */
#define NODE_QUANTITY       11
#define NODE_POOL_QUANTITY  2
#define NODE_NO_ENEMY       -1

/* nodes pools */
#define NODE_NO_POOL_ID         0
#define NODE_MAMMOTH_POOL_ID    1
#define NODE_MAMMOTH_POOL_SIZE  6
#define NODE_MAMMOTH_POOL_LEVEL 3
#define NODE_FRESCO_POOL_ID     2
#define NODE_FRESCO_POOL_SIZE   2
#define NODE_FRESCO_POOL_LEVEL  1
#define NODE_FIRE_POOL_ID       3
#define NODE_FIRE_POOL_SIZE     0
#define NODE_FIRE_POOL_LEVEL    0

/* Servo */
#define SERVO_POS_FRESCO_IN     2000    /* Servo position: Fresco panel all the way in */
#define SERVO_POS_FRESCO_OUT    1600    /* Servo position: Fresco panel all the way out */
#define SERVO_MOVING_DELAY      (200) /* Delay in ms to wait while the servo moves the whole way */ //TODO


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
extern node_t node_mammoth_1;


/* exported function prototypes -----------------------------------------------*/


#endif /* NODECONFIG_H_ */

/**
 * @}
 */
