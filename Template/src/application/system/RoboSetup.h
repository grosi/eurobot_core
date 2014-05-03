/**
 * \file    RoboSetup.h
 * \author  gross10
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   strategy setup state
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef ROBOSETUP_H_
#define ROBOSETUP_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* delays */
#define SETUP_BUTTON_DELAY      100    // sampling delay of the buttons in ms
#define SETUP_ELP_START_DELAY   2000


/* cursor */
#define DISPLAY_CURSOR_MODE 2      // 1 = cursor, 2 = blink-function, 3 = both


/* default values of the menus */
#define SETUP_TEAMCOLOR_CURSOR_DEFAULT 2
#define SETUP_TEAMCOLOR_RESULT_DEFAULT GIP_TEAMCOLOR_YELLOW // RED = 1, YELLOW = 0

#define SETUP_FRIEND_CURSOR_DEFAULT 10
#define SETUP_FRIEND_RESULT_DEFAULT 1 // 0, 1

#define SETUP_ENEMY_CURSOR_DEFAULT 7
#define SETUP_ENEMY_RESULT_DEFAULT 1 // 0, 1, 2

#define SETUP_ENEMY_1_OPTION "25"
#define SETUP_ENEMY_1_OPTION_CH1 '2' // a character is easier to read for the setup reset
#define SETUP_ENEMY_1_OPTION_CH2 '5'
#define SETUP_ENEMY_1_CURSOR_DEFAULT 7
#define SETUP_ENEMY_1_RESULT_DEFAULT 25 // 0... 50

#define SETUP_ENEMY_2_OPTION "25"
#define SETUP_ENEMY_2_OPTION_CH1 '2' // a character is easier to read for the setup reset
#define SETUP_ENEMY_2_OPTION_CH2 '5'
#define SETUP_ENEMY_2_CURSOR_DEFAULT 7
#define SETUP_ENEMY_2_RESULT_DEFAULT 25 // 0... 50

#define SETUP_STARTNODE_OPTION "01"
#define SETUP_STARTNODE_OPTION_CH1 '0'
#define SETUP_STARTNODE_OPTION_CH2 '1'
#define SETUP_STARTNODE_CURSOR_DEFAULT 7
#define SETUP_STARTNODE_RESULT_DEFAULT 1

#define SETUP_SETUPFINISH_CURSOR_DEFAULT 4
#define SETUP_SETUPFINISH_RESULT 0 // 0 = No, 1 = YES

#define SETUP_READY_CURSOR_DEFAULT 5
#define SETUP_READY_RESULT 0 // 0 = No, 1 = YES

/* info messages */
#define MESSAGE_WAIT "Please wait!"
#define MESSAGE_WAIT_COLUMN 0
#define MESSAGE_WAIT_ROW 1
#define MESSAGE_RESTART "Please restart!"
#define MESSAGE_RESTART_COLUMN 0
#define MESSAGE_RESTART_ROW 1
#define MESSAGE_CHECK "Please check!"
#define MESSAGE_CHECK_COLUMN 0
#define MESSAGE_CHECK_ROW 1
#define MESSAGE_EMERGENCY "Emergency"
#define MESSAGE_EMERGENCY_COLUMN 0
#define MESSAGE_EMERGENCY_ROW 0
#define MESSAGE_RUN "RUN"
#define MESSAGE_RUN_COLUMN 0
#define MESSAGE_RUN_ROW 0
#define MESSAGE_AWAY "GO AWAY!"
#define MESSAGE_AWAY_COLUMN 0
#define MESSAGE_AWAY_ROW 1
#define MESSAGE_INIT "Robo Init"
#define MESSAGE_INIT_COLUMN 0
#define MESSAGE_INIT_ROW 0
#define MESSAGE_GYRO "Gyro Setup"
#define MESSAGE_GYRO_COLUMN 0
#define MESSAGE_GYRO_ROW 0
#define MESSAGE_ERROR "Error"
#define MESSAGE_ERROR_COLUMN 0
#define MESSAGE_ERROR_ROW 0


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/

/* exported function prototypes -----------------------------------------------*/
extern void initRoboSetupState();
extern void setConfigRoboSetup2Default();
extern void runRoboSetupState(portTickType*);


#endif /* ROBOSETUP_H_ */
/**
 * @}
 */
