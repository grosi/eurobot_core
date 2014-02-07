/**
 * \file    button.h
 * \author  gross10
 * \date    2014-02-06
 *
 * \version 1.0
 *  create this file
 *
 * \brief   simple roboboard button access
 *
 * \defgroup button Button
 * \brief   roboboard button library
 * \ingroup firmware
 *
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BUTTON_H_
#define BUTTON_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
/* Pin and Port roboboard*/
#define BUTTON_S1_PIN       GPIO_Pin_3
#define BUTTON_S1_PIN_MODE  GPIO_Mode_IN
#define BUTTON_S1_PIN_SPEED GPIO_Speed_2MHz
#define BUTTON_S1_PIN_TYPE  GPIO_OType_PP
#define BUTTON_S1_PIN_PUPD  GPIO_PuPd_UP
#define BUTTON_S1_PORT_CLK  RCC_AHB1Periph_GPIOD
#define BUTTON_S1_PORT      GPIOD

#define BUTTON_S2_PIN       GPIO_Pin_2
#define BUTTON_S2_PIN_MODE  GPIO_Mode_IN
#define BUTTON_S2_PIN_SPEED GPIO_Speed_2MHz
#define BUTTON_S2_PIN_TYPE  GPIO_OType_PP
#define BUTTON_S2_PIN_PUPD  GPIO_PuPd_UP
#define BUTTON_S2_PORT_CLK  RCC_AHB1Periph_GPIOD
#define BUTTON_S2_PORT      GPIOD

#define BUTTON_S3_PIN       GPIO_Pin_11
#define BUTTON_S3_PIN_MODE  GPIO_Mode_IN
#define BUTTON_S3_PIN_SPEED GPIO_Speed_2MHz
#define BUTTON_S3_PIN_TYPE  GPIO_OType_PP
#define BUTTON_S3_PIN_PUPD  GPIO_PuPd_UP
#define BUTTON_S3_PORT_CLK  RCC_AHB1Periph_GPIOC
#define BUTTON_S3_PORT      GPIOC

#define BUTTON_S4_PIN       GPIO_Pin_15
#define BUTTON_S4_PIN_MODE  GPIO_Mode_IN
#define BUTTON_S4_PIN_SPEED GPIO_Speed_2MHz
#define BUTTON_S4_PIN_TYPE  GPIO_OType_PP
#define BUTTON_S4_PIN_PUPD  GPIO_PuPd_UP
#define BUTTON_S4_PORT_CLK  RCC_AHB1Periph_GPIOA
#define BUTTON_S4_PORT      GPIOA

/* Pin and Port descovery*/
#define BOARD_BUTTON_BLUE_PIN       GPIO_Pin_0
#define BOARD_BUTTON_BLUE_PIN_MODE  GPIO_Mode_IN
#define BOARD_BUTTON_BLUE_PIN_SPEED GPIO_Speed_2MHz
#define BOARD_BUTTON_BLUE_PIN_TYPE  GPIO_OType_PP
#define BOARD_BUTTON_BLUE_PIN_PUPD  GPIO_PuPd_DOWN
#define BOARD_BUTTON_BLUE_PORT_CLK  RCC_AHB1Periph_GPIOA
#define BOARD_BUTTON_BLUE_PORT      GPIOA

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
inline void initButton_S1();
inline uint8_t getButton_S1();
inline uint8_t getButtonPosEdge_S1(uint8_t*);
inline uint8_t getButtonNegEdge_S1(uint8_t*);
inline void initButton_S2();
inline uint8_t getButton_S2();
inline uint8_t getButtonPosEdge_S2(uint8_t*);
inline uint8_t getButtonNegEdge_S2(uint8_t*);
inline void initButton_S3();
inline uint8_t getButton_S3();
inline uint8_t getButtonPosEdge_S3(uint8_t*);
inline uint8_t getButtonNegEdge_S3(uint8_t*);
inline void initButton_S4();
inline uint8_t getButton_S4();
inline uint8_t getButtonPosEdge_S4(uint8_t*);
inline uint8_t getButtonNegEdge_S4(uint8_t*);

inline void initBoardButton_Blue();
inline uint8_t getBoardButton_Blue();
inline uint8_t getBoardButtonPosEdge_Blue(uint8_t*);
inline uint8_t getBoardButtonNegEdge_Blue(uint8_t*);


#endif /* BUTTON_H_ */

/**
 * @}
 */
