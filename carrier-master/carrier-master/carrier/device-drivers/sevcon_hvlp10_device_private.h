/******************************************************************************
 *
 *        Name: sevcon_hvlp10_device_private.h
 *
 * Description: This is the private header file for the sevcon_hvlp10
 *              device driver.
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef SEVCON_HVLP10_DEVICE_PRIVATE_H_
#define SEVCON_HVLP10_DEVICE_PRIVATE_H_

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

// Receieve CAN Messages
//
// Description: Transmits the most important paraments of the sevcon
// inverter
// Message CAN ID: 0x385
//
typedef struct{
    uint32_t sevcon_dc_link_voltage: 16;
    uint32_t sevcon_heatsink_temperature: 16;
    uint32_t sevcon_current: 16;
    uint32_t stepper_motor_temperature: 16;
} sevcon_hvlp10_rx_parameters_t;

typedef struct{
    uint32_t motor_rpm: 16;
    uint32_t unused1: 16;
    uint32_t unused2: 16;
    uint32_t unused3: 16;
} sevcon_hvlp10_rx_motor_rpm_t;


// Transmit CAN Messages
//
// Description
// Message:
//
typedef struct {
    uint32_t set_output_torque: 16;
    uint32_t sevcon_inverter_enable: 16;
    uint32_t set_torque_drive_limit: 16;
    uint32_t sequence_check: 8;
    uint32_t checksum: 8;
} sevcon_hvlp10_tx_data1_t;

typedef struct {
    uint32_t set_torque_regen_limit: 16;
    uint32_t set_forward_speed_limit: 16;
    uint32_t set_reverse_speed_limit: 16;
    uint32_t sequence_check: 8;
    uint32_t checksum: 8;
} sevcon_hvlp10_tx_data2_t;

typedef struct {
    uint32_t set_battery_discharge_limit: 16;
    uint32_t set_battery_recharge_limit: 16;
    uint32_t set_voltage_capacitor_limit: 16;
    uint32_t sequence_check: 8;
    uint32_t checksum: 8;
} sevcon_hvlp10_tx_data3_t;

//
// This is a heartbeat message that must be sent.
//
typedef struct {
    uint32_t unused1: 32;
    uint32_t umused2: 32;
} sevcon_hvlp10_tx_data4_t;

//
// A compound structure that encompasses all the RX CAN messages
// related to the device. One of these is allocated for each
// instantiation of a device.
//
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;

    // These are rx message data buffers
    sevcon_hvlp10_rx_parameters_t parameters;
    sevcon_hvlp10_rx_motor_rpm_t motor_rpm;

    // Each of the SEVCON CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    bool_t parameters_rx_ok;

    // These are tx message data buffers
    sevcon_hvlp10_tx_data1_t data1;
    sevcon_hvlp10_tx_data2_t data2;
    sevcon_hvlp10_tx_data3_t data3;
    sevcon_hvlp10_tx_data4_t data4;

    // These are pointers to CAN tx registration records
    can_tx_registration_t *send_data1_can_ptr;
    can_tx_registration_t *send_data2_can_ptr;
    can_tx_registration_t *send_data3_can_ptr;
    can_tx_registration_t *send_data4_can_ptr;

    uint32_t enable_pin;

} device_data_t;

//////////////////////////////////////////////////////////////////////////////
//
// Note: These two include files must be located here. This is the
// structure we are following with the idea that all of these device
// drivers will be built into a libray.
//
//////////////////////////////////////////////////////////////////////////////
//
#include "sevcon_hvlp10_device.h"
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


