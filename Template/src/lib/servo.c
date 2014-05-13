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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "servo.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
 * \fn      initServo_1
 * \brief   initialize the servo 1 output with the predefinded settings
 *
 * \param   None
 * \return  None
 */
void initServo_1()
{
    /* variable for servo init */
    GPIO_InitTypeDef servo_gpio;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* initialize gpio */
    servo_gpio.GPIO_Pin = SERVO_1_PIN;
    servo_gpio.GPIO_Mode = SERVO_1_PIN_MODE;
    servo_gpio.GPIO_OType = SERVO_1_PIN_TYPE;
    servo_gpio.GPIO_PuPd = SERVO_1_PIN_PUPD;
    servo_gpio.GPIO_Speed = SERVO_1_PIN_SPEED;

    /* Initialize the timer */
    TIM_TimeBaseStructure.TIM_Period = SERVO_1_RESOLUTION - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / SERVO_1_RESOLUTION) / (SERVO_1_FREQUENCY) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* Initialize the pwm-signal */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = SERVO_1_DUTY_CYCLE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    /* enable clock and get AF*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* enables port, pin and alternate function */
    GPIO_Init(SERVO_1_PORT,&servo_gpio);
    GPIO_PinAFConfig(SERVO_1_PORT, GPIO_PinSource9, GPIO_AF_TIM1);

    /* init timer */
    TIM_TimeBaseInit(SERVO_1_TIMER, &TIM_TimeBaseStructure);

    /* load and enable the correct channels */
    TIM_OC1Init(SERVO_1_TIMER, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(SERVO_1_TIMER, TIM_OCPreload_Enable);

    /* enable the channels in the timer */
    TIM_ARRPreloadConfig(SERVO_1_TIMER, ENABLE);

    /* for Advanced timer enable the pwm output */
    TIM_CtrlPWMOutputs(SERVO_1_TIMER, ENABLE);

    /* enable counter */
    TIM_Cmd(SERVO_1_TIMER, ENABLE);
}


/**
 * \fn      setServo_1
 * \brief   set the servo position
 * \note    only works with timer-cannel 1
 *
 * \param[in]   new_position    the new position
 * \return      None
 */
void setServo_1(uint16_t new_position)
{
    /* catch overflow */
    if(new_position > SERVO_1_RESOLUTION / 10)
    {
        new_position = SERVO_1_RESOLUTION / 10;
    }

    /* set the dutycycle */
    SERVO_1_TIMER->CCR1 = new_position;
}


/**
 * \fn      getServo_1
 * \brief   current servo position
 *
 * \return servo position [900-2000]
 */
uint16_t getServo_1(void)
{
    return SERVO_1_TIMER->CCR1;
}


/**
 * \fn      initServo_2
 * \brief   initialize the servo 2 output with the predefinded settings
 *
 * \param   None
 * \return  None
 */
void initServo_2()
{
    /* variable for servo init */
    GPIO_InitTypeDef servo_gpio;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* initialize gpio */
    servo_gpio.GPIO_Pin = SERVO_2_PIN;
    servo_gpio.GPIO_Mode = SERVO_2_PIN_MODE;
    servo_gpio.GPIO_OType = SERVO_2_PIN_TYPE;
    servo_gpio.GPIO_PuPd = SERVO_2_PIN_PUPD;
    servo_gpio.GPIO_Speed = SERVO_2_PIN_SPEED;

    /* Initialize the timer */
    TIM_TimeBaseStructure.TIM_Period = SERVO_2_RESOLUTION - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / SERVO_2_RESOLUTION) / (SERVO_2_FREQUENCY) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* Initialize the pwm-signal */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = SERVO_2_DUTY_CYCLE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    /* enable clock and get AF*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* enables port, pin and alternate function */
    GPIO_Init(SERVO_2_PORT,&servo_gpio);
    GPIO_PinAFConfig(SERVO_2_PORT, GPIO_PinSource11, GPIO_AF_TIM1);

    /* init timer */
    TIM_TimeBaseInit(SERVO_2_TIMER, &TIM_TimeBaseStructure);

    /* load and enable the correct channels */
    TIM_OC2Init(SERVO_2_TIMER, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(SERVO_2_TIMER, TIM_OCPreload_Enable);

    /* enable the channels in the timer */
    TIM_ARRPreloadConfig(SERVO_2_TIMER, ENABLE);

    /* for Advanced timer enable the pwm output */
    TIM_CtrlPWMOutputs(SERVO_2_TIMER, ENABLE);

    /* enable counter */
    TIM_Cmd(SERVO_2_TIMER, ENABLE);
}


/**
 * \fn      setServo_2
 * \brief   set the servo position
 * \note    only works with timer-cannel 2
 *
 * \param[in]   new_position    the new position
 * \return      None
 */
void setServo_2(uint16_t new_position)
{
    /* catch overflow */
    if(new_position > SERVO_2_RESOLUTION / 10)
    {
        new_position = SERVO_2_RESOLUTION / 10;
    }

    SERVO_2_TIMER->CCR2 = new_position;
}


/**
 * \fn      getServo_2
 * \brief   current servo position
 *
 * \return servo position [900-2000]
 */
uint16_t getServo_2(void)
{
    return SERVO_2_TIMER->CCR1;
}


/**
 * \fn      initServo_3
 * \brief   initialize the servo 3 output with the predefinded settings
 *
 * \param   None
 * \return  None
 */
void initServo_3()
{
    /* variable for servo init */
    GPIO_InitTypeDef servo_gpio;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* initialize gpio */
    servo_gpio.GPIO_Pin = SERVO_3_PIN;
    servo_gpio.GPIO_Mode = SERVO_3_PIN_MODE;
    servo_gpio.GPIO_OType = SERVO_3_PIN_TYPE;
    servo_gpio.GPIO_PuPd = SERVO_3_PIN_PUPD;
    servo_gpio.GPIO_Speed = SERVO_3_PIN_SPEED;

    /* Initialize the timer */
    TIM_TimeBaseStructure.TIM_Period = SERVO_3_RESOLUTION - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / SERVO_3_RESOLUTION) / (SERVO_3_FREQUENCY) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* Initialize the pwm-signal */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = SERVO_3_DUTY_CYCLE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    /* enable clock and get AF*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* enables port, pin and alternate function */
    GPIO_Init(SERVO_3_PORT,&servo_gpio);
    GPIO_PinAFConfig(SERVO_3_PORT, GPIO_PinSource13, GPIO_AF_TIM1);

    /* init timer */
    TIM_TimeBaseInit(SERVO_3_TIMER, &TIM_TimeBaseStructure);

    /* load and enable the correct channels */
    TIM_OC3Init(SERVO_3_TIMER, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(SERVO_3_TIMER, TIM_OCPreload_Enable);

    /* enable the channels in the timer */
    TIM_ARRPreloadConfig(SERVO_3_TIMER, ENABLE);

    /* for Advanced timer enable the pwm output */
    TIM_CtrlPWMOutputs(SERVO_3_TIMER, ENABLE);

    /* enable counter */
    TIM_Cmd(SERVO_3_TIMER, ENABLE);
}


/**
 * \fn      setServo_3
 * \brief   set the servo position
 * \note    only works with timer-cannel 3
 *
 * \param[in]   new_position    the new position
 * \return      None
 */
void setServo_3(uint16_t new_position)
{
    /* catch overflow */
    if(new_position > SERVO_3_RESOLUTION / 10)
    {
        new_position = SERVO_3_RESOLUTION / 10;
    }

    /* set the dutycycle */
    SERVO_3_TIMER->CCR3 = new_position;
}


/**
 * \fn      getServo_3
 * \brief   current servo position
 *
 * \return servo position [900-2000]
 */
uint16_t getServo_3(void)
{
    return SERVO_3_TIMER->CCR1;
}


/**
 * \fn      initServo_4
 * \brief   initialize the servo 4 output with the predefinded settings
 *
 * \param   None
 * \return  None
 */
void initServo_4()
{
    /* variable for servo init */
    GPIO_InitTypeDef servo_gpio;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* initialize gpio */
    servo_gpio.GPIO_Pin = SERVO_4_PIN;
    servo_gpio.GPIO_Mode = SERVO_4_PIN_MODE;
    servo_gpio.GPIO_OType = SERVO_4_PIN_TYPE;
    servo_gpio.GPIO_PuPd = SERVO_4_PIN_PUPD;
    servo_gpio.GPIO_Speed = SERVO_4_PIN_SPEED;

    /* Initialize the timer */
    TIM_TimeBaseStructure.TIM_Period = SERVO_4_RESOLUTION - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / SERVO_4_RESOLUTION) / (SERVO_4_FREQUENCY) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* Initialize the pwm-signal */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = SERVO_4_DUTY_CYCLE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    /* enable clock and get AF*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* enables port, pin and alternate function */
    GPIO_Init(SERVO_4_PORT,&servo_gpio);
    GPIO_PinAFConfig(SERVO_4_PORT, GPIO_PinSource14, GPIO_AF_TIM1);

    /* init timer */
    TIM_TimeBaseInit(SERVO_4_TIMER, &TIM_TimeBaseStructure);

    /* load and enable the correct channels */
    TIM_OC3Init(SERVO_4_TIMER, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(SERVO_4_TIMER, TIM_OCPreload_Enable);

    /* enable the channels in the timer */
    TIM_ARRPreloadConfig(SERVO_4_TIMER, ENABLE);

    /* for Advanced timer enable the pwm output */
    TIM_CtrlPWMOutputs(SERVO_4_TIMER, ENABLE);

    /* enable counter */
    TIM_Cmd(SERVO_4_TIMER, ENABLE);
}


/**
 * \fn      setServo_4
 * \brief   set the servo position
 * \note    only works with timer-cannel 4
 *
 * \param[in]   new_position    the new position
 * \return      None
 */
void setServo_4(uint16_t new_position)
{
    /* catch overflow */
    if(new_position > SERVO_4_RESOLUTION / 10)
    {
        new_position = SERVO_4_RESOLUTION / 10;
    }

    /* set the dutycycle */
    SERVO_4_TIMER->CCR4 = new_position;
}


/**
 * \fn      getServo_4
 * \brief   current servo position
 *
 * \return servo position [900-2000]
 */
uint16_t getServo_4(void)
{
    return SERVO_4_TIMER->CCR1;
}

/**
 * @}
 */
