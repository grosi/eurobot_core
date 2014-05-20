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


/* Define for compiler directives --------------------------------------------*/
/* define this only in debug mode, if a error is occured the programm is capture in a loop
 * define the "INCLUDE_uxTaskGetStackHighWaterMark 1" in FreeRTOS.h*/
//#define DEBUGGING

/* uncomment this define if your core node is not connect to all  */
//#define STANDALONE

/* define this to use USART debugging */
//#define DEBUGGIN_WITH_USART


/* Includes ------------------------------------------------------------------*/
/* RTOS (are necessary, because they could only include one time*/
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "memPoolService.h"
#include "CANGatekeeper.h"

#ifdef DEBUGGIN_WITH_USART
#include "lib/usart.h"
#endif

/* uncomment this if the userpanel doesnt works */
#define WITHOUT_USERPANEL

/* typedefs */
typedef enum { FALSE = 0, TRUE } boolean;
typedef enum { FUNC_SUCCESS = 0, FUNC_INCOMPLETE_LIGHT, FUNC_INCOMPLETE_HEAVY, FUNC_ERROR } func_report_t;  /* Typedef enum for function return status */
typedef enum { TEAM_YELLOW = GIP_TEAMCOLOR_YELLOW, TEAM_RED = GIP_TEAMCOLOR_RED } team_color_t;  /* Typedef enum for the team color, use the same values as CAN-Gatekeeper for compatibility */

/* defines */
#define IGNORED_VALUE 0

#endif

/**
 * @}
 */
