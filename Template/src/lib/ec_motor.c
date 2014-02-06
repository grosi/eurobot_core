/*******************************************************************************
 * @file    ec_motor.c
 * @author  Schüpbach Simon
 * @date    30.05.2013
 *
 * @version 1.0
 *	create this file
 *
 * @brief	this file conatains all function that are needed to use the ec-motor
*			all the signals are initialize and so you can control the ec-motor.
 *			all the signals of the ec-motor drive are detected
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <memPoolService.h>
#include "gpio.h"
#include "adc.h"
#include "dac.h"
#include "tim.h"
#include "motor.h"
#include "ec_motor.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* position of the ec-motor */
static int32_t ECMotorPosition = 0;

/* nominal rotation direction of the ec-motor */
static DirectionEnum NominalECMotorDirection = FORWARD;

/* effective rotation direction of the ec-motor */
static DirectionEnum ECMotorDirection = FORWARD;

/* buffer for the address of the counter-register of the timer */
static TimerCounterRegister ECMotorTachoTCR;

/* queue for the error handling */
xQueueHandle qECMotorError;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* fast or slow current control of the EC-Motor */
static PinInitStruct ECMotorMODE ={
	GPIOD,
	{
		GPIO_Pin_6,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* enable FET's of the EC-Motor */
static PinInitStruct ECMotorCOAST ={
	GPIOC,
	{
		GPIO_Pin_14,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* nominal direction of the EC-Motor */
static PinInitStruct ECMotorDIR ={
	GPIOD,
	{
		GPIO_Pin_7,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* brake handling of the EC-Motor */
static PinInitStruct ECMotorBRAKE ={
	GPIOB,
	{
		GPIO_Pin_4,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* enable of the EC-Motor */
static PinInitStruct ECMotorRESET ={
	GPIOC,
	{
		GPIO_Pin_13,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* error handling */
static PinInitStruct ECMotorESF ={
	GPIOE,
	{
		GPIO_Pin_7,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* actuell speed of the EC-Motor */
static PWMInitStruct ECMotorTACHO ={
		GPIOC,
	{
		GPIO_Pin_6,
		GPIO_Mode_AF,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_DOWN
	},
	TIM3,
	CH1,
	1000
};

/* actuell direction of the EC-Motor */
static IRQInitStruct ECMotorDIRO = {
	GPIOC,
	{
		GPIO_Pin_15,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
	{
		EXTI_Line15,
		EXTI_Mode_Interrupt,
		EXTI_Trigger_Rising_Falling,
		ENABLE
	},
	{
		EXTI15_10_IRQn,
		0x00,
		0x00,
		ENABLE
	}
};

/* error flag 1 */
static IRQInitStruct ECMotorFF1 = {
	GPIOE,
	{
		GPIO_Pin_2,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_DOWN
	},
	{
		EXTI_Line2,
		EXTI_Mode_Interrupt,
		EXTI_Trigger_Rising_Falling,
		ENABLE
	},
	{
		EXTI2_IRQn,
		0x00,
		0x00,
		ENABLE
	}
};

/* error flag 2 */
static IRQInitStruct ECMotorFF2 = {
	GPIOE,
	{
		GPIO_Pin_4,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_DOWN
	},
	{
		EXTI_Line4,
		EXTI_Mode_Interrupt,
		EXTI_Trigger_Rising_Falling,
		ENABLE
	},
	{
		EXTI4_IRQn,
		0x00,
		0x00,
		ENABLE
	}
};

/* current control */
static DACStruct ECMotorREF ={
	DAC1_PORT,
	DAC1_PIN,
	DAC1_CHANNEL
};

/* speed control */
static ServoInitStruct ECMotorPWM = {
	GPIOC,
	{
		GPIO_Pin_8,
		GPIO_Mode_AF,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
	TIM8,
	CH3,
	ECMotor_Frequency
};
/* initialisation-struct for Sensor 1*/
ADCInitStruct ECMotorCSOUT = {
	GPIOC,
	{
		GPIO_Pin_1,
		GPIO_Mode_AN,
		GPIO_Speed_50MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
	ADC1,
	ADC_Channel_10
};

/* pointer to the DutyCycle register of the speed control */
ServoDutyCycleRegister ECMotorPWMSpeed;


/**
	*************************************************************************************
  * @brief  Initializes all necessary for using the EC-Motor 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void initECMotor(void){

	
		/* create the queues */
	qECMotorError = xQueueCreate(10,sizeof(uint8_t)); 
	
	/* if the semaphore isn't created*/
	if(qECMotorError == NULL){		
		#ifdef DEBUGGING
			while(1);
		#endif
	}
	
	/* initialize the hardware counter for count the tacho signals*/
	ECMotorTachoTCR = initExternalSignalCounter(&ECMotorTACHO);
	
	/* initialize the output flags of the EC-Motor IC */
	initIRQ(&ECMotorDIRO);
	initIRQ(&ECMotorFF1);
	initIRQ(&ECMotorFF2);
	
	/* initialize the input flags of the EC-Motor IC */
	initPin(&ECMotorMODE);
	initPin(&ECMotorCOAST);
	initPin(&ECMotorBRAKE);
	initPin(&ECMotorRESET);
	initPin(&ECMotorESF);
	initPin(&ECMotorDIR);
	
	/* initialize the current control */
	initDAC(&ECMotorREF);
	
	/* initialize the current measure */
	initADC(&ECMotorCSOUT);
	
	/* initialize the speed control */
	ECMotorPWMSpeed = initServo(&ECMotorPWM);
	
}

/**
	*************************************************************************************
  * @brief  set the currrent limit of the EC-Motor
  * @param  Current: maximal current in Ampere
  * @retval None
	**************************************************************************************
  */
void setECMotorCurrentLimit(float Current){
	
	/* calculate the value for the DAC */
	uint16_t Value = 0xfff * VOLTAGE_DISCOVERYBOARD /(Current * CURRENT_2_VOLTAGE);
	
	/* set the voltage */
	setDAC(Value,&ECMotorREF);
}

/**
	*************************************************************************************
  * @brief  get the actuel currrent of the EC-Motor
  * @param  None
  * @retval actuel current
	**************************************************************************************
  */
float getECMotorCurrent(void){
	float Value;
	float Voltage;
	float Current;
	
	/* get the voltage level */
	Value = getADC(&ECMotorCSOUT);
	
	/* get the voltage in volt and subtract the offset voltage of the ec-motor-driver */
	Voltage = Value * VOLTAGE_DISCOVERYBOARD / 4095.0f - 0.32f ; 
	
	/* capture underflow */
	if(Voltage < 0 ){
		Voltage = 0;
	}
	
	/* calculate the current */
	Current = Voltage / CURRENT_2_VOLTAGE;
	
	/* return current */
	return Current;
}


/**
	*************************************************************************************
  * @brief  config the settings of the EC-Motor IC
  * @param  Setting: struct with all values of the settings
  * @retval None
	**************************************************************************************
  */
void configECMotor(ECMotorSettingStruct *Setting){
	
	/* (0) for fast current control with greater current ripple */
	/* 		 suitable for Motors wich run in the start/stop mode  */
	/* (1) for slow current control with few current ripple 		*/
	/* 		 suitable for Motors wich run at a constant speed 		*/
	if(Setting->MODE){
		setPin(&ECMotorMODE);
	}else{
		resetPin(&ECMotorMODE);
	}
	
	/* (0) Disable all Fet's  */
	/* (1) None	  */
	if(Setting->COAST){
		setPin(&ECMotorCOAST);
	}else{
		resetPin(&ECMotorCOAST);
	}
	
	/* (0) Brake motor, low-Side Fet turn on and high-Side Fet turn off */
	/* (1) None	  */		
	if(Setting->BRAKE){
		setPin(&ECMotorBRAKE);
	}else{
		resetPin(&ECMotorBRAKE);
	}
	
	/* (0) Disable power supply */
	/* (1) Enable	power supply (wait 3ms while the power is stabilized ) */
	if(Setting->RESET){
		setPin(&ECMotorRESET);
	}else{
		resetPin(&ECMotorRESET);
	}
	
	/************************************************************
	* FF1 * FF2 *		Fault								*					Action				*
	*			*			*												* 	ESF=0 	* 	ESF=1 	*
	*************************************************************
	*	 0  *  0  * Undervoltage					*  Disable  *  Disable  *
	*	 0  *  0  * Overtemperature				* No Action * No Action *
	*	 0  *  0  * Logic Fault						*  Disable  *  Disable  *
	*	 1  *  0  * Short to ground				* No Action *  Disable  *
	*	 1  *  0  * Short to supply				* No Action *  Disable  *
	*	 1  *  0  * Shorted Motor winding	* No Action *  Disable  *
	*	 0  *  1  * Low load current			* No Action * No Action *
	*	 1  *  1  * None									* No Action * No Action *
	************************************************************/
	
	/* (0) Disable the automatic error-handling, no safety!!! */
	/* (1) Enable	some error-handlings*/
	if(Setting->ESF){
		setPin(&ECMotorESF);
	}else{
		resetPin(&ECMotorESF);
	}
}

/**
	*************************************************************************************
  * @brief  set the nominal rotate dircetion of the EC-Motor
  * @param  Direction: could be FORWARD or REVERSE
  * @retval None
	**************************************************************************************
  */
void setECMotorDirection(DirectionEnum Direction){
	
	/* set or reset the direction pin */
	if(Direction == FORWARD){
		setPin(&ECMotorDIR);
	}else{
		resetPin(&ECMotorDIR);
	}
	NominalECMotorDirection = Direction;
}	

/**
	*************************************************************************************
  * @brief  get the actual rotate direction of the EC-Motor
  * @param  None
  * @retval None
	**************************************************************************************
  */
DirectionEnum getECMotorDirection(void){
	return ECMotorDirection;
}	

/**
	*************************************************************************************
  * @brief  if the hardwarecounter of the tacho-signal has an oveflow update EC-Motor position
  * @param  None
  * @retval None
	**************************************************************************************
  */
void TIM3_IRQHandler(void){
	
	/* clear interrupt flag */
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	/* update EC-Motor position */
	ECMotorPosition += COUNTER_SIZE * (ECMotorDirection - 1);
}

/**
	*************************************************************************************
  * @brief  irq handler of the exti lines 10 to 15
  * @param  None
  * @retval None
	**************************************************************************************
  */
void EXTI15_10_IRQHandler(void){

	/* if line 15 generate a interrupt */
	if(EXTI_GetITStatus(EXTI_Line15)){
		
		/* reset irq flag */
		EXTI_ClearITPendingBit(EXTI_Line15);
		
		/* update the EC-Motor position */
		getECMotorPosition();
		
		/* if the DIRO pin is set*/
		if(getIRQPin(&ECMotorDIRO)){
			
			/* FORWARD direction */
			ECMotorDirection = FORWARD;
		}else{
			
			/* REVERSE direction */
			ECMotorDirection = REVERSE;
		}
	}
}

/**
	*************************************************************************************
  * @brief  irq handler of exti line 2
  * @param  None
  * @retval None
	**************************************************************************************
  */
void EXTI2_IRQHandler(void){
	uint8_t Error;
	
	/* reset irq flag */
	EXTI_ClearITPendingBit(EXTI_Line2);
	
	/* Error with FF1 flag */
	Error =getIRQPin(&ECMotorFF1) | ERROR_FF1;
	
	/* send error to the queue */
	xQueueSendToBackFromISR(qECMotorError,&Error,0);
}

/**
	*************************************************************************************
  * @brief  irq handler of exti line 4
  * @param  None
  * @retval None
	**************************************************************************************
  */
void EXTI4_IRQHandler(void){
	uint8_t Error;
	
	/* reset irq flag */
	EXTI_ClearITPendingBit(EXTI_Line4);
	
	/* Error with FF2 flag */
	Error =getIRQPin(&ECMotorFF2) | ERROR_FF2;
	
	/* send error to the queue */
	xQueueSendToBackFromISR(qECMotorError,&Error,0);
}


/**
	*************************************************************************************
  * @brief  update and get the position of the EC-Motor
  * @param  None
  * @retval the updated position of the EC-Motor
	**************************************************************************************
  */
int32_t getECMotorPosition(void){
	uint16_t Number;
	
	/* read the number of pulses since the last update */
	Number = *ECMotorTachoTCR;
	
	/* reset counter */
	*ECMotorTachoTCR = 0;
	
	/* calculate the new position of the EC-Motor */
//	ECMotorPosition += Number * (ECMotorDirection - 1);
	ECMotorPosition += Number * (NominalECMotorDirection - 1);
	/* return this value */
	return ECMotorPosition;
}

/**
	*************************************************************************************
  * @brief  set the value of the position buffer
  * @param  Position:	set this value 
  * @retval None
	**************************************************************************************
  */
void setECMotorPosition(int32_t Position){
	ECMotorPosition = Position;
}

/**
	*************************************************************************************
  * @brief  set the nominal DutyCycle of the PWM wich controls the RotationSpeed
  * @param  Speed:	Value of the DutyCycle if (value < 0) then the direction is changed
	*									Value should be in range of 1000 >= Value >= -1000
  * @retval None
	**************************************************************************************
  */
void setECMotorPWMSpeed(int16_t Speed){
	
	/* if value is negativ drive reverse */
	if(Speed<0){
		/* drive reverse */
		setECMotorDirection(REVERSE);
		
		/* catch overflow */
		if(Speed<-1000){
			Speed = -1000;
		}
		/* set dutycycle */
		*ECMotorPWMSpeed = -Speed;
		
	/* else drive forward */
	}else{
		
		/* drive forward */
		setECMotorDirection(FORWARD);
		
		/* catch overflow */
		if(Speed > 1000){
			Speed = 1000;
		}
		
		/* set dutycycle */
		*ECMotorPWMSpeed = Speed;
	}
	getECMotorPosition();
}



