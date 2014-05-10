/**
 * \file    AppConfig.h
 * \author  gross10
 * \date    2013-11-28
 *
 * \version 1.0
 *  create this file
 *
 * \brief   all necessary values for configuration could be set in this file
 *
 * \defgroup app_config Application-configurations
 * \brief   global applications settings
 * \ingroup main
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* RTOS (are necessary, because they could only include one time*/
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "memPoolService.h"


/* define this only in debug mode, if a error is occured the programm is capture in a loop
 * define the "INCLUDE_uxTaskGetStackHighWaterMark 1" in FreeRTOS.h*/
//#define DEBUGGING

/* uncomment this define if your core node is not connect to all  */
#define STANDALONE

/* typedefs */
typedef enum { FALSE = 0, TRUE } boolean;
typedef enum { FUNC_SUCCESS = 0, FUNC_INCOMPLETE, FUNC_ERROR } func_report_t;  /* Typedef enum for function return status */


#endif

/**
 * @}
 */
