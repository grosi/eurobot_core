/**
 * \file    RoboSetup.c
 * \author  haldj3
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
#include "../System.h"
#include "../Timer.h"
#include "../ELP.h"
#include "RoboInitialisation.h" /* next state if the notstop switch is active */
#include "RoboRun.h" /* next state if this one is completed successfully */
#include "RoboSetup.h"

/* lib */
#include "lib/button.h"
#include "lib/display.h"
#include "lib/sensor.h"


/* Private typedef -----------------------------------------------------------*/
/**
 * \brief menu typedef
 */
typedef struct
{
    uint8_t title[MAX_NUMBER_COLUMN+1];
    uint8_t byte0[2];
    uint8_t byte15[2];
    uint8_t opt1[MAX_NUMBER_COLUMN+1];
    const uint8_t pos1;                         // 0... (MAX_NUMBER_COLUMN-1)
    uint8_t opt2[MAX_NUMBER_COLUMN+1];    // will change in an adjusting menu
    const uint8_t pos2;
    uint8_t opt3[MAX_NUMBER_COLUMN+1];
    const uint8_t pos3;
    uint8_t cursor_position;
    uint8_t max_result;
    uint8_t result;
    uint8_t confirmed;                          /*!< FALSE = not yet confirmed, TRUE = confirmed */
} menu_t;


/**
 * \brief type of menu
 * there are two types of menus
 */
typedef enum
{
    SELECTION_MENU = 0,
    ADJUSTING_MENU
} menu_type_t;


/**
 * \brief states of the statemaschine
 */
typedef enum
{
    TEAMCOLOR = 0,
    ENEMY_QUANTITY,
    FRIEND_QUANTITY,
    ENEMY_SIZE1,
    ENEMY_SIZE2,
    STARTNODE,
    SETUP_FINISHED,
    READY
}menu_current_t;

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/**
 * \brief menu-architecture
 */
/* TEAMCOLOR */
menu_t teamcolor =
{
    .title = "TEAMCOLOR",
    .byte0 = "<",
    .byte15 = ">",
    .opt1 = "YELLOW",
    .pos1 = 2,
    .opt2 = "RED",
    .pos2 = 10,
    .opt3 = "",
    .pos3 = 16, /*!< pos3 have to be >= 16 if not used! */
    .cursor_position = SETUP_TEAMCOLOR_CURSOR_DEFAULT,
    .max_result = 1,
    .result = SETUP_TEAMCOLOR_RESULT_DEFAULT, // TODO RED = 1, YELLOW = 0 GIP_TEAMCOLOR_RED
    .confirmed = FALSE
};

/* FRIEND QUANTITY */
menu_t friend_quantity =
{
    .title = "FRIEND QUANTITY",
    .byte0 = "<",
    .byte15 = ">",
    .opt1 = "0",
    .pos1 = 5,
    .opt2 = "1",
    .pos2 = 10,
    .opt3 = "",
    .pos3 =  16, /*!< pos3 have to be >= 16 if not used! */
    .cursor_position = SETUP_FRIEND_CURSOR_DEFAULT,
    .max_result = 1,
    .result = SETUP_FRIEND_RESULT_DEFAULT, // TODO 0, 1
    .confirmed = FALSE
};

/* ENEMY QUANTITY */
menu_t enemy_quantity =
{
    .title = "ENEMY QUANTITY",
    .byte0 = "<",
    .byte15 = ">",
    .opt1 = "0",
    .pos1 = 3,
    .opt2 = "1",
    .pos2 = 7,
    .opt3 = "2",
    .pos3 = 11,
    .cursor_position = SETUP_ENEMY_CURSOR_DEFAULT,
    .max_result = 2,
    .result = SETUP_ENEMY_RESULT_DEFAULT,    // 0, 1, 2
    .confirmed = FALSE
};

/* ENEMY 1 SIZE*/
menu_t enemy_size1 =
{
    .title = "ENEMY SIZE1 [cm]",
    .byte0 = "-",
    .byte15 = "+",
    .opt1 = "0...",
    .pos1 =1,
    .opt2 = SETUP_ENEMY_1_OPTION,
    .pos2 = 7,
    .opt3 = "..50",
    .pos3 = 11,
    .cursor_position = SETUP_ENEMY_1_CURSOR_DEFAULT,
    .max_result = 50,
    .result = SETUP_ENEMY_1_RESULT_DEFAULT,    // 0... 50
    .confirmed = FALSE
};

/* ENEMY 2 SIZE*/
menu_t enemy_size2 =
{
    .title = "ENEMY SIZE1 [cm]",
    .byte0 = "-",
    .byte15 = "+",
    .opt1 = "0...",
    .pos1 =1,
    .opt2 = SETUP_ENEMY_2_OPTION,
    .pos2 = 7,
    .opt3 = "..50",
    .pos3 = 11,
    .cursor_position = SETUP_ENEMY_2_CURSOR_DEFAULT,
    .max_result = 50,
    .result = SETUP_ENEMY_2_RESULT_DEFAULT,    // 0... 50
    .confirmed = FALSE
};

/* STARTNODE */
menu_t startnode =
{
    .title = "STARTNODE",
    .byte0 = "-",
    .byte15 = "+",
    .opt1 = "1...",
    .pos1 = 1,
    .opt2 = SETUP_STARTNODE_OPTION,
    .pos2 = 7,
    .opt3 = "..11",
    .pos3 = 11,
    .cursor_position = SETUP_STARTNODE_CURSOR_DEFAULT,
    .max_result = 11,  // ?
    .result = SETUP_STARTNODE_RESULT_DEFAULT,    // 0... ??
    .confirmed = FALSE
};

/* SETUP FINISHED */
menu_t setup_finished =
{
    .title = "SETUP FINISHED?",
    .byte0 = "<",
    .byte15 = ">",
    .opt1 = "NO",
    .pos1 = 4,
    .opt2 = "YES",
    .pos2 = 9,
    .opt3 = "",
    .pos3 = 16,    // pos3 have to be >= 16 if not used!
    .cursor_position = SETUP_SETUPFINISH_CURSOR_DEFAULT,
    .max_result = 1,
    .result = SETUP_SETUPFINISH_RESULT,    // 0 = NO, 1 = YES
    .confirmed = FALSE
};

/* READY */
menu_t ready =
{
    .title = "READY",
    .byte0 = " ",
    .byte15 = " ",
    .opt1 = "<ABORT>",
    .pos1 = 4,
    .opt2 = "",
    .pos2 = 16,
    .opt3 = "",
    .pos3 = 16,    // pos3 have to be >= 16 if not used!
    .cursor_position = SETUP_READY_CURSOR_DEFAULT,
    .max_result = 1,
    .result = SETUP_READY_RESULT,    // 0 = NO, 1 = YES
    .confirmed = FALSE
};

/* statemachine */
static menu_current_t current_menu;


/* Private function prototypes -----------------------------------------------*/
static void write_current_menu(menu_t **);
static void menu_handler(menu_t *, uint8_t, uint8_t, uint8_t, menu_type_t);
void setConfigRoboSetup2Default();

/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboSetupState
 * \brief   initialisation of the RoboSetupState
 *
 * \note    have to be called from the initSystem function
 */
void initRoboSetupState()
{
    /* hw modules */
    initUserPanelButtons();
    //initSensor_Key();

    /* set first state */
    current_menu = TEAMCOLOR;
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
    /* statemaschine */
    switch(current_menu)
    {
        case TEAMCOLOR:
            menu_handler(&teamcolor, 0, 1, 1, SELECTION_MENU);    // res1 = 0 (RED), res2 = 1 (YELLOW), res3 = res2 (not used), mode = selection-mode
            if (teamcolor.confirmed == TRUE)
            {
                teamcolor.confirmed = FALSE;
                current_menu = FRIEND_QUANTITY;
            }

            break;

        case FRIEND_QUANTITY:
            menu_handler(&friend_quantity, 0, 1, 1, SELECTION_MENU);    // res1 = 0, res2 = 1, res3 = res2 (not used), mode = selection-mode
            if (friend_quantity.confirmed == TRUE)
            {
                friend_quantity.confirmed = FALSE;
                current_menu = ENEMY_QUANTITY;
            }
            break;

        case ENEMY_QUANTITY:
            menu_handler(&enemy_quantity, 0, 1, 2, SELECTION_MENU);    // res1 = 0, res2 = 1, res3 = 2, mode = selection-mode
            if(enemy_quantity.confirmed == TRUE)
            {
                if(enemy_quantity.result >= 1)
                {
                    current_menu = ENEMY_SIZE1;
                }
                else
                {
                    current_menu = STARTNODE;
                }
                enemy_quantity.confirmed = FALSE;
            }
            break;

        case ENEMY_SIZE1:
            menu_handler(&enemy_size1, 0, 0, 0, ADJUSTING_MENU);    // res1-3 = 0 (not used), mode = adjusting-mode
            if (enemy_size1.confirmed == TRUE)
            {
                if(enemy_quantity.result > 1)
                {
                    current_menu = ENEMY_SIZE2;
                }
                else
                {
                    current_menu = STARTNODE;
                }
                enemy_size1.confirmed = FALSE;
            }
            break;

        case ENEMY_SIZE2:
            menu_handler(&enemy_size2, 0, 0, 0, ADJUSTING_MENU);    // res1-3 = 0 (not used), mode = adjusting-mode
            if (enemy_size2.confirmed == TRUE)
            {
                current_menu = STARTNODE;
                enemy_size2.confirmed = FALSE;
            }
            break;

        case STARTNODE:
            menu_handler(&startnode, 0, 0, 0, ADJUSTING_MENU);    // res1-3 = 0 (not used), mode = adjusting-mode
            if (startnode.confirmed == TRUE)
            {
                current_menu = SETUP_FINISHED;
                startnode.confirmed = FALSE;
            }
            break;

        case SETUP_FINISHED:
            menu_handler(&setup_finished, 0, 1, 1, SELECTION_MENU);    // res1 = 0 (NO), res2 = 1 (YES), res3 = res2 (not used), mode = selection-mode
            if(setup_finished.confirmed == TRUE)
            {
                /* if confirmed with YES... */
                if(setup_finished.result == 1)
                {
                    /* send the results and prepare for game*/
                    txStartConfigurationSet(teamcolor.result,enemy_quantity.result,
                            friend_quantity.result,enemy_size1.result,enemy_size2.result); /* CAN */
                    setConfigRoboRunState(startnode.result,teamcolor.result,enemy_quantity.result); /* run state */
                    resetGameTimer(); /* set game-timer to default */

                    /* wait for 1 second -> gyro initialisation */
                    vTaskDelayUntil(tick, SETUP_ELP_START_DELAY / portTICK_RATE_MS);    // wait 10s
                    startELP();

                    /* goto ready state */
                    current_menu = READY;
                }
                else
                {
                    current_menu = TEAMCOLOR;
                }
                setup_finished.confirmed = FALSE;
            }
            break;

        case READY:
            menu_handler(&ready, 0, 1, 1, SELECTION_MENU);

            /* game starts if key sensor is activated */
            if(getSensor_Key())
            {
                setConfigRoboSetup2Default();
                startGameTimer(); /* start game-timer */
                system_state = runRoboRunState;
            }

            /* back to setup */
            if(ready.confirmed == TRUE)
            {
                stopELP(); /* stop the ELP */
                current_menu = TEAMCOLOR;
                ready.confirmed = FALSE;
            }

            break;
    }

    /* state delay for button debouncing */
    vTaskDelayUntil(tick, SETUP_BUTTOM_DELAY / portTICK_RATE_MS);
}


/**
 * \fn      write_current_menu
 * \brief   writes the given menu to the display
 *
 * \param[in]   **current_menu   address of the address of the current_menu
 * \return  None
 */
static void write_current_menu(menu_t **current_menu)
{
    /* write title */
    LCD_set_cursor(0,0,0);
    LCD_write_string(0, 0, (*current_menu)->title, TRUE);

    /* write byte0 */
    LCD_set_cursor(MAX_NUMBER_ROW-1,0,0);
    LCD_write_string(MAX_NUMBER_ROW-1, 0, (*current_menu)->byte0, TRUE);

    /* write options */
    LCD_set_cursor(MAX_NUMBER_ROW-1,(*current_menu)->pos1,0);
    LCD_write_string(MAX_NUMBER_ROW-1, (*current_menu)->pos1, (*current_menu)->opt1, TRUE);
    LCD_set_cursor(MAX_NUMBER_ROW-1,(*current_menu)->pos2,0);
    LCD_write_string(MAX_NUMBER_ROW-1, (*current_menu)->pos2, (*current_menu)->opt2, TRUE);
    LCD_set_cursor(MAX_NUMBER_ROW-1,(*current_menu)->pos3,0);
    LCD_write_string(MAX_NUMBER_ROW-1, (*current_menu)->pos3, (*current_menu)->opt3, TRUE);

    /* write byte15 */
    LCD_set_cursor(MAX_NUMBER_ROW-1,MAX_NUMBER_COLUMN-1,0);
    LCD_write_string(MAX_NUMBER_ROW-1, MAX_NUMBER_COLUMN-1, (*current_menu)->byte15, TRUE);

    /* set cursor */
    LCD_set_cursor(MAX_NUMBER_ROW-1, (*current_menu)->cursor_position, DISPLAY_CURSOR_MODE);
}


/**
 * \fn      menu_handler
 * \brief   checks the buttons, controls the given menu and saves the result
 *
 * \param[in]   *current_menu   address of the current_menu (&menu)
 *              res1            possible result1
 *              res2            possible result2
 *              res3            possible result3
 *              mode            0 = selection-mode (< ... >)
 *                              1 = adjusting-mode (- ... +)
 * \return  None
 */
static void menu_handler(menu_t *current_menu, uint8_t res1, uint8_t res2, uint8_t res3, menu_type_t mode)
{
    /* local variables */
    static uint8_t button_mode_state = 0;
    static uint8_t button_left_state = 0;
    static uint8_t button_right_state = 0;
    static menu_t* last_menu = NULL;


    /* check if display is necessary */
    if(last_menu != current_menu)
    {
        write_current_menu(&current_menu);
        last_menu = current_menu;
    }

    /* MODE button */
    if(!getUserPanelButtonPosEdge_2(&button_mode_state))
    {
        switch(mode)
        {
            case SELECTION_MENU:

                /* check if button 1 (left) is pushed */
                if(getUserPanelButtonPosEdge_1(&button_left_state))
                {
                    /* set new cursor position and result if the cursor is not at pos1 */
                    if(current_menu->cursor_position >= current_menu->pos2)        // check pos2 & pos3
                    {
                        if(current_menu->cursor_position == current_menu->pos3)    // check pos3
                        {
                            current_menu->cursor_position = current_menu->pos2;
                            current_menu->result = res2;
                        }
                        else
                        {
                            current_menu->cursor_position = current_menu->pos1;
                            current_menu->result = res1;
                        }
                        /* update cursor */
                        LCD_set_cursor(MAX_NUMBER_ROW-1, current_menu->cursor_position, DISPLAY_CURSOR_MODE);
                    }
                }

                /* check if button 3 (right) pushed */
                if(getUserPanelButtonPosEdge_3(&button_right_state))
                {
                    /* set new cursor position and result if the cursor is not at pos3 */
                    if(current_menu->cursor_position <= current_menu->pos2)        // check pos1 & pos2
                    {
                        if(current_menu->cursor_position == current_menu->pos1)    // check pos1
                        {
                            current_menu->cursor_position = current_menu->pos2;
                            current_menu->result = res2;
                        }
                        else if(current_menu->pos3 < MAX_NUMBER_COLUMN)   // check if pos3 is used
                        {
                            current_menu->cursor_position = current_menu->pos3;
                            current_menu->result = res3;
                        }
                        /* update cursor */
                        LCD_set_cursor(MAX_NUMBER_ROW-1, current_menu->cursor_position, DISPLAY_CURSOR_MODE);
                    }
                }
                break;

                /* TODO */
            case ADJUSTING_MENU:

                /* check if button 1 is pushed and result > 0 */
                if (getUserPanelButtonPosEdge_1(&button_left_state) && current_menu->result > 0)
                {
                    current_menu->result--;    // decrease result
                    /* update display (option 2) */
                    current_menu->opt2[0] = ('0' + ((current_menu->result)/10));
                    current_menu->opt2[1] = ('0' + ((current_menu->result)%10));
                    current_menu->opt2[2] = '\0';
                    LCD_write_string(MAX_NUMBER_ROW-1, current_menu->pos2, current_menu->opt2, FALSE);
                }

                /* check if button 3 is pushed and result < max_result */
                if (getUserPanelButtonPosEdge_3(&button_right_state) && current_menu->result < current_menu->max_result)
                {
                    current_menu->result++;    // increase result

                    /* update display (option 2) */
                    current_menu->opt2[0] = ('0' + ((current_menu->result)/10));
                    current_menu->opt2[1] = ('0' + ((current_menu->result)%10));
                    current_menu->opt2[2] = '\0';
                    LCD_write_string(MAX_NUMBER_ROW-1, current_menu->pos2, current_menu->opt2, FALSE);
                }
                LCD_set_cursor(MAX_NUMBER_ROW-1, current_menu->cursor_position, DISPLAY_CURSOR_MODE);

                break;
        }
    }
    else
    {
        /* confirm chosen result to get into the next menu */
        current_menu->confirmed = TRUE;
    }
}


/**
 * \fn      setRoboSetup2Default
 * \brief   resets the whole setup to its defaults
 *
 * \param[in]   None
 * \return  None
 */
void setConfigRoboSetup2Default()
{
    teamcolor.cursor_position = SETUP_TEAMCOLOR_CURSOR_DEFAULT;
    teamcolor.result = SETUP_TEAMCOLOR_RESULT_DEFAULT;
    teamcolor.confirmed = FALSE;

    friend_quantity.cursor_position = SETUP_FRIEND_CURSOR_DEFAULT;
    friend_quantity.result = SETUP_FRIEND_RESULT_DEFAULT;
    friend_quantity.confirmed = FALSE;

    enemy_quantity.cursor_position = SETUP_ENEMY_CURSOR_DEFAULT;
    enemy_quantity.result = SETUP_ENEMY_RESULT_DEFAULT;
    enemy_quantity.confirmed = FALSE;

    enemy_size1.opt2[0] = SETUP_ENEMY_1_OPTION_CH1;
    enemy_size1.opt2[1] = SETUP_ENEMY_1_OPTION_CH2;
    enemy_size1.opt2[2] = '\0';
    enemy_size1.cursor_position = SETUP_ENEMY_1_CURSOR_DEFAULT;
    enemy_size1.result = SETUP_ENEMY_1_RESULT_DEFAULT;
    enemy_size1.confirmed = FALSE;

    enemy_size2.opt2[0] = SETUP_ENEMY_2_OPTION_CH1;
    enemy_size2.opt2[1] = SETUP_ENEMY_2_OPTION_CH2;
    enemy_size2.opt2[2] = '\0';
    enemy_size2.cursor_position = SETUP_ENEMY_2_CURSOR_DEFAULT;
    enemy_size2.result = SETUP_ENEMY_2_RESULT_DEFAULT;
    enemy_size2.confirmed = FALSE;

    startnode.opt2[0] = SETUP_STARTNODE_OPTION_CH1;
    startnode.opt2[1] = SETUP_STARTNODE_OPTION_CH2;
    startnode.opt2[2] = '\0';
    startnode.cursor_position = SETUP_STARTNODE_CURSOR_DEFAULT;
    startnode.result = SETUP_STARTNODE_RESULT_DEFAULT;
    startnode.confirmed = FALSE;

    setup_finished.cursor_position = SETUP_SETUPFINISH_CURSOR_DEFAULT;
    setup_finished.result = SETUP_SETUPFINISH_RESULT;
    setup_finished.confirmed = FALSE;

    ready.cursor_position = SETUP_SETUPFINISH_CURSOR_DEFAULT;
    ready.result = SETUP_SETUPFINISH_RESULT;
    ready.confirmed = FALSE;

    current_menu = TEAMCOLOR;
}

/**
 * @}
 */
