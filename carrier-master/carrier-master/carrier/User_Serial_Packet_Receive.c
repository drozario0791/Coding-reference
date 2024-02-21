/*
** ###################################################################
**     Filename  : User_Serial_Packet_Receive.c
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**          This is where the user can write applications for serial packet receives
**          This function gets called once per received serial packet
**          A packet consists of:
**          [ = start byte
**          * = denotes that it is a user packet
**          data (up to 147 bytes- 150 totalInclude start, packet Stype, and end byte
**          ] = close byte
**
**     (c) Copyright HED,Inc. 2006-2016
**     HED,Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** ###################################################################
*/

/*Include Header Files */
#include <stdio.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "User_Serial_Packet_Receive.h"


/* 
 * This function is called when a user serial packet is received - either via CANLine (802), Serial port (USB/RS232)
 * Parameters : 
 * message = one packet of data
 * type = type of connection
 * port = one of two different enumerations if type = SERIAL_SCI_CONNECTION it will be the 
 * SERIALLINE_ located in Prototypes_Serial.h otherwise it will be CANLINE_ located in Prototypes_CAN.h
 * 
 * Note: if message.Pending is not set to false the same message will continue to be sent to the function and no new messages will be received
 *       Only leave it as false if you can not handle the message right now (for example the response message will not fit in the tx buffer
 */
Orchestra_Serial_Message_ User_Serial_Packet_Receive(Orchestra_Serial_Message_ message, ORCHESTRA_SERIAL_CONNECTION_TYPE_ type, uint8_t port)
{
   UNUSED_PARAMETER message;/*This line is simply meant to prevent compiler warnings- delete if message is used*/
   UNUSED_PARAMETER type;/*This line is simply meant to prevent compiler warnings- delete if type is used*/
   UNUSED_PARAMETER port;/*This line is simply meant to prevent compiler warnings- delete if port is used*/

	/* Insert your custom serial packet code here */
#warning "Insert your custom serial packet code here, remove this warning after you have set it up"

   message.TimeOut = get_FIVE_SECONDS();
   message.Pending = FALSE;		
   return(message);
}

/* 
 * This function is called when the module detects that the serial connection has been terminated
 */
void User_Serial_Packet_Disconnect(void)
{
	/* Insert your custom serial packet disconnect code here */
#warning "Insert your custom serial packet disconnect code here, remove this warning after you have set it up"
}

/* 
 * This function is called when the serial connection is asked to change it's baud rate
 * Parameters : 
 * max_baud_rate is the maximum baud rate that the connect module can support.
 */
void User_Serial_Connect_Response(MAX_BAUD_RATE_VALUE_ max_baud_rate)
{
	UNUSED_PARAMETER max_baud_rate;/*This line is simply meant to prevent compiler warnings- delete if max_baud_rate is used*/
	
	/* Insert your custom serial packet code to change the baudr rate here */
#warning "Insert your custom serial packet code to change the baud rate here, remove this warning after you have set it up"
}


