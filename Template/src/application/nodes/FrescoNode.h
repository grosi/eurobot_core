/**
 * \file    FrescoNode.h
 * \author  kasen1, gross10
 * \date    2014-03-17
 *
 * \version 1.0
 *  create this file
 *
 * \brief   fresco node statemaschine
 *
 * \ingroup strategy
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef FRESCONODE_H_
#define FRESCONODE_H_

/* exported typedef -----------------------------------------------------------*/


/* exported define ------------------------------------------------------------*/
/* Node */
#define FRESCO_MAX_RETRIES   (3)     /*!< Nummber of retries after the first try */ //TODO


/* exported macro -------------------------------------------------------------*/


/* exported variables ---------------------------------------------------------*/


/* exported function prototypes -----------------------------------------------*/
extern void doFrescoNode(node_param_t*);


#endif /* FRESCONODE_H_ */

/**
 * @}
 */
