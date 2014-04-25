/**
 * \file    servo.c
 * \author  gross10
 * \date    2013-12-12

 * \version 1.0
 *  create this file
 *
 * \brief   servo access. Only for the for the roboboard-shield
 * \note    this module is just for the roboboard-shield. For other hardware
 *          configurations, the code have to adapted!
 *
 * \defgroup servo Servo
 * \brief   Servo library
 * \ingroup firmware
 *
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SERVO_H_
#define SERVO_H_


/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
/* Pin and Port of the Servo-Interfaces */
#define SERVO_1_PIN         GPIO_Pin_9 /*!< don't change -> roboboards-shield specific */
#define SERVO_1_PIN_MODE    GPIO_Mode_AF
#define SERVO_1_PIN_SPEED   GPIO_Speed_100MHz
#define SERVO_1_PIN_TYPE    GPIO_OType_PP
#define SERVO_1_PIN_PUPD    GPIO_PuPd_NOPULL
#define SERVO_1_PORT        GPIOE /*!< don't change -> roboboards-shield specific */
#define SERVO_1_CHANNEL     TIM_Channel_1 /*!< don't change -> roboboards-shield specific */
#define SERVO_1_TIMER       TIM1 /*!< don't change -> roboboards-shield specific */
#define SERVO_1_FREQUENCY   50
#define SERVO_1_DUTY_CYCLE  0 /*!< start duty cycle after initialisation */
#define SERVO_1_RESOLUTION  20000

#define SERVO_2_PIN         GPIO_Pin_11 /*!< don't change -> roboboards-shield specific */
#define SERVO_2_PIN_MODE    GPIO_Mode_AF
#define SERVO_2_PIN_SPEED   GPIO_Speed_100MHz
#define SERVO_2_PIN_TYPE    GPIO_OType_PP
#define SERVO_2_PIN_PUPD    GPIO_PuPd_NOPULL
#define SERVO_2_PORT        GPIOE /*!< don't change -> roboboards-shield specific */
#define SERVO_2_CHANNEL     TIM_Channel_2 /*!< don't change -> roboboards-shield specific */
#define SERVO_2_TIMER       TIM1 /*!< don't change -> roboboards-shield specific */
#define SERVO_2_FREQUENCY   50
#define SERVO_2_DUTY_CYCLE  0 /**< start duty cycle after initialisation */
#define SERVO_2_RESOLUTION  20000

#define SERVO_3_PIN         GPIO_Pin_13 /*!< don't change -> roboboards-shield specific */
#define SERVO_3_PIN_MODE    GPIO_Mode_AF
#define SERVO_3_PIN_SPEED   GPIO_Speed_100MHz
#define SERVO_3_PIN_TYPE    GPIO_OType_PP
#define SERVO_3_PIN_PUPD    GPIO_PuPd_NOPULL
#define SERVO_3_PORT        GPIOE /*!< don't change -> roboboards-shield specific */
#define SERVO_3_CHANNEL     TIM_Channel_3 /*!< don't change -> roboboards-shield specific */
#define SERVO_3_TIMER       TIM1 /*!< don't change -> roboboards-shield specific */
#define SERVO_3_FREQUENCY   50
#define SERVO_3_DUTY_CYCLE  0 /*!< start duty cycle after initialisation */
#define SERVO_3_RESOLUTION  20000

#define SERVO_4_PIN         GPIO_Pin_14 /*!< don't change -> roboboards-shield specific */
#define SERVO_4_PIN_MODE    GPIO_Mode_AF
#define SERVO_4_PIN_SPEED   GPIO_Speed_100MHz
#define SERVO_4_PIN_TYPE    GPIO_OType_PP
#define SERVO_4_PIN_PUPD    GPIO_PuPd_NOPULL
#define SERVO_4_PORT        GPIOE /*!< don't change -> roboboards-shield specific */
#define SERVO_4_CHANNEL     TIM_Channel_4 /*!< don't change -> roboboards-shield specific */
#define SERVO_4_TIMER       TIM1 /*!< don't change -> roboboards-shield specific */
#define SERVO_4_FREQUENCY   50
#define SERVO_4_DUTY_CYCLE  0 /*!< start duty cycle after initialisation */
#define SERVO_4_RESOLUTION  20000


/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initServo_1();
extern void setServo_1(uint16_t);
extern void initServo_2();
extern void setServo_2(uint16_t);
extern void initServo_3();
extern void setServo_3(uint16_t);
extern void initServo_4();
extern void setServo_4(uint16_t);

#endif /* SERVO_H_ */

/**
 * @}
 */
