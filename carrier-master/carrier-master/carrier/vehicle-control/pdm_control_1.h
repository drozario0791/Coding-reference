/******************************************************************************
 *
 *        Name: pdm_device_one.h
 *
 * Description: The Carrier specific PDM channels are configured and
 *              initialized in this module. The command channel output
 *              functions are named with the Carrier wiring diagram
 *              names.
 *
 *              The pdm_init(ONE, 0, CAN2) function must first be
 *              called from User_Init().
 *
 *              The pdm_control_1_init() must then be called from
 *              User_Init() prior to calling any of the
 *              pdm_n_output_channel_() functions.
 *
 *      Author: Tom
 *        Date: Wednesday, 14 August 2019
 *
 ******************************************************************************
 */


#ifndef PDM_DEVICE_ONE
#define PDM_DEVICE_ONE

//
// Rear deck init PDM functions and set output channels to default
// state.
//
void pdm_control_1_init();

//
// pdm_control_one output channel command functions.
//
void pdm_1_command_spare_output(
    bool_t switch_on);

void pdm_1_command_left_spray_solenoid(
    bool_t switch_on);

void pdm_1_command_center_spray_solenoid(
    bool_t switch_on);

void pdm_1_command_right_spray_solenoid(
    bool_t switch_on);

void pdm_1_command_reverse_lights_output(
    bool_t switch_on);

void pdm_1_command_brake_lights_output(
    bool_t switch_on);

void pdm_1_command_deck_lights_output(
    bool_t switch_on);

void pdm_1_command_brake_release_solenoid(
    bool_t switch_on);

void pdm_1_command_reverse_beeper_output(
    bool_t switch_on);

void pdm_1_command_reverse_beacon_output(
    bool_t switch_on);

void pdm_1_command_channel_11_pwr(
    bool_t switch_on);

void pdm_1_command_channel_12_pwr(
    bool_t switch_on);


//=============================================================================
//
// Current Feedback
//
//=============================================================================
//
uint8_t pdm_1_spare_output_current_feedback();
uint8_t pdm_1_left_spray_solenoid_current_feedback();
uint8_t pdm_1_center_spray_solenoid_current_feedback();
uint8_t pdm_1_right_spray_solenoid_current_feedback();
uint8_t pdm_1_reverse_lights_output_current_feedback();
uint8_t pdm_1_brake_lights_output_current_feedback();
uint8_t pdm_1_deck_lights_output_current_feedback();
uint8_t pdm_1_brake_release_solenoid_current_feedback();
uint8_t pdm_1_reverse_beeper_output_current_feedback();
uint8_t pdm_1_reverse_beacon_output_current_feedback();
uint8_t pdm_1_channel_11_current_feedback();
uint8_t pdm_1_channel_12__current_feedback();

#endif // PDM_DEVICE_ONE
