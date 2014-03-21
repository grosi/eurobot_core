/**
 * \file    spi.c
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
 *
 */

#include <stm32f4xx.h>
#include <stm32f4xx_spi.h>

#include "spi.h"

/**
 * \fn      init_SPI1
 * \brief   initializes the SPI1 peripheral
 */
void init_SPI1(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* configure pins used by SPI1
	 * PA5 = SCK
	 * (PA6 = MISO)	used as RS instead
	 * PA7 = MOSI
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5; // | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure the chip select pin
	   in this case we will use PB2 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_WriteBit(GPIOB, GPIO_Pin_2, SET); // set PB2 (CS) high

	// enable peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* configure SPI1 in Mode 0
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx; // set to simplex mode, (MOSI only)
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Hard; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is APB2 frequency / 4 (=42MHz)
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI1, &SPI_InitStruct);

	SPI_Cmd(SPI1, ENABLE); // enable SPI1
}


/**
 * \fn      SPI1_send
 * \brief   used to transmit data
 *
 * \param[in]   data to be transmitted
 * \return  None
 */
void SPI1_send(uint8_t data)
{
	SPI1->DR = data; // write data to be transmitted to the SPI data register
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, RESET); // set PB2 (CS) low
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	//while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI1->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	GPIO_WriteBit(GPIOB, GPIO_Pin_2, SET); // set PB2 (CS) high
	//return SPI1->DR; // return received data from SPI data register
}

// Use:
//
//	init_SPI1();
//
//  uint8_t data = 0xAA;
//
//	while(1)
//  {
//		SPI1_send(data);  // transmit data
//	}


/**
 * @}
 */
