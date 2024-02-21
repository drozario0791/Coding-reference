/******************************************************************************
 *
 *        Name: device_name_device_private.h
 *
 * Description: This is the private header file for the device_name
 *              device driver.
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef PKU2400_DEVICE_PRIVATE_H_
#define PKU2400_DEVICE_PRIVATE_H_

#include <string.h>
#include "can_service_devices.h"
#include "timer_service.h"

//////////////////////////////////////////////////////////////////////////////
//
// NOTE: There is a required #include file and two function prototypes
// at the end of this head file. This is required for all device
// drivers.
//
//////////////////////////////////////////////////////////////////////////////

//
// CAN ID: 0x18EFFFX0
// Type: Receive Message
//
typedef struct can_switches_pku2400_rx_s{
    uint32_t bottom_switch_4: 1;
    uint32_t bottom_switch_3: 1;
    uint32_t bottom_switch_2: 1;
    uint32_t bottom_switch_1: 1;

    uint32_t top_switch_4: 1;
    uint32_t top_switch_3: 1;
    uint32_t top_switch_2: 1;
    uint32_t top_switch_1: 1;

    uint32_t unused_1: 8;
    uint32_t unused_2: 16;

    uint32_t unused_3: 16;
    uint32_t unused_4: 16;
} can_switches_pku2400_rx_status_t;

//
// CAN ID: 0x18EFFFX1
// Type: Transmit Message
//
typedef struct can_switches_pku2400_tx_s{
    uint32_t green_led_top_switch_4: 1;
    uint32_t red_led_top_switch_4: 1;

    uint32_t green_led_top_switch_3: 1;
    uint32_t red_led_top_switch_3: 1;

    uint32_t green_led_top_switch_2: 1;
    uint32_t red_led_top_switch_2: 1;

    uint32_t green_led_top_switch_1: 1;
    uint32_t red_led_top_switch_1: 1;

    uint32_t green_led_bottom_switch_4: 1;
    uint32_t red_led_bottom_switch_4: 1;

    uint32_t green_led_bottom_switch_3: 1;
    uint32_t red_led_bottom_switch_3: 1;

    uint32_t green_led_bottom_switch_2: 1;
    uint32_t red_led_bottom_switch_2: 1;

    uint32_t green_led_bottom_switch_1: 1;
    uint32_t red_led_bottom_switch_1: 1;

    uint32_t blue_led_bottom_switch_4: 1;
    uint32_t blue_led_bottom_switch_3: 1;
    uint32_t blue_led_bottom_switch_2: 1;
    uint32_t blue_led_bottom_switch_1: 1;

    uint32_t blue_led_top_switch_4: 1;
    uint32_t blue_led_top_switch_3: 1;
    uint32_t blue_led_top_switch_2: 1;
    uint32_t blue_led_top_switch_1: 1;


    uint32_t unused1: 8;
    uint32_t unused2: 32;

} can_switches_pku2400_tx_led_control_t;


typedef struct switch_states_s
{
    bool_t momentary_input_switch_state;
    bool_t output_toggle_switch_state;
    bool_t toggled;
    bool_t previous_state;
} switch_states_t;

//
// A compound structure that encompasses all the RX CAN messages
// related to the Bel charger. One of these is allocated for each
// instantiation of a Bel Charger.
//
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;

    // These are rx message data buffers
    can_switches_pku2400_rx_status_t can_switch_status;

    // Each of the PDM CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    bool_t can_switch_status_rx_ok;

    // These are tx message data buffers
    can_switches_pku2400_tx_led_control_t can_switch_led_control;

    // These are pointers to CAN tx registration records
    can_tx_registration_t *send_can_switch_led_control_ptr;

    switch_states_t switch_states[8];

} device_data_t;


//////////////////////////////////////////////////////////////////////////////
//
// Note: These two include files must be located here. This is the
// structure we are following with the idea that all of these device
// drivers will be built into a libray.
//
//////////////////////////////////////////////////////////////////////////////
//
#include "pku2400_device.h"
#include "Prototypes_Time.h"

//////////////////////////////////////////////////////////////////////////////
//
// Note: These two function prototypes of functions defined in
// can_service.c are required here at this point in each device's
// private header file.
//
//////////////////////////////////////////////////////////////////////////////
//
device_data_t *
create_device_linked_data_record(
    device_instances_t device_instance,
    device_data_t **first_device_data_ptr,
    uint8_t *device_instance_counter,
    uint32_t sizeof_device_data_t);


device_data_t *
get_device_linked_data_record_instance_ptr(
    device_instances_t device,
    device_data_t *first_device_data_ptr);


#endif
