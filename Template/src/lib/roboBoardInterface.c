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

static bool isLed1Initialize 					= 	FALSE;
static bool isLed2Initialize 					= 	FALSE;
static bool isLed3Initialize 					= 	FALSE;
static bool isLed4Initialize 					= 	FALSE;
static bool isLed5Initialize 					= 	FALSE;
static bool isLed6Initialize 					= 	FALSE;

static bool isColorSensorLedInitialize			=		FALSE;

static bool isDiscoveryLedGreenInitialize 	=		FALSE;
static bool isDiscoveryLedOrangeInitialize 	=		FALSE;
static bool isDiscoveryLedRedInitialize 		=		FALSE;
static bool isDiscoveryLedBlueInitialize 		=		FALSE;

static bool isButton1Initialize 			= 	FALSE;
static bool isButton2Initialize 			= 	FALSE;
static bool isButton3Initialize 			= 	FALSE;
static bool isButton4Initialize 			= 	FALSE;
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
	
/* initialisation-struct for Led 1*/
static PinInitStruct Led1 = {
	GPIOA,
	{
		GPIO_Pin_8,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for Led 2*/
static PinInitStruct Led2 = {
	GPIOC,
	{
		GPIO_Pin_9,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for Led 3*/
static PinInitStruct Led3 = {
	GPIOD,
	{
		GPIO_Pin_8,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for Led 4*/
static PinInitStruct Led4 = {
	GPIOD,
	{
		GPIO_Pin_10,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for Led 5*/
static PinInitStruct Led5 = {
	GPIOD,
	{
		GPIO_Pin_9,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for Led 6*/
static PinInitStruct Led6 = {
	GPIOD,
	{
		GPIO_Pin_11,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for enable the LED's on the ColorSensorBoard */
static PinInitStruct ColorSensorLed = {
	GPIOB,
	{
		GPIO_Pin_13,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for the green Led on the DiscoveryBoard*/
static PinInitStruct DiscoveryLedGreen = {
	GPIOD,
	{
		GPIO_Pin_12,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for the orange Led on the DiscoveryBoard*/
static PinInitStruct DiscoveryLedOrange = {
	GPIOD,
	{
		GPIO_Pin_13,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for the red Led on the DiscoveryBoard*/
static PinInitStruct DiscoveryLedRed = {
	GPIOD,
	{
		GPIO_Pin_14,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for the blue Led on the DiscoveryBoard*/
static PinInitStruct DiscoveryLedBlue = {
	GPIOD,
	{
		GPIO_Pin_15,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* initialisation-struct for button 1*/
static PinInitStruct Button1 = {
	GPIOD,
	{
		GPIO_Pin_3,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
};

/* initialisation-struct for button 2*/
static PinInitStruct Button2 = {
	GPIOD,
	{
		GPIO_Pin_2,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
};

/* initialisation-struct for button 3*/
static PinInitStruct Button3 = {
	GPIOC,
	{
		GPIO_Pin_11,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
};

/* initialisation-struct for button 4*/
static PinInitStruct Button4 = {
	GPIOA,
	{
		GPIO_Pin_15,
		GPIO_Mode_IN,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	},
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

/**
	*************************************************************************************
  * @brief  switch on the LED
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setLed1(void){
	
	/* if the initialisation isn't done before */
	if(!isLed1Initialize){
		
		/* set the Flag */
		isLed1Initialize = TRUE;
		
		/* initialize */
		initPin(&Led1);
	}
	
	/* switch on the LED */
	setPin(&Led1);
}
/**
	*************************************************************************************
  * @brief  switch on the LED
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setLed2(void){
	
	/* if the initialisation isn't done before */
	if(!isLed2Initialize){
		
		/* set the Flag */
		isLed2Initialize = TRUE;
		
		/* initialize */
		initPin(&Led2);
	}
	
	/* switch on the LED */
	setPin(&Led2);
}

/**
	*************************************************************************************
  * @brief  switch on the LED
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setLed3(void){
	
	/* if the initialisation isn't done before */
	if(!isLed3Initialize){
		
		/* set the Flag */
		isLed3Initialize = TRUE;
		
		/* initialize */
		initPin(&Led3);
	}
	
	/* switch on the LED */
	setPin(&Led3);
}
/**
	*************************************************************************************
  * @brief  switch on the LED
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setLed4(void){
	
	/* if the initialisation isn't done before */
	if(!isLed4Initialize){
		
		/* set the Flag */
		isLed4Initialize = TRUE;
		
		/* initialize */
		initPin(&Led4);
	}
	
	/* switch on the LED */
	setPin(&Led4);
}
/**
	*************************************************************************************
  * @brief  switch on the LED
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setLed5(void){
	
	/* if the initialisation isn't done before */
	if(!isLed5Initialize){
		
		/* set the Flag */
		isLed5Initialize = TRUE;
		
		/* initialize */
		initPin(&Led5);
	}
	
	/* switch on the LED */
	setPin(&Led5);
}

/**
	*************************************************************************************
  * @brief  switch on the LED
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setLed6(void){
	
	/* if the initialisation isn't done before */
	if(!isLed6Initialize){
		
		/* set the Flag */
		isLed6Initialize = TRUE;
		
		/* initialize */
		initPin(&Led6);
	}
	
	/* switch on the LED */
	setPin(&Led6);
}

/**
	*************************************************************************************
  * @brief  switch on the LED on the ColorSensorBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setColorSensorLed(void){
	
	/* if the initialisation isn't done before */
	if(!isColorSensorLedInitialize){
		
		/* set the Flag */
		isColorSensorLedInitialize = TRUE;
		
		/* initialize */
		initPin(&ColorSensorLed);
	}
	
	/* switch on the LED */
	setPin(&ColorSensorLed);
}

/**
	*************************************************************************************
  * @brief  switch on the green LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setDiscoveryLedGreen(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedGreenInitialize){
		
		/* set the Flag */
		isDiscoveryLedGreenInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedGreen);
	}
	
	/* switch on the LED */
	setPin(&DiscoveryLedGreen);
}

/**
	*************************************************************************************
  * @brief  switch on the orange LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setDiscoveryLedOrange(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedOrangeInitialize){
		
		/* set the Flag */
		isDiscoveryLedOrangeInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedOrange);
	}
	
	/* switch on the LED */
	setPin(&DiscoveryLedOrange);
}

/**
	*************************************************************************************
  * @brief  switch on the red LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setDiscoveryLedRed(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedRedInitialize){
		
		/* set the Flag */
		isDiscoveryLedRedInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedRed);
	}
	
	/* switch on the LED */
	setPin(&DiscoveryLedRed);
}

/**
	*************************************************************************************
  * @brief  switch on the blue LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void setDiscoveryLedBlue(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedBlueInitialize){
		
		/* set the Flag */
		isDiscoveryLedBlueInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedBlue);
	}
	
	/* switch on the LED */
	setPin(&DiscoveryLedBlue);
}

/**
	*************************************************************************************
  * @brief  switch off the LED 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetLed1(void){
	
	/* if the initialisation isn't done before */
	if(!isLed1Initialize){
		
		/* set the Flag */
		isLed1Initialize = TRUE;
		
		/* initialize */
		initPin(&Led1);
	}
	
	/* switch off the LED */
	resetPin(&Led1);
}

/**
	*************************************************************************************
  * @brief  switch off the LED 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetLed2(void){
	
	/* if the initialisation isn't done before */
	if(!isLed2Initialize){
		
		/* set the Flag */
		isLed2Initialize = TRUE;
		
		/* initialize */
		initPin(&Led2);
	}
	
	/* switch off the LED */
	resetPin(&Led2);
}

/**
	*************************************************************************************
  * @brief  switch off the LED 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetLed3(void){
	
	/* if the initialisation isn't done before */
	if(!isLed3Initialize){
		
		/* set the Flag */
		isLed3Initialize = TRUE;
		
		/* initialize */
		initPin(&Led3);
	}
	
	/* switch off the LED */
	resetPin(&Led3);
}

/**
	*************************************************************************************
  * @brief  switch off the LED 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetLed4(void){
	
	/* if the initialisation isn't done before */
	if(!isLed4Initialize){
		
		/* set the Flag */
		isLed4Initialize = TRUE;
		
		/* initialize */
		initPin(&Led4);
	}
	
	/* switch off the LED */
	resetPin(&Led4);
}

/**
	*************************************************************************************
  * @brief  switch off the LED 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetLed5(void){
	
	/* if the initialisation isn't done before */
	if(!isLed5Initialize){
		
		/* set the Flag */
		isLed5Initialize = TRUE;
		
		/* initialize */
		initPin(&Led5);
	}
	
	/* switch off the LED */
	resetPin(&Led5);
}

/**
	*************************************************************************************
  * @brief  switch off the LED 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetLed6(void){
	
	/* if the initialisation isn't done before */
	if(!isLed6Initialize){
		
		/* set the Flag */
		isLed6Initialize = TRUE;
		
		/* initialize */
		initPin(&Led6);
	}
	
	/* switch off the LED */
	resetPin(&Led6);
}

/**
	*************************************************************************************
  * @brief  switch off the LED's on the ColorSensorBoard 
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetColorSensorLed(void){
	
	/* if the initialisation isn't done before */
	if(!isColorSensorLedInitialize){
		
		/* set the Flag */
		isColorSensorLedInitialize = TRUE;
		
		/* initialize */
		initPin(&ColorSensorLed);
	}
	
	/* switch off the LED */
	resetPin(&ColorSensorLed);
}

/**
	*************************************************************************************
  * @brief  switch off the green LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetDiscoveryLedGreen(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedGreenInitialize){
		
		/* set the Flag */
		isDiscoveryLedGreenInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedGreen);
	}
	
	/* switch off the LED */
	resetPin(&DiscoveryLedGreen);
}

/**
	*************************************************************************************
  * @brief  switch off the orange LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetDiscoveryLedOrange(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedOrangeInitialize){
		
		/* set the Flag */
		isDiscoveryLedOrangeInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedOrange);
	}
	
	/* switch off the LED */
	resetPin(&DiscoveryLedOrange);
}

/**
	*************************************************************************************
  * @brief  switch off the red LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetDiscoveryLedRed(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedRedInitialize){
		
		/* set the Flag */
		isDiscoveryLedRedInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedRed);
	}
	
	/* switch off the LED */
	resetPin(&DiscoveryLedRed);
}

/**
	*************************************************************************************
  * @brief  switch off the blue LED on the DiscoveryBoard
  * @param  None
  * @retval None
	**************************************************************************************
  */
void resetDiscoveryLedBlue(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedBlueInitialize){
		
		/* set the Flag */
		isDiscoveryLedBlueInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedBlue);
	}
	
	/* switch off the LED */
	resetPin(&DiscoveryLedBlue);
}

/**
	*************************************************************************************
  * @brief  change the state of the LED, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleLed1(void){
	
	/* if the initialisation isn't done before */
	if(!isLed1Initialize){
		
		/* set the Flag */
		isLed1Initialize = TRUE;
		
		/* initialize */
		initPin(&Led1);
	}
	
	/* change state of the LED */
	togglePin(&Led1);
}

/**
	*************************************************************************************
  * @brief  change the state of the LED, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleLed2(void){
	
	/* if the initialisation isn't done before */
	if(!isLed2Initialize){
		
		/* set the Flag */
		isLed2Initialize = TRUE;
		
		/* initialize */
		initPin(&Led2);
	}
	
	/* change state of the LED */
	togglePin(&Led2);
}

/**
	*************************************************************************************
  * @brief  change the state of the LED, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleLed3(void){
	
	/* if the initialisation isn't done before */
	if(!isLed3Initialize){
		
		/* set the Flag */
		isLed3Initialize = TRUE;
		
		/* initialize */
		initPin(&Led3);
	}
	
	/* change state of the LED */
	togglePin(&Led3);
}

/**
	*************************************************************************************
  * @brief  change the state of the LED, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleLed4(void){
	
	/* if the initialisation isn't done before */
	if(!isLed4Initialize){
		
		/* set the Flag */
		isLed4Initialize = TRUE;
		
		/* initialize */
		initPin(&Led4);
	}
	
	/* change state of the LED */
	togglePin(&Led4);
}

/**
	*************************************************************************************
  * @brief  change the state of the LED, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleLed5(void){
	
	/* if the initialisation isn't done before */
	if(!isLed5Initialize){
		
		/* set the Flag */
		isLed5Initialize = TRUE;
		
		/* initialize */
		initPin(&Led5);
	}
	
	/* change state of the LED */
	togglePin(&Led5);
}

/**
	*************************************************************************************
  * @brief  change the state of the LED, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleLed6(void){
	
	/* if the initialisation isn't done before */
	if(!isLed6Initialize){
		
		/* set the Flag */
		isLed6Initialize = TRUE;
		
		/* initialize */
		initPin(&Led6);
	}
	
	/* change state of the LED */
	togglePin(&Led6);
}

/**
	*************************************************************************************
  * @brief  change the state of the LED's on the ColorSensorBoard, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleColorSensorLed(void){
	
	/* if the initialisation isn't done before */
	if(!isColorSensorLedInitialize){
		
		/* set the Flag */
		isColorSensorLedInitialize = TRUE;
		
		/* initialize */
		initPin(&ColorSensorLed);
	}
	
	/* change state of the LED */
	togglePin(&ColorSensorLed);
}

/**
	*************************************************************************************
  * @brief  change the state of the green LED on the DiscoveryBoard, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleDiscoveryLedGreen(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedGreenInitialize){
		
		/* set the Flag */
		isDiscoveryLedGreenInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedGreen);
	}
	
	/* change state of the LED */
	togglePin(&DiscoveryLedGreen);
}

/**
	*************************************************************************************
  * @brief  change the state of the orange LED on the DiscoveryBoard, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleDiscoveryLedOrange(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedOrangeInitialize){
		
		/* set the Flag */
		isDiscoveryLedOrangeInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedOrange);
	}
	
	/* change state of the LED */
	togglePin(&DiscoveryLedOrange);
}

/**
	*************************************************************************************
  * @brief  change the state of the red LED on the DiscoveryBoard, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleDiscoveryLedRed(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedRedInitialize){
		
		/* set the Flag */
		isDiscoveryLedRedInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedRed);
	}
	
	/* change state of the LED */
	togglePin(&DiscoveryLedRed);
}

/**
	*************************************************************************************
  * @brief  change the state of the blue LED on the DiscoveryBoard, OFF-> ON .... ON -> OFF
  * @param  None
  * @retval None
	**************************************************************************************
  */
void toggleDiscoveryLedBlue(void){
	
	/* if the initialisation isn't done before */
	if(!isDiscoveryLedBlueInitialize){
		
		/* set the Flag */
		isDiscoveryLedBlueInitialize = TRUE;
		
		/* initialize */
		initPin(&DiscoveryLedBlue);
	}
	
	/* change state of the LED */
	togglePin(&DiscoveryLedBlue);
}

/**
	*************************************************************************************
  * @brief  get the state of the button
  * @param  None
  * @retval Pinstate 1(high) or 0(low)
	**************************************************************************************
  */
uint8_t getButton1(void){
	
	/* if the initialisation isn't done before */
	if(!isButton1Initialize){
			
			/* set the Flag */
		isButton1Initialize = TRUE;
		
		/* initialize */
		initPin(&Button1);
	}
	
	/* return the state */
	return getPin(&Button1);
}

/**
	*************************************************************************************
  * @brief  get the state of the button
  * @param  None
  * @retval Pinstate 1(high) or 0(low)
	**************************************************************************************
  */
uint8_t getButton2(void){
	
	/* if the initialisation isn't done before */
	if(!isButton2Initialize){
			
			/* set the Flag */
		isButton2Initialize = TRUE;
		
		/* initialize */
		initPin(&Button2);
	}
	
	/* return the state */
	return getPin(&Button2);
}

/**
	*************************************************************************************
  * @brief  get the state of the button
  * @param  None
  * @retval Pinstate 1(high) or 0(low)
	**************************************************************************************
  */
uint8_t getButton3(void){
	
	/* if the initialisation isn't done before */
	if(!isButton3Initialize){
			
			/* set the Flag */
		isButton3Initialize = TRUE;
		
		/* initialize */
		initPin(&Button3);
	}
	
	/* return the state */
	return getPin(&Button3);
}

/**
	*************************************************************************************
  * @brief  get the state of the button
  * @param  None
  * @retval Pinstate 1(high) or 0(low)
	**************************************************************************************
  */
uint8_t getButton4(void){
	
	/* if the initialisation isn't done before */
	if(!isButton4Initialize){
			
			/* set the Flag */
		isButton4Initialize = TRUE;
		
		/* initialize */
		initPin(&Button4);
	}
	
	/* return the state */
	return getPin(&Button4);
}

/**
	*************************************************************************************
  * @brief  get the state of the button
  * @param  None
  * @retval Pinstate 1(high) or 0(low)
	**************************************************************************************
  */
uint8_t getBlueButtonDiscoveryBoard(void){
	
	/* if the initialisation isn't done before */
	if(!isBlueButtonDiscoveryBoardInitialize){
			
			/* set the Flag */
		isBlueButtonDiscoveryBoardInitialize = TRUE;
		
		/* initialize */
		initPin(&BlueButtonDiscoveryBoard);
	}
	
	/* return the state */
	return getPin(&BlueButtonDiscoveryBoard);
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
