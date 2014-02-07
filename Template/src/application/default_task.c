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
#define NUMBER_OF_SERVOS 3
#define SERVO_STEP_SIZE 10
#define SERVO_MIN 800
#define SERVO_MAX 2000
#define SERVO_RIGHT_1 2000
#define SERVO_LEFT_1 800
#define SERVO_RIGHT_2 2000
#define SERVO_LEFT_2 800
#define SERVO_RIGHT_3 2000
#define SERVO_LEFT_3 800
#define SERVO_RIGHT_4 2000
#define SERVO_LEFT_4 800

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
typedef void (*led_t)(uint8_t);


/* Private function prototypes -----------------------------------------------*/
static void vDefaultTask(void*);
char* itoa(int, char*, int);


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
