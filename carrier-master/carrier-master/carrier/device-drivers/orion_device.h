/******************************************************************************
 *
 *        Name: orion_device.h
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */

#ifndef ORION_DEVICE_H_
#define ORION_DEVICE_H_

//=============================================================================
//
// Description: ORION BMS Initialisation Function
//
//=============================================================================
//
void init_orion_bms(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line);

//=============================================================================
//
// DESCRIPTION: SET RX Timeout Counter
//
//=============================================================================
//
void orion_bms_set_timeout_orion_bms_inst_data(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_dcl_ccl_temp(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_misc_data(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_custom_faults(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_faults_with_identifiers(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_cell_data_1(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_cell_data_2(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_pack_iso_fault(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

void orion_bms_set_timeout_orion_bms_cycle_data(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);


/******************************************************************************
 * Current: Units = 0.1A This is the current pack amperage value for
 *          the battery pack. This will report a constant 0 if no
 *          current sensor is selected.
 ******************************************************************************
 */
int16_t orion_get_instantaneous_pack_current(device_instances_t device);

/******************************************************************************
 * Instant Pack Voltage: Units = 0.1V This is the total pack voltage
 *      as read by a voltage sensor.
 *
 ******************************************************************************
 */
uint16_t orion_get_instantaneous_pack_voltage(device_instances_t device);

/******************************************************************************
 * Maximum Cell Voltage:
 *      Units = 0.0001V
 *      This is the voltage of the highest measured cell in the battery pack.
 *
 ******************************************************************************
 */
uint16_t orion_get_pack_high_cell_voltage(device_instances_t device);

/******************************************************************************
 * Minimum Cell Voltage: Units = 0.0001V This is the voltage of the
 *      lowest measured cell in the battery pack.
 *
 ******************************************************************************
 */
uint16_t orion_get_pack_low_cell_voltage(device_instances_t device);

/******************************************************************************
 * Pack Discharge Current Limit: Units = 1A The discharge current
 *                        limit (sometimes referred to as DCL for
 *                        short, or load current limit) represents the
 *                        maximum amount of current (measured in amps)
 *                        that can be pulled or drawn from the battery
 *                        pack without damaging or exceeding system
 *                        ratings.  This value can change due to a
 *                        number of reasons including temperature,
 *                        voltage, internal resistance and age.
 *
 ******************************************************************************
 */
uint16_t orion_get_pack_discharge_current_limit(device_instances_t device);

/******************************************************************************
 * Pack Charge Current Limit Units = 1A The charge current limit
 *                          (sometimes referred to as CCL for short,
 *                          or source current limit) represents the
 *                          maximum amount of current (measured in
 *                          amps) that can be put in or absorbed by
 *                          the battery pack without damaging or
 *                          exceeding system ratings. This value can
 *                          change due to a number of reasons
 *                          including temperature, voltage, internal
 *                          resistance and age.
 ******************************************************************************
 */
uint16_t orion_get_pack_charge_current_limit(device_instances_t device);

/******************************************************************************
 * Maximum Cell Temperature Units = 1C This signed value is the
 * highest temperature measured in the battery pack.
 *
 * NOTE: All temperature values have the same units and are signed
 ******************************************************************************
 */
int8_t orion_get_pack_high_cell_temperature(device_instances_t device);
int8_t orion_get_pack_low_cell_temperature(device_instances_t device);
int8_t orion_get_pack_average_cell_temperature(device_instances_t device);
int8_t orion_get_internal_bms_temperature(device_instances_t device);

/******************************************************************************
 * State of Charge: Units = 0.5% State of charge of the pack as a
 *                  percentage
 ******************************************************************************
 */
uint8_t orion_get_state_of_charge(device_instances_t device);

/******************************************************************************
 * Pack Resistance Units = 1mOhm (0.001Ohm) This is the combined
 * measured internal resistance for the entire battery pack.
 ******************************************************************************
 */
uint16_t orion_get_pack_total_resiatance(device_instances_t device);


/******************************************************************************
 * Pack Open Voltage Units = 0.1V Open voltage across entire pack.
 * The open voltage refers to the sitting or unloaded voltage of a
 * battery. In other words, the open voltage would be the voltage of a
 * battery when no current is going in or out of the battery. This is
 * important for determining how much current can go in or out of the
 * battery.
 ******************************************************************************
 */
uint16_t orion_get_pack_open_voltage(device_instances_t device);

/******************************************************************************
 * Pack Amp Hours Units = 0.1Ahr This is the calculated number of
 * amphours remaining in the battery pack. This is used in part to
 * calculate State of Charge.
 ******************************************************************************
 */
uint16_t orion_get_pack_amp_hours(device_instances_t device);
uint8_t orion_get_pack_health(device_instances_t device);
uint8_t orion_get_pack_relay_status(device_instances_t device);


/******************************************************************************
 * Average Cell Voltage: Units = 0.0001V This is the voltage of the
 *      average measured cell in the battery pack.
 *
 ******************************************************************************
 */
uint16_t orion_get_average_cell_voltage(device_instances_t device);

/******************************************************************************
 * Maximum Cell Resistance: Units = 0.01mOhm This is the Internal
 *                        Resistance of the highest measured cell in
 *                        the battery pack.
 *
 ******************************************************************************
 */
uint16_t orion_get_high_cell_resistance(device_instances_t device);

/******************************************************************************
 * Minimum Cell Resistance: Units = 0.01mOhm This is the Internal
 *                        Resistance of the lowest measured cell in
 *                        the battery pack
 *
 ******************************************************************************
 */
uint16_t orion_get_low_cell_resistance(device_instances_t device);

/******************************************************************************
 * Average Cell Resistance:
 *      Units = 0.01mOhm
 *      This is the Internal Resistance of the all cells, averaged in the battery pack
 *
 ******************************************************************************
 */
uint16_t orion_get_average_cell_resistance(device_instances_t device);


/******************************************************************************
 * Same as maximum cell voltage, but for open voltage instead of
 * instant
 ******************************************************************************
 */
uint16_t orion_get_high_open_cell_voltage(device_instances_t device);


/******************************************************************************
 * Same as minimum cell voltage, but for open voltage instead of
 * instant
 ******************************************************************************
 */
uint16_t orion_get_low_open_cell_voltage(device_instances_t device);


/******************************************************************************
 * This is the voltage of the power source that directly powers the
 *      BMS (12v nominal).  Units = 0.1V
 ******************************************************************************
 */
uint16_t orion_get_low_power_supply_indicator(device_instances_t device);


/******************************************************************************
 * The maximum number of cells that the BMS is capable of supporting
 ******************************************************************************
 */
uint8_t orion_get_maximum_number_of_cells(device_instances_t device);


/******************************************************************************
 * The number of cells actually populated in the BMS
 ******************************************************************************
 */
uint8_t orion_get_number_of_populated_cells(device_instances_t device);


/******************************************************************************
 * Total Pack Cycles:
 *
 *   This is the total number of cycles that the battery pack has been
 *      put through since the BMS has started keeping track.  A
 *      battery cycle is defined as a net transfer of the total
 *      Amphour capacity of the pack.  In other words, on a 40Ahr
 *      battery one cycle would be a net transfer of 40 Amphours.
 ******************************************************************************
 */
uint16_t orion_get_pack_total_cycles(device_instances_t device);


/******************************************************************************
 * This is a sanity-check value that increments automatically by 1
 * once every 100ms (maximum value is 255, after which it resets to 0
 * and starts again). It can be used by third party equipment to
 * verify that the BMS is providing new and updated data and has not
 * locked up.
 ******************************************************************************
 */
uint8_t orion_get_rolling_counter(device_instances_t device);


//=============================================================================
//
//
// ID refers to the cell number, ranging from 1 to 180 Each of these
// IDs tells which cell is the highest or lowest for the indicated
// parameter
//
//
//=============================================================================
//
uint8_t orion_get_pack_high_temperature_ID(device_instances_t device);
uint8_t orion_get_pack_low_temperature_ID(device_instances_t device);
uint8_t orion_get_pack_high_cell_voltage_ID(device_instances_t device);
uint8_t orion_get_pack_low_cell_voltage_ID(device_instances_t device);
uint8_t orion_get_pack_high_internal_resistance_ID(device_instances_t device);
uint8_t orion_get_pack_low_internal_resistance_ID(device_instances_t device);
uint8_t orion_get_pack_high_open_cell_voltage_ID(device_instances_t device);
uint8_t orion_get_pack_low_open_cell_voltage_ID(device_instances_t device);

//=============================================================================
//
//
// Status Bits
//
//
//=============================================================================
//


//=============================================================================
// Custom Flag 0 Status Bits
//=============================================================================
//
bool_t orion_get_flag0_discharge_relay(device_instances_t device);
bool_t orion_get_flag0_charge_relay(device_instances_t device);
bool_t orion_get_flag0_charger_safety(device_instances_t device);
bool_t orion_get_flag0_voltage_fail_safe(device_instances_t device);
bool_t orion_get_flag0_current_fail_safe(device_instances_t device);
bool_t orion_get_flag0_power_supply_failure(device_instances_t device);
bool_t orion_get_flag0_multipurpose_input_state(device_instances_t device);

//=============================================================================
// Custom Flag 1 Status Bits
//=============================================================================
//
bool_t orion_get_flag1_internal_communication_fault(device_instances_t device);
bool_t orion_get_flag1_internal_conversion_fault(device_instances_t device);
bool_t orion_get_flag1_weak_cell_fault(device_instances_t device);
bool_t orion_get_flag1_low_cell_voltage_fault(device_instances_t device);
bool_t orion_get_flag1_open_cell_voltage_fault(device_instances_t device);
bool_t orion_get_flag1_current_sensor_fault(device_instances_t device);
bool_t orion_get_flag1_voltage_redundancy_fault(device_instances_t device);

//=============================================================================
// Custom Flag 2 Status Bits
//=============================================================================
//
bool_t orion_get_flag2_weak_pack_fault(device_instances_t device);
bool_t orion_get_flag2_fan_monitor_fault(device_instances_t device);
bool_t orion_get_flag2_thermistor_fault(device_instances_t device);
bool_t orion_get_flag2_communication_fault(device_instances_t device);
bool_t orion_get_flag2_always_on_power_supply_fault(device_instances_t device);
bool_t orion_get_flag2_high_voltage_isolation_fault(device_instances_t device);
bool_t orion_get_flag2_power_supply_fault(device_instances_t device);
bool_t orion_get_flag2_charger_enable_relay_fault(device_instances_t device);

//=============================================================================
// Custom Flag 3 Status Bits
//=============================================================================
//
bool_t orion_get_flag3_discharge_enable_relay_fault(device_instances_t device);
bool_t orion_get_flag3_charger_safety_relay_fault(device_instances_t device);
bool_t orion_get_flag3_internal_thermistor_fault(device_instances_t device);
bool_t orion_get_flag3_internal_memory_fault(device_instances_t device);
bool_t orion_get_flag3_internal_logic_fault(device_instances_t device);

//=============================================================================
// Current Status
//=============================================================================
//
bool_t orion_get_current_status_discharge_current_limit_low_soc(device_instances_t device);
bool_t orion_get_current_status_discharge_current_limit_high_cell_resistance(device_instances_t device);
bool_t orion_get_current_status_discharge_current_limit_temperature(device_instances_t device);
bool_t orion_get_current_status_discharge_current_limit_low_cell_voltage(device_instances_t device);
bool_t orion_get_current_status_discharge_current_limit_low_pack_voltage(device_instances_t device);
bool_t orion_get_current_status_discharge_current_limit_voltage_failsafe(device_instances_t device);
bool_t orion_get_current_status_discharge_current_limit_communication_failsafe(device_instances_t device);
bool_t orion_get_current_status_charge_current_limit_high_soc(device_instances_t device);
bool_t orion_get_current_status_charge_current_limit_high_cell_resistance(device_instances_t device);
bool_t orion_get_current_status_charge_current_limit_temperature(device_instances_t device);
bool_t orion_get_current_status_charge_current_limit_high_cell_voltage(device_instances_t device);
bool_t orion_get_current_status_charge_current_limit_high_pack_voltage(device_instances_t device);
bool_t orion_get_current_status_charger_latch(device_instances_t device);
bool_t orion_get_current_status_charge_current_limit_alternate_current_limit(device_instances_t device);

bool_t orion_get_can_rx_ok(device_instances_t device);

#endif
