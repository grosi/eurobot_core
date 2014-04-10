/**
 * \file    interrupt_handlers.h
 * \author  kasen1
 * \date    2014-04-10
 *
 * \version 1.0
 *  create this file
 *
 * \brief Interrupt handlers which call custom functions for application modules.
 *
 * \note Currently there are only handlers for external interrupts implemented here.
 *       To add others, get the handler names from the file "src/startup.s".
 *
 * \todo unit test
 *
 * \defgroup interrupt_hangdlers Interrupt_hangdlers
 * \brief   interrupt handlers
 * \ingroup firmware
 *
 * @{
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERRUPT_HANDLERS_H_
#define INTERRUPT_HANDLERS_H_

/* exported functions --------------------------------------------------------*/
extern void EXTI0_IRQHandler(void);
extern void EXTI1_IRQHandler(void);
extern void EXTI2_IRQHandler(void);
extern void EXTI3_IRQHandler(void);
extern void EXTI4_IRQHandler(void);
extern void EXTI9_5_IRQHandler(void);
extern void EXTI15_10_IRQHandler(void);


#endif /* INTERRUPT_HANDLERS_H_ */

/**
 * @}
 */
