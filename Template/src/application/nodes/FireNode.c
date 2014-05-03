/**
 * \file    FireNode.c
 * \author  gross10
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fire node statemaschine
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
#include "FireNode.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          doFireNode
 * \brief       Tries to complete the fire node. Reports status.
 *
 * \param       param  node parameters
 * \return      None
 */
void doFireNode(node_param_t* param)
{
    /* local variables */
    /* Variable for CAN RX */
    CAN_data_t CAN_buffer;
    uint8_t CAN_ok = pdFALSE;

    /* Activate rangefinder */
    vTaskResume(xRangefinderTask_Handle);

    /* NORTH */
    if(param->angle >= NODE_NORTH_MIN_ANGLE && param->angle <= NODE_NORTH_MAX_ANGLE)
    {
        txGotoXY(param->x, param->y+FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER);

    }
    /* EAST */
    else if(param->angle >= NODE_EAST_MIN_ANGLE && param->angle <= NODE_EAST_MAX_ANGLE)
    {
        txGotoXY(param->x-FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER);
    }
    /* SOUTH */
    else if(param->angle >= NODE_SOUTH_MIN_ANGLE && param->angle <= NODE_SOUTH_MAX_ANGLE)
    {
        txGotoXY(param->x, param->y-FIRE_NODE_DELTA_GO, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER);
    }
    /* WEST */
    else
    {
        txGotoXY(param->x+FIRE_NODE_DELTA_GO, param->y, param->angle, FIRE_NODE_SPEED, FIRE_NODE_BARRIER);
    }

    /* Receive GoTo confirmation */
    CAN_ok = xQueueReceive(qGotoConfirm, &CAN_buffer, FIRE_NODE_ACK_DELAY / portTICK_RATE_MS);

    /* check CAN response */
    if(CAN_ok != pdTRUE)
    {
        /* Suspend rangefinder safely */
        suspendRangefinderTask();

        param->node_state = GOTO_CAN_ERROR;
        return;
    }

    /* Try to take semaphore from rangefinder task */
    if(xSemaphoreTake(sSyncNodeTask, FIRE_NODE_DRIVE_DELAY / portTICK_RATE_MS) == pdTRUE)
    {
        /* Semaphore received, this means an obstacle was detected! */
        //TODO
    }

    param->node_state = NODE_FINISH_SUCCESS;

    /* Suspend rangefinder safely */
    suspendRangefinderTask();

}

/**
 * @}
 */


