/*
 * nwk-user.c
 *
 *  Created on: Jul 30, 2024
 *      Author: Mr.hDung
 */

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "Source/App/Network/nwk-user.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
EmberEventControl joinNetworkEventControl;

uint32_t timeFindAndJoin = 0;
pNetworkHandle networkHandlerCallback = NULL;

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/
/*
 * @func:		NETWORK_Init
 *
 * @brief:		Ham khoi tao Network
 *
 * @params:		cb
 *
 * @retVal:		None
 *
 * @note:		None
 */
void NETWORK_Init (pNetworkHandle cb)
{
	networkHandlerCallback = cb;
}

/*
 * @func:		NETWORK_FindAndJoin
 *
 * @brief:		Ham thuc hien tim kiem mang sau 2s ke tu khi goi ham tim mang
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void NETWORK_FindAndJoin (void)
{
	if(emberAfNetworkState() == EMBER_NO_NETWORK)
	{
		emberEventControlSetDelayMS(joinNetworkEventControl, 2000);
	}
}

/*
 * @func:		NETWORK_StopFindAndJoin
 *
 * @brief:		Ham dung qua trinh tim kiem mang
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void NETWORK_StopFindAndJoin (void)
{
	if(emberAfNetworkState() == EMBER_JOINED_NETWORK)
	{
		emberEventControlSetDelayMS(joinNetworkEventControl, 2000);
	}
}

/*
 * @func:		joinNetworkEventHandler
 *
 * @brief:		Ham gia nhap mang khi chua co mang
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void joinNetworkEventHandler (void)
{
	emberEventControlSetInactive(joinNetworkEventControl);

	EmberStatus status = emberAfNetworkState();

	if(status == EMBER_NO_NETWORK)
	{
		emberAfPluginNetworkSteeringStart();
		timeFindAndJoin++;
		emberEventControlSetDelayMS(joinNetworkEventControl, 5000);
	}
	else if (status == EMBER_JOINED_NETWORK)
	{
		emberAfPluginNetworkSteeringStop();
		timeFindAndJoin = 0;
	}
}

/*
 * @func:		emberAfStackStatusCallback
 *
 * @brief:		Ham cap nhat trang thai cua ha tang mang Zigbee
 *
 * @params:		status
 *
 * @retVal:		true / false
 *
 * @note:		None
 */
boolean emberAfStackStatusCallback (EmberStatus status)
{
	emberAfCorePrintln("nwk-emberAfStackStatusCallback\n");

	if (status == EMBER_NETWORK_UP)
	{
		if (timeFindAndJoin > 0)	//join success
		{
			NETWORK_StopFindAndJoin();

			if(networkHandlerCallback != NULL)
			{
				networkHandlerCallback(NETWORK_JOIN_SUCCESS);
			}
		}
		else	//has parent already and connected to parent success
		{
			if(networkHandlerCallback != NULL)
			{
				networkHandlerCallback(NETWORK_HAS_PARENT);
			}
		}
	}
	else if (status == EMBER_NETWORK_DOWN)
	{
		EmberNetworkStatus nwkCurrentStatus = emberAfNetworkState();

		if(nwkCurrentStatus == EMBER_NO_NETWORK)
		{
			if(networkHandlerCallback != NULL)
			{
				networkHandlerCallback(NETWORK_OUT_NETWORK);
			}
		}
		else if (nwkCurrentStatus == EMBER_JOINED_NETWORK_NO_PARENT)
		{
			if(networkHandlerCallback != NULL)
			{
			    networkHandlerCallback(NETWORK_LOST_PARENT);
			}
		}
	}

	return false;
}

/* END FILE */
