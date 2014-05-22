/**
 * \file    NodeConfig.c
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

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "NodeConfig.h"
#include "FireNode.h"
#include "FireWallInversNode.h"
#include "FireWallNormalNode.h"
#include "FirePoolNode.h"
#include "NetNode.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* node fire 1 red*/
node_t node_fire_1_red =
{
    {
        .id = 1,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 6,                          	/*!<estimated node time [s]*/
        .x = 900 - FIRE1_APPROACH_DISTANCE, 	/*!<node x position [mm]*/
        .y = 1400,                          	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 0,                         	/*!<node arrive direction */
        .node_tries = 1,                   		/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 2 red*/
node_t node_fire_2_red =
{
    {
        .id = 2,                           		/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 6,                          	/*!<estimated node time [s]*/
        .x = 400,                           	/*!<node x position [mm]*/
        .y = 900 + FIRE_APPROACH_DISTANCE,		/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 270,                        	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 3 red*/
node_t node_fire_3_red =
{
    {
        .id = 3,                           		/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 6,                          	/*!<estimated node time [s]*/
        .x = 900 + FIRE_APPROACH_DISTANCE,      /*!<node x position [mm]*/
        .y = 400,                           	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 180,                       	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node wall fire pool red*/
node_t node_fire_pool_red =
{
    {
        .id = 4,                                /*!<node id */
        .points = 2,                            /*!<node points */
        .percent = 0.13,                        /*!<percent of the total points [%]*/
        .time = 10,                             /*!<estimated node time [s]*/
        .x = 900,                               /*!<node x position [mm]*/
        .y = 900 + FIREPOOL_APPROACH_DISTANCE,  /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,             /*!<node pool id */
        .angle = 270,                           /*!<node arrive direction */
        .node_tries = 1,                        /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,              /*!<node state */
    },
    doFirePoolNode
};

/* node wall fire normal 1 red*/
node_t node_fire_wall_normal_1_red =
{
    {
        .id = 5,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 8,                          	/*!<estimated node time [s]*/
        .x = 0 + FIREWALL_APPROACH_DISTANCE,	/*!<node x position [mm]*/
        .y = 1200,                          	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 180,                       	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire normal 2 red*/
node_t node_fire_wall_normal_2_red =
{
    {
        .id = 6,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 8,                          	/*!<estimated node time [s]*/
        .x = 1700,                          	/*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACH_DISTANCE,	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 270,                       	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire invers red*/
node_t node_fire_wall_invers_red =
{
    {
        .id = 7,                            	/*!<node id */
        .points = 2,                        	/*!<node points */
        .percent = 0.13,                    	/*!<percent of the total points [%]*/
        .time = 15,                         	/*!<estimated node time [s]*/
        .x = 1300,                          	/*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACH_DISTANCE,	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 270,                       	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireWallInversNode
};


/* yellow */
/* node fire 1 yellow*/
node_t node_fire_1_yellow =
{
    {
        .id = 1,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 6,                          	/*!<estimated node time [s]*/
        .x = 2100 + FIRE1_APPROACH_DISTANCE,  	/*!<node x position [mm]*/
        .y = 1400,                          	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 180,                         	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 2 yellow*/
node_t node_fire_2_yellow =
{
    {
        .id = 2,                           		/*!<node id */
        .points = 1,                       	 	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 6,                          	/*!<estimated node time [s]*/
        .x =  2600,                          	/*!<node x position [mm]*/
        .y = 900 + FIRE_APPROACH_DISTANCE,		/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 270,                        	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};

/* node fire 3 yellow*/
node_t node_fire_3_yellow =
{
    {
        .id = 3,                           		/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 6,                          	/*!<estimated node time [s]*/
        .x = 2100 - FIRE_APPROACH_DISTANCE,		/*!<node x position [mm]*/
        .y = 400,                           	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 0,                       		/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireNode
};


/* node wall fire pool red*/
node_t node_fire_pool_yellow =
{
    {
        .id = 4,                                /*!<node id */
        .points = 2,                            /*!<node points */
        .percent = 0.13,                        /*!<percent of the total points [%]*/
        .time = 10,                             /*!<estimated node time [s]*/
        .x = 2100,                              /*!<node x position [mm]*/
        .y = 900 + FIREPOOL_APPROACH_DISTANCE,  /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,             /*!<node pool id */
        .angle = 270,                           /*!<node arrive direction */
        .node_tries = 1,                        /*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,              /*!<node state */
    },
    doFirePoolNode
};

/* node wall fire normal 1 yellow*/
node_t node_fire_wall_normal_1_yellow =
{
    {
        .id = 5,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 11,                          	/*!<estimated node time [s]*/
        .x = 3000 - FIREWALL_APPROACH_DISTANCE, /*!<node x position [mm]*/
        .y = 1200,                          	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 0,                         	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire normal 2 yellow*/
node_t node_fire_wall_normal_2_yellow =
{
    {
        .id = 6,                            	/*!<node id */
        .points = 1,                        	/*!<node points */
        .percent = 0.06,                    	/*!<percent of the total points [%]*/
        .time = 11,                         	/*!<estimated node time [s]*/
        .x = 1300,                          	/*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACH_DISTANCE,	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 270,                       	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireWallNormalNode
};

/* node wall fire invers yellow*/
node_t node_fire_wall_invers_yellow =
{
    {
        .id = 7,                            	/*!<node id */
        .points = 2,                        	/*!<node points */
        .percent = 0.13,                    	/*!<percent of the total points [%]*/
        .time = 15,                         	/*!<estimated node time [s]*/
        .x = 1700,                          	/*!<node x position [mm]*/
        .y = 0 + FIREWALL_APPROACH_DISTANCE,	/*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         	/*!<node pool id */
        .angle = 270,                       	/*!<node arrive direction */
        .node_tries = 1,                    	/*!< node repeats (1 = default)*/
        .node_state = NODE_UNDONE,          	/*!<node state */
    },
    doFireWallInversNode
};


/* node net 1*/
node_t node_net_1 =
{
    {
        .id = 8,                                /*!<node id */
        .points = 6,                            /*!<node points */
        .percent = 0.40,                        /*!<percent of the total points [%]*/
        .time = 2,                              /*!<estimated node time [s]*/
        .x = 300,                               /*!<node x position [mm]*/
        .y = 1400,                              /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,             /*!<node pool id */
        .angle = 240,                           /*!<node arrive direction */
        .node_tries = 1,                        /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,      /*!<node state */
    },
    doNetNode
};

/* node net 2*/
node_t node_net_2 =
{
    {
        .id = 9,                                /*!<node id */
        .points = 6,                            /*!<node points */
        .percent = 0.40,                        /*!<percent of the total points [%]*/
        .time = 2,                              /*!<estimated node time [s]*/
        .x = 700,                               /*!<node x position [mm]*/
        .y = 1350,                              /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,             /*!<node pool id */
        .angle = 255,                           /*!<node arrive direction */
        .node_tries = 1,                        /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,      /*!<node state */
    },
    doNetNode
};

/* node net 3*/
node_t node_net_3 =
{
    {
        .id = 10,                               /*!<node id */
        .points = 6,                            /*!<node points */
        .percent = 0.40,                        /*!<percent of the total points [%]*/
        .time = 2,                              /*!<estimated node time [s]*/
        .x = 1350,                              /*!<node x position [mm]*/
        .y = 1550,                              /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,             /*!<node pool id */
        .angle = 305,                           /*!<node arrive direction */
        .node_tries = 1,                        /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,      /*!<node state */
    },
    doNetNode
};

/* node net 4*/
node_t node_net_4 =
{
    {
        .id = 11,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.40,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 1650,                          /*!<node x position [mm]*/
        .y = 1550,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 235,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};

/* node net 5*/
node_t node_net_5 =
{
    {
        .id = 12,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.40,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 2300,                          /*!<node x position [mm]*/
        .y = 1350,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 285,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};

/* node net 6*/
node_t node_net_6 =
{
    {
        .id = 13,                            /*!<node id */
        .points = 6,                        /*!<node points */
        .percent = 0.40,                    /*!<percent of the total points [%]*/
        .time = 2,                          /*!<estimated node time [s]*/
        .x = 2700,                          /*!<node x position [mm]*/
        .y = 1400,                          /*!<node y position [mm]*/
        .pool_id = NODE_NO_POOL_ID,         /*!<node pool id */
        .angle = 300,                         /*!<node arrive direction */
        .node_tries = 1,                    /*!< node repeats (1 = default)*/
        .node_state = NODE_FINISH_SUCCESS,          /*!<node state */
    },
    doNetNode
};






/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
 * @}
 */


