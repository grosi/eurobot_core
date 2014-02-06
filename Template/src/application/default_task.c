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
#include "..\lib\roboBoardInterface.h"
#include "../lib/usart.h"
#include "../lib/servo.h"

/* application */
#include "app_config.h"
#include "default_task.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


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
    initUSART();
    initServo_1();
    initServo_2();
    initServo_3();
    initServo_4();

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
    uint16_t servo = 1000;
    uint8_t up = 1;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );


    /* endless loop */
    for(;;)
    {


        setServo_1(servo);
        setServo_2(servo);
        setServo_3(servo);

        if(up == 1 && servo < 2000)
        {

            servo += 2;
            if(servo == 2000)
            up = 0;
        }
        if(up == 0 && servo > 0)
        {
            servo -= 2;
           if(servo == 1000)
               up = 1;
        };
        vTaskDelayUntil( &xLastFlashTime, 25 / portTICK_RATE_MS);
    }
}

/**
 * \fn      itoa
 * \brief   C++ version 0.4 char* style "itoa"
 *
 * \note    Written by Lukás Chmela; http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
 */
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

/**
 * @}
 */
