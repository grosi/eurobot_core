/**
 * \file    exti_sensor.h
 * \author  gross10
 * \date    2014-02-26
 *
 * \version 1.1
 *  added interrupt defines (kasen1)
 * \version 1.0
 *  create this file
 *
 * \brief   Sensor access for the ir sensor from the rangefinder module.
 *
 * \todo    unit test
 *
 * \defgroup ir_sensor IR_Sensor
 * \brief   ir sensor library
 * \ingroup firmware
 *
 * @{
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EXTI_SENSOR_H_
#define EXTI_SENSOR_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/


/* exported types ------------------------------------------------------------*/


/* exported constants --------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/


/* exported functions --------------------------------------------------------*/
extern void initSensorEXTI_EmergencyStop();
extern void initSensorEXTI_Fresco_1();
extern void initSensorEXTI_Fresco_2();
extern void initSensorEXTI_Wall();
extern void initIRSensorEXTI_BwLeft();
extern void initIRSensorEXTI_FwLeft();
extern void initIRSensorEXTI_BwRight();
extern void initIRSensorEXTI_FwRight();
extern void initSensorEXTI_Key();

#endif /* EXTI_SENSOR_H_ */

/**
 * @}
 */
