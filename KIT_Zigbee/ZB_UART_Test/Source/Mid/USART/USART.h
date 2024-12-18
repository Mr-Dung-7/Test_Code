/*
 * USART.h
 *
 *  Created on: Sep 25, 2024
 *      Author: Mr.hDung
 */

#ifndef SOURCE_MID_USART_USART_H_
#define SOURCE_MID_USART_USART_H_

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include <stdbool.h>
#include <stdlib.h>
#include "app/framework/include/af.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/

// The size of the array storing data retrieved from the buffer
#define RX_BUFFER_SIZE 					256

#define PAYLOAD_LENGTH_MAX				2

#define FRAME_START 					0xB1
#define CMD_ID							0x01

// Message retrieving information and status of the device from the host computer
#define CMD_TYPE_GET               		0x00

// Message responding with the device's information and status to the host computer
#define CMD_TYPE_RES               	 	0x01

// Message for controlling the device from the host computer
#define CMD_TYPE_SET               	 	0x02

#define CMD_SEQUENCE					0x00
#define CXOR_INIT_VAL 					0xFF

#define USART_PROCESS_SERIAL_INTERVAL	50

/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/

typedef struct
{
	uint8_t start;
	uint8_t length;
	EmberNodeId nodeId;
	uint8_t endpoint;
	uint8_t id;
	uint8_t type;
	uint8_t payload[PAYLOAD_LENGTH_MAX];
	uint8_t sequence;
	uint8_t cxor;
} st_USART_FRAME_TX;

typedef enum
{
    RX_STATE_START_BYTE,
    RX_STATE_DATA_BYTES,
    RX_STATE_CXOR_BYTE
} e_RX_STATE;

typedef enum
{
    USART_STATE_IDLE,
	USART_STATE_EMPTY,
    USART_STATE_DATA_RECEIVED,
	USART_STATE_DATA_ERROR,
    USART_STATE_ERROR,
    USART_STATE_RX_TIMEOUT,
} e_USART_STATE;


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
typedef void (*pUsartStateCallback)(e_USART_STATE);

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/
void USART2_Init (pUsartStateCallback callback);
void processSerialHandle (void);
uint8_t PollRxBuff (uint8_t port);
void USART_SendPacket (EmberNodeId byNodeId,	\
					   uint8_t byEndpoint,		\
					   uint8_t byCmdId, 		\
					   uint8_t byCmdType,		\
					   uint8_t *pPayload,		\
					   uint8_t byLengthPayload);

//void USART_SendPacket (uint8_t byCmdId, 		\
//					   uint8_t byCmdType,		\
//					   uint8_t *pPayload,		\
//					   uint8_t byLengthPayload);

void USART_WriteDataToCOM (uint8_t port, st_USART_FRAME_TX frame, uint8_t payloadLength);

/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/


#endif /* SOURCE_MID_USART_USART_H_ */

/* END FILE */

