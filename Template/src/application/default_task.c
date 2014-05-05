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
#include <math.h>

/* HW-library */
#include "../lib/usart.h"
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

void sendValUSART(char* string, int val, int base) {
	char buffer[32+1];
	sendStringUSART(string);
	itoa(val, buffer, base);
	sendStringUSART(buffer);
	sendCharacterUSART('\n');
}

/*******************************************************************************
 * \fn          vDefaultTask
 * \brief       test and dummy task
 *
 * \param[in]   None
 * \return      None
 ******************************************************************************/
static void vDefaultTask(void* pvParameters ) {

	int btn_alt = 0;
	int btn;

	int delta_x, delta_y, phi, beta, gamma, alpha;
	short distance;
	short x = 50;
	short y = 50;
	short enemy_1_x[] = {80, 20, 20, 80};
	short enemy_1_y[] = {30, 30, 70, 70};
	short angle = 30;
	if(angle > 180) angle -= 360;

	sendCharacterUSART('\n');

    /* endless loop */
    for(;;)
    {
        /* system delay of 25ms */
        vTaskDelay(50 / portTICK_RATE_MS);

        btn=getBoardButton_Blue();
        if(btn && !btn_alt) {

        	int i;
        	for(i=0; i<4; i++) {
				if(angle < 180) {
					alpha = angle;
				}
				else {
					alpha = angle - 360;
				}
				delta_x = enemy_1_x[i] - x;
				delta_y = enemy_1_y[i] - y;
				distance = round(sqrt(delta_x*delta_x + delta_y*delta_y));
				sendValUSART("delta_x:   ", delta_x, 10);
				sendValUSART("delta_y:   ", delta_y, 10);
				phi = -round(atan2f(delta_y, delta_x)/M_PI*180);
				sendValUSART("phi:       ", phi, 10);
				sendValUSART("delta_phi: ", phi-alpha, 10);
				sendCharacterUSART('\n');
        	}

//        	phi = round(atanf(delta_y/delta_x)/M_PI*180);
//        	sendValUSART("phi(atanf): ", phi, 10);
//        	sendCharacterUSART('\n');
//        	phi = round(acosf(delta_x/distance)/M_PI*180);
//        	sendValUSART("phi(acosf): ", phi, 10);
//        	sendCharacterUSART('\n');
//        	phi = round(asinf(delta_y/distance)/M_PI*180);
//        	sendValUSART("phi(asinf): ", phi, 10);
//        	sendCharacterUSART('\n');

//        	phi = round(acosf(delta_y/distance)/M_PI*180);
//        	sendValUSART("phi: ", phi, 10);
//        	gamma = atan2f(delta_y,delta_y)/M_PI*180;
//        	sendValUSART("gamma: ", gamma, 10);
//        	beta = 90-gamma+angle;
//        	sendValUSART("beta: ", beta, 10);
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
