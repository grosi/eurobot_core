/**
 * \file    air.h
 * \author  gross10
 * \date    2014-05-08
 *
 * \version 1.0
 *  create this file
 *
 * \brief   air pump and switch access
 *
 * \defgroup air Air
 * \brief   air pump and switch for the big robo
 * \ingroup firmware
 *
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef AIR_H_
#define AIR_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
#define AIR_ON  1
#define AIR_OFF 0
/* Pin and Port roboboard*/
#define AIR_IN1_PIN       GPIO_Pin_15
#define AIR_IN1_PIN_MODE  GPIO_Mode_OUT
#define AIR_IN1_PIN_SPEED GPIO_Speed_100MHz
#define AIR_IN1_PIN_TYPE  GPIO_OType_PP
#define AIR_IN1_PIN_PUPD  GPIO_PuPd_NOPULL
#define AIR_IN1_PORT_CLK  RCC_AHB1Periph_GPIOB
#define AIR_IN1_PORT      GPIOB

#define AIR_IN2_PIN       GPIO_Pin_14
#define AIR_IN2_PIN_MODE  GPIO_Mode_OUT
#define AIR_IN2_PIN_SPEED GPIO_Speed_100MHz
#define AIR_IN2_PIN_TYPE  GPIO_OType_PP
#define AIR_IN2_PIN_PUPD  GPIO_PuPd_NOPULL
#define AIR_IN2_PORT_CLK  RCC_AHB1Periph_GPIOB
#define AIR_IN2_PORT      GPIOB


/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initAir();
extern void setAir(uint8_t);

#endif /* AIR_H_ */

/**
 * @}
 */
