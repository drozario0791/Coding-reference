/******************************************************************************
 *
 *        Name: cvc_output_control.c
 *
 * Description: A module that controls the state of the pins that are
 *              configured as outputs. Higher level functions which
 *              can used by vehicle control.
 *
 *      Author: Deepak
 *        Date: Monday, 26 August 2019
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "typedefs.h"
#include "can_service.h"
#include "cvc_output_control.h"


//=============================================================================
//
// OUT_A02_power_for_charge_box_estop_led_control()
//
//=============================================================================
//
void OUT_A02_power_for_charge_box_estop_led_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_A14_power_for_charge_box_estop_led, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_A14_power_for_charge_box_estop_led, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_A01_turn_on_balancing_signal_control()
//
//=============================================================================
//
void OUT_A01_turn_on_balancing_signal_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_A13_turn_on_balancing_signal, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_A13_turn_on_balancing_signal, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_A14_screen_constant_12v_pwr_control()
//
//=============================================================================
//
void OUT_A14_screen_constant_12v_pwr_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_A02_screen_constant_12v_pwr, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_A02_screen_constant_12v_pwr, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_A13_telematics_constant_12v_pwr_control()
//
//=============================================================================
//
void OUT_A13_telematics_constant_12v_pwr_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_A01_telematics_constant_12v_pwr, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_A01_telematics_constant_12v_pwr, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_B09_low_accum_light_pwr_control()
//
//=============================================================================
//
void OUT_B09_low_accum_light_pwr_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_B09_low_accum_light_pwr, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_B09_low_accum_light_pwr, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_B10_brake_impending_light_control()
//
//=============================================================================
//
void OUT_B10_brake_impending_light_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_B10_brake_impending_light, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_B10_brake_impending_light, OUTPUT_OFF);

    }


}


//=============================================================================
//
// OUT_B02_status_green_led_signal_control()
//
//=============================================================================
//
void OUT_B02_status_green_led_signal_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_B02_status_bicolor_green_led, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_B02_status_bicolor_green_led, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_B03_stop_charge_button_led_control()
//
//=============================================================================
//
void OUT_B03_stop_charge_button_led_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_D01_battery_1_positive_contactor_control()
//
//=============================================================================
//
void OUT_D01_battery_1_positive_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_C09_seat_belt_light_pwr_control()
//
//=============================================================================
//
void OUT_C09_seat_belt_light_pwr_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_C09_seat_belt_light_pwr, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_C09_seat_belt_light_pwr, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_C10_seat_belt_buzzer_pwr_control()
//
//=============================================================================
//
void OUT_C10_seat_belt_buzzer_pwr_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_C10_seat_belt_buzzer_pwr, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_C10_seat_belt_buzzer_pwr, OUTPUT_OFF);

    }


}


//=============================================================================
//
// OUT_C04_aba_brake_button_led_pwr_control()
//
//=============================================================================
//
void OUT_C04_aba_brake_button_led_pwr_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_C04_aba_brake_button_led_pwr, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_C04_aba_brake_button_led_pwr, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_A10_blue_shutdown_light_signal_control()
//
//=============================================================================
//
void OUT_A10_blue_shutdown_light_signal_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_A10_blue_shutdown_light_signal, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_A10_blue_shutdown_light_signal, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_A09_master_contactor_signal_control()
//
//=============================================================================
//
void OUT_A09_master_contactor_signal_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_A09_master_contactor_signal, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_A09_master_contactor_signal, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_C13_spare_control()
//
//=============================================================================
//
void OUT_C13_spare_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_C13_spare, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_C13_spare, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_C14_battery_3_precharge_contactor_control()
//
//=============================================================================
//
void OUT_C14_battery_3_precharge_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_A03_charge_box_relay_signal_from_cvc_control()
//
//=============================================================================
//
void OUT_A03_charge_box_relay_signal_from_cvc_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_A03_charge_box_relay_signal_from_cvc, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_A03_charge_box_relay_signal_from_cvc, OUTPUT_OFF);

    }


}


//=============================================================================
//
// OUT_D04_battery_2_positive_contactor_control()
//
//=============================================================================
//
void OUT_D04_battery_2_positive_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_D13_battery_3_negative_contactor_control()
//
//=============================================================================
//
void OUT_D13_battery_3_negative_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

    }


}


//=============================================================================
//
// OUT_D10_battery_3_positive_contactor_control()
//
//=============================================================================
//
void OUT_D10_battery_3_positive_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

    }

}



//=============================================================================
//
// OUT_D09_battery_2_precharge_contactor_control()
//
//=============================================================================
//
void OUT_D09_battery_2_precharge_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_D02_battery_1_negative_contactor_control()
//
//=============================================================================
//
void OUT_D02_battery_1_negative_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_D03_battery_1_precharge_contactor_control()
//
//=============================================================================
//
void OUT_D03_battery_1_precharge_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_C02_reverse_gear_solenoid_control()
//
//=============================================================================
//
void OUT_C02_reverse_gear_solenoid_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_C02_reverse_gear_solenoid, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_C02_reverse_gear_solenoid, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_D14_dash_e_stop_led_pwr_control()
//
//=============================================================================
//
void OUT_D14_dash_e_stop_led_pwr_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D14_dash_e_stop_led_pwr, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D14_dash_e_stop_led_pwr, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_C01_forward_gear_solenoid_control()
//
//=============================================================================
//
void OUT_C01_forward_gear_solenoid_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_C01_forward_gear_solenoid, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_C01_forward_gear_solenoid, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_B01_status_red_led_control()
//
//=============================================================================
//
void OUT_B01_status_red_led_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_B01_status_bicolor_red_led, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_B01_status_bicolor_red_led, OUTPUT_OFF);

    }

}


//=============================================================================
//
// OUT_D07_battery_2_negative_contactor_control()
//
//=============================================================================
//
void OUT_D07_battery_2_negative_contactor_control(bool_t control)
{

    // Enable or disable the output of the CVC based on the control
    // argument provided by the user
    if (control == TRUE)
    {

        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_ON);

    }
    else
    {
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);

    }

}
