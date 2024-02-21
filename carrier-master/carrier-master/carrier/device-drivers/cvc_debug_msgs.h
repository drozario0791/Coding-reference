/******************************************************************************
 *
 *        Name: cvc_debug_msgs.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 23 September 2019
 *
 ******************************************************************************
 */

#ifndef CVC_DEBUG_MSGS_H_
#define CVC_DEBUG_MSGS_H_

void send_debug_can_messages_32bits(uint32_t can_id,
                                    uint32_t data1,
                                    uint32_t data2);

void send_debug_can_messages_16bits(uint32_t can_id,
                                    uint16_t data1,
                                    uint16_t data2,
                                    uint16_t data3,
                                    uint16_t data4);

void send_debug_can_messages_8bits(uint32_t can_id,
                                   uint8_t data1,
                                   uint8_t  data2,
                                   uint8_t data3,
                                   uint8_t data4,
                                   uint8_t data5,
                                   uint8_t data6,
                                   uint8_t data7,
                                   uint8_t data8);


#endif
