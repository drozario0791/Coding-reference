/******************************************************************************
 *
 *        Name: cvc_output_control.h
 *
 * Description: A module that controls the state of the pins that are
 *              configured as outputs.
 *
 *      Author: Deepak
 *        Date: Monday, 26 August 2019
 *
 ******************************************************************************
 */

#ifndef CVC_OUTPUT_CONTROL_H_
#define CVC_OUTPUT_CONTROL_H_

// Port A
void OUT_A01_turn_on_balancing_signal_control(bool_t control);
void OUT_A02_power_for_charge_box_estop_led_control(bool_t control);
void OUT_A03_charge_box_relay_signal_from_cvc_control(bool_t control);
void OUT_A09_master_contactor_signal_control(bool_t control);
void OUT_A10_blue_shutdown_light_signal_control(bool_t control);
void OUT_A13_telematics_constant_12v_pwr_control(bool_t control);
void OUT_A14_screen_constant_12v_pwr_control(bool_t control);

// Port B
void OUT_B01_status_red_led_control(bool_t control);
void OUT_B02_status_green_led_signal_control(bool_t control);
void OUT_B03_stop_charge_button_led_control(bool_t control);
void OUT_B09_low_accum_light_pwr_control(bool_t control);
void OUT_B10_brake_impending_light_control(bool_t control);

// Port C
void OUT_C01_forward_gear_solenoid_control(bool_t control);
void OUT_C02_reverse_gear_solenoid_control(bool_t control);
void OUT_C04_aba_brake_button_led_pwr_control(bool_t control);
void OUT_C09_seat_belt_light_pwr_control(bool_t control);
void OUT_C10_seat_belt_buzzer_pwr_control(bool_t control);
void OUT_C13_spare_control(bool_t control);
void OUT_C14_battery_3_precharge_contactor_control(bool_t control);

// Port D
void OUT_D01_battery_1_positive_contactor_control(bool_t control);
void OUT_D02_battery_1_negative_contactor_control(bool_t control);
void OUT_D03_battery_1_precharge_contactor_control(bool_t control);
void OUT_D04_battery_2_positive_contactor_control(bool_t control);
void OUT_D07_battery_2_negative_contactor_control(bool_t control);
void OUT_D09_battery_2_precharge_contactor_control(bool_t control);
void OUT_D10_battery_3_positive_contactor_control(bool_t control);
void OUT_D13_battery_3_negative_contactor_control(bool_t control);
void OUT_D14_dash_e_stop_led_pwr_control(bool_t control);

#endif
