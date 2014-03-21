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

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../app_config.h"
#include "node_config.h"
#include "fire_node.h"
#include "fresco_node.h"
#include "mammut_node.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* mammut nodes configs */
/* node 1 */
node_t node_mammut_1 =
{
    {
        0,                  /*!<node id */
        4,                  /*!<node points */
        0.19,               /*!<percent of the total points [%]*/
        5,                  /*!<estimated node time [s]*/
        0.62,               /*!<node x position [m]*/
        0.49,               /*!<node y position [m]*/
        NODE_MAMMUT_POOL_ID,/*!<node pool id */
        NODE_UNDONE,        /*!<node state */
    },
    doMammutNode
};


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * @}
 */


