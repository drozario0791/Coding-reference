/******************************************************************************
 *
 *        Name: orion_device_private.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Tuesday, 22 July 2019
 *
 ******************************************************************************
 */

#ifndef ORION_DEVICE_PRIVATE_H_
#define ORION_DEVICE_PRIVATE_H_

#include <string.h>
#include "can_service_devices.h"
#include "timer_service.h"
#include "orion_device.h"


//////////////////////////////////////////////////////////////////////////////
//
// NOTE: There is a required #include file and two function prototypes
// at the end of this head file. This is required for all device
// drivers.
//
//////////////////////////////////////////////////////////////////////////////


//
// Refer to CSV File
//
// Can ID: 0x00NB5001
//
typedef struct bms_pack_data1_s
{
	uint32_t instantaneous_pack_current: 16;
	uint32_t instantaneous_pack_voltage: 16;
	uint32_t pack_high_cell_voltage: 16;
	uint32_t pack_low_cell_voltage: 16;
} bms_pack_data1_t;

//
// Can ID: 0x00NB5002
//
typedef struct bms_pack_data2_s
{
	uint32_t pack_discharge_current_limit: 16;
	uint32_t pack_charge_current_limit: 16;
	uint32_t pack_high_temperature: 8;
	uint32_t pack_low_temperature: 8;
	uint32_t pack_average_temperature: 8;
	uint32_t internal_temperature: 8;
} bms_pack_data2_t;

//
// Can ID: 0x00NB003
//
typedef struct bms_pack_data3_s
{
	uint32_t unused: 8;
	uint32_t state_of_charge: 8;
	uint32_t pack_total_resiatance: 16;
	uint32_t pack_open_voltage: 16;
	uint32_t pack_amp_hours: 16;
} bms_pack_data3_t;

//
// Can ID: 0x00NB004
// Consists of custom flags, which is used to protect
// the battery pack and BMS.
//
typedef struct bms_pack_data4_s
{
	// Custom Flags 0
	// From MSB to LSB
	uint32_t unused_1: 1;
	uint32_t multipurpose_input_state: 1;
	uint32_t power_supply_failure: 1;
	uint32_t current_fail_safe: 1;
	uint32_t voltage_fail_safe: 1;
	uint32_t charger_safety: 1;
	uint32_t charge_relay: 1;
	uint32_t discharge_relay: 1;

	// Custom Flags 1
	// From MSB to LSB
	uint32_t unused_3: 1;
	uint32_t voltage_redundancy_fault: 1;
	uint32_t current_sensor_fault: 1;
	uint32_t open_cell_voltage_fault: 1;
	uint32_t low_cell_voltage_fault: 1;
	uint32_t weak_cell_fault: 1;
	uint32_t internal_conversion_fault: 1;
	uint32_t internal_communication_fault: 1;

	// Custom Flags 2
	// From MSB to LSB
	uint32_t charger_enable_relay_fault: 1;
	uint32_t power_supply_fault: 1;
	uint32_t high_voltage_isolation_fault: 1;
	uint32_t always_on_power_supply_fault: 1;
	uint32_t communication_fault: 1;
	uint32_t thermistor_fault: 1;
	uint32_t fan_monitor_fault: 1;
	uint32_t weak_pack_fault: 1;

	// Custom Flags 3
	// From MSB to LSB
	uint32_t unused_7: 1;
	uint32_t unused_6: 1;
	uint32_t unused_5: 1;
	uint32_t internal_logic_fault: 1;
	uint32_t internal_thermistor_fault: 1;
	uint32_t internal_memory_fault: 1;
	uint32_t charger_safety_relay_fault: 1;
	uint32_t discharge_enable_relay_fault: 1;

	// Pack Health
	uint32_t pack_health: 8;

	// Current Limit Status

	// MSB to LSB
	// Byte Swapped
	uint32_t charge_current_limit_alternate_current_limit: 1;
	uint32_t charger_latch: 1;
	uint32_t charge_current_limit_high_pack_voltage: 1;
	uint32_t charge_current_limit_high_cell_voltage: 1;

	uint32_t charge_current_limit_temperature: 1;
	uint32_t charge_current_limit_high_cell_resistance: 1;
	uint32_t charge_current_limit_high_soc: 1;
	uint32_t unused_9:1;


	// MSB to LSB

	uint32_t discharge_current_limit_communication_failsafe: 1;
	uint32_t discharge_current_limit_voltage_failsafe: 1;
	uint32_t unused_8:1;
	uint32_t discharge_current_limit_low_pack_voltage: 1;

	uint32_t discharge_current_limit_low_cell_voltage: 1;
	uint32_t discharge_current_limit_temperature: 1;
	uint32_t discharge_current_limit_high_cell_resistance: 1;
	uint32_t discharge_current_limit_low_soc: 1;


	// pack relay status
	uint32_t pack_relay_status: 8;
} bms_pack_data4_t;

// Can ID: 0x00NB005
// Motec Logging
typedef struct bms_pack_data5_s
{
	uint32_t pack_high_temperature_ID: 8;
	uint32_t pack_low_temperature_ID: 8;
	uint32_t pack_high_cell_voltage_ID: 8;
	uint32_t pack_low_cell_voltage_ID: 8;
	uint32_t pack_high_internal_resistance_ID: 8;
	uint32_t pack_low_internal_resistance_ID: 8;
	uint32_t pack_high_open_cell_voltage_ID: 8;
	uint32_t pack_low_open_cell_voltage_ID: 8;
} bms_pack_data5_t;

// Can ID: 0x00NB006
typedef struct bms_pack_data6_s
{
	uint32_t average_cell_voltage: 16;
	uint32_t high_cell_resistance: 16;
	uint32_t low_cell_resistance: 16;
	uint32_t average_cell_resistance: 16;
} bms_pack_data6_t;

// Can ID: 0x00NB007
typedef struct bms_pack_data7_s
{
	uint32_t high_open_cell_voltage: 16;
	uint32_t low_open_cell_voltage: 16;
	uint32_t low_power_supply_indicator: 16;
	uint32_t maximum_number_of_cells: 8;
	uint32_t number_of_populated_cells: 8;
} bms_pack_data7_t;

// Can ID: 0x00NB008
/*
 * These are raw values from the isolation sensor.
 * They should not be used for control, but rather for debugging / reference only.
 * See the Orion BMS documentation for more details
 */
typedef struct bms_pack_data8_s
{
	uint32_t pack_isolation_adc: 16;
	uint32_t pack_shortest_wave: 16;
	uint32_t pack_isolation_clipping: 16;
	uint32_t pack_isolation_threshold: 16;
} bms_pack_data8_t;

// Can ID: 0x00NB009
typedef struct bms_pack_data9_s
{
	uint32_t pack_total_cycles: 16;
	uint32_t rolling_counter: 8;
	uint32_t unused1: 8;
	uint32_t unused2: 16;
	uint32_t unused3: 16;
} bms_pack_data9_t;


// A compound structure that encompasses all the RX CAN messages
// related to the Murphy PDM. One of these is allocated for each
// instantiation of a PDM.
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;

	// These are the receive CAN message data structures.
	bms_pack_data1_t bms_data1;
	bms_pack_data2_t bms_data2;
	bms_pack_data3_t bms_data3;
	bms_pack_data4_t custom_faults;
	bms_pack_data5_t bms_data5;
	bms_pack_data6_t bms_data6;
	bms_pack_data7_t bms_data7;
	bms_pack_data8_t bms_data8;
	bms_pack_data9_t bms_data9;

    //
    // Each of the BMS CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    //
	bool_t bms_data1_rx_ok;
	bool_t bms_data2_rx_ok;
	bool_t bms_data3_rx_ok;
	bool_t custom_faults_rx_ok;
	bool_t bms_data5_rx_ok;
	bool_t bms_data6_rx_ok;
	bool_t bms_data7_rx_ok;
	bool_t bms_data8_rx_ok;
	bool_t bms_data9_rx_ok;

} device_data_t;

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
