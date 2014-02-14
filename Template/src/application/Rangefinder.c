/**
 * \file    Rangefinder.c
 * \author  kasen1
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   task for the rangefinder sensors
 *
 * \defgroup rangefinder Rangefinder
 * \brief   Rangefinder task
 * \ingroup app_task
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "app_config.h"
#include "Rangefinder.h"

/* HW-library */
//#include "..\lib\roboBoardInterface.h" /*!< \todo remove if not used */

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void vRangefinderTask(void*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initRangefinderTask
 * \brief       Initialisation of the Rangefinder Task
 *
 * \param[in]   None
 * \return      None
 */
void initRangefinderTask(void){

    /* create the task */
    xTaskCreate( vRangefinderTask, ( signed char * ) RANGEFINDER_TASK_NAME, RANGEFINDER_STACK_SIZE, NULL, RANGEFINDER_TASK_PRIORITY, NULL );

}

/**
 * \fn          vRangefinderTask
 * \brief       Task to watch the near range in front and back of the robot
 *
 * \param[in]   None
 * \return      None
 */
static void vRangefinderTask(void* pvParameters ) {

    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 / portTICK_RATE_MS);

    for(;;)
    {
        /* wait */
        vTaskDelayUntil( &xLastFlashTime, 100 / portTICK_RATE_MS);

    }
}

/**
 * @}
 */
