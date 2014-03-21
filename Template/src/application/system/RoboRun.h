/**
 * \file    RoboRun.h
 * \author  gross10
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy run state
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef ROBORUN_H_
#define ROBORUN_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* RTOS */
#define SYSTEM_NODE_TASK_1_NAME           "Node Task 1"
#define SYSTEM_NODE_TASK_2_NAME           "Node Task 2"
#define SYSTEM_NODE_STACK_SIZE            configMINIMAL_STACK_SIZE
#define SYSTEM_NODE_TASK_PRIORITY         (configMAX_PRIORITIES - 5UL) /*!< priority 2 */

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
extern void initRoboRunState();
extern void runRoboRunState();


#endif /* ROBORUN_H_ */
/**
 * @}
 */
