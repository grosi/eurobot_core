/**
 * \file    display.h
 * \author  Haldemann Jascha
 * \author  haldj3
 * \date    2014-03-21
 *
 * \version 1.0
 *  create this file
 *
 * \brief   this file contains function for the display handling.
 *
 * \defgroup display DISPLAY
 * \brief   DISPLAY library
 * \ingroup firmware
 *
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_H
#define __DISPLAY_H

/* exported typedef ----------------------------------------------------------*/

/* exported define -----------------------------------------------------------*/
#define DISPLAY_PIN_RS      GPIO_Pin_6
#define DISPLAY_PORT_RS     GPIOA        // enable related clock in display.c

#define MAX_NUMBER_ROW      2            // the display has 2 rows
#define MAX_NUMBER_COLUMN   16           // and 16 columns

/* exported types ------------------------------------------------------------*/
/* exported constants --------------------------------------------------------*/
/* exported macro ------------------------------------------------------------*/

/* exported functions --------------------------------------------------------*/
extern void init_display();
extern void LCD_clear();
extern void LCD_write_byte_data(uint8_t, uint8_t, uint8_t);
extern void LCD_write_string(uint8_t, uint8_t, uint8_t*, uint8_t);
extern void LCD_set_highlighter(uint8_t, uint8_t, uint8_t);

extern void LCD_on();
extern void LCD_off();
extern void LCD_shift_display_right();
extern void LCD_shift_display_left();
extern void LCD_shift_cursor_right();
extern void LCD_shift_cursor_left();
extern void LCD_shift_return();
extern void LCD_set_contrast(uint8_t);
extern void LCD_line_mode_one();
extern void LCD_line_mode_two();

#endif

/**
 * @}
 */
