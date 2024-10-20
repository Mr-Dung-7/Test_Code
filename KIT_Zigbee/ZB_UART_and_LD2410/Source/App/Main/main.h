/*
 * main.h
 *
 *  Created on: Aug 8, 2024
 *      Author: Mr.hDung
 */

#ifndef SOURCE_APP_MAIN_MAIN_H_
#define SOURCE_APP_MAIN_MAIN_H_

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "app/framework/include/af.h"
#include "protocol/zigbee/stack/include/binding-table.h"
#include "Source/App/Network/nwk-user.h"
#include "Source/App/Receive/receive-user.h"
#include "Source/App/Send/send-user.h"
#include "Source/Mid/Button/button-user.h"
#include "Source/Mid/Kalman-Filter/kalman_filter.h"
#include "Source/Mid/LD2410/ld2410.h"
#include "Source/Mid/LDR/ldr-user.h"
#include "Source/Mid/Led/led-user.h"
#include "Source/Mid/Si7020/TemHumSensor.h"
#include "Source/Mid/Timer/timer-user.h"
#include "Source/Mid/Usart/USART.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/
#define ENDPOINT_1					1
#define ENDPOINT_2					2
#define ENDPOINT_3					3

#define THRESHOLD_LUX_REPORT		30
#define THRESHOLD_LUX_CONTROL_LED	500

/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/
typedef enum
{
	POWER_ON_STATE,
	REPORT_STATE,
	IDLE_STATE,
	LEAVE_NETWORK,
	REBOOT_STATE
} MainState_e;

/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/


/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/
void USER_NetworkHandle (NetworkState_e networkResult);
void USER_ButtonPressHandle (uint8_t button, uint8_t pressCount);
void USER_ButtonHoldHandle (uint8_t button, uint8_t holdCount);
void USER_ReceiveOnOffClusterHandle (EmberAfClusterCommand* cmd);
void USER_ReceiveLeaveHandle (EmberNodeId nodeId, RECEIVE_CMD_ID_e receiveId);
void USER_LD2410Handle (LD2410_Action_e action);
void USER_LdrUpdateValueLight (void);
void USER_Usart2RxHandle (USART_STATE_e UsartStateRx);

/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/


#endif /* SOURCE_APP_MAIN_MAIN_H_ */

/* END FILE */
