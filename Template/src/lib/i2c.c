/**
  ******************************************************************************
  * @file    i2c.c 
  * @author  Schüpbach Simon
*					 base frame: http://eliaselectronics.com/stm32f4-tutorials/stm32f4-i2c-master-tutorial/
* @date    22.05.2013
*
* @version 1.0
*	create this file
*
  * @brief   this file include functions for the i2c handling
  * \todo   rewrite the init function -> direct STM-lib access
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/**
	*************************************************************************************
  * @brief  initialize the i2c interface
  * @param  None
	* @retval None
	**************************************************************************************
  */
void initI2C(void){
//	GPIO_InitTypeDef GPIO_InitStruct;
//	I2C_InitTypeDef I2C_InitStruct;
//
//	/* value for the AF function */
//	uint16_t AF_Function;
//
//	/* enable I2C and set AF function */
//	if(I2C == I2C1){
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//		AF_Function = GPIO_AF_I2C1;
//	}else {
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
//		AF_Function = GPIO_AF_I2C2;
//	}
//
//	/* initilaizes I2C_SCL*/
//	GPIO_InitStruct.GPIO_Pin = I2C_SCL_PIN; 			/* set SCL pin*/
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			/* set pins to alternate function */
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;/* set GPIO speed */
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;		/* set output to open drain --> the line has to be only pulled low, not driven high */
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* enable pull up resistors */
//	enableAFPort(I2C_SCL_PORT, GPIO_InitStruct, AF_Function); /* enable SCL Pin*/
//
//	/* initilaizes I2C_SDA*/
//	GPIO_InitStruct.GPIO_Pin = I2C_SDA_PIN; 			/* set SDA pin*/
//	enableAFPort(I2C_SDA_PORT, GPIO_InitStruct, AF_Function); /* enable SDA Pin*/
//
//
//	/* configure I2C1 */
//	I2C_InitStruct.I2C_ClockSpeed = CLOCK_SPEED; 		/* set speed */
//	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;					/* set I2C mode */
//	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	/* set 50% duty cycle --> standard */
//	I2C_InitStruct.I2C_OwnAddress1 = OWN_ADRESS;		/* own address */
//	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;				/* enable acknowledge when reading */
//	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; /* set address length to 7 bit addresses */
//	I2C_Init(I2C, &I2C_InitStruct);				/* init I2C1 */
//
//	/* enable I2C */
//	I2C_Cmd(I2C, ENABLE);
}

/**
	*************************************************************************************
  * @brief  start a conversation
  * @param  adress:	adress of the device
  * @param  direction:	I2C_Direction_Transmitter or I2C_Direction_Receiver	
  * @retval None
	**************************************************************************************
  */
static void startByteI2C(uint8_t Address, uint8_t Direction){
	
	/* wait until I2C1 is not busy anymore */
	while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY));

	/* Send I2C1 START condition */
	I2C_GenerateSTART(I2C, ENABLE);

	/* wait for I2C1 EV5 --> Slave has acknowledged start condition */
	while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send slave Address for write */
	I2C_Send7bitAddress(I2C, Address, Direction);

	/* wait for I2C1 EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */
	if(Direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(Direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/**
	*************************************************************************************
  * @brief  write one byte 
  * @param  Data: Byte to write
  * @retval None
	**************************************************************************************
  */
static void writeByteI2C(uint8_t Data){
	
	/* write byte*/
	I2C_SendData(I2C, Data);
	
	/* wait while the byte has been transmitted */
	while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/**
	*************************************************************************************
  * @brief  write one byte and stop condition
  * @param  Data: Byte to write
  * @retval None
	**************************************************************************************
  */
static void writeEndByteI2C(uint8_t Data){
	
	/* write byte*/
	I2C_SendData(I2C, Data);
	
	/* waitwhile the byte has been transmitted */
	while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	/* send stop condition */
	I2C_GenerateSTOP(I2C, ENABLE);
}

/**
	*************************************************************************************
  * @brief  read one byte and acknowledge
  * @param  None
  * @retval the received byte
	**************************************************************************************
  */
static uint8_t readByteI2C(void){
	
	/* enable acknowledge of recieved data */
	I2C_AcknowledgeConfig(I2C, ENABLE);
	
	/* wait until one byte has been received */
	while( !I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	
	/* read data from I2C data register and return data byte */
	return I2C_ReceiveData(I2C);
}


/**
	*************************************************************************************
  * @brief  read one byte without acknowledge and end conversation
  * @param  None
  * @retval the received byte
	**************************************************************************************
  */
static uint8_t readEndByteI2C(void){
	
	/* buffer for the received byte */
	uint8_t ReceivedByte;
	
	/* disabe acknowledge of received data */
	I2C_AcknowledgeConfig(I2C, DISABLE);
	
	/* wait until one byte has been received */
	while( !I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	
	
	/* read data from I2C data register and return data byte */
	ReceivedByte = I2C_ReceiveData(I2C);
	
	/* send stop condition */
	I2C_GenerateSTOP(I2C, ENABLE);
	
	/* return value */
	return ReceivedByte;
}


/**
	*************************************************************************************
  * @brief  write data to the SPI-Bus
  * @param  SlaveAddr: adress of the slave
  * @param  WriteAddr: Adress of the register
  * @param  pBuffer: data to send
	* @param  NumByteToWrite: number of bytes to send
	* @retval the received byte
	**************************************************************************************
  */
void writeI2C(SlaveI2C SlaveAddr, uint8_t WriteAddr, uint8_t* pBuffer,uint16_t NumByteToWrite ){
	
	/* Send the Address of the indexed register */
  startByteI2C(SlaveAddr,I2C_Direction_Transmitter);
	
	
  /* Send the data that will be written into the device (MSB First) */
	writeByteI2C(WriteAddr);
	
	/* if there are bytes to send */
	if(NumByteToWrite > 0x00){
		
		/* send every byte */
		while(NumByteToWrite > 0x01)
		{
			/* send byte */
			writeByteI2C(*pBuffer);
			
			/* decrement byte to send */
			NumByteToWrite--;
			
			/* incrment the buffer position */
			pBuffer++;
		}
		
		/* send the last byte */
		writeEndByteI2C(*pBuffer);
		
	}else{
		
		/* send stop condition */
		I2C_GenerateSTOP(I2C, ENABLE);
	}
}

/**
	*************************************************************************************
  * @brief  Reads a block of data
  * @param  SlaveAddr : address of the slave
  * @param  pBuffer : pointer to the buffer that receives the data
  * @param  NumByteToRead : number of bytes to read
  * @retval None
	**************************************************************************************
  */
void readI2C(SlaveI2C SlaveAddr, uint8_t* pBuffer, uint16_t NumByteToRead )
{  
  
  /* Send the Address of the indexed register */
  startByteI2C(SlaveAddr,I2C_Direction_Receiver);
	
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x01)
  {
    /* read byte */
    *pBuffer = readByteI2C();
		
		/* decrement byte to read */
    NumByteToRead--;
		
		/* increment buffer of the received bytes */
    pBuffer++;
  }
	
	/* read last byte */
	*pBuffer = readEndByteI2C(); 
}


