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
#include "../lib/usart.h"
#include "../lib/servo.h"
#include "../lib/button.h"
#include "../lib/led.h"

/* application */
#include "app_config.h"
#include "default_task.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define NUMBER_OF_SERVOS 2
#define SERVO_STEP_SIZE 10
#define SERVO_MIN 1000
#define SERVO_MAX 2000
#define SERVO_RIGHT_1 2000
#define SERVO_LEFT_1 1000
#define SERVO_RIGHT_2 2000
#define SERVO_LEFT_2 1000
#define SERVO_RIGHT_3 2000
#define SERVO_LEFT_3 1000
#define SERVO_RIGHT_4 2000
#define SERVO_LEFT_4 1000

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
typedef void (*servo_t)(uint16_t);


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

/*******************************************************************************
 * \fn          vDefaultTask
 * \brief       test and dummy task
 *
 * \param[in]   None
 * \return      None
 ******************************************************************************/
static void vDefaultTask(void* pvParameters ) {

    /* local variables  */
    portTickType xLastFlashTime;
    uint8_t button_s1_state, button_s1_edge;
    uint8_t button_s2_state, button_s2_edge;
    uint8_t button_blue_state, button_blue_edge;
    char servo_messages[4][2][16] = {{{"SERVO 1 LEFT:  "},{"SERVO 1 RIGHT: "}},
                                  {{"SERVO 2 LEFT:  "},{"SERVO 2 RIGHT: "}},
                                  {{"SERVO 3 LEFT:  "},{"SERVO 3 RIGHT: "}},
                                  {{"SERVO 4 LEFT:  "},{"SERVO 4 RIGHT: "}}};
    uint16_t servo_pos[4][2] = {{SERVO_LEFT_1,SERVO_RIGHT_1},
                               {SERVO_LEFT_2,SERVO_RIGHT_2},
                               {SERVO_LEFT_3,SERVO_RIGHT_3},
                               {SERVO_LEFT_4,SERVO_RIGHT_4}};
    servo_t setServo[4] = {setServo_1,
                           setServo_2,
                           setServo_3,
                           setServo_4};
    uint8_t servo = 0;
    uint8_t block = 0; /* 0=left; 1=right */
    char buffer[5];

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );

    /* servos to NULL-positions */
    setServo_1(SERVO_LEFT_1);
    setServo_2(SERVO_LEFT_2);
    setServo_3(SERVO_LEFT_3);
    setServo_4(SERVO_LEFT_4);

    /* endless loop */
    for(;;)
    {
        /* system delay of 25ms */
        vTaskDelayUntil( &xLastFlashTime, 25 / portTICK_RATE_MS);

        /* read in the buttons */
        button_s1_edge = getButtonPosEdge_S1(&button_s1_state);
        button_s2_edge = getButtonPosEdge_S2(&button_s2_state);
        button_blue_edge = getBoardButtonPosEdge_Blue(&button_blue_state);

        /* change servo */
        if(button_s1_edge)
        {
            servo++;
            if(servo >= NUMBER_OF_SERVOS)
            {
                servo = 0;
            }
        }

        /* change block (left/right) */
        if(button_s2_edge)
        {
            if(block)
            {
                block = 0;
            }
            else
            {
                block = 1;
            }
        }

        /* turn left */
        if(getButton_S3())
        {
            /* left block */
            if(!block)
            {
                if(servo_pos[servo][block] > SERVO_MIN)
                {
                    servo_pos[servo][block]--;
                }
            }
            /* right block */
            else
            {
                if(servo_pos[servo][block] > servo_pos[servo][0])
                {
                    servo_pos[servo][block]--;
                }
            }
        }

        /* turn right */
        if(getButton_S4())
        {
            /* left block */
            if(!block)
            {
                if(servo_pos[servo][block] < servo_pos[servo][1])
                {
                    servo_pos[servo][block]++;
                }
            }
            /* right block */
            else
            {
                if(servo_pos[servo][block] < SERVO_MAX)
                {
                    servo_pos[servo][block]++;
                }
            }
        }

        /* print current setting */
        if(button_blue_edge)
        {
            /* left */
            sendStringUSART(&servo_messages[servo][0][0]);
            itoa(servo_pos[servo][0],buffer,10);
            sendStringUSART(buffer);
            sendCharacterUSART('\n');
            /* right */
            sendStringUSART(&servo_messages[servo][1][0]);
            itoa(servo_pos[servo][1],buffer,10);
            sendStringUSART(buffer);
            sendCharacterUSART('\n');
            sendCharacterUSART('\n');
        }

        /* set servo */
        setServo[servo](servo_pos[servo][block]);

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
