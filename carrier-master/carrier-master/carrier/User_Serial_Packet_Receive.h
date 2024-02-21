/*
** ###################################################################
**     Filename  : User_Serial_Packet_Receive.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
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
#ifndef _USER_SERIAL_PACKET_RECEIVE_H_
#define _USER_SERIAL_PACKET_RECEIVE_H_

/*
 * Function Prototypes
 */
Orchestra_Serial_Message_ User_Serial_Packet_Receive(Orchestra_Serial_Message_ message, ORCHESTRA_SERIAL_CONNECTION_TYPE_ type, uint8_t number);
void User_Serial_Packet_Disconnect(void);
void User_Serial_Connect_Response(MAX_BAUD_RATE_VALUE_ max_baud_rate);

#endif /* _USER_SERIAL_PACKET_RECEIVE_H_ */

