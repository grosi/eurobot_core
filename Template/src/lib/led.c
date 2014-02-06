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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "led.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/**
 * \fn      initLED_1
 * \brief   led 1 initialisation
 */
void initLED_1()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = LED_1_PIN;
    led_gpio.GPIO_Mode = LED_1_PIN_MODE;
    led_gpio.GPIO_OType = LED_1_PIN_TYPE;
    led_gpio.GPIO_PuPd = LED_1_PIN_PUPD;
    led_gpio.GPIO_Speed = LED_1_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* enables port and pin */
    GPIO_Init(LED_1_PORT,&led_gpio);
}


/**
 * \fn      setLED_1
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_1(uint8_t value)
{
    GPIO_SetBits(LED_1_PORT,LED_1_PIN);
}


/**
 * \fn      initLED_2
 * \brief   led 2 initialisation
 */
void initLED_2()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = LED_2_PIN;
    led_gpio.GPIO_Mode = LED_2_PIN_MODE;
    led_gpio.GPIO_OType = LED_2_PIN_TYPE;
    led_gpio.GPIO_PuPd = LED_2_PIN_PUPD;
    led_gpio.GPIO_Speed = LED_2_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* enables port and pin */
    GPIO_Init(LED_2_PORT,&led_gpio);
}


/**
 * \fn      setLED_2
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_2(uint8_t value)
{
    GPIO_SetBits(LED_2_PORT,LED_2_PIN);
}


/**
 * \fn      initLED_3
 * \brief   led 3 initialisation
 */
void initLED_3()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = LED_3_PIN;
    led_gpio.GPIO_Mode = LED_3_PIN_MODE;
    led_gpio.GPIO_OType = LED_3_PIN_TYPE;
    led_gpio.GPIO_PuPd = LED_3_PIN_PUPD;
    led_gpio.GPIO_Speed = LED_3_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(LED_3_PORT,&led_gpio);
}


/**
 * \fn      setLED_3
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_3(uint8_t value)
{
    GPIO_SetBits(LED_3_PORT,LED_3_PIN);
}


/**
 * \fn      initLED_4
 * \brief   led 4 initialisation
 */
void initLED_4()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = LED_4_PIN;
    led_gpio.GPIO_Mode = LED_4_PIN_MODE;
    led_gpio.GPIO_OType = LED_4_PIN_TYPE;
    led_gpio.GPIO_PuPd = LED_4_PIN_PUPD;
    led_gpio.GPIO_Speed = LED_4_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(LED_4_PORT,&led_gpio);
}


/**
 * \fn      setLED_4
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_4(uint8_t value)
{
    GPIO_SetBits(LED_4_PORT,LED_4_PIN);
}


/**
 * \fn      initLED_5
 * \brief   led 5 initialisation
 */
void initLED_5()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = LED_5_PIN;
    led_gpio.GPIO_Mode = LED_5_PIN_MODE;
    led_gpio.GPIO_OType = LED_5_PIN_TYPE;
    led_gpio.GPIO_PuPd = LED_5_PIN_PUPD;
    led_gpio.GPIO_Speed = LED_5_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(LED_5_PORT,&led_gpio);
}


/**
 * \fn      setLED_5
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_5(uint8_t value)
{
    GPIO_SetBits(LED_5_PORT,LED_5_PIN);
}


/**
 * \fn      initLED_6
 * \brief   led 6 initialisation
 */
void initLED_6()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = LED_6_PIN;
    led_gpio.GPIO_Mode = LED_6_PIN_MODE;
    led_gpio.GPIO_OType = LED_6_PIN_TYPE;
    led_gpio.GPIO_PuPd = LED_6_PIN_PUPD;
    led_gpio.GPIO_Speed = LED_6_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(LED_6_PORT,&led_gpio);
}


/**
 * \fn      setLED_6
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_6(uint8_t value)
{
    GPIO_SetBits(LED_6_PORT,LED_6_PIN);
}


/**
 * \fn      initBoardLED_red
 * \brief   led red of the descovery initialisation
 */
void initBoardLED_red()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = BOARD_LED_RED_PIN;
    led_gpio.GPIO_Mode = BOARD_LED_RED_PIN_MODE;
    led_gpio.GPIO_OType = BOARD_LED_RED_PIN_TYPE;
    led_gpio.GPIO_PuPd = BOARD_LED_RED_PIN_PUPD;
    led_gpio.GPIO_Speed = BOARD_LED_RED_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(BOARD_LED_RED_PORT,&led_gpio);
}


/**
 * \fn      setBoardLED_red
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_red(uint8_t value)
{
    GPIO_SetBits(BOARD_LED_RED_PORT,BOARD_LED_RED_PIN);
}


/**
 * \fn      initBoardLED_orange
 * \brief   led orange of the descovery initialisation
 */
void initBoardLED_orange()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = BOARD_LED_ORANGE_PIN;
    led_gpio.GPIO_Mode = BOARD_LED_ORANGE_PIN_MODE;
    led_gpio.GPIO_OType = BOARD_LED_ORANGE_PIN_TYPE;
    led_gpio.GPIO_PuPd = BOARD_LED_ORANGE_PIN_PUPD;
    led_gpio.GPIO_Speed = BOARD_LED_ORANGE_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(BOARD_LED_ORANGE_PORT,&led_gpio);
}


/**
 * \fn      setBoardLED_orange
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_orange(uint8_t value)
{
    GPIO_SetBits(BOARD_LED_ORANGE_PORT,BOARD_LED_ORANGE_PIN);
}


/**
 * \fn      initBoardLED_blue
 * \brief   led blue of the descovery initialisation
 */
void initBoardLED_blue()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = BOARD_LED_BLUE_PIN;
    led_gpio.GPIO_Mode = BOARD_LED_BLUE_PIN_MODE;
    led_gpio.GPIO_OType = BOARD_LED_BLUE_PIN_TYPE;
    led_gpio.GPIO_PuPd = BOARD_LED_BLUE_PIN_PUPD;
    led_gpio.GPIO_Speed = BOARD_LED_BLUE_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(BOARD_LED_BLUE_PORT,&led_gpio);
}


/**
 * \fn      setBoardLED_blue
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_blue(uint8_t value)
{
    GPIO_SetBits(BOARD_LED_BLUE_PORT,BOARD_LED_BLUE_PIN);
}


/**
 * \fn      initBoardLED_green
 * \brief   led green of the descovery initialisation
 */
void initBoardLED_green()
{
    /* variable for button init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = BOARD_LED_GREEN_PIN;
    led_gpio.GPIO_Mode = BOARD_LED_GREEN_PIN_MODE;
    led_gpio.GPIO_OType = BOARD_LED_GREEN_PIN_TYPE;
    led_gpio.GPIO_PuPd = BOARD_LED_GREEN_PIN_PUPD;
    led_gpio.GPIO_Speed = BOARD_LED_GREEN_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(BOARD_LED_GREEN_PORT,&led_gpio);
}


/**
 * \fn      setBoardLED_green
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_green(uint8_t value)
{
    GPIO_SetBits(BOARD_LED_GREEN_PORT,BOARD_LED_GREEN_PIN);
}



/**
 * @}
 */
