/**
 * \file    default_task.c
 * \author  gross10
 * \date    2013--11-15
 *
 * \version 1.0
 *  create this file
 *
 * \brief   test task with temporary code sequences
 *
 * \defgroup default default_task
 * \brief   test task
 * \ingroup app_task
 *
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* RTOS */
#include <memPoolService.h>         /* Memory pool manager service */

/* HW-library */
#include "../lib/led.h"

/* application */
#include "app_config.h"
#include "default_task.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
typedef void (*led_t)(uint8_t);


/* Private function prototypes -----------------------------------------------*/
static void vDefaultTask(void*);


/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
 * \fn          initDefaultTask
 * \brief       Initialisation the test and dummy task
 *
 * \param[in]   None
 * \return      None
 ******************************************************************************/
void initDefaultTask(void){

    /* module initialisation */
    /* leds */
    initBoardLED_blue();
    initBoardLED_green();
    initBoardLED_red();
    initBoardLED_orange();

    /* create the task */
    xTaskCreate( vDefaultTask, ( signed char * ) DEFAULT_TASK_NAME, DEFAULT_STACK_SIZE, NULL, DEFAULT_TASK_PRIORITY, NULL );
}

/*******************************************************************************
 * \fn          vDefaultTask
 * \brief       test and dummy task
 *
 * \param[in]   None
 * \return      None
 ******************************************************************************/
static void vDefaultTask(void* pvParameters ) {

    /* local variables */
    portTickType xLastFlashTime;
    uint8_t led_loop;
    led_t setled[4] = {setBoardLED_red,
                       setBoardLED_orange,
                       setBoardLED_green,
                       setBoardLED_blue};

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );

    /* endless loop */
    for(;;)
    {
        /* LED on */
        for(led_loop = 0; led_loop < 4; led_loop++)
        {
            setled[led_loop](1); /* led ON */
            /* task delay of 200ms */
            vTaskDelayUntil( &xLastFlashTime, 200 / portTICK_RATE_MS);
        }

        /* LED off */
        for(led_loop = 0; led_loop < 4; led_loop++)
        {
            setled[led_loop](0); /* led OFF */
            /* task delay of 200ms */
            vTaskDelayUntil( &xLastFlashTime, 200 / portTICK_RATE_MS);
        }
    }
}


/**
 * @}
 */
