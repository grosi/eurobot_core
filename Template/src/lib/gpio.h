/*******************************************************************************
 * @file	gpio.c
 * @author	Schüpbach Simon
 * @date	17.05.2013
 *
 * @version	2.0
 * 	change the parameter of the function to pointer
 *
 * @version	1.0
 * 	create this file
 *
 * @brief	this file contains function for initialize pins and ports or
 * 			enable clocks and alternate functions.
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 
/* Exported types ------------------------------------------------------------*/

/**
 * @brief  Pin Init structure definition
 */
/*******************************************************************************
 * Initialization example
 ************************
 * PinInitStruct Led1 = {
 * 	GPIOD,
 *	{
 *		GPIO_Pin_2,
 *		GPIO_Mode_OUT,
 *		GPIO_Speed_2MHz,
 *		GPIO_OType_PP,
 *		GPIO_PuPd_UP
 *	}
 * };
 *
 * initPin(Led1);
 ******************************************************************************/
typedef struct {
	GPIO_TypeDef* Port; /* GPIOA...GPIOH */
	GPIO_InitTypeDef Gpio; /* Pins, mode, speed,pullup resistor */
}PinInitStruct;

/**
  * @brief  IRQ Init structure definition
  */
/*******************************************************************************
 * Initialization example
 ************************
* IRQInitStruct Button1 = {
*	GPIOE,
*	{
*		GPIO_Pin_4,
*		GPIO_Mode_IN,
*		GPIO_Speed_2MHz,
*		GPIO_OType_PP,
*		GPIO_PuPd_NOPULL
*	},
*	{
*		EXTI_Line4,
*		EXTI_Mode_Interrupt,
*		EXTI_Trigger_Rising,
*		ENABLE
*	},
*	{
*		EXTI4_IRQn,
*		0x00,
*		0x00,
*		ENABLE
*	}
* };
*
* initIRQ(Button1);
*/
typedef struct {
	GPIO_TypeDef* Port; /* GPIOA...GPIOH */
	GPIO_InitTypeDef Gpio; /* Pins, mode, speed,pullup resistor */
	EXTI_InitTypeDef Exti; /* Line, mode, trigger */
	NVIC_InitTypeDef Nvic; /* IRQ, PreemptionPriority, subpPriority*/
}IRQInitStruct;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/  

/* enable the clock of a port */
extern uint8_t enablePort(GPIO_TypeDef* GPIOx);

/* enable the clock of a port and initialize a pin*/
extern void initPin(PinInitStruct *Gpio);

/* enable the clock of a port, initialize a pin and config in AF-mode */
extern void enableAFPort(GPIO_TypeDef* GPIOx ,GPIO_InitTypeDef Gpio, uint16_t AF_Function);

/* enable the clock of a port, initialize a pin and config in interrupt-mode */
extern void initIRQ(IRQInitStruct *Irq);

/* set the bit of a gpio*/
extern inline void setPin(PinInitStruct *Pin);

/* reset the bit of a gpio */
extern inline void resetPin(PinInitStruct *Pin);

/* toggle the bit of a gpio */
extern inline void togglePin(PinInitStruct *Pin);

/* get the bit of a gpio */
extern inline uint8_t getPin(PinInitStruct *Pin);

/* set the bit of a gpio*/
extern inline void setIRQPin(IRQInitStruct *Pin);

/* reset the bit of a gpio */
extern inline void resetIRQPin(IRQInitStruct *Pin);

/* toggle the bit of a gpio */
extern inline void toggleIRQPin(IRQInitStruct *Pin);

/* get the bit of a gpio */
extern inline uint8_t getIRQPin(IRQInitStruct *Pin);

#endif
