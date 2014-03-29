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
#define SYSTEM_NODE_STACK_SIZE      configMINIMAL_STACK_SIZE
#define SYSTEM_NODE_TASK_PRIORITY   (configMAX_PRIORITIES - 5UL) /*!< priority 2 */
#define ROBORUN_TIMEOUT             1000 /*!< wait for max. 1s */

/* game configurations */
#define PLAY_TIME                   90 /*[s]*/
#define ROBO_AVERAGE_SPEED          0.5 /*[m/s]*/
#define PLAYGROUND_WIDTH            3 /*[m]*/
#define PLAYGROUND_HEIGH            2 /*[m]*/
#define ENEMY_GRID_SIZE_X           0.1 /*[m]*/
#define ENEMY_GRID_SIZE_Y           0.1 /*[m]*/
#define ENEMY_GRID_CENTER_WEIGHT    1
#define ENEMY_GRID_FRAME_WEIGHT     0.5

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/
//extern node_t* next_node;
extern xSemaphoreHandle sSyncRoboRunNodeTask1;
extern xSemaphoreHandle sSyncRoboRunNodeTask2;
extern xSemaphoreHandle sSyncNodeTaskxDrive;
extern xSemaphoreHandle sSyncNodeTaskxServo;

/* exported function prototypes -----------------------------------------------*/
extern void initRoboRunState();
extern uint8_t setConfigRoboRunState(uint8_t, uint8_t, uint8_t);
extern void runRoboRunState(portTickType*);


#endif /* ROBORUN_H_ */
/**
 * @}
 */