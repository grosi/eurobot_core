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
#include "CANGatekeeper.h"
#include "default_task.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static xQueueHandle qTest;


/* Private function prototypes -----------------------------------------------*/
static void vDefaultTask(void*);
static void CANRxTest(uint16_t, CAN_data_t*);
static void CANRxTest2(uint16_t, CAN_data_t*);

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


    /* create the queues */
    qTest = xQueueCreate(10,sizeof(CAN_data_t)); /* RX-Message Queue */

    /* create the task */
    xTaskCreate( vDefaultTask, ( signed char * ) DEFAULT_TASK_NAME, DEFAULT_STACK_SIZE, NULL, DEFAULT_TASK_PRIORITY, NULL );

    /* set the CAN listener */
    setFunctionCANListener(CANRxTest,EMERGENCY_STOP);
    //setFunctionCANListener(CANRxTest,GOTO_XY);
    //setFunctionCANListener(CANRxTest,GOTO_STATE_RESPONSE);
    //setFunctionCANListener(CANRxTest,LASER_POSITION_RESPONSE);
    //setFunctionCANListener(CANRxTest2,GOTO_STATE_REQUEST);
    setQueueCANListener(qTest,LASER_POSITION_REQUEST);
}

/*******************************************************************************
 * \fn          vDefaultTask
 * \brief       test and dummy task
 *
 * \param[in]   None
 * \return      None
 ******************************************************************************/
static void vDefaultTask(void* pvParameters ) {

    portTickType xLastFlashTime;
    uint8_t test;
    int8_t points[2][2] = {{20,20},{20,20}};
    uint16_t servo = 1000;
    uint8_t up = 1;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );



    for(;;)
    {

//        /* set the green led */
//        setDiscoveryLedGreen();
//
//        /* wait */
//        vTaskDelayUntil( &xLastFlashTime, 50 / portTICK_RATE_MS);
//
//        /* reset the green led */
//        resetDiscoveryLedGreen();
//
//        /* set the orange led */
//        setDiscoveryLedOrange();
//
//        /* wait */
//        vTaskDelayUntil( &xLastFlashTime, 50 / portTICK_RATE_MS);
//
//        /* reset the orange led */
//        resetDiscoveryLedOrange();
//
//        /* set the red led */
//        setDiscoveryLedRed();
//
//        /* wait */
//        vTaskDelayUntil( &xLastFlashTime, 50 / portTICK_RATE_MS);
//
//        /* reset the red led */
//        resetDiscoveryLedRed();
//
//        /* set the blue led */
//        setDiscoveryLedBlue();
//
//        /* wait */
//        vTaskDelayUntil( &xLastFlashTime, 50 / portTICK_RATE_MS);
//
//        /* reset the blue led */
//        resetDiscoveryLedBlue();

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
        }
//        if(xQueueReceive(qTest,&test,portMAX_DELAY) == pdTRUE)
//        {
//            toggleLed3();
//
//            txGotoXY(12,30,130,20,points);
//        }
        //setServo1(105);
        vTaskDelayUntil( &xLastFlashTime, 25 / portTICK_RATE_MS);
//        sendStringUSART("Hallo Welt\n");
//        txStopDrive();
        //sendStringVCP("Hallo Welt");

    }
}


/**
 * \fn CANRxTest
 * \brief set LED 1
 *
 * \param[in] id    CAN ID
 * \param[in] len   Data-length (byte)
 * \param[in] data  pointer to the first data segement
 */
static void CANRxTest(uint16_t id, CAN_data_t* data)
{
    uint16_t time;
    setLed4();

    time = data->goto_x;
    time = data->goto_angle;



}


/**
 * \fn CANRxTest2
 * \brief reset LED 1
 *
 * \param[in] id    CAN ID
 * \param[in] len   Data-length (byte)
 * \param[in] data  pointer to the first data segement
 */
static void CANRxTest2(uint16_t id, CAN_data_t* data)
{
    resetLed4();
}


/**
 * @}
 */
