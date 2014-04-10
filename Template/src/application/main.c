/**
 * \file        Main.c
 * \brief       Startup file. In this file is the main function, which is
 *              called at the startup of the controller.
 * \author      meert1, gross10, haldj3
 * \date        2013-11-26
 * \version     0.1
 *
 *
 * \mainpage    Eurobot - Kernknoten
 * \brief
 *
 * \authors     meert1, gross10, rohrp1, roth3, haldj3
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
/* application modules*/
#include "AppConfig.h"
#include "DefaultTask.h"
#include "CANGatekeeper.h"
#include "System.h"
#include "Timer.h"
#include "ELP.h"
#include "Rangefinder.h"
#include "interrupt_handlers.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * \fn          main
 * \brief       the application starts here
 *
 * \param[in]   None
 * \return      0
 */
int main(void)
{
    /* Configure the NVIC Preemption Priority Bits for STM-Lib V3.1+ */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* module initialization */
    initTimerTask();
    initSystemTask();
    initELPTask();
    initRangefinderTask();
    initDefaultTask();
    initCANGatekeeper(); /* have to be the last initialization modul! */

    vTaskStartScheduler();

    /* endless loop */
    for (;;);

    return 0;
}


/**
 * \fn      vApplicationStackOverflowHook
 * \brief   this function is called if an stackoverflow is recognized
 *          -> FreeRTOSConfig.h "configCHECK_FOR_STACK_OVERFLOW"
 *
 * \param   xTask       task-handler from the overflow-task
 * \param   pcTaskName  pointer to the task-name with the overflow
 */
extern void vApplicationStackOverflowHook( xTaskHandle xTask, signed char *pcTaskName )
{
    deleteDefaultTask();
}


/**
 * @}
 */
