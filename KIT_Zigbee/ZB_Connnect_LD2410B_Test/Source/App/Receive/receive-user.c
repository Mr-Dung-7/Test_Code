/*
 * receive-user.c
 *
 *  Created on: Jul 30, 2024
 *      Author: Mr.hDung
 */

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "Source/App/Receive/receive-user.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/


/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/
/*
 * @func:		emberAfPreMessageReceivedCallback
 *
 * @brief:		This function is called when any Zigbee message is received and
 * 				indicates whether the message has been processed or not
 *
 * @params:		im
 *
 * @retVal:		true / false
 *
 * @note:		None
 */
boolean emberAfPreMessageReceivedCallback (EmberAfIncomingMessage* im)
{
	uint16_t clusterID = im->apsFrame->clusterId;

	if(clusterID == ACTIVE_ENDPOINTS_RESPONSE)
	{
		return true;
	}

	return false;
}

/* END FILE */
