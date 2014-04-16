/**
 * \file     Rangefinder.c
 * \author   kasen1
 * \date     2013-04-13
 *
 * \version 1.2
 *  - IR sensors in new arrangement
 *  - Added flag for separation blocked alarm
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
#define SRF08_MEAS_CM   0x51            /* Measure in centimeters */
#define SRF08_MEAS_US   0x52            /* Measure in micro-seconds */
#define SRF08_ANN_IN    0x53            /* Measure in inches (ANN mode) */
#define SRF08_ANN_CM    0x54            /* Measure in centimeters (ANN mode) */
#define SRF08_ANN_US    0x55            /* Measure in micro-seconds (ANN mode) */


/* Private variables ---------------------------------------------------------*/
/* Variables for comparing the last three values */
volatile uint8_t flag_FwAlarmIR_last[2] = {0,0};
volatile uint8_t flag_BwAlarmIR_last[2] = {0,0};
volatile uint8_t flag_FwAlarmUS_last[2] = {0,0};
volatile uint8_t flag_BwAlarmUS_last[2] = {0,0};

/* Sensor values */
uint16_t distance_fw;                   /* Variable for the distance detected by the fowrward SRF08 (lower- /higher byte joined) */
uint16_t distance_bw;                   /* Variable for the distance detected by the backward SRF08 (lower- /higher byte joined) */

/* Globale variables ---------------------------------------------------------*/
/* Mutex for I2C */
xSemaphoreHandle mHwI2C = NULL;
xSemaphoreHandle sSyncNodeTask = NULL;

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
    xTaskCreate( vRangefinderTask, ( signed char * ) RANGEFINDER_TASK_NAME, RANGEFINDER_STACK_SIZE, NULL, RANGEFINDER_TASK_PRIORITY, NULL );
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
    setSRF08Range(SRF08_ADDR_BW, RANGEFINDER_RANGE * 10); /* In mm */

    /* Set the gain register to evaluated value */
    setSRF08Gain(SRF08_ADDR_FW, 0x1F);
    setSRF08Gain(SRF08_ADDR_BW, 0x1F);

    for(EVER) {

        /* Start the ultrasonic measures */
        startSRF08Meas(SRF08_ADDR_FW, SRF08_MEAS_CM);
        startSRF08Meas(SRF08_ADDR_BW, SRF08_MEAS_CM);

        /* Wait 70 ms, for the sound to travel s_max (11 m) twice (=ca. 64.1 ms), and rounded up (65 ms still made trouble sometimes). */
        vTaskDelay(70 / portTICK_RATE_MS);

        /* Get distance from the modules */
        distance_fw = readSRF08Meas(SRF08_ADDR_FW);
        distance_bw = readSRF08Meas(SRF08_ADDR_BW);

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
}

/**
 * \fn
 * \brief  This function is called by the external line 10-15 interrupt handler
 *
 * \param  None
 * \retval None
 */
void IRSensorBwRight_IT(void) {

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
 * @}
 */
