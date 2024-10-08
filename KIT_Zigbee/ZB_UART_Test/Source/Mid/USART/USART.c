/*
 * USART.c
 *
 *  Created on: Sep 25, 2024
 *      Author: Mr.hDung
 */

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "Source/Mid/USART/USART.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/

/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
EmberEventControl processSerialEventControl;
EmberEventControl usart2ScanEventControl;

UsartFrameCallback usart2FrameCB = NULL;
UsartStateCallback usart2StateCB = NULL;


// Received data storage array
uint8_t 	g_strRxBuffer[RX_BUFFER_SIZE] = {0};
uint8_t 	g_IndexRxBuf = 0;

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/


void USART2_Init (UsartStateCallback cb)
{
	//usart2: Tx: PC01
	//usart2: Rx: PC02

	emberSerialInit(COM_USART2, 115200, PARITY_NONE, 1);

	usart2StateCB = cb;

	emberEventControlSetDelayMS(usart2ScanEventControl, 1000);
}



// Dinh ky lay so luong byte duoc nhan
void usart2ScanEventHandler(void)
{
	emberEventControlSetInactive(usart2ScanEventControl);

	uint8_t 	dataByte;
	uint16_t 	bytesReceived = 0;        // Bien luu so byte da nhan

	// Lay so luong byte da nhan duoc
	uint16_t numOfByteAvail = emberSerialReadAvailable(COM_USART2);

	if(numOfByteAvail > 0)
	{
		while (numOfByteAvail > 0 && bytesReceived < RX_BUFFER_SIZE)
		{
			emberSerialReadByte(COM_USART2, &dataByte);

			g_strRxBuffer[bytesReceived++] = dataByte;	// Luu byte vao buffer
			numOfByteAvail--; 	// Giam so byte con lai can doc
		}

		processSerialHandle();
	}

	emberEventControlSetDelayMS(usart2ScanEventControl, 50);	// 50
}


/*
 * @func:  		processSerialHandle
 *
 * @brief:
 *
 * @param:		None
 *
 * @retval:		None
 *
 * @note:		None
 */
void processSerialHandle (void)
{
	USART_STATE	RxState = PollRxBuff(COM_USART2);

	if (RxState == USART_STATE_DATA_RECEIVED || RxState == USART_STATE_ERROR)
	{
		usart2StateCB(RxState);	// Gui du lieu len han main de thuc thi su kien
		static uint8_t index = 0;
		index++;
		emberAfCorePrintln("Count = %d, state = %d", index, RxState);
	}
}


/*
 * @func:  		PollRxBuff
 *
 * @brief:		The function to process received messages based on format
 *
 * @param:		port
 *
 * @retval:		None
 *
 * @note:		None
 */

uint8_t PollRxBuff (uint8_t port)
{
	uint8_t 	byUartState = (uint8_t) USART_STATE_IDLE;
	uint8_t 	RxBufState = RX_STATE_START_BYTE;
	uint8_t 	byCheckXorRxBuf = 0;

	g_IndexRxBuf = 0;

	while ((port == COM_USART2) && (byUartState == USART_STATE_IDLE))
	{
		// Variable to store value after retrieved from buffer
		uint8_t 	byRxData = g_strRxBuffer[g_IndexRxBuf];

		switch (RxBufState)
		{
			case RX_STATE_START_BYTE:
			{
				if (byRxData == FRAME_START)
				{
					g_IndexRxBuf++;
					byCheckXorRxBuf = CXOR_INIT_VAL;
					RxBufState = RX_STATE_DATA_BYTES;
				}
				else
				{
					byUartState = USART_STATE_ERROR;//EMPTY
				}
			} break;

			case RX_STATE_DATA_BYTES:
			{
				if (g_IndexRxBuf < RX_BUFFER_SIZE)
				{
					if (g_IndexRxBuf > 1)
					{
						byCheckXorRxBuf ^= byRxData;		// Calculator CXOR
					}

					if (g_IndexRxBuf == *(g_strRxBuffer + 1))		// g_IndexRxBuf == FRAME_LENGTH
					{
						RxBufState = RX_STATE_CXOR_BYTE;
					}

					g_IndexRxBuf++;
				}
				else
				{
					RxBufState = RX_STATE_START_BYTE;
					byUartState = USART_STATE_ERROR;
				}
			} break;

			case RX_STATE_CXOR_BYTE:
			{
				if (byRxData == byCheckXorRxBuf)
				{
					byUartState = USART_STATE_DATA_RECEIVED;
				}
				else
				{
					byUartState = USART_STATE_ERROR;
				}

				RxBufState = RX_STATE_START_BYTE;
			} break;

			default:
				RxBufState = RX_STATE_START_BYTE;
				break;
		}
	}

	return byUartState;
}


void USART_WriteDataToFrame(uint8_t port, USART_FRAME_TX frame)
{
	emberAfCorePrintln("USART_WriteDataToFrame");

	uint8_t nodeId_byte1 = (uint8_t)((frame.nodeId >> 8) & 0xFF);
	uint8_t nodeId_byte2 = (uint8_t)(frame.nodeId & 0xFF);

//	frame.length = 6;
//	frame.id = 2;
//	frame.type = 0;
//	frame.payload[0] = 1;
//	frame.payload[1] = 170;
//	frame.sequence = 1;
//	frame.cxor = 0x57;

	uint8_t startByte = FRAME_START;

	emberSerialWriteData(COM_USART2, &startByte, 1);
	emberSerialWriteData(COM_USART2, &frame.length, 1);
	emberSerialWriteData(COM_USART2, &nodeId_byte1, 1);
	emberSerialWriteData(COM_USART2, &nodeId_byte2, 1);
	emberSerialWriteData(COM_USART2, &frame.endpoint,1);
	emberSerialWriteData(COM_USART2, &frame.id, 1);
	emberSerialWriteData(COM_USART2, &frame.type, 1);

	for(uint8_t i = 0; i < PAYLOAD_MAX_LENGTH; i++)
	{
		emberSerialWriteData(COM_USART2, (&frame.payload[i]), 1);
	}

	emberSerialWriteData(COM_USART2, &frame.sequence, 1);
	emberSerialWriteData(COM_USART2, &frame.cxor, 1);
}



/*
 * @func:  		Serial_SendPacketCustom
 *
 * @brief:		The function send text to PC_Simulator_KIT
 *
 * @param[1]:	nodeID - Byte Option of the frame
 * @param[2]:	endpoint - Byte endpoint of the frame
 * @param[3]:	cmdId - Byte cmdId of the frame
 * @param[4]:	cmdType - Byte cmdType of the frame
 * @param[5]:	pPayload - Byte pPayload of the frame
 * @param[6]:	lengthPayload - Data size
 *
 * @retval:		None
 *
 * @note:		None
 */
void USART_SendFrame (EmberNodeId nodeID,		\
					  uint8_t endpoint,			\
					  uint8_t cmdId, 			\
					  uint8_t cmdType,		\
					  uint8_t *pPayload,		\
					  uint8_t lengthPayload)
{
	uint8_t length = lengthPayload + 7;
	uint8_t sequence = 0;
	uint8_t cxor = CXOR_INIT_VAL;




}
