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

/* lib */
#include "../../lib/servo.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* mammoth nodes configs */
/* node 1 */
node_t node_mammoth_1 =
{
    {
        .id = 1,                            /*!<node id */
        .points = 4,                        /*!<node points */
        .percent = 0.19,                    /*!<percent of the total points [%]*/
        .time = 5,                          /*!<estimated node time [s]*/
        .x = 0.62,                          /*!<node x position [m]*/
        .y = 0.49,                          /*!<node y position [m]*/
        .pool_id = NODE_MAMMOTH_POOL_ID,    /*!<node pool id */
        .angle = 180,                       /*!<node arrive direction */
        .node_state = NODE_UNDONE,          /*!<node state */
    },
    doMammothNode
};


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initNodeResources
 * \brief       Function to initialise all the resources needed by the node task
 *
 * \TODO init sensors (incl. exti)
 *
 * \param[in]   None
 * \return      None
 */
void initNodeResources()
{
	/* Initialise fresco/separation servo */
	initServo_1();
	/* Initialise launcher servo */
	initServo_2();
}


/**
 * @}
 */


