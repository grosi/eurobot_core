/**
 * \file    ir_sensor.c
 * \author  gross10
 * \date    2014-02-13
 *
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
#include "ir_sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void initIRSensor(uint32_t, GPIOMode_TypeDef, GPIOOType_TypeDef,
        GPIOPuPd_TypeDef, GPIOSpeed_TypeDef, GPIO_TypeDef*, uint32_t);


/**
 * \fn      initIRSensors
 * \brief   initialisation of the 4 IR sensors
 */
void initIRSensors()
{
    initIRSensor(IR_SENSOR_BACK_PIN, IR_SENSOR_BACK_PIN_MODE, IR_SENSOR_BACK_PIN_TYPE, IR_SENSOR_BACK_PIN_PUPD, IR_SENSOR_BACK_PIN_SPEED,
            IR_SENSOR_BACK_PORT, IR_SENSOR_BACK_PORT_CLK);
    initIRSensor(IR_SENSOR_FRONT_PIN, IR_SENSOR_FRONT_PIN_MODE, IR_SENSOR_FRONT_PIN_TYPE, IR_SENSOR_FRONT_PIN_PUPD, IR_SENSOR_FRONT_PIN_SPEED,
                IR_SENSOR_FRONT_PORT, IR_SENSOR_FRONT_PORT_CLK);
    initIRSensor(IR_SENSOR_LEFT_PIN, IR_SENSOR_LEFT_PIN_MODE, IR_SENSOR_LEFT_PIN_TYPE, IR_SENSOR_LEFT_PIN_PUPD, IR_SENSOR_LEFT_PIN_SPEED,
                IR_SENSOR_LEFT_PORT, IR_SENSOR_LEFT_PORT_CLK);
    initIRSensor(IR_SENSOR_RIGHT_PIN, IR_SENSOR_RIGHT_PIN_MODE, IR_SENSOR_RIGHT_PIN_TYPE, IR_SENSOR_RIGHT_PIN_PUPD, IR_SENSOR_RIGHT_PIN_SPEED,
                IR_SENSOR_RIGHT_PORT, IR_SENSOR_RIGHT_PORT_CLK);
}


/**
 * \fn      getIRSensor_Back
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getIRSensor_Back()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_BACK_PORT, IR_SENSOR_BACK_PIN);
}


/**
 * \fn      getIRSensor_Front
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getIRSensor_Front()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_FRONT_PORT, IR_SENSOR_FRONT_PIN);
}


/**
 * \fn      getIRSensor_Left
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getIRSensor_Left()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_LEFT_PORT, IR_SENSOR_LEFT_PIN);
}


/**
 * \fn      getIRSensor_Right
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
inline uint8_t getIRSensor_Right()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_RIGHT_PORT, IR_SENSOR_RIGHT_PIN);
}


/**
 * \fn      initSensor
 * \brief   initialisation a sensor
 *
 * \param   pin         pin number
 * \param   mode        pin mode -> output
 * \param   type        pin type -> pushpull
 * \param   pupd        pin pullup/pulldown
 * \param   speed       pin speed
 * \param   port        port letter
 * \param   port_clk    port clock source
 */
void initIRSensor(uint32_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type,
        GPIOPuPd_TypeDef pupd, GPIOSpeed_TypeDef speed, GPIO_TypeDef* port, uint32_t port_clk)
{
    /* variable for sensor init */
    GPIO_InitTypeDef sensor_gpio;

    /* initialize gpio */
    sensor_gpio.GPIO_Pin = pin;
    sensor_gpio.GPIO_Mode = mode;
    sensor_gpio.GPIO_OType = type;
    sensor_gpio.GPIO_PuPd = pupd;
    sensor_gpio.GPIO_Speed = speed;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(port_clk, ENABLE);

    /* enables port and pin */
    GPIO_Init(port,&sensor_gpio);
}


/**
 * @}
 */
