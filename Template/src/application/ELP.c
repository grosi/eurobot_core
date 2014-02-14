/**
 * \file    ELP.h
 * \author  meert1
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   task to pull the ELP's
 *
 * \defgroup elp ELP
 * \brief   ELP task
 * \ingroup app_task
 *
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* application */
#include "app_config.h"
#include "CANGatekeeper.h"
#include "ELP.h"


/* HW-library */
#include "..\lib\SWD.h" /** \todo remove if LED are not used */
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
//#define ELP_CAN_ON

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static uint8_t laserRateCounter;
static uint8_t ultrasonicRateCounter;
static uint8_t kalmanRateCounter;
static uint8_t enemy1RateCounter;
static uint8_t enemy2RateCounter;
static uint8_t confederateRateCounter;

/* Private function prototypes -----------------------------------------------*/
static void vELPTask(void*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initELPTask
 * \brief       Initialisation of the ELP Task
 *
 * \param[in]   None
 * \return      None
 */
void initELPTask(void){

    /* Set the counters to 0*/
    laserRateCounter = 1;
    ultrasonicRateCounter = 1;
    kalmanRateCounter = 1;
    enemy1RateCounter = 1;
    enemy2RateCounter = 1;
    confederateRateCounter = 1;

    /* create the task */
    xTaskCreate( vELPTask, ( signed char * ) ELP_TASK_NAME, ELP_STACK_SIZE, NULL, ELP_TASK_PRIORITY, NULL );

}

/*
 * \fn          vELPTask
 * \brief       Task to pull the ELP-MSG's
 *
 * \param[in]   None
 * \return      None
 */
static void vELPTask(void* pvParameters ) {

    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 / portTICK_RATE_MS);

    for(;;)
    {
        /* wait */
        vTaskDelayUntil( &xLastFlashTime, ELP_TASK_SPEED / portTICK_RATE_MS);

        /* Request the laser's ELP */
        if(laserRateCounter == ELP_LASER_POSITION_REQUEST_RATE)
        {
#ifdef ELP_CAN_ON
            txLaserPostionRequest();
#endif
            /* Reset the laserRateCounter */
            laserRateCounter = 1;
        }
        else
        {
            /* Increase the counter */
            laserRateCounter++;
        }

        /* Request the ultrasonic's ELP */
        if(ultrasonicRateCounter == ELP_ULTRASONIC_POSITION_REQUEST_RATE)
        {
#ifdef ELP_CAN_ON
            txUltrasonicPositionRequest();
#endif
            /* Reset the laserRateCounter */
            ultrasonicRateCounter = 1;
        }
        else
        {
            /* Increase the counter */
            ultrasonicRateCounter++;
        }

        /* Request the Kalman's ELP */
        if(kalmanRateCounter == ELP_KALMAN_POSITION_REQUEST_RATE)
        {
#ifdef ELP_CAN_ON
            txKalmanPositionRequest();
#endif
            /* Reset the laserRateCounter */
            kalmanRateCounter = 1;
        }
        else
        {
            /* Increase the counter */
            kalmanRateCounter++;
        }

        /* Request the first enemy's ELP */
        if(enemy1RateCounter == ELP_ENEMY1_POSITION_REQUEST_RATE)
        {
#ifdef ELP_CAN_ON
            txEnemey1PositionRequest();
#endif
            /* Reset the laserRateCounter */
            enemy1RateCounter = 1;
        }
        else
        {
            /* Increase the counter */
            enemy1RateCounter++;
        }

        /* Request the second enemy's ELP */
        if(enemy2RateCounter == ELP_ENEMY2_POSITION_REQUEST_RATE)
        {
#ifdef ELP_CAN_ON
            txEnemey2PositionRequest();
#endif
            /* Reset the laserRateCounter */
            enemy2RateCounter = 1;
        }
        else
        {
            /* Increase the counter */
            enemy2RateCounter++;
        }

        /* Request the confederate's ELP */
        if(confederateRateCounter == ELP_CONFEDERATE_POSITION_REQUEST_RATE)
        {
#ifdef ELP_CAN_ON
            txConfederatePositionRequest();
#endif
            /* Reset the laserRateCounter */
            confederateRateCounter = 1;
        }
        else
        {
            /* Increase the counter */
            confederateRateCounter++;
        }


    }
}

/**
 * @}
 */
