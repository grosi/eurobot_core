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
 * \defgroup usart USART
 * \brief   USART library
 * \ingroup firmware
 *
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USART_H_
#define USART_H_


/* exported typedef ----------------------------------------------------------*/
typedef void (*CANRxCatch_t)(CanRxMsg); /* function-handler to application-area */


/* exported define -----------------------------------------------------------*/
/* Pin and Port of the USART-Interface */
#define USART_PIN_TX_NUMBER     8
#define USART_PIN_RX_NUMBER     9
#define USART_PINS_MODE         GPIO_Mode_AF
#define USART_PINS_SPEED        GPIO_Speed_50MHz
#define USART_PINS_TYPE         GPIO_OType_PP
#define USART_PINS_PUPD         GPIO_PuPd_UP
#define USART_PORT_LETTER       'D' /*!< port A to D */
#define USART_INTERFACE_NUMBER  3 /*!< USART number (1 to 6) */
#define USART_BAUD              115200 /*!< kBit/s Baudrate */
#define USART_WORDLENGTH        USART_WordLength_8b
#define USART_STOPBITS          USART_StopBits_1
#define USART_PARITY            USART_Parity_No
#define USART_FLOWCONTROL       USART_HardwareFlowControl_None
#define USART_MODE              USART_Mode_Rx | USART_Mode_Tx


/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initUSART(void);
extern void sendCharacterUSART(char);
extern void sendStringUSART(char*);


#endif /* USART_H_ */

/**
 * @}
 */
