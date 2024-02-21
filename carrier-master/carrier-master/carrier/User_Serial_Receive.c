/*
** ###################################################################
**     Filename  : User_Serial_Receive.c
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**          This is where the user can write applications for serial byte receives
**          This function gets called once per received serial byte
**          The user needs to return if their application uses the byte
**          If the user mistakenly uses an HED message it will cause the PC tools not to work properly
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
#include "User_Serial_Receive.h"

/* 
 * This function is called when a serial byte is received - either via CANLine (802), Serial port (USB/RS232)
 * Parameters : 
 * sci_byte = one character of data
 * type = type of connection
 * port = one of two different enumerations if type = SERIAL_SCI_CONNECTION it will be the 
 * SERIALLINE_ located in Prototypes_Serial.h otherwise it will be CANLINE_ located in Prototypes_CAN.h
 */
void User_Serial_Receive(uint8_t sci_byte, ORCHESTRA_SERIAL_CONNECTION_TYPE_ type, uint8_t port)
{
	UNUSED_PARAMETER sci_byte;/*This line is simply meant to prevent compiler warnings- delete if scibyte is used*/
	UNUSED_PARAMETER type;/*This line is simply meant to prevent compiler warnings- delete if type is used*/
	UNUSED_PARAMETER port;/*This line is simply meant to prevent compiler warnings- delete if port is used*/

	/* Insert your custom serial byte rx code here */
#warning "Insert your custom serial byte rx code here, remove this warning after you have set it up"
}
