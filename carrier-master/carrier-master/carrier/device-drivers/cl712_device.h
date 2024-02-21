/******************************************************************************
 *
 *        Name: device_name_device.h
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef CL712_DEVICE_H_
#define CL712_DEVICE_H_

#include "can_service.h"

//=============================================================================
//
// Public initialization function for Bel Charger.
//
//=============================================================================
//
void cl712_init(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line);


//=============================================================================
//
// These functions transmit the registered Bel Charger CAN
// messages. Use the setter functions to populate the structure data
// that each one of these refers to.
//
//=============================================================================
//
void cl712_tx_can_message1(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message2(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message3(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message4(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message5(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message6(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message7(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message8(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message9(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message10(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message11(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message12(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message13(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message14(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message15(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message16(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message17(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message18(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message19(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message20(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message21(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message22(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message23(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message24(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message25(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message26(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message27(
    device_instances_t device,
    can_rate_t transmit_counter_limit);

void cl712_tx_can_message28(
    device_instances_t device,
    can_rate_t transmit_counter_limit);


//=============================================================================
//
// Public functions to set the timeout counter limit for each of the
// PDM receive CAN messages.
//
//=============================================================================
//


//=============================================================================
//
// Getters for device_name NAME.
//
//=============================================================================
//
bool_t get_hydraulic_motor_pump_override(device_instances_t device);
bool_t get_traction_motor_pump_override(device_instances_t device);
bool_t pe_pump_override(device_instances_t device);
bool_t charger_pump_override(device_instances_t device);
bool_t charger_fan_override(device_instances_t device);
bool_t pe_fan_override(device_instances_t device);

//=============================================================================
//
// Setters for Bel Charger setpoints, gpios, and limits.
//
//=============================================================================
//

//=============================================================================
//
// MAIN SCREEN MESSAGES
//
//=============================================================================
//
void cl712_set_tx_main_screen_message1(
	device_instances_t device,
	uint32_t odometer_m,
	uint32_t hourmeter_s);

void cl712_set_tx_main_screen_message2(
	device_instances_t device,
    uint32_t time_until_service_s,
    uint16_t groundspeed_cmps,
    uint8_t soc_percent,
    uint8_t gear_direction);

void cl712_set_tx_main_screen_message3(
	device_instances_t device,
	uint8_t gear_ratio,
	uint8_t vehicle_state,
	uint8_t misc_status,
	uint8_t can1_rx_4_spare_1,
    uint32_t yellow_warning_reason);

void cl712_set_tx_main_screen_message4(
	device_instances_t device,
	uint32_t crit_fail_hv_off_reason,
	uint32_t crit_fail_hv_on_reason);

//=============================================================================
//
// BMS SCREEN MESSAGE
//
//=============================================================================
//
void cl712_set_tx_bms_screen_message1(
    device_instances_t device,
    uint16_t battery_1_high_cell_voltage_mv,
    uint16_t battery_1_low_cell_voltage_mv,
    uint16_t battery_1_cell_voltage_delta_mv,
    uint16_t battery_1_pack_voltage_v10);

void cl712_set_tx_bms_screen_message2(
	device_instances_t device,
    int16_t battery_1_temperature_c,
    int16_t battery_1_pack_current_v10,
    uint8_t battery_1_isolation_fault_status,
    uint8_t battery_1_precharge_status,
    uint8_t battery_1_positive_contactor_status,
    uint8_t battery_1_negative_contactor_status);

void cl712_set_tx_bms_screen_message3(
	device_instances_t device,
    uint16_t battery_2_high_cell_voltage_mv,
    uint16_t battery_2_low_cell_voltage_mv,
    uint16_t battery_2_cell_voltage_delta_mv,
    uint16_t battery_2_pack_voltage_v10);

void cl712_set_tx_bms_screen_message4(
	device_instances_t device,
    int16_t battery_2_temperature_c,
    int16_t battery_2_pack_current_v10,
    uint8_t battery_2_isolation_fault_status,
    uint8_t battery_2_precharge_status,
    uint8_t battery_2_positive_contactor_status,
    uint8_t battery_2_negative_contactor_status);

void cl712_set_tx_bms_screen_message5(
	device_instances_t device,
    uint16_t battery_3_high_cell_voltage_mv,
    uint16_t battery_3_low_cell_voltage_mv,
    uint16_t battery_3_cell_voltage_delta_mv,
    uint16_t battery_3_pack_voltage_v10);

void cl712_set_tx_bms_screen_message6(
	device_instances_t device,
    int16_t battery_3_temperature_c,
    int16_t battery_3_pack_current_v10,
    uint8_t battery_3_isolation_fault_status,
    uint8_t battery_3_precharge_status,
    uint8_t battery_3_positive_contactor_status,
    uint8_t battery_3_negative_contactor_status);

//=============================================================================
//
// PDM STATUS MESSAGE
//
//=============================================================================
//
void cl712_set_tx_pdm_status_screen_message1(
	device_instances_t device,
    uint16_t pdm_1_supply_voltage_v10,
    uint16_t pdm_2_supply_voltage_v10,
    uint16_t pdm_3_supply_voltage_v10,
    uint8_t  pdm_1_status,
    uint8_t  pdm_2_status);

void cl712_set_tx_pdm_status_screen_message2(
	device_instances_t device,
	uint8_t pdm_3_status,
	uint8_t arg2,
	uint8_t arg3,
	uint8_t arg4,
    uint8_t arg5,
	uint8_t arg6,
	uint8_t arg7,
	uint8_t arg8);

//=============================================================================
//
// PDM_1_STATUS_AND_CURRENTS
//
//=============================================================================
//
void cl712_set_tx_pdm_1_status_screen_message1(
	device_instances_t device,
	uint8_t pm_1_reverse_lights_status,
    uint8_t pdm_1_brake_lights_status,
    uint8_t pdm_1_deck_lights_status,
    uint8_t pdm_1_brake_release_status,
    uint8_t pdm_1_reverse_beeper_status,
    uint8_t pdm_1_reverse_beacon_status,
    uint8_t pdm_1_right_spray_selenoid_status,
    uint8_t pdm_1_center_spray_selenoid_status);

void cl712_set_tx_pdm_1_status_and_current_screen_message2(
	device_instances_t device,
    uint8_t pdm_1_left_spray_selenoid_status,
    uint8_t pdm_1_spare_status,
    uint8_t pdm_1_channel_11_status,
    uint8_t pdm_1_channel_12_status,
    uint8_t pdm_1_reverse_lights_current,
    uint8_t pdm_1_brake_lights_current,
    uint8_t pdm_1_deck_lights_current,
    uint8_t pdm_1_brake_release_current);

void cl712_set_tx_pdm_1_current_screen_message3(
	device_instances_t device,
    uint8_t pdm_1_reverse_beeper_current,
    uint8_t pdm_1_reverse_beacon_current,
    uint8_t pdm_1_right_spray_selenoid_current,
    uint8_t pdm_1_center_spray_selenoid_current,
    uint8_t pdm_1_left_spray_selenoid_current,
    uint8_t pdm_1_spare_current,
    uint8_t pdm_1_channel_11_current,
    uint8_t pdm_1_channel_12_current);


//=============================================================================
//
// PDM_2_STATUS_AND_CURRENTS
//
//=============================================================================
//
void cl712_set_tx_pdm_2_status_screen_message1(
	device_instances_t device,
    uint8_t pdm_2_start_drain_selenoid_status,
    uint8_t pdm_2_aux_hydraulics_12v_bussbar_status,
    uint8_t pdm_2_gear_1_selenoid_status,
    uint8_t pdm_2_gear_2_selenoid_status,
    uint8_t pdm_2_gear_3_selenoid_status,
    uint8_t pdm_2_gear_4_selenoid_status,
    uint8_t pdm_2_power_electronics_pump_status,
    uint8_t pdm_2_selector_selenoid_status);

void cl712_set_tx_pdm_2_status_and_current_screen_message2(
	device_instances_t device,
    uint8_t pdm_2_brake_release_selenoid_status,
    uint8_t pdm_2_service_brake_selenoid_status,
    uint8_t pdm_2_hydraulic_motor_cooling_pump_status,
    uint8_t pdm_2_hydraulic_motor_radiator_cooling_fan_status,
    uint8_t pdm_2_start_drain_selenoid_current,
    uint8_t pdm_2_aux_hydraulics_12v_bussbar_current,
    uint8_t pdm_2_gear_1_selenoid_current,
    uint8_t pdm_2_gear_2_selenoid_current);

void cl712_set_tx_pdm_2_current_screen_message3(
	device_instances_t device,
    uint8_t pdm_2_gear_3_selenoid_current,
    uint8_t pdm_2_gear_4_selenoid_current,
    uint8_t pdm_2_power_electronics_pump_current,
    uint8_t pdm_2_selector_selenoid_current,
    uint8_t pdm_2_brake_release_selenoid_current,
    uint8_t pdm_2_service_brake_selenoid_current,
    uint8_t pdm_2_hydraulic_motor_cooling_pump_current,
    uint8_t pdm_2_hydraulic_motor_radiator_cooling_fan_current);

//=============================================================================
//
// PDM_3_STATUS_AND_CURRENTS
//
//=============================================================================
//
void cl712_set_tx_pdm_3_status_screen_message1(
	device_instances_t device,
    uint8_t pdm_3_work_lights_status,
    uint8_t pdm_3_beacon_status,
    uint8_t pdm_3_head_light_status,
    uint8_t pdm_3_horn_status,
    uint8_t pdm_3_charger_1_bussbar_status,
    uint8_t pdm_3_drive_motor_cooling_pump_status,
    uint8_t pdm_3_imt_cooling_pump_status,
    uint8_t pdm_3_fan_radiator_1_2_status);

void cl712_set_tx_pdm_3_status_and_current_screen_message2(
	device_instances_t device,
    uint8_t pdm_3_pump_charger_1_cooling_status,
    uint8_t pdm_3_pump_charger_2_cooling_status,
    uint8_t pdm_3_main_bussbar_supply_status,
    uint8_t pdm_3_hydraulic_fan_selenoid_status,
    uint8_t pdm_3_work_lights_current,
    uint8_t pdm_3_beacon_current,
    uint8_t pdm_3_head_light_current,
    uint8_t pdm_3_horn_current);

void cl712_set_tx_pdm_3_current_screen_message3(
	device_instances_t device,
    uint8_t pdm_3_charger_1_bussbar_current,
    uint8_t pdm_3_drive_motor_cooling_pump_current,
    uint8_t pdm_3_imt_cooling_pump_current,
    uint8_t pdm_3_fan_radiator_1_2_current,
    uint8_t pdm_3_pump_charger_1_cooling_current,
    uint8_t pdm_3_pump_charger_2_cooling_current,
    uint8_t pdm_3_main_bussbar_supply_current,
    uint8_t pdm_3_hydraulic_fan_selenoid_current);

//=============================================================================
//
// Traction Drive
//
//=============================================================================
//
void cl712_set_tx_drive_system_screen_message1(
	device_instances_t device,
    uint16_t drive_dc_voltage_v10,
    uint16_t drive_motor_rpm,
    uint8_t drive_throttle_percentage,
    uint8_t drive_inverter_pcb_temp_c,
    uint8_t drive_inverter_temp_c,
    uint8_t drive_motor_temp_c);

void cl712_set_tx_drive_system_screen_message2(
    device_instances_t device,
    uint16_t throttle_adc_raw,
    uint16_t throttle_released,
    uint16_t throttle_depressed);

//=============================================================================
//
// Hydraulic Inverter
//
//=============================================================================
//
void cl712_set_tx_hydraulic_drive_system_screen_message1(
	device_instances_t device,
    uint16_t ehs_dc_voltage_v10,
    uint16_t ehs_motor_rpm,
    uint8_t ehs_inverter_pcb_temp_c,
    uint8_t ehs_inverter_temp_c,
    uint8_t ehs_motor_temp_c,
    uint8_t can1_rx_25_spare_1);

//=============================================================================
//
// System Pressure
//
//=============================================================================
//
void cl712_set_tx_system_pressures_screen_message1(
	device_instances_t device,
    uint16_t main_pressure_psi,
    uint16_t front_brake_pressure_psi,
    uint16_t rear_brake_pressure_psi,
    uint16_t driveline_pressure_psi);

void cl712_set_tx_system_pressures_screen_message2(
    device_instances_t device,
    uint16_t front_parking_brake_psi,
    uint16_t front_accumulator_pressure_psi,
    uint16_t rear_accumulator_pressure_psi,
    uint16_t transmission_pressure_10x_psi);

//=============================================================================
//
// Pump and Fan Status
//
//=============================================================================
//
void cl712_set_pump_fan_status_message1(
    device_instances_t device,
    uint8_t traction_pump_status,
    uint8_t hydraulic_pump_status,
    uint8_t pe_fan_status,
    uint8_t pe_pump_status,
    uint8_t charger_fan_status,
    uint8_t charger_pump_status);

//=============================================================================
//
// Pump and Fan Override Status
//
//=============================================================================
//
void cl712_set_pump_fan_override_status_message1(
    device_instances_t device,
    uint8_t traction_pump_override_status,
    uint8_t hydraulic_pump_override_status,
    uint8_t pe_fan_override_status,
    uint8_t pe_pump_override_status,
    uint8_t charger_fan_override_status,
    uint8_t charger_pump_override_status);


#endif
