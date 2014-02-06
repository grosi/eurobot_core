/**
 * \file    Strategy.h
 * \author  meert1
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

/* Includes ------------------------------------------------------------------*/
/* RTOS */
#include <memPoolService.h>         /* Memory pool manager service */

/* HW-library */
//#include "..\lib\roboBoardInterface.h" /*!< \todo remove if not used */

/* application */
#include "app_config.h"
#include "CANGatekeeper.h"
#include "Strategy.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void vStrategyTask(void*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initStrategyTask
 * \brief       Initialisation of the strategy task
 *
 * \param[in]   None
 * \return      None
 */
void initStrategyTask(void){

    /* create the task */
    xTaskCreate( vStrategyTask, ( signed char * ) STRATEGY_TASK_NAME, STRATEGY_STACK_SIZE, NULL, STRATEGY_TASK_PRIORITY, NULL );

}

/**
 * \fn          vStrategyTask
 * \brief       Task to handle the strategy
 *
 * \param[in]   None
 * \return      None
 */
static void vStrategyTask(void* pvParameters ) {

    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 / portTICK_RATE_MS);

    for(;;)
    {
        /* wait */
        vTaskDelayUntil( &xLastFlashTime, 10 / portTICK_RATE_MS);

    }
}

/**
 * @}
 */
