/**
 * \file    NodeConfig.c
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

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "NodeConfig.h"
#include "FireNode.h"
#include "FrescoNode.h"
#include "MammothNode.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* mammoth nodes configs */
/* node 1 */
node_t node_mammoth_1 =
{
    {
        .id = 1,                  /*!<node id */
        4,                  /*!<node points */
        0.19,               /*!<percent of the total points [%]*/
        5,                  /*!<estimated node time [s]*/
        0.62,               /*!<node x position [m]*/
        0.49,               /*!<node y position [m]*/
        NODE_MAMMOTH_POOL_ID,/*!<node pool id */
        WEST,               /*!<node arrive direction */
        NODE_UNDONE,        /*!<node state */
    },
    doMammothNode
};


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * @}
 */


