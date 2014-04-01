/**
 * \file    ELP.h
 * \author  meert1
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   task to pull the ELP's
 *
 * \defgroup elp ELP
 * \brief   ELP task
 * \ingroup app_task
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef ELP_TASK_H_
#define ELP_TASK_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* ELP-task RTOS configuration */
#define ELP_TASK_NAME           "ELP Task"
#define ELP_STACK_SIZE          configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define ELP_TASK_PRIORITY       (configMAX_PRIORITIES - 3UL) /*!< priority 4 */

#define ELP_TASK_SPEED  100 /* Taskspeed in ms */

/* Define the request rates of the ELP's. Rate = Taskspeed * Requestrate */
#define ELP_NAVI_POSITION_REQUEST_RATE              4   /*!< Request the Navis's ELP every 4th time -> 400ms */
#define ELP_KALMAN_POSITION_REQUEST_RATE            6   /*!< Request the Kalman's ELP every 6th time -> 600ms */
#define ELP_ENEMY1_POSITION_REQUEST_RATE            8   /*!< Request the Enemy1's ELP every 8th time -> 800ms */
#define ELP_ENEMY2_POSITION_REQUEST_RATE            8   /*!< Request the Enemy2's ELP every 8th time -> 800ms*/
#define ELP_CONFEDERATE_POSITION_REQUEST_RATE       6   /*!< Request the Confederate's ELP every 6th time -> 600ms */

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initELPTask(void);


#endif /* ELP_TASK_H_ */

/**
 * @}
 */
