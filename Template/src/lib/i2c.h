/**
  ******************************************************************************
  * @file    i2c.h 
  * @author  Schüpbach Simon
	*					 base frame: http://eliaselectronics.com/stm32f4-tutorials/stm32f4-i2c-master-tutorial/
	* @date    22.05.2013
	*
	* @version 1.0	
	*	create this file
	*
  * @brief   this file include functions for the i2c handling
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	
/* add here the address of the I2C-Slave */
typedef enum{
	AUDIO = 0x94,
	DISPLAY_7Segment = 0x70
}SlaveI2C;


#define I2C							I2C1 					/* use I2C1 */

#define I2C_SCL_PIN 		GPIO_Pin_6		/* PB.6 */		
#define I2C_SCL_PORT 		GPIOB
#define I2C_SDA_PIN 	  GPIO_Pin_9		/* PB.9 */
#define I2C_SDA_PORT 		GPIOB			
	

/* Speed of I2C Bus , 100kHz */
#define CLOCK_SPEED			100000

/* own Adress */
#define OWN_ADRESS			0x20
	 
/* initialize th I2C interface*/
extern void initI2C(void);

/* write data with I2C */
extern void writeI2C(SlaveI2C SlaveAddr, uint8_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite );

/* read data with I2C */
extern void readI2C(SlaveI2C SlaveAddr, uint8_t* pBuffer, uint16_t NumByteToRead );

#endif
