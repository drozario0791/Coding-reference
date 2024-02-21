/*
** ###################################################################
**     Filename  : Prototypes_Serial.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**         This file contains all the function and structure prototypes 
**         that are required for Serial Messaging
**
**     (c) Copyright HED,Inc. 2006-2016
**     HED,Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** #########################################################################
**                            !!!  WARNING !!!
**     ALTERING THIS FILE IN ANY WAY MAY CAUSE PERSONAL INJURY AND/OR DEATH.
** #########################################################################
*/

#ifndef PROTOTYPES_SERIAL_H_
#define PROTOTYPES_SERIAL_H_

#include "typedefs.h"


typedef struct
{
   uint8_t Data[MAX_RX_MESSAGE_SIZE];	/* Payload of the messages*/
   uint16_t Count;						/* Count of active bytes in payload*/
   bool_t Pending;						/* If the application is finished with the message set pending to false. 
   	   	   	   	   	   	   	   	   	   	   The firmware will keep calling the function once a loop until the 
   	   	   	   	   	   	   	   	   	   	   application sets pending to false.  No additional messages will be parsed.*/
   uint32_t TimeOut;					/*  Application needs to set what the timeout is in loop counts.  If 
   	   	   	   	   	   	   	   	   	   	   the timeout gets to 0 the module will run the disconnect sequence*/
} Orchestra_Serial_Message_;

/* A enumeration for the serial Connection mechanism the current Serial line is using*/
typedef enum
{
   NO_CONNECTION = -1,					/* No connection established*/
   SERIAL_SCI_CONNECTION,				/* Serial Line connected -either through USB Virtual Com Port or RS-232*/
   CL_802_CONNECTION,				    /* Simulated serial connection using 802 Serial to CAN module*/
   MODULE_CONNECTION					/* Module to Module connection*/
}ORCHESTRA_SERIAL_CONNECTION_TYPE_;

/* A enumeration for the serial line the user wants to reference*/
typedef enum 
{
   SERIALLINE_NOT_USED = -1,
   SERIAL_1,
   SERIAL_2,
   SERIAL_3
} SERIALLINE_;

/* A enumeration for the baud rate the Serial Connect Response is set too*/
typedef enum
{
   MAX_BAUD_RATE_115200 = 5,
   MAX_BAUD_RATE_57600 = 4,
   MAX_BAUD_RATE_9600 = 1,
   MAX_BAUD_RATE_INVALID = -1
} MAX_BAUD_RATE_VALUE_;

/* A enumeration for the return value of Send SCI Status*/
typedef enum
{
	SEND_SCI_BYTE_OK,
	SEND_SCI_BYTE_TRANSMIT_BUFFER_FULL
} SEND_SCI_BYTE_STATUS_;

/* This is to should be filled with SERIALLINE_ the printf function sends the characters out on */
extern SERIALLINE_ SciLineForPrintf;

/*
 * This function sets the baud rate of the SCI line 
 * sci_line = the line of the module you want to modify
 * baud_rate = this is the baud rate that the SCI will be set to
 * 
 * NOTE**  The baud rate is defaulted to 9600
 *        All HED PC programs connection sequence start out at 9600
 *        If the baud rate is not at 9600 the HED tools will fail to connect
 * Parameters:
 * sci_line = serial line to change baud rate on
 * baud_rate = the actual baud rate desired. 
 */
void Init_SCI(SERIALLINE_ sci_line, uint32_t baud_rate);

/*
 * This function sends a byte on the SCI 
 * Parameters:
 * sci_line = the line to send the byte on
 * data_byte = byte that to send
 * Returns:
 */
SEND_SCI_BYTE_STATUS_ Send_SCI_Byte(SERIALLINE_ sci_line, uint8_t data_byte);

/* 
 * This function will change whether the module parses HED's proprietary communication protocol.
 * It also changes which function gets called.  If disabled each byte is sent to User_Serial, 
 * if enabled it will go to Conductor, Downloader, or User_Serial_Packet
 * on the given line.
 * The purpose of these API's are to eliminate any potential conflict between a third-party 
 * protocol and HED's
 * Parameters:
 * type = (see Orchestra_Serial_Connection_Type_ enumeration for different modes)
 * port = either SERIALLINE_ or CANLINE_ depending on the Orchestra_Serial_Connection_Type_ mode
 * disabled = TRUE to disabled HED parsing
 */
void SetHedSerialParsing(ORCHESTRA_SERIAL_CONNECTION_TYPE_ type, uint8_t port, bool_t disabled);

/* 
 * This function will return the status indicating whether the module will parses HED's proprietary
 * communication protocol on the given line.
 * Parameters:
 * type = (see Orchestra_Serial_Connection_Type_ enumeration for different modes)
 * port = either SERIALLINE_ or CANLINE_ depending on the Orchestra_Serial_Connection_Type_ mode
 */
bool_t GetHedSerialParsing(ORCHESTRA_SERIAL_CONNECTION_TYPE_ type, uint8_t port);


#endif /* PROTOTYPES_SERIAL_H_ */
