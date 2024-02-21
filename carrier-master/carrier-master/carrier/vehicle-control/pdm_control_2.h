/******************************************************************************
 *
 *        Name: pdm_control_2.h
 *
 * Description: The Carrier specific PDM channels are configured and
 *              initialized in this module. The command channel output
 *              functions are named with the Carrier wiring diagram
 *              names.
 *
 *              The pdm_init(TWO, 0, CAN2) function must first be
 *              called from User_Init().
 *
 *              The pdm_control_2_init() must then be called from
 *              User_Init() prior to calling any of the
 *              pdm_n_output_channel_() functions.
 *
 *      Author: Tom
 *        Date: Wednesday, 14 August 2019
 *
 ******************************************************************************
 */


#ifndef PDM_CONTROL_TWO_H
#define PDM_CONTROL_TWO_H

//
// Rear deck init PDM functions and set output channels to default
// state.
//
void pdm_control_2_init();


//
// pdm_device_one output channel command functions.
//
void pdm_2_command_start_drain_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(
    bool_t switch_on);

void pdm_2_command_1st_gear_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_2nd_gear_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_3rd_gear_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_4th_gear_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_power_electronics_pump_pwr(
    bool_t switch_on);

void pdm_2_command_selector_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_brake_release_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_service_brake_solenoid_pwr(
    bool_t switch_on);

void pdm_2_command_hydraulic_motor_cooling_pump_pwr(
    bool_t switch_on);

void pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(
    bool_t switch_on);

//
// pdm_device_two current feedback function calls.
//
uint8_t pdm_2_get_start_drain_solenoid_current_feedback();
uint8_t pdm_2_get_aux_hydraulicss_12v_bussbar_current_feedback();
uint8_t pdm_2_get_1st_gear_solenoid_current_feedback();
uint8_t pdm_2_get_2nd_gear_solenoid_current_feedback();
uint8_t pdm_2_get_3rd_gear_solenoid_current_feedback();
uint8_t pdm_2_get_4th_gear_solenoid_current_feedback();
uint8_t pdm_2_get_power_electronics_pump_current_feedback();
uint8_t pdm_2_get_selector_solenoid_current_feedback();
uint8_t pdm_2_get_brake_release_solenoid_current_feedback();
uint8_t pdm_2_get_service_brake_solenoid_current_feedback();
uint8_t pdm_2_get_hydraulic_motor_cooling_pump_current_feedback();
uint8_t pdm_2_get_hydraulic_motor_radiator_cooling_fan_current_feedback();
uint8_t pdm_2_get_power_electronics_pump_current_feedback();


//
// pdm_device to call ti measure the analog sensor value on
// pdm_device_2
//
uint16_t pdm_2_get_rear_accum_pressure_analog_input();
uint16_t pdm_2_get_transmission_oil_pressure_analog_input();
uint16_t pdm_2_get_front_accum_pressure_analog_input();
uint16_t pdm_2_get_front_axil_parking_pressure_analog_input();
uint16_t pdm_2_get_drive_shaft_brake_pressure_analog_input();
uint16_t pdm_2_get_front_brake_pressure_analog_input();

//
//
// Digital Input
//
//
//
uint8_t pdm_2_get_seat_belt_switch_signal();


#endif // PDM_CONTROL_TWO_H
