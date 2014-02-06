/**
  ******************************************************************************
  * @file    spi.c
  * @author  Schüpbach Simon
	* @date    27.05.2013
	*
	* @version 1.0	
	*	create this file
	*
  * @brief   this file include functions for the spi handling
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "gpio.h"
#include "spi.h"
//#include "include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Dummy Byte Send by the SPI Master device in order to generate the SPI Clock  */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/* struct for the CS of the motion sensor */
PinInitStruct MotionSensorCS = {
	GPIOE,
	{
		GPIO_Pin_3,
		GPIO_Mode_OUT,
		GPIO_Speed_2MHz,
		GPIO_OType_PP,
		GPIO_PuPd_UP
	}
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
	*************************************************************************************
  * @brief  set the ChipSelect to low
  * @param  Peripherie: Enum of the peripherie
	* @retval None
	**************************************************************************************
  */
void enableCS_SPI(PeriphSPI Peripherie){
	switch(Peripherie){
		case MOTION_SENSOR: resetPin(&MotionSensorCS);
		break;
	}
}


/**
	*************************************************************************************
  * @brief  set the ChipSelect to high
  * @param  Peripherie: Enum of the peripherie
	* @retval None
	**************************************************************************************
  */
void disableCS_SPI(PeriphSPI Peripherie){
	switch(Peripherie){
		case MOTION_SENSOR: setPin(&MotionSensorCS);
		break;
	}
}


/**
	*************************************************************************************
  * @brief  start a conversation
  * @param  None
	* @retval None
	**************************************************************************************
  */
void initSPI( void ) {
  SPI_InitTypeDef  SPI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* value for the AF function */
	uint16_t AF_Function;
	
	/* enable SPI and set AF function */
	switch((intptr_t)SPI){
		case (intptr_t)SPI1:	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
													AF_Function = GPIO_AF_SPI1;
		break;
		case (intptr_t)SPI2:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
													AF_Function = GPIO_AF_SPI2;
		break;
		case (intptr_t)SPI3:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);
													AF_Function = GPIO_AF_SPI3;
		break;
	}
			

  /* Enable the pgio clock*/
  GPIO_InitStruct.GPIO_Pin = SPI_SCK_PIN; 			/* set SCK pin*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			/* set pins to alternate function */
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	/* set GPIO speed */
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		/* set output to push pull */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;		/* enable pull up resistors */
	enableAFPort(SPI_SCK_PORT, GPIO_InitStruct, AF_Function); /* enable CLK Pin*/
  
	GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN; 			/* set MISO pin*/
	enableAFPort(SPI_MISO_PORT, GPIO_InitStruct, AF_Function); /* enable */
	
	GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN; 			/* set MOSI pin*/
	enableAFPort(SPI_MOSI_PORT, GPIO_InitStruct, AF_Function); /* enable */
	
  /* SPI configuration */
	/* Select 4 wire mode */
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	/* Define device as master */
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	/* Set datasize to 8 bit */
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	/* Set clock polarity to high level */
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	/* Set clock phase to first edge use */
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	/* Disable slave select */
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	/* Set baudrate */
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	/* Set msb first mode */
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	/* Reset crcpolinomial (7 = reset value) */
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	
	/* Initialize the spi with this configuration */
  SPI_Init(SPI, &SPI_InitStruct);

	/* Enable the spi port */
  SPI_Cmd(SPI, ENABLE);
}



/**
	*************************************************************************************
  * @brief  send a byte
  * @param  byte: data to send
	* @retval the received byte
	**************************************************************************************
  */
static uint8_t sendByte(uint8_t byte)
{
	/* Time to wait before abort */
	volatile uint16_t TimeOut = 0x1000;
	
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((TimeOut--) == 0){
			#ifdef DEBUGGING
				while(1);
			#endif
			return 0;
		}
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(SPI, byte);
  
  /* Wait to receive a Byte */
  TimeOut = 0x1000;
  while (SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((TimeOut--) == 0){
			#ifdef DEBUGGING
				while(1);
			#endif
			return 0;
		}
  }
  
  /* Return the byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(SPI);
}


/**
	*************************************************************************************
  * @brief  write data to the SPI-Bus
  * @param  Peripherie: data to send
  * @param  WriteAddr: Adress of the register
	* @param  NumByteToWrite: number of bytes to send
  * @param  Data: data to send
	* @retval the received byte
	**************************************************************************************
  */
void writeSPI(PeriphSPI Peripherie, uint8_t WriteAddr, uint16_t NumByteToWrite, uint8_t* Data){
	
	/* Set the chip select line to low level */
  enableCS_SPI(Peripherie);
	
	/* Send the Address of the indexed register */
  sendByte(WriteAddr);
	
  /* Send the data that will be written into the device (MSB First) */
	while(NumByteToWrite >= 0x01)
  {
    sendByte(*Data);
    NumByteToWrite--;
    Data++;
  }
	
  /* Set the chip select line to high level */
  disableCS_SPI(Peripherie);
}


/**
	*************************************************************************************
  * @brief  read data from a SPI-Slave
  * @param  Peripherie: Peripherie-Slave
  * @param  ReadAddr: Adress of the register
	* @param  NumByteToWrite: number of bytes to send
  * @param  Data: data to send
	* @retval the received byte
	**************************************************************************************
  */
void readSPI(PeriphSPI Peripherie, uint8_t ReadAddr, uint16_t NumByteToRead, uint8_t* Data)
{  
	/* enable CS */
	enableCS_SPI(Peripherie);
  
  /* Send the Address of the indexed register */
  sendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock  */
    *Data = sendByte(DUMMY_BYTE);
    NumByteToRead--;
    Data++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  disableCS_SPI(Peripherie);
}

/**
	*************************************************************************************
  * @brief  initialize the CS of the motion sensor and config them
  * @param  None
	* @retval None
	**************************************************************************************
  */
void initMotionSensor(void){
	uint8_t Data[8]={0};
	
	/* initialize */
	initPin(&MotionSensorCS);
	
	/* set to high -> disable */
	setPin(&MotionSensorCS);

	/* settings, more in datasheet of lis302dl */ 	
	Data[0] = 0x47; 		
	
	/* write data to the motion sensor */
 	writeSPI(MOTION_SENSOR, 0x20, 1, Data);
}

/**
	*************************************************************************************
  * @brief  get the x y and z data of the motion sensor
  * @param  Buffer for the Data
	* @retval None
	**************************************************************************************
  */
void getXYZDataMotionSensor(uint8_t *Data){
	
	/* read the data */
	 readSPI(MOTION_SENSOR, 0x29 | 0x80 |0x40 ,6 , Data);
}
