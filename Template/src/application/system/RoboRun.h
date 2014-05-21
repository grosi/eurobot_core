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
#define SYSTEM_NODE_TASK_NAME       "Node Task"
#define SYSTEM_NODE_STACK_SIZE      4*configMINIMAL_STACK_SIZE
#define SYSTEM_NODE_TASK_PRIORITY   (configMAX_PRIORITIES - 5UL) /*!< priority 2 */
#define ROBORUN_TIMEOUT             1000 /*!< wait for max. 1s */

/* game configurations */
#define ROBO_AVERAGE_SPEED          0.5  /*!< [m/s] */
#define ENEMY_GRID_SIZE_X           100  /*!< [mm] */
#define ENEMY_GRID_SIZE_Y           100  /*!< [mm] */
#define ENEMY_GRID_CENTER_WEIGHT    5
#define ENEMY_GRID_FRAME_WEIGHT     3

/* node weighting settings */
#define NODE_ARRIVE_FRAME       100 /*!< [mm] */
#define NODE_WORST_ARRIVE       4
#define NODE_BAD_ARRIVE         3
#define NODE_WELL_ARRIVE        2
#define NODE_PERFECT_ARRIVE     1
#define NODE_REPEAT             2

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initRoboRunState();
extern uint8_t setConfigRoboRunState(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
extern void setConfigRoboRunState2Default();
extern void setFunnyBreak();
extern void runRoboRunState(portTickType*);


#endif /* ROBORUN_H_ */
/**
 * @}
 */
