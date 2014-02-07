/**
 * \file    led.h
 * \author  gross10
 * \date    2014-02-06
 *
 * \version 1.0
 *  create this file
 *
 * \brief   simple roboboard LED access
 *
 * \defgroup button Button
 * \brief   roboboard led library
 * \ingroup firmware
 *
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LED_H_
#define LED_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
/* Pin and Port  roboboard*/
#define LED_1_PIN       GPIO_Pin_8
#define LED_1_PIN_MODE  GPIO_Mode_OUT
#define LED_1_PIN_SPEED GPIO_Speed_2MHz
#define LED_1_PIN_TYPE  GPIO_OType_PP
#define LED_1_PIN_PUPD  GPIO_PuPd_UP
#define LED_1_PORT_CLK  RCC_AHB1Periph_GPIOA
#define LED_1_PORT      GPIOA

#define LED_2_PIN       GPIO_Pin_9
#define LED_2_PIN_MODE  GPIO_Mode_OUT
#define LED_2_PIN_SPEED GPIO_Speed_2MHz
#define LED_2_PIN_TYPE  GPIO_OType_PP
#define LED_2_PIN_PUPD  GPIO_PuPd_UP
#define LED_2_PORT_CLK  RCC_AHB1Periph_GPIOC
#define LED_2_PORT      GPIOC

#define LED_3_PIN       GPIO_Pin_8
#define LED_3_PIN_MODE  GPIO_Mode_OUT
#define LED_3_PIN_SPEED GPIO_Speed_2MHz
#define LED_3_PIN_TYPE  GPIO_OType_PP
#define LED_3_PIN_PUPD  GPIO_PuPd_UP
#define LED_3_PORT_CLK  RCC_AHB1Periph_GPIOD
#define LED_3_PORT      GPIOD

#define LED_4_PIN       GPIO_Pin_10
#define LED_4_PIN_MODE  GPIO_Mode_OUT
#define LED_4_PIN_SPEED GPIO_Speed_2MHz
#define LED_4_PIN_TYPE  GPIO_OType_PP
#define LED_4_PIN_PUPD  GPIO_PuPd_UP
#define LED_4_PORT_CLK  RCC_AHB1Periph_GPIOD
#define LED_4_PORT      GPIOD

#define LED_5_PIN       GPIO_Pin_9
#define LED_5_PIN_MODE  GPIO_Mode_OUT
#define LED_5_PIN_SPEED GPIO_Speed_2MHz
#define LED_5_PIN_TYPE  GPIO_OType_PP
#define LED_5_PIN_PUPD  GPIO_PuPd_UP
#define LED_5_PORT_CLK  RCC_AHB1Periph_GPIOD
#define LED_5_PORT      GPIOD

#define LED_6_PIN       GPIO_Pin_11
#define LED_6_PIN_MODE  GPIO_Mode_OUT
#define LED_6_PIN_SPEED GPIO_Speed_2MHz
#define LED_6_PIN_TYPE  GPIO_OType_PP
#define LED_6_PIN_PUPD  GPIO_PuPd_UP
#define LED_6_PORT_CLK  RCC_AHB1Periph_GPIOD
#define LED_6_PORT      GPIOD

#define LED_COLOR_PIN       GPIO_Pin_13
#define LED_COLOR_PIN_MODE  GPIO_Mode_OUT
#define LED_COLOR_PIN_SPEED GPIO_Speed_2MHz
#define LED_COLOR_PIN_TYPE  GPIO_OType_PP
#define LED_COLOR_PIN_PUPD  GPIO_PuPd_UP
#define LED_COLOR_PORT_CLK  RCC_AHB1Periph_GPIOB
#define LED_COLOR_PORT      GPIOB

/* Pin and Port descovery*/
#define BOARD_LED_RED_PIN           GPIO_Pin_14
#define BOARD_LED_RED_PIN_MODE      GPIO_Mode_OUT
#define BOARD_LED_RED_PIN_SPEED     GPIO_Speed_2MHz
#define BOARD_LED_RED_PIN_TYPE      GPIO_OType_PP
#define BOARD_LED_RED_PIN_PUPD      GPIO_PuPd_UP
#define BOARD_LED_RED_PORT_CLK      RCC_AHB1Periph_GPIOD
#define BOARD_LED_RED_PORT          GPIOD

#define BOARD_LED_ORANGE_PIN        GPIO_Pin_13
#define BOARD_LED_ORANGE_PIN_MODE   GPIO_Mode_OUT
#define BOARD_LED_ORANGE_PIN_SPEED  GPIO_Speed_2MHz
#define BOARD_LED_ORANGE_PIN_TYPE   GPIO_OType_PP
#define BOARD_LED_ORANGE_PIN_PUPD   GPIO_PuPd_UP
#define BOARD_LED_ORANGE_PORT_CLK   RCC_AHB1Periph_GPIOD
#define BOARD_LED_ORANGE_PORT       GPIOD

#define BOARD_LED_BLUE_PIN          GPIO_Pin_15
#define BOARD_LED_BLUE_PIN_MODE     GPIO_Mode_OUT
#define BOARD_LED_BLUE_PIN_SPEED    GPIO_Speed_2MHz
#define BOARD_LED_BLUE_PIN_TYPE     GPIO_OType_PP
#define BOARD_LED_BLUE_PIN_PUPD     GPIO_PuPd_UP
#define BOARD_LED_BLUE_PORT_CLK     RCC_AHB1Periph_GPIOD
#define BOARD_LED_BLUE_PORT         GPIOD

#define BOARD_LED_GREEN_PIN         GPIO_Pin_12
#define BOARD_LED_GREEN_PIN_MODE    GPIO_Mode_OUT
#define BOARD_LED_GREEN_PIN_SPEED   GPIO_Speed_2MHz
#define BOARD_LED_GREEN_PIN_TYPE    GPIO_OType_PP
#define BOARD_LED_GREEN_PIN_PUPD    GPIO_PuPd_UP
#define BOARD_LED_GREEN_PORT_CLK    RCC_AHB1Periph_GPIOD
#define BOARD_LED_GREEN_PORT        GPIOD

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
inline void initLED_1();
inline void setLED_1(uint8_t);
inline void initLED_2();
inline void setLED_2(uint8_t);
inline void initLED_3();
inline void setLED_3(uint8_t);
inline void initLED_4();
inline void setLED_4(uint8_t);
inline void initLED_5();
inline void setLED_5(uint8_t);
inline void initLED_6();
inline void setLED_6(uint8_t);
inline void initLED_Color();
inline void setLED_Color(uint8_t);


inline void initBoardLED_red();
inline void setBoardLED_red(uint8_t);
inline void initBoardLED_orange();
inline void setBoardLED_orange(uint8_t);
inline void initBoardLED_blue();
inline void setBoardLED_blue(uint8_t);
inline void initBoardLED_green();
inline void setBoardLED_green(uint8_t);


#endif /* LED_H_ */

/**
 * @}
 */
