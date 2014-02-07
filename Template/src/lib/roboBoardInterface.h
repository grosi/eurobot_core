	/**
  ******************************************************************************
  * @file    roboBoardInterface.h
  * @author  Schüpbach Simon
	* @date    07.06.2013
	*
	* @version 1.0	
	*	create this file
	*
  * @brief   this file include all functions to use a servo or dc-motor, 
	*					 LED or buttons,  ADC or DAC.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ROBOBOARDINTERFACE_H
#define __ROBOBOARDINTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
	
/* Exported types ------------------------------------------------------------*/

typedef struct{
 float Red;
 float Green;
 float Blue;
}ColorSensorValues;


/* exported define ------------------------------------------------------------*/



/*******************************************************************************
 * RoboBoard configurations
 ******************************************************************************/

/* dc-motor signal */
#define DCMotor1_Frequency 20000
#define DCMotor1_Frequency 20000

/* ec-motor, nominal speed signal */
#define ECMotor_Frequency 6000

/* if the adc measure is with dma */
//#define	ADC_WITH_DMA


/* voltage of the discoveryboard, measure at pin 3V */
#define VOLTAGE_DISCOVERYBOARD		2.98f

/* convert a current value of the ecmotor to a voltage value for the DAC */
#define CURRENT_2_VOLTAGE 1
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/  

/* get the sensor value */
float getSensor1(void);
float getSensor2(void);
float getSensor3(void);
float getSensor4(void);
	 
/* get the three sensor values */
ColorSensorValues getColorSensor(void);

/* set the dcmotor dutycycles */
void setDCMotor1(uint16_t DutyCycleA, uint16_t DutyCycleB);
void setDCMotor2(uint16_t DutyCycleA, uint16_t DutyCycleB);


/* get the button state */
uint8_t getButton1(void);
uint8_t getButton2(void);
uint8_t getButton3(void);
uint8_t getButton4(void);
uint8_t getBlueButtonDiscoveryBoard(void);



#endif
