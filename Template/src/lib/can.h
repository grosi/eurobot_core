/**
 * \file    can.c
 * \author  Schüpbach Simon
 * \author  gross10
 * \date    2013-12-15
 *
 * \version 3.1
 *  add en-pin functionality
 *
 * \version 3.0
 *  separation between the CAN-library and RTOS
 *
 * \version 2.0
 *  realise the id-switch with void pointer
 *
 * \version 1.0
 *  create this file
 *
 * \brief   this file contains function for the can handling.
 *
 * \defgroup can CAN
 * \brief   CAN library
 * \ingroup firmware
 *
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H
#define __CAN_H

/* exported typedef ----------------------------------------------------------*/
/**
 * \brief function-handler to application-area (callback-function)
 */
typedef void (*CANRxCatch_t)(CanRxMsg);

/* exported define -----------------------------------------------------------*/
/* Pin and Port of the CAN-Interface */
#define CAN_PIN_TX_NUMBER       0 /*!< tx pin-number */
#define CAN_PIN_RX_NUMBER       1 /*!< rx pin-number */
//#define CAN_PIN_EN_NUMBER       2 /*!< if not used -> comment it! */
#define CAN_PINS_MODE           GPIO_Mode_AF
//#define CAN_PIN_EN_MODE         GPIO_Mode_OUT /*!< if not used -> comment it! */
//#define CAN_LEVEL_ACTIVE        'L' /*!< L = low active, H = high active;  if not used -> comment it! */
#define CAN_PINS_SPEED          GPIO_Speed_50MHz
#define CAN_PINS_TYPE           GPIO_OType_PP
#define CAN_PINS_PUPD           GPIO_PuPd_UP
#define CAN_PORT_LETTER         'D' /*!< port letter */
//#define CAN_EN_PORT_LETTER      'D' /*!< if not used -> comment it! */
#define CAN_INTERFACE_NUMBER    1 /*!< 1 or 2 */
#define CAN_BAUD                500 /*!< kBit/s Baudrate */
#define CAN_FILTER              14 /*!< Amount of filter banks -> max. 14 */

#if CAN_INTERFACE_NUMBER == 1
    #ifndef CAN_INTERFACE
        #define CAN_INTERFACE     CAN1
    #endif
#endif
#if CAN_INTERFACE_NUMBER == 2
    #ifndef CAN_INTERFACE
        #define CAN_INTERFACE     CAN2
    #endif
#endif

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initCAN(CANRxCatch_t);
extern uint8_t setCANFilter(uint16_t);

#endif

/**
 * @}
 */
