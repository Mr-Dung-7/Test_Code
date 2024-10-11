/*
 * send-user.h
 *
 *  Created on: Jul 30, 2024
 *      Author: Mr.hDung
 */

#ifndef SOURCE_APP_SEND_SEND_USER_H_
#define SOURCE_APP_SEND_SEND_USER_H_

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "app/framework/include/af.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/
#define SOURCE_ENDPOINT_PRIMARY				1
#define DESTINATION_ENDPOINT				1
#define HC_NETWORK_ADDRESS					0x0000

/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/


/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/
void SEND_FillBufferGlobalCommand (EmberAfClusterId clusterId,
							 	   EmberAfAttributeId attributeId,
								   uint8_t commandId,
								   uint8_t *pDataRead,
								   uint8_t length,
								   uint8_t dataType);
void SEND_SendCommandUnicast (uint8_t sourceEP, uint8_t destEP, uint32_t nwkAddress);
void SEND_ZigDevRequest (void);
void SEND_ReportInfoToHC (void);
void SEND_OnOffStateReport (uint8_t srcEP, uint8_t state);
void SEND_LevelStateReport (uint8_t srcEP, uint8_t value);
void SEND_PIRStateReport (uint8_t srcEP, uint8_t value);

/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/


#endif /* SOURCE_APP_SEND_SEND_USER_H_ */

/* END FILE */