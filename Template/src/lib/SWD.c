/**
 * \file    SWD.c
 * \author  meert1
 * \date    2012-12-10
 *
 * \version 1.0
 *  create this file
 *
 * \brief   Functions to send MSG's to the Serial Wire Viewer (SWD)
 * \note    source from http://virtual-shed.blogspot.ch/
 *
 * \defgroup swd SWD
 * \brief   SWD library
 * \ingroup firmware
 *
 * @{
 */
/* Includes ------------------------------------------------------------------*/
//#include <memPoolService.h>         /* Memory pool manager service */
#include <stm32f4xx.h>


/**
 * \brief   This function sends strings to the serial wire viewer.
 * \param  *s: pointer to string to be displayed on SWV
 * \retval None
 */
void SWV_puts(const char *s )
{
    while (*s) ITM_SendChar(*s++);

}

/**
 * \brief   This function sends numbers to the serial wire viewer.
 * \param  number: number to be displayed on SWV
 * \retval None
 */
void SWV_printnum(long number)
{
    unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars.
    unsigned int i = 0;

    //if number is 0
    if (number == 0)
    {
        ITM_SendChar('0'); //if number is zero
        return;
    }
    //account for negative numbers
    if (number < 0)
    {
        ITM_SendChar('-');
        number = number * -1;
    }
    while(number > 0)
    {
        buf[i++] = number % 10; //display in base 10
        number = number / 10;
        //NOTE: the effect of i++ means that the i variable will be at number of digits + 1
    }
    for(; i > 0; i--)
    {
        ITM_SendChar((char)('0' + buf[i-1]));
    }
}

/**
 * \brief  This function sends numbers to the serial wire viewer.
 * \param  number: number to be displayed on SWV
 * \param  digits: number of digits after decimal point
 * \retval None
 */
void SWV_printfloat(double number, int digits)
{
    int i = 0;
    //handle negative numbers
    if(number < 0.0)
    {
        ITM_SendChar('-');
        number = -number;
    }
    //round correctly so that uart_printfloat(1.999, 2) shows as "2.00"
    double rounding = 0.5;
    for(i = 0; i < digits; ++i) rounding = rounding / 10.0;
    number = number + rounding;

    //extract the integer part of the number and print it
    unsigned long int_part = (unsigned long) number;
    double remainder = (double)(number - (double)int_part);
    SWV_printnum(int_part); //print the integer part
    if(digits > 0) ITM_SendChar('.'); //print decimal pint
    int toprint;
    while(digits-- > 0)
    {
        remainder = remainder * 10.0;
        toprint = (int)remainder;
        SWV_printnum(toprint);
        remainder = remainder - toprint;
    }
}

/**
 * @}
 */
