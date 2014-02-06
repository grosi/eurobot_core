/**
 * \file    button.c
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "button.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/**
 * \fn      initButton_S1
 * \brief   button S1 initialisation
 */
void initButton_S1()
{
    /* variable for button init */
    GPIO_InitTypeDef button_gpio;

    /* initialize gpio */
    button_gpio.GPIO_Pin = BUTTON_S1_PIN;
    button_gpio.GPIO_Mode = BUTTON_S1_PIN_MODE;
    button_gpio.GPIO_OType = BUTTON_S1_PIN_TYPE;
    button_gpio.GPIO_PuPd = BUTTON_S1_PIN_PUPD;
    button_gpio.GPIO_Speed = BUTTON_S1_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(BUTTON_S1_PORT,&button_gpio);
}


/**
 * \fn      getButton_S1
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getButton_S1()
{
    return GPIO_ReadInputDataBit(BUTTON_S1_PORT, BUTTON_S1_PIN);
}


/**
 * \fn      initButton_S2
 * \brief   button S2 initialisation
 */
void initButton_S2()
{
    /* variable for button init */
    GPIO_InitTypeDef button_gpio;

    /* initialize gpio */
    button_gpio.GPIO_Pin = BUTTON_S2_PIN;
    button_gpio.GPIO_Mode = BUTTON_S2_PIN_MODE;
    button_gpio.GPIO_OType = BUTTON_S2_PIN_TYPE;
    button_gpio.GPIO_PuPd = BUTTON_S2_PIN_PUPD;
    button_gpio.GPIO_Speed = BUTTON_S2_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* enables port and pin */
    GPIO_Init(BUTTON_S2_PORT,&button_gpio);
}


/**
 * \fn      getButton_S2
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getButton_S2()
{
    return GPIO_ReadInputDataBit(BUTTON_S2_PORT, BUTTON_S2_PIN);
}


/**
 * \fn      initButton_S3
 * \brief   button S3 initialisation
 */
void initButton_S3()
{
    /* variable for button init */
    GPIO_InitTypeDef button_gpio;

    /* initialize gpio */
    button_gpio.GPIO_Pin = BUTTON_S3_PIN;
    button_gpio.GPIO_Mode = BUTTON_S3_PIN_MODE;
    button_gpio.GPIO_OType = BUTTON_S3_PIN_TYPE;
    button_gpio.GPIO_PuPd = BUTTON_S3_PIN_PUPD;
    button_gpio.GPIO_Speed = BUTTON_S3_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* enables port and pin */
    GPIO_Init(BUTTON_S3_PORT,&button_gpio);
}


/**
 * \fn      getButton_S3
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getButton_S3()
{
    return GPIO_ReadInputDataBit(BUTTON_S3_PORT, BUTTON_S3_PIN);
}


/**
 * \fn      initButton_S4
 * \brief   button S4 initialisation
 */
void initButton_S4()
{
    /* variable for button init */
    GPIO_InitTypeDef button_gpio;

    /* initialize gpio */
    button_gpio.GPIO_Pin = BUTTON_S4_PIN;
    button_gpio.GPIO_Mode = BUTTON_S4_PIN_MODE;
    button_gpio.GPIO_OType = BUTTON_S4_PIN_TYPE;
    button_gpio.GPIO_PuPd = BUTTON_S4_PIN_PUPD;
    button_gpio.GPIO_Speed = BUTTON_S4_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* enables port and pin */
    GPIO_Init(BUTTON_S4_PORT,&button_gpio);
}


/**
 * \fn      getButton_S4
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getButton_S4()
{
    return GPIO_ReadInputDataBit(BUTTON_S4_PORT, BUTTON_S4_PIN);
}


/**
 * @}
 */

