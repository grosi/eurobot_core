/**
 * \file    ir_sensor.h
 * \author  gross10
 * \date    2014-02-26
 *
 * \version 1.1
 *  added interrupt defines (kasen1)
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
/* Pin and Port roboboard*/
#define IR_SENSOR_BACK_PIN         GPIO_Pin_5
#define IR_SENSOR_BACK_PIN_MODE    GPIO_Mode_IN
#define IR_SENSOR_BACK_PIN_SPEED   GPIO_Speed_2MHz
#define IR_SENSOR_BACK_PIN_TYPE    GPIO_OType_PP
#define IR_SENSOR_BACK_PIN_PUPD    GPIO_PuPd_UP
#define IR_SENSOR_BACK_PORT_CLK    RCC_AHB1Periph_GPIOB
#define IR_SENSOR_BACK_PORT        GPIOB
#define IR_SENSOR_BACK_EXTI_PORT   EXTI_PortSourceGPIOB
#define IR_SENSOR_BACK_EXTI_PIN    EXTI_PinSource5
#define IR_SENSOR_BACK_EXTI_LINE   EXTI_Line5
#define IR_SENSOR_BACK_EXTI_TRIG   EXTI_Trigger_Falling
#define IR_SENSOR_BACK_NVIC_CHAN   EXTI9_5_IRQn
#define IR_SENSOR_BACK_NVIC_PPRIO  0x01
#define IR_SENSOR_BACK_NVIC_SPRIO  0x01

#define IR_SENSOR_FRONT_PIN        GPIO_Pin_11
#define IR_SENSOR_FRONT_PIN_MODE   GPIO_Mode_IN
#define IR_SENSOR_FRONT_PIN_SPEED  GPIO_Speed_2MHz
#define IR_SENSOR_FRONT_PIN_TYPE   GPIO_OType_PP
#define IR_SENSOR_FRONT_PIN_PUPD   GPIO_PuPd_UP
#define IR_SENSOR_FRONT_PORT_CLK   RCC_AHB1Periph_GPIOB
#define IR_SENSOR_FRONT_PORT       GPIOB
#define IR_SENSOR_FRONT_EXTI_PORT  EXTI_PortSourceGPIOB
#define IR_SENSOR_FRONT_EXTI_PIN   EXTI_PinSource11
#define IR_SENSOR_FRONT_EXTI_LINE  EXTI_Line11
#define IR_SENSOR_FRONT_EXTI_TRIG  EXTI_Trigger_Falling
#define IR_SENSOR_FRONT_NVIC_CHAN  EXTI15_10_IRQn
#define IR_SENSOR_FRONT_NVIC_PPRIO 0x01
#define IR_SENSOR_FRONT_NVIC_SPRIO 0x01

#define IR_SENSOR_LEFT_PIN         GPIO_Pin_15
#define IR_SENSOR_LEFT_PIN_MODE    GPIO_Mode_IN
#define IR_SENSOR_LEFT_PIN_SPEED   GPIO_Speed_2MHz
#define IR_SENSOR_LEFT_PIN_TYPE    GPIO_OType_PP
#define IR_SENSOR_LEFT_PIN_PUPD    GPIO_PuPd_UP
#define IR_SENSOR_LEFT_PORT_CLK    RCC_AHB1Periph_GPIOE
#define IR_SENSOR_LEFT_PORT        GPIOE
#define IR_SENSOR_LEFT_EXTI_PORT   EXTI_PortSourceGPIOE
#define IR_SENSOR_LEFT_EXTI_PIN    EXTI_PinSource15
#define IR_SENSOR_LEFT_EXTI_LINE   EXTI_Line15
#define IR_SENSOR_LEFT_EXTI_TRIG   EXTI_Trigger_Falling
#define IR_SENSOR_LEFT_NVIC_CHAN   EXTI15_10_IRQn
#define IR_SENSOR_LEFT_NVIC_PPRIO  0x01
#define IR_SENSOR_LEFT_NVIC_SPRIO  0x01

#define IR_SENSOR_RIGHT_PIN        GPIO_Pin_12
#define IR_SENSOR_RIGHT_PIN_MODE   GPIO_Mode_IN
#define IR_SENSOR_RIGHT_PIN_SPEED  GPIO_Speed_2MHz
#define IR_SENSOR_RIGHT_PIN_TYPE   GPIO_OType_PP
#define IR_SENSOR_RIGHT_PIN_PUPD   GPIO_PuPd_UP
#define IR_SENSOR_RIGHT_PORT_CLK   RCC_AHB1Periph_GPIOB
#define IR_SENSOR_RIGHT_PORT       GPIOB
#define IR_SENSOR_RIGHT_EXTI_PORT  EXTI_PortSourceGPIOB
#define IR_SENSOR_RIGHT_EXTI_PIN   EXTI_PinSource12
#define IR_SENSOR_RIGHT_EXTI_LINE  EXTI_Line12
#define IR_SENSOR_RIGHT_EXTI_TRIG  EXTI_Trigger_Falling
#define IR_SENSOR_RIGHT_NVIC_CHAN  EXTI15_10_IRQn
#define IR_SENSOR_RIGHT_NVIC_PPRIO 0x01
#define IR_SENSOR_RIGHT_NVIC_SPRIO 0x01

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
void initIRSensors();
inline uint8_t getIRSensor_Back();
inline uint8_t getIRSensor_Front();
inline uint8_t getIRSensor_Left();
inline uint8_t getIRSensor_Right();
void initIREXTILines();

#endif /* IR_SENSOR_H_ */

/**
 * @}
 */
