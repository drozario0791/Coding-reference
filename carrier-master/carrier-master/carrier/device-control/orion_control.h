/******************************************************************************
 *
 *        Name: orion_control.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 30 August 2019
 *
 ******************************************************************************
 */

#ifndef ORION_CONTROL_H_
#define ORION_CONTROL_H_


//=============================================================================
//
// Get the number of active BMS's on the carrier
//
//=============================================================================
//
uint8_t get_number_of_bms();


//=============================================================================
//
// State of Charge of the Battery pack
//
//=============================================================================
//
uint8_t get_battery_pack_SOC();
uint8_t get_screen_SOC();


//=============================================================================
//
// Battery Pack Voltage and Current
//
// Returns the total current sourced by the battery packs on the
// vehilce.
//
// Returns the average voltage of the battery packs connected in
// parallel.
//
//=============================================================================
//
int16_t get_instantaneous_battery_pack_current();
uint16_t get_battery_pack_voltage();


//=============================================================================
//
//  High and Low Cell Voltages of all the battery packs on the system
//
//  Returns the highest cell voltage measured across all three battery
//  packs.
//
//  Returns the lowesr cell voltage measured across all three battery
//  packs.
//
//=============================================================================
//
uint16_t get_pack_high_cell_voltage();
uint16_t get_pack_low_cell_voltage();


//=============================================================================
//
// Returns the maximum of low cell temperatures across all three battery
// packs.
//
// Returns the maximum of high cell temperature across all three
// battery packs.
//
//=============================================================================
//
int8_t get_battery_pack_low_cell_max_temperature();
int8_t get_battery_pack_high_cell_max_temperature();


//=============================================================================
//
//  Checks a set of conditions to determine if a fault
//  has been incurred by the BMS.
//
//=============================================================================
//
bool_t get_battery_pack_error_status(device_instances_t device);


//=============================================================================
//
// Returns true if any of the BMS thresholds has been exceeded.
//
//=============================================================================
//
bool_t bms_fan_controls();


//=============================================================================
//
// Enable and disable cell balancing of the three battery packs.
//
//=============================================================================
//
void enable_cell_balancing();


//=============================================================================
//
// Driving Range in minutes
//
//=============================================================================
//
uint16_t get_range_in_minutes_flats();
uint16_t get_range_in_minutes_slopes();

//=============================================================================
//
// BMS_Fresh Data
//
//=============================================================================
//
bool_t bms_fresh_data_1();
bool_t bms_fresh_data_2();
bool_t bms_fresh_data_3();


#endif
