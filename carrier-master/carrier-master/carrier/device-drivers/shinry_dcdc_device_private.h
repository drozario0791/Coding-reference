/******************************************************************************
 *
 *        Name:shinry_dcdc_device_private.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */

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
// The following CAN message control the Shinry DC-DC converter.
// CAN ID: 0x10262B2
// Format: Extended
// Reference: Shinry CAN Communication Matrix
//

typedef struct
{
    ////////////////////////////
    // WORD 1 -> BYTE0 & BYTE1
    ////////////////////////////
    uint32_t unused1: 16;

    ////////////////////////////
    // WORD2 -> BYTE1 & BYTE3
    ////////////////////////////
    uint32_t unused2: 16;

    ////////////////////////////
    // WORD3 -> BYTE4 & BYTE5
    ////////////////////////////
    uint32_t unused3: 16;

    ////////////////////////////
    // BYTE 6
    ////////////////////////////
    uint32_t dcdc_enable: 8;

    ////////////////////////////
    // BYTE 7
    ////////////////////////////
    uint32_t vcu_dcdc_voltage_req: 8;


} shinry_dcdc_tx_control_t;

typedef struct
{
    ////////////////////////////
    // BYTE 0
    ////////////////////////////
    // Description: Operation Mode
    // Startbit: 6
    // Length: 2
    uint32_t operation_mode: 2;

    // Description: Reserved
    // Startbit: 5
    // Length: 1
    uint32_t rsvd2: 1;

    // Description: can_cmd_lost
    // Startbit: 4
    // Length: 1
    uint32_t can_cmd_lost: 1;

    // Description: input_over_voltage
    // Startbit: 3
    // Length: 1
    uint32_t input_over_voltage: 1;

    // Description: input_over_voltage
    // Startbit: 2
    // Length: 1
    uint32_t input_under_voltage: 1;

    // Description: input_under_voltage
    // Startbit: 1
    // Length: 1
    uint32_t output_over_voltage: 1;

    // Description: output_over_voltage
    // Startbit: 0
    // Length: 1
    uint32_t output_under_voltage: 1;

    ////////////////////////////
    // BYTE 1 & BYTE 2
    ////////////////////////////
    // Description: output_current
    // Startbit: 12
    // Length: 12
    uint32_t instantaneous_current_voltage: 12;

    // Description: Over current fault
    // Startbit: 11
    // Length: 1
    uint32_t over_current_fault: 1;

    // Description: Over temperature fault
    // Startbit: 10
    // Length: 1
    uint32_t over_temperature_fault: 1;

    // Description: Reserved
    // Startbit: 9
    // Length: 1
    uint32_t rsvd1: 1;

    // Description: DC-DC derating
    // Startbit: 8
    // Length: 1
    uint32_t derating: 1;

    ////////////////////////////
    // BYTE 3
    ////////////////////////////
    // Description: temperature
    // Startbit: 24
    // Length: 8
    uint32_t instantaneous_temperature: 8;

    ////////////////////////////
    // BYTE 4 & Half of 5
    ////////////////////////////
    // Description: output_voltage
    // Startbit: 32
    // Length: 12
    uint32_t instantaneous_output_voltage: 12;

    ////////////////////////////
    // Half of 5 & BYTE 6
    ////////////////////////////
    // Description: input_voltage
    // Start bit: 44
    // Length: 12
    uint32_t instantaneous_input_voltage: 12;

    ////////////////////////////
    // BYTE 7
    ////////////////////////////
    // Description: version_number
    // Startbit : 56
    // length: 8
    uint32_t software_version: 8;

} shinry_dcdc_rx_status_t;

// DC-DC Master Structure
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;

    // These are rx message data structures
    shinry_dcdc_rx_status_t status;

    // Each of the Shinry CAN receive functions sets bool associated
    // with itself to TRUE when its message is received.
    bool_t status_rx_ok;

    // These are tx message data buffers
    shinry_dcdc_tx_control_t control;

    // These are pointers to CAN tx registration records
	can_tx_registration_t *send_dcdc_control_ptr;

    // power pin
    uint8_t dcdc_power_pin;
} device_data_t;

#include "shinry_dcdc_device.h"

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

//#endif // SKAI2_INVERTER_PRIVATE_H_
