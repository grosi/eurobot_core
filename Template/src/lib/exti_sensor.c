/**
 * \file    exti_sensor.c
 * \author  gross10
 * \date    2014-02-26
 *
 * \version 1.1
 *  added initEXTILines (kasen1)
 * \version 1.0
 *  create this file
 *
 * \brief   Sensor access for the ir sensor from the rangefinder module.
 *
 * \todo    unit test
 *
 * \defgroup ir_sensor IR_Sensor
 * \brief   ir sensor library
 * \ingroup firmware
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "sensor.h"
#include "exti_sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static void initEXTILine(uint32_t, uint8_t, uint8_t, uint32_t, EXTITrigger_TypeDef,
        uint8_t, uint8_t, uint8_t);



/**
 * \fn      initSensorEXTI_EmergencyStop
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_EmergencyStop()
{
    initEXTILine(SENSOR_EMERGENCYSTOP_PORT_CLK, SENSOR_EMERGENCYSTOP_EXTI_PORT, SENSOR_EMERGENCYSTOP_EXTI_PIN,
                SENSOR_EMERGENCYSTOP_EXTI_LINE, SENSOR_EMERGENCYSTOP_EXTI_TRIG, SENSOR_EMERGENCYSTOP_EXTI_CHAN,
                SENSOR_EMERGENCYSTOP_EXTI_PPRIO, SENSOR_EMERGENCYSTOP_EXTI_SPRIO);
}


/**
 * \fn      initSensorEXTI_Fresco_1
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_Fresco_1()
{
    initEXTILine(SENSOR_FRESCO_1_PORT_CLK, SENSOR_FRESCO_1_EXTI_PORT, SENSOR_FRESCO_1_EXTI_PIN,
                SENSOR_FRESCO_1_EXTI_LINE, SENSOR_FRESCO_1_EXTI_TRIG, SENSOR_FRESCO_1_EXTI_CHAN,
                SENSOR_FRESCO_1_EXTI_PPRIO, SENSOR_FRESCO_1_EXTI_SPRIO);
}


/**
 * \fn      initSensorEXTI_Fresco_2
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_Fresco_2()
{
    initEXTILine(SENSOR_FRESCO_2_PORT_CLK, SENSOR_FRESCO_2_EXTI_PORT, SENSOR_FRESCO_2_EXTI_PIN,
                SENSOR_FRESCO_2_EXTI_LINE, SENSOR_FRESCO_2_EXTI_TRIG, SENSOR_FRESCO_2_EXTI_CHAN,
                SENSOR_FRESCO_2_EXTI_PPRIO, SENSOR_FRESCO_2_EXTI_SPRIO);
}


/**
 * \fn      initSensorEXTI_Fresco_2
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_Wall()
{
    initEXTILine(SENSOR_FRESCO_WALL_PORT_CLK, SENSOR_FRESCO_WALL_EXTI_PORT, SENSOR_FRESCO_WALL_EXTI_PIN,
                SENSOR_FRESCO_WALL_EXTI_LINE, SENSOR_FRESCO_WALL_EXTI_TRIG, SENSOR_FRESCO_WALL_EXTI_CHAN,
                SENSOR_FRESCO_WALL_EXTI_PPRIO, SENSOR_FRESCO_WALL_EXTI_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_Back
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_Back()
{
    initEXTILine(IR_SENSOR_BACK_PORT_CLK, IR_SENSOR_BACK_EXTI_PORT, IR_SENSOR_BACK_EXTI_PIN,
            IR_SENSOR_BACK_EXTI_LINE, IR_SENSOR_BACK_EXTI_TRIG, IR_SENSOR_BACK_NVIC_CHAN,
            IR_SENSOR_BACK_NVIC_PPRIO, IR_SENSOR_BACK_NVIC_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_Front
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_Front()
{
    initEXTILine(IR_SENSOR_FRONT_PORT_CLK, IR_SENSOR_FRONT_EXTI_PORT, IR_SENSOR_FRONT_EXTI_PIN,
            IR_SENSOR_FRONT_EXTI_LINE, IR_SENSOR_FRONT_EXTI_TRIG, IR_SENSOR_FRONT_NVIC_CHAN,
            IR_SENSOR_FRONT_NVIC_PPRIO, IR_SENSOR_FRONT_NVIC_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_Left
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_Left()
{
    initEXTILine(IR_SENSOR_LEFT_PORT_CLK, IR_SENSOR_LEFT_EXTI_PORT, IR_SENSOR_LEFT_EXTI_PIN,
            IR_SENSOR_LEFT_EXTI_LINE, IR_SENSOR_LEFT_EXTI_TRIG, IR_SENSOR_LEFT_NVIC_CHAN,
            IR_SENSOR_LEFT_NVIC_PPRIO, IR_SENSOR_LEFT_NVIC_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_Right
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_Right()
{
    initEXTILine(IR_SENSOR_RIGHT_PORT_CLK, IR_SENSOR_RIGHT_EXTI_PORT, IR_SENSOR_RIGHT_EXTI_PIN,
            IR_SENSOR_RIGHT_EXTI_LINE, IR_SENSOR_RIGHT_EXTI_TRIG, IR_SENSOR_RIGHT_NVIC_CHAN,
            IR_SENSOR_RIGHT_NVIC_PPRIO, IR_SENSOR_RIGHT_NVIC_SPRIO);
}


/**
 * \fn      initSensorEXTI_Key
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_Key()
{
    initEXTILine(SENSOR_KEY_PORT_CLK, SENSOR_KEY_EXTI_PORT, SENSOR_KEY_EXTI_PIN,
                SENSOR_KEY_EXTI_LINE, SENSOR_KEY_EXTI_TRIG, SENSOR_KEY_EXTI_CHAN,
                SENSOR_KEY_EXTI_PPRIO, SENSOR_KEY_EXTI_SPRIO);
}

/**
 * \fn      initEXTILine
 * \brief   Configures EXTI Line in interrupt mode
 *
 * \param   RCC_AHB1Periph                    e.g. RCC_AHB1Periph_GPIOB (for PB5)
 * \param   EXTI_PortSourceGPIOx              e.g. EXTI_PortSourceGPIOB (for PB5)
 * \param   EXTI_PinSourcex                   e.g. EXTI_PinSource5      (for PB5)
 * \param   EXTI_Line                         e.g. EXTI_Line5           (for PB5)
 * \param   EXTI_Trigger                      e.g. EXTI_Trigger_Falling
 * \param   NVIC_IRQChannel                   e.g. EXTI9_5_IRQn         (for PB5)
 * \param   NVIC_IRQChannelPreemptionPriority e.g. 0x01 (lowest)
 * \param   NVIC_IRQChannelSubPriority        e.g. 0x01 (lowest)
 */
static void initEXTILine(uint32_t RCC_AHB1Periph, uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex, uint32_t EXTI_Line, EXTITrigger_TypeDef EXTI_Trigger,
        uint8_t NVIC_IRQChannel, uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable GPIO clock and SYSCFG clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Connect EXTI Line to pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);

    /* Configure EXTI Line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI Line Interrupt to priority */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
    NVIC_Init(&NVIC_InitStructure);
}



/**
 * @}
 */
