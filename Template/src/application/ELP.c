/**
 * \file    ELP.h
 * \author  meert1
 * \date    2013-11-29
 *
 * \version 1.0
 *  create this file
 *
 * \brief   task to pull the ELP's
 *
 * \defgroup elp ELP
 * \brief   ELP task
 * \ingroup app_task
 *
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* application */
#include "AppConfig.h"
#include "CANGatekeeper.h"
#include "system/RoboSetup.h"
#include "ELP.h"

/* Private typedef -----------------------------------------------------------*/
/**
 * \brief struct describing a table entry for cyclical OS
 */
typedef struct
{
    uint8_t first_activation;   /* Cycle when action is called first time */
    uint8_t cyle_activation;    /* Periodic cycle */
    void (*elp_function)(void); /* Function pointer to action */
}cycle_table_t;


/* Private define ------------------------------------------------------------*/
#define ELP_CAN_ON
#define LAST_ENTRY   0  /* Last entry for table_entry table[]         */
#define CYCLE_RESET  5
/* Number of cycles until cycle-counter reset */

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static const cycle_table_t cycle_table[] =
{
    {ELP_NAVI_POSITION_REQUEST_START,ELP_NAVI_POSITION_REQUEST_RATE,txNaviPositionRequest},
    {ELP_KALMAN_POSITION_REQUEST_START,ELP_KALMAN_POSITION_REQUEST_RATE,txKalmanPositionRequest},
    {ELP_CONFEDERATE_POSITION_REQUEST_START,ELP_CONFEDERATE_POSITION_REQUEST_RATE,txConfederatePositionRequest},
    {ELP_ENEMY1_POSITION_REQUEST_START,ELP_ENEMY1_POSITION_REQUEST_RATE,txEnemey1PositionRequest},
    {ELP_ENEMY2_POSITION_REQUEST_START,ELP_ENEMY2_POSITION_REQUEST_RATE,txEnemey2PositionRequest},

    {0,0,LAST_ENTRY}
};

static xTimerHandle xELPTimer;

/* Private function prototypes -----------------------------------------------*/
static void vELPTimerCallback(xTimerHandle);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initELPTask
 * \brief       Initialisation of the ELP Task
 *
 * \param[in]   None
 * \return      None
 */
void initELPTimer(void)
{
    /* create sw-timer */
    xELPTimer = xTimerCreate(( signed char * )ELP_NAME,
            ELP_PERIODE / portTICK_RATE_MS,
            pdTRUE,
            NULL,
            vELPTimerCallback);
}


/**
 * \fn          vELPTimerCallback
 * \brief       Time base for the cycle ELP tasks
 *
 * \param[in]   None
 * \return      None
 */
static void vELPTimerCallback(xTimerHandle pxTimer)
{
    /* locale variable */
    static uint8_t cyle_count = 0;
    uint8_t table_entry = 0; /* Start with first table entry */


    /* Process table and call task if necessary */
    /* Still task in table?                     */
    while (cycle_table[table_entry].elp_function != LAST_ENTRY)
    {
        /* Modulo 0 yields undefined result! Prevent this */
        if(cycle_table[table_entry].cyle_activation != 0)
        {
            /* Call  task? */
            if (((cyle_count % cycle_table[table_entry].cyle_activation) -
                    (cycle_table[table_entry].first_activation)) == 0)
            {
#ifdef ELP_CAN_ON
                (*cycle_table[table_entry].elp_function)();
#endif
            }
        }
        table_entry++;  /* Prepare next task */
    }

    /* Prepare next cycle and check if counter has to be reset */
    cyle_count++;
    if (cyle_count >= CYCLE_RESET) {
        cyle_count = 0;
    }
}


/**
 * \fn          startELP
 * \brief       starts the elp-timer
 *
 * \param[in]   None
 * \return      None
 */
inline void startELP(void)
{
    /* Start the timer */
    xTimerStart(xELPTimer,0);
}


/**
 * \fn          stopELP
 * \brief       stops the elp-timer
 *
 * \param[in]   None
 * \return      None
 */
inline void stopELP(void)
{
    /* Stop the timer */
    xTimerStop(xELPTimer,0);
}

/**
 * @}
 */
