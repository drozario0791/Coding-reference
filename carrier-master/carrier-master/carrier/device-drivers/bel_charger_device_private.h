/******************************************************************************
 *
 *        Name: bel_charger_device_private.h
 *
 * Description: This is the private header file for the bel charger
 *              device driver.
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef BEL_CHARGER_DEVICE_PRIVATE_H_
#define BEL_CHARGER_DEVICE_PRIVATE_H_

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


typedef enum
{
    INITIALIZATION_SEQUENCE         = 0,
    IDLE                            = 1,
    START_UP                        = 2,
    PRERUNNING_CHECK                = 3,
    RUNNING                         = 4,
    FAULT                           = 5,
    OVER_TEMPERATURE_PROTECT        = 6,
    LATCHED_FAULT                   = 7,
    SOFT_FAULT                      = 8,
    OUTPUT_UNDER_VOLTAGE_PROTECT    = 9,
    OUTPUT_OVER_VOLTAGE_PROTECT     = 10,
    UPLOADING                       = 11
} bel_charger_state_t;


//
// Charger Status: (Page 14 - Bel Charger Manual):
// Message: PGN 0x00FD4XX
//
typedef struct bel_charger_status_s{
    uint32_t i2c_fault : 1;
    uint32_t unit_overtemperature : 1;
    uint32_t output_overcurrent : 1;
    uint32_t hv_battery_voltage_in_range : 1;
    uint32_t hv_battery_overvoltage : 1;
    uint32_t hv_battery_undervoltage : 1;
    uint32_t input_frequency_ok : 1;
    uint32_t input_voltage_ok : 1;

    uint32_t converter_latched_off_due_to_fault : 1;
    uint32_t output_off_fault_bit_set : 1;
    uint32_t key_switch_voltage_in_range : 1;
    uint32_t unit_enabled : 1;
    uint32_t lv_battery_in_range : 1;
    uint32_t unit_thermistor_fault : 1;
    uint32_t eeprom_memory_fault : 1;
    uint32_t can_bus_fault : 1;

    uint32_t signal_dsp_uploading_firmware_to_control_dsp : 1;
    uint32_t proprietary_a : 3;
    uint32_t control_dsp_in_bootloader : 1;
    uint32_t output_off_control_dsp_module_failure : 1;
    uint32_t start_up_init_done_self_test_passed : 1;
    uint32_t converter_commanded_off : 1;

    uint32_t proximity_signal_ok : 1;
    uint32_t pilot_signal_ok : 1;
    uint32_t proximity_and_pilot_enabled : 1;
    uint32_t buck_undervoltage : 1;
    uint32_t hvil_loop_disconnected : 1;
    uint32_t gpo_up_power_switch_failure : 1;
    uint32_t wake_up_power_switch_failure : 1;
    uint32_t gpi_signal_on_external_signal_conenctor : 1;

    uint32_t not_used : 16;
    uint32_t proprietary_b : 8;
    uint32_t chg_state : 8;
} bel_charger_status_t;


//
// Name: Measured Values (Page 13 - Bel Charger Manual):
// Message: PGN 0x00FD6XX
//
typedef struct {
    uint32_t pin_17_vtd_v_20 : 16;
    uint32_t max_charging_current_ava : 16;

    uint32_t buck_current_a_20 : 16;
    uint32_t pilot_duty_cycle_percent : 8;
    uint32_t not_used : 8;
} bel_charger_measured_values_t;

//
// Name: Primary Values (Page 12 - Bel Charger Manual):
// Message: PGN 0x00FD6XX
//
typedef struct {
    uint32_t input_voltage_rms_phase_1_2_v : 16;
    uint32_t input_voltage_rms_phase_2_3_v : 16;

    uint32_t input_voltage_rms_phase_3_1_v : 16;
    uint32_t input_voltage_frequency_hz_20 : 16;
} bel_charger_primary_values_t;

//
// Name: Secondary Values (Page 13 - Bel Charger Manual):
// Message: PGN 0x00FD7XX
//
typedef struct {
    uint32_t output_voltage_v_20 : 16;
    uint32_t output_current_a_20 : 16;

    uint32_t chassis_temperature_c : 8;
    uint32_t aux_battery_voltage_v_20 : 16;
    uint32_t internal_ambient_temperature_c : 8;
} bel_charger_secondary_values_t;


typedef struct {
    uint32_t j1939_word : 16;
    uint32_t current_command : 16;

    uint32_t battery_voltage_limit : 16;
    uint32_t charger_enable : 8;
    uint32_t unused_7 : 8;
} bel_charger_tx_setpoint_t;


typedef struct {
    uint32_t j1939_word : 16;
    uint32_t wake_up_ecu_en : 1;
    uint32_t gpo_en : 1;
    uint32_t unused_bits : 14;

    uint32_t unused_32bits : 32;
} bel_charger_tx_gpio_t;


typedef struct {
    uint32_t j1939_word : 16;
    uint32_t led_duty : 8;
    uint32_t unused_3 : 8;

    uint32_t unused_4 : 8;
    uint32_t unused_5 : 8;
    uint32_t unused_6 : 8;
    uint32_t unused_7 : 8;
} bel_charger_tx_led_t;


typedef struct {
    uint32_t j1939_word : 16;
    uint32_t battery_overvoltage_limit : 16;

    uint32_t battery_undervoltage_limit : 16;
    uint32_t unused_6 : 8;
    uint32_t unused_7 : 8;
} bel_charger_tx_battery_under_over_voltage_limits_t;


//
// A compound structure that encompasses all the RX CAN messages
// related to the Bel charger. One of these is allocated for each
// instantiation of a Bel Charger.
//
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;

    // These are rx message data buffers
    bel_charger_status_t status;
    bel_charger_measured_values_t measured_values;
    bel_charger_primary_values_t primary_values;
    bel_charger_secondary_values_t secondary_values;

    // Each of the PDM CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    bool_t status_rx_ok;
    bool_t measured_values_rx_ok;
    bool_t primary_values_rx_ok;
    bool_t secondary_values_rx_ok;

    // These are tx message data buffers
    bel_charger_tx_setpoint_t setpoints;
    bel_charger_tx_gpio_t gpios;
    bel_charger_tx_led_t leds;
    bel_charger_tx_battery_under_over_voltage_limits_t limits;

    // These are pointers to CAN tx registration records
    can_tx_registration_t *send_setpoint_j1939_can_ptr;
    can_tx_registration_t *send_gpio_setting_j1939_can_ptr;
    can_tx_registration_t *send_led_setting_j1939_can_ptr;
    can_tx_registration_t *send_battery_voltage_limits_j1939_can_ptr;

} device_data_t;


//////////////////////////////////////////////////////////////////////////////
//
// Note: These two include files must be located here. This is the
// structure we are following with the idea that all of these device
// drivers will be built into a libray.
//
//////////////////////////////////////////////////////////////////////////////
//
#include "bel_charger_device.h"
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


