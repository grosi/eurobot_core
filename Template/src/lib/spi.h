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
/* exported types ------------------------------------------------------------*/
/* exported constants --------------------------------------------------------*/
/* exported macro ------------------------------------------------------------*/

/* exported functions --------------------------------------------------------*/
extern void initSPI1();
extern void SPI1_send(uint8_t);

#endif

/**
 * @}
 */
