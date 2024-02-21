/******************************************************************************
 *
 *        Name: pdm_control_three.h
 *
 * Description: The Carrier specific PDM channels are configured and
 *              initialized in this module. The command channel output
 *              functions are named with the Carrier wiring diagram
 *              names.
 *
 *              The pdm_init(THREE, 0, CAN2) function must first be
 *              called from User_Init().
 *
 *              The pdm_control_3_init() must then be called from
 *              User_Init() prior to calling any of the
 *              pdm_n_output_channel_() functions.
 *
 *      Author: Tom
 *        Date: Wednesday, 14 August 2019
 *
 ******************************************************************************
 */


#ifndef PDM_CONTROL_THREE_H
#define PDM_CONTROL_THREE_H

//
// Rear deck init PDM functions and set output channels to default
// state.
//
void pdm_control_3_init();


//
// pdm_device_one output channel command functions.
//
void pdm_3_command_work_lights_pwr(
    bool_t switch_on);

void pdm_3_command_beacon_pwr(
    bool_t switch_on);

void pdm_3_command_head_light_pwr(
    bool_t switch_on);

void pdm_3_command_horn_pwr(
    bool_t switch_on);

void pdm_3_command_charger_1_busbar_pwr(
    bool_t switch_on);

void pdm_3_command_drive_motor_cooling_pump_pwr(
    bool_t switch_on);

void pdm_3_command_imt_cooling_pump_pwr(
    bool_t switch_on);

void pdm_3_command_fan_radiator_1_2_pwr(
    bool_t switch_on);

void pdm_3_command_pump_charger_1_cooling_pwr(
    bool_t switch_on);

void pdm_3_command_pump_charger_2_cooling_pwr(
    bool_t switch_on);

void pdm_3_command_main_bussbar_supply_pwr(
    bool_t switch_on);

void pdm_3_command_hydraulic_fan_solenoid_pwr(
    bool_t switch_on);


//=============================================================================
//
// Current Feedback
//
//=============================================================================
//
uint8_t pdm_3_get_work_lights_current_feedback();
uint8_t pdm_3_get_beacon_current_feedback();
uint8_t pdm_3_get_head_light_current_feedback();
uint8_t pdm_3_get_horn_current_feedback();
uint8_t pdm_3_get_charger_1_busbar_current_feedback();
uint8_t pdm_3_get_drive_motor_cooling_pump_current_feedback();
uint8_t pdm_3_get_imt_cooling_pump_current_feedback();
uint8_t pdm_3_get_fan_radiator_1_2_current_feedback();
uint8_t pdm_3_get_pump_charger_1_cooling_current_feedback();
uint8_t pdm_3_get_pump_charger_2_cooling_current_feedback();
uint8_t pdm_3_get_main_bussbar_supply_current_feedback();
uint8_t pdm_3_get_hydraulic_fan_solenoid_current_feedback();


//=============================================================================
//
// Analog Input
//
//=============================================================================
//
uint16_t pdm_3_get_front_traction_temperature_sensor_1_analog_input();
uint16_t pdm_3_get_front_traction_temperature_sensor_2_analog_input();

#endif // PDM_DEVICE_THREE_H
