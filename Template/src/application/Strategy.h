/**
 * \file    Strategy.h
 * \author  gross10
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Task for the strategy
 *
 * \defgroup strategy Strategy
 * \brief   Strategy task
 * \ingroup app_task
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef Strategy_TASK_H_
#define Strategy_TASK_H_

/* exported typedef -----------------------------------------------------------*/

/* exported define ------------------------------------------------------------*/
/* Strategy-task RTOS configuration */
#define STRATEGY_SCHEDULER_TASK_NAME        "Strategy Task"
#define STRATEGY_NODE_TASK_1_NAME           "Node Task 1"
#define STRATEGY_NODE_TASK_2_NAME           "Node Task 2"
#define STRATEGY_QUEUE_LENGTH               20 /*!< Size of the message queues */
#define STRATEGY_STACK_SIZE                 configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define STRATEGY_SCHEDULER_TASK_PRIORITY    (configMAX_PRIORITIES - 4UL) /*!< priority 3 */
#define STRATEGY_NODE_TASK_PRIORITY         (configMAX_PRIORITIES - 5UL) /*!< priority 2 */

/* node and gamme configurations */
#define PLAYGROUND_WIDTH            3 /*[m]*/
#define PLAYGROUND_HEIGH            2 /*[m]*/
#define ENEMY_GRID_SIZE_X           0.1 /*[m]*/
#define ENEMY_GRID_SIZE_Y           0.1 /*[m]*/
#define ENEMY_GRID_CENTER_WEIGHT    1
#define ENEMY_GRID_FRAME_WEIGHT     0.5


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initStrategyTask(void);


#endif /* STRATEGY_TASK_H_ */

/**
 * @}
 */
