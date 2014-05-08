/**
 * \file    Rangefinder.c
 * \author  kasen1
 * \date    2013-04-13
 *
 * \version 1.3
 *  - Added function isRobotInFront to compare rangefinder with navigation informations
 * \version 1.2
 *  - IR sensors in new arrangement
 *  - Added flag for separation blocked alarm
 *  - Added option to only use forward rangefinder by defining RANGEFINDER_ONLY_FW
 *  - Task is suspended at beginning, so ultrasonic only running when really used
 *  - Semaphore for synchronising with node task added (not in test protocol, but is tested)
 *  - Software tested (01.05.2014)
 * \version 1.1
 *  - Changed implementation of IR detection to external interrupt
 *  - Implemented comparison of last three US measures
 *  - Fixed I2C error, where the bus was always busy after disconnecting a slave
 *  - Software tested (12.03.2014)
 * \version 1.0
 *  Software tested (14.01.2014)
 * \version 0.1
 *  Import from template (14.01.2014)
 *
 * \brief    task for the rangefinder sensors
 *
 * \todo     Unit test for version 1.2 on robot
 *
 * \defgroup rangefinder Rangefinder
 * \brief    Rangefinder task
 * \ingroup  hw_task
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* standard libraries */
#include <math.h>

/* application */
#include "AppConfig.h"
#include "Rangefinder.h"

/* HW-library */
#include "../lib/i2c.h"
#include "../lib/sensor.h"
#include "../lib/exti_sensor.h"



/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Common */
#define EVER            ;;              /* For infinite loop --> for(;;) */
#define I2C_TIMEOUT     1000            /* Number of times a while loop checks for status, to prevent deadlock */

/* SRF08 I2C addresses */
#define SRF08_ADDR_FW   0xE0            /* Slave address of the SRF08 facing forward */
#define SRF08_ADDR_BW   0xE2            /* Slave address of the SRF08 facing backward */

/* SRF08 I2C registers */
#define SRF08_REG_CMD   0x00            /* Write: Command register */
#define SRF08_REG_GAIN  0x01            /* Write: Gain register */
#define SRF08_REG_RANGE 0x02            /* Write: Range register */
#define SRF08_REG_SWVER 0x00            /* Read:  Software revision register */
#define SRF08_REG_LIGHT 0x01            /* Read:  Light sensor register */
#define SRF08_REG_H(n)  (0x00+((n)*2))  /* Read:  Highbyte register of the n-th echo (1 <= n <= 17) */
#define SRF08_REG_L(n)  (0x01+((n)*2))  /* Read:  Lowbyte register of the n-th echo (1 <= n <= 17) */

/* SRF08 I2C commands */
#define SRF08_MEAS_IN   0x50            /* Measure in inches */
#define SRF08_MEAS_CM   0x51            /* Measure in centimetres */
#define SRF08_MEAS_US   0x52            /* Measure in micro-seconds */
#define SRF08_ANN_IN    0x53            /* Measure in inches (ANN mode) */
#define SRF08_ANN_CM    0x54            /* Measure in centimetres (ANN mode) */
#define SRF08_ANN_US    0x55            /* Measure in micro-seconds (ANN mode) */

/* Robot information */
#define ROBOT_BALLERINA_RADIUS  120  /* Distance from nose to center (navigation) in mm */
#define ROBOT_B52_RADIUS        230  /* Distance from nose to center (navigation) in mm */


/* Private variables ---------------------------------------------------------*/
/* Variables for comparing the last three values */
volatile uint8_t flag_FwAlarmUS_last[2] = {0,0};
#ifndef RANGEFINDER_ONLY_FW
volatile uint8_t flag_BwAlarmUS_last[2] = {0,0};
#endif /* RANGEFINDER_ONLY_FW */

/* Sensor values */
uint16_t distance_fw;                   /* Variable for the distance detected by the forward SRF08 (lower- /higher byte joined) */
uint16_t distance_bw;                   /* Variable for the distance detected by the backward SRF08 (lower- /higher byte joined) */

/* Globale variables ---------------------------------------------------------*/
/* RTOS */
xTaskHandle xRangefinderTask_Handle = NULL;
xSemaphoreHandle mHwI2C = NULL;  /* Mutex for I2C */
xSemaphoreHandle sSyncNodeTask = NULL;  /* Semaphore for node task synchronisation */

/* Alarm flags, 1 if object detected, 0 if no object detected */
volatile uint8_t Rangefinder_flag_FwAlarmIR = 0; /* Infrared forward alarm */
volatile uint8_t Rangefinder_flag_BwAlarmIR = 0; /* Infrared backward alarm */
volatile uint8_t Rangefinder_flag_FwAlarmUS = 0; /* Ultrasonic forward alarm */
volatile uint8_t Rangefinder_flag_BwAlarmUS = 0; /* Ultrasonic backward alarm */
volatile uint8_t Rangefinder_flag_SeAlarmUS = 0; /* Ultrasonic separation alarm */

/* Private function prototypes -----------------------------------------------*/
static void vRangefinderTask(void*);

/* Private functions ---------------------------------------------------------*/
void setSRF08Range(uint8_t slave_address, uint16_t range_in_mm);
void setSRF08Gain(uint8_t slave_address, uint8_t gain_value);
void startSRF08Meas(uint8_t slave_address, uint8_t meas_mode);
uint16_t readSRF08Meas(uint8_t slave_address);

/**
 * \fn          initRangefinderTasks
 * \brief       Initialisation of the Rangefinder Tasks
 *
 * \param[in]   None
 * \return      None
 */
void initRangefinderTask(void) {

	/* sensors initialisations */
	/* IR: init GPIOs */
	initIRSensor_FwLeft();
	initIRSensor_FwRight();
	initIRSensor_BwLeft();
	initIRSensor_BwRight();

	/* Configure EXTI Line in interrupt mode */
	initIRSensorEXTI_FwLeft();
	initIRSensorEXTI_FwRight();
	initIRSensorEXTI_BwLeft();
	initIRSensorEXTI_BwRight();

	/* For testing: Generate software interrupt: simulate a rising edge applied on EXTI5 line
	EXTI_GenerateSWInterrupt(EXTI_Line5); */

	/* US: I2C */
	initI2C();

	/* Create mutex for I2C */
	mHwI2C = xSemaphoreCreateMutex();

	/* Create semaphore for informing node task */
	vSemaphoreCreateBinary(sSyncNodeTask);

	/* Create the tasks */
	xTaskCreate( vRangefinderTask, ( signed char * ) RANGEFINDER_TASK_NAME, RANGEFINDER_STACK_SIZE, NULL, RANGEFINDER_TASK_PRIORITY, &xRangefinderTask_Handle);
}

/**
 * \fn          vRangefinderTaskUS
 * \brief       Task to watch the near range in front and back of the robot
 *              by ultrasonic
 *
 * \param[in]   None
 * \return      None
 */
static void vRangefinderTask(void* pvParameters ) {

	portTickType xLastFlashTime;

	/* We need to initialise xLastFlashTime prior to the first call to vTaskDelayUntil() */
	xLastFlashTime = xTaskGetTickCount();

	/* Set the range */
	setSRF08Range(SRF08_ADDR_FW, RANGEFINDER_RANGE * 10); /* In mm */
#ifndef RANGEFINDER_ONLY_FW
	setSRF08Range(SRF08_ADDR_BW, RANGEFINDER_RANGE * 10); /* In mm */
#endif /* RANGEFINDER_ONLY_FW */

	/* Set the gain register to evaluated value */
	setSRF08Gain(SRF08_ADDR_FW, 0x1F);
#ifndef RANGEFINDER_ONLY_FW
	setSRF08Gain(SRF08_ADDR_BW, 0x1F);
#endif /* RANGEFINDER_ONLY_FW */

	/* Suspend ourselves, so the task is only running when really used.
	 * This way there is less possible ultrasonic disturbance (navigation and other robot),
	 * infrared is still running. */
	vTaskSuspend(NULL);

	for(EVER) {

		/* Start the ultrasonic measures */
		startSRF08Meas(SRF08_ADDR_FW, SRF08_MEAS_CM);
#ifndef RANGEFINDER_ONLY_FW
		startSRF08Meas(SRF08_ADDR_BW, SRF08_MEAS_CM);
#endif /* RANGEFINDER_ONLY_FW */

		/* Wait 70 ms, for the sound to travel s_max (11 m) twice (=ca. 64.1 ms), and rounded up (65 ms still made trouble sometimes). */
		vTaskDelay(70 / portTICK_RATE_MS);

		/* Get distance from the modules */
		distance_fw = readSRF08Meas(SRF08_ADDR_FW);
#ifndef RANGEFINDER_ONLY_FW
		distance_bw = readSRF08Meas(SRF08_ADDR_BW);
#endif /* RANGEFINDER_ONLY_FW */

		/* Check front */
		/* Check for error */
		if(distance_fw == 0xFFFF) {
			/* ERROR (Semaphore not created correctly or timeout) */
		}
		/* Check if an obstacle is to close */
		else if(distance_fw != 0 && distance_fw < RANGEFINDER_THRESHOLD_FW) {

			/* Compare last three measures, only set alarm if at least two were positive.
			 * Inside of this block the current measure is positive, so check if at least one of the last two was positive too */
			if(flag_FwAlarmUS_last[1] || flag_FwAlarmUS_last[0])
			{
				/* Two of three were positive, set alarm (object detected) */
				Rangefinder_flag_FwAlarmUS = 1;
				/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
				xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
			}
			else
			{
				/* Two of three were negative, so don't set the alarm yet. */
				Rangefinder_flag_FwAlarmUS = 0;
			}

			/* Remember measure (NOT flag which is affected by the last two measures) */
			flag_FwAlarmUS_last[1] = flag_FwAlarmUS_last[0];  /* Move last measure one back */
			flag_FwAlarmUS_last[0] = 1;  /* Current measure is positive */
		}
		else {

			/* Compare last three measures, only reset alarm if at least two were negative.
			 * Inside of this block the current measure is negative, so check if at least one of the last two was negative too */
			if(!(flag_FwAlarmUS_last[1] && flag_FwAlarmUS_last[0]))
			{
				/* Two of three were negative, reset alarm  (nothing detected) */
				Rangefinder_flag_FwAlarmUS = 0;
			}
			else
			{
				/* Two of three were positive, so don't reset the alarm yet. */
				Rangefinder_flag_FwAlarmUS = 1;
				/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
				xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
			}

			/* Remember measure (NOT flag which is affected by the last two measures) */
			flag_FwAlarmUS_last[1] = flag_FwAlarmUS_last[0];  /* Move last measure one back */
			flag_FwAlarmUS_last[0] = 0;  /* Current measure is negative */
		}
		/* Check if separation space is blocked */
		if(distance_fw != 0xFFFF) {
			if(distance_fw <= RANGEFINDER_THRESHOLD_SE) {

				/* Set flag */
				Rangefinder_flag_SeAlarmUS = 1;
			}
			else {

				/* Reset flag */
				Rangefinder_flag_SeAlarmUS = 0;
			}
		}

#ifndef RANGEFINDER_ONLY_FW
		/* Check rear */
		/* Check for error */
		if(distance_bw == 0xFFFF) {
			/* ERROR (Semaphore not created correctly or timeout) */
		}
		/* Check if an obstacle is to close */
		else if(distance_bw != 0 && distance_bw < RANGEFINDER_THRESHOLD_BW) {

			/* Compare last three measures, only set alarm if at least two were positive.
			 * Inside of this block the current measure is positive, so check if at least one of the last two was positive too */
			if(flag_BwAlarmUS_last[1] || flag_BwAlarmUS_last[0])
			{
				/* Two of three were positive, set alarm (object detected) */
				Rangefinder_flag_BwAlarmUS = 1;
				/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
				xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
			}
			else
			{
				/* Two of three were negative, so don't set the alarm yet. */
				Rangefinder_flag_BwAlarmUS = 0;
			}

			/* Remember measure (NOT flag which is affected by the last two measures) */
			flag_BwAlarmUS_last[1] = flag_BwAlarmUS_last[0];  /* Move last measure one back */
			flag_BwAlarmUS_last[0] = 1;  /* Current measure is positive */
		}
		else {

			/* Compare last three measures, only reset alarm if at least two were negative.
			 * Inside of this block the current measure is negative, so check if at least one of the last two was negative too */
			if(!(flag_BwAlarmUS_last[1] && flag_BwAlarmUS_last[0]))
			{
				/* Two of three were negative, reset alarm  (nothing detected) */
				Rangefinder_flag_BwAlarmUS = 0;
			}
			else
			{
				/* Two of three were positive, so don't reset the alarm yet. */
				Rangefinder_flag_BwAlarmUS = 1;
				/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
				xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
			}

			/* Remember measure (NOT flag which is affected by the last two measures) */
			flag_BwAlarmUS_last[1] = flag_BwAlarmUS_last[0];  /* Move last measure one back */
			flag_BwAlarmUS_last[0] = 0;  /* Current measure is negative */
		}
#endif /* RANGEFINDER_ONLY_FW */

		/* Delay until defined time passed */
		vTaskDelayUntil( &xLastFlashTime, RANGEFINDER_DELAY / portTICK_RATE_MS);
	}
}

/**
 * \fn
 * \brief  This function is called by the external line 10-15 interrupt handler
 *
 * \param  None
 * \retval None
 */
void IRSensorFwLeft_IT(void) {

	/* Check if rising or falling interrupt */
	if(getIRSensor_FwLeft()) {

		/* Nothing detected, all ok */
		Rangefinder_flag_FwAlarmIR = 0;
	}
	else {

		/* Object detected, set alarm */
		Rangefinder_flag_FwAlarmIR = 1;
		/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
		xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
	}
}

/**
 * \fn
 * \brief  This function is called by the external line 10-15 interrupt handler
 *
 * \param  None
 * \retval None
 */
void IRSensorFwRight_IT(void) {

	/* Check if rising or falling interrupt */
	if(getIRSensor_FwRight()) {

		/* Nothing detected, all ok */
		Rangefinder_flag_FwAlarmIR = 0;
	}
	else {

		/* Object detected, set alarm */
		Rangefinder_flag_FwAlarmIR = 1;
		/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
		xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
	}
}

/**
 * \fn
 * \brief  This function is called by the external line 5-9 interrupt handler
 *
 * \param  None
 * \retval None
 */
void IRSensorBwLeft_IT(void) {
#ifndef RANGEFINDER_ONLY_FW

	/* Check if rising or falling interrupt */
	if(getIRSensor_BwLeft()) {

		/* Nothing detected, all ok */
		Rangefinder_flag_BwAlarmIR = 0;
	}
	else {

		/* Object detected, set alarm */
		Rangefinder_flag_BwAlarmIR = 1;
		/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
		xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
	}
#endif /* RANGEFINDER_ONLY_FW */
}

/**
 * \fn
 * \brief  This function is called by the external line 10-15 interrupt handler
 *
 * \param  None
 * \retval None
 */
void IRSensorBwRight_IT(void) {
#ifndef RANGEFINDER_ONLY_FW

	/* Check if rising or falling interrupt */
	if(getIRSensor_BwRight()) {

		/* Nothing detected, all ok */
		Rangefinder_flag_BwAlarmIR = 0;
	}
	else {

		/* Object detected, set alarm */
		Rangefinder_flag_BwAlarmIR = 1;
		/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
		xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
	}
#endif /* RANGEFINDER_ONLY_FW */
}

/**
 * \fn          setSRF08Range
 * \brief       Sets the maximum detection range of the SRF08 sensor module via
 *              I2C. I2C has to be initialised first (use the function initI2C).
 * \note        mHwI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 * \param[in]   range_in_mm Maximum detection range in millimetres (43 to 11008)
 * \return      None
 */
void setSRF08Range(uint8_t slave_address, uint16_t range_in_mm) {

	/* Do nothing, if the semaphore wasn't created correctly */
	if(mHwI2C == NULL) return;

	/* Lower limit: 43 mm */
	if(43 > range_in_mm) range_in_mm = 43;
	/* Higher limit: 11008 mm */
	if(range_in_mm > 11008) range_in_mm = 11008;

	/* Calculate register value with the formula (range = (reg x 43 mm) + 43 mm) */
	float value = ((float)range_in_mm - 43) / 43;

	/* If necessary, round up */
	if((value - (int)value) >= 0.5) value++;

	/* Get semaphore for I2C access, without timeout */
	if(xSemaphoreTake(mHwI2C, portMAX_DELAY) == pdTRUE) {

		/* Send calculated value to the SRF08 to be written in the range register */
		uint8_t buffer = (uint8_t)value;
		writeI2C(slave_address, SRF08_REG_RANGE, &buffer, 1, I2C_TIMEOUT);

		/* Release semaphore */
		xSemaphoreGive(mHwI2C);
	}

	/* Handle i2c error */
	if(i2c_timeout_flag) {
		reinitI2C();
		return;
	}
}

/**
 * \fn          setSRF08Gain
 * \brief       Sets the maximum gain for the analogue stages of the SRF08 sensor module
 *              via I2C. I2C has to be initialised first (use the function initI2C).
 * \note        mHwI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 * \param[in]   gain_value Value for the maximum gain (see datasheet). Value from 0x00 to 0x1F.
 * \return      None
 */
void setSRF08Gain(uint8_t slave_address, uint8_t gain_value) {

	/* Do nothing, if the semaphore wasn't created correctly */
	if(mHwI2C == NULL) return;

	/* Higher limit: 0x1F */
	if(gain_value > 0x1F) gain_value = 0x1F;

	/* Get semaphore for I2C access, without timeout */
	if(xSemaphoreTake(mHwI2C, portMAX_DELAY) == pdTRUE) {

		/* Send the value to the SRF08 to be written in the gain register */
		uint8_t buffer = gain_value;
		writeI2C(slave_address, SRF08_REG_GAIN, &buffer, 1, I2C_TIMEOUT);

		/* Release semaphore */
		xSemaphoreGive(mHwI2C);
	}

	/* Handle i2c error */
	if(i2c_timeout_flag) {
		reinitI2C();
		return;
	}
}

/**
 * \fn          startSRF08Meas
 * \brief       Start the ultrasonic measure of the SRF08 sensor module via I2C.
 *              I2C has to be initialised first (use the function initI2C).
 * \note        mHwI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 *              meas_mode Data to measure: SRF08_MEAS_IN, SRF08_MEAS_CM or SRF08_MEAS_US.
 * \return      None
 */
void startSRF08Meas(uint8_t slave_address, uint8_t meas_mode) {

	/* Do nothing, if the semaphore wasn't created correctly */
	if(mHwI2C == NULL) return;

	/* Get semaphore for I2C access, without timeout */
	if(xSemaphoreTake(mHwI2C, portMAX_DELAY) == pdTRUE) {

		/* Send the command to measure in cm to the SRF08 */
		writeI2C(slave_address, SRF08_REG_CMD, &meas_mode, 1, I2C_TIMEOUT);

		/* Release semaphore */
		xSemaphoreGive(mHwI2C);
	}

	/* Handle i2c error */
	if(i2c_timeout_flag) {
		reinitI2C();
		return;
	}
}

/**
 * \fn          readSRF08Meas
 * \brief       Read the distance measured by the SRF08 sensor module (started
 *              with startSRF08Meas) via I2C.
 *              I2C has to be initialised first (use the function initI2C).
 * \note        Wait at least 65 ms between startSRF08Meas and readSRF08Meas,
 *              because the module is busy waiting for the echo and may lock I2C.
 *              mHwI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 * \return      meassure Measured distance/time, 0xFFFF if something is not ok.
 */
uint16_t readSRF08Meas(uint8_t slave_address) {

	/* Return error, if the semaphore wasn't created correctly */
	if(mHwI2C == NULL) return 0xFFFF;

	uint8_t buffer;
	uint16_t meassure;

	/* Get semaphore for I2C access, without timeout */
	if(xSemaphoreTake(mHwI2C, portMAX_DELAY) == pdTRUE) {

		/* Send address of the register we want to read (highbyte of the 1. echo) */
		writeI2C(slave_address, SRF08_REG_H(1), &buffer, 0, I2C_TIMEOUT); /* buffer not used, because NumByteToWrite is 0 */
		/* Read register */
		readI2C(slave_address, &buffer, 1, I2C_TIMEOUT);

		/* Release semaphore */
		xSemaphoreGive(mHwI2C);
	}

	/* Handle i2c error */
	if(i2c_timeout_flag) {
		reinitI2C();
		return 0xFFFF;
	}

	/* Store highbyte */
	meassure = buffer << 8;

	/* Get semaphore for I2C access, without timeout */
	if(xSemaphoreTake(mHwI2C, portMAX_DELAY) == pdTRUE) {

		/* Send address of the register we want to read (lowbyte of the 1. echo) */
		writeI2C(slave_address, SRF08_REG_L(1), &buffer, 0, I2C_TIMEOUT); /* buffer not used, because NumByteToWrite is 0 */
		/* Read register */
		readI2C(slave_address, &buffer, 1, I2C_TIMEOUT);

		/* Release semaphore */
		xSemaphoreGive(mHwI2C);
	}

	/* Handle i2c error */
	if(i2c_timeout_flag) {
		reinitI2C();
		return 0xFFFF;
	}

	/* Store lowbyte */
	meassure = (meassure & 0xFF00) | (buffer & 0x00FF);

	return meassure;
}

/**
 * \fn          suspendRangefinderTask
 * \brief       Suspend the rangefinder task and do some configuration.
 *              To resume, use default freeRTOS function (vTaskResume).
 *
 * \param[in]   None
 * \return      None
 */
void suspendRangefinderTask(void) {

	/* Suspend task */
	vTaskSuspend(xRangefinderTask_Handle);

	/* Reset variables for comparing the last three values */
	flag_FwAlarmUS_last[0] = 0;
	flag_FwAlarmUS_last[1] = 0;
#ifndef RANGEFINDER_ONLY_FW
	flag_BwAlarmUS_last[0] = 0;
	flag_BwAlarmUS_last[1] = 0;
#endif /* RANGEFINDER_ONLY_FW */

	/* Reset alarm flags */
	Rangefinder_flag_FwAlarmIR = 0;
	Rangefinder_flag_BwAlarmIR = 0;
	Rangefinder_flag_FwAlarmUS = 0;
	Rangefinder_flag_BwAlarmUS = 0;
	Rangefinder_flag_SeAlarmUS = 0;
}


/**
 * \fn          isRobotInFront
 * \brief       Function to check if an enemy/confederate is in front of robot within range
 *
 * \param[in]   game_state_t* game_state Game infos (navi)
 * \return      boolean
 */
boolean isRobotInFront(volatile game_state_t* game_state, uint8_t n_enemies, uint8_t n_friends) {

    /* local variables */
    int16_t delta_x, delta_y;
    uint16_t distance, distance_treshold;
    int16_t alpha, phi;

	/* Copy current game state, so it wont be changed during calculation */
	taskENTER_CRITICAL();
	game_state_t game_state_copy = *game_state;
	taskEXIT_CRITICAL();

	/* Check 0, 1 or both enemies */
	int8_t current_robot_check;  /* Signed, so in worst case it starts on negative numbers and thus still does the loop */
	for(current_robot_check = 1-n_friends;  /* Start on 0 if we have a confederate robot */
			current_robot_check <= n_enemies; current_robot_check++)
	{
		/* Get deltas and distance for current robot */
		/* If we have a confederate robot, the counting starts with 0 */
		if(current_robot_check == 0) {

			delta_x = game_state_copy.confederate_x - game_state_copy.x;
			delta_y = game_state_copy.confederate_y - game_state_copy.y;
			distance_treshold = ROBOT_B52_RADIUS + ROBOT_BALLERINA_RADIUS + RANGEFINDER_THRESHOLD_FW*10;
		}
		/* Enemies always start with 1 */
		else if(current_robot_check == 1) {

			delta_x = game_state_copy.enemy_1_x - game_state_copy.x;
			delta_y = game_state_copy.enemy_1_y - game_state_copy.y;
			distance_treshold = game_state_copy.enemy_1_diameter/2 + ROBOT_BALLERINA_RADIUS + RANGEFINDER_THRESHOLD_FW*10;
		}
		else if(current_robot_check == 2) {

			delta_x = game_state_copy.enemy_2_x - game_state_copy.x;
			delta_y = game_state_copy.enemy_2_y - game_state_copy.y;
			distance_treshold = game_state_copy.enemy_2_diameter/2 + ROBOT_BALLERINA_RADIUS + RANGEFINDER_THRESHOLD_FW*10;
		}
		/* Else:
		 *  current_robot_check > 2: (More than 2 enemies)      Not possible in eurobot 2014 scenario
		 *  current_robot_check < 0: (More than 1 confederate)  Not possible in eurobot 2014 scenario */

		/* Calculate distance to the enemy (mm) */
		distance = round(sqrt(delta_x*delta_x + delta_y*delta_y));

		/* Check if a robot is within threshold range (mm) */
		if(distance <= distance_treshold) {

			/* Convert angle to -180 <= alpha < 180 */
			if(game_state_copy.angle < 180) {
				alpha = game_state_copy.angle;
			}
			else {
				alpha = game_state_copy.angle - 360;
			}
			/* Calculate the angle to the enemy (relative the map grid, -180 <= phi < 180) */
			phi = round(atan2f(delta_y, delta_x)/M_PI*180);
			/* Calculate the angle to the enemy (relative to our angle, -180 <= phi < 180) */
			phi = phi-alpha;

			/* Check if the enemy is within our angle */
			if(fabs(phi) <= RANGEFINDER_ANGLE) {
				return TRUE;
			}
		}
	}

	return FALSE;
}


/**
 * @}
 */
