/**
 * \file    CANGatekeeper.h
 * \author  gross10
 * \date    2013-11-16
 *
 * \version 1.0
 *  create this file
 *
 * \brief   gatekeeper for the resource "can"
 *
 * \defgroup cangatekeeper CANGatekeeper
 * \brief   gatekeeper for the node-communication
 * \ingroup hw_task
 *
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CANGATEKEEPER_H_
#define CANGATEKEEPER_H_

/* exported typedef ----------------------------------------------------------*/

/**
 *  \brief rx data-handler
 */
typedef union
{
    /* GotoXY data-set */
    struct
    {
        uint16_t goto_x; /*!< x-position */
        uint16_t goto_y; /*!< y-position */
        uint16_t goto_angle; /*!< angle in degree */
        uint8_t goto_speed; /*!< speed in % */
        uint8_t goto_direction; /*!< drive direction */
        uint16_t goto_barrier; /*!< barrier-flags */
        uint8_t goto_route; /*!< routing-flag */
    };
    /* ELP data-set */
    struct
    {
        uint16_t elp_x; /*!< x-position */
        uint16_t elp_y; /*!< y-position */
        uint16_t elp_angle; /*!< angle in degree */
        uint8_t elp_id; /*!< id of the information source */
    };
    /* GIP data-set */
    struct
    {
        uint8_t gip_color; /*!< teamcolor (0=yellow; 1=red) */
        uint8_t gip_enemy; /*!< enemey quantity (0=0 enemey; 1=1 enemy; 2=2 enemies) */
        uint8_t gip_confederate; /*!< confederate quantity (0=0 confederate; 1=1 condeferate) */
        uint8_t gip_enemy_1_size; /*!< 0-50cm with 1cm step-size */
        uint8_t gip_enemy_2_size; /*!< 0-50cm with 1cm step-size */
    };
    /* State Response data-set */
    uint32_t state_time; /*!< time until the roboter stops */
    /* Extended stop data-set */
    uint8_t stop_obstacle_id; /*!< id of the obstacle */
} CAN_data_t;

/**
 * \brief function pointer for CAN-listeners
 */
typedef void (*CAN_function_listener_t) (uint16_t, CAN_data_t*);

/**
 * \brief   type for listener-database
 */
typedef struct
{
    uint16_t id; /*!< id of the CAN-message */
    xQueueHandle* queue; /*!< pointer to a rx-queue */
    CAN_function_listener_t function; /*!< id to a callback function */
}CAN_listener_t;


/* exported define -----------------------------------------------------------*/
/* CAN-interface RTOS configuration */
#define CAN_TX_TASK_NAME            "CAN Tx"
#define CAN_RX_TASK_NAME            "CAN Rx"
#define CAN_QUEUE_LENGTH            20 /*!< Size of the message queues */
#define CAN_STACK_SIZE              configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define CAN_TASK_PRIORITY           (configMAX_PRIORITIES - 1UL) /**< priority (6) of the receive and transmit task */
#define CAN_LISTENER_BUFFER_SIZE    20 /*!< adjust to the needed size */
#define CAN_TX_MAX_WAIT_TIME        5 /*!< max waiting for empty tx-mailbox (* 10 ms) */


/**
 * \brief   Possible CAN-messages with theirs identifiers
 */
/* Emergency */
#define EMERGENCY_SHUTDOWN              0x001
#define EMERGENCY_STOP                  0x002
/* Normal */
#define STOP_DRIVE                      0x004
#define GOTO_XY                         0x008
#define GOTO_CONFIRM                    0x00C
#define GOTO_STATE_REQUEST              0x010
#define GOTO_STATE_RESPONSE             0x014
/* Information */
#define NAVI_POSITION_REQUEST          	0x040
#define NAVI_POSITION_RESPONSE         	0x080
#define KALMAN_POSITION_REQUEST         0x0C0
#define KALMAN_POSITION_RESPONSE        0x100
#define ENEMEY_1_POSITION_REQUEST       0x140
#define ENEMEY_1_POSITION_RESPONSE      0x180
#define ENEMEY_2_POSITION_REQUEST       0x1C0
#define ENEMEY_2_POSITION_RESPONSE      0x200
#define CONFEDERATE_POSITION_REQUEST    0x240
#define CONFEDERATE_POSITION_RESPONSE   0x280
#define START_CONFIGURATION_SET         0x2C0
#define START_CONFIGURATION_CONFIRM     0x300
#define CHECK_NAVI_REQUEST              0x340
#define CHECK_NAVI_RESPONSE             0x380
#define CHECK_DRIVE_REQUEST             0x3C0
#define CHECK_DRIVE_RESPONSE            0x400


/**
 * \brief   ID high-priority level
 */
#define ID_HIGH_LEVEL_PRIORITY          0x004

/**
 * \brief	Possible Goto-Barrier
 */
#define GOTO_NO_BARRIER         0x0000
#define GOTO_FIRE_1             0x0001
#define GOTO_FIRE_1_FORCE       0x0002
#define GOTO_FIRE_2             0x0004
#define GOTO_FIRE_2_FORCE       0x0008
#define GOTO_FIRE_3             0x0010
#define GOTO_FIRE_3_FORCE       0x0020
#define GOTO_FIRE_4             0x0040
#define GOTO_FIRE_4_FORCE       0x0080
#define GOTO_FIRE_5             0x0100
#define GOTO_FIRE_5_FORCE       0x0200
#define GOTO_FIRE_6             0x0400
#define GOTO_FIRE_6_FORCE       0x0800
#define GOTO_FIRE_POOL_1        0x1000
#define GOTO_FIRE_POOL_1_FORCE  0x2000
#define GOTO_FIRE_POOL_2        0x4000
#define GOTO_FIRE_POOL_2_FORCE  0x8000

/**
 * \brief   drive direction
 */
#define GOTO_DRIVE_FORWARD      0
#define GOTO_DRIVE_BACKWARD     1

/*
 * \brief   drive errors
 */
#define GOTO_NOT_POSSIBLE_ATM  0xFFFFFF /* Message: It's not possible to go to that position at the moment */
#define GOTO_BLOCKED_ATM       0xFFFFFE /* Message: It's blocked at the moment */


/*
 * \brief   drive rounting
 */
#define GOTO_ROUTE              1
#define GOTO_NO_ROUTE           0


/**
 * \brief   Possible ELP-response id's
 */
#define ELP_INTERNAL 0x0
#define ELP_CONFEDERATE 0x1
#define ELP_ENEMEY_1 0x2
#define ELP_ENEMEY_2 0x3

/**
 * \brief   Possible GIP-parameters
 */
#define GIP_TEAMCOLOR_YELLOW        0
#define GIP_TEAMCOLOR_RED           1
#define GIP_ENEMY_QUANTITY_0        0
#define GIP_ENEMY_QUANTITY_1        1
#define GIP_ENEMY_QUANTITY_2		2
#define GIP_CONFEDERATE_QUANTITY_0  0
#define GIP_CONFEDERATE_QUANTITY_1  1


/* exported macro ------------------------------------------------------------*/


/* exported variables --------------------------------------------------------*/


/* exported function prototypes ----------------------------------------------*/
extern void initCANGatekeeper(void);
extern void setQueueCANListener(xQueueHandle*, uint16_t);
extern void setFunctionCANListener(CAN_function_listener_t, uint16_t);
extern void txEmergencyShutdown();
extern void txEmergencyStop(uint8_t);
extern void txStopDrive();
extern void txGotoXY(uint16_t, uint16_t, uint16_t, uint8_t, uint16_t, uint8_t, uint8_t);
extern void txGotoConfirm();
extern void txGotoStateRequest();
extern void txGotoStateResponse(uint32_t time);
extern void txNaviPositionRequest();
extern void txNaviPositionResponse(int16_t, int16_t, int16_t, uint8_t);
extern void txKalmanPositionRequest();
extern void txKalmanPositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern void txEnemey1PositionRequest();
extern void txEnemey1PositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern void txEnemey2PositionRequest();
extern void txEnemey2PositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern void txConfederatePositionRequest();
extern void txConfederatePositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern void txStartConfigurationSet(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
extern void txStartConfigurationConfirm();
extern void txCheckNaviRequest();
extern void txCheckNaviResponse();
extern void txCheckDriveRequest();
extern void txCheckDriveResponse();

#endif /* CANGATEKEEPER_H_ */
/**
 * @}
 */
