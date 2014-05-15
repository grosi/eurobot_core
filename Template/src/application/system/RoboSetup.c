/**
 * \file    RoboSetup.c
 * \author  haldj3, gross10
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
#include "lib/led.h"
#include "lib/sensor.h"


/* Private typedef -----------------------------------------------------------*/
/**
 * \brief menu typedef
 * \note for usage with an LCD
 */
#ifndef WITHOUT_USERPANEL
typedef struct
{
    char title[MAX_NUMBER_COLUMN+1];   // configuration title
    char byte0[2];                     // byte0 for "<" or "-"
    char byte15[2];                    // byte15 for ">" or "+"
    char opt1[MAX_NUMBER_COLUMN+1];    // option 1
    const uint8_t pos1;                   // position of option 1 (0... (MAX_NUMBER_COLUMN-1))
    char opt2[MAX_NUMBER_COLUMN+1];    // option 2 (could change in an adjusting menu)
    const uint8_t pos2;                   // position of option 2 (0... (MAX_NUMBER_COLUMN-1))
    char opt3[MAX_NUMBER_COLUMN+1];    // option 3
    const uint8_t pos3;                   // position of option 3 (0... (MAX_NUMBER_COLUMN-1))
    uint8_t cursor_position;              // current position of the cursor
    uint8_t min_result;                   // min. possible result
    uint8_t max_result;                   // max. possible result
    uint8_t result;                       // current result
    uint8_t confirmed;                    /*!< FALSE = not yet confirmed, TRUE = confirmed */
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

#else


/**
 * \brief menu typedef
 * \note for non LCD mode
 */
typedef struct
{
    uint8_t id_pattern;
    uint8_t result;
    uint8_t min_result;
    uint8_t max_result;
    uint8_t confirmed;

} menu_t;
#endif


/**
 * \brief states of the state machine
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
 * \brief menu-architecture display
 */
#ifndef WITHOUT_USERPANEL
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
    .min_result = 0,
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
    .min_result = 0,
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
    .min_result = 0,
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
    .opt1 = "10..",
    .pos1 =1,
    .opt2 = SETUP_ENEMY_1_OPTION,
    .pos2 = 7,
    .opt3 = "..50",
    .pos3 = 11,
    .cursor_position = SETUP_ENEMY_1_CURSOR_DEFAULT,
    .min_result = 10,
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
    .opt1 = "10..",
    .pos1 =1,
    .opt2 = SETUP_ENEMY_2_OPTION,
    .pos2 = 7,
    .opt3 = "..50",
    .pos3 = 11,
    .cursor_position = SETUP_ENEMY_2_CURSOR_DEFAULT,
    .min_result = 10,
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
    .min_result = 1,
    .max_result = 11,
    .result = SETUP_STARTNODE_RESULT_DEFAULT,    // 0... 11
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
    .pos3 = 16,    /*!< pos3 have to be >= 16 if not used! */
    .cursor_position = SETUP_SETUPFINISH_CURSOR_DEFAULT,
    .min_result = 1,
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
    .pos2 = 16,    /*!< pos2 have to be >= 16 if not used! */
    .opt3 = "",
    .pos3 = 16,    /*!< pos3 have to be >= 16 if not used! */
    .cursor_position = SETUP_READY_CURSOR_DEFAULT,
    .min_result = 0,
    .max_result = 1,
    .result = SETUP_READY_RESULT,    // 0 = NO, 1 = YES
    .confirmed = FALSE
};

#else

/* TEAMCOLOR */
menu_t teamcolor =
{
    .id_pattern= 0x08,
    .min_result = 0,
    .max_result = 1,
    .result = SETUP_TEAMCOLOR_RESULT_DEFAULT , // TODO RED = 1, YELLOW = 0 GIP_TEAMCOLOR_RED
    .confirmed = FALSE
};

/* FRIEND QUANTITY */
menu_t friend_quantity =
{
    .id_pattern= 0x10,
    .min_result = 0,
    .max_result = 1,
    .result = SETUP_FRIEND_RESULT_DEFAULT,
    .confirmed = FALSE
};

/* ENEMY QUANTITY */
menu_t enemy_quantity =
{
    .id_pattern= 0x18,
    .min_result = 0,
    .max_result = 2,
    .result = SETUP_ENEMY_RESULT_DEFAULT,
    .confirmed = FALSE
};

/* ENEMY 1 SIZE*/
menu_t enemy_size1 =
{
    .id_pattern= 0x20,
    .min_result = 1,
    .max_result = 7,
    .result = SETUP_ENEMY_1_RESULT_DEFAULT-23,
    .confirmed = FALSE
};

/* ENEMY 2 SIZE*/
menu_t enemy_size2 =
{
    .id_pattern= 0x28,
    .min_result = 1,
    .max_result = 7,
    .result = SETUP_ENEMY_2_RESULT_DEFAULT-23,
    .confirmed = FALSE
};

/* STARTNODE */
menu_t startnode =
{
    .id_pattern= 0x30,
    .min_result = 1,
    .max_result = 7,
    .result = SETUP_STARTNODE_RESULT_DEFAULT,
    .confirmed = FALSE
};

/* SETUP FINISHED */
menu_t setup_finished =
{
    .id_pattern= 0x38,
    .min_result = 0,
    .max_result = 1,
    .result = SETUP_SETUPFINISH_RESULT,
    .confirmed = FALSE
};

/* READY */
menu_t ready =
{
    .id_pattern= 0x31,
    .min_result = 0,
    .max_result = 1,
    .result = SETUP_READY_RESULT,
    .confirmed = FALSE
};
#endif


/* state machine */
static menu_current_t current_menu;
static menu_t* last_menu = NULL;


/* Private function prototypes -----------------------------------------------*/
#ifndef WITHOUT_USERPANEL
static void write_current_menu(menu_t **);
static void menu_handler(menu_t *, uint8_t, uint8_t, uint8_t, menu_type_t);
#else
static void menu_handler(menu_t *);
#endif
void setConfigRoboSetup2Default();

/* Private functions ---------------------------------------------------------*/


/**
 * \fn      initRoboSetupState
 * \brief   initialization of the RoboSetupState
 *
 * \note    have to be called from the initSystem function
 */
void initRoboSetupState()
{
    /* hw modules */
#ifndef WITHOUT_USERPANEL
    initUserPanelButtons();
#else
    initButton_S1();
    initButton_S2();
    initButton_S4();
#endif
    initLED_1();
    initLED_2();
    initLED_3();
    initLED_4();
    initLED_5();
    initLED_6();

    /* disable every LED */
    setLED_1(0);
    setLED_2(0);
    setLED_3(0);
    setLED_4(0);
    setLED_5(0);
    setLED_6(0);

    /* set first state */
    current_menu = TEAMCOLOR;
}


/**
 * \fn      runRoboSetupState
 * \brief   run the runRoboSetupState
 *
 * \param[in]   tick    pointer to systick timer for absolute delays
 * \todo    menu flow
 */
void runRoboSetupState(portTickType* tick)
{
    /* statemachine */
    switch(current_menu)
    {
        case TEAMCOLOR:
#ifndef WITHOUT_USERPANEL
            menu_handler(&teamcolor, 0, 1, 1, SELECTION_MENU);    // res1 = 0 (RED), res2 = 1 (YELLOW), res3 = res2 (not used), mode = selection-mode
#else
            menu_handler(&teamcolor);
#endif
            if (teamcolor.confirmed == TRUE)
            {
                teamcolor.confirmed = FALSE;
                current_menu = FRIEND_QUANTITY;
            }

            break;

        case FRIEND_QUANTITY:
#ifndef WITHOUT_USERPANEL
            menu_handler(&friend_quantity, 0, 1, 1, SELECTION_MENU);    // res1 = 0, res2 = 1, res3 = res2 (not used), mode = selection-mode
#else
            menu_handler(&friend_quantity);
#endif
            if (friend_quantity.confirmed == TRUE)
            {
                friend_quantity.confirmed = FALSE;
                current_menu = ENEMY_QUANTITY;
            }
            break;

        case ENEMY_QUANTITY:
#ifndef WITHOUT_USERPANEL
            menu_handler(&enemy_quantity, 0, 1, 2, SELECTION_MENU);    // res1 = 0, res2 = 1, res3 = 2, mode = selection-mode
#else
            menu_handler(&enemy_quantity);
#endif
            if(enemy_quantity.confirmed == TRUE)
            {
                if(enemy_quantity.result >= 1)
                {
                    current_menu = ENEMY_SIZE1;
                }
                else
                {
                    current_menu = STARTNODE;
                    enemy_size1.result = 0;
                    enemy_size2.result = 0;
                }
                enemy_quantity.confirmed = FALSE;
            }
            break;

        case ENEMY_SIZE1:
#ifndef WITHOUT_USERPANEL
            menu_handler(&enemy_size1, 0, 0, 0, ADJUSTING_MENU);    // res1-3 = 0 (not used), mode = adjusting-mode
#else
            menu_handler(&enemy_size1);
#endif
            if (enemy_size1.confirmed == TRUE)
            {
                if(enemy_quantity.result > 1)
                {
                    current_menu = ENEMY_SIZE2;
                }
                else
                {
                    current_menu = STARTNODE;
                    enemy_size2.result = 0;
                }
                enemy_size1.confirmed = FALSE;
            }
            break;

        case ENEMY_SIZE2:
#ifndef WITHOUT_USERPANEL
            menu_handler(&enemy_size2, 0, 0, 0, ADJUSTING_MENU);    // res1-3 = 0 (not used), mode = adjusting-mode
#else
            menu_handler(&enemy_size2);
#endif
            if (enemy_size2.confirmed == TRUE)
            {
                current_menu = STARTNODE;
                enemy_size2.confirmed = FALSE;
            }
            break;

        case STARTNODE:
#ifndef WITHOUT_USERPANEL
            menu_handler(&startnode, 0, 0, 0, ADJUSTING_MENU);    // res1-3 = 0 (not used), mode = adjusting-mode
#else
            menu_handler(&startnode);
#endif
            if (startnode.confirmed == TRUE)
            {
                current_menu = SETUP_FINISHED;
                startnode.confirmed = FALSE;
            }
            break;

        case SETUP_FINISHED:
#ifndef WITHOUT_USERPANEL
            menu_handler(&setup_finished, 0, 1, 1, SELECTION_MENU);    // res1 = 0 (NO), res2 = 1 (YES), res3 = res2 (not used), mode = selection-mode
#else
            menu_handler(&setup_finished);
#endif
            if(setup_finished.confirmed == TRUE)
            {
                /* if confirmed with YES... */
                if(setup_finished.result == 1)
                {
                    /* send the results and prepare for game*/
#ifndef WITHOUT_USERPANEL
                    txStartConfigurationSet(teamcolor.result,enemy_quantity.result,
                            friend_quantity.result,enemy_size1.result,enemy_size2.result); /* CAN */
                    setConfigRoboRunState(startnode.result,teamcolor.result,enemy_quantity.result,friend_quantity.result,
                            enemy_size1.result, enemy_size2.result); /* run state */
#else
                    if(enemy_quantity.result > 0)
                    {
                        if(enemy_quantity.result > 1)
                        {
                            txStartConfigurationSet(teamcolor.result,enemy_quantity.result,
                                    friend_quantity.result,15+5*enemy_size1.result,15+5*enemy_size2.result); /* CAN */
                            setConfigRoboRunState(startnode.result,teamcolor.result,enemy_quantity.result,friend_quantity.result,
                                    enemy_size1.result, enemy_size2.result); /* run state */
                        }
                        else
                        {
                            txStartConfigurationSet(teamcolor.result,enemy_quantity.result,
                                    friend_quantity.result,15+5*enemy_size1.result,enemy_size2.result); /* CAN */
                            setConfigRoboRunState(startnode.result,teamcolor.result,enemy_quantity.result,friend_quantity.result,
                                    enemy_size1.result, enemy_size2.result); /* run state */
                        }
                    }
                    else
                    {
                        txStartConfigurationSet(teamcolor.result,enemy_quantity.result,
                                friend_quantity.result,enemy_size1.result,enemy_size2.result); /* CAN */
                        setConfigRoboRunState(startnode.result,teamcolor.result,enemy_quantity.result,friend_quantity.result,
                                enemy_size1.result, enemy_size2.result); /* run state */
                    }
#endif
                    resetGameTimer(); /* set game-timer to default */

                    /* wait for 2 second -> gyro initialisation and show a message on screen*/
                    /* message */
#ifndef WITHOUT_USERPANEL
                    LCD_write_string(MESSAGE_GYRO_ROW,MESSAGE_GYRO_COLUMN,MESSAGE_GYRO, TRUE);
                    LCD_write_string(MESSAGE_WAIT_ROW,MESSAGE_WAIT_COLUMN,MESSAGE_WAIT, TRUE);
#endif

                    vTaskDelay(SETUP_ELP_START_DELAY / portTICK_RATE_MS);    // wait 2s

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
#ifndef WITHOUT_USERPANEL
            menu_handler(&ready, 0, 1, 1, SELECTION_MENU);
#else
            //menu_handler(&ready);
#endif
#ifndef STANDALONE
            /* game starts if key sensor is activated */
            if(getSensor_Key())
            {
#endif
                /* set current to default */
                setConfigRoboSetup2Default();

                /* message */
#ifndef WITHOUT_USERPANEL
                LCD_write_string(MESSAGE_RUN_ROW,MESSAGE_RUN_COLUMN,MESSAGE_RUN,TRUE);
                LCD_write_string(MESSAGE_AWAY_ROW,MESSAGE_AWAY_COLUMN,MESSAGE_AWAY,TRUE);
#endif

                startGameTimer(); /* start game-timer */
                system_state = runRoboRunState;
#ifndef STANDALONE
            }
#endif

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
    vTaskDelayUntil(tick, SETUP_BUTTON_DELAY / portTICK_RATE_MS);
}


/**
 * \fn      write_current_menu
 * \brief   writes the given menu to the display
 *
 * \param[in]   **current_menu   address of the address of the current_menu
 * \return  None
 */
#ifndef WITHOUT_USERPANEL
static void write_current_menu(menu_t **current_menu)
{
    /* write title */
    LCD_write_string(0, 0, (*current_menu)->title, TRUE);

    /* write byte0 */
    LCD_write_string(MAX_NUMBER_ROW-1, 0, (*current_menu)->byte0, TRUE);

    /* write options */
    LCD_write_string(MAX_NUMBER_ROW-1, (*current_menu)->pos1, (*current_menu)->opt1, TRUE);
    LCD_write_string(MAX_NUMBER_ROW-1, (*current_menu)->pos2, (*current_menu)->opt2, TRUE);
    LCD_write_string(MAX_NUMBER_ROW-1, (*current_menu)->pos3, (*current_menu)->opt3, TRUE);

    /* write byte15 */
    LCD_write_string(MAX_NUMBER_ROW-1, MAX_NUMBER_COLUMN-1, (*current_menu)->byte15, TRUE);

    /* set cursor */
    LCD_set_cursor(MAX_NUMBER_ROW-1, (*current_menu)->cursor_position, DISPLAY_CURSOR_MODE);
}
#endif


/**
 * \fn      menu_handler
 * \brief   checks the buttons, controls the given menu and saves the result
 * \note    for usage with LCD
 *
 * \param[in]   *current_menu   address of the current_menu (&menu)
 *              res1            possible result1
 *              res2            possible result2
 *              res3            possible result3
 *              mode            0 = selection-mode (< ... >)
 *                              1 = adjusting-mode (- ... +)
 * \return  None
 */
#ifndef WITHOUT_USERPANEL
static void menu_handler(menu_t *current_menu, uint8_t res1, uint8_t res2, uint8_t res3, menu_type_t mode)
{
    /* local variables */
    static uint8_t button_mode_state = 0;
    static uint8_t button_left_state = 0;
    static uint8_t button_right_state = 0;


    /* check if a change of the display is necessary */
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
                        if((current_menu->cursor_position == current_menu->pos1) && (current_menu->pos2 < MAX_NUMBER_COLUMN))    // check pos1 and if pos2 is used
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

            case ADJUSTING_MENU:

                /* check if button 1 (left) is pushed and result > 0 */
                if (getUserPanelButtonPosEdge_1(&button_left_state) && (current_menu->result > current_menu->min_result))
                {
                    current_menu->result--;    // decrease result
                    /* update display (option 2) */
                    current_menu->opt2[0] = ('0' + ((current_menu->result)/10));
                    current_menu->opt2[1] = ('0' + ((current_menu->result)%10));
                    current_menu->opt2[2] = '\0';
                    LCD_write_string(MAX_NUMBER_ROW-1, current_menu->pos2, current_menu->opt2, FALSE);
                }

                /* check if button 3 (right) is pushed and result < max_result */
                if (getUserPanelButtonPosEdge_3(&button_right_state) && (current_menu->result < current_menu->max_result))
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
    else    // button 2 (mid) pushed:
    {
        /* confirm chosen result to get into the next menu */
        current_menu->confirmed = TRUE;
    }
}
#else
static void menu_handler(menu_t* current_menu)
{
    /* local variables */
    static uint8_t button_mode_state = 0;
    static uint8_t button_left_state = 0;
    static uint8_t button_right_state = 0;


    /* check if a change of the display is necessary */
    if(last_menu != current_menu)
    {
        /* parameters */
        setLED_1(current_menu->result & 0x01);
        setLED_2(current_menu->result & 0x02);
        setLED_3(current_menu->result & 0x04);
        /* menu ID */
        setLED_4(current_menu->id_pattern & 0x08);
        setLED_5(current_menu->id_pattern & 0x10);
        setLED_6(current_menu->id_pattern & 0x20);
        last_menu = current_menu;
    }

    /* MODE button */
    if(!getButtonPosEdge_S4(&button_mode_state))
    {
        /* check if button 1 (right) is pushed */
        if(getButtonPosEdge_S1(&button_right_state) && current_menu->result < current_menu->max_result)
        {
            current_menu->result++;

            /* parameters */
            setLED_1(current_menu->result & 0x01);
            setLED_2(current_menu->result & 0x02);
            setLED_3(current_menu->result & 0x04);
        }

        /* check if button 3 (left) pushed */
        if(getButtonPosEdge_S2(&button_left_state) && current_menu->result > current_menu->min_result)
        {
            current_menu->result--;

            /* parameters */
            setLED_1(current_menu->result & 0x01);
            setLED_2(current_menu->result & 0x02);
            setLED_3(current_menu->result & 0x04);
        }
    }
    else    // button 2 (mid) pushed:
    {
        /* confirm chosen result to get into the next menu */
        current_menu->confirmed = TRUE;
    }
}
#endif


/**
 * \fn      setRoboSetup2Default
 * \brief   resets the whole setup to its defaults
 *
 * \param[in]   None
 * \return  None
 */
void setConfigRoboSetup2Default()
{
#ifndef WITHOUT_USERPANEL
    teamcolor.cursor_position = SETUP_TEAMCOLOR_CURSOR_DEFAULT;
#endif
    teamcolor.result = SETUP_TEAMCOLOR_RESULT_DEFAULT;
    teamcolor.confirmed = FALSE;

#ifndef WITHOUT_USERPANEL
    friend_quantity.cursor_position = SETUP_FRIEND_CURSOR_DEFAULT;
#endif
    friend_quantity.result = SETUP_FRIEND_RESULT_DEFAULT;
    friend_quantity.confirmed = FALSE;

#ifndef WITHOUT_USERPANEL
    enemy_quantity.cursor_position = SETUP_ENEMY_CURSOR_DEFAULT;
#endif
    enemy_quantity.result = SETUP_ENEMY_RESULT_DEFAULT;
    enemy_quantity.confirmed = FALSE;

#ifndef WITHOUT_USERPANEL
    enemy_size1.opt2[0] = SETUP_ENEMY_1_OPTION_CH1;
    enemy_size1.opt2[1] = SETUP_ENEMY_1_OPTION_CH2;
    enemy_size1.opt2[2] = '\0';
    enemy_size1.cursor_position = SETUP_ENEMY_1_CURSOR_DEFAULT;
#endif
    enemy_size1.result = SETUP_ENEMY_1_RESULT_DEFAULT;
    enemy_size1.confirmed = FALSE;

#ifndef WITHOUT_USERPANEL
    enemy_size2.opt2[0] = SETUP_ENEMY_2_OPTION_CH1;
    enemy_size2.opt2[1] = SETUP_ENEMY_2_OPTION_CH2;
    enemy_size2.opt2[2] = '\0';
    enemy_size2.cursor_position = SETUP_ENEMY_2_CURSOR_DEFAULT;
#endif
    enemy_size2.result = SETUP_ENEMY_2_RESULT_DEFAULT;
    enemy_size2.confirmed = FALSE;

#ifndef WITHOUT_USERPANEL
    startnode.opt2[0] = SETUP_STARTNODE_OPTION_CH1;
    startnode.opt2[1] = SETUP_STARTNODE_OPTION_CH2;
    startnode.opt2[2] = '\0';
    startnode.cursor_position = SETUP_STARTNODE_CURSOR_DEFAULT;
#endif
    startnode.result = SETUP_STARTNODE_RESULT_DEFAULT;
    startnode.confirmed = FALSE;

#ifndef WITHOUT_USERPANEL
    setup_finished.cursor_position = SETUP_SETUPFINISH_CURSOR_DEFAULT;
#endif
    setup_finished.result = SETUP_SETUPFINISH_RESULT;
    setup_finished.confirmed = FALSE;

#ifndef WITHOUT_USERPANEL
    ready.cursor_position = SETUP_SETUPFINISH_CURSOR_DEFAULT;
#endif
    ready.result = SETUP_SETUPFINISH_RESULT;
    ready.confirmed = FALSE;

#ifdef WITHOUT_USERPANEL
    setLED_1(0);
    setLED_2(0);
    setLED_3(0);
    setLED_4(0);
    setLED_5(0);
    setLED_6(0);
#endif



    current_menu = TEAMCOLOR;
    last_menu = NULL;
}

/**
 * @}
 */
