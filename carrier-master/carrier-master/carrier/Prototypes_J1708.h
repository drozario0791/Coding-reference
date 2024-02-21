/*
** ###################################################################
**     Filename  : Prototypes_J1708.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**     This file contains all the function and structure prototypes 
**     that are required for J1708 messaging
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
#ifndef PROTOTYPES_J1708_H_
#define PROTOTYPES_J1708_H_

#include "typedefs.h"

/* 
* NOTE** J1708 is a hardware specific item which is not supported on all modules.  Check the module data sheet
* A J1708 message is made up of
*/
typedef struct 
{
   uint8_t data[MAX_J1708_LENGTH]; 	/* data -> these are the data bytes in messages */
   uint8_t length;					/* length -> how many data bytes are used in the message */
   uint8_t priority;				/* priority -> only used when transmitting a message, this used to determine when to send the message*/
} J1708_Message_;

typedef enum
{
	J1708_WRITE_OK, 		/*Message added to Transmit buffer*/
	J1708_WRITE_BUFFER_FULL /*Transmit buffer is full, could not add*/
}J1708_WRITE_STATUS;

/* This function sends a J1708 message
 * j1708 message is a structure that contains all the info about the j1708 message
 * it will return the following
 * Parameters:
 * j1708message = outgoing J1708 MessageS
 * Returns:  <see J1708_WRITE_STATUS enumeration for details >
 */  
J1708_WRITE_STATUS WriteJ1708(J1708_Message_ j1708message);

#endif /* PROTOTYPES_J1708_H_ */
