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
    uint16_t enemy_1_diameter; /*!< 0 = no enemy 1 [mm] */
    uint16_t enemy_2_x;         /*!< 0xFFFF = no enemy 2 or error */
    uint16_t enemy_2_y;         /*!< 0xFFFF = no enemy 2 or error */
    uint16_t enemy_2_diameter; /*!< 0 = no enemy 2 [mm] */

    uint8_t  confederate_count; /*!< Number of confederate: 0 or 1 */
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
    const float time; /*!< [s] */
    const uint16_t x; /*!< [mm] */
    const uint16_t y; /*!< [mm] */
    const uint8_t pool_id;
    const uint16_t angle; /*!< 10 bit, 1 bit =ca. 1° */
    uint16_t node_tries; /*!< weight factor for retrying the current node (grows when node couldn't be completed) */
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
#define NODE_QUANTITY              12
#define NODE_POOL_QUANTITY         2
#define NODE_NO_CONFEDERATE        0xFFFF
#define NODE_NO_ENEMY              0xFFFF
#define NODE_NO_ENEMY_DIAMETER     0

/* nodes pools */
#define NODE_NO_POOL_ID            0
#define NODE_MAMMOTH_POOL_ID       1
#define NODE_MAMMOTH_POOL_SIZE     6
#define NODE_MAMMOTH_POOL_LEVEL    3
#define NODE_FRESCO_POOL_ID        2
#define NODE_FRESCO_POOL_SIZE      3
#define NODE_FRESCO_POOL_LEVEL     2
#define NODE_FIRE_POOL_ID          3
#define NODE_FIRE_POOL_SIZE        0
#define NODE_FIRE_POOL_LEVEL       0

/* nodes arrive direction ranges */
#define NODE_SOUTH_MIN_ANGLE       45
#define NODE_SOUTH_MAX_ANGLE       134
#define NODE_EAST_MIN_ANGLE        135
#define NODE_EAST_MAX_ANGLE        224
#define NODE_NORTH_MIN_ANGLE       225
#define NODE_NORTH_MAX_ANGLE       314
#define NODE_WEST_MIN_ANGLE        315 /*!< note, that the zero line is crossed */
#define NODE_WEST_MAX_ANGLE        44  /*!< note, that the zero line is crossed */

/* Servo */
#define SERVO_MOVING_DELAY         400  /* Delay in ms to wait while the servo moves the whole way */
#define SERVO_POS_FRESCO_IN        2000 /* Servo position: Fresco panel all the way in */
#define SERVO_POS_FRESCO_OUT       900  /* Servo position: Fresco panel all the way out */
#define SERVO_POS_LAUNCHER_LOAD    1780 /* Servo position: Launcher all the way front */
#define SERVO_POS_LAUNCHER_LAUNCH  1200 /* Servo position: Launcher all the way back */

/* Fresco node */
#define FRESCO_APPROACH_DISTANCE   50   /* Distance to drive closer to the wall */
#define FRESCO_APPROACH_SPEED      30   /* Speed in percent to drive closer to the wall */
#define FRESCO_APPROACH_DELAY      2000 /* Time to wait while driving closer to the wall and moving Fresco out*/
#define FRESCO_WALL_POSITION       1950 /* Position to place the Fresco on the wall */
#define FRESCO_WALL_SPEED          20   /* Speed in percent to finally drive all the way to the wall */
#define FRESCO_WALL_TIME           4000 /* Time in ms to wait while driving all the way to the wall */

/* Fresco node */
#define FIRE_APPROACH_DISTANCE     250  /* Distance in mm to drive into the fire */  //TODO: Evaluate good value (as short as possible)


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
extern node_t node_mammoth_1;
extern node_t node_mammoth_2;
extern node_t node_mammoth_3;
extern node_t node_mammoth_4;
extern node_t node_mammoth_5;
extern node_t node_mammoth_6;
extern node_t node_fresco_1;
extern node_t node_fresco_2;
extern node_t node_fresco_3;
extern node_t node_fire_1_red;
extern node_t node_fire_2_red;
extern node_t node_fire_3_red;
extern node_t node_fire_1_yellow;
extern node_t node_fire_2_yellow;
extern node_t node_fire_3_yellow;

extern xQueueHandle qGotoConfirm;
extern xQueueHandle qGotoStateResp;


/* exported function prototypes -----------------------------------------------*/


#endif /* NODECONFIG_H_ */

/**
 * @}
 */
