/*
** ###################################################################
**     Filename  : User_J1708_Receive.c
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**          This is where the user can write applications for J1708 message receives
**          This function gets called once per received J1708 message
**
**     (c) Copyright HED,Inc. 2006-2016
**     HED,Inc.S
**     2120 Constitution Ave.S
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** ###################################################################
*/

/*Include Header Files */
#include "reserved.h"
#include "Prototypes.h"
#ifdef PROTOTYPES_J1708_HEADER_FILE	
#include "Constants.h"
#include "User_App.h"
#include "User_J1708_Receive.h"

/* 
 * This function gets called from the main loop when J1708 messages are 
 * present in FIFO buffer
 * Parameter:
 * j1708message = parsed J1708 message received
 */
void User_J1708_Receive(J1708_Message_ j1708message)
{
	UNUSED_PARAMETER j1708message;/*This line is simply meant to prevent compiler warnings- delete if j1708message is used*/

	/* Insert your custom J1708 RX parsing code here */
#warning "Insert your J1708 RX parsing code here, remove this warning after you have set it up"
}
#endif
