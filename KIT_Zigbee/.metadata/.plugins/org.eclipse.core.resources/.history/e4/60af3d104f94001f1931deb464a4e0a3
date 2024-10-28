/*
 * ld2410.c
 *
 *  Created on: Sep 8, 2023
 *      Author: Administrator
 */

/******************************************************************************/
/*                              INCLUDEs                                      */
/******************************************************************************/
#include "Source/Mid/LD2410/ld2410.h"

/******************************************************************************/
/*                               DEFINEs                              		  */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
EmberEventControl Ld2410DetectEventControl;

pLd2410ActionHandle ld2410HardHandle = NULL;
LD2410_State_e ld2410State = LD2410_STATE_DEBOUNCE;

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/
/*
 * @func:		LD2410_Init
 *
 * @brief:		Ham khoi tao LD2410
 *
 * @params:		handle
 *
 * @retVal:		None
 *
 * @note:		None
 */
void LD2410_Init (pLd2410ActionHandle handle)
{
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(LD2410_PORT, LD2410_PIN, gpioModeInput, 1);

	// Register callbacks before setting up and enabling pin interrupt
	GPIOINT_CallbackRegister(LD2410_EXTI_CHANNEL, LD2410_INTSignalHandle);

	ld2410HardHandle = handle;

	LD2410_Enable(true);
}

/*
 * @func:		LD2410_Enable
 *
 * @brief:		Ham kich hoat cam bien LD2410
 *
 * @params:		en
 *
 * @retVal:		None
 *
 * @note:		None
 */
void LD2410_Enable (boolean en)
{
	if(en)
	{
		GPIO_ExtIntConfig(LD2410_PORT, LD2410_PIN, LD2410_EXTI_CHANNEL, true, false, true);
	}
	else
	{
		GPIO_ExtIntConfig(LD2410_PORT, LD2410_PIN, LD2410_EXTI_CHANNEL, false, false, false);
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
	uint8_t ret = (uint8_t)(GPIO_PinOutGet(LD2410_PORT, LD2410_PIN));
	return (ret & 0x01);
}

/*
 * @func:		LD2410_INTSignalHandle
 *
 * @brief:		Ham xu ly tin hieu ngat tu chan cam bien chuyen dong
 *
 * @params:		intNo
 *
 * @retVal:		None
 *
 * @note:		None
 */
void LD2410_INTSignalHandle (uint8_t intNo)
{
//	emberAfCorePrintln("LD2410_INTSignalHandle");

	if (intNo != (LD2410_EXTI_CHANNEL))
	{
		return;
	}

	if (isMotionSignal())
	{
		ld2410State = LD2410_STATE_DEBOUNCE;
		LD2410_Enable(false);

		emberEventControlSetInactive(Ld2410DetectEventControl);
		emberEventControlSetDelayMS(Ld2410DetectEventControl, 200);	// Delay 200ms de tranh nhieu
	}
}

/*
 * @func:		Ld2410DetectEventHandler
 *
 * @brief:		Ham xu ly cac trang thai cua cam bien chuyen dong
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void Ld2410DetectEventHandler (void)
{
	emberEventControlSetInactive(Ld2410DetectEventControl);

	switch(ld2410State)
	{
		case LD2410_STATE_DEBOUNCE:
		{
			if (isMotionSignal())
			{
				emberAfCorePrintln("LD2410_DETECT_MOTION");
				ld2410State = LD2410_STATE_WAIT_5S;

				if(ld2410HardHandle != NULL)
				{
					ld2410HardHandle(LD2410_MOTION);
				}

				emberEventControlSetDelayMS(Ld2410DetectEventControl, 5000);
			}
			else
			{
				LD2410_Enable(true);
			}
		} break;

		case LD2410_STATE_WAIT_5S:
		{
			ld2410State = LD2410_STATE_WAIT_30S;
			LD2410_Enable(true);

			emberEventControlSetDelayMS(Ld2410DetectEventControl, 30000);
		} break;

		case LD2410_STATE_WAIT_30S:
		{
			if (ld2410HardHandle != NULL)
			{
				emberAfCorePrintln("LD2410_DETECT_UNMOTION");
				ld2410HardHandle(LD2410_UNMOTION);
			}
		} break;

		default:
			break;
	}
}


/* END FILE */
