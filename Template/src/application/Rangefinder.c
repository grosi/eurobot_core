/**
 * \file    Rangefinder.c
 * \author  kasen1
 * \date    2013-05-18
 *
 * \version 2.1b
 *  - Updated function to compare rangefinder with navigation informations
 *  - Changed all distances from cm to mm
 * \version 2.0b
 *  - Changed to two ultrasonic sensor in front (for the big robot "B52")
 *  - Removed code for IR-Sensors, as they're not used in this robot
 *  - Updated navigation comparing function with option to check backwards
 *  - Version tested (2013-05-18)
 * \version 1.3
 *  - Added function to compare rangefinder with navigation informations
 *  - Releasing semaphore on i2c error for safety reason
 *  - Version tested (2013-05-13)
 * \version 1.2
 *  - IR sensors in new arrangement
 *  - Added flag for separation blocked alarm
 *  - Added option to only use forward rangefinder by defining RANGEFINDER_ONLY_FW
 *  - Task is suspended at beginning, so ultrasonic only running when really used
 *  - Semaphore for synchronising with node task added (not in test protocol, but is tested)
 *  - Version tested (2013-05-01)
 * \version 1.1
 *  - Changed implementation of IR detection to external interrupt
 *  - Implemented comparison of last three US measures
 *  - Fixed I2C error, where the bus was always busy after disconnecting a slave
 *  - Version tested (2013-03-12)
 * \version 1.0
 *  - Implemented first version
 *  - Version tested (2013-01-14)
 * \version 0.1
 *  - Import from template (2013-01-14)
 *
 * \brief    task for the rangefinder sensors and helper functions
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
#define SRF08_ADDR_L    0xE0            /* Slave address of the SRF08 on the left */
#define SRF08_ADDR_R    0xE2            /* Slave address of the SRF08 on the right */

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
#define ROBOT_OUR_RADIUS           230  /* Distance from nose to center (navigation) in mm */
#define ROBOT_CONFEDERATE_RADIUS   120  /* Distance from nose to center (navigation) in mm */


/* Private variables ---------------------------------------------------------*/
/* Variables for comparing the last three values */
volatile uint8_t flag_LAlarmUS_last[2] = {0,0};
volatile uint8_t flag_RAlarmUS_last[2] = {0,0};

/* Sensor values */
uint16_t distance_l;                   /* Variable for the distance detected by the left SRF08 (lower- /higher byte joined) */
uint16_t distance_r;                   /* Variable for the distance detected by the right SRF08 (lower- /higher byte joined) */

/* Globale variables ---------------------------------------------------------*/
/* RTOS */
xTaskHandle xRangefinderTask_Handle = NULL;
xSemaphoreHandle mHwI2C = NULL;  /* Mutex for I2C */
xSemaphoreHandle sSyncNodeTask = NULL;  /* Semaphore for node task synchronisation */

/* Alarm flags, 1 if object detected, 0 if no object detected */
volatile uint8_t Rangefinder_flag_LAlarmUS = 0; /* Ultrasonic left alarm */
volatile uint8_t Rangefinder_flag_RAlarmUS = 0; /* Ultrasonic right alarm */
volatile uint8_t Rangefinder_flag_FiAlarmUS = 0; /* Ultrasonic fire node alarm */

/* Private function prototypes -----------------------------------------------*/
static void vRangefinderTask(void*);
void setSRF08Range(uint8_t slave_address, uint16_t range_in_mm);
void setSRF08Gain(uint8_t slave_address, uint8_t gain_value);
void startSRF08Meas(uint8_t slave_address, uint8_t meas_mode);
uint16_t readSRF08Meas(uint8_t slave_address);
void checkLeftUS(void);
void checkRightUS(void);

/**
 * \fn          initRangefinderTasks
 * \brief       Initialisation of the Rangefinder Tasks
 *
 * \param[in]   None
 * \return      None
 */
void initRangefinderTask(void) {

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
	setSRF08Range(SRF08_ADDR_L, RANGEFINDER_RANGE);
	setSRF08Range(SRF08_ADDR_R, RANGEFINDER_RANGE);

	/* Set the gain register to evaluated value */
	setSRF08Gain(SRF08_ADDR_L, 0x1F);
	setSRF08Gain(SRF08_ADDR_R, 0x1F);

	/* Suspend ourselves, so the task is only running when really used.
	 * This way there is less possible ultrasonic disturbance (navigation and other robot),
	 * infrared is still running. */
	vTaskSuspend(NULL);

	/* Use variable to toggle between left and right ultrasonic measure */
	boolean toggle_var = TRUE;

	for(EVER) {

		/* Toggle left and right measure, so the ultrasonic sounds don't interfere with each other */
		if(toggle_var) {
			checkLeftUS();
			toggle_var = !toggle_var;
		}
		else {
			checkRightUS();
			toggle_var = !toggle_var;
		}

		/* Delay until defined time passed */
		vTaskDelayUntil( &xLastFlashTime, RANGEFINDER_DELAY / portTICK_RATE_MS);
	}
}


/**
 * \fn          checkLeftUS
 * \brief       Handles the left ultrasonic rangefinder
 *
 * \param[in]   None
 * \return      None
 */
void checkLeftUS(void) {

	/* Start the ultrasonic measure */
	startSRF08Meas(SRF08_ADDR_L, SRF08_MEAS_CM);

	/* Wait 70 ms, for the sound to travel s_max (11 m) twice (=ca. 64.1 ms), and rounded up (65 ms still made trouble sometimes). */
	vTaskDelay(70 / portTICK_RATE_MS);

	/* Get distance from the module */
	distance_l = readSRF08Meas(SRF08_ADDR_L);

	/* Check distance */
	/* Check for error */
	if(distance_l == 0xFFFF) {

		/* ERROR (Semaphore not created correctly or timeout) */

		/* Release semaphore to so the range is at least checked by navigation,
		 * "FromISR" because it's possible the semaphore is released already */
		xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
	}
	/* Check if an obstacle is to close */
	else if(distance_l != 0 && distance_l < RANGEFINDER_THRESHOLD_FW/10) {

		/* Compare last three measures, only set alarm if at least two were positive.
		 * Inside of this block the current measure is positive, so check if at least one of the last two was positive too */
		if(flag_LAlarmUS_last[1] || flag_LAlarmUS_last[0])
		{
			/* Two of three were positive, set alarm (object detected) */
			Rangefinder_flag_LAlarmUS = 1;
			/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
			xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
		}
		else
		{
			/* Two of three were negative, so don't set the alarm yet. */
			Rangefinder_flag_LAlarmUS = 0;
		}

		/* Remember measure (NOT flag which is affected by the last two measures) */
		flag_LAlarmUS_last[1] = flag_LAlarmUS_last[0];  /* Move last measure one back */
		flag_LAlarmUS_last[0] = 1;  /* Current measure is positive */
	}
	else {

		/* Compare last three measures, only reset alarm if at least two were negative.
		 * Inside of this block the current measure is negative, so check if at least one of the last two was negative too */
		if(!(flag_LAlarmUS_last[1] && flag_LAlarmUS_last[0]))
		{
			/* Two of three were negative, reset alarm  (nothing detected) */
			Rangefinder_flag_LAlarmUS = 0;
		}
		else
		{
			/* Two of three were positive, so don't reset the alarm yet. */
			Rangefinder_flag_LAlarmUS = 1;
			/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
			xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
		}

		/* Remember measure (NOT flag which is affected by the last two measures) */
		flag_LAlarmUS_last[1] = flag_LAlarmUS_last[0];  /* Move last measure one back */
		flag_LAlarmUS_last[0] = 0;  /* Current measure is negative */
	}
	/* Check space for fire node */
	if(distance_l != 0xFFFF) {
		if(distance_l <= RANGEFINDER_THRESHOLD_FI/10) {

			/* Set flag */
			Rangefinder_flag_FiAlarmUS = 1;
		}
		else {

			/* Reset flag */
			Rangefinder_flag_FiAlarmUS = 0;
		}
	}
}


/**
 * \fn          checkRightUS
 * \brief       Handles the right ultrasonic rangefinder
 *
 * \param[in]   None
 * \return      None
 */
void checkRightUS(void) {

	/* Start the ultrasonic measure */
	startSRF08Meas(SRF08_ADDR_R, SRF08_MEAS_CM);

	/* Wait 70 ms, for the sound to travel s_max (11 m) twice (=ca. 64.1 ms), and rounded up (65 ms still made trouble sometimes). */
	vTaskDelay(70 / portTICK_RATE_MS);

	/* Get distance from the module */
	distance_r = readSRF08Meas(SRF08_ADDR_R);

	/* Check distance */
	/* Check for error */
	if(distance_r == 0xFFFF) {

		/* ERROR (Semaphore not created correctly or timeout) */

		/* Release semaphore to so the range is at least checked by navigation,
		 * "FromISR" because it's possible the semaphore is released already */
		xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
	}
	/* Check if an obstacle is to close */
	else if(distance_r != 0 && distance_r < RANGEFINDER_THRESHOLD_FW/10) {

		/* Compare last three measures, only set alarm if at least two were positive.
		 * Inside of this block the current measure is positive, so check if at least one of the last two was positive too */
		if(flag_RAlarmUS_last[1] || flag_RAlarmUS_last[0])
		{
			/* Two of three were positive, set alarm (object detected) */
			Rangefinder_flag_RAlarmUS = 1;
			/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
			xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
		}
		else
		{
			/* Two of three were negative, so don't set the alarm yet. */
			Rangefinder_flag_RAlarmUS = 0;
		}

		/* Remember measure (NOT flag which is affected by the last two measures) */
		flag_RAlarmUS_last[1] = flag_RAlarmUS_last[0];  /* Move last measure one back */
		flag_RAlarmUS_last[0] = 1;  /* Current measure is positive */
	}
	else {

		/* Compare last three measures, only reset alarm if at least two were negative.
		 * Inside of this block the current measure is negative, so check if at least one of the last two was negative too */
		if(!(flag_RAlarmUS_last[1] && flag_RAlarmUS_last[0]))
		{
			/* Two of three were negative, reset alarm  (nothing detected) */
			Rangefinder_flag_RAlarmUS = 0;
		}
		else
		{
			/* Two of three were positive, so don't reset the alarm yet. */
			Rangefinder_flag_RAlarmUS = 1;
			/* Release semaphore to indicate detection of an obstacle, "FromISR" because it's possible the semaphore is released already */
			xSemaphoreGiveFromISR(sSyncNodeTask, NULL);
		}

		/* Remember measure (NOT flag which is affected by the last two measures) */
		flag_RAlarmUS_last[1] = flag_RAlarmUS_last[0];  /* Move last measure one back */
		flag_RAlarmUS_last[0] = 0;  /* Current measure is negative */
	}
	/* Check space for fire node */
	if(distance_r != 0xFFFF) {
		if(distance_r <= RANGEFINDER_THRESHOLD_FI/10) {

			/* Set flag */
			Rangefinder_flag_FiAlarmUS = 1;
		}
		else {

			/* Reset flag */
			Rangefinder_flag_FiAlarmUS = 0;
		}
	}
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
	flag_LAlarmUS_last[0] = 0;
	flag_LAlarmUS_last[1] = 0;
	flag_RAlarmUS_last[0] = 0;
	flag_RAlarmUS_last[1] = 0;

	/* Reset alarm flags */
	Rangefinder_flag_LAlarmUS = 0;
	Rangefinder_flag_RAlarmUS = 0;
}


/**
 * \fn          isRobotInRange
 * \brief       Function to check if an enemy/confederate is in front/front of robot within range
 *
 * \param[in]   game_state_t* game_state Game infos (navi)
 * \retval      0 robot is not in range
 * \retval      1...    robot is in range
 */
uint16_t isRobotInRange(volatile game_state_t* game_state, boolean in_back) {

    /* local variables */
    int16_t delta_x, delta_y;
    uint16_t distance, distance_offset;
    int16_t alpha, phi;

	/* Copy current game state, so it wont be changed during calculation */
	taskENTER_CRITICAL();
	game_state_t game_state_copy = *game_state;
	taskEXIT_CRITICAL();

	/* Check 0, 1 or both enemies */
	int8_t current_robot_check;  /* Signed, so in worst case it starts on negative numbers and thus still does the loop */
	for(current_robot_check = 1-game_state_copy.confederate_count;  /* Start on 0 if we have a confederate robot */
			current_robot_check <= game_state_copy.enemy_count; current_robot_check++)
	{
		/* Get deltas and distance for current robot */
		/* If we have a confederate robot, the counting starts with 0 */
		if(current_robot_check == 0) {

			if(game_state_copy.confederate_x == 0 || game_state_copy.confederate_y == 0)
			{
				continue;
			}
			delta_x = game_state_copy.confederate_x - game_state_copy.x;
			delta_y = game_state_copy.confederate_y - game_state_copy.y;
			distance_offset = ROBOT_CONFEDERATE_RADIUS + ROBOT_OUR_RADIUS;
		}
		/* Enemies always start with 1 */
		else if(current_robot_check == 1) {

			if(game_state_copy.enemy_1_x == 0 || game_state_copy.enemy_1_y == 0)
			{
				continue;
			}
			delta_x = game_state_copy.enemy_1_x - game_state_copy.x;
			delta_y = game_state_copy.enemy_1_y - game_state_copy.y;
			distance_offset = game_state_copy.enemy_1_diameter/2 + ROBOT_OUR_RADIUS;
		}
		else if(current_robot_check == 2) {

			if(game_state_copy.enemy_2_x == 0 || game_state_copy.enemy_2_y == 0)
			{
				continue;
			}
			delta_x = game_state_copy.enemy_2_x - game_state_copy.x;
			delta_y = game_state_copy.enemy_2_y - game_state_copy.y;
			distance_offset = game_state_copy.enemy_2_diameter/2 + ROBOT_OUR_RADIUS;
		}
		/* Else:
		 *  current_robot_check > 2: (More than 2 enemies)      Not possible in eurobot 2014 scenario
		 *  current_robot_check < 0: (More than 1 confederate)  Not possible in eurobot 2014 scenario */

		/* Calculate distance to the enemy (mm) */
		distance = round(sqrt(delta_x*delta_x + delta_y*delta_y));

		/* Check if a robot is within threshold range (mm) */
		if(distance <= distance_offset + RANGEFINDER_THRESHOLD_FW) {

			/* Convert (0 <= angle < 360) to (-180 <= alpha < 180) */
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

			/* If we have to check the back, rotate phi +180 */
			if(in_back) {
				phi += 180;
				if(phi >= 180) {
					phi -= 360;
				}
			}

			/* Check if the enemy is within our angle */
			if(fabs(phi) <= RANGEFINDER_ANGLE) {
				/* Return the external distance (without radiuses) */

				//TODO Please Test: Handle bug when distance_offset >= distance (because of wrong display input)
				if(distance <= distance_offset)
				{
					return 1;
				}
				else
				{
					return (distance - distance_offset);
				}
			}
		}
	}

	return 0;
}


/**
 * @}
 */
