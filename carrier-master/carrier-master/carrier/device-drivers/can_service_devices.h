/******************************************************************************
 *
 *        Name: can_service.h
 *
 * Description:
 *
 *      Author: Tom & Deepak
 *        Date: Wednesday, 19 June 2019
 *
 ******************************************************************************
 */

#ifndef CAN_SERVICE_H
#define CAN_SERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Prototypes_CAN.h"
#include "typedefs.h"
#include "can_service.h"

//*****************************************************************************
//
// These typedefs form the basis for how we are able to access the 8 bytes
// of CAN data. The data may be accessed as a combination of 8-bit, 16-bit,
// and 32-bit values. All data is stored in little endian format.
//
//*****************************************************************************
typedef struct {        // Bits  Little Endian Description
    uint32_t BIT_0:1;   // 0     bit 0
    uint32_t BIT_1:1;   // 0     bit 1
    uint32_t BIT_2:1;   // 0     bit 2
    uint32_t BIT_3:1;   // 0     bit 3
    uint32_t BIT_4:1;   // 0     bit 4
    uint32_t BIT_5:1;   // 0     bit 5
    uint32_t BIT_6:1;   // 0     bit 6
    uint32_t BIT_7:1;   // 0     bit 7
    uint32_t BIT_8:1;   // 0     bit 8
    uint32_t BIT_9:1;   // 0     bit 9
    uint32_t BIT_10:1;  // 0     bit 10
    uint32_t BIT_11:1;  // 0     bit 11
    uint32_t BIT_12:1;  // 0     bit 12
    uint32_t BIT_13:1;  // 0     bit 13
    uint32_t BIT_14:1;  // 0     bit 14
    uint32_t BIT_15:1;  // 0     bit 15
    uint32_t BIT_16:1;  // 0     bit 16
    uint32_t BIT_17:1;  // 0     bit 17
    uint32_t BIT_18:1;  // 0     bit 18
    uint32_t BIT_19:1;  // 0     bit 19
    uint32_t BIT_20:1;  // 0     bit 20
    uint32_t BIT_21:1;  // 0     bit 21
    uint32_t BIT_22:1;  // 0     bit 22
    uint32_t BIT_23:1;  // 0     bit 23
    uint32_t BIT_24:1;  // 0     bit 24
    uint32_t BIT_25:1;  // 0     bit 25
    uint32_t BIT_26:1;  // 0     bit 26
    uint32_t BIT_27:1;  // 0     bit 27
    uint32_t BIT_28:1;  // 0     bit 28
    uint32_t BIT_29:1;  // 0     bit 29
    uint32_t BIT_30:1;  // 0     bit 30
    uint32_t BIT_31:1;  // 0     bit 31
} can_word_bits_t;

typedef struct {        // Bits   Little Endian Description
    uint16_t BYTE0:8;   // 7:0    byte 0
    uint16_t BYTE1:8;   // 15:8   byte 1
    uint16_t BYTE2:8;   // 23:16  byte 2
    uint16_t BYTE3:8;   // 31:24  byte 3
} can_word_bytes_t;


typedef struct {        // Bits   Little Endian Description
    uint16_t HALF0;     // 15:0   uint16_t 0
    uint16_t HALF1;     // 32:16  uint16_t 1
} can_word_halves_t;

//
// Each can_word_t (4 bytes) may be accessed as one of the following.
//
typedef union {
    float             bit32float;
    uint32_t          bit32;
    can_word_halves_t bit16;
    can_word_bytes_t  bit8;
    can_word_bits_t   bit1;
} can_word_t;

typedef struct {
    can_word_t MDL;      // Least significant 32-bits
    can_word_t MDH;      // Most significant 32-bits
} can_data_t;

typedef struct can_tx_registration_s
{
    uint8_t module_id;
    // One of three CAN interfaces
    CANLINE_ can_line;
    // A counter that is incremented when any of a device's CAN
    // transmit methods are called. This will occur at the rate that
    // User_App() is called by the underlying HED system services. The
    // specific message registered will be transmitted only when this
    // counter exceeds the transmit_counter_limit.
    uint16_t transmit_counter;
    // See comment above. This value is set for each particular CAN
    // transmit message, with the transmit_counter_limit argument, to
    // any of the device's CAN transmit methods.
    can_rate_t transmit_counter_limit;
    bool_t transmit_message_sent_first_time;
    // A NORMAL or J1939 message
    uint16_t j1939_word;
    // A NORMAL or J1939 message
    uint8_t j1939_byte;
    // The device_index is zero-indexed. Use the enums ONE, TWO,
    // THREE, ... which equate to 0, 1, 2, ...
    device_instances_t device;
    Can_Message_ tx_message;
} can_tx_registration_t;


//
// Typedef of function prototype representing the function called by
// fvt_can_process_rx_message(). A pointer to this function is stored
// in a linked list of the can_registration_record_t structures. There
// are one for each registered interest in a CAN ID.
//
typedef void (*p_rx_f_t)(device_instances_t, can_data_t *, int16_t *);
typedef void (*p_rx_not_ok)(device_instances_t, uint8_t, CANLINE_, uint32_t, uint8_t);

//
// Function prototypes visible only to device drivers.
//
bool_t fvt_can_register_receive_id(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    p_rx_f_t handler_function_p,
    p_rx_not_ok timeout_function_p);

bool_t fvt_can_register_receive_id_j1939_byte(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte,
    p_rx_f_t handler_function_p,
    p_rx_not_ok timeout_function_p);

can_tx_registration_t *
fvt_can_register_transmit_id(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    IDENTIFIER_TYPE_ type);

can_tx_registration_t *
fvt_can_register_transmit_id_j1939(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    IDENTIFIER_TYPE_ type,
    uint16_t j1939_word);

can_tx_registration_t *
fvt_can_register_transmit_id_j1939_byte(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    IDENTIFIER_TYPE_ type,
    uint8_t j1939_byte);

void rx_message_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);


#endif // CAN_SERVICE_H_
