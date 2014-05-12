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
/* ELP-timer RTOS configuration */
#define ELP_NAME                "ELP Timer"
#define ELP_PERIODE             100 /*!< Time between to interruptions in ms */

/* Define the request rates of the ELP's. Rate = Taskspeed * Requestrate */
#define ELP_NAVI_POSITION_REQUEST_RATE              8   /*!< Request the Navis's ELP every 5th time -> 500ms */
#define ELP_NAVI_POSITION_REQUEST_START             0
#define ELP_KALMAN_POSITION_REQUEST_RATE            8   /*!< Request the Kalman's ELP every 5th time -> 500ms */
#define ELP_KALMAN_POSITION_REQUEST_START           0
#define ELP_CONFEDERATE_POSITION_REQUEST_RATE       8   /*!< Request the Confederate's ELP every 5th time -> 500ms */
#define ELP_CONFEDERATE_POSITION_REQUEST_START      2
#define ELP_ENEMY1_POSITION_REQUEST_RATE            8   /*!< Request the Enemy1's ELP every 5th time -> 500ms */
#define ELP_ENEMY1_POSITION_REQUEST_START           4
#define ELP_ENEMY2_POSITION_REQUEST_RATE            8   /*!< Request the Enemy2's ELP every 5th time -> 500ms*/
#define ELP_ENEMY2_POSITION_REQUEST_START           4

/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void initELPTimer(void);
extern inline void startELP(void);
extern inline void stopELP(void);


#endif /* ELP_TASK_H_ */

/**
 * @}
 */
