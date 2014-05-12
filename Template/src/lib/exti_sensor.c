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
 * \fn      initSensorEXTI_Fire_Pool_Right
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_Fire_Pool_Right()
{
    initEXTILine(SENSOR_FIRE_POOL_RIGHT_PORT_CLK, SENSOR_FIRE_POOL_RIGHT_EXTI_PORT, SENSOR_FIRE_POOL_RIGHT_EXTI_PIN,
                SENSOR_FIRE_POOL_RIGHT_EXTI_LINE, SENSOR_FIRE_POOL_RIGHT_EXTI_TRIG, SENSOR_FIRE_POOL_RIGHT_EXTI_CHAN,
                SENSOR_FIRE_POOL_RIGHT_EXTI_PPRIO, SENSOR_FIRE_POOL_RIGHT_EXTI_SPRIO);
}


/**
 * \fn      initSensorEXTI_Fresco_2
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_Air()
{
    initEXTILine(SENSOR_AIR_PORT_CLK, SENSOR_AIR_EXTI_PORT, SENSOR_AIR_EXTI_PIN,
                SENSOR_AIR_EXTI_LINE, SENSOR_AIR_EXTI_TRIG, SENSOR_AIR_EXTI_CHAN,
                SENSOR_AIR_EXTI_PPRIO, SENSOR_AIR_EXTI_SPRIO);
}


/**
 * \fn      initSensorEXTI_Fire_Pool_Left
 * \brief   initialisation of the EXTI-line
 */
void initSensorEXTI_Fire_Pool_Left()
{
    initEXTILine(SENSOR_FIRE_POOL_LEFT_PORT_CLK, SENSOR_FIRE_POOL_LEFT_EXTI_PORT, SENSOR_FIRE_POOL_LEFT_EXTI_PIN,
                SENSOR_FIRE_POOL_LEFT_EXTI_LINE, SENSOR_FIRE_POOL_LEFT_EXTI_TRIG, SENSOR_FIRE_POOL_LEFT_EXTI_CHAN,
                SENSOR_FIRE_POOL_LEFT_EXTI_PPRIO, SENSOR_FIRE_POOL_LEFT_EXTI_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_BwLeft
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_BwLeft()
{
    initEXTILine(IR_SENSOR_BWLEFT_PORT_CLK, IR_SENSOR_BWLEFT_EXTI_PORT, IR_SENSOR_BWLEFT_EXTI_PIN,
            IR_SENSOR_BWLEFT_EXTI_LINE, IR_SENSOR_BWLEFT_EXTI_TRIG, IR_SENSOR_BWLEFT_NVIC_CHAN,
            IR_SENSOR_BWLEFT_NVIC_PPRIO, IR_SENSOR_BWLEFT_NVIC_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_FwLeft
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_FwLeft()
{
    initEXTILine(IR_SENSOR_FWLEFT_PORT_CLK, IR_SENSOR_FWLEFT_EXTI_PORT, IR_SENSOR_FWLEFT_EXTI_PIN,
            IR_SENSOR_FWLEFT_EXTI_LINE, IR_SENSOR_FWLEFT_EXTI_TRIG, IR_SENSOR_FWLEFT_NVIC_CHAN,
            IR_SENSOR_FWLEFT_NVIC_PPRIO, IR_SENSOR_FWLEFT_NVIC_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_BwRight
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_BwRight()
{
    initEXTILine(IR_SENSOR_BWRIGHT_PORT_CLK, IR_SENSOR_BWRIGHT_EXTI_PORT, IR_SENSOR_BWRIGHT_EXTI_PIN,
            IR_SENSOR_BWRIGHT_EXTI_LINE, IR_SENSOR_BWRIGHT_EXTI_TRIG, IR_SENSOR_BWRIGHT_NVIC_CHAN,
            IR_SENSOR_BWRIGHT_NVIC_PPRIO, IR_SENSOR_BWRIGHT_NVIC_SPRIO);
}


/**
 * \fn      initIRSensorEXTI_FwRight
 * \brief   initialisation of the EXTI-line
 */
void initIRSensorEXTI_FwRight()
{
    initEXTILine(IR_SENSOR_FWRIGHT_PORT_CLK, IR_SENSOR_FWRIGHT_EXTI_PORT, IR_SENSOR_FWRIGHT_EXTI_PIN,
            IR_SENSOR_FWRIGHT_EXTI_LINE, IR_SENSOR_FWRIGHT_EXTI_TRIG, IR_SENSOR_FWRIGHT_NVIC_CHAN,
            IR_SENSOR_FWRIGHT_NVIC_PPRIO, IR_SENSOR_FWRIGHT_NVIC_SPRIO);
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
