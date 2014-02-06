/*******************************************************************************
 * @file	dac.h
 * @author	Schüpbach Simon
 * @date	17.05.2013
 *
 * @version	1.0
 * 	create this file
 *
 * @brief	config the DAC for a single output voltage genration. The voltage change
 * 			every time the value in the function setADC() is changed.
 ******************************************************************************/

/* this is only used for the definition VOLTAGE_DISCOVERY */
//#include "..\application\app_config.h"


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DAC_H
#define __DAC_H

/* DAC Channel */
#define DAC1_CHANNEL DAC_Channel_1
#define DAC2_CHANNEL DAC_Channel_2

/* Pin configuration*/
#define DAC1_PIN GPIO_Pin_4
#define DAC1_PORT GPIOA
#define DAC2_PIN GPIO_Pin_5
#define DAC2_PORT GPIOA

/* ratio between the 12b channel value and volt */

#define VALUE_2_VOLTAGE (float)((float)4095.0 / (float)VOLTAGE_DISCOVERYBOARD) /* 4095 (12bit-resolution) */

typedef struct{
	GPIO_TypeDef* Port; /* GPIOA...GPIOH */
	uint32_t Pin; /* GPIO_Pin_0...GPIO_Pin_15 */
	uint32_t Channel; /* DAC_Channel_1 or DAC_Channel_2 */
}DACStruct;

/* initialize the dac */
void initDAC(DACStruct *Dac);

/* set the output value */
void setDAC(uint16_t Value, DACStruct *Dac);

/* get the output value */
uint16_t getDAC(DACStruct *Dac);
#endif
