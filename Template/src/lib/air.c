/**
 * \file    air.c
 * \author  gross10
 * \date    2014-05-08
 *
 * \version 1.0
 *  create this file
 *
 * \brief   air pump and switch access
 *
 * \defgroup air Air
 * \brief   air pump and switch for the big robo
 * \ingroup firmware
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "air.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/**
 * \fn      initAir
 * \brief   init air-system
 */
void initAir()
{
    /* variable for sensor init */
    GPIO_InitTypeDef air_gpio;

    /* initialize gpio */
    air_gpio.GPIO_Pin = AIR_IN1_PIN | AIR_IN2_PIN;
    air_gpio.GPIO_Mode = AIR_IN1_PIN_MODE;
    air_gpio.GPIO_OType = AIR_IN1_PIN_TYPE;
    air_gpio.GPIO_PuPd = AIR_IN1_PIN_PUPD;
    air_gpio.GPIO_Speed = AIR_IN1_PIN_SPEED;

    /* enable clock */
    RCC_AHB1PeriphClockCmd(AIR_IN1_PORT_CLK, ENABLE);

    /* enables port and pin */
    GPIO_Init(AIR_IN1_PORT,&air_gpio);

    /* set IN2 always on */
    GPIO_WriteBit(AIR_IN2_PORT,AIR_IN2_PIN,ENABLE);
    GPIO_WriteBit(AIR_IN2_PORT,AIR_IN1_PIN,ENABLE);

}


/**
 * \fn      setAir
 * \brief   enable/disable air-system
 *
 * \param[in]   state   0:=off; 1:=on
 */
void setAir(uint8_t state)
{
    /* air-system on */
    if(state)
    {
        GPIO_WriteBit(AIR_IN2_PORT,AIR_IN1_PIN,DISABLE);
    }
    /* air-system off */
    else
    {
        GPIO_WriteBit(AIR_IN2_PORT,AIR_IN1_PIN,ENABLE);
    }
}


/**
 * @}
 */
