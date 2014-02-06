/**
  ******************************************************************************
  * @file    spi.h
  * @author  Schüpbach Simon
	* @date    27.05.2013
	*
	* @version 1.0	
	*	create this file
	*
  * @brief   this file include functions for the spi handling
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/

	 
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* use SPI1 */
#define SPI                           SPI1
																				
#define SPI_SCK_PIN                   GPIO_Pin_5									/* PA.5 */		
#define SPI_SCK_PORT             			GPIOA									

#define SPI_MISO_PIN                  GPIO_Pin_6									/* PA.6 */
#define SPI_MISO_PORT            			GPIOA									

#define SPI_MOSI_PIN                  GPIO_Pin_7									/* PA.7 */
#define SPI_MOSI_PORT            			GPIOA									


/* Exported macro ------------------------------------------------------------*/

typedef enum {
	MOTION_SENSOR = 1
	/* add new peripherie */
}PeriphSPI;

/* Exported functions --------------------------------------------------------*/  

void initSPI( void );
void readSPI(PeriphSPI Peripherie, uint8_t ReadAddr, uint16_t NumByteToRead, uint8_t* Data);
void writeSPI(PeriphSPI Peripherie, uint8_t WriteAddr, uint16_t NumByteToWrite, uint8_t* Data);

/* initialize CS and config the motion sensor */
void initMotionSensor(void);

/* get the x, y and z data of the motion sensor */
void getXYZDataMotionSensor(uint8_t *Data);
#endif /* __SPI_H */
