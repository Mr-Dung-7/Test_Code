/*
 * main_ZB_LD2410B_Test.h
 *
 *  Created on: Aug 3, 2024
 *      Author: Mr.hDung
 */

#ifndef SOURCE_APP_MAIN_MAIN_ZB_LD2410B_TEST_H_
#define SOURCE_APP_MAIN_MAIN_ZB_LD2410B_TEST_H_

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "app/framework/include/af.h"
#include "Source/App/Network/nwk-user.h"
#include "Source/App/Receive/receive-user.h"
#include "Source/App/Send/send-user.h"
#include "Source/Mid/Led/led-user.h"
#include "Source/Mid/PIR/PIR.h"
#include "Source/Mid/Timer/timer-user.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/
#define BUTTON_TIMEOUT		1500

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
} e_MainState;

/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/


/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/
void USER_networkHandle (e_NetworkState networkResult);
void USER_PIRHandle (e_PIR_Action action);
void USER_ReceiveOnOffClusterHandle (EmberAfClusterCommand* cmd);

/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/


#endif /* SOURCE_APP_MAIN_MAIN_ZB_LD2410B_TEST_H_ */

/* END FILE */
