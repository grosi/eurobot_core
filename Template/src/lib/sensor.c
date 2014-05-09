/**
 * \file    sensor.c
 * \author  gross10
 * \date    2014-02-11
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Sensor access for the roboboard and user-panel. This libary is only for the small
 *          roboter
 * \note    the sensors must be connected in low-active mode
 *
 * \todo    unit test
 *
 * \defgroup button Button
 * \brief   roboboard sensor library
 * \ingroup firmware
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "sensor.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void initSensor(uint32_t, GPIOMode_TypeDef, GPIOOType_TypeDef,
        GPIOPuPd_TypeDef, GPIOSpeed_TypeDef, GPIO_TypeDef*, uint32_t);
uint8_t getSensorPosEdge(GPIO_TypeDef*, uint16_t, uint8_t** );
uint8_t getSensorNegEdge(GPIO_TypeDef*, uint16_t, uint8_t** );



/**
 * \fn      initSensor_Air
 * \brief   sensor air initialisation
 */
void initSensor_Air()
{
    initSensor(SENSOR_AIR_PIN, SENSOR_AIR_PIN_MODE, SENSOR_AIR_PIN_TYPE, SENSOR_AIR_PIN_PUPD, SENSOR_AIR_PIN_SPEED,
            SENSOR_AIR_PORT, SENSOR_AIR_PORT_CLK);
}


/**
 * \fn      getSensor_Air
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getSensor_Air()
{
    return !GPIO_ReadInputDataBit(SENSOR_AIR_PORT, SENSOR_AIR_PIN);
}


/**
 * \fn      getSensorPosEdge_Air
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_Air(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_AIR_PORT,SENSOR_AIR_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_Air
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_Air(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_AIR_PORT,SENSOR_AIR_PIN, &sensor_state);
}


/**
 * \fn      initSensor_Fire_Pool
 * \brief   sensor fire-pool initialisation
 */
void initSensor_Fire_Pool()
{
    initSensor(SENSOR_FIRE_POOL_PIN, SENSOR_FIRE_POOL_PIN_MODE, SENSOR_FIRE_POOL_PIN_TYPE, SENSOR_FIRE_POOL_PIN_PUPD, SENSOR_FIRE_POOL_PIN_SPEED,
            SENSOR_FIRE_POOL_PORT, SENSOR_FIRE_POOL_PORT_CLK);
}


/**
 * \fn      getSensor_Fire_Pool
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getSensor_Fire_Pool()
{
    return !GPIO_ReadInputDataBit(SENSOR_FIRE_POOL_PORT, SENSOR_FIRE_POOL_PIN);
}


/**
 * \fn      getSensorPosEdge_Fire_Pool
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_Fire_Pool(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_FIRE_POOL_PORT,SENSOR_FIRE_POOL_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_Fire_Pool
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_Fire_Pool(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_FIRE_POOL_PORT,SENSOR_FIRE_POOL_PIN, &sensor_state);
}


/**
 * \fn      initSensor_1
 * \brief   sensor 1 initialisation
 */
void initSensor_1()
{
    initSensor(SENSOR_1_PIN, SENSOR_1_PIN_MODE, SENSOR_1_PIN_TYPE, SENSOR_1_PIN_PUPD, SENSOR_1_PIN_SPEED,
            SENSOR_1_PORT, SENSOR_1_PORT_CLK);
}


/**
 * \fn      getSensor_1
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getSensor_1()
{
    return !GPIO_ReadInputDataBit(SENSOR_1_PORT, SENSOR_1_PIN);
}


/**
 * \fn      getSensorPosEdge_1
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_1(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_1_PORT,SENSOR_1_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_1
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_1(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_1_PORT,SENSOR_1_PIN, &sensor_state);
}


/**
 * \fn      initSensor_EmergencyStop
 * \brief   emergency stop initialisation
 */
void initSensor_EmergencyStop()
{
    initSensor(SENSOR_EMERGENCYSTOP_PIN, SENSOR_EMERGENCYSTOP_PIN_MODE, SENSOR_EMERGENCYSTOP_PIN_TYPE, SENSOR_EMERGENCYSTOP_PIN_PUPD, SENSOR_EMERGENCYSTOP_PIN_SPEED,
            SENSOR_EMERGENCYSTOP_PORT, SENSOR_EMERGENCYSTOP_PORT_CLK);
}


/**
 * \fn      getSensor_EmergencyStop
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getSensor_EmergencyStop()
{
    return !GPIO_ReadInputDataBit(SENSOR_EMERGENCYSTOP_PORT, SENSOR_EMERGENCYSTOP_PIN);
}


/**
 * \fn      getSensorPosEdge_EmergencyStop
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_EmergencyStop(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_EMERGENCYSTOP_PORT,SENSOR_EMERGENCYSTOP_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_EmergencyStop
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_EmergencyStop(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_EMERGENCYSTOP_PORT,SENSOR_EMERGENCYSTOP_PIN, &sensor_state);
}


/**
 * \fn      initIRSensor_BwLeft
 * \brief   IR sensor initialisation
 */
void initIRSensor_BwLeft()
{
    initSensor(IR_SENSOR_BWLEFT_PIN, IR_SENSOR_BWLEFT_PIN_MODE, IR_SENSOR_BWLEFT_PIN_TYPE, IR_SENSOR_BWLEFT_PIN_PUPD, IR_SENSOR_BWLEFT_PIN_SPEED,
            IR_SENSOR_BWLEFT_PORT, IR_SENSOR_BWLEFT_PORT_CLK);
}


/**
 * \fn      getIRSensor_BwLeft
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_BwLeft()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_BWLEFT_PORT, IR_SENSOR_BWLEFT_PIN);
}


/**
 * \fn      getIRSensorPosEdge_BwLeft
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_BwLeft(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_BWLEFT_PORT,IR_SENSOR_BWLEFT_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_BwLeft
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_BwLeft(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_BWLEFT_PORT,IR_SENSOR_BWLEFT_PIN, &sensor_state);
}


/**
 * \fn      initIRSensor_FwLeft
 * \brief   IR sensor initialisation
 */
void initIRSensor_FwLeft()
{
    initSensor(IR_SENSOR_FWLEFT_PIN, IR_SENSOR_FWLEFT_PIN_MODE, IR_SENSOR_FWLEFT_PIN_TYPE, IR_SENSOR_FWLEFT_PIN_PUPD, IR_SENSOR_FWLEFT_PIN_SPEED,
            IR_SENSOR_FWLEFT_PORT, IR_SENSOR_FWLEFT_PORT_CLK);
}


/**
 * \fn      getIRSensor_FwLeft
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_FwLeft()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_FWLEFT_PORT, IR_SENSOR_FWLEFT_PIN);
}


/**
 * \fn      getIRSensorPosEdge_FwLeft
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_FwLeft(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_FWLEFT_PORT,IR_SENSOR_FWLEFT_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_FwLeft
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_FwLeft(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_FWLEFT_PORT,IR_SENSOR_FWLEFT_PIN, &sensor_state);
}


/**
 * \fn      initIRSensor_BwRight
 * \brief   IR sensor initialisation
 */
void initIRSensor_BwRight()
{
    initSensor(IR_SENSOR_BWRIGHT_PIN, IR_SENSOR_BWRIGHT_PIN_MODE, IR_SENSOR_BWRIGHT_PIN_TYPE, IR_SENSOR_BWRIGHT_PIN_PUPD, IR_SENSOR_BWRIGHT_PIN_SPEED,
            IR_SENSOR_BWRIGHT_PORT, IR_SENSOR_BWRIGHT_PORT_CLK);
}


/**
 * \fn      getIRSensor_BwRight
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_BwRight()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_BWRIGHT_PORT, IR_SENSOR_BWRIGHT_PIN);
}


/**
 * \fn      getIRSensorPosEdge_BwRight
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_BwRight(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_BWRIGHT_PORT,IR_SENSOR_BWRIGHT_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_BwRight
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_BwRight(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_BWRIGHT_PORT,IR_SENSOR_BWRIGHT_PIN, &sensor_state);
}


/**
 * \fn      initIRSensor_FwRight
 * \brief   IR sensor initialisation
 */
void initIRSensor_FwRight()
{
    initSensor(IR_SENSOR_FWRIGHT_PIN, IR_SENSOR_FWRIGHT_PIN_MODE, IR_SENSOR_FWRIGHT_PIN_TYPE, IR_SENSOR_FWRIGHT_PIN_PUPD, IR_SENSOR_FWRIGHT_PIN_SPEED,
            IR_SENSOR_FWRIGHT_PORT, IR_SENSOR_FWRIGHT_PORT_CLK);
}


/**
 * \fn      getIRSensor_FwRight
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_FwRight()
{
    return GPIO_ReadInputDataBit(IR_SENSOR_FWRIGHT_PORT, IR_SENSOR_FWRIGHT_PIN);
}


/**
 * \fn      getIRSensorPosEdge_FwRight
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_FwRight(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_FWRIGHT_PORT,IR_SENSOR_FWRIGHT_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_FwRight
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_FwRight(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_FWRIGHT_PORT,IR_SENSOR_FWRIGHT_PIN, &sensor_state);
}


/**
 * \fn      initSensor_Key
 * \brief   button S1 initialisation
 */
void initSensor_Key()
{
    initSensor(SENSOR_KEY_PIN, SENSOR_KEY_PIN_MODE, SENSOR_KEY_PIN_TYPE, SENSOR_KEY_PIN_PUPD, SENSOR_KEY_PIN_SPEED,
            SENSOR_KEY_PORT, SENSOR_KEY_PORT_CLK);
}


/**
 * \fn      getSensor_Key
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getSensor_Key()
{
    return !GPIO_ReadInputDataBit(SENSOR_KEY_PORT, SENSOR_KEY_PIN);
}


/**
 * \fn      getSensorPosEdge_Key
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_Key(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_KEY_PORT,SENSOR_KEY_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_Key
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_Key(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_KEY_PORT,SENSOR_KEY_PIN, &sensor_state);
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
void initSensor(uint32_t pin, GPIOMode_TypeDef mode, GPIOOType_TypeDef type,
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
 * \fn      getSensorPosEdge
 * \brief   calculate the positive edge based on the last button state
 *
 * \param   port    pin port
 * \param   pin     pin number
 * \param   sensor_state pointer to a pointer with the last sensor state
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge(GPIO_TypeDef* port, uint16_t pin, uint8_t** sensor_state)
{
    uint8_t last_sensor_state;

    last_sensor_state = **sensor_state;
    **sensor_state = !GPIO_ReadInputDataBit(port, pin);

    return (**sensor_state ^ last_sensor_state) & **sensor_state;
}


/**
 * \fn      getSensorNegEdge
 * \brief   calculate the negative edge based on the last button state
 *
 * \param   port    pin port
 * \param   pin     pin number
 * \param   sensor_state pointer to a pointer with the last button state
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge(GPIO_TypeDef* port, uint16_t pin, uint8_t** sensor_state)
{
    uint8_t last_sensor_state;

    last_sensor_state = **sensor_state;
    **sensor_state = !GPIO_ReadInputDataBit(port, pin);

    return (**sensor_state ^ last_sensor_state) & last_sensor_state;
}


/**
 * @}
 */
