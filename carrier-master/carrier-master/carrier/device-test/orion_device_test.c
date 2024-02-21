/*
 * orion_BMS1.c
 *
 *  Created on: Jul 23, 2019
 *      Author: Cyrus
 */
/*Include Header Files */
#include <stdio.h>
#include <math.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "typedefs.h"
#include "can_service.h"
#include "orion_device.h"
#include "orion_device_test.h"

void test_orion_8bit_getter_functions(device_instances_t device)
{
	int8_t high_cell_temp =  orion_get_pack_high_cell_temperature(device);
	int8_t low_cell_temp = orion_get_pack_low_cell_temperature(device);
	int8_t avg_cell_temp = orion_get_pack_average_cell_temperature(device);
	int8_t internal_temp = orion_get_internal_bms_temperature(device);
	uint8_t state_of_charge = orion_get_state_of_charge( device);
	uint8_t pack_health = orion_get_pack_health( device);
	uint8_t pack_relay_status = orion_get_pack_relay_status( device);
	uint8_t max_cells = orion_get_maximum_number_of_cells(device);
	uint8_t populated_cells = orion_get_number_of_populated_cells(device);
	uint8_t rooling_counter = orion_get_rolling_counter(device);
	uint8_t a = orion_get_pack_high_temperature_ID(device);
	uint8_t b = orion_get_pack_low_temperature_ID(device);
	uint8_t c = orion_get_pack_high_cell_voltage_ID(device);
	uint8_t d = orion_get_pack_low_cell_voltage_ID(device);
	uint8_t e = orion_get_pack_high_internal_resistance_ID(device);
	uint8_t f = orion_get_pack_low_internal_resistance_ID(device);
	uint8_t g = orion_get_pack_high_open_cell_voltage_ID(device);
	uint8_t h = orion_get_pack_low_open_cell_voltage_ID(device);
}

void test_orion_16bit_getter_functions(device_instances_t device)
{
	int16_t instantaneous_pack_current = orion_get_instantaneous_pack_current(device);
	uint16_t instantaneous_pack_voltage = orion_get_instantaneous_pack_voltage(device);
	uint16_t pack_high_cell_voltage= orion_get_pack_high_cell_voltage(device);
	uint16_t low_cell_voltage = orion_get_low_cell_voltage(device);
	uint16_t pack_discharge_current_lim = orion_get_pack_discharge_current_limit(device);
	uint16_t pack_charge_current_limit = orion_get_pack_charge_current_limit(device);
	uint16_t pack_total_resiatance = orion_get_pack_total_resiatance(device);
	uint16_t pack_open_voltage = orion_get_pack_open_voltage(device);
	uint16_t average_cell_voltage = orion_get_average_cell_voltage(device);
	uint16_t high_cell_resistance =  orion_get_high_cell_resistance(device);
	uint16_t low_cell_resistance = orion_get_low_cell_resistance(device);
	uint16_t average_cell_resistance = orion_get_average_cell_resistance(device);
	uint16_t high_open_cell_voltage = orion_get_high_open_cell_voltage(device);
	uint16_t low_open_cell_voltage = orion_get_low_open_cell_voltage(device);
	uint16_t low_power_supply_indicator = orion_get_low_power_supply_indicator(device);
	uint8_t maximum_number_of_cells = orion_get_maximum_number_of_cells(device);
}

void test_orion_custom_status_bits(device_instances_t device)
{
	// Flag 0
	// TESTED and VERIFIED
	bool_t discharge_relay = orion_get_flag0_discharge_relay(device);
	bool_t charge_relay = orion_get_flag0_charge_relay(device);
	bool_t charger_safety = orion_get_flag0_charger_safety(device);;
	bool_t voltage_fail_safe = orion_get_flag0_voltage_fail_safe(device);;
	bool_t current_fail_safe = orion_get_flag0_current_fail_safe(device);;
	bool_t power_supply_failure = orion_get_flag0_power_supply_failure(device);;
	bool_t multipurpose_input_state = orion_get_flag0_multipurpose_input_state(device);;

	// Flag 1
	// TESTED AND VERIFIRED
	bool_t internal_communication_fault = orion_get_flag1_internal_communication_fault(device);;
	bool_t internal_conversion_fault = orion_get_flag1_internal_conversion_fault(device);
	bool_t weak_cell_fault = orion_get_flag1_weak_cell_fault(device);
	bool_t low_cell_voltage_fault = orion_get_flag1_low_cell_voltage_fault(device);
	bool_t open_cell_voltage_fault = orion_get_flag1_open_cell_voltage_fault(device);
	bool_t current_sensor_fault = orion_get_flag1_current_sensor_fault(device);
	bool_t voltage_redundancy_fault = orion_get_flag1_voltage_redundancy_fault(device);

	// Flag2
	// TESTED AND VERIFIRED
	bool_t weak_pack_fault = orion_get_flag2_weak_pack_fault(device);
	bool_t fan_monitor_fault = orion_get_flag2_fan_monitor_fault(device);
	bool_t thermistor_fault =  orion_get_flag2_thermistor_fault(device);
	bool_t communication_fault =  orion_get_flag2_communication_fault(device);
	bool_t always_on_power_supply_fault =  orion_get_flag2_always_on_power_supply_fault(device);
	bool_t high_voltage_isolation_fault =  orion_get_flag2_high_voltage_isolation_fault(device);
	bool_t power_supply_fault =  orion_get_flag2_power_supply_fault(device);
	bool_t charger_enable_relay_fault =  orion_get_flag2_charger_enable_relay_fault(device);

	// Flag3
	// TESTED AND VERIFIRED
	bool_t discharge_enable_relay_fault =  orion_get_flag3_discharge_enable_relay_fault(device);
	bool_t charger_safety_relay_fault = orion_get_flag3_charger_safety_relay_fault(device);
	bool_t internal_memory_fault = orion_get_flag3_internal_memory_fault(device);
	bool_t internal_thermistor_fault = orion_get_flag3_internal_thermistor_fault(device);
	bool_t internal_logic_fault = orion_get_flag3_internal_logic_fault(device);

	// Current Status Limit
	// TESTED AND VERIFIRED
	bool_t discharge_current_limit_low_soc =  orion_get_current_status_discharge_current_limit_low_soc(device);
	bool_t discharge_current_limit_high_cell_resistance = orion_get_current_status_discharge_current_limit_high_cell_resistance(device);
	bool_t discharge_current_limit_temperature =  orion_get_current_status_discharge_current_limit_temperature(device);
	bool_t discharge_current_limit_low_cell_voltage =  orion_get_current_status_discharge_current_limit_low_cell_voltage(device);
	bool_t discharge_current_limit_low_pack_voltage =  orion_get_current_status_discharge_current_limit_low_pack_voltage(device);
	bool_t discharge_current_limit_voltage_failsafe =  orion_get_current_status_discharge_current_limit_voltage_failsafe(device);
	bool_t discharge_current_limit_communication_failsafe =  orion_get_current_status_discharge_current_limit_communication_failsafe(device);
	bool_t charge_current_limit_high_soc =  orion_get_current_status_charge_current_limit_high_soc(device);
	bool_t charge_current_limit_high_cell_resistance =  orion_get_current_status_charge_current_limit_high_cell_resistance(device);
	bool_t charge_current_limit_temperature = orion_get_current_status_charge_current_limit_temperature(device);
	bool_t charge_current_limit_high_cell_voltage =  orion_get_current_status_charge_current_limit_high_cell_voltage(device);
	bool_t charge_current_limit_high_pack_voltage = orion_get_current_status_charge_current_limit_high_pack_voltage(device);
	bool_t charger_latch =  orion_get_current_status_charger_latch(device);
	bool_t charge_current_limit_alternate_current_limit =  orion_get_current_status_charge_current_limit_alternate_current_limit(device);

}
