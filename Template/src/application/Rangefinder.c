/**
 * \file    Rangefinder.c
 * \author  kasen1
 * \date    2013-11-29
 *
 * \version 1.0
 *  Softwared tested (14.01.2014)
 * \version 1.0
 *  import from template (14.01.2014)
 *
 * \brief   task for the rangefinder sensors
 *
 * \defgroup rangefinder Rangefinder
 * \brief   Rangefinder task
 * \ingroup hw_task
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* RTOS */
#include <memPoolService.h>         /* Memory pool manager service */

/* HW-library */
#include "../lib/i2c.h"
#include "../lib/ir_sensor.h"
#include "../lib/ext_interrupt.h"
#include "../lib/led.h" //TODO: Remove if not used anymore

/* application */
#include "app_config.h"
#include "Rangefinder.h"

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
/* Common */
xSemaphoreHandle xSemaphoreI2C = NULL; //TODO: Define extern (main etc)

/* Globale variables ---------------------------------------------------------*/
/* Alarm flags, 1 if object detected, 0 if no object detected */
volatile uint8_t RangefinderIR_FwAlarm_flag = 0; /* Infrared forward alarm */
volatile uint8_t RangefinderIR_BwAlarm_flag = 0; /* Infrared backward alarm */
volatile uint8_t RangefinderUS_FwAlarm_flag = 0; /* Ultrasonic forward alarm */
volatile uint8_t RangefinderUS_BwAlarm_flag = 0; /* Ultrasonic backward alarm */
volatile uint8_t shadow1 = 0; //TODO: Remove
volatile uint8_t shadow2 = 0; //TODO: Remove
volatile uint8_t shadow3 = 0; //TODO: Remove
volatile uint8_t shadow4 = 0; //TODO: Remove

/* Sensor values */
uint16_t distance_fw;                   /* Variable for the distance detected by the fowrward SRF08 (lower- /higher byte joined) */
uint16_t distance_bw;                   /* Variable for the distance detected by the backward SRF08 (lower- /higher byte joined) */


/* Private function prototypes -----------------------------------------------*/
static void vRangefinderTask(void*);

/* Private functions ---------------------------------------------------------*/

/**
 * \fn          setSRF08Range
 * \brief       Sets the maximum detection range of the SRF08 sensor module via
 *              I2C. I2C has to be initialised first (use the function initI2C).
 * \note        xSemaphoreI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 * \param[in]   range_in_mm Maximum detection range in millimetres (43 to 11008)
 * \return      None
 */
void setSRF08Range(uint8_t slave_address, uint16_t range_in_mm) {

    /* Do nothing, if the semaphore wasn't created correctly */
    if(xSemaphoreI2C == NULL) return;

    /* Lower limit: 43 mm */
    if(43 > range_in_mm) range_in_mm = 43;
    /* Higher limit: 11008 mm */
    if(range_in_mm > 11008) range_in_mm = 11008;

    /* Calculate register value with the formula (range = (reg x 43 mm) + 43 mm) */
    float value = ((float)range_in_mm - 43) / 43;

    /* If necessary, round up */
    if((value - (int)value) >= 0.5) value++;

    /* Get semaphore for I2C access, without timeout */
    if(xSemaphoreTake(xSemaphoreI2C, portMAX_DELAY) == pdTRUE) {

        /* Send calculated value to the SRF08 to be written in the range register */
        uint8_t buffer = (uint8_t)value;
        writeI2C(slave_address, SRF08_REG_RANGE, &buffer, 1, I2C_TIMEOUT);

        /* Release semaphore */
        xSemaphoreGive(xSemaphoreI2C);
    }
}

/**
 * \fn          setSRF08Gain
 * \brief       Sets the maximum gain for the analogue stages of the SRF08 sensor module
 *              via I2C. I2C has to be initialised first (use the function initI2C).
 * \note        xSemaphoreI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 * \param[in]   gain_value Value for the maximum gain (see datasheet). Value from 0x00 to 0x1F.
 * \return      None
 */
void setSRF08Gain(uint8_t slave_address, uint8_t gain_value) {

    /* Do nothing, if the semaphore wasn't created correctly */
    if(xSemaphoreI2C == NULL) return;

    /* Higher limit: 0x1F */
    if(gain_value > 0x1F) gain_value = 0x1F;

    /* Get semaphore for I2C access, without timeout */
    if(xSemaphoreTake(xSemaphoreI2C, portMAX_DELAY) == pdTRUE) {

        /* Send the value to the SRF08 to be written in the gain register */
        uint8_t buffer = gain_value;
        writeI2C(slave_address, SRF08_REG_GAIN, &buffer, 1, I2C_TIMEOUT);

        /* Release semaphore */
        xSemaphoreGive(xSemaphoreI2C);
    }
}

/**
 * \fn          startSRF08Meas
 * \brief       Start the ultrasonic measure of the SRF08 sensor module via I2C.
 *              I2C has to be initialised first (use the function initI2C).
 * \note        xSemaphoreI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 *              meas_mode Data to measure: SRF08_MEAS_IN, SRF08_MEAS_CM or SRF08_MEAS_US.
 * \return      None
 */
void startSRF08Meas(uint8_t slave_address, uint8_t meas_mode) {

    /* Do nothing, if the semaphore wasn't created correctly */
    if(xSemaphoreI2C == NULL) return;

    /* Get semaphore for I2C access, without timeout */
    if(xSemaphoreTake(xSemaphoreI2C, portMAX_DELAY) == pdTRUE) {

        /* Send the command to measure in cm to the SRF08 */
        writeI2C(slave_address, SRF08_REG_CMD, &meas_mode, 1, I2C_TIMEOUT);

        /* Release semaphore */
        xSemaphoreGive(xSemaphoreI2C);
    }
}

/**
 * \fn          readSRF08Meas
 * \brief       Read the distance measured by the SRF08 sensor module (started
 *              with startSRF08Meas) via I2C.
 *              I2C has to be initialised first (use the function initI2C).
 * \note        Wait at least 65 ms between startSRF08Meas and readSRF08Meas,
 *              because the module is busy waiting for the echo and may lock I2C.
 *              xSemaphoreI2C has to be created (e.g. use xSemaphoreCreateMutex()).
 *              Waits without timeout (portMAX_DELAY) to get the semaphore.
 *
 * \param[in]   slave_address Address of the device. Values: 0xE0, 0xE2, 0xE4, ... , 0xFE.
 * \return      meassure Measured distance/time, 0xFFFF if something is not ok.
 */
uint16_t readSRF08Meas(uint8_t slave_address) {

    /* Return error, if the semaphore wasn't created correctly */
    if(xSemaphoreI2C == NULL) return 0xFFFF;

    uint8_t buffer;
    uint16_t meassure;

    /* Get semaphore for I2C access, without timeout */
    if(xSemaphoreTake(xSemaphoreI2C, portMAX_DELAY) == pdTRUE) {

        /* Send address of the register we want to read (highbyte of the 1. echo) */
        writeI2C(slave_address, SRF08_REG_H(1), &buffer, 0, I2C_TIMEOUT); /* buffer not used, because NumByteToWrite is 0 */
        /* Read register */
        readI2C(slave_address, &buffer, 1, I2C_TIMEOUT);

        /* Release semaphore */
        xSemaphoreGive(xSemaphoreI2C);
    }
    if(i2c_timeout_flag) return 0xFFFF;
    /* Store highbyte */
    meassure = buffer << 8;

    /* Get semaphore for I2C access, without timeout */
    if(xSemaphoreTake(xSemaphoreI2C, portMAX_DELAY) == pdTRUE) {

        /* Send address of the register we want to read (lowbyte of the 1. echo) */
        writeI2C(slave_address, SRF08_REG_L(1), &buffer, 0, I2C_TIMEOUT); /* buffer not used, because NumByteToWrite is 0 */
        /* Read register */
        readI2C(slave_address, &buffer, 1, I2C_TIMEOUT);

        /* Release semaphore */
        xSemaphoreGive(xSemaphoreI2C);
    }
    if(i2c_timeout_flag) return 0xFFFF;
    /* Store lowbyte */
    meassure = (meassure & 0xFF00) | (buffer & 0x00FF);

    return meassure;
}

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
    initIRSensors();
    /* Configure EXTI Line in interrupt mode */
    initIREXTILines();

    /* Init Roboboard LEDs */
    initLED_all(); //TODO: Remove if not used

    /* For evaluation: Generate software interrupt: simulate a rising edge applied on EXTI0 line */
    EXTI_GenerateSWInterrupt(EXTI_Line5);

    /* US: I2C */
    initI2C();

    /* Create semaphore for I2C */ //TODO: Move to extern (main etc)
    xSemaphoreI2C = xSemaphoreCreateMutex();

    /* Create the tasks */
    xTaskCreate( vRangefinderTask, ( signed char * ) RANGEFINDER_TASK_NAME, RANGEFINDER_STACK_SIZE, NULL, RANGEFINDER_TASK_PRIORITY, NULL );
}

//Old IR task:
///**
// * \fn          vRangefinderTaskIR
// * \brief       Task to watch the near range in front and back of the robot
// *              by infrared
// *
// * \param[in]   None
// * \return      None
// */
//static void vRangefinderTaskIR(void* pvParameters ) {
//
//    portTickType xLastFlashTime;
//
//    /* We need to initialise xLastFlashTime prior to the first call to vTaskDelayUntil() */
//    xLastFlashTime = xTaskGetTickCount();
//
//    for(EVER) {
//
//        /* Check front */
//        /* Get IR sensor values, AND because TRUE if object detected, FALSE if no object detected */
//        if(getIRSensor_Left() && getIRSensor_Front() && getIRSensor_Right()) {
//
//            /* Nothing detected, all ok */
//            RangefinderIR_FwAlarm_flag = 0;
//        }
//        else {
//
//            /* Object detected, set alarm */
//            RangefinderIR_FwAlarm_flag = 1;
//        }
//
//        /* Check rear */
//        /* Get IR sensor values */
//        if(getIRSensor_Back()) {
//
//            /* Nothing detected, all ok */
//            RangefinderIR_BwAlarm_flag = 0;
//        }
//        else {
//
//            /* Object detected, set alarm */
//            RangefinderIR_BwAlarm_flag = 1;
//        }
//
//        /* Delay until defined time passed */
//        vTaskDelayUntil( &xLastFlashTime, RANGEFINDER_DELAY / portTICK_RATE_MS);
//    }
//}

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

        /* Wait 65 ms, for the sound to travel s_max (11 m) twice (=ca. 64.1 ms), and rounded up to 65 ms. */
        vTaskDelay(65 / portTICK_RATE_MS);

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

            /* Object detected, set alarm */
            RangefinderUS_FwAlarm_flag = 1;
        }
        else {

            /* Nothing detected, all ok */
            RangefinderUS_FwAlarm_flag = 0;
        }

        /* Check rear */
        /* Check for error */
        if(distance_bw == 0xFFFF) {
            /* ERROR (Semaphore not created correctly or timeout) */
        }
        /* Check if an obstacle is to close */
        else if(distance_bw != 0 && distance_bw < RANGEFINDER_THRESHOLD_BW) {

            /* Object detected, set alarm */
            RangefinderUS_BwAlarm_flag = 1;
        }
        else {

            /* Nothing detected, all ok */
            RangefinderUS_BwAlarm_flag = 0;
        }

        /* Delay until defined time passed */
        vTaskDelayUntil( &xLastFlashTime, RANGEFINDER_DELAY / portTICK_RATE_MS);
    }
}

/**
 * \fn
 * \brief  This function handles External line 5-9 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		/* Toggle LED */
		//setLEDs_bin(++shadow);
		setLED_1(shadow1);
		shadow1 = !shadow1;

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}

//TODO: Still some problems
/**
 * \fn
 * \brief  This function handles External line 10-15 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) {

		/* Toggle LED */
		//setLEDs_bin(++shadow);
		setLED_2(shadow2);
		shadow2 = !shadow2;

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) {

		/* Toggle LED */
		//setLEDs_bin(++shadow);
		setLED_3(shadow3);
		shadow3 = !shadow4;

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line15) != RESET) {

		/* Toggle LED */
		//setLEDs_bin(++shadow);
		setLED_4(shadow4);
		shadow4 = !shadow4;

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}


/**
 * @}
 */
