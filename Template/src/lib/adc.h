/*******************************************************************************
 * @file    adc.h
 * @author  Schüpbach Simon
 * @date    17.05.2013
 *
 * @version 2.0
 * 	change the parameter of the function to pointer
 *
 * @version 1.0
 * 	create this file
 *
 * @brief   this file contains function for config a pin for a analog-signal measure
 ******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


#define SAMPLE_TIME 	ADC_SampleTime_15Cycles
		 
/* Exported types ------------------------------------------------------------*/

/*******************************************************************************
 * @brief  ADC Init structure definition
 ******************************************************************************/
/*********************************************
 * Initialization example
 *********************************************
 * ADCInitStruct Sensor1 = {
 *		GPIOB,
 *		{
 *			GPIO_Pin_0,
 *			GPIO_Mode_AN,
 *			GPIO_Speed_50MHz,
 *			GPIO_OType_PP,
 *			GPIO_PuPd_NOPULL
 *		},
 *		ADC1,
 *		ADC_Channel_8
 *	};
 *
 * initADC(&Sensor1);
 ******************************************************************************/
typedef struct{
	GPIO_TypeDef* Port;		/* GPIOA...GPIO */
	GPIO_InitTypeDef Gpio;	/* Pins, mode, speed,pullup resistor */
	ADC_TypeDef* Adc;		/* ADC1 or ADC2 or ADC3 */
	uint8_t Channel;		/* ADC_Channel_1....ADC_Channel_15 */
}ADCInitStruct;

/*******************************************************************************
 * @brief  RGB Init structure definition
 ******************************************************************************/
/*******************************************************************************
 * @attention	for trouble-free functionality it is important that an ADC isn't
 * 				double used. (should be ADC1 and ADC2 and ADC3 )
 *******************************************************************************
 * Initialization example
 ********************************
 * RGBInitStruct RgbSensor1 = {
 * {
 * 	GPIOA,
 *	{
 *		GPIO_Pin_1,
 *		GPIO_Mode_AN,
 *		GPIO_Speed_50MHz,
 *		GPIO_OType_PP,
 *		GPIO_PuPd_NOPULL
 *	},
 *	ADC1,
 *	ADC_Channel_1
 * },
 * {
 *	GPIOA,
 *	{
 *		GPIO_Pin_2,
 *		GPIO_Mode_AN,
 *		GPIO_Speed_50MHz,
 *		GPIO_OType_PP,
 *		GPIO_PuPd_NOPULL
 *	},
 *	ADC2,
 *	ADC_Channel_2
 * },
 * {
 *	GPIOA,
 *	{
 *		GPIO_Pin_3,
 *		GPIO_Mode_AN,
 *		GPIO_Speed_50MHz,
 *		GPIO_OType_PP,
 *		GPIO_PuPd_NOPULL
 *	},
 *	ADC3,
 *	ADC_Channel_3
 * }
 *};
 *
 * initRGB(&RgbSensor1);
 ******************************************************************************/
typedef struct{
	ADCInitStruct Red_Part;
	ADCInitStruct Green_Part;
	ADCInitStruct Blue_Part;
}RGBInitStruct;

/*******************************************************************************
 * @brief  Struct for 3 values of a RGB-LED
 ******************************************************************************/
typedef struct{
	uint16_t Red_Part;
	uint16_t Green_Part;
	uint16_t Blue_Part;
}RGBValue;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* init a pin in ADC-mode */
void initADC(ADCInitStruct *Adc);

/* init 3 pins in ADC-mode */
void initRGB(RGBInitStruct *Rgb);

/* get the measurement value of a ADC channel */
uint16_t getADC(ADCInitStruct *Adc);

/* get the 3 measurements values of the 3 ADC channels */
RGBValue getRGB(RGBInitStruct *Rgb);

/* initialize the DMA and ADC for ADC-Measurement */
void initADCwithDMA(ADCInitStruct *ADCStruct_Array, uint8_t NumOfEntry, ADC_TypeDef* ADCx, uint16_t *Value);

#endif
