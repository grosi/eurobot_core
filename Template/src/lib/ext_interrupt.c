/**
 * \file    ir_sensor.c
 * \author  kasen1
 * \date    2014-02-26
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Configuration of external interrupts
 *
 * \defgroup ext_interrupt Ext_Interrupt
 * \brief   external interrupt functions
 * \ingroup firmware
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/**
 * \fn      initEXTILine
 * \brief   Configures EXTI Line in interrupt mode
 *
 * \param   RCC_AHB1Periph                    e.g. RCC_AHB1Periph_GPIOB (for PB5)
 * \param   EXTI_PortSourceGPIOx              e.g. EXTI_PortSourceGPIOB (for PB5)
 * \param   EXTI_PinSourcex                   e.g. EXTI_PinSource5      (for PB5)
 * \param   EXTI_Line                         e.g. EXTI_Line5           (for PB5)
 * \param   EXTI_Trigger                      e.g. EXTI_Trigger_Falling
 * \param   NVIC_IRQChannel                   e.g. EXTI9_5_IRQn         (for PB5)
 * \param   NVIC_IRQChannelPreemptionPriority e.g. 0x01 (lowest)
 * \param   NVIC_IRQChannelSubPriority        e.g. 0x01 (lowest)
 */
void initEXTILine(uint32_t RCC_AHB1Periph, uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex, uint32_t EXTI_Line, EXTITrigger_TypeDef EXTI_Trigger,
		uint8_t NVIC_IRQChannel, uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock and SYSCFG clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI Line to pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);

	/* Configure EXTI Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line Interrupt to priority */
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
	NVIC_Init(&NVIC_InitStructure);
}


/**
 * @}
 */
