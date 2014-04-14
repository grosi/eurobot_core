/**
 * \file    RoboError.c
 * \author  gross10
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy error sate
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "RoboError.h"

/* lib */
#include "lib/led.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboSetupState
 * \brief   initialisation of the RoboSetupState
 *
 * \note    have to be called from the initSystem function
 */
void initRoboErrorState()
{
    /* hw modules */
    initLED_1();
    initLED_2();
    initLED_3();
    initLED_4();
    initLED_5();
    initLED_6();

}

void runRoboErrorState(portTickType* tick)
{
    /* local variable */
    static uint8_t led_state = 0;

    setLED_1(led_state);
    setLED_2(led_state);
    setLED_3(led_state);
    setLED_4(led_state);
    setLED_5(led_state);
    setLED_6(led_state);

    led_state ^= 1;

    vTaskDelayUntil(tick,200/portTICK_RATE_MS);

}

/**
 * @}
 */
