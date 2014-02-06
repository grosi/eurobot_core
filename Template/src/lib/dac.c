/*******************************************************************************
 * @file	dac.c
 * @author	Schüpbach Simon
 * @date	17.05.2013
 *
 * @version 1.0
 * 	create this file
 *
 * @brief	config the DAC for a single output voltage genration. The voltage change
 * 			every time the value in the function setADC() is changed.
 ******************************************************************************/
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "gpio.h"
#include "dac.h"
//#include "include.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
 * @brief  initialize the dac
 * @param  None
 * @retval None
 ******************************************************************************/
void initDAC(DACStruct *Dac){

	GPIO_InitTypeDef GPIO_InitStruct;
	DAC_InitTypeDef  DAC_InitStruct;

	/* config clock for using DAC*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	/* enable clock for DAC pins */
	enablePort(Dac->Port);
	
	/* pin configuration for DAC */
	GPIO_InitStruct.GPIO_Pin = Dac->Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(Dac->Port, &GPIO_InitStruct);

	/* DAC channel configuration */
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None; 
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_Init(Dac->Channel, &DAC_InitStruct);

	/* Enable DAC channel */
	DAC_Cmd(Dac->Channel, ENABLE);
}

/*******************************************************************************
 * @brief  set the output value
 * @param  Value: Output value, 0 to 0x0fff
 * @param  DAC_Channel: DAC_Channel_1 or DAC_Channel_2
 * @retval None
 ******************************************************************************/
void setDAC(uint16_t Value, DACStruct *Dac){


	/* write the value in the selected channel register */
	switch(Dac->Channel){
		case DAC_Channel_1: DAC_SetChannel1Data(DAC_Align_12b_R, Value);
			break;
		case DAC_Channel_2: DAC_SetChannel2Data(DAC_Align_12b_R, Value);
			break;
	}
}

/*******************************************************************************
 * @brief  get the last generated output value
 * @param  DAC_Channel: DAC_Channel_1 or DAC_Channel_2
 * @retval get the last output value
 ******************************************************************************/
uint16_t getDAC(DACStruct *Dac){
	/* get the last output value */
	return DAC_GetDataOutputValue(Dac->Channel);
 }

