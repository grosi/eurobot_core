/**
 * \file    CANGatekeeper.h
 * \author  Grossenbacher Simon
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
        int8_t goto_points[2][2]; /*!< points */
    };
    /* ELP data-set */
    struct
    {
        uint16_t elp_x; /*!< x-position */
        uint16_t elp_y; /*!< y-position */
        uint16_t elp_angle; /*!< angle in degree */
        uint8_t elp_id; /*!< id of the information source */
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
#define CAN_TASK_PRIORITY           (configMAX_PRIORITIES - 3UL) /**< priority (3) of the receive and transmit task */
#define CAN_LISTENER_BUFFER_SIZE    20 /*!< adjust to the needed size */
#define CAN_TX_MAX_WAIT_TIME        5 /*!< max waiting for empty tx-mailbox (* 10 ms) */


/**
 * \brief   Possible CAN-messages with theirs identifiers
 */
#define EMERGENCY_SHUTDOWN              0x001
#define EMERGENCY_STOP                  0x002
#define STOP_DRIVE                      0x004
#define GOTO_XY                         0x008
#define GOTO_CONFIRM                    0x00C
#define GOTO_STATE_REQUEST              0x010
#define GOTO_STATE_RESPONSE             0x014
#define LASER_POSITION_REQUEST          0x040
#define LASER_POSITION_RESPONSE         0x080
#define ULTRASONIC_POSITION_REQUEST     0x0C0
#define ULTRASONIC_POSITION_RESPONSE    0x100
#define KALMAN_POSITION_REQUEST         0x140
#define KALMAN_POSITION_RESPONSE        0x180
#define ENEMEY_1_POSITION_REQUEST       0x1C0
#define ENEMEY_1_POSITION_RESPONSE      0x200
#define ENEMEY_2_POSITION_REQUEST       0x240
#define ENEMEY_2_POSITION_RESPONSE      0x280
#define CONFEDERATE_POSITION_REQUEST    0x2C0
#define CONFEDERATE_POISTION_RESPONSE   0x300


/**
 * \brief   Possible ELP-response id's
 */
#define ELP_INTERNAL 0x0
#define ELP_CONFEDERATE 0x1
#define ELP_ENEMEY_1 0x2
#define ELP_ENEMEY_2 0x3


/* exported macro ------------------------------------------------------------*/


/* exported variables --------------------------------------------------------*/


/* exported function prototypes ----------------------------------------------*/
extern void initCANGatekeeper(void);
extern void setQueueCANListener(xQueueHandle*, uint16_t);
extern void setFunctionCANListener(CAN_function_listener_t, uint16_t);
extern inline void txEmergencyShutdown();
extern inline void txEmergencyStop(uint8_t);
extern inline void txStopDrive();
extern void txGotoXY(uint16_t, uint16_t, uint16_t, uint8_t, int8_t[2][2]);
extern inline void txGotoConfirm();
extern inline void txGotoStateRequest();
extern void txGotoStateResponse(uint32_t time);
extern inline void txLaserPostionRequest();
extern inline void txLaserPositionResponse(int16_t, int16_t, int16_t, uint8_t);
extern inline void txUltrasonicPositionRequest();
extern inline void txUltrasonicPositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern inline void txKalmanPositionRequest();
extern inline void txKalmanPositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern inline void txEnemey1PositionRequest();
extern inline void txEnemey1PositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern inline void txEnemey2PositionRequest();
extern inline void txEnemey2PositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);
extern inline void txConfederatePositionRequest();
extern inline void txConfederatePositionResponse(uint16_t, uint16_t, uint16_t, uint8_t);

#endif /* CANGATEKEEPER_H_ */
/**
 * @}
 */
