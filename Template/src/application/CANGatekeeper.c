/**
 * \file    CANGatekeeper.c
 * \author  gross10
 * \date    2013-11-16
 *
 * \version 1.0
 *  create this file
 *
 * \brief   gatekeeper for the resource "can"
 *
 * \todo    tmp-buffer for filter setup replace with direct filter-register access
 * \todo    safety breakout (while{}, vCANTx) handlen with the tx-interrupt
 * \todo    maybe change the CAN_data_t to a bit-filed/union construct -> storage/speed optimization
 *
 * \defgroup cangatekeeper CANGatekeeper
 * \brief   gatekeeper for the node-communication
 * \ingroup hw_task
 *
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* application */
#include "AppConfig.h" /* global application-settings */
#include "CANGatekeeper.h"

/* HW-library */
#include "stm32f4xx_can.h" /* for CAN rx/tx data types */
#include "../lib/can.h" /* CAN library */


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* GOTO-protocol */
#define EMERGENCY_STOP_OBSTACLE_TX_MASK 0x01
#define GOTO_X_OFFSET_D0				4
#define GOTO_X_OFFSET_D1                4
#define GOTO_X_TX_MASK_D0               0xFF0
#define GOTO_X_TX_MASK_D1               0x0F
#define GOTO_X_RX_MASK_D0               0xFF
#define GOTO_X_RX_MASK_D1               0xF0
#define GOTO_Y_OFFSET_D1                8
#define GOTO_Y_OFFSET_D2                0
#define GOTO_Y_TX_MASK_D1               0xF00
#define GOTO_Y_TX_MASK_D2               0xFF
#define GOTO_Y_RX_MASK_D1               0x0F
#define GOTO_Y_RX_MASK_D2               0xFF
#define GOTO_ANGLE_OFFSET_D3            2
#define GOTO_ANGLE_OFFSET_D4            6
#define GOTO_ANGLE_TX_MASK_D3           0x3FC
#define GOTO_ANGLE_TX_MASK_D4           0x03
#define GOTO_ANGLE_RX_MASK_D3           0xFF
#define GOTO_ANGLE_RX_MASK_D4           0xC0
#define GOTO_SPEED_OFFSET_D4            2
#define GOTO_SPEED_OFFSET_D5            6
#define GOTO_SPEED_TX_MASK_D4           0xFC
#define GOTO_SPEED_TX_MASK_D5           0x03
#define GOTO_SPEED_RX_MASK_D4           0x3F
#define GOTO_SPEED_RX_MASK_D5           0xC0
#define GOTO_BARRIER_OFFSET_D5          10
#define GOTO_BARRIER_OFFSET_D6          2
#define GOTO_BARRIER_OFFSET_D7          6
#define GOTO_BARRIER_TX_MASK_D5         0xFC00
#define GOTO_BARRIER_TX_MASK_D6         0x03FC
#define GOTO_BARRIER_TX_MASK_D7         0x0003
#define GOTO_BARRIER_RX_MASK_D5         0x3F
#define GOTO_BARRIER_RX_MASK_D6         0xFF
#define GOTO_BARRIER_RX_MASK_D7         0xC0
#define GOTO_DIRECTION_OFFSET_D7        5
#define GOTO_DIRECTION_TX_MASK_D7       0x01
#define GOTO_DIRECTION_RX_MASK_D7       0x20

#define GOTO_POINT_1_X_OFFSET_D4        4
#define GOTO_POINT_1_X_OFFSET_D5        4
#define GOTO_POINT_1_X_TX_MASK_D4       0x0F
#define GOTO_POINT_1_X_TX_MASK_D5       0x70
#define GOTO_POINT_1_X_RX_MASK_D4       0xF0
#define GOTO_POINT_1_X_RX_MASK_D5       0x07
#define GOTO_POINT_1_Y_OFFSET_D5        3
#define GOTO_POINT_1_Y_OFFSET_D6        5
#define GOTO_POINT_1_Y_TX_MASK_D5       0x1F
#define GOTO_POINT_1_Y_TX_MASK_D6       0x60
#define GOTO_POINT_1_Y_RX_MASK_D5       0xF8
#define GOTO_POINT_1_Y_RX_MASK_D6       0x03
#define GOTO_POINT_2_X_OFFSET_D6        2
#define GOTO_POINT_2_X_OFFSET_D7        6
#define GOTO_POINT_2_X_TX_MASK_D6       0x3F
#define GOTO_POINT_2_X_TX_MASK_D7       0x40
#define GOTO_POINT_2_X_RX_MASK_D6       0xFC
#define GOTO_POINT_2_X_RX_MASK_D7       0x01
#define GOTO_POINT_2_Y_OFFSET_D7        1
#define GOTO_POINT_2_Y_TX_MASK_D7       0x7F
#define GOTO_POINT_2_Y_RX_MASK_D7       0xFE
#define GOTO_TIME_OFFSET_D0             0
#define GOTO_TIME_TX_MASK_D0            0x000000FF
#define GOTO_TIME_OFFSET_D1             8
#define GOTO_TIME_TX_MASK_D1            0x0000FF00
#define GOTO_TIME_OFFSET_D2             16
#define GOTO_TIME_TX_MASK_D2            0x00FF0000

/* ELP-protocol */
#define ELP_X_OFFSET_D1                 8
#define ELP_X_TX_MASK_D0                0xFF
#define ELP_X_TX_MASK_D1                0xF00
#define ELP_X_RX_MASK_D1                0x0F
#define ELP_Y_OFFSET_D1                 4
#define ELP_Y_OFFSET_D2                 4
#define ELP_Y_TX_MASK_D1                0x0F
#define ELP_Y_TX_MASK_D2                0xFF0
#define ELP_Y_RX_MASK_D1                0xF0
#define ELP_ANGLE_OFFSET_D4             8
#define ELP_ANGLE_TX_MASK_D3            0xFF
#define ELP_ANGLE_TX_MASK_D4            0x300
#define ELP_ANGLE_RX_MASK_D4            0x03
#define ELP_ID_OFFSET_D4                2
#define ELP_ID_TX_MASK_D4               0x0F
#define ELP_ID_RX_MASK_D4               0x3C

/* GIP-protocol */
#define GIP_COLOR_OFFSET_D0             7
#define GIP_COLOR_TX_MASK_D0            0x01
#define GIP_COLOR_RX_MASK_D0            0x80
#define GIP_ENEMY_OFFSET_D0             5
#define GIP_ENEMY_TX_MASK_D0            0x03
#define GIP_ENEMY_RX_MASK_D0            0x60
#define GIP_CONFEDERATE_OFFSET_D0       4
#define GIP_CONFEDERATE_TX_MASK_D0      0x01
#define GIP_CONFEDERATE_RX_MASK_D0      0x10
#define GIP_ENEMY_1_SIZE_OFFSET_D0      2
#define GIP_ENEMY_1_SIZE_TX_MASK_D0     0x3C
#define GIP_ENEMY_1_SIZE_RX_MASK_D0     0x0F
#define GIP_ENEMY_1_SIZE_OFFSET_D1      6
#define GIP_ENEMY_1_SIZE_TX_MASK_D1     0x03
#define GIP_ENEMY_1_SIZE_RX_MASK_D1     0xC0
#define GIP_ENEMY_2_SIZE_OFFSET_D1      0
#define GIP_ENEMY_2_SIZE_TX_MASK_D1     0x3F
#define GIP_ENEMY_2_SIZE_RX_MASK_D1     0x3F


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static xQueueHandle qCANRx;
static xQueueHandle qCANTx;
static CAN_listener_t can_listener_buffer[CAN_LISTENER_BUFFER_SIZE];
static uint8_t current_buffer_size = 0;


/* Private function prototypes -----------------------------------------------*/
static void txPositionResponse(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t);
static inline CAN_data_t rxEmergencyStop(CanRxMsg*);
static inline CAN_data_t rxGotoXY(CanRxMsg*);
static inline CAN_data_t rxGotoStateResponse(CanRxMsg*);
static inline CAN_data_t rxPositionResponse(CanRxMsg*);
static inline CAN_data_t rxStartConfigurationSet(CanRxMsg*);
static void vCANRx(void*);
static void vCANTx(void*);
static void catchCANRx(CanRxMsg);


/* Private functions ---------------------------------------------------------*/

/**
 * \fn          initCANGatekeeper
 * \brief       Initialisation the gatekeeper for CAN communications. Must be
 *              the last module in the init-sequency!
 *
 * \param[in]   None
 * \return      None
 */
void initCANGatekeeper(void)
{
    /* interface initialisation */
    initCAN(catchCANRx); /* for settings: can.h */

    /* create the queues */
    qCANRx = xQueueCreate(CAN_QUEUE_LENGTH,sizeof(CanRxMsg)); /* RX-Message Queue */
    qCANTx = xQueueCreate(CAN_QUEUE_LENGTH,sizeof(CanTxMsg)); /* TX-Message Queue */

#ifdef DEBUGGING
    vQueueAddToRegistry(qCANRx, (signed char*) "CAN RX");
    vQueueAddToRegistry(qCANTx, (signed char*) "CAN TX");
#endif

    /* create tasks */
    xTaskCreate( vCANRx, ( signed char * ) CAN_RX_TASK_NAME, CAN_STACK_SIZE, NULL, CAN_TASK_PRIORITY, NULL );
    xTaskCreate( vCANTx, ( signed char * ) CAN_TX_TASK_NAME, CAN_STACK_SIZE, NULL, CAN_TASK_PRIORITY, NULL );
}


/**
 * \fn          setQueueCANListener
 * \brief       set a new listener to a CAN-message
 *
 * \param       queue   pointer to a queue
 * \param       id      corresponding CAN-message ID
 * \return      None
 */
void setQueueCANListener(xQueueHandle* queue, uint16_t id)
{
    if(current_buffer_size < CAN_LISTENER_BUFFER_SIZE)
    {
        can_listener_buffer[current_buffer_size].id = id;
        can_listener_buffer[current_buffer_size].queue = queue;
        can_listener_buffer[current_buffer_size].function = 0;
        current_buffer_size++;

        setCANFilter(id);
    }
    else
    {
        #ifdef DEBUGGING
            while(1){};
        #endif
    }
}


/**
 * \fn          setFunctionCANListener
 * \brief       set a new listener to a CAN-message
 *
 * \param[in]   function    pointer to a function
 * \param[in]   id          corresponding CAN-message ID
 * \return      None
 */
void setFunctionCANListener(CAN_function_listener_t function, uint16_t id)
{
    if(current_buffer_size < CAN_LISTENER_BUFFER_SIZE)
    {
        can_listener_buffer[current_buffer_size].id = id;
        can_listener_buffer[current_buffer_size].queue = 0;
        can_listener_buffer[current_buffer_size].function = function;
        current_buffer_size++;

        setCANFilter(id);
    }
    else
    {
        #ifdef DEBUGGING
            while(1){};
        #endif
    }
}


/**
 * \fn      createCANMessage
 * \brief   this function create a CAN-message and send it to the message-queue
 *
 * \param[in]   id Id of the CAN-message
 * \param[in]   dlc Number of data-paket
 * \param[in]   data Data-paket of the CAN-message
 * \retval      None
 */
void createCANMessage(uint16_t id, uint8_t dlc, uint8_t data[8])
{
    CanTxMsg tx_message;

    tx_message.StdId = id;
    tx_message.ExtId = id;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = dlc;

    /* write the data in to the CAN-message */
    while(dlc > 0 ){
        tx_message.Data[dlc-1] = data[dlc-1];
        dlc--;
    }

    /* send the message to the queue depenced to the ID priority*/
    if(tx_message.StdId <= ID_HIGH_LEVEL_PRIORITY)
    {
        xQueueSendToFront(qCANTx, &tx_message,0);
    }
    else
    {
        xQueueSendToBack(qCANTx, &tx_message,0);
    }
}


/**
 * \fn      txEmergencyShutdown
 * \brief   stop the robo and puts him to default-state
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txEmergencyShutdown()
{
    createCANMessage(EMERGENCY_SHUTDOWN,0,0);
}


/**
 * \fn      txEmergencyStop
 * \brief   stop the robo
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   obstacle_id id of the obstacle (0 = playground, 1 = enemey)
 * \return      None
 */
void txEmergencyStop(uint8_t obstacle_id)
{
    uint8_t data = obstacle_id & EMERGENCY_STOP_OBSTACLE_TX_MASK;
    createCANMessage(EMERGENCY_STOP,1,&data);
}


/**
 * \fn      txStopDrive
 * \brief   stop the robo
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txStopDrive()
{
    createCANMessage(STOP_DRIVE,0,0);
}


/**
 * \fn      txGotoXY
 * \brief   drive-command
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   x x-coordinate of the end position [12bit]
 * \param[in]   y y-coordinate of the end position [12bit]
 * \param[in]   angle angle of the end position [10bit]
 * \param[in]   speed robo-speed [8bit]
 * \param[in]   barrier flag-set of different barriers
 * \param[in]   drive_direction    0:=robo drives forward; 1:=robo drives backward
 * \return      None
 */
void txGotoXY(uint16_t x, uint16_t y, uint16_t angle, uint8_t speed, uint16_t barrier, uint8_t drive_direction)
{
    uint8_t data[8];

    data[0] = (x & GOTO_X_TX_MASK_D0) >> GOTO_X_OFFSET_D0;
    data[1] = ((x & GOTO_X_TX_MASK_D1) << GOTO_X_OFFSET_D1) | ((y & GOTO_Y_TX_MASK_D1) >> GOTO_Y_OFFSET_D1);
    data[2] = ((y & GOTO_Y_TX_MASK_D2) >> GOTO_Y_OFFSET_D2);
    data[3] = ((angle & GOTO_ANGLE_TX_MASK_D3) >> GOTO_ANGLE_OFFSET_D3);
    data[4] = ((angle & GOTO_ANGLE_TX_MASK_D4) << GOTO_ANGLE_OFFSET_D4) | ((speed & GOTO_SPEED_TX_MASK_D4) >> GOTO_SPEED_OFFSET_D4);
    data[5] = ((speed & GOTO_SPEED_TX_MASK_D5) << GOTO_SPEED_OFFSET_D5) | ((barrier & GOTO_BARRIER_TX_MASK_D5) >> GOTO_BARRIER_OFFSET_D5);
    data[6] = ((barrier & GOTO_BARRIER_TX_MASK_D6) >> GOTO_BARRIER_OFFSET_D6);
    data[7] = ((barrier & GOTO_BARRIER_TX_MASK_D7) << GOTO_BARRIER_OFFSET_D7) | ((drive_direction & GOTO_DIRECTION_TX_MASK_D7) << GOTO_DIRECTION_OFFSET_D7);

    /* send the Goto-command to the queue */
    createCANMessage(GOTO_XY,8,data);
}


/**
 * \fn      txGotoConfirm
 * \brief   confirm the reception of the GotoXY-command
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txGotoConfirm()
{
    createCANMessage(GOTO_CONFIRM,0,0);
}


/**
 * \fn      txGotoStateRequest
 * \brief   request a state of the currently running GotoXY-command
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txGotoStateRequest()
{
    createCANMessage(GOTO_STATE_REQUEST,0,0);
}


/**
 * \fn      txGotoStateResponse
 * \brief   respone the remaining time to the end position from last GotoXY-cmd
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   time remaining time to the end position [24bit]
 * \return      None
 */
void txGotoStateResponse(uint32_t time)
{
    uint8_t data[3];

    data[0] = (time & GOTO_TIME_TX_MASK_D0) >> GOTO_TIME_OFFSET_D0;
    data[1] = (time & GOTO_TIME_TX_MASK_D1) >> GOTO_TIME_OFFSET_D1;
    data[2] = (time & GOTO_TIME_TX_MASK_D2) >> GOTO_TIME_OFFSET_D2;

    createCANMessage(GOTO_STATE_RESPONSE,3,data);
}


/**
 * \fn      txNaviPositionRequest
 * \brief   request the position-data of the navi-node
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txNaviPositionRequest()
{
    createCANMessage(NAVI_POSITION_REQUEST,0,0);
}


/**
 * \fn      txNaviPositionResponse
 * \brief   estimated position of the navi-node
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   x x-coordinate of the position [12bit]
 * \param[in]   y y-coordinate of the position [12bit]
 * \param[in]   angle angle of the end position [10bit]
 * \param[in]   speed robo-speed [8bit]
 * \param[in]   id
 * \return      None
 */
void txNaviPositionResponse(int16_t x, int16_t y, int16_t angle, uint8_t id)
{
    txPositionResponse(NAVI_POSITION_RESPONSE,x,y,angle,id);
}


/**
 * \fn      txKalmanPositionRequest
 * \brief   request the position-data of the kalman-filter
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txKalmanPositionRequest()
{
    createCANMessage(KALMAN_POSITION_REQUEST,0,0);
}


/**
 * \fn      txKalmanPositionResponse
 * \brief   estimated position of the ultrasonic-node
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   x x-coordinate of the position [12bit]
 * \param[in]   y y-coordinate of the position [12bit]
 * \param[in]   angle angle of the end position [10bit]
 * \param[in]   id
 * \return      None
 */
void txKalmanPositionResponse(uint16_t x, uint16_t y, uint16_t angle, uint8_t id)
{
    txPositionResponse(KALMAN_POSITION_RESPONSE,x,y,angle,id);
}


/**
 * \fn      txEnemey1PositionRequest
 * \brief   request the position-data of enemey 1
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txEnemey1PositionRequest()
{
    createCANMessage(ENEMEY_1_POSITION_REQUEST,0,0);
}


/**
 * \fn      txEnemey1PositionResponse
 * \brief   estimated position of the enemey 1
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   x x-coordinate of the position [12bit]
 * \param[in]   y y-coordinate of the position [12bit]
 * \param[in]   angle angle of the end position [10bit]
 * \param[in]   speed robo-speed [8bit]
 * \param[in]   id
 * \return      None
 */
void txEnemey1PositionResponse(uint16_t x, uint16_t y, uint16_t angle, uint8_t id)
{
    txPositionResponse(ENEMEY_1_POSITION_RESPONSE,x,y,angle,id);
}


/**
 * \fn      txEnemey2PositionRequest
 * \brief   request the position-data of enemey 2
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txEnemey2PositionRequest()
{
    createCANMessage(ENEMEY_2_POSITION_REQUEST,0,0);
}


/**
 * \fn      txEnemey2PositionResponse
 * \brief   estimated position of the enemey 2
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   x x-coordinate of the position [12bit]
 * \param[in]   y y-coordinate of the position [12bit]
 * \param[in]   angle angle of the end position [10bit]
 * \param[in]   speed robo-speed [8bit]
 * \param[in]   id
 * \return      None
 */
void txEnemey2PositionResponse(uint16_t x, uint16_t y, uint16_t angle, uint8_t id)
{
    txPositionResponse(ENEMEY_2_POSITION_RESPONSE,x,y,angle,id);
}


/**
 * \fn      txConfederatePositionRequest
 * \brief   request the position-data of confederate
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   None
 * \return      None
 */
void txConfederatePositionRequest()
{
    createCANMessage(CONFEDERATE_POSITION_REQUEST,0,0);
}


/**
 * \fn      txConfederatePositionResponse
 * \brief   estimated position of the confiderate
 *          convert the drive-data to the Goto-protocol
 *
 * \param[in]   x x-coordinate of the position [12bit]
 * \param[in]   y y-coordinate of the position [12bit]
 * \param[in]   angle angle of the end position [10bit]
 * \param[in]   speed robo-speed [8bit]
 * \param[in]   id
 * \return      None
 */
void txConfederatePositionResponse(uint16_t x, uint16_t y, uint16_t angle, uint8_t id)
{
    txPositionResponse(CONFEDERATE_POSITION_RESPONSE,x,y,angle,id);
}


/**
 * \fn      txStartConfigurationSet
 * \brief   start settings for every node in the system
 *          convert the start-data to the GIP-protocol
 *
 * \param[in]   color team color: 0=Yellow; 1=Red [1bit]
 * \param[in]   enemy enemy quantity: 0-2 [2bit]
 * \param[in]   confederate confederate quantity [1bit]
 * \param[in]   enemy_1_size diameter of enemy 1: 0-50cm [6bit]
 * \param[in]   enemy_2_size diameter of enemy 2: 0-50cm [6bit]
 * \return      None
 */
void txStartConfigurationSet(uint8_t color, uint8_t enemy, uint8_t confederate, uint8_t enemy_1_size, uint8_t enemy_2_size)
{
	uint8_t data[2];

    data[0] = ((color & GIP_COLOR_TX_MASK_D0) << GIP_COLOR_OFFSET_D0) | ((enemy & GIP_ENEMY_TX_MASK_D0) << GIP_ENEMY_OFFSET_D0) |
            ((confederate & GIP_CONFEDERATE_TX_MASK_D0) << GIP_CONFEDERATE_OFFSET_D0) | ((enemy_1_size & GIP_ENEMY_1_SIZE_TX_MASK_D0) >> GIP_ENEMY_1_SIZE_OFFSET_D0);
    data[1] = ((enemy_1_size & GIP_ENEMY_1_SIZE_TX_MASK_D1) << GIP_ENEMY_1_SIZE_OFFSET_D1) | ((enemy_2_size & GIP_ENEMY_2_SIZE_TX_MASK_D1) << GIP_ENEMY_2_SIZE_OFFSET_D1);

    createCANMessage(START_CONFIGURATION_SET,2,data);
}


/**
 * \fn		txStartConfigurationConfirm
 * \brief	confirm the reception of the start data
 */
void txStartConfigurationConfirm()
{
    createCANMessage(START_CONFIGURATION_CONFIRM,0,0);
}


/**
 * \fn		txCheckNaviRequest
 * \brief	check if the navi-node is available
 */
void txCheckNaviRequest()
{
	createCANMessage(CHECK_NAVI_REQUEST,0,0);
}


/**
 * \fn		txCheckNaviResponse
 * \brief	navi-node confirm
 */
void txCheckNaviResponse()
{
	createCANMessage(CHECK_NAVI_RESPONSE,0,0);
}


/**
 * \fn		txCheckDriveRequest
 * \brief	check if the drive-node is available
 */
inline void txCheckDriveRequest()
{
	createCANMessage(CHECK_DRIVE_REQUEST,0,0);
}


/**
 * \fn		txCheckDriveResponse
 * \brief	drive-node confirm
 */
void txCheckDriveResponse()
{
	createCANMessage(CHECK_DRIVE_RESPONSE,0,0);
}


/**
 * \fn      txPositionResponse
 * \brief   generic function for the ELP-response
 *
 * \param[in]   x x-coordinate of the position [12bit]
 * \param[in]   y y-coordinate of the position [12bit]
 * \param[in]   angle angle of the end position [10bit]
 * \param[in]   speed robo-speed [8bit]
 * \param[in]   id
 * \return      None
 */
static void txPositionResponse(uint16_t can_id, uint16_t x, uint16_t y, uint16_t angle, uint8_t id)
{
    uint8_t data[5];

    data[0] = x & ELP_X_TX_MASK_D0;
    data[1] = ((x & ELP_X_TX_MASK_D1) >> ELP_X_OFFSET_D1) | ((y & ELP_Y_TX_MASK_D1) << ELP_Y_OFFSET_D1);
    data[2] = (y & ELP_Y_TX_MASK_D2) >> ELP_Y_OFFSET_D2;
    data[3] = angle & ELP_ANGLE_TX_MASK_D3;
    data[4] = ((angle & ELP_ANGLE_TX_MASK_D4) >> ELP_ANGLE_OFFSET_D4) | ((id & ELP_ID_TX_MASK_D4) << ELP_ID_OFFSET_D4);

    createCANMessage(can_id,5,data);
}


/**
 * \fn      rxEmergencyStop
 * \brief   convert the bus-data to Goto Emergency stop
 *
 * \param[in]   rx_message pointer to the can-data
 * \return      goto state response
 */
static inline CAN_data_t rxEmergencyStop(CanRxMsg* rx_message)
{
    /* data buffer */
    CAN_data_t message_data;

    /* convert to emergency stop */
    message_data.stop_obstacle_id = rx_message->Data[0];

    return message_data;
}


/**
 * \fn      rxGotoXY
 * \brief   convert the bus-data to GLP
 *
 * \param[in]   rx_message pointer to the can-data
 * \return      goto-data
 */
static inline CAN_data_t rxGotoXY(CanRxMsg* rx_message)
{
    /* data buffer */
    CAN_data_t message_data;

    /* convert to GLP */
    message_data.goto_x = ((rx_message->Data[0] & GOTO_X_RX_MASK_D0) << GOTO_X_OFFSET_D0) | ((rx_message->Data[1] & GOTO_X_RX_MASK_D1) >> GOTO_X_OFFSET_D1);
    message_data.goto_y = ((rx_message->Data[1] & GOTO_Y_RX_MASK_D1) << GOTO_Y_OFFSET_D1) | ((rx_message->Data[2] & GOTO_Y_RX_MASK_D2) >> GOTO_Y_OFFSET_D2);
    message_data.goto_angle = ((rx_message->Data[3] & GOTO_ANGLE_RX_MASK_D3) << GOTO_ANGLE_OFFSET_D3) | ((rx_message->Data[4] & GOTO_ANGLE_RX_MASK_D4) >> GOTO_ANGLE_OFFSET_D4);
    message_data.goto_speed = ((rx_message->Data[4] & GOTO_SPEED_RX_MASK_D4) << GOTO_SPEED_OFFSET_D4) | ((rx_message->Data[5] & GOTO_SPEED_RX_MASK_D5) >> GOTO_SPEED_OFFSET_D5);
    message_data.goto_barrier = ((rx_message->Data[5] & GOTO_BARRIER_RX_MASK_D5) << GOTO_BARRIER_OFFSET_D5) | ((rx_message->Data[6] & GOTO_BARRIER_RX_MASK_D6) << GOTO_BARRIER_OFFSET_D6);
    message_data.goto_barrier |= ((rx_message->Data[7] & GOTO_BARRIER_RX_MASK_D7) >> GOTO_BARRIER_OFFSET_D7);
    message_data.goto_direction = ((rx_message->Data[7] & GOTO_DIRECTION_RX_MASK_D7) >> GOTO_DIRECTION_OFFSET_D7);

    return message_data;
}


/**
 * \fn      rxGotoStateResponse
 * \brief   convert the bus-data to Goto state response
 *
 * \param[in]   rx_message pointer to the can-data
 * \return      goto state response
 */
static inline CAN_data_t rxGotoStateResponse(CanRxMsg* rx_message)
{
    /* data buffer */
    CAN_data_t message_data;

    /* convert to state response */
    message_data.state_time = (rx_message->Data[0] << GOTO_TIME_OFFSET_D0) | (rx_message->Data[1] << GOTO_TIME_OFFSET_D1) | (rx_message->Data[2] << GOTO_TIME_OFFSET_D2);

    return message_data;
}


/**
 * \fn      rxPositionResponse
 * \brief   convert the bus-data to ELP
 *
 * \param[in]   rx_message pointer to the can-data
 * \return      elp-data
 */
static inline CAN_data_t rxPositionResponse(CanRxMsg* rx_message)
{
    /* data buffer */
    CAN_data_t message_data;

    /* convert to ELP */
    message_data.elp_x = rx_message->Data[0] | ((rx_message->Data[1] & ELP_X_RX_MASK_D1) << ELP_X_OFFSET_D1);
    message_data.elp_y = ((rx_message->Data[1] & ELP_Y_RX_MASK_D1) >> ELP_Y_OFFSET_D1) | (rx_message->Data[2] << ELP_Y_OFFSET_D2);
    message_data.elp_angle = rx_message->Data[3] | ((rx_message->Data[4] & ELP_ANGLE_RX_MASK_D4) << ELP_ANGLE_OFFSET_D4);
    message_data.elp_id = (rx_message->Data[4] & ELP_ID_RX_MASK_D4) >> ELP_ID_OFFSET_D4;

    return message_data;
}


/**
 * \fn      rxStartConfigurationSet
 * \brief   convert the bus-data to GIP
 *
 * \param[in]   rx_message pointer to the can-data
 * \return      gip-data
 */
static inline CAN_data_t rxStartConfigurationSet(CanRxMsg* rx_message)
{
    /* data buffer */
    CAN_data_t message_data;

    message_data.gip_color = (rx_message->Data[0] & GIP_COLOR_RX_MASK_D0) >> GIP_COLOR_OFFSET_D0;
    message_data.gip_enemy = (rx_message->Data[0] & GIP_ENEMY_RX_MASK_D0) >> GIP_ENEMY_OFFSET_D0;
    message_data.gip_confederate = (rx_message->Data[0] & GIP_CONFEDERATE_RX_MASK_D0) >> GIP_CONFEDERATE_OFFSET_D0;
    message_data.gip_enemy_1_size = (rx_message->Data[0] & GIP_ENEMY_1_SIZE_RX_MASK_D0) << GIP_ENEMY_1_SIZE_OFFSET_D0;
    message_data.gip_enemy_1_size |= (rx_message->Data[1] & GIP_ENEMY_1_SIZE_RX_MASK_D1) >> GIP_ENEMY_1_SIZE_OFFSET_D1;
    message_data.gip_enemy_2_size = (rx_message->Data[1] & GIP_ENEMY_2_SIZE_RX_MASK_D1) >> GIP_ENEMY_2_SIZE_OFFSET_D1;

    return message_data;
}


/**
 * \fn          vCANRx
 * \brief       Receive task for CAN-messages
 *
 * \param[in]   None
 * \return      None
 */
static void vCANRx(void* pvParameters )
{
    /* Number of the function/queue entry */
    uint8_t i = 0;

    /* buffers for a received CAN-message */
    CanRxMsg rx_message;
    CAN_data_t message_data;

#ifdef DEBUGGING
    unsigned portBASE_TYPE uxHighWaterMark;
    /* Inspect our own high water mark on entering the task. */
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
#endif


    /* for ever */
    for(;;)
    {
        /* if a new CAN-Message is received, call the rx-function or push the data to a queue */
        if(xQueueReceive(qCANRx,&rx_message,portMAX_DELAY) == pdTRUE)
        {
            for(i = 0; i < current_buffer_size; i++)
            {
                if(can_listener_buffer[i].id == rx_message.StdId)
                {
                    /* convert the received data to the specific protocol */
                    switch(can_listener_buffer[i].id)
                    {
                        case EMERGENCY_STOP:
                            message_data = rxEmergencyStop(&rx_message);
                        break;

                        case GOTO_XY:
                            message_data = rxGotoXY(&rx_message);
                        break;

                        case GOTO_STATE_RESPONSE:
                            message_data = rxGotoStateResponse(&rx_message);
                        break;

                        case NAVI_POSITION_RESPONSE:
                        case KALMAN_POSITION_RESPONSE:
                        case ENEMEY_1_POSITION_RESPONSE:
                        case ENEMEY_2_POSITION_RESPONSE:
                        case CONFEDERATE_POSITION_RESPONSE:
                            message_data = rxPositionResponse(&rx_message);
                            break;
                        case START_CONFIGURATION_SET:
                            message_data = rxStartConfigurationSet(&rx_message);
                    }

                    /* send the message-data to the registered queue */
                    if(can_listener_buffer[i].queue != 0)
                    {
                        xQueueSendToBack(can_listener_buffer[i].queue, &message_data,0);
                    }
                    /* call the registered rx-function */
                    if(can_listener_buffer[i].function != 0)
                    {
                        (can_listener_buffer[i].function)(can_listener_buffer[i].id, &message_data);//TODO
                    }
                }
            }
        }
#ifdef DEBUGGING
        /* Calling the function will have used some stack space, we would
        therefore now expect uxTaskGetStackHighWaterMark() to return a
        value lower than when it was called on entering the task. */
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
#endif
    }
}


/**
 * \fn          vCANTx
 * \brief       Transmit task for CAN-messages
 *
 * \param[in]   None
 * \return      None
 */
static void vCANTx(void* pvParameters )
{
    CanTxMsg tx_Message; /* buffer for a received CAN-message */
    portTickType tx_wait_time;
    uint8_t max_wait_time = 0;
#ifdef DEBUGGING
    unsigned portBASE_TYPE uxHighWaterMark;
#endif

    /* We need to initialise tx_wait_time prior to the first call to vTaskDelayUntil(). */
    tx_wait_time = xTaskGetTickCount();

#ifdef DEBUGGING
    /* Inspect our own high water mark on entering the task. */
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
#endif

    /* for ever */
    for(;;)
    {
        /* if a new CAN-Message wait for transmitting */
        if(xQueueReceive(qCANTx,(void *)&tx_Message,portMAX_DELAY) == pdTRUE)
        {
            /* wait until one message-box is empty or timeout runs out */
            while(!(CAN_INTERFACE->TSR & CAN_TSR_TME0 ||
                    CAN_INTERFACE->TSR & CAN_TSR_TME1 ||
                    CAN_INTERFACE->TSR & CAN_TSR_TME2))
            {
                vTaskDelayUntil( &tx_wait_time, 10 / portTICK_RATE_MS); /* wait for 10ms */
                max_wait_time++;

                /* after CAN_TX_MAX_TIME skip waiting -> the message will not sended */
                if(max_wait_time >= CAN_TX_MAX_WAIT_TIME)
                {
                    break;
                }
            }

            /* transmit the message */
            CAN_Transmit(CAN_INTERFACE, &tx_Message);

            /* reset breakout timer */
            max_wait_time = 0;
        }
#ifdef DEBUGGING
        /* Calling the function will have used some stack space, we would
        therefore now expect uxTaskGetStackHighWaterMark() to return a
        value lower than when it was called on entering the task. */
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
#endif
    }
}


/**
 * \fn          catchCANRx
 * \brief       called by the CAN-RX interrupthandler
 *
 * \param       can_rx_data: received data from the CAN-bus
 * \return      None
 */
static void catchCANRx(CanRxMsg can_rx_data)
{
    xQueueSendToBackFromISR(qCANRx,&can_rx_data,0);
}

/**
 * @}
 */
