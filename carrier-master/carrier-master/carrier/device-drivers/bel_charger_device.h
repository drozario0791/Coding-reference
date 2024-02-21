/******************************************************************************
 *
 *        Name: bel_charger_device.h
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef BEL_CHARGER_DEVICE_H_
#define BEL_CHARGER_DEVICE_H_

//=============================================================================
//
// Public initialization function for Bel Charger.
//
//=============================================================================
//
void bel_charger_init(device_instances_t device, uint8_t module_id, CANLINE_ can_line);


//=============================================================================
//
// These functions transmit the registered Bel Charger CAN
// messages. Use the setter functions to populate the structure data
// that each one of these refers to.
//
//=============================================================================
//
void bel_tx_can_setpoint(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void bel_tx_can_gpio(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void bel_tx_can_battery_under_over_voltage_limits(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void bel_tx_can_led_duty_cycle(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

//=============================================================================
//
// Public functions to set the timeout counter limit for each of the
// PDM receive CAN messages.
//
//=============================================================================
//
void bel_set_timeout_status(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void bel_set_timeout_measured_values(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void bel_set_timeout_primary_values(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void bel_set_timeout_secondary_values(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);


//=============================================================================
//
// Getters for Bel Charger status.
//
//=============================================================================
//
bool_t bel_get_input_voltage_ok(device_instances_t device);
bool_t bel_get_input_frequency_ok(device_instances_t device);
bool_t bel_get_hv_battery_undervoltage(device_instances_t device);
bool_t bel_get_hv_battery_overvoltage(device_instances_t device);
bool_t bel_get_hv_battery_voltage_in_range(device_instances_t device);
bool_t bel_get_output_overcurrent(device_instances_t device);
bool_t bel_get_unit_overtemperature(device_instances_t device);
bool_t bel_get_i2c_fault(device_instances_t device);
bool_t bel_get_can_bus_fault(device_instances_t device);
bool_t bel_get_eeprom_memory_fault(device_instances_t device);
bool_t bel_get_unit_thermistor_fault(device_instances_t device);
bool_t bel_get_lv_battery_in_range(device_instances_t device);
bool_t bel_get_unit_enabled(device_instances_t device);
bool_t bel_get_key_switch_voltage_in_range(device_instances_t device);
bool_t bel_get_output_off_fault_bit_set(device_instances_t device);
bool_t bel_get_converter_latched_off_due_to_fault(device_instances_t device);
bool_t bel_get_converter_commanded_off(device_instances_t device);
bool_t bel_get_start_up_init_done_self_test_passed(device_instances_t device);
bool_t bel_get_output_off_control_dsp_module_failure(device_instances_t device);
bool_t bel_get_control_dsp_in_bootloader(device_instances_t device);
bool_t bel_get_proprietary_a(device_instances_t device);
bool_t bel_get_signal_dsp_uploading_firmware_to_control_dsp(device_instances_t device);
bool_t bel_get_gpi_signal_on_external_signal_conenctor(device_instances_t device);
bool_t bel_get_wake_up_power_switch_failure(device_instances_t device);
bool_t bel_get_gpo_up_power_switch_failure(device_instances_t device);
bool_t bel_get_hvil_loop_disconnected(device_instances_t device);
bool_t bel_get_buck_undervoltage(device_instances_t device);
bool_t bel_get_proximity_and_pilot_enabled(device_instances_t device);
bool_t bel_get_pilot_signal_ok(device_instances_t device);
bool_t bel_get_proximity_signal_ok(device_instances_t device);
uint8_t bel_get_chg_state(device_instances_t device);
bool_t bel_get_can_rx_ok(device_instances_t device);


//=============================================================================
//
// Getters for Bel Charger measured_values.
//
//=============================================================================
//
uint16_t bel_get_pin_17_vtd_v_20(device_instances_t device);
uint16_t bel_get_max_charging_current_ava(device_instances_t device);
uint16_t bel_get_buck_current_a_20(device_instances_t device);
uint8_t  bel_get_pilot_duty_cycle_percent(device_instances_t device);


//=============================================================================
//
// Getters for Bel Charger primary_values.
//
//=============================================================================
//
uint16_t bel_get_input_voltage_rms_phase_1_2_v(device_instances_t device);
uint16_t bel_get_input_voltage_rms_phase_2_3_v(device_instances_t device);
uint16_t bel_get_input_voltage_rms_phase_3_1_v(device_instances_t device);
uint16_t bel_get_input_voltage_frequency_hz_20(device_instances_t device);


//=============================================================================
//
// Getters for Bel Charger secondary_values.
//
//=============================================================================
//
uint16_t bel_get_output_voltage_v_20(device_instances_t device);
uint16_t bel_get_output_current_a_20(device_instances_t device);
int8_t bel_get_chassis_temperature_c(device_instances_t device);
uint16_t bel_get_aux_battery_voltage_v_20(device_instances_t device);
int8_t bel_get_internal_ambient_temperature_c(device_instances_t device);


//=============================================================================
//
// Setters for Bel Charger setpoints, gpios, and limits.
//
//=============================================================================
//
void bel_set_setpoint(
    device_instances_t device,
    uint16_t current_command,
    uint16_t battery_voltage_limit,
    uint8_t charger_enable);

void bel_set_gpio(
    device_instances_t device,
    bool_t wake_up_ecu_enable,
    bool_t gpo_enable);

void bel_set_battery_under_over_voltage_limits(
    device_instances_t device,
    uint16_t battery_undervoltage_limit,
    uint16_t battery_overvoltage_limit);

void bel_set_led_duty_cycle(
    device_instances_t device,
    uint8_t led_duty_cycle);


#endif
