/**
 * \file    usart.c
 * \author  gross10
 * \date    2013-11-23
 *
 * \version 1.0
 *  create this file
 *
 * \brief   a simple usart library
 *
 * \todo    USARTx_IRQHandler have to implemented
 *
 * \defgroup usart USART
 * \brief   USART library
 * \ingroup firmware
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* additional configurations DON'T EDIT!*/
/* interface */
#if USART_INTERFACE_NUMBER == 1
    #define USART_INTERFACE         USART1
    #define USART_PIN_AF            GPIO_AF_USART1
    #define USART_NVIC_CHANNEL      USART1_IRQn
    #define USART_PIN_RCC           RCC_APB2Periph_USART1
#endif
#if USART_INTERFACE_NUMBER == 2
    #define USART_INTERFACE         USART2
    #define USART_PIN_AF            GPIO_AF_USART2
    #define USART_NVIC_CHANNEL      USART2_IRQn
    #define USART_PIN_RCC           RCC_APB1Periph_USART2
#endif
#if USART_INTERFACE_NUMBER == 3
    #define USART_INTERFACE         USART3
    #define USART_PIN_AF            GPIO_AF_USART3
    #define USART_NVIC_CHANNEL      USART3_IRQn
    #define USART_PIN_RCC           RCC_APB1Periph_USART3
#endif
#if USART_INTERFACE_NUMBER == 4
    #define USART_INTERFACE         USART4
    #define USART_PIN_AF            GPIO_AF_USART4
    #define USART_NVIC_CHANNEL      USART4_IRQn
    #define USART_PIN_RCC           RCC_APB1Periph_USART4
#endif
#if USART_INTERFACE_NUMBER == 5
    #define USART_INTERFACE         USART5
    #define USART_PIN_AF            GPIO_AF_USART5
    #define USART_NVIC_CHANNEL      USART5_IRQn
    #define USART_PIN_RCC           RCC_APB1Periph_USART5
#endif
#if USART_INTERFACE_NUMBER == 6
    #define USART_INTERFACE         USART6
    #define USART_PIN_AF            GPIO_AF_USART6
    #define USART_NVIC_CHANNEL      USART6_IRQn
    #define USART_PIN_RCC           RCC_APB2Periph_USART6
#endif
/* TX */
#if USART_PIN_TX_NUMBER == 5
    #define USART_PIN_TX            GPIO_Pin_5
    #define USART_PIN_TX_SOURCE     GPIO_PinSource5
#endif
#if USART_PIN_TX_NUMBER == 6
    #define USART_PIN_TX            GPIO_Pin_6
    #define USART_PIN_TX_SOURCE     GPIO_PinSource6
#endif
#if USART_PIN_TX_NUMBER == 8
    #define USART_PIN_TX            GPIO_Pin_8
    #define USART_PIN_TX_SOURCE     GPIO_PinSource8
#endif
#if USART_PIN_TX_NUMBER == 9
    #define USART_PIN_TX            GPIO_Pin_9
    #define USART_PIN_TX_SOURCE     GPIO_PinSource9
#endif
#if USART_PIN_TX_NUMBER == 10
    #define USART_PIN_TX            GPIO_Pin_10
    #define USART_PIN_TX_SOURCE     GPIO_PinSource10
#endif
/* RX */
#if USART_PIN_RX_NUMBER == 3
    #define USART_PIN_RX            GPIO_Pin_3
    #define USART_PIN_RX_SOURCE     GPIO_PinSource3
#endif
#if USART_PIN_RX_NUMBER == 6
    #define USART_PIN_RX            GPIO_Pin_6
    #define USART_PIN_RX_SOURCE     GPIO_PinSource6
#endif
#if USART_PIN_RX_NUMBER == 7
    #define USART_PIN_RX            GPIO_Pin_7
    #define USART_PIN_RX_SOURCE     GPIO_PinSource7
#endif
#if USART_PIN_RX_NUMBER == 9
    #define USART_PIN_RX            GPIO_Pin_9
    #define USART_PIN_RX_SOURCE     GPIO_PinSource9
#endif
#if USART_PIN_RX_NUMBER == 10
    #define USART_PIN_RX            GPIO_Pin_10
    #define USART_PIN_RX_SOURCE     GPIO_PinSource10
#endif
#if USART_PIN_RX_NUMBER == 11
    #define USART_PIN_RX            GPIO_Pin_11
    #define USART_PIN_RX_SOURCE     GPIO_PinSource11
#endif
/* port */
#if USART_PORT_LETTER == 'A'
    #define USART_PORT              GPIOA
    #define USART_PORT_RCC          RCC_AHB1Periph_GPIOA
#endif
#if USART_PORT_LETTER == 'B'
    #define USART_PORT              GPIOB
    #define USART_PORT_RCC          RCC_AHB1Periph_GPIOB
#endif
#if USART_PORT_LETTER == 'C'
    #define USART_PORT              GPIOC
    #define USART_PORT_RCC          RCC_AHB1Periph_GPIOC
#endif
#if USART_PORT_LETTER == 'D'
    #define USART_PORT              GPIOD
    #define USART_PORT_RCC          RCC_AHB1Periph_GPIOD
#endif


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/**
 * \fn      initUSART
 * \brief   initialisation of the USART interface
 * \todo    Callback-Funktion fuer Interrupt implementieren
 */
void initUSART(void)
{
    /* variable for CAN init */
    GPIO_InitTypeDef USART_gpio;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;


    /* pins configuration */
    USART_gpio.GPIO_Pin = USART_PIN_RX | USART_PIN_TX;
    USART_gpio.GPIO_Mode = USART_PINS_MODE;
    USART_gpio.GPIO_OType = USART_PINS_TYPE;
    USART_gpio.GPIO_Speed = USART_PINS_SPEED;
    USART_gpio.GPIO_PuPd = USART_PINS_PUPD;

    /* interface configuration */
    USART_InitStructure.USART_BaudRate = USART_BAUD;
    USART_InitStructure.USART_WordLength = USART_WORDLENGTH;
    USART_InitStructure.USART_StopBits = USART_STOPBITS;
    USART_InitStructure.USART_Parity = USART_PARITY;
    USART_InitStructure.USART_HardwareFlowControl = USART_FLOWCONTROL;
    USART_InitStructure.USART_Mode = USART_MODE;

    /* interrupt configuration */
    NVIC_InitStructure.NVIC_IRQChannel = USART_NVIC_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(USART_PORT_RCC, ENABLE);

    /* Enable UART clock */
#if USART_INTERFACE_NUMBER == 1 || USART_INTERFACE_NUMBER == 6
    RCC_APB2PeriphClockCmd(USART_PIN_RCC, ENABLE);
#elif USART_INTERFACE_NUMBER == 2 || USART_INTERFACE_NUMBER == 3 || USART_INTERFACE_NUMBER == 4 || USART_INTERFACE_NUMBER == 5
    RCC_APB1PeriphClockCmd(USART_PIN_RCC, ENABLE);
#endif
    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(USART_PORT, USART_PIN_TX_SOURCE, USART_PIN_AF);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(USART_PORT, USART_PIN_RX_SOURCE, USART_PIN_AF);
    GPIO_Init(USART_PORT, &USART_gpio);

    /* init */
    USART_Init(USART_INTERFACE, &USART_InitStructure);
    USART_ITConfig(USART_INTERFACE,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART_INTERFACE, ENABLE);
    NVIC_Init(&NVIC_InitStructure);
}


/**
 * \fn      sendCharacterUSART
 * \brief   send a byte over USART
 *
 * \param[in]   character   8 bit value
 * \return       None
 */
void sendCharacterUSART(char character)
{
    USART_SendData(USART_INTERFACE,character);
    while (USART_GetFlagStatus(USART_INTERFACE, USART_FLAG_TC) == RESET){}
}


/**
 * \fn      sendStringUSART
 * \brief   send a string over USART
 *
 * \param[in]   string  pointer to string
 * \return      None
 */
void sendStringUSART(char* string)
{
    while(*string)
    {
        sendCharacterUSART(*string);
        string++;
    }
}


/**
 * \fn      USARTx_IRQHandler
 * \brief   USART interrupt handler
 */
#if USART_INTERFACE_NUMBER == 1
void USART1_IRQHandler(void)
#elif USART_INTERFACE_NUMBER == 2
void USART2_IRQHandler(void)
#elif USART_INTERFACE_NUMBER == 3
void USART3_IRQHandler(void)
#elif USART_INTERFACE_NUMBER == 4
void USART4_IRQHandler(void)
#elif USART_INTERFACE_NUMBER == 5
void USART5_IRQHandler(void)
#elif USART_INTERFACE_NUMBER == 6
void USART6_IRQHandler(void)
#endif
{
    if (USART_GetITStatus(USART_INTERFACE,USART_IT_RXNE) == SET)
    {
        USART_ClearITPendingBit(USART_INTERFACE,USART_IT_RXNE);

    }
}


/**
 * \fn      itoa
 * \brief   C++ version 0.4 char* style "itoa"
 *
 * \note    Written by Lukás Chmela; http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
 */
char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}


/**
 * @}
 */
