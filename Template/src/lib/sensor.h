/**
 * \file    sensor.h
 * \author  gross10
 * \date    2014-02-11
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Sensor access for the roboboard and user-panel. This libary is only for the small
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
#define SENSOR_EMERGENCYSTOP_PIN        GPIO_Pin_0
#define SENSOR_EMERGENCYSTOP_PIN_MODE   GPIO_Mode_IN
#define SENSOR_EMERGENCYSTOP_PIN_SPEED  GPIO_Speed_2MHz
#define SENSOR_EMERGENCYSTOP_PIN_TYPE   GPIO_OType_PP
#define SENSOR_EMERGENCYSTOP_PIN_PUPD   GPIO_PuPd_UP
#define SENSOR_EMERGENCYSTOP_PORT_CLK   RCC_AHB1Periph_GPIOB
#define SENSOR_EMERGENCYSTOP_PORT       GPIOB
#define SENSOR_EMERGENCYSTOP_EXTI_PORT  EXTI_PortSourceGPIOB
#define SENSOR_EMERGENCYSTOP_EXTI_PIN   EXTI_PinSource0
#define SENSOR_EMERGENCYSTOP_EXTI_LINE  EXTI_Line0
#define SENSOR_EMERGENCYSTOP_EXTI_TRIG  EXTI_Trigger_Rising_Falling
#define SENSOR_EMERGENCYSTOP_EXTI_CHAN  EXTI0_IRQn
#define SENSOR_EMERGENCYSTOP_EXTI_PPRIO 1
#define SENSOR_EMERGENCYSTOP_EXTI_SPRIO 0

#define SENSOR_FRESCO_1_PIN             GPIO_Pin_5
#define SENSOR_FRESCO_1_PIN_MODE        GPIO_Mode_IN
#define SENSOR_FRESCO_1_PIN_SPEED       GPIO_Speed_2MHz
#define SENSOR_FRESCO_1_PIN_TYPE        GPIO_OType_PP
#define SENSOR_FRESCO_1_PIN_PUPD        GPIO_PuPd_UP
#define SENSOR_FRESCO_1_PORT_CLK        RCC_AHB1Periph_GPIOC
#define SENSOR_FRESCO_1_PORT            GPIOC
#define SENSOR_FRESCO_1_EXTI_PORT       EXTI_PortSourceGPIOC
#define SENSOR_FRESCO_1_EXTI_PIN        EXTI_PinSource5
#define SENSOR_FRESCO_1_EXTI_LINE       EXTI_Line5
#define SENSOR_FRESCO_1_EXTI_TRIG       EXTI_Trigger_Rising_Falling
#define SENSOR_FRESCO_1_EXTI_CHAN       EXTI9_5_IRQn
#define SENSOR_FRESCO_1_EXTI_PPRIO      1
#define SENSOR_FRESCO_1_EXTI_SPRIO      0

#define SENSOR_FRESCO_2_PIN             GPIO_Pin_4
#define SENSOR_FRESCO_2_PIN_MODE        GPIO_Mode_IN
#define SENSOR_FRESCO_2_PIN_SPEED       GPIO_Speed_2MHz
#define SENSOR_FRESCO_2_PIN_TYPE        GPIO_OType_PP
#define SENSOR_FRESCO_2_PIN_PUPD        GPIO_PuPd_UP
#define SENSOR_FRESCO_2_PORT_CLK        RCC_AHB1Periph_GPIOC
#define SENSOR_FRESCO_2_PORT            GPIOC
#define SENSOR_FRESCO_2_EXTI_PORT       EXTI_PortSourceGPIOC
#define SENSOR_FRESCO_2_EXTI_PIN        EXTI_PinSource4
#define SENSOR_FRESCO_2_EXTI_LINE       EXTI_Line4
#define SENSOR_FRESCO_2_EXTI_TRIG       EXTI_Trigger_Rising_Falling
#define SENSOR_FRESCO_2_EXTI_CHAN       EXTI4_IRQn
#define SENSOR_FRESCO_2_EXTI_PPRIO      1
#define SENSOR_FRESCO_2_EXTI_SPRIO      0

#define SENSOR_FRESCO_WALL_PIN          GPIO_Pin_1
#define SENSOR_FRESCO_WALL_PIN_MODE     GPIO_Mode_IN
#define SENSOR_FRESCO_WALL_PIN_SPEED    GPIO_Speed_2MHz
#define SENSOR_FRESCO_WALL_PIN_TYPE     GPIO_OType_PP
#define SENSOR_FRESCO_WALL_PIN_PUPD     GPIO_PuPd_UP
#define SENSOR_FRESCO_WALL_PORT_CLK     RCC_AHB1Periph_GPIOB
#define SENSOR_FRESCO_WALL_PORT         GPIOB
#define SENSOR_FRESCO_WALL_EXTI_PORT    EXTI_PortSourceGPIOB
#define SENSOR_FRESCO_WALL_EXTI_PIN     EXTI_PinSource1
#define SENSOR_FRESCO_WALL_EXTI_LINE    EXTI_Line1
#define SENSOR_FRESCO_WALL_EXTI_TRIG    EXTI_Trigger_Rising_Falling
#define SENSOR_FRESCO_WALL_EXTI_CHAN    EXTI1_IRQn
#define SENSOR_FRESCO_WALL_EXTI_PPRIO   1
#define SENSOR_FRESCO_WALL_EXTI_SPRIO   0

#define IR_SENSOR_BACK_PIN              GPIO_Pin_5
#define IR_SENSOR_BACK_PIN_MODE         GPIO_Mode_IN
#define IR_SENSOR_BACK_PIN_SPEED        GPIO_Speed_2MHz
#define IR_SENSOR_BACK_PIN_TYPE         GPIO_OType_PP
#define IR_SENSOR_BACK_PIN_PUPD         GPIO_PuPd_UP
#define IR_SENSOR_BACK_PORT_CLK         RCC_AHB1Periph_GPIOB
#define IR_SENSOR_BACK_PORT             GPIOB
#define IR_SENSOR_BACK_EXTI_PORT        EXTI_PortSourceGPIOB
#define IR_SENSOR_BACK_EXTI_PIN         EXTI_PinSource5
#define IR_SENSOR_BACK_EXTI_LINE        EXTI_Line5
#define IR_SENSOR_BACK_EXTI_TRIG        EXTI_Trigger_Rising_Falling
#define IR_SENSOR_BACK_NVIC_CHAN        EXTI9_5_IRQn
#define IR_SENSOR_BACK_NVIC_PPRIO       1
#define IR_SENSOR_BACK_NVIC_SPRIO       1

#define IR_SENSOR_FRONT_PIN             GPIO_Pin_11
#define IR_SENSOR_FRONT_PIN_MODE        GPIO_Mode_IN
#define IR_SENSOR_FRONT_PIN_SPEED       GPIO_Speed_2MHz
#define IR_SENSOR_FRONT_PIN_TYPE        GPIO_OType_PP
#define IR_SENSOR_FRONT_PIN_PUPD        GPIO_PuPd_UP
#define IR_SENSOR_FRONT_PORT_CLK        RCC_AHB1Periph_GPIOB
#define IR_SENSOR_FRONT_PORT            GPIOB
#define IR_SENSOR_FRONT_EXTI_PORT       EXTI_PortSourceGPIOB
#define IR_SENSOR_FRONT_EXTI_PIN        EXTI_PinSource11
#define IR_SENSOR_FRONT_EXTI_LINE       EXTI_Line11
#define IR_SENSOR_FRONT_EXTI_TRIG       EXTI_Trigger_Rising_Falling
#define IR_SENSOR_FRONT_NVIC_CHAN       EXTI15_10_IRQn
#define IR_SENSOR_FRONT_NVIC_PPRIO      0x01
#define IR_SENSOR_FRONT_NVIC_SPRIO      0x01

#define IR_SENSOR_LEFT_PIN              GPIO_Pin_15
#define IR_SENSOR_LEFT_PIN_MODE         GPIO_Mode_IN
#define IR_SENSOR_LEFT_PIN_SPEED        GPIO_Speed_2MHz
#define IR_SENSOR_LEFT_PIN_TYPE         GPIO_OType_PP
#define IR_SENSOR_LEFT_PIN_PUPD         GPIO_PuPd_UP
#define IR_SENSOR_LEFT_PORT_CLK         RCC_AHB1Periph_GPIOE
#define IR_SENSOR_LEFT_PORT             GPIOE
#define IR_SENSOR_LEFT_EXTI_PORT        EXTI_PortSourceGPIOE
#define IR_SENSOR_LEFT_EXTI_PIN         EXTI_PinSource15
#define IR_SENSOR_LEFT_EXTI_LINE        EXTI_Line15
#define IR_SENSOR_LEFT_EXTI_TRIG        EXTI_Trigger_Rising_Falling
#define IR_SENSOR_LEFT_NVIC_CHAN        EXTI15_10_IRQn
#define IR_SENSOR_LEFT_NVIC_PPRIO       0x01
#define IR_SENSOR_LEFT_NVIC_SPRIO       0x01

#define IR_SENSOR_RIGHT_PIN             GPIO_Pin_12
#define IR_SENSOR_RIGHT_PIN_MODE        GPIO_Mode_IN
#define IR_SENSOR_RIGHT_PIN_SPEED       GPIO_Speed_2MHz
#define IR_SENSOR_RIGHT_PIN_TYPE        GPIO_OType_PP
#define IR_SENSOR_RIGHT_PIN_PUPD        GPIO_PuPd_UP
#define IR_SENSOR_RIGHT_PORT_CLK        RCC_AHB1Periph_GPIOB
#define IR_SENSOR_RIGHT_PORT            GPIOB
#define IR_SENSOR_RIGHT_EXTI_PORT       EXTI_PortSourceGPIOB
#define IR_SENSOR_RIGHT_EXTI_PIN        EXTI_PinSource12
#define IR_SENSOR_RIGHT_EXTI_LINE       EXTI_Line12
#define IR_SENSOR_RIGHT_EXTI_TRIG       EXTI_Trigger_Rising_Falling
#define IR_SENSOR_RIGHT_NVIC_CHAN       EXTI15_10_IRQn
#define IR_SENSOR_RIGHT_NVIC_PPRIO      0x01
#define IR_SENSOR_RIGHT_NVIC_SPRIO      0x01

/* Pin and Port user panel */
#define SENSOR_KEY_PIN                  GPIO_Pin_7
#define SENSOR_KEY_PIN_MODE             GPIO_Mode_IN
#define SENSOR_KEY_PIN_SPEED            GPIO_Speed_2MHz
#define SENSOR_KEY_PIN_TYPE             GPIO_OType_PP
#define SENSOR_KEY_PIN_PUPD             GPIO_PuPd_UP
#define SENSOR_KEY_PORT_CLK             RCC_AHB1Periph_GPIOB
#define SENSOR_KEY_PORT                 GPIOB
#define SENSOR_KEY_EXTI_PORT            EXTI_PortSourceGPIOB
#define SENSOR_KEY_EXTI_PIN             EXTI_PinSource7
#define SENSOR_KEY_EXTI_LINE            EXTI_Line7
#define SENSOR_KEY_EXTI_TRIG            EXTI_Trigger_Rising_Falling
#define SENSOR_KEY_EXTI_CHAN            EXTI9_5_IRQn
#define SENSOR_KEY_EXTI_PPRIO           1
#define SENSOR_KEY_EXTI_SPRIO           0

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initSensor_EmergencyStop();
extern uint8_t getSensor_EmergencyStop();
extern uint8_t getSensorPosEdge_EmergencyStop(uint8_t*);
extern uint8_t getSensorNegEdge_EmergencyStop(uint8_t*);
extern void initSensor_Fresco_1();
extern uint8_t getSensor_Fresco_1();
extern uint8_t getSensorPosEdge_Fresco_1(uint8_t*);
extern uint8_t getSensorNegEdge_Fresco_1(uint8_t*);
extern void initButton_Fresco_2();
extern uint8_t getSensor_Fresco_2();
extern uint8_t getSensorPosEdge_Fresco_2(uint8_t*);
extern uint8_t SensorNegEdge_Freco_2(uint8_t*);
extern void initSensor_Fresco_Wall();
extern uint8_t getSensor_Fresco_Wall();
extern uint8_t getSensorPosEdge_Fresco_Wall(uint8_t*);
extern uint8_t getSensorNegEdge_Fresco_Wall(uint8_t*);
extern void initIRSensor_Back();
extern uint8_t getIRSensor_Back();
extern uint8_t getIRSensorPosEdge_Back(uint8_t*);
extern uint8_t getIRSensorNegEdge_Back(uint8_t*);
extern void initIRSensor_Front();
extern uint8_t getIRSensor_Front();
extern uint8_t getIRSensorPosEdge_Front(uint8_t*);
extern uint8_t getIRSensorNegEdge_Front(uint8_t*);
extern void initIRSensor_Left();
extern uint8_t getIRSensor_Left();
extern uint8_t getIRSensorPosEdge_Left(uint8_t*);
extern uint8_t getIRSensorNegEdge_Left(uint8_t*);
extern void initIRSensor_Right();
extern uint8_t getIRSensor_Right();
extern uint8_t getIRSensorPosEdge_Right(uint8_t*);
extern uint8_t getIRSensorNegEdge_Right(uint8_t*);
extern void initSensor_Key();
extern uint8_t getSensor_Key();
extern uint8_t getSensorPosEdge_Key(uint8_t*);
extern uint8_t getSensorNegEdge_Key(uint8_t*);

#endif /* SENSOR_H_ */

/**
 * @}
 */
