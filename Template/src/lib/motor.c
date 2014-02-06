/**
  ******************************************************************************
  * @file    motor.c
  * @author  Schüpbach Simon
  * @date    07.06.2013
	*
	* @version 1.0	
	*	create this file
	*
  * @brief   this file converts the pwm-function to the servo or the dc-motor
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "tim.h"
#include "motor.h"
//#include "include.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
	*************************************************************************************
  * @brief  Initializes one pins for the PWM-OUTPUT-MODE and get the adress of the associated dutycycle register.
  * @param  Servo: include all datas for initialize
  * @retval ServoDutyCycleRegister
	**************************************************************************************
  */
ServoDutyCycleRegister initServo(const ServoInitStruct *Servo){
	
	/* convert to te PWMStruct */
	PWMInitStruct PWM;
	PWM.Port = Servo->Port;
	PWM.Gpio = Servo->Gpio;
	PWM.Timer = Servo->Timer;
	PWM.Channel = Servo->Channel;
	PWM.Frequenz = Servo->Frequenz;
	
	/* initialize */
	initPWM(&PWM);
	
	/* get dutycycle register */
	return (ServoDutyCycleRegister) getDutyCycleRegister(&PWM);
}

/**
	*************************************************************************************
  * @brief  Initializes two pins for the PWM-OUTPUT-MODE and get the adresses of the associated dutycycle registers.
  * @param  DCMotor: include all datas for initialize
  * @retval DCMotorDutyCycleRegister
	**************************************************************************************
  */
DCMotorDutyCycleRegister initDCMotor(const DCMotorInitStruct *DCMotor){
	DCMotorDutyCycleRegister DCMotorDCR;
	
	/* convert to te PWMStruct */
	PWMInitStruct PWM;
	PWM.Port = DCMotor->Port;
	PWM.Gpio = DCMotor->Gpio;
	PWM.Timer = DCMotor->Timer;
	PWM.Channel = DCMotor->ChannelA | DCMotor->ChannelB;
	PWM.Frequenz = DCMotor->Frequenz;
	
	/* initialize */
	initPWM(&PWM);
	
	/* get first dutycycle register */
	PWM.Channel = DCMotor->ChannelA;
	DCMotorDCR.A = (uint32_t*)getDutyCycleRegister(&PWM);
	
	/* get second dutycycle register */
	PWM.Channel = DCMotor->ChannelB;
	DCMotorDCR.B = (uint32_t*)getDutyCycleRegister(&PWM);
	
	/* return addresses */
	return DCMotorDCR;
}

