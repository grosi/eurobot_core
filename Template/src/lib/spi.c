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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* SPI_INTERFACE */
#if SPI_INTERFACE_NUMBER == 1
    #define SPI                      SPI1
    #define GPIO_AF                  GPIO_AF_SPI1
    #define RCC_APBPeriph            RCC_APB2Periph_SPI1
//    #define RCC_APBPeriphClockCmd    RCC_APB2PeriphClockCmd    // allowed?
#endif
#if SPI_INTERFACE_NUMBER == 2
    #define SPI                      SPI2
    #define GPIO_AF                  GPIO_AF_SPI2
    #define RCC_APBPeriph            RCC_APB1Periph_SPI2
//    #define RCC_APBPeriphClockCmd    RCC_APB1PeriphClockCmd
#endif
#if SPI_INTERFACE_NUMBER == 3
    #define SPI                      SPI3
    #define GPIO_AF                  GPIO_AF_SPI3
    #define RCC_APBPeriph            RCC_APB1Periph_SPI3
//    #define RCC_APBPeriphClockCmd    RCC_APB1PeriphClockCmd
#endif
#if SPI_INTERFACE_NUMBER == 4
    #define SPI                      SPI4
    #define GPIO_AF                  GPIO_AF_SPI4
    #define RCC_APBPeriph            RCC_APB2Periph_SPI4
//    #define RCC_APBPeriphClockCmd    RCC_APB2PeriphClockCmd
#endif
#if SPI_INTERFACE_NUMBER == 5
    #define SPI                      SPI5
    #define GPIO_AF                  GPIO_AF_SPI5
    #define RCC_APBPeriph            RCC_APB2Periph_SPI5
//    #define RCC_APBPeriphClockCmd    RCC_APB2PeriphClockCmd
#endif
#if SPI_INTERFACE_NUMBER == 6
    #define SPI                      SPI6
    #define GPIO_AF                  GPIO_AF_SPI6
    #define RCC_APBPeriph            RCC_APB2Periph_SPI6
//    #define RCC_APBPeriphClockCmd    RCC_APB2PeriphClockCmd
#endif

///* SPI_PORT */
//#if SPI_PORT_LETTER == 'A'
//    #define SPI_PORT              GPIOA
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOA
//#endif
//#if SPI_PORT_LETTER == 'B'
//    #define SPI_PORT              GPIOB
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOB
//#endif
//#if SPI_PORT_LETTER == 'C'
//    #define SPI_PORT              GPIOC
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOC
//#endif
//#if SPI_PORT_LETTER == 'D'
//    #define SPI_PORT              GPIOD
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOD
//#endif
//#if SPI_PORT_LETTER == 'E'
//    #define SPI_PORT              GPIOE
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOE
//#endif
//#if SPI_PORT_LETTER == 'F'
//    #define SPI_PORT              GPIOF
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOF
//#endif
//#if SPI_PORT_LETTER == 'G'
//    #define SPI_PORT              GPIOG
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOG
//#endif
//#if SPI_PORT_LETTER == 'H'
//    #define SPI_PORT              GPIOH
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOH
//#endif
//#if SPI_PORT_LETTER == 'I'
//    #define SPI_PORT              GPIOI
//    #define SPI_PORT_RCC          RCC_AHB1Periph_GPIOI
//#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/**=============================================================================
 * \fn      init_SPI
 * \brief   initializes the SPI peripheral
 *
 * \param[in]   None
 * \return  None
 =============================================================================*/
void init_SPI(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	/* enable clock for used IO pins */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* configure the SCK pin */
	GPIO_InitStruct.GPIO_Pin = SPI_PIN_SCK;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_PORT_SCK, &GPIO_InitStruct);

	/* Configure the MOSI pin */
	GPIO_InitStruct.GPIO_Pin = SPI_PIN_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_PORT_MOSI, &GPIO_InitStruct);

	/* connect SPI pins to SPI alternate function */
	GPIO_PinAFConfig(SPI_PORT_SCK, GPIO_PinSource_SCK, GPIO_AF);
	GPIO_PinAFConfig(SPI_PORT_MOSI, GPIO_PinSource_MOSI, GPIO_AF);

	/* enable peripheral clock for SPI configurations */
	RCC_APB2PeriphClockCmd(RCC_APBPeriph, ENABLE);

	/* SPI configurations */
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx; // set to simplex mode, (MOSI only)
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;      // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;  // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;         // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;       // data is sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = BaudRatePrescaler; // SPI frequency is APB2 frequency / x
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; // data is transmitted MSB first
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI, &SPI_InitStruct);

	SPI_CalculateCRC(SPI, ENABLE);                  // enable CRC calculation

	SPI_Cmd(SPI, ENABLE); // enable SPI
}


/**=============================================================================
 * \fn      SPI_send
 * \brief   used to transmit a data-byte via SPI
 *
 * \param[in]   data-byte to be transmitted
 * \return  None
 =============================================================================*/
void SPI_send_byte(uint8_t data)
{
	SPI_I2S_SendData(SPI,data);    // write data to be transmitted to the SPI data register
	while (SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_TXE) == RESET);  // wait until transmission complete
	while(SPI_I2S_GetFlagStatus(SPI, SPI_I2S_FLAG_BSY)); // wait until SPI is not busy anymore
}

/**
 * @}
 */
