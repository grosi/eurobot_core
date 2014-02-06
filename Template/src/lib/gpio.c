/**
 * @file	gpio.c
 * @author	Schüpbach Simon
 * @date	17.05.2013
 *
 * @version	2.0
 * 	change the parameter of the function to pointer
 *
 * @version	1.0
 * create this file
 *
 * @brief	this file contains function for initialize pins and ports or
 * 			enable clocks and alternate functions.
 */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/******************************************************************** **********
 * @brief  this function enable the clock of a port
 * @param  GPIOx: GPIOA....GPIOI
 * @retval the port number, GPIOA=1....GPIOI=8
 ******************************************************************************/
uint8_t enablePort(GPIO_TypeDef* GPIOx)
{
	uint8_t value;
	if(GPIOx == GPIOA) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); value = 0;}
	if(GPIOx == GPIOB) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); value = 1;}
	if(GPIOx == GPIOC) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); value = 2;}
	if(GPIOx == GPIOD) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); value = 3;}
	if(GPIOx == GPIOE) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); value = 4;}
	if(GPIOx == GPIOF) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); value = 5;}
	if(GPIOx == GPIOG) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); value = 6;}
	if(GPIOx == GPIOH) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE); value = 7;}
	if(GPIOx == GPIOI) { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE); value = 8;}
	return value;
}


/*******************************************************************************
 * @brief  this function enable the clock of a port and config the AF of a pin
 * @param  GPIOx: GPIOA....GPIOI
 * @param  Gpio: Struct for the initialisation of pins
 * @param  AF_Function: the selected AF
 * @retval None
 ******************************************************************************/
void enableAFPort(GPIO_TypeDef* GPIOx ,GPIO_InitTypeDef Gpio, uint16_t AF_Function)
{
	/* Enables the AHB1 peripheral clock */
	enablePort(GPIOx);
	
	/* init the selected pin of a port */
	GPIO_Init(GPIOx,&Gpio);
	
	/* config a pin in AF-mode */
	if((Gpio.GPIO_Pin & GPIO_Pin_0 ) == GPIO_Pin_0){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource0, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_1 ) == GPIO_Pin_1){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource1, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_2 ) == GPIO_Pin_2){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource2, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_3 ) == GPIO_Pin_3){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource3, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_4 ) == GPIO_Pin_4){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource4, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_5 ) == GPIO_Pin_5){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource5, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_6 ) == GPIO_Pin_6){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource6, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_7 ) == GPIO_Pin_7){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource7, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_8 ) == GPIO_Pin_8){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource8, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_9 ) == GPIO_Pin_9){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource9, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_10 ) == GPIO_Pin_10){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource10, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_11 ) == GPIO_Pin_11){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource11, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_12 ) == GPIO_Pin_12){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource12, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_13 ) == GPIO_Pin_13){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource13, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_14 ) == GPIO_Pin_14){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource14, AF_Function); }
	if((Gpio.GPIO_Pin & GPIO_Pin_15 ) == GPIO_Pin_15){ GPIO_PinAFConfig(GPIOx, GPIO_PinSource15, AF_Function); }
}


/*******************************************************************************
 * @brief  this function enable the clock of a port and config a pin
 * @param  Pin: Struct with all necessary values for initalisation
 * @retval None
 ******************************************************************************/
void initPin(PinInitStruct *Pin)
{
	/* Enables the AHB1 peripheral clock */
	enablePort(Pin->Port);
	
	/* init the selected pin of a port */
	GPIO_Init(Pin->Port,&(Pin->Gpio));
}


/*******************************************************************************
 * @brief   this function enable the clock of a port and config a pin and controll the
 *          he input with a interrupt
 * @param   Irq: Struct with all necessary values for initalisation
 * @retval  None
 ******************************************************************************/
void initIRQ(IRQInitStruct *Irq){
	
	/* variable to convert the pin into a value*/
	uint8_t PinSource=0;
	
	/* variable to convert the port into a value*/
	uint8_t PortSource=0;
	
	/* variable to compare with a pin*/
	uint32_t Pin=0x00000001;
	
	/* Enables the AHB1 peripheral clock and get the port number*/
	PortSource=enablePort(Irq->Port);
	
	/* init the selected pin of a port */
	GPIO_Init(Irq->Port,&(Irq->Gpio));
	
	/* enable the clock of EXTI */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		
	
	/* find out the pin-number */
	while(Irq->Gpio.GPIO_Pin != Pin){
		PinSource++;
		Pin=Pin<<1;
	}

	/* enable the EXTI for the selected pin and port*/
	SYSCFG_EXTILineConfig(PortSource, PinSource);
	
	/* init the EXTI*/
	EXTI_Init(&Irq->Exti);
	
	/* init the interrupt */
	NVIC_Init(&Irq->Nvic);

}


/*******************************************************************************
 * @brief  this function set the bit of the selected pin
 * @param  Pin: Struct with pin and port information
 * @retval None
 ******************************************************************************/
inline void setPin(PinInitStruct *Pin)
{
	GPIO_SetBits(Pin->Port,Pin->Gpio.GPIO_Pin);
}


/*******************************************************************************
 * @brief  this function reset the bit of the selected pin
 * @param  Pin: Struct with pin and port information
 * @retval None
 ******************************************************************************/
inline void resetPin(PinInitStruct *Pin)
{
	GPIO_ResetBits(Pin->Port, Pin->Gpio.GPIO_Pin);
}

/*******************************************************************************
 * @brief  this function toggle the bit of the selected pin
 * @param  Pin: Struct with pin and port information
 * @retval None
 ******************************************************************************/
inline void togglePin(PinInitStruct *Pin)
{
	GPIO_ToggleBits(Pin->Port, Pin->Gpio.GPIO_Pin);
}

/*******************************************************************************
 * @brief  this function get the bit of the selected pin
 * @param  Pin: Struct with pin and port information
 * @retval None
 *******************************************************************************/
inline uint8_t getPin(PinInitStruct *Pin)
{
	return GPIO_ReadInputDataBit(Pin->Port, Pin->Gpio.GPIO_Pin);
}

/*******************************************************************************
 * @brief  this function set the bit of the selected pin
 * @param  Pin: IRQ Struct with pin and port information
 * @retval None
 ******************************************************************************/
inline void setIRQPin(IRQInitStruct *Pin)
{
	GPIO_SetBits(Pin->Port, Pin->Gpio.GPIO_Pin);
}

/*******************************************************************************
 * @brief  this function reset the bit of the selected pin
 * @param  Pin: Struct with pin and port information
 * @retval None
 ******************************************************************************/
inline void resetIRQPin(IRQInitStruct *Pin)
{
	GPIO_ResetBits(Pin->Port, Pin->Gpio.GPIO_Pin);
}

/*******************************************************************************
 * @brief  this function toggle the bit of the selected pin
 * @param  Pin: Struct with pin and port information
 * @retval None
 ******************************************************************************/
inline void toggleIRQPin(IRQInitStruct *Pin)
{
	GPIO_ToggleBits(Pin->Port, Pin->Gpio.GPIO_Pin);
}

/************** ****************************************************************
 * @brief  this function get the bit of the selected pin
 * @param  Pin: Struct with pin and port information
 * @retval None
 ******************************************************************************/
inline uint8_t getIRQPin(IRQInitStruct *Pin)
{
	return GPIO_ReadOutputDataBit(Pin->Port, Pin->Gpio.GPIO_Pin);
}
