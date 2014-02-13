/**
  ******************************************************************************
  * @file    i2c.h
  * @author  Schüpbach Simon, N. Käser
    *                    base frame: http://eliaselectronics.com/stm32f4-tutorials/stm32f4-i2c-master-tutorial/
    * @date    14.01.2014
    *
    * @version 1.1
    *   updated functions: added timeout variable to while loops (14.01.2014)
    * @version 1.0
    *   create this file (22.05.2013)
    *
  * @brief   this file include functions for the i2c handling
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
//#include "include.h"


/* add here the address of the I2C-Slave */
typedef enum{
    AUDIO = 0x94,
    DISPLAY_7Segment = 0x70
}SlaveI2C;


/* pin/port and interface configurations */
#define I2C_INTERFACE               I2C1 /* use I2C1 */
#define I2C_PORT_CLK                RCC_APB1Periph_I2C1
#define I2C_AF                      GPIO_AF_I2C1
#define I2C_ERROR_INTERRUPT_CHANNEL I2C1_ER_IRQn
#define I2C_ERROR_INTERRUPT         I2C1_ER_IRQHandler

#define I2C_SCL_PIN         GPIO_Pin_6      /* PB.6 */
#define I2C_SCL_SOURCE      GPIO_PinSource6
#define I2C_SCL_PORT        GPIOB
#define I2C_SDA_PIN         GPIO_Pin_9        /* PB.9 */
#define I2C_SDA_SOURCE      GPIO_PinSource9
#define I2C_SDA_PORT        GPIOB


/* Speed of I2C Bus , 100kHz */
#define CLOCK_SPEED         100000

/* own Adress */
#define OWN_ADRESS          0x20

/* initialize th I2C interface*/
extern void initI2C(void);

/* write data with I2C */
extern void writeI2C(SlaveI2C SlaveAddr, uint8_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite, uint32_t timeout );

/* read data with I2C */
extern void readI2C(SlaveI2C SlaveAddr, uint8_t* pBuffer, uint16_t NumByteToRead, uint32_t timeout );

/* while-timeout flag, 1 if timeout occured, 0 if no timeout occured */
extern uint8_t i2c_timeout_flag;

#endif
