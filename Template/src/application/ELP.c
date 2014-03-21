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
#include "app_config.h"
#include "CANGatekeeper.h"
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
#define CYCLE_RESET  24  /* Number of cycles until cycle-counter reset */

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static const cycle_table_t cycle_table[] =
{
    {0,4,txNaviPositionRequest},
    {2,6,txKalmanPositionRequest},
    {4,6,txConfederatePositionRequest},
    {6,8,txEnemey1PositionRequest},
    {8,8,txEnemey2PositionRequest},

    {0,0,LAST_ENTRY}
};

/* Private function prototypes -----------------------------------------------*/
static void vELPTask(void*);

/* Private functions ---------------------------------------------------------*/


/**
 * \fn          initELPTask
 * \brief       Initialisation of the ELP Task
 *
 * \param[in]   None
 * \return      None
 */
void initELPTask(void)
{
    /* create the task */
    xTaskCreate( vELPTask, ( signed char * ) ELP_TASK_NAME, ELP_STACK_SIZE, NULL, ELP_TASK_PRIORITY, NULL );

}

/*
 * \fn          vELPTask
 * \brief       Task to pull the ELP-MSG's
 *
 * \param[in]   None
 * \return      None
 */
static void vELPTask(void* pvParameters )
{
    /* locale variable */
    portTickType xLastFlashTime;
    uint8_t cyle_count = 0;
    uint8_t table_entry = 0;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* endless */
    for(;;)
    {
        table_entry = 0;    /* Start with first table entry */

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

        /* wait for ELP_TASK_SPEED (should be 100ms) */
        vTaskDelayUntil( &xLastFlashTime, ELP_TASK_SPEED / portTICK_RATE_MS);
    }
}

/**
 * @}
 */
