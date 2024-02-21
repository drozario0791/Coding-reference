/******************************************************************************
 *
 *        Name: cvc_debug_msgs.h
 *
 * Description: Transmits Msgs from the CVC
 *
 *      Author: Deepak
 *        Date: Monday, 23 September 2019
 *
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "typedefs.h"
#include "Prototypes.h"
#include "Prototypes_CAN.h"
#include "can_service_private.h"
#include "can_service_devices.h"
#include "cvc_debug_msgs.h"


void send_debug_can_messages_32bits(uint32_t can_id,
                                    uint32_t data1,
                                    uint32_t data2)
{
    static uint16_t count = 0;
    static uint16_t timer = 100;

    Can_Message_ tx_message;

	can_data_t *data_ptr = (can_data_t *)tx_message.data;

    tx_message.identifier = can_id;
    tx_message.length = 8;
    tx_message.type = EXTENDED;

    data_ptr->MDL.bit32 = BYTE_SWAP32(data1);
    data_ptr->MDH.bit32 = BYTE_SWAP32(data2);

    if(count > timer)
    {
        Send_CAN_Message(0, CAN3, tx_message);
        count = 0;
    }

    count++;

}



void send_debug_can_messages_16bits(uint32_t can_id,
		uint16_t data1,
		uint16_t data2,
		uint16_t data3,
		uint16_t data4)
{
    static uint16_t count = 0;
    static uint16_t timer = 10;

    Can_Message_ tx_message;

	can_data_t *data_ptr = (can_data_t *)tx_message.data;

    tx_message.identifier = can_id;
    tx_message.length = 8;
    tx_message.type = EXTENDED;

    data_ptr->MDL.bit16.HALF0 = BYTE_SWAP16(data1);
    data_ptr->MDL.bit16.HALF1 = BYTE_SWAP16(data2);
    data_ptr->MDH.bit16.HALF0 = BYTE_SWAP16(data3);
    data_ptr->MDH.bit16.HALF1 = BYTE_SWAP16(data4);

    if(count > timer)
    {
        Send_CAN_Message(0, CAN3, tx_message);
        count = 0;
    }

    count++;
}


void send_debug_can_messages_8bits(uint32_t can_id,
		uint8_t data1,
		uint8_t  data2,
		uint8_t data3,
		uint8_t data4,
		uint8_t data5,
		uint8_t data6,
		uint8_t data7,
		uint8_t data8)
{

    static uint16_t count = 0;
    static uint16_t timer = 10;
    Can_Message_ tx_message;
    can_data_t *data_ptr = (can_data_t *)tx_message.data;

    tx_message.identifier = can_id;
    tx_message.length = 8;
    tx_message.type = EXTENDED;

    data_ptr->MDL.bit8.BYTE0 = data1;
    data_ptr->MDL.bit8.BYTE1 = data2;
    data_ptr->MDL.bit8.BYTE2 = data3;
    data_ptr->MDL.bit8.BYTE3 = data4;

    data_ptr->MDH.bit8.BYTE0 = data5;
    data_ptr->MDH.bit8.BYTE1 = data6;
    data_ptr->MDH.bit8.BYTE2 = data7;
    data_ptr->MDH.bit8.BYTE3 = data8;

    if(count > timer)
    {
        Send_CAN_Message(0, CAN3, tx_message);
        count = 0;

    }

    count++;

}
