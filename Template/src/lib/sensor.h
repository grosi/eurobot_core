/**
 * \file    sensor.h
 * \author  gross10
 * \date    2014-02-11
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Sensor access for the roboboard. This libary is only for the small
 *          roboter.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SENSOR_H_
#define SENSOR_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
/* Pin and Port roboboard*/
#define SENSOR_KEY_PIN       GPIO_Pin_5
#define SENSOR_KEY_PIN_MODE  GPIO_Mode_IN
#define SENSOR_KEY_PIN_SPEED GPIO_Speed_2MHz
#define SENSOR_KEY_PIN_TYPE  GPIO_OType_PP
#define SENSOR_KEY_PIN_PUPD  GPIO_PuPd_UP
#define SENSOR_KEY_PORT_CLK  RCC_AHB1Periph_GPIOC
#define SENSOR_KEY_PORT      GPIOC

#define SENSOR_FRESCO_1_PIN       GPIO_Pin_4
#define SENSOR_FRESCO_1_PIN_MODE  GPIO_Mode_IN
#define SENSOR_FRESCO_1_PIN_SPEED GPIO_Speed_2MHz
#define SENSOR_FRESCO_1_PIN_TYPE  GPIO_OType_PP
#define SENSOR_FRESCO_1_PIN_PUPD  GPIO_PuPd_UP
#define SENSOR_FRESCO_1_PORT_CLK  RCC_AHB1Periph_GPIOC
#define SENSOR_FRESCO_1_PORT      GPIOC

#define SENSOR_FRESCO_2_PIN       GPIO_Pin_1
#define SENSOR_FRESCO_2_PIN_MODE  GPIO_Mode_IN
#define SENSOR_FRESCO_2_PIN_SPEED GPIO_Speed_2MHz
#define SENSOR_FRESCO_2_PIN_TYPE  GPIO_OType_PP
#define SENSOR_FRESCO_2_PIN_PUPD  GPIO_PuPd_UP
#define SENSOR_FRESCO_2_PORT_CLK  RCC_AHB1Periph_GPIOB
#define SENSOR_FRESCO_2_PORT      GPIOB

#define SENSOR_FRESCO_WALL_PIN       GPIO_Pin_0
#define SENSOR_FRESCO_WALL_PIN_MODE  GPIO_Mode_IN
#define SENSOR_FRESCO_WALL_PIN_SPEED GPIO_Speed_2MHz
#define SENSOR_FRESCO_WALL_PIN_TYPE  GPIO_OType_PP
#define SENSOR_FRESCO_WALL_PIN_PUPD  GPIO_PuPd_UP
#define SENSOR_FRESCO_WALL_PORT_CLK  RCC_AHB1Periph_GPIOB
#define SENSOR_FRESCO_WALL_PORT      GPIOB

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
inline void initSensor_Key();
inline uint8_t getSensor_Key();
inline void initSensor_Fresco_1();
inline uint8_t getSensor_Fresco_1();
inline uint8_t getSensorPosEdge_Fresco_1(uint8_t*);
inline uint8_t getSensorNegEdge_Fresco_1(uint8_t*);
inline void initButton_Fresco_2();
inline uint8_t getSensor_Fresco_2();
inline uint8_t getSensorPosEdge_Fresco_2(uint8_t*);
inline uint8_t SensorNegEdge_Frecko_2(uint8_t*);
inline void initSensor_Fresco_Wall();
inline uint8_t getSensor_Fresco_Wall();
inline uint8_t getSensorPosEdge_Fresco_Wall(uint8_t*);
inline uint8_t getSensorNegEdge_Fresco_Wall(uint8_t*);

#endif /* SENSOR_H_ */

/**
 * @}
 */
