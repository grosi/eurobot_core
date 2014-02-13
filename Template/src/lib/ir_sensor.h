/**
 * \file    ir_sensor.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
/* Pin and Port roboboard*/
#define IR_SENSOR_BACK_PIN          GPIO_Pin_5
#define IR_SENSOR_BACK_PIN_MODE     GPIO_Mode_IN
#define IR_SENSOR_BACK_PIN_SPEED    GPIO_Speed_2MHz
#define IR_SENSOR_BACK_PIN_TYPE     GPIO_OType_PP
#define IR_SENSOR_BACK_PIN_PUPD     GPIO_PuPd_UP
#define IR_SENSOR_BACK_PORT_CLK     RCC_AHB1Periph_GPIOB
#define IR_SENSOR_BACK_PORT         GPIOB

#define IR_SENSOR_FRONT_PIN       GPIO_Pin_11
#define IR_SENSOR_FRONT_PIN_MODE  GPIO_Mode_IN
#define IR_SENSOR_FRONT_PIN_SPEED GPIO_Speed_2MHz
#define IR_SENSOR_FRONT_PIN_TYPE  GPIO_OType_PP
#define IR_SENSOR_FRONT_PIN_PUPD  GPIO_PuPd_UP
#define IR_SENSOR_FRONT_PORT_CLK  RCC_AHB1Periph_GPIOB
#define IR_SENSOR_FRONT_PORT      GPIOB

#define IR_SENSOR_LEFT_PIN       GPIO_Pin_15
#define IR_SENSOR_LEFT_PIN_MODE  GPIO_Mode_IN
#define IR_SENSOR_LEFT_PIN_SPEED GPIO_Speed_2MHz
#define IR_SENSOR_LEFT_PIN_TYPE  GPIO_OType_PP
#define IR_SENSOR_LEFT_PIN_PUPD  GPIO_PuPd_UP
#define IR_SENSOR_LEFT_PORT_CLK  RCC_AHB1Periph_GPIOE
#define IR_SENSOR_LEFT_PORT      GPIOE

#define IR_SENSOR_RIGHT_PIN       GPIO_Pin_12
#define IR_SENSOR_RIGHT_PIN_MODE  GPIO_Mode_IN
#define IR_SENSOR_RIGHT_PIN_SPEED GPIO_Speed_2MHz
#define IR_SENSOR_RIGHT_PIN_TYPE  GPIO_OType_PP
#define IR_SENSOR_RIGHT_PIN_PUPD  GPIO_PuPd_UP
#define IR_SENSOR_RIGHT_PORT_CLK  RCC_AHB1Periph_GPIOB
#define IR_SENSOR_RIGHT_PORT      GPIOB

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
void initIRSensors();
inline uint8_t getIRSensor_Back();
inline uint8_t getIRSensor_Front();
inline uint8_t getIRSensor_Left();
inline uint8_t getIRSensor_Right();

#endif /* IR_SENSOR_H_ */

/**
 * @}
 */
