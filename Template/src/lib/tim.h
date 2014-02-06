/**
 * @file    tim.h
 * @author  Schüpbach Simon
 * @date    17.05.2013
 *
 * @version 2.0
 *  change the parameter of the function to pointer
 *  add the impulse counter functions
 *
 * @version 1.0
 *  create this file
 *
 * @brief   this file contains function to config a pin to generate a pwm-signal
 *  or as a impuls counter.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  PWM Init structure definition
  */
/*********************************************
	* Initialization example
	********************************************
	* PWMInitStruct Motor1 = {
	*		GPIOE,
	*		{
	*			GPIO_Pin_5 | GPIO_Pin_6,
	*			GPIO_Mode_AF,
	*			GPIO_Speed_100MHz,
	*			GPIO_OType_PP,
	*			GPIO_PuPd_UP,
	*		},
	*		TIM9,
	*		CH1,
	*		CH2,
	*		16000,
	*		&(TIM9->CCR1),
	*		&(TIM9->CCR2)
	*	};
	*
	* initTIM(Motor1);
*/ 
// typedef struct {
// 	GPIO_TypeDef* 		Port; 				/* GPIOA...GPIOH											*/
// 	GPIO_InitTypeDef 	Gpio;					/* Pins, mode, speed,pullup resistor  */
// 	TIM_TypeDef* 			Timer;				/* TIM1...TIM14												*/
// 	uint16_t 					ChannelA;			/* CH1...CH4													*/
// 	uint16_t 					ChannelB;			/* CH1...CH4													*/
// 	uint32_t 					Frequenz;			/* frequenzy of the signal in Hz			*/
// 	__IO uint32_t			*DutyCycleA;	/* pointer to the timer register CCR  */
// 	__IO uint32_t			*DutyCycleB;	/* initalize....  &(TIMx->CCRx)				*/
// }TwoPWMInitStruct;

/**
  * @brief  PWM Init structure definition
  */
/*********************************************/
typedef struct{
	GPIO_TypeDef* 		Port; 				/* GPIOA...GPIOH											*/
	GPIO_InitTypeDef	Gpio;					/* Pins, mode, speed,pullup resistor  */
	TIM_TypeDef* 			Timer;				/* TIM1...TIM14												*/
	uint8_t 					Channel;			/* CH1...CH4													*/
	uint32_t 					Frequenz;			/* frequenzy of the signal in Hz			*/
}PWMInitStruct;


typedef uint32_t *DutyCycleRegister;
typedef uint32_t *TimerCounterRegister;
/* Exported constants --------------------------------------------------------*/

/* constants for the channel-handling */
#define CH1	0x01
#define CH2	0x02
#define CH3	0x04
#define CH4	0x08

#define COUNTER_SIZE 0xffff

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 

/* initialize a timer in pwm-output-mode with two pwm-signals*/
void initPWM(const PWMInitStruct *PWM);	
DutyCycleRegister getDutyCycleRegister(const PWMInitStruct *PWM);

/* initialize a timer in pwm-output-mode with one pwm-signals*/
//extern void initOnePWM(OnePWMInitStruct Pwm);
TimerCounterRegister initExternalSignalCounter(PWMInitStruct *Ext);
#endif
