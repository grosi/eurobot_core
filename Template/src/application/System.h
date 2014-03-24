/**
 * \file    System.h
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
#ifndef SYSTEM_H_
#define SYSTEM_H_

/* exported typedef -----------------------------------------------------------*/

/* exported define ------------------------------------------------------------*/
/* Strategy-task RTOS configuration */
#define SYSTEM_TASK_NAME        "System Task"
//#define SYSTEM_NODE_TASK_1_NAME           "Node Task 1"
//#define SYSTEM_NODE_TASK_2_NAME           "Node Task 2"
#define SYSTEM_STACK_SIZE                 configMINIMAL_STACK_SIZE
#define SYSTEM_TASK_PRIORITY    (configMAX_PRIORITIES - 4UL) /*!< priority 3 */
//#define SYSTEM_NODE_TASK_PRIORITY         (configMAX_PRIORITIES - 5UL) /*!< priority 2 */

///* node and gamme configurations */
//#define PLAYGROUND_WIDTH            3 /*[m]*/
//#define PLAYGROUND_HEIGH            2 /*[m]*/
//#define ENEMY_GRID_SIZE_X           0.1 /*[m]*/
//#define ENEMY_GRID_SIZE_Y           0.1 /*[m]*/
//#define ENEMY_GRID_CENTER_WEIGHT    1
//#define ENEMY_GRID_FRAME_WEIGHT     0.5


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
extern void (*system_state)(portTickType*);
//extern node_t* node_task_1;
//extern node_t* node_task_2;
//extern xTaskHandle xNodeTask1_Handle;
//extern xTaskHandle xNodeTask2_Handle;

/* exported function prototypes -----------------------------------------------*/
extern void initSystemTask(void);


#endif /* SYSTEM_H_ */

/**
 * @}
 */
