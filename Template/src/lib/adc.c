/*******************************************************************************
 * @file    adc.c
 * @author  Schüpbach Simon
 * @date    17.05.2013
 *
 * @version 2.0
 *	change the parameter of the function to pointer
 *
 * @version 1.0
 *	create this file
 *
 * @brief   this file contains function for config a pin for a analog-signal measure
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "adc.h"
//#include "include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
 * @brief  this function init 3 ADC and could be used for initialisation of a RGB-LED
 * @param  Rgb:	Struct with all necessary information for initialization
 * @retval None
 ******************************************************************************/
void initRGB(RGBInitStruct *Rgb){
	
	/* init the 3 Channels as normal ADC of the RGB */
	initADC(&(Rgb->Red_Part));
	initADC(&(Rgb->Green_Part));
	initADC(&(Rgb->Blue_Part));
}

/******************************************************************** **********
 * @brief  this function init a pin for analog input meaure
 * @param  Adc:	Struct with all necessary information for initialization
 * @retval None
 ******************************************************************************/
void initADC(ADCInitStruct *Adc)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* Enables the AHB1 peripheral clock */
	enablePort(Adc->Port);
	
 
	/* enables the selected adc clock */
	if(Adc->Adc == ADC1) { RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); }
	if(Adc->Adc == ADC2) { RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); }
	if(Adc->Adc == ADC3) { RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); }

	/* init the gpio */
	GPIO_Init(Adc->Port,&Adc->Gpio);

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_14Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC Init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(Adc->Adc, &ADC_InitStructure);

	/* ADC regular channel configuration */
	ADC_RegularChannelConfig(Adc->Adc, Adc->Channel, 1, SAMPLE_TIME);

	/* enable ADC */
	ADC_Cmd(Adc->Adc, ENABLE);
}

/*******************************************************************************
 * @brief		this function read 3 ADC channels
 * @param  		Rgb:	Struct with all necessary information for reading a adc value
 * @retval 		value: the 3 values of the 3 measurements
 *
 * @attention	for trouble-free functionality it is important that an ADC isn't
 * 				double used. (should be ADC1 and ADC2 and ADC3 )
 ******************************************************************************/
RGBValue getRGB(RGBInitStruct *Rgb)
{
	
	/* variable for the 3 return values*/
	RGBValue value = {0,0,0};

	/* config measurement 1 */
 	ADC_RegularChannelConfig(Rgb->Red_Part.Adc, Rgb->Red_Part.Channel, 1, SAMPLE_TIME);
	
	/* start measurement 1 */
	ADC_SoftwareStartConv(Rgb->Red_Part.Adc);

	/* check if a ADC isn't double used */
	if( Rgb->Green_Part.Adc != Rgb->Red_Part.Adc ){
		
		/* config measurement 2 */
		ADC_RegularChannelConfig(Rgb->Green_Part.Adc, Rgb->Green_Part.Channel, 1, SAMPLE_TIME);
		
		/* start measurement 2 */
		ADC_SoftwareStartConv(Rgb->Green_Part.Adc);	
		
		/* check if a ADC isn't double used */
		if((Rgb->Blue_Part.Adc != Rgb->Red_Part.Adc )&&(Rgb->Blue_Part.Adc != Rgb->Green_Part.Adc)){

			/* config measurement 3 */
			ADC_RegularChannelConfig(Rgb->Blue_Part.Adc , Rgb->Blue_Part.Channel, 1, SAMPLE_TIME);
			
			/* start measurement 3 */
			ADC_SoftwareStartConv(Rgb->Blue_Part.Adc);
			
			/* wait while the measurements aren`t finished */
			while((ADC_GetFlagStatus(Rgb->Red_Part.Adc,ADC_FLAG_EOC) != SET)||
					(ADC_GetFlagStatus(Rgb->Green_Part.Adc,ADC_FLAG_EOC) != SET)||
					(ADC_GetFlagStatus(Rgb->Blue_Part.Adc,ADC_FLAG_EOC) != SET))
			{
				;
			}
			
			/* get the 3 measurements values */
			value.Red_Part = ADC_GetConversionValue(Rgb->Red_Part.Adc);
			value.Green_Part = ADC_GetConversionValue(Rgb->Green_Part.Adc);
			value.Blue_Part = ADC_GetConversionValue(Rgb->Blue_Part.Adc);		
		}else{
			#ifdef DEBUGGING
				while(1){}
			#endif 
	}
	}else{
		#ifdef DEBUGGING
			while(1){}
		#endif 
	}
	
	/* return the measurements */
	return value;
}

/*******************************************************************************
 * @brief  this function read a ADC channel
 * @param  Adc:	Struct with all necessary information for reading a adc value
 * @retval value: the value of the measurement
 ******************************************************************************/
uint16_t getADC(ADCInitStruct *Adc)
{
	/* variable for the return value */
	uint16_t value = 0;
	
	/* config measurement */
	ADC_RegularChannelConfig(Adc->Adc, Adc->Channel, 1, SAMPLE_TIME);
	
	/* start measurement */
	ADC_SoftwareStartConv(Adc->Adc);
	
	/* wait while the measurements aren`t finished */	
	while(ADC_GetFlagStatus(Adc->Adc,ADC_FLAG_EOC) != SET){;}
	
	/* get the measurement */
	value = ADC_GetConversionValue(Adc->Adc);
		
	/* return the measurement value */
	return value;
}


/*******************************************************************************
 * @brief  this function initialize the DMA and ADC for ADC-Measurement
 * @param  First_ACD: address of the first ADC-struct wich is to initializise
 * @param  NumOfEntry: number if ADC-struct wich are to initializise
 * @param  ACDx: ADC wich measure ( ADC1 or ADC2 or ADC3 )
 * @param  Value: address of the buffer in wich the dma store the values
 * @retval None
 ******************************************************************************/
void initADCwithDMA(ADCInitStruct *ADCStruct_Array, uint8_t NumOfEntry, ADC_TypeDef* ADCx, uint16_t *Value){

	DMA_Stream_TypeDef* 	DMA_Stream;
	ADCInitStruct*				ADC_Array = ADCStruct_Array;
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef  			DMA_InitStructure;
	uint32_t 							DMA_Channel;
	uint32_t 							Adress;
	uint8_t 							a=0;
	
	/* Enables the AHB1 peripheral clock */
	for(a=0;a<NumOfEntry;a++){
		enablePort(ADC_Array->Port);
		ADC_Array++;
	}
	
	/* enables the selected adc clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	/* enables the selected adc clock  and get setting to the associated streams and channel of the DMA*/
	if(ADCx == ADC1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
		DMA_Stream = DMA2_Stream0;
		DMA_Channel = DMA_Channel_0;
		Adress =0x4001204C;
	}
	if(ADCx == ADC2) { 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); 
		DMA_Stream = DMA2_Stream2;
		DMA_Channel = DMA_Channel_1;
		Adress =0x4001214C;
	}
	if(ADCx == ADC3) { 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); 
		DMA_Stream = DMA2_Stream0;
		DMA_Channel = DMA_Channel_2;
		Adress =0x4001224C;
	}

	/* set the pointer to the first entry */
	ADC_Array = ADCStruct_Array;
	
	/* init the gpio */
	for(a=0;a<NumOfEntry;a++){
		GPIO_Init(ADC_Array->Port,&(ADC_Array->Gpio));
		ADC_Array++;
	}
		
	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_14Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC Init */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = NumOfEntry;
	ADC_Init(ADCx, &ADC_InitStructure);

	/* DMA Stream and channel configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)Adress);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize =NumOfEntry;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_Stream, &DMA_InitStructure);
	
	/* Enable DMA */
	DMA_Cmd(DMA_Stream, ENABLE);
	
	
	/* set the pointer to the first entry */
	ADC_Array = ADCStruct_Array;
	
	/* ADC regular channel configuration */
	for(a=0;a<NumOfEntry;a++){
		ADC_RegularChannelConfig(ADCx, ADC_Array->Channel, a+1, SAMPLE_TIME);
		ADC_Array++;
	}
	
	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

	/* Enable ADC DMA */
	ADC_DMACmd(ADCx, ENABLE);
	
	/* enable ADC */
	ADC_Cmd(ADCx, ENABLE);
	
		/* start measurement */
	ADC_SoftwareStartConv(ADCx);
}
