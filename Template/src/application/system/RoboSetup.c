/**
 * \file    RoboSetup.c
 * \author  gross10
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy setup sate
 *
 * \ingroup strategy
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* application */
#include "../AppConfig.h"
#include "../CANGatekeeper.h"
#include "RoboInitialisation.h" /* next state if the notstop switch is active */
#include "RoboRun.h" /* next state if this one is completed successfully */
#include "RoboSetup.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    TEAMCOLOR = 0,
    ENEMY_QUANTITY,
    CONDEDERATE_QUNATITY,
    ENEMY_1_SIZE,
    ENEMY_2_SIZE,
    START_NODE,
    SET_STARTCONFIG
}robo_setup_t;

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
xSemaphoreHandle sELP = NULL;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboSetupState
 * \brief   initialisation of the RoboSetupState
 *
 * \note    have to be called from the initSystem function
 */
void initRoboSetupState()
{
    /* create semaphore for elp task synchronisation */
    vSemaphoreCreateBinary(sELP); /* bug in FreeRTOS -> can't create a binary semaphore */
    xSemaphoreTake(sELP,0); /* take the semaphore */
}


/**
 * \fn      runRoboSetupState
 * \brief   run the runRoboSetupState
 *
 * \param[in]   tick    pointer to systick timer for absolut delays
 * \todo    menu flow
 */
void runRoboSetupState(portTickType* tick)
{
    static robo_setup_t state = TEAMCOLOR;



}
/**
 * @}
 */
