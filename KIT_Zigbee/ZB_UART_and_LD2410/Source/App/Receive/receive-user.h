/*
 * receive-user.h
 *
 *  Created on: Jul 30, 2024
 *      Author: Mr.hDung
 */

#ifndef SOURCE_APP_RECEIVE_RECEIVE_USER_H_
#define SOURCE_APP_RECEIVE_RECEIVE_USER_H_

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "app/framework/include/af.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/
typedef enum
{
	DEVICE_JOIN_NETWORK,
	DEVICE_CONNECTED,
	DEVICE_DISCONNECTED,
	LEVEL_CONTROL,
	DEVICE_LEAVE_NETWORK
} RECEIVE_CMD_ID_e;

/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
typedef void (*pReceiveHandler)(EmberNodeId nodeID, RECEIVE_CMD_ID_e receiveId);

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/
void Receive_Init (pReceiveHandler callback);

/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/


#endif /* SOURCE_APP_RECEIVE_RECEIVE_USER_H_ */

/* END FILE */
