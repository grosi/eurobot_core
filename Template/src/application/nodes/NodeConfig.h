/**
 * \file    NodeConfig.h
 * \author  gross10, kasen1
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
    NODE_UNDONE = 0,        /*!< no node is in ready mode -> still undone, arrival was not possible */
    NODE_FINISH_SUCCESS,    /*!< node is done and the node-task ready for new tasks */
    NODE_FINISH_UNSUCCESS,  /*!< node is done with errors and the node-task ready for new tasks -> try again later */
    NODE_FINISH_ERROR,      /*!< node is done with errors and the node-task ready for new tasks*/
    GOTO_CAN_ERROR,         /*!< didn't receive GoTo confirmation via CAN after several retries */
}node_state_t;


/**
 * \brief current robo state
 */
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t angle;
    team_color_t teamcolor;

    uint8_t  enemy_count;       /*!< Number of enemies: 0, 1 or 2 */
    uint16_t enemy_1_x;         /*!< 0xFFFF = no enemy 1 or error */
    uint16_t enemy_1_y;         /*!< 0xFFFF = no enemy 1 or error */
    uint16_t  enemy_1_diameter;  /*!< 0 = no enemy 1 [mm] */
    uint16_t enemy_2_x;         /*!< 0xFFFF = no enemy 2 or error */
    uint16_t enemy_2_y;         /*!< 0xFFFF = no enemy 2 or error */
    uint16_t  enemy_2_diameter;  /*!< 0 = no enemy 2 [mm] */

    uint8_t  confederate_count; /*!< Number of confederate: 0, 1 or 2 */
    uint16_t confederate_x;     /*!< 0xFFFF = no confederate or error */
    uint16_t confederate_y;     /*!< 0xFFFF = no confederate or error */

    uint16_t barrier;           /*!< barriers flags */
}game_state_t;


/**
 *  \brief node parameters
 */
typedef struct
{
    const uint8_t id;
    const float points;
    const float   percent;
    const float time; /*[s]*/
    const uint16_t x; /*!<[mm]*/
    const uint16_t y; /*!<[mm]*/
    const uint8_t pool_id;
    const uint16_t angle; /*10 bit, 1 bit =ca. 1°*/
    uint16_t node_tries; /*!< how many times the node was the current node*/
    node_state_t node_state;
}node_param_t;


/**
 * \brief node complete data
 */
typedef struct
{
    node_param_t param;
    void (*node_function)(node_param_t*, volatile game_state_t*);
}node_t;


/* exported define ------------------------------------------------------------*/
/* general */
#define NODE_QUANTITY              10 //TODO
#define NODE_POOL_QUANTITY         1
#define NODE_NO_ENEMY              0xFFFF
#define NODE_NO_ENEMY_DIAMETER     0
#define NODE_NO_CONFEDERATE        0

/* nodes pools */
#define NODE_NO_POOL_ID            0
#define NODE_NET_POOL_ID           1
#define NODE_NET_POOL_SIZE         3
#define NODE_NET_POOL_LEVEL        1

/* nodes arrive direction ranges */
#define NODE_SOUTH_MIN_ANGLE       45
#define NODE_SOUTH_MAX_ANGLE       134
#define NODE_EAST_MIN_ANGLE        135
#define NODE_EAST_MAX_ANGLE        224
#define NODE_NORTH_MIN_ANGLE       225
#define NODE_NORTH_MAX_ANGLE       314
#define NODE_WEST_MIN_ANGLE        315 /*!< note, that the zero line is crossed */
#define NODE_WEST_MAX_ANGLE        44  /*!< note, that the zero line is crossed */


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
/* red */
extern node_t node_fire_1_red;
extern node_t node_fire_2_red;
extern node_t node_fire_3_red;
extern node_t node_fire_pool_red;
extern node_t node_fire_wall_normal_1_red;
extern node_t node_fire_wall_normal_2_red;
extern node_t node_fire_wall_invers_red;
extern node_t node_net_1_red;
extern node_t node_net_2_red;
extern node_t node_net_3_red;

/* yellow */
extern node_t node_fire_1_yellow;
extern node_t node_fire_2_yellow;
extern node_t node_fire_3_yellow;
extern node_t node_fire_pool_yellow;
extern node_t node_fire_wall_normal_1_yellow;
extern node_t node_fire_wall_normal_2_yellow;
extern node_t node_fire_wall_invers_yellow;
extern node_t node_net_1_yellow;
extern node_t node_net_2_yellow;
extern node_t node_net_3_yellow;

/* CAN */
extern xQueueHandle qGotoConfirm;
extern xQueueHandle qGotoStateResp;


/* exported function prototypes -----------------------------------------------*/


#endif /* NODECONFIG_H_ */

/**
 * @}
 */
