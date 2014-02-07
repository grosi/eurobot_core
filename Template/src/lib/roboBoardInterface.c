/******************************************************************************
 * @file    roboBoardInterface.c
 * @author  Schüpbach Simon
 * @date    07.06.2013
 *
 * @version 1.0
 *	create this file
 *
 * @brief   this file include all functions to use a servo or dc-motor,
 *					 LED or buttons,  ADC or DAC.
******************************************************************************/
	
/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "motor.h"
#include "roboBoardInterface.h"


/* Private typedef -----------------------------------------------------------*/
typedef enum{
 FALSE = 0,
 TRUE = !FALSE
}bool;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* the following bools are all used to check if a component is already initialize */	
#ifndef ADC_WITH_DMA
	static bool isSensor1Initialize 			= 	FALSE;
	static bool isSensor2Initialize 			= 	FALSE;
	static bool isSensor3Initialize 			= 	FALSE;
	static bool isSensor4Initialize 			= 	FALSE;

	static bool isColorSensorInitialize 	= 	FALSE;
#else
	static bool isDMAInitialize						=		FALSE;	
#endif

static bool isDCMotor1Initialize 			= 	FALSE;
static bool isDCMotor2Initialize 			= 	FALSE;




static bool isBlueButtonDiscoveryBoardInitialize 		= 	FALSE;

/* this variables are use to point to the dutycycle register of a timer,		*/ 

static DCMotorDutyCycleRegister DCMotorDutyCycle1;
static DCMotorDutyCycleRegister DCMotorDutyCycle2;


/* initialisation-struct for DCMotor 1*/
static DCMotorInitStruct DCMotor1 = {
	GPIOB,
	{
		GPIO_Pin_14 | GPIO_Pin_15,
		GPIO_Mode_AF,
		GPIO_Speed_100MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
	TIM12,
	CH1,
	CH2,
	DCMotor1_Frequency
};

/* initialisation-struct for DCMotor 2*/
static DCMotorInitStruct DCMotor2 = {
	GPIOE,
	{
		GPIO_Pin_5 | GPIO_Pin_6,
		GPIO_Mode_AF,
		GPIO_Speed_100MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
	TIM9,
	CH1,
	CH2,
	DCMotor1_Frequency
};

/* initialisation-struct for Sensor 1*/
ADCInitStruct Sensor1 = {
	GPIOC,
	{
		GPIO_Pin_5,
		GPIO_Mode_AN,
		GPIO_Speed_50MHz,
		GPIO_OType_PP,
		GPIO_PuPd_NOPULL
	},
	ADC1,
	ADC_Channel_15
};

/* initialisation-struct for Sensor 2*/
ADCInitStruct Sensor2 = {
	GPIOC,
	{
		GPIO_Pin_4,
		GPIO_Mode_AN,
		GPIO_Speed_50MHz,
		GPIO_OType_PP,
		GPIO_PuPd_NOPULL
	},
	ADC1,
	ADC_Channel_14
};

/* initialisation-struct for Sensor 3*/
ADCInitStruct Sensor3 = {
	GPIOB,
	{
		GPIO_Pin_1,
		GPIO_Mode_AN,
		GPIO_Speed_50MHz,
		GPIO_OType_PP,
		GPIO_PuPd_NOPULL
	},
	ADC1,
	ADC_Channel_9
};

/* initialisation-struct for Sensor 4*/
ADCInitStruct Sensor4 = {
	GPIOB,
	{
		GPIO_Pin_0,
		GPIO_Mode_AN,
		GPIO_Speed_50MHz,
		GPIO_OType_PP,
		GPIO_PuPd_NOPULL
	},
	ADC1,
	ADC_Channel_8
};

/* initialisation-struct for the Color-Sensor*/
RGBInitStruct ColorSensor = {
	{
		GPIOA,
			{
			GPIO_Pin_2,
			GPIO_Mode_AN,
			GPIO_Speed_50MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL
		},
		ADC2,
		ADC_Channel_2,
	},
	{
		GPIOA,
		{
			GPIO_Pin_3,
			GPIO_Mode_AN,
			GPIO_Speed_50MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL
		},
		ADC3,
		ADC_Channel_3,
	},
	{
		GPIOA,
		{
			GPIO_Pin_1,
			GPIO_Mode_AN,
			GPIO_Speed_50MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL
		},	
		ADC1,
		ADC_Channel_1,
	}
};
	




/* initialisation-struct for the blue button of the discoveryboard */
static PinInitStruct BlueButtonDiscoveryBoard = {
	GPIOA,
	{
		GPIO_Pin_0,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_DOWN
	},
};


/* Private function prototypes -----------------------------------------------*/


#ifdef ADC_WITH_DMA
	/* use ADC measure with DMA */
	static void initDMA(void);

	/* buffer for the DMA values */
	static uint16_t ADCwithDMAvalue[7];
#endif

/* Private functions ---------------------------------------------------------*/




/**
	*************************************************************************************
  * @brief  get the sensor value and initialize him, if this isn't done before
  * @param  None
  * @retval value of the sensor
	**************************************************************************************
  */
float getSensor1(void){
	
	/* normal mode */
	#ifndef ADC_WITH_DMA
		
		/* if the initialisation isn't done before */
		if(!isSensor1Initialize){
			
			/* set the Flag */
			isSensor1Initialize = TRUE;
			
			/* initialize */
			initADC(&Sensor1);
		}
		/* return value */
		return (float)(getADC(&Sensor1)/ 4095.0 * VOLTAGE_DISCOVERYBOARD);
		
	/* DMA mode */
	#else
		
		/* if the initialisation isn't done before */
		if(!isDMAInitialize){
		
			/* set the Flag */
			isDMAInitialize = TRUE;
			
			/* initialize */
			initDMA();
		}
		
		/* return value*/
		return (float)((float)ADCwithDMAvalue[0]) / 4095 * VOLTAGE_DISCOVERYBOARD;
	#endif
}

/**
	*************************************************************************************
  * @brief  get the sensor value and initialize him, if this isn't done before
  * @param  None
  * @retval value of the sensor
	**************************************************************************************
  */
float getSensor2(void){
	/* normal mode */
	#ifndef ADC_WITH_DMA
		
		/* if the initialisation isn't done before */
		if(!isSensor2Initialize){
			
			/* set the Flag */
			isSensor2Initialize = TRUE;
			
			/* initialize */
			initADC(&Sensor2);
		}
		/* return value */
		return (float)(getADC(&Sensor2)/ 4095.0 * VOLTAGE_DISCOVERYBOARD);
		
	/* DMA mode */
	#else
		
		/* if the initialisation isn't done before */
		if(!isDMAInitialize){
		
			/* set the Flag */
			isDMAInitialize = TRUE;
			
			/* initialize */
			initDMA();
		}
		
		/* return value*/
		return ADCwithDMAvalue[1]/ 4095.0 * VOLTAGE_DISCOVERYBOARD;;
	#endif
}

/**
	*************************************************************************************
  * @brief  get the sensor value and initialize him, if this isn't done before
  * @param  None
  * @retval value of the sensor
	**************************************************************************************
  */
float getSensor3(void){
	/* normal mode */
	#ifndef ADC_WITH_DMA
		
		/* if the initialisation isn't done before */
		if(!isSensor3Initialize){
			
			/* set the Flag */
			isSensor3Initialize = TRUE;
			
			/* initialize */
			initADC(&Sensor3);
		}
		/* return value */
		return (float)(getADC(&Sensor3) / 4095.0 * VOLTAGE_DISCOVERYBOARD);
		
	/* DMA mode */
	#else
		
		/* if the initialisation isn't done before */
		if(!isDMAInitialize){
		
			/* set the Flag */
			isDMAInitialize = TRUE;
			
			/* initialize */
			initDMA();
		}
		
		/* return value*/
		return ADCwithDMAvalue[2]/ 4095.0 * VOLTAGE_DISCOVERYBOARD;
	#endif
}

/**
	*************************************************************************************
  * @brief  get the sensor value and initialize him, if this isn't done before
  * @param  None
  * @retval value of the sensor
	**************************************************************************************
  */
float getSensor4(void){
	/* normal mode */
	#ifndef ADC_WITH_DMA
		
		/* if the initialisation isn't done before */
		if(!isSensor4Initialize){
			
			/* set the Flag */
			isSensor4Initialize = TRUE;
			
			/* initialize */
			initADC(&Sensor4);
		}
		/* return value */
		return (float)(getADC(&Sensor4) / 4095.0 * VOLTAGE_DISCOVERYBOARD);
		
	/* DMA mode */
	#else
		
		/* if the initialisation isn't done before */
		if(!isDMAInitialize){
		
			/* set the Flag */
			isDMAInitialize = TRUE;
			
			/* initialize */
			initDMA();
		}
		
		/* return value*/
		return ADCwithDMAvalue[3]/ 4095.0 * VOLTAGE_DISCOVERYBOARD;
	#endif
}

/**
	*************************************************************************************
  * @brief  get the three sensor value of the color sensor and initialize him, if this isn't done before
  * @param  None
  * @retval values of the Color sensor
	**************************************************************************************
  */
ColorSensorValues getColorSensor(void){

	ColorSensorValues ColorSensorVal;
	
	/* normal mode */
	#ifndef ADC_WITH_DMA
		RGBValue SensorValue;
	
		/* if the initialisation isn't done before */
		if(!isColorSensorInitialize){
			
			/* set the Flag */
			isColorSensorInitialize = TRUE;
			
			/* initialize */
			initRGB(&ColorSensor);
		}
		/* return value */
		SensorValue = getRGB(&ColorSensor);
		ColorSensorVal.Red = (float)(SensorValue.Red_Part / 4095.0 * VOLTAGE_DISCOVERYBOARD);
		ColorSensorVal.Green = (float)(SensorValue.Green_Part / 4095.0 * VOLTAGE_DISCOVERYBOARD);
		ColorSensorVal.Blue = (float)(SensorValue.Blue_Part / 4095.0 * VOLTAGE_DISCOVERYBOARD);
		
		/* return value*/
		return ColorSensorVal;
		
	/* DMA mode */
	#else
		
		/* if the initialisation isn't done before */
		if(!isDMAInitialize){
			
			/* set the Flag */
			initDMA();
			
			/* initialize */
			isDMAInitialize = TRUE;
		}
		
		/* get the sensor values */
		ColorSensorVal.Red = ADCwithDMAvalue[4] / 4095.0 * VOLTAGE_DISCOVERYBOARD;
		ColorSensorVal.Green = ADCwithDMAvalue[5] / 4095.0 * VOLTAGE_DISCOVERYBOARD;
		ColorSensorVal.Blue = ADCwithDMAvalue[6] / 4095.0 * VOLTAGE_DISCOVERYBOARD;
		
		/* return value*/
		return ColorSensorVal;
	#endif
}


/**
	*************************************************************************************
  * @brief  set the dutycycle of the DCMotor
  * @param  DutyCycleA: dutycycle of the first register, value between 0 and 1000
  * @param  DutyCycleB: dutycycle of the second register, value between 0 and 1000
  * @retval None
	**************************************************************************************
  */
void setDCMotor1(uint16_t DutyCycleA, uint16_t DutyCycleB){
	
	/* if the initialisation isn't done before */
	if(!isDCMotor1Initialize){
		
		/* set the Flag */
		isDCMotor1Initialize = TRUE;
		
		/* initialize */
		DCMotorDutyCycle1 = initDCMotor(&DCMotor1);
	}
	
	/* catch overflow of DutyCycleA */
	if(DutyCycleA > 1000){
		DutyCycleA = 1000;
	}
	
	/* catch overflow of DutyCycleB  */
	if(DutyCycleB > 1000){
		DutyCycleB = 1000;
	}
	
	/* set first dutycycle */
	*DCMotorDutyCycle1.A = DutyCycleA;
	
	/* set second dutycycle */
	*DCMotorDutyCycle1.B = DutyCycleB;
}

/**
	*************************************************************************************
  * @brief  set the dutycycle of the DCMotor
  * @param  DutyCycleA: dutycycle of the first register, value between 0 and 1000
  * @param  DutyCycleB: dutycycle of the second register, value between 0 and 1000
  * @retval None
	**************************************************************************************
  */
void setDCMotor2(uint16_t DutyCycleA, uint16_t DutyCycleB){
	
	/* if the initialisation isn't done before */
	if(!isDCMotor2Initialize){
		
		/* set the Flag */
		isDCMotor2Initialize = TRUE;
		
		/* initialize */
		DCMotorDutyCycle2 = initDCMotor(&DCMotor2);
	}
		
	/* catch overflow of DutyCycleA */
	if(DutyCycleA > 1000){
		DutyCycleA = 1000;
	}
	
	/* catch overflow of DutyCycleB  */
	if(DutyCycleB > 1000){
		DutyCycleB = 1000;
	}
	
	/* set first dutycycle */
	*DCMotorDutyCycle2.A = DutyCycleA;
	
	/* set second dutycycle */
	*DCMotorDutyCycle2.B = DutyCycleB;
}










#ifdef ADC_WITH_DMA
	/**
		*************************************************************************************
		* @brief  take preparations for a DMA initialisation
		* @param  None
		* @retval None
		**************************************************************************************
		*/
	static void initDMA(void){
		
		/* array with all quest for the dma */
		ADCInitStruct* Array[]={
				{&Sensor1},
				{&Sensor2},
				{&Sensor3},
				{&Sensor4},
				{&ColorSensor.Red_Part},
				{&ColorSensor.Green_Part},
				{&ColorSensor.Blue_Part}
			};	
			
			/* initialize the dma */
			initADCwithDMA(*Array, 7, ADC1, ADCwithDMAvalue);
	}
#endif
