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
void setLED(uint16_t, GPIO_TypeDef*, uint8_t);
void initLED(uint32_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type,
        GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed, GPIO_TypeDef* port, uint32_t port_clk);


/**
 * \fn      initLED_1
 * \brief   led 1 initialisation
 */
inline void initLED_1()
{
    initLED(LED_1_PIN, LED_1_PIN_MODE, LED_1_PIN_TYPE, LED_1_PIN_PUPD, LED_1_PIN_SPEED,
            LED_1_PORT, LED_1_PORT_CLK);
}


/**
 * \fn      setLED_1
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_1(uint8_t value)
{
    setLED(LED_1_PIN, LED_1_PORT, value);
}


/**
 * \fn      initLED_2
 * \brief   led 2 initialisation
 */
inline void initLED_2()
{
    initLED(LED_2_PIN, LED_2_PIN_MODE, LED_2_PIN_TYPE, LED_2_PIN_PUPD, LED_2_PIN_SPEED,
                LED_2_PORT, LED_2_PORT_CLK);
}


/**
 * \fn      setLED_2
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_2(uint8_t value)
{
    setLED(LED_2_PIN, LED_2_PORT, value);
}


/**
 * \fn      initLED_3
 * \brief   led 3 initialisation
 */
inline void initLED_3()
{
    initLED(LED_3_PIN, LED_3_PIN_MODE, LED_3_PIN_TYPE, LED_3_PIN_PUPD, LED_3_PIN_SPEED,
                LED_3_PORT, LED_3_PORT_CLK);
}


/**
 * \fn      setLED_3
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_3(uint8_t value)
{
    setLED(LED_3_PIN, LED_3_PORT, value);
}


/**
 * \fn      initLED_4
 * \brief   led 4 initialisation
 */
inline void initLED_4()
{
    initLED(LED_4_PIN, LED_4_PIN_MODE, LED_4_PIN_TYPE, LED_4_PIN_PUPD, LED_4_PIN_SPEED,
                LED_4_PORT, LED_4_PORT_CLK);
}


/**
 * \fn      setLED_4
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_4(uint8_t value)
{
    setLED(LED_4_PIN, LED_4_PORT, value);
}


/**
 * \fn      initLED_5
 * \brief   led 5 initialisation
 */
inline void initLED_5()
{
    initLED(LED_5_PIN, LED_5_PIN_MODE, LED_5_PIN_TYPE, LED_5_PIN_PUPD, LED_5_PIN_SPEED,
                LED_5_PORT, LED_5_PORT_CLK);
}


/**
 * \fn      setLED_5
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_5(uint8_t value)
{
    setLED(LED_5_PIN, LED_5_PORT, value);
}


/**
 * \fn      initLED_6
 * \brief   led 6 initialisation
 */
inline void initLED_6()
{
    initLED(LED_6_PIN, LED_6_PIN_MODE, LED_6_PIN_TYPE, LED_6_PIN_PUPD, LED_6_PIN_SPEED,
                LED_6_PORT, LED_6_PORT_CLK);
}


/**
 * \fn      setLED_6
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_6(uint8_t value)
{
    setLED(LED_6_PIN, LED_6_PORT, value);
}


/**
 * \fn      initLED_all
 * \brief   all leds initialisation
 */
inline void initLED_all()
{
	initLED_1();
	initLED_2();
	initLED_3();
	initLED_4();
	initLED_5();
	initLED_6();
}


/**
 * \fn      setLEDs_bin
 * \brief   set the leds to binary value
 * \param   value   0x00 - 0x3F (6 bit); 1=LED on; 0=LED off
 */
void setLEDs_bin(uint8_t value)
{
	setLED_1(0x1 & (value));
	setLED_2(0x1 & (value>>1));
	setLED_3(0x1 & (value>>2));
	setLED_4(0x1 & (value>>3));
	setLED_5(0x1 & (value>>4));
	setLED_6(0x1 & (value>>5));
}


/**
 * \fn      initLED_Color
 * \brief   led color initialisation
 */
inline void initLED_Color()
{
    initLED(LED_COLOR_PIN, LED_COLOR_PIN_MODE, LED_COLOR_PIN_TYPE, LED_COLOR_PIN_PUPD, LED_COLOR_PIN_SPEED,
            LED_COLOR_PORT, LED_COLOR_PORT_CLK);
}


/**
 * \fn      setLED_Color
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setLED_Color(uint8_t value)
{
    setLED(LED_COLOR_PIN, LED_COLOR_PORT, value);
}


/**
 * \fn      initBoardLED_red
 * \brief   led red of the descovery initialisation
 */
inline void initBoardLED_red()
{
    initLED(BOARD_LED_RED_PIN, BOARD_LED_RED_PIN_MODE, BOARD_LED_RED_PIN_TYPE, BOARD_LED_RED_PIN_PUPD, BOARD_LED_RED_PIN_SPEED,
                    BOARD_LED_RED_PORT, BOARD_LED_RED_PORT_CLK);
}


/**
 * \fn      setBoardLED_red
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_red(uint8_t value)
{
    setLED(BOARD_LED_RED_PIN, BOARD_LED_RED_PORT, value);
}


/**
 * \fn      initBoardLED_orange
 * \brief   led orange of the descovery initialisation
 */
inline void initBoardLED_orange()
{
    initLED(BOARD_LED_ORANGE_PIN, BOARD_LED_ORANGE_PIN_MODE, BOARD_LED_ORANGE_PIN_TYPE, BOARD_LED_ORANGE_PIN_PUPD, BOARD_LED_ORANGE_PIN_SPEED,
                BOARD_LED_ORANGE_PORT, BOARD_LED_ORANGE_PORT_CLK);
}


/**
 * \fn      setBoardLED_orange
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_orange(uint8_t value)
{
    setLED(BOARD_LED_ORANGE_PIN, BOARD_LED_ORANGE_PORT, value);
}


/**
 * \fn      initBoardLED_blue
 * \brief   led blue of the descovery initialisation
 */
inline void initBoardLED_blue()
{

    initLED(BOARD_LED_BLUE_PIN, BOARD_LED_BLUE_PIN_MODE, BOARD_LED_BLUE_PIN_TYPE, BOARD_LED_BLUE_PIN_PUPD, BOARD_LED_BLUE_PIN_SPEED,
            BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PORT_CLK);
}


/**
 * \fn      setBoardLED_blue
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_blue(uint8_t value)
{
    setLED(BOARD_LED_BLUE_PIN, BOARD_LED_BLUE_PORT, value);

}


/**
 * \fn      initBoardLED_green
 * \brief   led green of the descovery initialisation
 */
inline void initBoardLED_green()
{
    initLED(BOARD_LED_GREEN_PIN, BOARD_LED_GREEN_PIN_MODE, BOARD_LED_GREEN_PIN_TYPE, BOARD_LED_GREEN_PIN_PUPD, BOARD_LED_GREEN_PIN_SPEED,
                BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PORT_CLK);
}


/**
 * \fn      setBoardLED_green
 * \brief   set the current pin value
 * \param   value   1=LED on; 0=LED off
 */
inline void setBoardLED_green(uint8_t value)
{
    setLED(BOARD_LED_GREEN_PIN, BOARD_LED_GREEN_PORT, value);
}

/**
 * \fn      initLED
 * \brief   initialisation a led
 *
 * \param   pin         pin number
 * \param   mode        pin mode -> output
 * \param   type        pin type -> pushpull
 * \param   pupd        pin pullup/pulldown
 * \param   speed       pin speed
 * \param   port        port letter
 * \param   port_clk    port clock source
 */
void initLED(uint32_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type,
        GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed, GPIO_TypeDef* port, uint32_t port_clk)
{
    /* variable for led init */
    GPIO_InitTypeDef led_gpio;

    /* initialize gpio */
    led_gpio.GPIO_Pin = pin;
    led_gpio.GPIO_Mode = mode;
    led_gpio.GPIO_OType = type;
    led_gpio.GPIO_PuPd = pupd;
    led_gpio.GPIO_Speed = speed;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(port_clk, ENABLE);

    /* enables port and pin */
    GPIO_Init(port,&led_gpio);
}


/**
 * \fn      setLED
 * \brief   set a LED
 *
 * \param   pin     pin number
 * \param   port    port letter
 * \param   value   1=led ON; 0=led OFF
 */
void setLED(uint16_t pin, GPIO_TypeDef* port, uint8_t value)
{
    if(value)
    {
        GPIO_SetBits(port,pin);
    }
    else
    {
        GPIO_ResetBits(port,pin);
    }
}

/**
 * @}
 */
