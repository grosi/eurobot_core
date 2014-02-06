/**
  ******************************************************************************
  * @file    motor.h
  * @author  Schüpbach Simon
  * @date    07.06.2013
	*
	* @version 1.0	
	*	create this file
	*
  * @brief   this file converts the pwm-function to the servo or the dc-motor
  ******************************************************************************  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H
	#define __MOTOR_H

	//#include "stm32f4xx.h"
	
	/* dutycycle register of a dc-motor */
	typedef struct{
		uint32_t 					*A;
		uint32_t 					*B;
	}DCMotorDutyCycleRegister;

	/* dutycycle register of a servo */
	typedef uint32_t *ServoDutyCycleRegister;
	
		/**
  * @brief  DCMotor Init structure definition
  */
/*********************************************
	* Initialization example
	********************************************
	* DCMotorStruct DCMotor = {
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
	*	};
	*
	*	DCMotorDutyCycleRegister DCMotorDutyCycle;
	*
	* DCMotorDutyCycle = initDCMotor(&DCMotor);
	*
	* *DCMotorDutyCycle.A = 100; // -> 10% DutyCycle
	* *DCMotorDutyCycle.B = 0; 	 // -> 00% DutyCycle
	*/
	typedef struct{
		GPIO_TypeDef* 		Port; 				/* GPIOA...GPIOH											*/
		GPIO_InitTypeDef 	Gpio;					/* Pins, mode, speed,pullup resistor  */
		TIM_TypeDef* 			Timer;				/* TIM1...TIM14												*/
		uint8_t 					ChannelA;			/* CH1...CH4													*/
		uint8_t 					ChannelB;			/* CH1...CH4													*/
		uint32_t 					Frequenz;			/* frequenzy of the signal in Hz			*/
	}DCMotorInitStruct;

	/**
  * @brief  Servo Init structure definition
  */
/*********************************************
	* Initialization example
	********************************************
	* ServoStruct Servo = {
	*		GPIOE,
	*		{
	*			GPIO_Pin_9,
	*			GPIO_Mode_AF,
	*			GPIO_Speed_100MHz,
	*			GPIO_OType_PP,
	*			GPIO_PuPd_UP
	*		},
	*		TIM1,
	*		CH1,
	*		50
	*	};
	*
	*	ServoDutyCycleRegister ServoDutyCycle;
	*
	* ServoDutyCycle = initServo(&Servo);
	*
	* *ServoDutyCycle = 100; // -> 10% DutyCycle
*/ 
	typedef struct{
		GPIO_TypeDef* 		Port; 				/* GPIOA...GPIOH											*/
		GPIO_InitTypeDef 	Gpio;					/* Pins, mode, speed,pullup resistor  */
		TIM_TypeDef* 			Timer;				/* TIM1...TIM14												*/
		uint8_t 					Channel;			/* CH1...CH4													*/
		uint32_t 					Frequenz;			/* frequenzy of the signal in Hz			*/
	}ServoInitStruct;
	

	/* Initializes one pins for the PWM-OUTPUT-MODE and get the adress of the associated dutycycle register */
	extern ServoDutyCycleRegister initServo(const ServoInitStruct *Servo);
	
	/*Initializes two pins for the PWM-OUTPUT-MODE and get the adresses of the associated dutycycle registers */
	extern DCMotorDutyCycleRegister initDCMotor(const DCMotorInitStruct *DCMotor);

#endif
