/*
 * USART.c
 *
 *  Created on: Sep 25, 2024
 *      Author: Mr.hDung
 */

/******************************************************************************/
/*                                INCLUDEs                                    */
/******************************************************************************/
#include "Source/Mid/Usart/USART.h"

/******************************************************************************/
/*                                 DEFINEs                                    */
/******************************************************************************/


/******************************************************************************/
/*                            STRUCTs AND ENUMs                               */
/******************************************************************************/


/******************************************************************************/
/*                       EVENTs AND GLOBAL VARIABLEs                          */
/******************************************************************************/
EmberEventControl usart2ScanEventControl;

pUsartStateCallback usart2StateCallback = NULL;

uint8_t 	g_IndexRxBuf = 0;

// Received data storage array
uint8_t 	g_strRxBuffer[RX_BUFFER_SIZE] = {0};

/******************************************************************************/
/*                           FUNCTIONs  PROTOTYPE                             */
/******************************************************************************/


/******************************************************************************/
/*                               FUNCTIONs                              	  */
/******************************************************************************/

/*
 * @func:  		USART2_Init
 *
 * @brief:		Initialize USART
 *
 * @param:		callback
 *
 * @retval:		None
 *
 * @note:
 * 				+ usart2: Tx - PC01
 * 				+ usart2: Rx - PC02
 */
void USART2_Init (pUsartStateCallback callback)
{
	emberSerialInit(COM_USART2, 115200, PARITY_NONE, 1);

	usart2StateCallback = callback;

	// Start the scan event, with a delay on the first call: waiting for peripheral initialization
	emberEventControlSetDelayMS(usart2ScanEventControl, 1000);
}

/*
 * @func:  		usart2ScanEventHandler
 *
 * @brief:		Periodic data retrieval function
 *
 * @param:		None
 *
 * @retval:		None
 *
 * @note:		None
 */
void usart2ScanEventHandler (void)
{
	emberEventControlSetInactive(usart2ScanEventControl);

	uint8_t 	dataByte;
	uint16_t 	bytesReceived = 0;        // Variable storing the number of received bytes

	// Get the number of received bytes
	uint16_t numOfByteAvail = emberSerialReadAvailable(COM_USART2);

	if(numOfByteAvail > 0)
	{
		while (numOfByteAvail > 0 && bytesReceived < RX_BUFFER_SIZE)
		{
			emberSerialReadByte(COM_USART2, &dataByte);

			g_strRxBuffer[bytesReceived++] = dataByte;		// Store the received byte in the buffer
			numOfByteAvail--;
		}

		processSerialHandle();
	}

	emberEventControlSetDelayMS(usart2ScanEventControl, USART_PROCESS_SERIAL_INTERVAL);
}


/*
 * @func:  		processSerialHandle
 *
 * @brief:		The function retrieves and sends data to the main function for event execution
 *
 * @param:		None
 *
 * @retval:		None
 *
 * @note:		None
 */
void processSerialHandle (void)
{
	USART_STATE_e	RxState = PollRxBuff(COM_USART2);

	if (RxState == USART_STATE_DATA_RECEIVED || RxState == USART_STATE_ERROR)
	{
		if (usart2StateCallback != NULL)
		{
			usart2StateCallback(RxState);
		}
	}
}

/*
 * @func:  		PollRxBuff
 *
 * @brief:		The function to process received messages based on format
 *
 * @param:		port - USART port
 *
 * @retval:		byUartState - USART state
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
					byUartState = USART_STATE_EMPTY;	// Buffer EMPTY
				}
			} break;

			case RX_STATE_DATA_BYTES:
			{
				if (g_IndexRxBuf < RX_BUFFER_SIZE)
				{
					if (g_IndexRxBuf > 1)
					{
						byCheckXorRxBuf ^= byRxData;		// Calculate CXOR
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
					byUartState = USART_STATE_DATA_ERROR;
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

/*
 * @func:  		USART_SendPacket
 *
 * @brief:		The function sends data in the specified format
 *
 * @param[1]:	byNodeId - Byte nodeID of the frame
 * @param[2]:	byEndpoint - Byte endpoint of the frame
 * @param[3]:	byCmdId - Byte cmdId of the frame
 * @param[4]:	byCmdType - Byte cmdType of the frame
 * @param[5]:	pPayload - Byte Data of the frame
 * @param[6]:	byLengthPayload - Data length
 *
 * @retval:		None
 *
 * @note:		None
 */
void USART_SendPacket (EmberNodeId byNodeId,	\
					   uint8_t byEndpoint,		\
					   uint8_t byCmdId, 		\
					   uint8_t byCmdType,		\
					   uint8_t *pPayload,		\
					   uint8_t byLengthPayload)
{
	uint8_t byLength = byLengthPayload + 7;
	static uint8_t bySequence = CMD_SEQUENCE;
	uint8_t byCXOR = CXOR_INIT_VAL;
	uint8_t index = 0;
	uint8_t Frame[byLength + 2];

	Frame[index++] = FRAME_START;
	Frame[index++] = byLength;
	Frame[index++] = (byNodeId >> 8) & 0xFF;
	Frame[index++] = byNodeId & 0xFF;
	Frame[index++] = byEndpoint;
	Frame[index++] = byCmdId;
	Frame[index++] = byCmdType;

	for (uint8_t i = 0; i < byLengthPayload; i++)
	{
		Frame[index++] = pPayload[i];
	}

	Frame[index++] = bySequence++;

	for (uint8_t j = 2; j < index; j++)
	{
		byCXOR ^= Frame[j];
	}
	Frame[index++] = byCXOR;

	emberSerialWriteData(COM_USART2, Frame, sizeof(Frame));

}

/* END FILE */
