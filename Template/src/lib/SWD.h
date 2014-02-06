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

#ifndef SWD_H_
#define SWD_H_

/* exported function prototypes -----------------------------------------------*/
void SWV_puts(const char *s );
void SWV_printnum(long number);
void SWV_printfloat(double number, int digits);

#endif /* SWD_H_ */

/**
 * @}
 */
