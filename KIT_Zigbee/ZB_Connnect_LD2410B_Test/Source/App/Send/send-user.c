/*
 * send-user.c
 *
 *  Created on: Jul 30, 2024
 *      Author: Mr.hDung
 */

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "Source/App/Send/send-user.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/
#define MAX_DATA_COMMAND_SIZE		50


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
 * @func:		SEND_FillBufferGlobalCommand
 *
 * @brief:		Ham thuc hien viec tao Frame va truyen vao Buffer
 *
 * @params[1]:	clusterId
 * @params[2]:	attributeId
 * @params[3]:	commandId
 * @params[4]:	pDataRead
 * @params[5]:	length
 * @params[6]:	dataType
 *
 * @retVal:		None
 *
 * @note:		None
 */
void SEND_FillBufferGlobalCommand (EmberAfClusterId clusterId,
							 	   EmberAfAttributeId attributeId,
								   uint8_t commandId,
								   uint8_t *pDataRead,
								   uint8_t length,
								   uint8_t dataType)
{
	uint8_t data[MAX_DATA_COMMAND_SIZE];
	data[0] = (uint8_t)(attributeId & 0x00FF);
	data[1] = (uint8_t)((attributeId & 0xFF00) >> 8);
	data[2] = EMBER_SUCCESS;
	data[3] = dataType;

	memcpy(&data[4], pDataRead, length);

	(void)emberAfFillExternalBuffer((ZCL_GLOBAL_COMMAND |
									 ZCL_FRAME_CONTROL_CLIENT_TO_SERVER |
									 ZCL_DISABLE_DEFAULT_RESPONSE_MASK),
									 clusterId, commandId, "b", data, length + 4);
}

/*
 * @func:		SEND_SendCommandUnicast
 *
 * @brief:		Ham cai dat source endpoint va destination endpoint cho command,
 * 				sau do gui di bang unicast
 *
 * @params[1]:	sourceEP
 * @params[2]:	destEP
 * @params[3]:	nwkAddress
 *
 * @retVal:		None
 *
 * @note:		None
 */
void SEND_SendCommandUnicast (uint8_t sourceEP, uint8_t destEP, uint32_t nwkAddress)
{
	emberAfSetCommandEndpoints(sourceEP, destEP);
	(void)emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, nwkAddress);
}

/*
 * @func:		SEND_ZigDevRequest
 *
 * @brief:		Ham thuc hien gui zdo thong bao roi mang cho ZC
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void SEND_ZigDevRequest (void)
{
	uint8_t contents[2];
	contents[1] = 0x00;

	// Truyen ban tin ZDO
	emberSendZigDevRequest(HC_NETWORK_ADDRESS,
						   LEAVE_RESPONSE,
						   EMBER_AF_DEFAULT_APS_OPTIONS,
						   contents,
						   sizeof(contents));
}

/*
 * @func:		SEND_ReportInfoToHC
 *
 * @brief:		Ham thuc hien gui cac thong tin cho Home Controller
 *
 * @params:		None
 *
 * @retVal:		None
 *
 * @note:		None
 */
void SEND_ReportInfoToHC (void)
{
	uint8_t modelID[] = {'P', 'I', 'R', '1'};
	uint8_t manufactureID[] = {'L', 'u', 'm', 'i'};
	uint8_t version[] = {'1'};

	if(emberAfNetworkState() != EMBER_JOINED_NETWORK)
	{
		return;
	}

	//Send model to HC
	SEND_FillBufferGlobalCommand(ZCL_BASIC_CLUSTER_ID,
								 ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
								 ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								 modelID,
								 4,
								 ZCL_CHAR_STRING_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(SOURCE_ENDPOINT_PRIMARY,
							DESTINATION_ENDPOINT,
							HC_NETWORK_ADDRESS);

	//Send manufacture to HC
	SEND_FillBufferGlobalCommand(ZCL_BASIC_CLUSTER_ID,
								 ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
								 ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								 manufactureID,
								 4,
								 ZCL_CHAR_STRING_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(SOURCE_ENDPOINT_PRIMARY,
							DESTINATION_ENDPOINT,
							HC_NETWORK_ADDRESS);

	//Send version to HC
	SEND_FillBufferGlobalCommand(ZCL_BASIC_CLUSTER_ID,
								 ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID,
								 ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								 version,
								 1,
								 ZCL_CHAR_STRING_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(SOURCE_ENDPOINT_PRIMARY,
							DESTINATION_ENDPOINT,
							HC_NETWORK_ADDRESS);
}

/*
 * @func:		SEND_OnOffStateReport
 *
 * @brief:		Ham hien gui thong bao On/Off Led cho HC
 *
 * @params[1]:	srcEP
 * @params[2]:	state
 *
 * @retVal:		None
 *
 * @note:		None
 */
void SEND_OnOffStateReport (uint8_t srcEP, uint8_t state)
{
	SEND_FillBufferGlobalCommand(ZCL_ON_OFF_CLUSTER_ID,
								 ZCL_ON_OFF_ATTRIBUTE_ID,
								 ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								 (uint8_t*)&state,
								 1,
								 ZCL_BOOLEAN_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(srcEP,
							DESTINATION_ENDPOINT,
							HC_NETWORK_ADDRESS);
}

/*
 * @func:		SEND_LevelStateReport
 *
 * @brief:		Ham thuc hien gui gia tri do sang Led cho HC
 *
 * @params[1]:	srcEP
 * @params[2]:	value
 *
 * @retVal:		None
 *
 * @note:		None
 */
void SEND_LevelStateReport (uint8_t srcEP, uint8_t value)
{
	SEND_FillBufferGlobalCommand(ZCL_LEVEL_CONTROL_CLUSTER_ID,
								 ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
								 ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								 (uint8_t*)&value,
								 1,
								 ZCL_INT8U_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(srcEP,
							DESTINATION_ENDPOINT,
							HC_NETWORK_ADDRESS);
}

/*
 * @func:		SEND_PIRStateReport
 *
 * @brief:		Ham thuc hien gui trang thai phat hien duoc tu cam bien PIR cho HC
 *
 * @params[1]:	srcEP
 * @params[2]:	value
 *
 * @retVal:		None
 *
 * @note:		None
 */
void SEND_PIRStateReport (uint8_t srcEP, uint8_t value)
{
	SEND_FillBufferGlobalCommand(ZCL_IAS_ZONE_CLUSTER_ID,
								 ZCL_ZONE_STATUS_ATTRIBUTE_ID,
								 ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID,
								 (uint16_t*)&value,
								 2,
								 ZCL_BITMAP16_ATTRIBUTE_TYPE);

	SEND_SendCommandUnicast(srcEP,
							DESTINATION_ENDPOINT,
							HC_NETWORK_ADDRESS);
}

/* END FILE */
