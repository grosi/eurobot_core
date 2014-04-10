/**
 * \file    ext_interrupt.h
 * \author  kasen1
 * \date    2014-02-26
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Configuration of external interrupts
 *
 * \defgroup ir_sensor IR_Sensor
 * \brief   ir sensor library
 * \ingroup firmware
 *
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EXT_INTERRUPT_H_
#define EXT_INTERRUPT_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/


/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initEXTILine(uint32_t RCC_AHB1Periph, uint8_t EXTI_PortSourceGPIOx,
		uint8_t EXTI_PinSourcex, uint32_t EXTI_Line, EXTITrigger_TypeDef EXTI_Trigger,
		uint8_t NVIC_IRQChannel, uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority);


#endif /* EXT_INTERRUPT_H_ */

/**
 * @}
 */
