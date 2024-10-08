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
#include "Source/Utils/Queue/Queue.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/
// Kich thuoc hang doi
#define SIZE_QUEUE_DATA_RX	256

// The size of the array storing data retrieved from the queue
#define RX_BUFFER_SIZE 					256

#define PAYLOAD_MAX_LENGTH				2

#define FRAME_START 					0xB1

// Message retrieving information and status of the device from the host computer
#define CMD_TYPE_GET               		0x00

// Message responding with the device's information and status to the host computer
#define CMD_TYPE_RES               	 	0x01

// Message for controlling the device from the host computer
#define CMD_TYPE_SET               	 	0x02

#define CMD_SEQUENCE					0x00
#define CXOR_INIT_VAL 					0xFF

#define FRAME_ACK 						0x06
#define FRAME_NACK 						0x15

#define USART_PROCESS_SERIAL_INTERVAL	50

/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/

typedef struct
{
	uint8_t length;
	EmberNodeId nodeId;
	uint8_t endpoint;
	uint8_t id;
	uint8_t type;
	uint8_t payload[PAYLOAD_MAX_LENGTH];
	uint8_t sequence;
	uint8_t cxor;
} USART_FRAME_TX;

typedef enum
{
    RX_STATE_START_BYTE,
    RX_STATE_DATA_BYTES,
    RX_STATE_CXOR_BYTE
} RX_STATE;

typedef enum
{
    USART_STATE_IDLE,
    USART_STATE_DATA_RECEIVED,
    USART_STATE_ERROR,
    USART_STATE_RX_TIMEOUT,
} USART_STATE;


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
typedef void (*UsartFrameCallback)(USART_FRAME_TX);
typedef void (*UsartStateCallback)(USART_STATE);

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/
void USART2_Init (UsartStateCallback cb);
uint8_t PollRxBuff (uint8_t port);
void processSerialHandle (void);
void USART_SendFrame(uint8_t port, USART_FRAME_TX frame);


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/










#endif /* SOURCE_MID_USART_USART_H_ */

/* END FILE */

