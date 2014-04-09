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
#include "../lib/servo.h"
#include "../lib/button.h"
#include "../lib/led.h"

/* application */
#include "AppConfig.h"
#include "default_task.h"
#include "nodes/FrescoNode.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


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
    initUSART();

    /* servos */
    initServo_1();
    initServo_2();
    initServo_3();
    initServo_4();

    /* buttons */
    initButton_S1();
    initButton_S2();
    initButton_S3();
    initButton_S3();
    initButton_S4();
    initBoardButton_Blue();

    /* leds */
    initBoardLED_blue();
    initBoardLED_green();
    initBoardLED_red();

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

	setServo_1(2000);

	int btn_alt = 0;
	int btn;

    /* endless loop */
    for(;;)
    {
        /* system delay of 25ms */
        vTaskDelay(50 / portTICK_RATE_MS);

        btn=getButton_S1();
        if(btn && !btn_alt) {
        	doFrescoNode();
        }
        btn_alt=btn;
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
