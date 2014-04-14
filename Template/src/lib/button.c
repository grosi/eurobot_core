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
static void initButton(uint32_t, GPIOMode_TypeDef, GPIOOType_TypeDef,
        GPIOPuPd_TypeDef, GPIOSpeed_TypeDef, GPIO_TypeDef*, uint32_t);
static uint8_t getButtonPosEdge(GPIO_TypeDef*, uint16_t, uint8_t** );
static uint8_t getButtonNegEdge(GPIO_TypeDef*, uint16_t, uint8_t** );


/**
 * \fn      initButton_S1
 * \brief   button S1 initialisation
 */
void initButton_S1()
{
    initButton(BUTTON_S1_PIN, BUTTON_S1_PIN_MODE, BUTTON_S1_PIN_TYPE, BUTTON_S1_PIN_PUPD, BUTTON_S1_PIN_SPEED,
            BUTTON_S1_PORT, BUTTON_S1_PORT_CLK);
}


/**
 * \fn      getButton_S1
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getButton_S1()
{
    return !GPIO_ReadInputDataBit(BUTTON_S1_PORT, BUTTON_S1_PIN);
}


/**
 * \fn      getButtonPosEdge_S1
 * \brief   get the positive edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getButtonPosEdge_S1(uint8_t* button_state)
{
    return getButtonPosEdge(BUTTON_S1_PORT,BUTTON_S1_PIN, &button_state);
}


/**
 * \fn      getButtonNegEdge_S1
 * \brief   get the negative edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getButtonNegEdge_S1(uint8_t* button_state)
{
    return getButtonNegEdge(BUTTON_S1_PORT,BUTTON_S1_PIN, &button_state);
}


/**
 * \fn      initButton_S2
 * \brief   button S2 initialisation
 */
void initButton_S2()
{
    initButton(BUTTON_S2_PIN, BUTTON_S2_PIN_MODE, BUTTON_S2_PIN_TYPE, BUTTON_S2_PIN_PUPD, BUTTON_S2_PIN_SPEED,
                BUTTON_S2_PORT, BUTTON_S2_PORT_CLK);
}


/**
 * \fn      getButton_S2
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getButton_S2()
{
    return !GPIO_ReadInputDataBit(BUTTON_S2_PORT, BUTTON_S2_PIN);
}


/**
 * \fn      getButtonPosEdge_S2
 * \brief   get the positive edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getButtonPosEdge_S2(uint8_t* button_state)
{
    return getButtonPosEdge(BUTTON_S2_PORT,BUTTON_S2_PIN, &button_state);
}


/**
 * \fn      getButtonNegEdge_S2
 * \brief   get the negative edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getButtonNegEdge_S2(uint8_t* button_state)
{
    return getButtonNegEdge(BUTTON_S2_PORT,BUTTON_S2_PIN, &button_state);
}


/**
 * \fn      initButton_S3
 * \brief   button S3 initialisation
 */
void initButton_S3()
{
    initButton(BUTTON_S3_PIN, BUTTON_S3_PIN_MODE, BUTTON_S3_PIN_TYPE, BUTTON_S3_PIN_PUPD, BUTTON_S3_PIN_SPEED,
                BUTTON_S3_PORT, BUTTON_S3_PORT_CLK);
}


/**
 * \fn      getButton_S3
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getButton_S3()
{
    return !GPIO_ReadInputDataBit(BUTTON_S3_PORT, BUTTON_S3_PIN);
}


/**
 * \fn      getButtonPosEdge_S3
 * \brief   get the positive edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getButtonPosEdge_S3(uint8_t* button_state)
{
    return getButtonPosEdge(BUTTON_S3_PORT,BUTTON_S3_PIN, &button_state);
}


/**
 * \fn      getButtonNegEdge_S3
 * \brief   get the negative edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getButtonNegEdge_S3(uint8_t* button_state)
{
    return getButtonNegEdge(BUTTON_S3_PORT,BUTTON_S3_PIN, &button_state);
}

/**
 * \fn      initButton_S4
 * \brief   button S4 initialisation
 */
void initButton_S4()
{
    initButton(BUTTON_S4_PIN, BUTTON_S4_PIN_MODE, BUTTON_S4_PIN_TYPE, BUTTON_S4_PIN_PUPD, BUTTON_S4_PIN_SPEED,
                BUTTON_S4_PORT, BUTTON_S4_PORT_CLK);
}


/**
 * \fn      getButton_S4
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getButton_S4()
{
    return !GPIO_ReadInputDataBit(BUTTON_S4_PORT, BUTTON_S4_PIN);
}


/**
 * \fn      getButtonPosEdge_S4
 * \brief   get the positive edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getButtonPosEdge_S4(uint8_t* button_state)
{
    return getButtonPosEdge(BUTTON_S4_PORT,BUTTON_S4_PIN, &button_state);
}


/**
 * \fn      getButtonNegEdge_S4
 * \brief   get the negative edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getButtonNegEdge_S4(uint8_t* button_state)
{
    return getButtonNegEdge(BUTTON_S4_PORT,BUTTON_S4_PIN, &button_state);
}

/**
 * \fn      initBoardButton_Blue
 * \brief   button blue initialisation
 */
void initBoardButton_Blue()
{
    initButton(BOARD_BUTTON_BLUE_PIN, BOARD_BUTTON_BLUE_PIN_MODE, BOARD_BUTTON_BLUE_PIN_TYPE, BOARD_BUTTON_BLUE_PIN_PUPD, BOARD_BUTTON_BLUE_PIN_SPEED,
            BOARD_BUTTON_BLUE_PORT, BOARD_BUTTON_BLUE_PORT_CLK);
}


/**
 * \fn      getBoardButton_Blue
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getBoardButton_Blue()
{
    return GPIO_ReadInputDataBit(BOARD_BUTTON_BLUE_PORT, BOARD_BUTTON_BLUE_PIN);
}


/**
 * \fn      getBoardButtonPosEdge_Blue
 * \brief   get the positive edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getBoardButtonPosEdge_Blue(uint8_t* button_state)
{
    return getButtonPosEdge(BOARD_BUTTON_BLUE_PORT, BOARD_BUTTON_BLUE_PIN, &button_state);
}


/**
 * \fn      getBoardButtonNegEdge_Blue
 * \brief   get the negative edge from the blue descovery button
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the negative edge, if there
 */
uint8_t getBoardButtonNegEdge_Blue(uint8_t* button_state)
{
    return getButtonNegEdge(BOARD_BUTTON_BLUE_PORT, BOARD_BUTTON_BLUE_PIN, &button_state);
}






/**
 * \fn      initUserPanelButton_1
 * \brief   user panel button 1 initialization
 */
void initUserPanelButton_1()
{
    initButton(USERPANEL_BUTTON_1_PIN, USERPANEL_BUTTON_1_PIN_MODE, USERPANEL_BUTTON_1_PIN_TYPE, USERPANEL_BUTTON_1_PIN_PUPD, USERPANEL_BUTTON_1_PIN_SPEED,
    		USERPANEL_BUTTON_1_PORT, USERPANEL_BUTTON_1_PORT_CLK);
}

/**
 * \fn      getUserPanelButton_1
 * \brief   get the current pin value
 * \note    the pin is not debounced, low active
 * \return  current button value
 */
uint8_t getUserPanelButton_1()
{
    return !GPIO_ReadInputDataBit(USERPANEL_BUTTON_1_PORT, USERPANEL_BUTTON_1_PIN);
}

/**
 * \fn      getUserPanelButtonPosEdge_1
 * \brief   get the positive edge from an user panel button
 * \note    this function have to call every program loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getUserPanelButtonPosEdge_1(uint8_t* button_state)
{
    return getButtonPosEdge(USERPANEL_BUTTON_1_PORT, USERPANEL_BUTTON_1_PIN, &button_state);
}

/**
 * \fn      getUserPanelButtonNegEdge_1
 * \brief   get the negative edge from an user panel button
 * \note    this function have to call every program loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the negative edge, if there
 */
uint8_t getUserPanelButtonNegEdge_1(uint8_t* button_state)
{
    return getButtonNegEdge(USERPANEL_BUTTON_1_PORT, USERPANEL_BUTTON_1_PIN, &button_state);
}

/**
 * \fn      initUserPanelButton_2
 * \brief   user panel button 2 initialization
 */
void initUserPanelButton_2()
{
    initButton(USERPANEL_BUTTON_2_PIN, USERPANEL_BUTTON_2_PIN_MODE, USERPANEL_BUTTON_2_PIN_TYPE, USERPANEL_BUTTON_2_PIN_PUPD, USERPANEL_BUTTON_2_PIN_SPEED,
    		USERPANEL_BUTTON_2_PORT, USERPANEL_BUTTON_2_PORT_CLK);
}

/**
 * \fn      getUserPanelButton_2
 * \brief   get the current pin value
 * \note    the pin is not debounced, low active
 * \return  current button value
 */
uint8_t getUserPanelButton_2()
{
    return !GPIO_ReadInputDataBit(USERPANEL_BUTTON_2_PORT, USERPANEL_BUTTON_2_PIN);
}

/**
 * \fn      getUserPanelButtonPosEdge_2
 * \brief   get the positive edge from an user panel button
 * \note    this function have to call every program loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getUserPanelButtonPosEdge_2(uint8_t* button_state)
{
    return getButtonPosEdge(USERPANEL_BUTTON_2_PORT, USERPANEL_BUTTON_2_PIN, &button_state);
}

/**
 * \fn      getUserPanelButtonNegEdge_2
 * \brief   get the negative edge from an user panel button
 * \note    this function have to call every program loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the negative edge, if there
 */
uint8_t getUserPanelButtonNegEdge_2(uint8_t* button_state)
{
    return getButtonNegEdge(USERPANEL_BUTTON_2_PORT, USERPANEL_BUTTON_2_PIN, &button_state);
}

/**
 * \fn      initUserPanelButton_3
 * \brief   user panel button 3 initialization
 */
void initUserPanelButton_3()
{
    initButton(USERPANEL_BUTTON_3_PIN, USERPANEL_BUTTON_3_PIN_MODE, USERPANEL_BUTTON_3_PIN_TYPE, USERPANEL_BUTTON_3_PIN_PUPD, USERPANEL_BUTTON_3_PIN_SPEED,
    		USERPANEL_BUTTON_3_PORT, USERPANEL_BUTTON_3_PORT_CLK);
}

/**
 * \fn      getUserPanelButton_3
 * \brief   get the current pin value
 * \note    the pin is not debounced, low active
 * \return  current button value
 */
uint8_t getUserPanelButton_3()
{
    return !GPIO_ReadInputDataBit(USERPANEL_BUTTON_3_PORT, USERPANEL_BUTTON_3_PIN);
}

/**
 * \fn      getUserPanelButtonPosEdge_3
 * \brief   get the positive edge from an user panel button
 * \note    this function have to call every program loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getUserPanelButtonPosEdge_3(uint8_t* button_state)
{
    return getButtonPosEdge(USERPANEL_BUTTON_3_PORT, USERPANEL_BUTTON_3_PIN, &button_state);
}

/**
 * \fn      getUserPanelButtonNegEdge_3
 * \brief   get the negative edge from an user panel button
 * \note    this function have to call every program loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the negative edge, if there
 */
uint8_t getUserPanelButtonNegEdge_3(uint8_t* button_state)
{
    return getButtonNegEdge(USERPANEL_BUTTON_3_PORT, USERPANEL_BUTTON_3_PIN, &button_state);
}


/**
 * \fn      initUserPanelButtons
 * \brief   init user-panel buttons
 */
void initUserPanelButtons()
{
	initUserPanelButton_1();
	initUserPanelButton_2();
	initUserPanelButton_3();
}

/**
 * \fn      initButton
 * \brief   initialisation a button
 *
 * \param   pin         pin number
 * \param   mode        pin mode -> output
 * \param   type        pin type -> pushpull
 * \param   pupd        pin pullup/pulldown
 * \param   speed       pin speed
 * \param   port        port letter
 * \param   port_clk    port clock source
 */
static void initButton(uint32_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type,
        GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed, GPIO_TypeDef* port, uint32_t port_clk)
{
    /* variable for button init */
    GPIO_InitTypeDef button_gpio;

    /* initialize gpio */
    button_gpio.GPIO_Pin = pin;
    button_gpio.GPIO_Mode = mode;
    button_gpio.GPIO_OType = type;
    button_gpio.GPIO_PuPd = pupd;
    button_gpio.GPIO_Speed = speed;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(port_clk, ENABLE);

    /* enables port and pin */
    GPIO_Init(port,&button_gpio);
}


/**
 * \fn      getButtonPosEdge
 * \brief   calculate the positive edge based on the last button state
 *
 * \param   port    pin port
 * \param   pin     pin number
 * \param   pointer to a pointer with the last button state
 * \return  the positive edge, if there
 */
static uint8_t getButtonPosEdge(GPIO_TypeDef* port, uint16_t pin, uint8_t** button_state)
{
    uint8_t last_button_state;

    last_button_state = **button_state;
    **button_state = !GPIO_ReadInputDataBit(port, pin);

    return (**button_state ^ last_button_state) & **button_state;
}


/**
 * \fn      getButtonNegEdge
 * \brief   calculate the negative edge based on the last button state
 *
 * \param   port    pin port
 * \param   pin     pin number
 * \param   pointer to a pointer with the last button state
 * \return  the neagtive edge, if there
 */
static uint8_t getButtonNegEdge(GPIO_TypeDef* port, uint16_t pin, uint8_t** button_state)
{
    uint8_t last_button_state;

    last_button_state = **button_state;
    **button_state = !GPIO_ReadInputDataBit(port, pin);

    return (**button_state ^ last_button_state) & last_button_state;
}


/**
 * @}
 */
