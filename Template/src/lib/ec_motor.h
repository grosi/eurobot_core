/**
  ******************************************************************************
  * @file    ec_motor.h
  * @author  Schüpbach Simon
  * @date    30.05.2013
	*
	* @version 1.0	
	*	create this file
	*
  * @brief   this file conatains all function that are needed to use the ec-motor
	*					 all the signals are initialize and so you can control the ec-motor.
	*					 all the signals of the ec-motor drive are detected
  ******************************************************************************  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EC_MOTOR_H
#define __EC_MOTOR_H

//#include "include.h"

/* ec-motor, nominal speed signal */
#define ECMotor_Frequency 6000

/* convert a current value of the ecmotor to a voltage value for the DAC */
#define CURRENT_2_VOLTAGE 1

/* voltage of the discoveryboard, measure at pin 3V */
#define VOLTAGE_DISCOVERYBOARD		2.98f

/* define values for the queue-message */
#define ERROR_FF1 0x10
#define ERROR_FF2 0x20

/* define direction */
typedef enum{
	REVERSE = 0,
	FORWARD = 2
}DirectionEnum;

typedef struct {
	
	/* (0) for fast current control with greater current ripple */
	/* 		 suitable for Motors wich run in the start/stop mode  */
	/* (1) for slow current control with few current ripple 		*/
	/* 		 suitable for Motors wich run at a constant speed 		*/
	uint8_t MODE;		

	/* (0) Disable all Fet's  */
	/* (1) None	  */
	uint8_t COAST;
	
	/* (0) Brake motor, low-Side Fet turn on and high-Side Fet turn off */
	/* (1) None	  */	
	uint8_t BRAKE;
	
	/* (0) Disable power supply */
	/* (1) Enable	power supply (wait 3ms while the power is stabilized ) */
	uint8_t RESET;
	
	
	/************************************************************
	* FF1 * FF2 *		Fault								*					Action				*
	*			*			*												* 	ESF=0 	* 	ESF=1 	*
	*************************************************************
	*	 0  *  0  * Undervoltage					*  Disable  *  Disable  *
	*	 0  *  0  * Overtemperature				* No Action * No Action *
	*	 0  *  0  * Logic Fault						*  Disable  *  Disable  *
	*	 1  *  0  * Short to ground				* No Action *  Disable  *
	*	 1  *  0  * Short to supply				* No Action *  Disable  *
	*	 1  *  0  * Shorted Motor winding	* No Action *  Disable  *
	*	 0  *  1  * Low load current			* No Action * No Action *
	*	 1  *  1  * None									* No Action * No Action *
	************************************************************/
	
	/* (0) Disable the automatic error-handling, no safety!!! */
	/* (1) Enable	some error-handlings*/
	uint8_t ESF;
	
}ECMotorSettingStruct;

/* queue for the error handling */
extern xQueueHandle qECMotorError;

/*	Initializes all necessary for using the EC-Motor  */
void initECMotor(void);

/* set the currrent limit of the EC-Motor */
void setECMotorCurrentLimit(float Current);

/* get the actuel currrent of the EC-Motor */
float getECMotorCurrent(void);

/* config the settings of the EC-Motor IC */
void configECMotor(ECMotorSettingStruct *Setting);

/* set the nominal rotate dircetion of the EC-Motor */
void setECMotorDirection(DirectionEnum Direction);

/* get the actual rotate direction of the EC-Motor */
DirectionEnum getECMotorDirection(void);

/* update and get the position of the EC-Motor */
int32_t getECMotorPosition(void);

/* set the value of the position buffer */
void setECMotorPosition(int32_t Position);

/* set the nominal DutyCycle of the PWM wich controls the RotationSpeed */
void setECMotorPWMSpeed(int16_t Speed);

#endif
