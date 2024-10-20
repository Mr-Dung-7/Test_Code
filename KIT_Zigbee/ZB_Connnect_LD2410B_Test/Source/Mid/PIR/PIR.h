/*
 * PIR.h
 *
 *  Created on: Sep 8, 2023
 *      Author: Administrator
 */

#ifndef SOURCE_MID_PIR_PIR_H_
#define SOURCE_MID_PIR_PIR_H_

/******************************************************************************/
/*                              INCLUDEs                                      */
/******************************************************************************/
#include "app/framework/include/af.h"
#include "Source/App/Send/send-user.h"
#include "Source/Mid/Led/led-user.h"

/******************************************************************************/
/*                               DEFINEs                              		  */
/******************************************************************************/
//#define PIR_PORT				gpioPortC
//#define PIR_PIN					(4U)
#define PIR_PORT				gpioPortC
#define PIR_PIN					(2U)
#define PIR_EXTI_CHANNEL		(PIR_PIN)

/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/
typedef enum
{
	PIR_STATE_DEBOUNCE,
	PIR_STATE_WAIT_5S,
	PIR_STATE_WAIT_30S
} e_PIR_State;

typedef enum
{
	PIR_UNMOTION,
	PIR_MOTION
} e_PIR_Action;

/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
typedef void (*pPirActionHandle_t)(uint8_t);

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/
void 		PIR_Init (pPirActionHandle_t handle);
void 		PIR_Enable (boolean en);
boolean 	isMotionSignal (void);
void		PIR_INTSignalHandle (uint8_t intNo);

/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/


#endif /* SOURCE_MID_PIR_PIR_H_ */

/* END FILE */
