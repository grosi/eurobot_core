/**
 * \file    FirePoolNode.c
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

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "../Rangefinder.h"
#include "../CANGatekeeper.h"
#include "NodeConfig.h"
#include "FirePoolNode.h"
/* lib */
#include "lib/air.h"
#include "lib/servo.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doFireWallInversNode
 * \brief       Tries to complete the fire node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doFirePoolNode(node_param_t* param)
{

    setAir(1);
    vTaskDelay(2000);
    setAir(0);

    setServo_1(SERVO_POS_AIR_DOWN);
    vTaskDelay(2000);
    setServo_1(SERVO_POS_AIR_THIRD_FIRE);
    vTaskDelay(2000);
    setServo_1(SERVO_POS_AIR_SECOND_FIRE);
    vTaskDelay(2000);
    setServo_1(SERVO_POS_AIR_FIRST_FIRE);
    vTaskDelay(2000);
    setServo_1(SERVO_POS_AIR_UP);

    vTaskDelay(2000);
    setServo_2(SERVO_POS_NET_LAUNCH);
    vTaskDelay(2000);
    setServo_2(SERVO_POS_NET_LOAD);
    vTaskDelay(2000);
//    /* local variables */
//    /* Variable for CAN RX */
//    CAN_data_t CAN_buffer;
//    uint8_t CAN_ok = pdFALSE;
//
//    /* Activate rangefinder */
//    vTaskResume(xRangefinderTask_Handle);
//
//    /* Move the separation all the way out */
//    //setServo_1(SERVO_POS_FRESCO_OUT);
//
//    /* Wait some time while servo moves */
//    vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);
//
//    /* Drive through fire from NORTH */
//    if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
//    {
//        txGotoXY(param->x, param->y-FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER, GOTO_DRIVE_FORWARD);
//
//    }
//    /* Drive through fire from EAST */
//    else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
//    {
//        txGotoXY(param->x-FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER, GOTO_DRIVE_FORWARD);
//    }
//    /* Drive through fire from SOUTH */
//    else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
//    {
//        txGotoXY(param->x, param->y+FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER, GOTO_DRIVE_FORWARD);
//    }
//    /* Drive through fire from WEST */
//    else
//    {
//        txGotoXY(param->x+FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER, GOTO_DRIVE_FORWARD);
//    }
//
//    /* Receive GoTo confirmation */
//    CAN_ok = xQueueReceive(qGotoConfirm, &CAN_buffer, FIRE_NODE_ACK_DELAY / portTICK_RATE_MS);
//
//    /* check CAN response */
//    if(CAN_ok != pdTRUE)
//    {
////        /* Suspend rangefinder safely */
////        suspendRangefinderTask();
////
////        param->node_state = GOTO_CAN_ERROR;
////        return;
//    }
//
//    //TODO add function to chek if there is any enemy or friend in my way while driving
//
//    /* Put seperation in after driving through the fire */
//    //setServo_1(SERVO_POS_FRESCO_IN);
//
//    /* Wait some time while servo moves */
//    vTaskDelay(SERVO_MOVING_DELAY / portTICK_RATE_MS);
//
//    /* Fire has fallen */
//    if(Rangefinder_flag_SeAlarmUS == 0){
//
//        param->node_state = NODE_FINISH_SUCCESS;
//    }
//
//    /* Suspend rangefinder safely */
//    suspendRangefinderTask();

}

/**
 * @}
 */
