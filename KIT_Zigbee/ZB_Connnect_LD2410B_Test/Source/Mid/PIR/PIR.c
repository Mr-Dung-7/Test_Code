/*
 * PIR.c
 *
 *  Created on: Sep 8, 2023
 *      Author: Administrator
 */

/******************************************************************************/
/*                              INCLUDEs                                      */
/******************************************************************************/
#include "Source/Mid/PIR/PIR.h"

/******************************************************************************/
/*                               DEFINEs                              		  */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
EmberEventControl pirDetectEventControl;

pPirActionHandle_t pirHardHandle = NULL;
e_PIR_State pirState = PIR_STATE_DEBOUNCE;

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/
/*
 * @func:		PIR_Init
 *
 * @brief:		Ham khoi tao PIR
 *
 * @params:		handle
 *
 * @retVal:		None
 *
 * @note:		None
 */
void PIR_Init (pPirActionHandle_t handle)
{
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(PIR_PORT, PIR_PIN, gpioModeInput, 1);

	// Register callbacks before setting up and enabling pin interrupt
	// Dang ky callback khi co ngat xay ra tai EXTI channel (Cau hinh Output EXTI channel)
	GPIOINT_CallbackRegister(PIR_EXTI_CHANNEL, PIR_INTSignalHandle);

	pirHardHandle = handle;

	PIR_Enable(true);
}

/*
 * @func:		PIR_Enable
 *
 * @brief:		Ham kich hoat cam bien PIR
 *
 * @params:		en
 *
 * @retVal:		None
 *
 * @note:		None
 */
void PIR_Enable (boolean en)
{
	if(en)
	{
		// Cau hinh Input cho EXTI channel
		GPIO_ExtIntConfig(PIR_PORT, PIR_PIN, PIR_EXTI_CHANNEL, true, false, true);
	}
	else
	{
		GPIO_ExtIntConfig(PIR_PORT, PIR_PIN, PIR_EXTI_CHANNEL, false, false, false);
	}
}

/*
 * @func:		isMotionSignal
 *
 * @brief:		Ham lay tin hieu tu GPIO Pin khi co chuyen dong
 *
 * @params:		None
 *
 * @retVal:		true / false
 *
 * @note:		None
 */
boolean isMotionSignal (void)
{
	emberAfCorePrintln("isMotionSignal");

	uint8_t ret = (uint8_t)(GPIO_PinOutGet(PIR_PORT, PIR_PIN));
	return (ret & 0x01);
}

/*
 * @func:		PIR_INTSignalHandle
 *
 * @brief:		Ham xu ly tin hieu ngat tu chan cam bien chuyen dong
 *
 * @params:		intNo
 *
 * @retVal:		None
 *
 * @note:		None
 */
void PIR_INTSignalHandle (uint8_t intNo)
{
	emberAfCorePrintln("PIR_INTSignalHandle");

	if (intNo != (PIR_EXTI_CHANNEL))
	{
		return;
	}

	if (isMotionSignal())
	{
		pirState = PIR_STATE_DEBOUNCE;
		PIR_Enable(false);

		emberEventControlSetInactive(pirDetectEventControl);
		emberEventControlSetDelayMS(pirDetectEventControl, 200);	// Delay 200ms de tranh nhieu
	}
}

/*
 * @func:		pirDetectEventHandler
 *
 * @brief:		Ham xu ly cac trang thai cua cam bien chuyen dong
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void pirDetectEventHandler (void)
{
	emberEventControlSetInactive(pirDetectEventControl);

	switch(pirState)
	{
		case PIR_STATE_DEBOUNCE:
		{
			if (isMotionSignal())
			{
				emberAfCorePrintln("PIR_DETECT_MOTION");
				pirState = PIR_STATE_WAIT_5S;

				if(pirHardHandle != NULL)
				{
					pirHardHandle(PIR_MOTION);
				}

				emberEventControlSetDelayMS(pirDetectEventControl, 5000);
			}
			else	// Xu ly khi tin hieu bi nhieu
			{
				PIR_Enable(true);
			}
		} break;

		case PIR_STATE_WAIT_5S:
		{
			pirState = PIR_STATE_WAIT_30S;
			PIR_Enable(true);

			emberEventControlSetDelayMS(pirDetectEventControl, 30000);
		} break;

		case PIR_STATE_WAIT_30S:
		{
			if (pirHardHandle != NULL)
			{
				emberAfCorePrintln("PIR_DETECT_UNMOTION");
				pirHardHandle(PIR_UNMOTION);
			}
		} break;

		default:
			break;
	}
}


/* END FILE */
