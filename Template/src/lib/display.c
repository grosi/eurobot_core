/**
 * \file    display.c
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
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "display.h"
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    DISPLAY_FINISH = 0,
    DISPLAY_INSTRUCTION,
    DISPLAY_DATA
}display_com_t;

typedef enum
{
    SPI_CS_HIGH = 0,
    SPI_CS_LOW,
    SPI_SET_DATA,
    SPI_WRITE_DATA
}spi_state_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
void (*Delay)(long);
static volatile display_com_t display_command = DISPLAY_FINISH;
static volatile uint8_t display_data;

/* Private function prototypes -----------------------------------------------*/
void LCD_write_byte_instruction(uint8_t);


/**=============================================================================
 * \fn      LCD_init
 * \brief   initializes the display (within ~253ms)
 *
 * \param[in]   function-pointer to the RTOS-delay-function
 * \return  None
 =============================================================================*/
void LCD_init(void (*delay)(long))
{
    /* local variable */
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef timer_init;
    NVIC_InitTypeDef interrupt_init;

	Delay = delay;    // saves function pointer in a private variable

	/* enable clock (42MHz) */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* initialize the RS pin */
	GPIO_InitStruct.GPIO_Pin = DISPLAY_PIN_RS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(DISPLAY_PORT, &GPIO_InitStruct);

	/* SCK pin */
	GPIO_InitStruct.GPIO_Pin = DISPLAY_PIN_SCK;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DISPLAY_PORT, &GPIO_InitStruct);

	/* MOSI pin */
	GPIO_InitStruct.GPIO_Pin = DISPLAY_PIN_MOSI;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DISPLAY_PORT, &GPIO_InitStruct);

	/* CS pin */
	GPIO_InitStruct.GPIO_Pin = DISPLAY_PIN_CS;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(DISPLAY_PORT_CS, &GPIO_InitStruct);

    GPIO_WriteBit(DISPLAY_PORT_CS, DISPLAY_PIN_CS, SET); /* set display inactive */
    GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_SCK, SET); /* set SCK */
    GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_RS, RESET); /* set RS low */

    /* TIM2 init */
    TIM_DeInit(TIM2);
    timer_init.TIM_CounterMode = TIM_CounterMode_Up;
    timer_init.TIM_Prescaler = 420 - 1; /* 42MHz / 420 = 100kHz */
    timer_init.TIM_ClockDivision = TIM_CKD_DIV1; /* 100KHz / 1 = 100KHz */
    timer_init.TIM_Period = 1; /* every 10us a interrupt will fired */
    TIM_TimeBaseInit(TIM2, &timer_init);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* init interrupt-controller */
    interrupt_init.NVIC_IRQChannel = TIM2_IRQn;
    interrupt_init.NVIC_IRQChannelPreemptionPriority = 0;
    interrupt_init.NVIC_IRQChannelSubPriority = 0;
    interrupt_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&interrupt_init);

	/* wait until power supply is stable (50ms) */
	Delay(50);    // works only when the scheduler's active

	/* basic initialization: serial interface */
	LCD_write_byte_instruction(0x39);    // Function Set (Instruction table 1) 8bit-Mode + 2 line
	LCD_write_byte_instruction(0x14);    // Bias Set (1/4 = 0x1C, 1/5 = 0x14)
	LCD_write_byte_instruction(0x55);    // Booster on & Contrast set adjustment (C5, C4)
	LCD_write_byte_instruction(0x6D);    // Put voltage follower and gain (3.3V)
	Delay(200);                          // wait (200ms) for power stable
	LCD_write_byte_instruction(0x78);    // Contrast set (C3, C2, C1, C0)
	LCD_write_byte_instruction(0x38);    // Function Set (return to the Instruction table 0)
	LCD_write_byte_instruction(0x0D);    // Display on, Cursor off, Blink-function on

	LCD_clear();                         // Reset display (0x01)
	LCD_write_byte_instruction(0x06);    // Entry Mode Set
}


/**
 * \fn      TIM2_IRQHandler
 * \brief   interrupt handler of timer 2
 */
void TIM2_IRQHandler(void)
{
    /* local variables */
    static uint8_t bit = 0;
    static spi_state_t spi_state = SPI_CS_LOW;

    /* have to be the first function in an interrupt-handler */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /* SPI statemachine */
    switch(spi_state)
    {
        /* CS low */
        case SPI_CS_LOW:
            GPIO_WriteBit(DISPLAY_PORT_CS, DISPLAY_PIN_CS, RESET); /* reset CS */
            spi_state = SPI_SET_DATA;

            break;

        /* set data */
        case SPI_SET_DATA:
            GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_SCK, RESET); /* reset SCK */

            /* set data bit */
            if((display_data << bit) & 0x80)
                {GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_MOSI, SET);} /* set MOSI */
            else {GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_MOSI, RESET);} /* reset MOSI */

            /* set RS pin if the command is type of SPI data */
            if(display_command == DISPLAY_DATA && bit == 7)
                {GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_RS, SET);} /* set RS */

            spi_state = SPI_WRITE_DATA;
            bit++;

            break;

        /* write data */
        case SPI_WRITE_DATA:
            GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_SCK, SET); /* set SCK */


            /* goto CS high (last SPI state) */
            if(bit == 8)
            {
                bit = 0;
                spi_state = SPI_CS_HIGH;
            }
            /* set next data bit */
            else
            {
                spi_state = SPI_SET_DATA;
            }

            break;

        /* CS high + communication finished */
        case SPI_CS_HIGH:
            GPIO_WriteBit(DISPLAY_PORT_CS, DISPLAY_PIN_CS, SET); /* set CS */
            GPIO_WriteBit(DISPLAY_PORT, DISPLAY_PIN_RS, RESET); /* reset RS */
            display_command = DISPLAY_FINISH;
            spi_state = SPI_CS_LOW;
            TIM_Cmd(TIM2, DISABLE); /* stop SW SPI */

            break;
    }
}


/**=============================================================================
 * \fn      LCD_write_byte_instruction
 * \brief   writes an instruction-byte to the display
 *
 * \param[in]   instruction    instruction-byte
 * \return  None
 =============================================================================*/
void LCD_write_byte_instruction(uint8_t instruction)
{
    /* wait until the previous data is sent */
    while(display_command != DISPLAY_FINISH);

    /* send next data block */
    display_data = instruction;
    display_command = DISPLAY_INSTRUCTION;
    TIM_Cmd(TIM2, ENABLE); /* start SW SPI */

    Delay(1); /* minimum waiting time until the next byte can be sent (= 30us)*/
}


/**=============================================================================
 * \fn      LCD_clear
 * \brief   clears/resets the display
 *
 * \param[in]   None
 * \return  None
 =============================================================================*/
void LCD_clear(void)
{
	LCD_write_byte_instruction(0x01);    // clear display
	Delay(2);                            // wait (2ms)
}


/**=============================================================================
 * \fn      LCD_write_byte_data
 * \brief   writes an ASCII-character (data-byte) on the display at a specific position
 *
 * \param[in]   lcd_row       Row in which the character shall appear (0...MAX_NUMBER_ROW-1)
 *              lcd_column    Column in which the character shall appear (0...MAX_NUMBER_ROW-1)
 *              data          ASCII-character
 * \return  None
 =============================================================================*/
void LCD_write_byte_data(uint8_t data)
{
    /* wait until the previous data is sent */
    while(display_command != DISPLAY_FINISH);

    /* send next data block */
    display_data = data;
    display_command = DISPLAY_DATA;
    TIM_Cmd(TIM2, ENABLE); /* start SW SPI */

    Delay(1); /* minimum waiting time until the next byte can be sent (= 30us)*/
}


/**=============================================================================
 * \fn      LCD_write_string
 * \brief   writes a string on the display at a specific position
 *
 * \param[in]   lcd_row       Row in which the string shall appear (0...MAX_NUMBER_ROW-1)
 *              lcd_column    Column in which the string shall appear (0...MAX_NUMBER_ROW-1)
 *              *string       ASCII-string
 *              clr_line      if TRUE (=1), the rest of the line will be cleared
 * \return  None
 =============================================================================*/
void LCD_write_string(uint8_t lcd_row, uint8_t lcd_column, uint8_t* string, uint8_t clr_line)
{
	uint8_t string_length;
	uint8_t c = 1;    // initialize c unequal 0

	/* count the bytes (characters) */
	for(string_length = 0; c != '\0'; string_length++)
	{
		c = string[string_length];
	}

	string_length--;    // delete the count for the '\0'

	/* check if string will be within the LCD */
    if((lcd_column + string_length) > (MAX_NUMBER_COLUMN))
	{
	    lcd_column = 0;
        string[0] = 'x';    // write "x" otherwise (overflow)
        string[1] = '\0';

        string_length = 1;
        clr_line = 1;       // enable clear line
	}

    c = 0;    // will be used as a (char-)counter from now on

    /* write every character individually */
    for(; string_length > 0; string_length--)
    {
    	LCD_write_byte_data(string[c]);
    	c++;
    }

    /* clear rest of the line if clr_line is set */
    if(clr_line > 0)
    {
    	for(; (MAX_NUMBER_COLUMN - (lcd_column + c)) > 0; c++)
    	{
    		LCD_write_byte_data(' ');
    	}
    }
}


/**=============================================================================
 * \fn      LCD_set_cursor
 * \brief   sets the cursor and/or blink-function on the display at a specific position
 *
 * \param[in]   lcd_row       Row in which the cursor shall appear (0...MAX_NUMBER_ROW-1)
 *              lcd_column    Column in which the cursor shall appear (0...MAX_NUMBER_ROW-1)
 *              mode          0 = cursor and blink-function off
 *                            1 = cursor on
 *                            2 = blink-function on
 *                            3 = cursor and blink-function on
 * \return  None
 =============================================================================*/
void LCD_set_cursor(uint8_t lcd_row, uint8_t lcd_column, uint8_t mode)
{
	uint8_t lcd_offset;

	/* check if the cursor will be within the LCD, set max. otherwise */
    if(lcd_row > (MAX_NUMBER_ROW - 1)) lcd_row = (MAX_NUMBER_ROW - 1);
    if(lcd_column > (MAX_NUMBER_COLUMN - 1)) lcd_column = (MAX_NUMBER_COLUMN - 1);

    /* saves the offset of the defined row in a variable (Set RAM Adr: 1xxx'xxxx) */
    switch(lcd_row)
    {
        case 0:    lcd_offset = 0x80;    // Row 1 (Adr 0 = (1)000'0000 = 80h)
    	    break;
        case 1:    lcd_offset = 0xC0;    // Row 2 (Adr 64 = (1)100'0000 = C0h)
            break;
        default:   lcd_offset = 0x80;    // Row 1
    };

    /* set RAM (cursor) position */
    LCD_write_byte_instruction(lcd_column + lcd_offset);

    switch(mode)
    {
        case 1:    LCD_write_byte_instruction(0x0E);    // turn cursor on
    	    break;
        case 2:    LCD_write_byte_instruction(0x0D);    // turn blink-function on
            break;
        case 3:    LCD_write_byte_instruction(0x0F);    // turn cursor and blink-function on
            break;
        default:   LCD_write_byte_instruction(0x0C);    // turn cursor and blink-function off otherwise
    };
}


/**=============================================================================
 * \fn      LCD_set_contrast
 * \brief   sets the contrast of the display
 *
 * \param[in]   intensity    0... 63 (24 = default)
 * \return  None
 =============================================================================*/
void LCD_set_contrast(uint8_t intensity)
{
	/* check if the intensity is an allowed value */
	if(intensity < 64)
	{
		LCD_write_byte_instruction(0x39);    // Function Set (Instruction table 1) 8bit-Mode + 2 line

		/* set contrast */
		LCD_write_byte_instruction(0x5C|(intensity >> 4));    // set bit: C5 & C4
		LCD_write_byte_instruction(0x70|(intensity & 0x0F));  // set bit: C3, C2, C1 & C0

		LCD_write_byte_instruction(0x38);    // Function Set (return to the Instruction table 0)
	}
}


/*****************************Other LCD-functions:*****************************/
void LCD_on(void)
{
	LCD_write_byte_instruction(0x0C);
}

void LCD_off(void)
{
	LCD_write_byte_instruction(0x08);
}

void LCD_shift_display_right(void)
{
	LCD_write_byte_instruction(0x1C);
}

void LCD_shift_display_left(void)
{
	LCD_write_byte_instruction(0x18);
}

void LCD_shift_cursor_right(void)
{
	LCD_write_byte_instruction(0x14);
}

void LCD_shift_cursor_left(void)
{
	LCD_write_byte_instruction(0x10);
}

void LCD_shift_return(void)
{
	LCD_write_byte_instruction(0x02);    // returns display to its original status, if shifted
}

void LCD_line_mode_one(void)
{
	LCD_write_byte_instruction(0x35);    // set 1-line display mode and 5x16 font
}

void LCD_line_mode_two(void)
{
	LCD_write_byte_instruction(0x39);    // set 2-line display mode and 5x8 font (=std)
}

/**
 * @}
 */
