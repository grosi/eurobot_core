/**
 * \file    InterruptHandlers.c
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
 * \defgroup interrupt_hangdlers Interrupt_handlers
 * \brief   interrupt handlers
 * \ingroup firmware
 *
 * @{
 */

/* Includes ------------------------------------------------------------------*/
/* BSP */
#include "stm32f4xx.h"

/* application */
#include "Rangefinder.h"
#include "System.h"
#include "InterruptHandlers.h"

//TODO:
// #include "file_with_emergencystop_function.h"
// and remove following line:
void EmergencyStop_IT(void){};
// also see the TODO in code below.


/**
 * \fn
 * \brief  This function handles external line 0 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI0_IRQHandler(void) {

	/*
	 * Add your_function_IT() here
	 */
    EmergencyStop_Handler();

	/* Clear the EXTI line pending bit */
	EXTI_ClearITPendingBit(EXTI_Line0);
}


/**
 * \fn
 * \brief  This function handles external line 1 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI1_IRQHandler(void) {

	/*
	 * Add your_function_IT() here
	 */

	/* Clear the EXTI line pending bit */
	EXTI_ClearITPendingBit(EXTI_Line1);
}


/**
 * \fn
 * \brief  This function handles external line 2 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI2_IRQHandler(void) {

	/*
	 * Add your_function_IT() here
	 */

	/* Clear the EXTI line pending bit */
	EXTI_ClearITPendingBit(EXTI_Line2);
}


/**
 * \fn
 * \brief  This function handles external line 3 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI3_IRQHandler(void) {

	/*
	 * Add your_function_IT() here
	 */

	/* Clear the EXTI line pending bit */
	EXTI_ClearITPendingBit(EXTI_Line3);
}


/**
 * \fn
 * \brief  This function handles external line 4 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI4_IRQHandler(void) {

	/*
	 * Add your_function_IT() here
	 */

	/* Clear the EXTI line pending bit */
	EXTI_ClearITPendingBit(EXTI_Line4);
}


/**
 * \fn
 * \brief  This function handles external line 5-9 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI9_5_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) {

		/* Call rangefinder function */
		IRSensorBack_IT();

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) {

		/*
		 * Add your_function_IT() here
		 */

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) {

	    /*
         * Add your_function_IT() here
         */


		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line8) != RESET) {

		/*
		 * Add your_function_IT() here
		 */

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9) != RESET) {

		/*
		 * Add your_function_IT() here
		 */

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

/**
 * \fn
 * \brief  This function handles external line 10-15 interrupt request.
 *
 * \param  None
 * \retval None
 */
void EXTI15_10_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line10) != RESET) {

		/*
		 * Add your_function_IT() here
		 */

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) {

		/* Call rangefinder function */
		IRSensorFront_IT();

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) {

		/* Call rangefinder function */
		IRSensorRight_IT();

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) {

		/*
		 * Add your_function_IT() here
		 */

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14) != RESET) {

		/*
		 * Add your_function_IT() here
		 */

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetITStatus(EXTI_Line15) != RESET) {

		/* Call rangefinder function */
		IRSensorLeft_IT();

		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}


/**
 * @}
 */
