/******************************************************************************
 *
 *        Name: cvc_input_control.h
 *
 * Description: There are inputs and outputs present on a CVC. This
 *              module is dedicated to read the input values of the
 *              CVC and return the status of the input pins. These
 *              values are used by various vehicle control functions
 *              to perform the desired actions.
 *
 *      Author: Deepak
 *        Date: Monday, 26 August 2019
 *
 ******************************************************************************
 */

#ifndef CVC_INPUT_CONTROL_H_
#define CVC_INPUT_CONTROL_H_

typedef enum {
    FREQ_1HZ   = 10,
    FREQ_10HZ  = 10,
    FREQ_50HZ  = 50,
    FREQ_100HZ = 100,
    FREQ_200HZ = 200,
    FREQ_500HZ = 500,
} sampling_frequency_t;

//=============================================================================
//
// Return the status of the CVC pins that are configured as inputs.
//
//=============================================================================
//
Input_State_t IN_A15_stop_charging_button_signal_status();
Input_State_t IN_A16_dash_e_stop_signal_status();
Input_State_t IN_A17_charge_box_e_stop_signal_status();
Input_State_t IN_A18_diagnositic_switch_signal_status();

Input_State_t IN_B15_forward_switch_status();
Input_State_t IN_B16_1st_gear_switch_status();
Input_State_t IN_B17_2nd_gear_switch_status();
Input_State_t IN_B18_3rd_gear_switch_status();

Input_State_t IN_C11_neutral_switch_status();
Input_State_t IN_C15_aba_brake_button_signal_status();
Input_State_t IN_C16_aba_brake_momentary_signal_status();
Input_State_t IN_C18_reverse_switch_status();


//=============================================================================
//
// Returns the analog sensed value
//
//=============================================================================
//
uint16_t IN_E01_cvc_zener_input_status();
uint16_t IN_E03_throttle_position_sensor_signal_status();

#endif
