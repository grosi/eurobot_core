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
 * \fn      initSensor_Fresco_1
 * \brief   sensor fresco 1 initialisation
 */
void initSensor_Fresco_1()
{
    initSensor(SENSOR_FRESCO_1_PIN, SENSOR_FRESCO_1_PIN_MODE, SENSOR_FRESCO_1_PIN_TYPE, SENSOR_FRESCO_1_PIN_PUPD, SENSOR_FRESCO_1_PIN_SPEED,
            SENSOR_FRESCO_1_PORT, SENSOR_FRESCO_1_PORT_CLK);
}


/**
 * \fn      getSensor_Fresco_1
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getSensor_Fresco_1()
{
    return !GPIO_ReadInputDataBit(SENSOR_FRESCO_1_PORT, SENSOR_FRESCO_1_PIN);
}


/**
 * \fn      getSensorPosEdge_Fresco_1
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_Fresco_1(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_FRESCO_1_PORT,SENSOR_FRESCO_1_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_Fresco_1
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_Fresco_1(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_FRESCO_1_PORT,SENSOR_FRESCO_1_PIN, &sensor_state);
}


/**
 * \fn      initSensor_Fresco_2
 * \brief   sensor fresco 2 initialisation
 */
void initSensor_Fresco_2()
{
    initSensor(SENSOR_FRESCO_2_PIN, SENSOR_FRESCO_2_PIN_MODE, SENSOR_FRESCO_2_PIN_TYPE, SENSOR_FRESCO_2_PIN_PUPD, SENSOR_FRESCO_2_PIN_SPEED,
            SENSOR_FRESCO_2_PORT, SENSOR_FRESCO_2_PORT_CLK);
}


/**
 * \fn      getSensor_Fresco_2
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getSensor_Fresco_2()
{
    return !GPIO_ReadInputDataBit(SENSOR_FRESCO_2_PORT, SENSOR_FRESCO_2_PIN);
}


/**
 * \fn      getSensorPosEdge_Fresco_2
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_Fresco_2(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_FRESCO_2_PORT,SENSOR_FRESCO_2_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_Fresco_2
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_Fresco_2(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_FRESCO_2_PORT,SENSOR_FRESCO_2_PIN, &sensor_state);
}


/**
 * \fn      initSensor_Fresco_Wall
 * \brief   sensor fresco 1 initialisation
 */
void initSensor_Fresco_Wall()
{
    initSensor(SENSOR_FRESCO_WALL_PIN, SENSOR_FRESCO_WALL_PIN_MODE, SENSOR_FRESCO_WALL_PIN_TYPE, SENSOR_FRESCO_WALL_PIN_PUPD, SENSOR_FRESCO_WALL_PIN_SPEED,
            SENSOR_FRESCO_WALL_PORT, SENSOR_FRESCO_WALL_PORT_CLK);
}


/**
 * \fn      getSensor_Fresco_Wall
 * \brief   get the current pin value
 * \note    the pin is not debounced
 * \return  current button value
 */
uint8_t getSensor_Fresco_Wall()
{
    return !GPIO_ReadInputDataBit(SENSOR_FRESCO_WALL_PORT, SENSOR_FRESCO_WALL_PIN);
}


/**
 * \fn      getSensorPosEdge_Fresco_Wall
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getSensorPosEdge_Fresco_Wall(uint8_t* sensor_state)
{
    return getSensorPosEdge(SENSOR_FRESCO_WALL_PORT,SENSOR_FRESCO_WALL_PIN, &sensor_state);
}


/**
 * \fn      getSensorNegEdge_Fresco_Wall
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getSensorNegEdge_Fresco_Wall(uint8_t* sensor_state)
{
    return getSensorNegEdge(SENSOR_FRESCO_WALL_PORT,SENSOR_FRESCO_WALL_PIN, &sensor_state);
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
 * \fn      initIRSensor_Back
 * \brief   IR sensor initialisation
 */
void initIRSensor_Back()
{
    initSensor(IR_SENSOR_BACK_PIN, IR_SENSOR_BACK_PIN_MODE, IR_SENSOR_BACK_PIN_TYPE, IR_SENSOR_BACK_PIN_PUPD, IR_SENSOR_BACK_PIN_SPEED,
            IR_SENSOR_BACK_PORT, IR_SENSOR_BACK_PORT_CLK);
}


/**
 * \fn      getIRSensor_Back
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_Back()
{
    return !GPIO_ReadInputDataBit(IR_SENSOR_BACK_PORT, IR_SENSOR_BACK_PIN);
}


/**
 * \fn      getIRSensorPosEdge_Back
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_Back(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_BACK_PORT,IR_SENSOR_BACK_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_Back
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_Back(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_BACK_PORT,IR_SENSOR_BACK_PIN, &sensor_state);
}


/**
 * \fn      initIRSensor_Front
 * \brief   IR sensor initialisation
 */
void initIRSensor_Front()
{
    initSensor(IR_SENSOR_FRONT_PIN, IR_SENSOR_FRONT_PIN_MODE, IR_SENSOR_FRONT_PIN_TYPE, IR_SENSOR_FRONT_PIN_PUPD, IR_SENSOR_FRONT_PIN_SPEED,
            IR_SENSOR_FRONT_PORT, IR_SENSOR_FRONT_PORT_CLK);
}


/**
 * \fn      getIRSensor_Front
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_Front()
{
    return !GPIO_ReadInputDataBit(IR_SENSOR_FRONT_PORT, IR_SENSOR_FRONT_PIN);
}


/**
 * \fn      getIRSensorPosEdge_Front
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_Front(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_FRONT_PORT,IR_SENSOR_FRONT_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_Front
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_Front(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_FRONT_PORT,IR_SENSOR_FRONT_PIN, &sensor_state);
}


/**
 * \fn      initIRSensor_Left
 * \brief   IR sensor initialisation
 */
void initIRSensor_Left()
{
    initSensor(IR_SENSOR_LEFT_PIN, IR_SENSOR_LEFT_PIN_MODE, IR_SENSOR_LEFT_PIN_TYPE, IR_SENSOR_LEFT_PIN_PUPD, IR_SENSOR_LEFT_PIN_SPEED,
            IR_SENSOR_LEFT_PORT, IR_SENSOR_LEFT_PORT_CLK);
}


/**
 * \fn      getIRSensor_Left
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_Left()
{
    return !GPIO_ReadInputDataBit(IR_SENSOR_LEFT_PORT, IR_SENSOR_LEFT_PIN);
}


/**
 * \fn      getIRSensorPosEdge_Left
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_Left(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_LEFT_PORT,IR_SENSOR_LEFT_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_Left
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_Left(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_LEFT_PORT,IR_SENSOR_LEFT_PIN, &sensor_state);
}


/**
 * \fn      initIRSensor_Right
 * \brief   IR sensor initialisation
 */
void initIRSensor_Right()
{
    initSensor(IR_SENSOR_RIGHT_PIN, IR_SENSOR_RIGHT_PIN_MODE, IR_SENSOR_RIGHT_PIN_TYPE, IR_SENSOR_RIGHT_PIN_PUPD, IR_SENSOR_RIGHT_PIN_SPEED,
            IR_SENSOR_RIGHT_PORT, IR_SENSOR_RIGHT_PORT_CLK);
}


/**
 * \fn      getIRSensor_Right
 * \brief   get the current pin value
 * \note    the pin is not debounced, high active
 * \return  current button value
 */
uint8_t getIRSensor_Right()
{
    return !GPIO_ReadInputDataBit(IR_SENSOR_RIGHT_PORT, IR_SENSOR_RIGHT_PIN);
}


/**
 * \fn      getIRSensorPosEdge_Right
 * \brief   get the positive edge
 * \note    this function have to call every programm loop
 *
 * \param   sensor_state    pointer to a temporary button state memory
 * \return  the positive edge, if there
 */
uint8_t getIRSensorPosEdge_Right(uint8_t* sensor_state)
{
    return getSensorPosEdge(IR_SENSOR_RIGHT_PORT,IR_SENSOR_RIGHT_PIN, &sensor_state);
}


/**
 * \fn      getIRSensorNegEdge_Right
 * \brief   get the negative edge
 * \note    this function have to call every programm loop
 *
 * \param   button_state    pointer to a temporary button state memory
 * \return  the neagtive edge, if there
 */
uint8_t getIRSensorNegEdge_Right(uint8_t* sensor_state)
{
    return getSensorNegEdge(IR_SENSOR_RIGHT_PORT,IR_SENSOR_RIGHT_PIN, &sensor_state);
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
