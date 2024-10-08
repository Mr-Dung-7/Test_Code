/*
 * main_ZB_LD2410B_Test.c
 *
 *  Created on: Jul 30, 2024
 *      Author: Mr.hDung
 */

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include <Source/App/Main/main_ZB_LD2410B_Test.h>

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
EmberEventControl mainStateEventControl;
EmberEventControl networkLeaveEventControl;
EmberEventControl delayEventControl;

e_MainState systemState;

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
void emberAfMainInitCallback(void)
{
	emberAfCorePrintln("emberAfMainInitCallback");

	led_Init();

	NETWORK_Init(USER_networkHandle);

//	PIR_Init(USER_PIRHandle);

	systemState = POWER_ON_STATE;

	emberEventControlSetActive(mainStateEventControl);
}

/*
 * @func:		emberAfPreCommandReceivedCallback
 *
 * @brief:		Ham xu ly cac ban tin nhan ve
 *
 * @params:		cmd
 *
 * @retVal:		true / false
 *
 * @note:		None
 */
boolean emberAfPreCommandReceivedCallback (EmberAfClusterCommand* cmd)
{
	if (cmd->clusterSpecific)
	{
		switch(cmd->apsFrame->clusterId)
		{
			case ZCL_ON_OFF_CLUSTER_ID:
			{
				USER_ReceiveOnOffClusterHandle(cmd);
				return true;
			} break;

			default:
				break;
		}
	}

	return false;
}

/*
 * @func:		mainStateEventHandler
 *
 * @brief:		Ham xu ly trang thai cua chuong trinh
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void mainStateEventHandler (void)
{
	emberAfCorePrintln("mainStateEventHandler");
	emberEventControlSetInactive(mainStateEventControl);

	EmberNetworkStatus nwkCurrentStatus;

	switch(systemState)
	{
		case POWER_ON_STATE:
		{
			nwkCurrentStatus = emberAfNetworkState();	// Kiem tra trang thai mang

			if(nwkCurrentStatus == EMBER_NO_NETWORK)
			{
				toggleLed(LED_1, RED, 3, 300, 300);
//				NETWORK_FindAndJoin();
			}

			systemState = IDLE_STATE;
		} break;

		case REPORT_STATE:
		{
			systemState = IDLE_STATE;
			SEND_ReportInfoToHC();
			emberAfCorePrintln("Report State\n");
		} break;

		case IDLE_STATE:
			break;

		case LEAVE_NETWORK:
		{
			SEND_ZigDevRequest();			// Gui ban tin Leave Respond toi HC

			// Need create event timer for leave network produce
			// because after send msg leave network, it must need coor's response
			emberEventControlSetDelayMS(networkLeaveEventControl, 2000);
		} break;

		case REBOOT_STATE:
		{
			systemState = IDLE_STATE;
			halReboot();
		} break;

		default:
			break;
	}
}

/*
 * @func:		networkLeaveEventHandler
 *
 * @brief:		Ham xu ly khi thiet bi roi mang
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void networkLeaveEventHandler (void)
{
	emberEventControlSetInactive (networkLeaveEventControl);

	emberAfCorePrintln("Leaving network");
	systemState = REBOOT_STATE;
	emberClearBindingTable();
	emberLeaveNetwork();

	emberEventControlSetDelayMS(mainStateEventControl, 1000);
}

/*
 * @func:		USER_networkHandle
 *
 * @brief:		Ham xu ly cac su kien cua tang network
 *
 * @params:		networkResult
 *
 * @retVal:		None
 *
 * @note:		None
 */
void USER_networkHandle (e_NetworkState networkResult)
{
	emberAfCorePrintln("USER_networkHandle");

	static bool networkReady = false;

	switch(networkResult)
	{
		case NETWORK_HAS_PARENT:
		{
			emberAfCorePrintln("NETWORK_HAS_PARENT");
			toggleLed(LED_1, PINK, 3, 300, 300);
			networkReady = true;
			systemState = REPORT_STATE;

			emberEventControlSetDelayMS(delayEventControl, 5000);
			emberEventControlSetDelayMS(mainStateEventControl, 1000);
		} break;

		case NETWORK_JOIN_FAIL:
		{
			emberAfCorePrintln("NETWORK_JOIN_FAIL");
			systemState = IDLE_STATE;
			toggleLed(LED_1, RED, 2, 500, 500);

			emberEventControlSetDelayMS(mainStateEventControl, 1000);
		} break;

		case NETWORK_JOIN_SUCCESS:
		{
			emberAfCorePrintln("NETWORK_JOIN_SUCCESS");
			toggleLed(LED_1, PINK, 2, 500, 500);
			networkReady = true;
			systemState = REPORT_STATE;

			emberEventControlSetDelayMS(mainStateEventControl, 1000);
		} break;

		case NETWORK_LOST_PARENT:
		{
			emberAfCorePrintln("NETWORK_LOST_PARENT");
			toggleLed(LED_1, YELLOW, 3, 300, 300);
			systemState = IDLE_STATE;

			emberEventControlSetDelayMS(mainStateEventControl, 1000);
		} break;

		case NETWORK_OUT_NETWORK:
		{
			if(networkReady)
			{
				emberAfCorePrintln("NETWORK_OUT_NETWORK");
				toggleLed(LED_1, BLUE, 3, 300, 300);
				systemState = REBOOT_STATE;

				emberEventControlSetDelayMS(mainStateEventControl, 1000);
			}
		} break;

		default:
			break;
	}
}

///*
// * @func:		USER_PIRHandle
// *
// * @brief:		Ham xu ly cac su kien cua PIR va gui thong tin ve HC
// *
// * @params:		action
// *
// * @retVal:		None
// *
// * @note:		None
// */
//void USER_PIRHandle (e_PIR_Action action)
//{
//	static boolean sendFlag = true;
//
//	switch(action)
//	{
//		case PIR_MOTION:
//		{
//			if(sendFlag)
//			{
//				sendFlag = false;
//				SEND_PIRStateReport(2, PIR_MOTION);
//			}
//
//			toggleLed(LED_1, GREEN, 1, 200, 200);
//			led_turnOn(LED_2, CYAN);
//		} break;
//
//		case PIR_UNMOTION:
//		{
//			sendFlag = true;
//			led_turnOff(LED_2);
//			SEND_PIRStateReport(2, PIR_UNMOTION);
//		} break;
//
//		default:
//			break;
//	}
//}

/*
 * @func:		USER_ReceiveOnOffClusterHandle
 *
 * @brief:		Ham thuc thi ZCL On/Off
 *
 * @params:		cmd
 *
 * @retVal:		None
 *
 * @note:		None
 */
void USER_ReceiveOnOffClusterHandle (EmberAfClusterCommand* cmd)
{
	uint8_t commandID = cmd -> commandId;
	uint8_t endPoint  = cmd -> apsFrame -> destinationEndpoint;

	emberAfCorePrintln("RECEIVE_HandleOnOffCluster SourceEndPoint = %d, CommandID = %d/n",endPoint, commandID);

	switch(commandID)
	{
		case ZCL_ON_COMMAND_ID:
		{
			led_turnOn(LED_1, GREEN);
			SEND_OnOffStateReport(cmd -> apsFrame -> sourceEndpoint, GREEN);
		} break;

		case ZCL_OFF_COMMAND_ID:
		{
			led_turnOff(LED_1);
			SEND_OnOffStateReport(0x01, OFF);
		} break;

		default:
			break;
	}
}

/*
 * @func:		delayEventHandler
 *
 * @brief:		Ham delay thoi gian de goi cac su kien
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void delayEventHandler (void)
{
	emberEventControlSetInactive(delayEventControl);

	PIR_Init(USER_PIRHandle);
}

/* END FILE */
