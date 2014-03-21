/**
 * \file    spi.h
 * \author  Haldemann Jascha
 * \author  haldj3
 * \date    2014-03-20
 *
 * \version 1.0
 *  create this file
 *
 * \brief   this file contains function for the spi handling (send only).
 *
 * \defgroup spi SPI
 * \brief   SPI library
 * \ingroup firmware
 *
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

/* exported typedef ----------------------------------------------------------*/

/* exported define -----------------------------------------------------------*/
#define SPI_INTERFACE_NUMBER    1

#define SPI_PIN_SCK         GPIO_Pin_5
#define SPI_PORT_SCK        GPIOA            // enable related clock in spi.c
#define GPIO_PinSource_SCK  GPIO_PinSource5

//#define SPI_PIN_MISO        GPIO_Pin_6    // not used
//#define SPI_PORT_MISO       GPIOA         // enable related clock in spi.c
//#define GPIO_PinSource_MISO GPIO_PinSource6

#define SPI_PIN_MOSI        GPIO_Pin_7
#define SPI_PORT_MOSI       GPIOA           // enable related clock in spi.c
#define GPIO_PinSource_MOSI GPIO_PinSource7

#define SPI_PIN_CS          GPIO_Pin_2
#define SPI_PORT_CS         GPIOB

/* exported types ------------------------------------------------------------*/
/* exported constants --------------------------------------------------------*/
/* exported macro ------------------------------------------------------------*/

/* exported functions --------------------------------------------------------*/
extern void init_SPI();
extern void SPI_send_byte(uint8_t);

#endif

/**
 * @}
 */
