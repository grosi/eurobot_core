/**
 * \file        main.c
 * \brief       Startup file. In this file is the main function, which is
 *              called at the startup of the controller.
 * \author      meert1, gross10
 * \date        2013-11-26
 * \version     0.1
 *
 *
 * \mainpage    Eurobot - Kernknoten
 * \brief
 *
 * \authors     meert1, gross10, rohrp1, roth3
 * \date        2014-01-15
 * \version     0.1
 * \details     <p><b>Description</b>
 *              <p>This doxygen-documentation decribes the code of the core-node
 *
 * \defgroup    hw_task HW-Task
 * \brief       Task with depencies to the RTOS, Firmware and BSP
 *
 * \defgroup    app_task App-Task
 * \brief       Task with depencies to the RTOS and Firmware
 *
 * \defgroup    firmware Firmware
 * \brief       Libraries with depencies to the BSP
 *
 * \addtogroup  main
 * \brief       Startup file. In this file is the main function, which is
 *              called at the startup of the controller.
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* RTOS */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include <memPoolService.h>

/* application */
#include "app_config.h"
#include "default_task.h"
#include "CANGatekeeper.h"
#include "Strategy.h"
#include "Timer.h"
#include "ELP.h"
#include "Rangefinder.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \fn          main
 * \brief       The application starts here.
 *
 * \param[in]   None
 * \return      0
 */
int main(void) {

    /* Configure the NVIC Preemption Priority Bits for STM-Lib V3.1+ */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //TODO

    /* module initialisation */
    initTimerTask();
    initStrategyTask();
    initELPTask();
    initRangefinderTask();
    initDefaultTask();
    initCANGatekeeper(); /* have to the last initialisation modul! */

    vTaskStartScheduler();

    /* endless loop */
    for (;;);

    return 0;
}

/**
 * @}
 */
