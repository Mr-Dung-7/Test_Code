/*
 * main.c
 *
 *  Created on: Sep 25, 2024
 *      Author: Mr.hDung
 */

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "Source/App/main.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
st_USART_FRAME_TX 	TxFrame;

uint8_t data[] = {0,1};
char text[] = "IOT";

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/
/** @brief Main Init
 *
 * This function is called from the application's main function. It gives the
 * application a chance to do any initialization required at system startup.
 * Any code that you would normally put into the top of the application's
 * main() routine should be put into this function.
        Note: No callback
 * in the Application Framework is associated with resource cleanup. If you
 * are implementing your application on a Unix host where resource cleanup is
 * a consideration, we expect that you will use the standard Posix system
 * calls, including the use of atexit() and handlers for signals such as
 * SIGTERM, SIGINT, SIGCHLD, SIGPIPE and so on. If you use the signal()
 * function to register your signal handler, please mind the returned value
 * which may be an Application Framework function. If the return value is
 * non-null, please make sure that you call the returned function from your
 * handler to avoid negating the resource cleanup of the Application Framework
 * itself.
 *
 */
void emberAfMainInitCallback (void)
{
	emberAfCorePrintln("emberAfMainInitCallback");

	USART2_Init(USER_Usart2RxHandle);

	Button_Init(USER_ButtonPressHandle, USER_ButtonHoldHandle);

	led_Init();

	Timer_Init(10);
}

/*
 * @func:  		USER_Usart2RxHandle
 *
 * @brief:		The function executes the event upon receiving the corresponding message
 * 				in the specified format
 *
 * @param:		UartStateRx - Received status
 *
 * @retval:		None
 *
 * @note:		None
 */
void USER_Usart2RxHandle (e_USART_STATE UartStateRx)
{
	if (UartStateRx != USART_STATE_IDLE)
	{
		switch (UartStateRx)
		{
			case USART_STATE_EMPTY:
			{
				emberAfCorePrintln("USART_STATE_EMPTY\n");
			} break;

			case USART_STATE_DATA_RECEIVED:
			{
				led_turnOn(LED_2, PINK);
				emberAfCorePrintln("USART_STATE_DATA_RECEIVED\n");
			} break;

			case USART_STATE_DATA_ERROR:
			{
				emberAfCorePrintln("USART_STATE_DATA_ERROR\n");
			} break;

			case USART_STATE_ERROR:
			case USART_STATE_RX_TIMEOUT:
			{
				led_turnOff(LED_2);
				emberAfCorePrintln("USART_STATE_ERROR_OR_RX_TIMEOUT\n");
			} break;

			default:
				break;
		}
	}
}

/*
 * @func:		USER_ButtonPressHandle
 *
 * @brief:		The function handles events when a button is pressed
 *
 * @params[1]:	button - Button to be used
 * @params[2]:	pressCount - Number of button presses
 *
 * @retVal:		None
 *
 * @note:		None
 */
void USER_ButtonPressHandle (uint8_t button, uint8_t pressCount)
{
	if(button == SW1)
	{
		switch(pressCount)
		{
			case press_1:
			{
				emberAfCorePrintln("Turn on LED_1: BLUE");
				led_turnOn(LED_1, BLUE);

//				USART_SendPacket(CMD_ID, CMD_TYPE_SET, data, sizeof(data));
				USART_SendPacket(0x1234, 0x01, CMD_ID, CMD_TYPE_SET, data, sizeof(data));
			} break;

			case press_2:
			{
				emberAfCorePrintln("Turn off LED_1");
				led_turnOff(LED_1);

//				USART_SendPacket(CMD_ID, CMD_TYPE_SET, data, sizeof(data));
			} break;

			case press_3:
			{

			} break;

			case press_4:
			{

			} break;

			case press_5:
			{

			} break;

			default:
				break;
		}
	}
	else if (button == SW2)
	{
		switch(pressCount)
		{
			case press_1:
			{

			} break;

			case press_2:
			{

			} break;

			case press_3:
				break;

			case press_4:
				break;

			case press_5:
				break;

			default:
				break;
		}
	}
}

/*
 * @func:		USER_ButtonHoldHandle
 *
 * @brief:		The function handles events when a button is held down
 *
 * @params[1]:	button - Button to be used
 * @params[2]:	holdCount - Button hold time
 *
 * @retVal:		None
 *
 * @note:		None
 */
void USER_ButtonHoldHandle (uint8_t button, uint8_t holdCount)
{
	if(button == SW1)
	{
		switch(holdCount)
		{
			case hold_1s:
				break;

			case hold_2s:
				break;

			case hold_3s:
				break;

			default:
				break;
		}
	}
	else if (button == SW2)
	{
		switch(holdCount)
		{
			case hold_1s:
			{

			} break;

			case hold_2s:
				break;

			case hold_3s:
				break;

			default:
				break;
		}
	}
}

/* END FILE */
