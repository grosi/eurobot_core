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
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "can.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* interface */
#if CAN_INTERFACE_NUMBER == 1
    #ifndef CAN_INTERFACE
        #define CAN_INTERFACE     CAN1
    #endif
    #define CAN_PIN_AF            GPIO_AF_CAN1
    #define CAN_NVIC_CHANNEL      CAN1_RX0_IRQn
    #define CAN_PIN_RCC           RCC_APB1Periph_CAN1
#endif
#if CAN_INTERFACE_NUMBER == 2
    #ifndef CAN_INTERFACE
        #define CAN_INTERFACE     CAN2
    #endif
    #define CAN_PIN_AF            GPIO_AF_CAN2
    #define CAN_NVIC_CHANNEL      CAN2_RX0_IRQn
    #define CAN_PIN_RCC           RCC_APB1Periph_CAN2
#endif
/* TX */
#if CAN_PIN_TX_NUMBER == 0
    #define CAN_PIN_TX            GPIO_Pin_0
    #define CAN_PIN_TX_SOURCE     GPIO_PinSource0
#endif
#if CAN_PIN_TX_NUMBER == 1
    #define CAN_PIN_TX            GPIO_Pin_1
    #define CAN_PIN_TX_SOURCE     GPIO_PinSource1
#endif
#if CAN_PIN_TX_NUMBER == 6
    #define CAN_PIN_TX            GPIO_Pin_6
    #define CAN_PIN_TX_SOURCE     GPIO_PinSource6
#endif
#if CAN_PIN_TX_NUMBER == 9
    #define CAN_PIN_TX            GPIO_Pin_9
    #define CAN_PIN_TX_SOURCE     GPIO_PinSource9
#endif
#if CAN_PIN_TX_NUMBER == 12
    #define CAN_PIN_TX            GPIO_Pin_12
    #define CAN_PIN_TX_SOURCE     GPIO_PinSource12
#endif
#if CAN_PIN_TX_NUMBER == 13
    #define CAN_PIN_TX            GPIO_Pin_13
    #define CAN_PIN_TX_SOURCE     GPIO_PinSource13
#endif
/* RX */
#if CAN_PIN_RX_NUMBER == 0
    #define CAN_PIN_RX            GPIO_Pin_0
    #define CAN_PIN_RX_SOURCE     GPIO_PinSource0
#endif
#if CAN_PIN_RX_NUMBER == 1
    #define CAN_PIN_RX            GPIO_Pin_1
    #define CAN_PIN_RX_SOURCE     GPIO_PinSource1
#endif
#if CAN_PIN_RX_NUMBER == 5
    #define CAN_PIN_RX            GPIO_Pin_5
    #define CAN_PIN_RX_SOURCE     GPIO_PinSource5
#endif
#if CAN_PIN_RX_NUMBER == 8
    #define CAN_PIN_RX            GPIO_Pin_8
    #define CAN_PIN_RX_SOURCE     GPIO_PinSource8
#endif
#if CAN_PIN_RX_NUMBER == 11
    #define CAN_PIN_RX            GPIO_Pin_11
    #define CAN_PIN_RX_SOURCE     GPIO_PinSource11
#endif
#if CAN_PIN_RX_NUMBER == 12
    #define CAN_PIN_RX            GPIO_Pin_12
    #define CAN_PIN_RX_SOURCE     GPIO_PinSource12
#endif
/* EN */
#if CAN_PIN_EN_NUMBER == 1
    #define CAN_PIN_EN            GPIO_Pin_1
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource1
#endif
#if CAN_PIN_EN_NUMBER == 2
    #define CAN_PIN_EN            GPIO_Pin_2
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource2
#endif
#if CAN_PIN_EN_NUMBER == 3
    #define CAN_PIN_EN            GPIO_Pin_3
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource3
#endif
#if CAN_PIN_EN_NUMBER == 4
    #define CAN_PIN_EN            GPIO_Pin_4
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource4
#endif
#if CAN_PIN_EN_NUMBER == 5
    #define CAN_PIN_EN            GPIO_Pin_5
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource5
#endif
#if CAN_PIN_EN_NUMBER == 5
    #define CAN_PIN_EN            GPIO_Pin_5
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource5
#endif
#if CAN_PIN_EN_NUMBER == 6
    #define CAN_PIN_EN            GPIO_Pin_6
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource6
#endif
#if CAN_PIN_EN_NUMBER == 7
    #define CAN_PIN_EN            GPIO_Pin_7
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource7
#endif
#if CAN_PIN_EN_NUMBER == 8
    #define CAN_PIN_EN            GPIO_Pin_8
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource8
#endif
#if CAN_PIN_EN_NUMBER == 9
    #define CAN_PIN_EN            GPIO_Pin_9
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource9
#endif
#if CAN_PIN_EN_NUMBER == 10
    #define CAN_PIN_EN            GPIO_Pin_10
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource10
#endif
#if CAN_PIN_EN_NUMBER == 11
    #define CAN_PIN_EN            GPIO_Pin_11
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource11
#endif
#if CAN_PIN_EN_NUMBER == 12
    #define CAN_PIN_EN            GPIO_Pin_12
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource12
#endif
#if CAN_PIN_EN_NUMBER == 13
    #define CAN_PIN_EN            GPIO_Pin_13
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource13
#endif
#if CAN_PIN_EN_NUMBER == 14
    #define CAN_PIN_EN            GPIO_Pin_14
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource14
#endif
#if CAN_PIN_EN_NUMBER == 15
    #define CAN_PIN_EN            GPIO_Pin_15
    #define CAN_PIN_EN_SOURCE     GPIO_PinSource15
#endif
/* port TX/RX*/
#if CAN_PORT_LETTER == 'A'
    #define CAN_PORT              GPIOA
    #define CAN_PORT_RCC          RCC_AHB1Periph_GPIOA
#endif
#if CAN_PORT_LETTER == 'B'
    #define CAN_PORT              GPIOB
    #define CAN_PORT_RCC          RCC_AHB1Periph_GPIOB
#endif
#if CAN_PORT_LETTER == 'D'
    #define CAN_PORT              GPIOD
    #define CAN_PORT_RCC          RCC_AHB1Periph_GPIOD
#endif
/* port EN */
#if CAN_EN_PORT_LETTER == 'A'
    #define CAN_PORT              GPIOA
    #define CAN_PORT_RCC          RCC_AHB1Periph_GPIOA
#endif
#if CAN_EN_PORT_LETTER == 'B'
    #define CAN_PORT              GPIOB
    #define CAN_PORT_RCC          RCC_AHB1Periph_GPIOB
#endif
#if CAN_EN_PORT_LETTER == 'C'
    #define CAN_PORT              GPIOC
    #define CAN_PORT_RCC          RCC_AHB1Periph_GPIOC
#endif
#if CAN_EN_PORT_LETTER == 'D'
    #define CAN_EN_PORT           GPIOD
    #define CAN_EN_PORT_RCC       RCC_AHB1Periph_GPIOD
#endif
#if CAN_EN_PORT_LETTER == 'E'
    #define CAN_EN_PORT           GPIOE
    #define CAN_EN_PORT_RCC       RCC_AHB1Periph_GPIOE
#endif
#if CAN_EN_PORT_LETTER == 'F'
    #define CAN_EN_PORT           GPIOF
    #define CAN_EN_PORT_RCC       RCC_AHB1Periph_GPIOF
#endif
#if CAN_EN_PORT_LETTER == 'G'
    #define CAN_EN_PORT           GPIOG
    #define CAN_EN_PORT_RCC       RCC_AHB1Periph_GPIOG
#endif
#if CAN_EN_PORT_LETTER == 'H'
    #define CAN_EN_PORT           GPIOH
    #define CAN_EN_PORT_RCC       RCC_AHB1Periph_GPIOH
#endif
#if CAN_EN_PORT_LETTER == 'I'
    #define CAN_EN_PORT           GPIOI
    #define CAN_EN_PORT_RCC       RCC_AHB1Periph_GPIOI
#endif
/* baudrate */
#if CAN_BAUD == 1000
    #define CAN_TIME_BS1    CAN_BS1_12tq; /*!< Baudrate = 42MHz / (Prescaler * (BS1 + BS2 + 1)) */
    #define CAN_TIME_BS2    CAN_BS2_8tq; /*!< default baudrate = 42MHz / (2 * (12+8+1) = 1MBd */
    #define CAN_PRESCALER   2
#elif CAN_BAUD == 500
    #define CAN_TIME_BS1    CAN_BS1_12tq; /*!< Baudrate = 42MHz / (Prescaler * (BS1 + BS2 + 1)) */
    #define CAN_TIME_BS2    CAN_BS2_8tq; /*!< default baudrate = 42MHz / (4 * (12+8+1) = 500kBit/s */
    #define CAN_PRESCALER   4
#elif CAN_BAUD == 250
    #define CAN_TIME_BS1    CAN_BS1_12tq; /*!< Baudrate = 42MHz / (Prescaler * (BS1 + BS2 + 1)) */
    #define CAN_TIME_BS2    CAN_BS2_8tq; /*!< default baudrate = 42MHz / (2 * (12+8+1) = 250 kBit/s */
    #define CAN_PRESCALER   8
#else
    #error "undefined baudrate"
#endif
/*shift offsets */
#define CAN_ID_FILTER_OFFSET 5


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static CANRxCatch_t rx_function; /*!< callback-function pointer */
static uint16_t filter_bank[CAN_FILTER][4]; /*!< 4 filter-values per filter-bank */
static uint8_t filter_bank_number = 0; /*!< current filter-bank number */


/* Private function prototypes -----------------------------------------------*/
static void initCANInterrupt();


/**
 * \fn      initCAN
 * \brief   initialize the can interface
 *
 * \param[in]   app_rx_funktion pointer to callback function
 * \return  None
 */
void initCAN(CANRxCatch_t app_rx_funktion)
{
    /* variable for CAN init */
    GPIO_InitTypeDef CAN_gpio;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    uint8_t filter_count;


    /* pins configuration */
    CAN_gpio.GPIO_Pin = CAN_PIN_TX | CAN_PIN_RX;
    CAN_gpio.GPIO_Mode = CAN_PINS_MODE;
    CAN_gpio.GPIO_OType = CAN_PINS_TYPE;
    CAN_gpio.GPIO_Speed = CAN_PINS_SPEED;
    CAN_gpio.GPIO_PuPd = CAN_PINS_PUPD;

    /* enable the CAN-interface interrupt */
    rx_function = app_rx_funktion;
    initCANInterrupt(CAN_INTERFACE);

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(CAN_PORT_RCC,ENABLE);
#if defined(CAN_PIN_EN_NUMBER) && defined(CAN_EN_PORT_LETTER) && defined(CAN_PIN_EN_MODE) && defined(CAN_LEVEL_ACTIVE)
    RCC_AHB1PeriphClockCmd(CAN_EN_PORT_RCC,ENABLE); /* EN Pin */
#endif

    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(CAN_PIN_RCC, ENABLE);

    /* Connect PXx to CANTx_Tx*/
    GPIO_PinAFConfig(CAN_PORT, CAN_PIN_TX_SOURCE, CAN_PIN_AF);

    /* Connect PXx to CANRx_Rx*/
    GPIO_PinAFConfig(CAN_PORT, CAN_PIN_RX_SOURCE, CAN_PIN_AF);

    /* Init Tx- and Rx-pins */
    GPIO_Init(CAN_PORT,&CAN_gpio);

    /* CAN EN Pin */
#if defined(CAN_PIN_EN_NUMBER) && defined(CAN_EN_PORT_LETTER) && defined(CAN_PIN_EN_MODE) && defined(CAN_LEVEL_ACTIVE)
    CAN_gpio.GPIO_Pin = CAN_PIN_EN;
    CAN_gpio.GPIO_Mode = CAN_PIN_EN_MODE;
    GPIO_Init( CAN_EN_PORT, &CAN_gpio );
#if CAN_LEVEL_ACTIVE == 'L'
    GPIO_ResetBits(CAN_EN_PORT,CAN_PIN_EN); /* set the RS pin of the CAN-transceiver to low -> no sleep-mode */
#else
    GPIO_SetBits(CAN_EN_PORT,CAN_PIN_EN); /* set the RS pin of the CAN-transceiver to low -> no sleep-mode */
#endif
#endif

    /* CAN register reset */
    CAN_DeInit(CAN_INTERFACE);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE; /* Time Triggered Communication Mode */
    CAN_InitStructure.CAN_ABOM = DISABLE; /* Automatic Bus-Off Management */
    CAN_InitStructure.CAN_AWUM = DISABLE; /* Automatic Wakeup Mode */
    CAN_InitStructure.CAN_NART = ENABLE; /* No Automatic Retransmission */
    CAN_InitStructure.CAN_RFLM = DISABLE; /* Receive FIFO Locked Mode  */
    CAN_InitStructure.CAN_TXFP = DISABLE; /* Transmit FIFO Priority */
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

    /* CAN Baudrate */
    CAN_InitStructure.CAN_BS1 = CAN_TIME_BS1;
    CAN_InitStructure.CAN_BS2 = CAN_TIME_BS2;
    CAN_InitStructure.CAN_Prescaler = CAN_PRESCALER;
    CAN_Init(CAN_INTERFACE, &CAN_InitStructure);
    
    /* CAN filter init */
    for(filter_count = 0; filter_count < filter_bank_number + 1; filter_count++)
    {
        CAN_FilterInitStructure.CAN_FilterNumber = filter_count;
        CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
        CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

        CAN_FilterInitStructure.CAN_FilterIdLow = filter_bank[filter_count][0]; /* lower 16bit CAN_FxR1 */
        CAN_FilterInitStructure.CAN_FilterMaskIdLow = filter_bank[filter_count][1]; /* upper 16bit CAN_FxR1 */
        CAN_FilterInitStructure.CAN_FilterIdHigh = filter_bank[filter_count][2]; /* lower 16bit CAN_FxR2 */
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh = filter_bank[filter_count][3]; /* upper 16bit CAN_FxR2 */

        CAN_FilterInit(&CAN_FilterInitStructure);
    }

    /* Enable FIFO 0 message pending Interrupt */
    CAN_ITConfig(CAN_INTERFACE, CAN_IT_FMP0, ENABLE);
}


/**
 * \fn      setCANFilter
 * \brief   initialize the can filter-banks. The filter-methode is ID-based
 *
 * \param[in]   filter_id    Identifier of the can-message
 * \retval  0   error
 * \retval  1   success
 */
uint8_t setCANFilter(uint16_t filter_id)
{
    static uint8_t filter_id_number = 0;
    uint8_t result = 1;
    uint8_t i,j,id_exist = 0;

    /* check if the id number already written */
    for(i = 0;i < filter_bank_number + 1; i++)
    {
        /* sweep trough the ID's in the current filter-bank */
        for(j = 0; j < 4; j++)
        {
            if(filter_bank[i][j] == filter_id << CAN_ID_FILTER_OFFSET)
            {
                i = filter_id_number; /* break the outer loop too */
                id_exist = 1;
                break;
            }
        }
    }

    /* control and configure the filter-bank-array */
    if(filter_id_number >= 4)
    {
        if(filter_bank_number < CAN_FILTER - 1)
        {
            filter_id_number = 0;
            filter_bank_number++;
        }
        else
        {
            result = 0;
        }

    }

    /* set the filter-bank-id */
    if(result && !id_exist)
    {
        filter_bank[filter_bank_number][filter_id_number] = filter_id << CAN_ID_FILTER_OFFSET;
        filter_id_number++;
    }

    return result;
}


/**
 * \fn      initCANInterrupt
 * \brief   initialize the interrupt for a can interface
 *
 * \param   None
 * \retval  None
 */
static inline void initCANInterrupt(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    /* config the interrupt channel */
    NVIC_InitStructure.NVIC_IRQChannel = CAN_NVIC_CHANNEL;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**
 * \fn      CAN1_RX0_IRQHandler
 * \brief   This function handles CAN1 RX0 request.
 *
 * \param   None
 * \return  None
 */
#if CAN_INTERFACE_NUMBER == 1
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RX_Message;
    
    /* receive CAN-message */
    CAN_Receive(CAN1, CAN_FIFO0, &RX_Message);
    
    /* call rx catch-function (see CAN gatekeeper for more infos) */
    rx_function(RX_Message);
}
#endif


/**
 * \fn      CAN2_RX0_IRQHandler
 * \brief   This function handles CAN2 RX0 request.
 *
 * \param   None
 * \retval  None
 */
#if CAN_INTERFACE_NUMBER == 2
void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg RX_Message;
    
    /* receive CAN-message */
    CAN_Receive(CAN2, CAN_FIFO0, &RX_Message);
    
    /* call rx catch-function (see CAN gatekeeper for more infos) */
    rx_function(RX_Message);
}
#endif


/**
 * @}
 */
