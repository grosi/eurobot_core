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
#define SENSOR_EMERGENCYSTOP_EXTI_TRIG  EXTI_Trigger_Rising
#define SENSOR_EMERGENCYSTOP_EXTI_CHAN  EXTI0_IRQn
#define SENSOR_EMERGENCYSTOP_EXTI_PPRIO 5
#define SENSOR_EMERGENCYSTOP_EXTI_SPRIO 0

#define SENSOR_FIRE_POOL_LEFT_PIN             GPIO_Pin_5
#define SENSOR_FIRE_POOL_LEFT_PIN_MODE        GPIO_Mode_IN
#define SENSOR_FIRE_POOL_LEFT_PIN_SPEED       GPIO_Speed_2MHz
#define SENSOR_FIRE_POOL_LEFT_PIN_TYPE        GPIO_OType_PP
#define SENSOR_FIRE_POOL_LEFT_PIN_PUPD        GPIO_PuPd_UP
#define SENSOR_FIRE_POOL_LEFT_PORT_CLK        RCC_AHB1Periph_GPIOC
#define SENSOR_FIRE_POOL_LEFT_PORT            GPIOC
#define SENSOR_FIRE_POOL_LEFT_EXTI_PORT       EXTI_PortSourceGPIOC
#define SENSOR_FIRE_POOL_LEFT_EXTI_PIN        EXTI_PinSource5
#define SENSOR_FIRE_POOL_LEFT_EXTI_LINE       EXTI_Line5
#define SENSOR_FIRE_POOL_LEFT_EXTI_TRIG       EXTI_Trigger_Falling
#define SENSOR_FIRE_POOL_LEFT_EXTI_CHAN       EXTI9_5_IRQn
#define SENSOR_FIRE_POOL_LEFT_EXTI_PPRIO      5
#define SENSOR_FIRE_POOL_LEFT_EXTI_SPRIO      0

#define SENSOR_FIRE_POOL_RIGHT_PIN             GPIO_Pin_4
#define SENSOR_FIRE_POOL_RIGHT_PIN_MODE        GPIO_Mode_IN
#define SENSOR_FIRE_POOL_RIGHT_PIN_SPEED       GPIO_Speed_2MHz
#define SENSOR_FIRE_POOL_RIGHT_PIN_TYPE        GPIO_OType_PP
#define SENSOR_FIRE_POOL_RIGHT_PIN_PUPD        GPIO_PuPd_UP
#define SENSOR_FIRE_POOL_RIGHT_PORT_CLK        RCC_AHB1Periph_GPIOC
#define SENSOR_FIRE_POOL_RIGHT_PORT            GPIOC
#define SENSOR_FIRE_POOL_RIGHT_EXTI_PORT       EXTI_PortSourceGPIOC
#define SENSOR_FIRE_POOL_RIGHT_EXTI_PIN        EXTI_PinSource4
#define SENSOR_FIRE_POOL_RIGHT_EXTI_LINE       EXTI_Line4
#define SENSOR_FIRE_POOL_RIGHT_EXTI_TRIG       EXTI_Trigger_Falling
#define SENSOR_FIRE_POOL_RIGHT_EXTI_CHAN       EXTI4_IRQn
#define SENSOR_FIRE_POOL_RIGHT_EXTI_PPRIO      5
#define SENSOR_FIRE_POOL_RIGHT_EXTI_SPRIO      0

#define SENSOR_AIR_PIN          GPIO_Pin_1
#define SENSOR_AIR_PIN_MODE     GPIO_Mode_IN
#define SENSOR_AIR_PIN_SPEED    GPIO_Speed_2MHz
#define SENSOR_AIR_PIN_TYPE     GPIO_OType_PP
#define SENSOR_AIR_PIN_PUPD     GPIO_PuPd_UP
#define SENSOR_AIR_PORT_CLK     RCC_AHB1Periph_GPIOB
#define SENSOR_AIR_PORT         GPIOB
#define SENSOR_AIR_EXTI_PORT    EXTI_PortSourceGPIOB
#define SENSOR_AIR_EXTI_PIN     EXTI_PinSource1
#define SENSOR_AIR_EXTI_LINE    EXTI_Line1
#define SENSOR_AIR_EXTI_TRIG    EXTI_Trigger_Falling
#define SENSOR_AIR_EXTI_CHAN    EXTI1_IRQn
#define SENSOR_AIR_EXTI_PPRIO   5
#define SENSOR_AIR_EXTI_SPRIO   0

#define IR_SENSOR_FWLEFT_PIN             GPIO_Pin_11
#define IR_SENSOR_FWLEFT_PIN_MODE        GPIO_Mode_IN
#define IR_SENSOR_FWLEFT_PIN_SPEED       GPIO_Speed_2MHz
#define IR_SENSOR_FWLEFT_PIN_TYPE        GPIO_OType_PP
#define IR_SENSOR_FWLEFT_PIN_PUPD        GPIO_PuPd_UP
#define IR_SENSOR_FWLEFT_PORT_CLK        RCC_AHB1Periph_GPIOB
#define IR_SENSOR_FWLEFT_PORT            GPIOB
#define IR_SENSOR_FWLEFT_EXTI_PORT       EXTI_PortSourceGPIOB
#define IR_SENSOR_FWLEFT_EXTI_PIN        EXTI_PinSource11
#define IR_SENSOR_FWLEFT_EXTI_LINE       EXTI_Line11
#define IR_SENSOR_FWLEFT_EXTI_TRIG       EXTI_Trigger_Rising_Falling
#define IR_SENSOR_FWLEFT_NVIC_CHAN       EXTI15_10_IRQn
#define IR_SENSOR_FWLEFT_NVIC_PPRIO      5
#define IR_SENSOR_FWLEFT_NVIC_SPRIO      0

#define IR_SENSOR_FWRIGHT_PIN             GPIO_Pin_12
#define IR_SENSOR_FWRIGHT_PIN_MODE        GPIO_Mode_IN
#define IR_SENSOR_FWRIGHT_PIN_SPEED       GPIO_Speed_2MHz
#define IR_SENSOR_FWRIGHT_PIN_TYPE        GPIO_OType_PP
#define IR_SENSOR_FWRIGHT_PIN_PUPD        GPIO_PuPd_UP
#define IR_SENSOR_FWRIGHT_PORT_CLK        RCC_AHB1Periph_GPIOB
#define IR_SENSOR_FWRIGHT_PORT            GPIOB
#define IR_SENSOR_FWRIGHT_EXTI_PORT       EXTI_PortSourceGPIOB
#define IR_SENSOR_FWRIGHT_EXTI_PIN        EXTI_PinSource12
#define IR_SENSOR_FWRIGHT_EXTI_LINE       EXTI_Line12
#define IR_SENSOR_FWRIGHT_EXTI_TRIG       EXTI_Trigger_Rising_Falling
#define IR_SENSOR_FWRIGHT_NVIC_CHAN       EXTI15_10_IRQn
#define IR_SENSOR_FWRIGHT_NVIC_PPRIO      5
#define IR_SENSOR_FWRIGHT_NVIC_SPRIO      0

#define IR_SENSOR_BWLEFT_PIN              GPIO_Pin_5
#define IR_SENSOR_BWLEFT_PIN_MODE         GPIO_Mode_IN
#define IR_SENSOR_BWLEFT_PIN_SPEED        GPIO_Speed_2MHz
#define IR_SENSOR_BWLEFT_PIN_TYPE         GPIO_OType_PP
#define IR_SENSOR_BWLEFT_PIN_PUPD         GPIO_PuPd_UP
#define IR_SENSOR_BWLEFT_PORT_CLK         RCC_AHB1Periph_GPIOB
#define IR_SENSOR_BWLEFT_PORT             GPIOB
#define IR_SENSOR_BWLEFT_EXTI_PORT        EXTI_PortSourceGPIOB
#define IR_SENSOR_BWLEFT_EXTI_PIN         EXTI_PinSource5
#define IR_SENSOR_BWLEFT_EXTI_LINE        EXTI_Line5
#define IR_SENSOR_BWLEFT_EXTI_TRIG        EXTI_Trigger_Falling
#define IR_SENSOR_BWLEFT_NVIC_CHAN        EXTI9_5_IRQn
#define IR_SENSOR_BWLEFT_NVIC_PPRIO       5
#define IR_SENSOR_BWLEFT_NVIC_SPRIO       0

#define IR_SENSOR_BWRIGHT_PIN              GPIO_Pin_15
#define IR_SENSOR_BWRIGHT_PIN_MODE         GPIO_Mode_IN
#define IR_SENSOR_BWRIGHT_PIN_SPEED        GPIO_Speed_2MHz
#define IR_SENSOR_BWRIGHT_PIN_TYPE         GPIO_OType_PP
#define IR_SENSOR_BWRIGHT_PIN_PUPD         GPIO_PuPd_UP
#define IR_SENSOR_BWRIGHT_PORT_CLK         RCC_AHB1Periph_GPIOE
#define IR_SENSOR_BWRIGHT_PORT             GPIOE
#define IR_SENSOR_BWRIGHT_EXTI_PORT        EXTI_PortSourceGPIOE
#define IR_SENSOR_BWRIGHT_EXTI_PIN         EXTI_PinSource15
#define IR_SENSOR_BWRIGHT_EXTI_LINE        EXTI_Line15
#define IR_SENSOR_BWRIGHT_EXTI_TRIG        EXTI_Trigger_Rising_Falling
#define IR_SENSOR_BWRIGHT_NVIC_CHAN        EXTI15_10_IRQn
#define IR_SENSOR_BWRIGHT_NVIC_PPRIO       5
#define IR_SENSOR_BWRIGHT_NVIC_SPRIO       0

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
#define SENSOR_KEY_EXTI_TRIG            EXTI_Trigger_Falling
#define SENSOR_KEY_EXTI_CHAN            EXTI9_5_IRQn
#define SENSOR_KEY_EXTI_PPRIO           5
#define SENSOR_KEY_EXTI_SPRIO           0

/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initSensor_EmergencyStop();
extern uint8_t getSensor_EmergencyStop();
extern uint8_t getSensorPosEdge_EmergencyStop(uint8_t*);
extern uint8_t getSensorNegEdge_EmergencyStop(uint8_t*);
extern void initSensor_Fire_Pool_Right();
extern uint8_t getSensor_Fire_Pool_Right();
extern uint8_t getSensorPosEdge_Fire_Pool_Right(uint8_t*);
extern uint8_t getSensorNegEdge_Fire_Pool_Right(uint8_t*);
extern void initSensor_Air();
extern uint8_t getSensor_Air();
extern uint8_t getSensorPosEdge_Air(uint8_t*);
extern uint8_t SensorNegEdge_Air(uint8_t*);
extern void initSensor_Fire_Pool_Left();
extern uint8_t getSensor_Fire_Pool_Left();
extern uint8_t getSensorPosEdge_Fire_Pool_Left(uint8_t*);
extern uint8_t getSensorNegEdge_Fire_Pool_Left(uint8_t*);
extern void initIRSensor_BwLeft();
extern uint8_t getIRSensor_BwLeft();
extern uint8_t getIRSensorPosEdge_BwLeft(uint8_t*);
extern uint8_t getIRSensorNegEdge_BwLeft(uint8_t*);
extern void initIRSensor_FwLeft();
extern uint8_t getIRSensor_FwLeft();
extern uint8_t getIRSensorPosEdge_FwLeft(uint8_t*);
extern uint8_t getIRSensorNegEdge_FwLeft(uint8_t*);
extern void initIRSensor_BwRight();
extern uint8_t getIRSensor_BwRight();
extern uint8_t getIRSensorPosEdge_BwRight(uint8_t*);
extern uint8_t getIRSensorNegEdge_BwRight(uint8_t*);
extern void initIRSensor_FwRight();
extern uint8_t getIRSensor_FwRight();
extern uint8_t getIRSensorPosEdge_FwRight(uint8_t*);
extern uint8_t getIRSensorNegEdge_FwRight(uint8_t*);
extern void initSensor_Key();
extern uint8_t getSensor_Key();
extern uint8_t getSensorPosEdge_Key(uint8_t*);
extern uint8_t getSensorNegEdge_Key(uint8_t*);

#endif /* SENSOR_H_ */

/**
 * @}
 */
