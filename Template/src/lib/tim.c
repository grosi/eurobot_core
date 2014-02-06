/**
 * @file    tim.c
 * @author  Schüpbach Simon
 * @date    17.05.2013
 *
 * @version 2.0
 *  change the parameter of the function to pointer
 *  add the impulse counter functions
 *
 * @version 1.0
 *  create this file
 *
 * @brief   this file contains function to config a pin to generate a pwm-signal
 *  or as a impuls counter.
 */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
#include "tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* resolution for one pwm period */
#define PERIODE 1000

/* pulsewidth of the pwmsignal after the config */
#define DUTY_CYCLE 0

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Initializes the timer clock and get the alternate function number */
static uint8_t initTim(TIM_TypeDef* Timer);

/* get the interrupt name of the associated timer */
static IRQn_Type getTimerIRQ(TIM_TypeDef *Timer);


/**
 * \fn      initPWM
 * @brief   Initializes two pins for the PWM-OUTPUT-MODE.
 *
 * @param   Pwm: include all datas for initialize
 * @retval  None
 * \todo WARUM 2 Pins????
 */
void initPWM(const PWMInitStruct *PWM){

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Variable for the pwm channels*/
    uint16_t Channel = PWM->Channel;

    /* Variable for differents AF-Timer values*/
    uint8_t AF_Tim = 0x00;

    /* prescaler to generate the wished frequenzy */
    uint16_t prescaler = 168000 / ( PWM->Frequenz );

    /* enable clock and get AF*/
    AF_Tim = initTim(PWM->Timer);

    /* some timer has an other clock frequenzy */
    if((PWM->Timer == TIM3) || (PWM->Timer == TIM12)){
        prescaler = prescaler / 2;
    }

    /* enables port, pin and alternate function */
    enableAFPort(PWM->Port, PWM->Gpio, AF_Tim);

    /* Initialize the timer */
    TIM_TimeBaseStructure.TIM_Period = PERIODE;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(PWM->Timer, &TIM_TimeBaseStructure);

    /* for Advanced timer enable the arr-register */
    if((PWM->Timer == TIM1) || (PWM->Timer == TIM8)){
        TIM_ARRPreloadConfig(PWM->Timer, ENABLE);
    }

    /* Initialize the pwm-signal */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = DUTY_CYCLE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    /* load and enable the correct channels */
    if((Channel & CH1 ) == CH1 ){
        TIM_OC1Init(PWM->Timer, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(PWM->Timer, TIM_OCPreload_Enable);
    }
    if((Channel & CH2 ) == CH2 ){
        TIM_OC2Init(PWM->Timer, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(PWM->Timer, TIM_OCPreload_Enable);
    }
    if((Channel & CH3 ) == CH3 ){
        TIM_OC3Init(PWM->Timer, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(PWM->Timer, TIM_OCPreload_Enable);
    }
    if((Channel & CH4 ) == CH4 ){
        TIM_OC4Init(PWM->Timer, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(PWM->Timer, TIM_OCPreload_Enable);
    }

    /* enable the channels in the timer */
    TIM_ARRPreloadConfig(PWM->Timer, ENABLE);

    /* for Advanced timer enable the pwm output */
    if((PWM->Timer == TIM1) || (PWM->Timer == TIM8)){
        TIM_CtrlPWMOutputs(PWM->Timer, ENABLE);
    }

    /* enable counter */
    TIM_Cmd(PWM->Timer, ENABLE);
}


/**
 * \fn      getDutyCycleRegister
 * @brief   return the address of the dutycycle register
 *
 * @param   PWM include all datas for initialize
 * @retval  Adress of the dutycycle register
 */
DutyCycleRegister getDutyCycleRegister(const PWMInitStruct *PWM){
	DutyCycleRegister DCR = 0;
	
	/* find the channel */
	if((PWM->Channel & CH1 ) == CH1 ){
		DCR =(DutyCycleRegister) &(PWM->Timer->CCR1);
	}
	if((PWM->Channel & CH2 ) == CH2 ){
		DCR = (DutyCycleRegister)&(PWM->Timer->CCR2);
	}
	if((PWM->Channel & CH3 ) == CH3 ){
		DCR =(DutyCycleRegister) &(PWM->Timer->CCR3);
	}
	if((PWM->Channel & CH4 ) == CH4 ){
		DCR =(DutyCycleRegister) &(PWM->Timer->CCR4);
	}
	
	/* return value */
	return DCR;
}


/**
 * \fn      initTim
 * @brief   Initializes the timer clock and get the alternate function number
 * @param   Timer handled timer
 * @retval  AF_Tim alternate function number
 */
static uint8_t initTim(TIM_TypeDef* Timer){
	
	/* buffer for the alternate function */
	uint8_t AF_Tim = 0;
	
	/* Enables the APB1 low-speed clock or the APB2 high-speed clock */
	/* Choose the specific AF timer value */
	switch((intptr_t)(Timer)){
		case (intptr_t)TIM1:	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
													AF_Tim = GPIO_AF_TIM1;
			break;
		case (intptr_t)TIM2:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
													AF_Tim = GPIO_AF_TIM2;
			break;
		case (intptr_t)TIM3:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
													AF_Tim = GPIO_AF_TIM3;
			break;
		case (intptr_t)TIM4:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
													AF_Tim = GPIO_AF_TIM4;
			break;
		case (intptr_t)TIM5:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
													AF_Tim = GPIO_AF_TIM5;
			break;
		case (intptr_t)TIM6:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 
			break;
		case (intptr_t)TIM7:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
			break;
		case (intptr_t)TIM8:	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
													AF_Tim = GPIO_AF_TIM8;
			break;
		case (intptr_t)TIM9:	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE); 
													AF_Tim = GPIO_AF_TIM9;
			break;
		case (intptr_t)TIM10:	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); 
													AF_Tim = GPIO_AF_TIM10;
			break;
		case (intptr_t)TIM11:	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE); 
													AF_Tim = GPIO_AF_TIM11;
			break;
		case (intptr_t)TIM12:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); 
													AF_Tim = GPIO_AF_TIM12;
			break;
		case (intptr_t)TIM13:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE); 
													AF_Tim = GPIO_AF_TIM13;
			break;
		case (intptr_t)TIM14:	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); 
													AF_Tim = GPIO_AF_TIM14;
		break;
		default:	
			#ifdef DEBUGGING
					while(1){}
			#endif
		break;
	}
	
	/* return value */
	return AF_Tim;
}

/**
 * \fn      initExternalSignalCounter
 * @brief   Initializes a pin with a extern Clock, this is usefull to count the impulses of a signal.
 *
 * @param   Ext: include all datas for initialize
 * @retval  None
 */
TimerCounterRegister initExternalSignalCounter(PWMInitStruct *Ext){	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Variable for differents AF-Timer values*/
	uint8_t AF_Tim;
	
	/* enable clock and get AF*/
	AF_Tim = initTim(Ext->Timer);

	/* enables port, pin and alternate function */
	enableAFPort(Ext->Port,Ext->Gpio,AF_Tim);	

	/*interrupt settings */
	NVIC_InitStructure.NVIC_IRQChannel = getTimerIRQ(Ext->Timer);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* timer settings */
	TIM_TimeBaseStructure.TIM_Period = COUNTER_SIZE;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(Ext->Timer, &TIM_TimeBaseStructure);		
	TIM_ITConfig(Ext->Timer, TIM_IT_Update, ENABLE);
	TIM_TIxExternalClockConfig(Ext->Timer,TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
	
	/* enable timer */
	TIM_Cmd(Ext->Timer, ENABLE);
	
	return (TimerCounterRegister)&(Ext->Timer->CNT);
}

/**
 * \fn      getTimerIRQ
 * @brief   get the interrupt name of the associated timer
 *
 * @param   Timer: associated timer
 * @retval  None
 */
IRQn_Type getTimerIRQ(TIM_TypeDef *Timer){
	IRQn_Type Irq;
	
	/* get the interrupt channel */
	switch((intptr_t)Timer){
		case (intptr_t)TIM1:	Irq = TIM1_UP_TIM10_IRQn;
		break;
		case (intptr_t)TIM2:	Irq = TIM2_IRQn;
		break;
		case (intptr_t)TIM3:	Irq = TIM3_IRQn;
		break;
		case (intptr_t)TIM4:	Irq = TIM4_IRQn;
		break;
		case (intptr_t)TIM5:	Irq = TIM5_IRQn;
		break;
		case (intptr_t)TIM6:	Irq = TIM6_DAC_IRQn;
		break;
		case (intptr_t)TIM7:	Irq = TIM7_IRQn;
		break;
		case (intptr_t)TIM8:	Irq = TIM8_UP_TIM13_IRQn;
		break;
		case (intptr_t)TIM9:	Irq = TIM1_BRK_TIM9_IRQn;
		break;
		case (intptr_t)TIM10:	Irq = TIM1_UP_TIM10_IRQn;
		break;
		case (intptr_t)TIM11:	Irq = TIM1_TRG_COM_TIM11_IRQn;
		break;
		case (intptr_t)TIM12:	Irq = TIM8_BRK_TIM12_IRQn;
		break;
		case (intptr_t)TIM13:	Irq = TIM8_UP_TIM13_IRQn ;
		break;
		case (intptr_t)TIM14:	Irq = TIM8_TRG_COM_TIM14_IRQn;
		break;
	}
	
	/* return name */
	return Irq;
}
