/******************************************************************************
 *
 *        Name: accessories_support.c
 *
 * Description: A module that controls the working of all the
 *              accessories on the carrier.
 *
 *      Author: Deepak
 *        Date: Saturday, 28 September 2019
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
#include "cvc_input_control.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "power_up_devices.h"
#include "state_machine.h"
#include "accessories_support.h"
#include "hydraulic_system.h"
#include "timer_service.h"
#include "orion_control.h"
#include "gears_and_transmission.h"
#include "fvt_library.h"
#include "cvc_debug_msgs.h"
#include "skai2_inverter_vissim.h"
#include "can_switches.h"
#include "pku2400_device.h"

#define ENABLE_BALANCING_CURRENT_LIMIT 30

static OUTPUT_STATUS_ turn_on_balancing_signal_control();
static OUTPUT_STATUS_ low_accum_light_control();
static OUTPUT_STATUS_ brake_impending_light_control();
static OUTPUT_STATUS_ status_green_led_signal_control();
static OUTPUT_STATUS_ stop_charge_button_led_control();
static OUTPUT_STATUS_ seat_belt_light_pwr_control();
static OUTPUT_STATUS_ seat_belt_buzzer_pwr_control();
static OUTPUT_STATUS_ aba_brake_button_led_pwr_control();
static void status_led_signal_control();
static void odometer_calculate();
static void estop_led_control();
static void enable_reverse_beeper_becaon();
static void enable_hydraulic_sprayer_motor_solenoid();

extern bool_t bottom_switch_1_status;
extern bool_t left_solenoid_sprayer_status;
extern bool_t middle_solenoid_sprayer_status;
extern bool_t right_solenoid_sprayer_status;


bool_t get_charger_plugged_in_status();
bool_t get_charger_plug_live_status();

bool_t get_charger_plugged_in_status()
{
    return 0;
}
bool_t get_charger_plug_live_status()
{
    return 0;
}

void set_can_switch_led_initial_state();


//=============================================================================
//
// Static Global
//
//=============================================================================
//
static bool_t brakes_desired = TRUE;


//=============================================================================
//
// accessories_support()
//
//=============================================================================
//
void accessories_support()
{

    static bool_t keyswitch_firstpass = FALSE;

    if(IN_A11_keyswitch < 4000)
    {

        disable_outputs();
        pku2400_set_all_leds_to_off_state(ONE);
        keyswitch_firstpass = TRUE;
    }
    else
    {

        if(keyswitch_firstpass == TRUE)
        {
            set_can_switch_led_initial_state();
            keyswitch_firstpass = FALSE;
        }

        //
        // Reverse Becon, reverse lights, reverse beeper control
        //
        enable_reverse_beeper_becaon();

        //
        // Check to see if the state machine is in Estop state
        //
        estop_led_control();

        //
        // Turn on balancing if the total current sourced by the battery
        // pack is less than 30A.
        //
        OUTPUT_STATUS_ balancing_active_status =
            turn_on_balancing_signal_control();

        //
        // Enable the light on the dash if the accummuator pressure is
        // less than the set threshold value.
        //
        OUTPUT_STATUS_ low_accum_status =
            low_accum_light_control();

        //
        // Enable the light on the dash of the main system pressure is
        // less than the set threshold value.
        //
        OUTPUT_STATUS_ brake_impending_light =
            brake_impending_light_control();

        //
        // aba_brake_button: On serious issues now. The Aba brake button
        // is used to enable and disable the brakes on the vehicle.
        //
        OUTPUT_STATUS_ aba_brake_button =
            aba_brake_button_led_pwr_control();

        //
        // Seat Belt Control: Make sure that the operator dude is wearing
        // his seatbelt while driving. He has people waiting back
        // home. (Instill the fear of God in him, if he has not followed
        // the commandement).
        //
        // Enables power to the seatbelt light if seatbelt is not
        // connected.
        OUTPUT_STATUS_ light_pwr = 
        		seat_belt_light_pwr_control();

        OUTPUT_STATUS_ buzzer_pwr = 
        		seat_belt_buzzer_pwr_control();

        //
        // Status LED: indicates if the vehicle is in normal operational
        // state (green light) or if shit hit the fan (red light - critical
        // faults) or the 12V battery is about to die (yellow light).
        //
        status_led_signal_control();

        //
        // Enables or disable the hydraulic motor solenoid on the
        // carrier. Whne enabled hydraulic fluid flows through the
        // hydraulic pump in the rear of the carrier. This is enable if
        // any one of the solenoids are commanded to be turned ON.
        //
        enable_hydraulic_sprayer_motor_solenoid();

        //
        // Odometer Reading: To indicate how much driving is done by the
        // vehilce. Give us bragging rights as well.
        //
        odometer_calculate();
    }

}

//=============================================================================
//
// estop_led_control()
//
//=============================================================================
//
void estop_led_control()
{

    //
    // Determine the state of the vehicle
    //
    state_t sm_state = get_sm_current_state();

    if(sm_state == E_STOP || !(IN_A16_dash_e_stop_signal == INPUT_ON)
       || !(IN_A17_charge_box_e_stop_signal == INPUT_ON))
    {
        //
        // If Estop is pressed, Blink the LEDs
        //
        Update_Output(OUT_A14_power_for_charge_box_estop_led, OUTPUT_OFF);
        Update_Output(OUT_D14_dash_e_stop_led_pwr, OUTPUT_OFF);

    }
    else
    {

        //
        // Steady Red Light
        //
        Update_Output(OUT_A14_power_for_charge_box_estop_led, OUTPUT_ON);
        Update_Output(OUT_D14_dash_e_stop_led_pwr, OUTPUT_ON);


    }

}

//=============================================================================
//
// turn_on_balancing_signal_control()
//
//=============================================================================
//
static OUTPUT_STATUS_ turn_on_balancing_signal_control()
{

    int16_t battery_pack_current =
        (get_instantaneous_battery_pack_current() / 10);

    if(battery_pack_current < ENABLE_BALANCING_CURRENT_LIMIT)
    {

        //
        // Enable balancing if current is under 30A.
        //
        Update_Output(OUT_A13_turn_on_balancing_signal, OUTPUT_ON);

    }
    else
    {

        //
        // Disable balancing if current is over 30A.
        //
        Update_Output(OUT_A13_turn_on_balancing_signal, OUTPUT_OFF);

    }

    return (OUTPUT_STATUS_)OUT_A13_turn_on_balancing_signal_STATUS;

}

//=============================================================================
//
// low_accum_light_control()
//
//=============================================================================
//
static OUTPUT_STATUS_ low_accum_light_control()
{

    bool_t enable_high_voltage =
        get_sm_status_enable_hv_systems();

    uint16_t front_accum_pressure =
        get_front_accum_pressure();

    uint16_t rear_accum_pressure =
        get_rear_accum_pressure();

    uint16_t transmission_pressure =
        get_transmission_pressure();


    if((enable_high_voltage) && ((front_accum_pressure < 1300)
                                 || (rear_accum_pressure < 1300) || (transmission_pressure < 175)))
    {

        //
        // Enable the Light to warn that the accum pressure is low.
        //
        Update_Output(OUT_B09_low_accum_light_pwr, OUTPUT_ON);

    }
    else
    {
        //
        // Disable the light.
        //
        Update_Output(OUT_B09_low_accum_light_pwr, OUTPUT_OFF);

    }

    return (OUTPUT_STATUS_)OUT_B09_low_accum_light_pwr_STATUS;

}

//=============================================================================
//
// brake_impending_light_control()
//
//=============================================================================
//
static OUTPUT_STATUS_ brake_impending_light_control()
{

    bool_t enable_high_voltage =
        get_sm_status_enable_hv_systems();

    uint16_t front_accum_pressure =
        get_front_accum_pressure();

    uint16_t rear_accum_pressure =
        get_rear_accum_pressure();

    uint16_t transmission_pressure =
        get_transmission_pressure();


    if((enable_high_voltage) && ((front_accum_pressure < 1150)
                                 || (rear_accum_pressure < 1150) || (transmission_pressure < 175)))
    {
        //
        // Enable Light
        //
        Update_Output(OUT_B10_brake_impending_light, OUTPUT_ON);

    }
    else
    {
        //
        // Disable Light
        //
        Update_Output(OUT_B10_brake_impending_light, OUTPUT_OFF);

     }

    return (OUTPUT_STATUS_)OUT_B10_brake_impending_light_STATUS;
}

//=============================================================================
//
// status_green_led_signal_control()
//
//=============================================================================
//
void status_led_signal_control()
{

    //
    // Determine the state of the vehicle
    //
    state_t sm_state = get_sm_current_state();


    //
    // Enable the Green Led in the Low Voltage Cab if there are
    //

    if(check_critical_fault_high_voltage_off() && sm_state == CRITICAL_FAILURE_HV_OFF)
    {
        //
        // If a critical error has occured. Enable the red led and
        // disable the green led. This is done to indictae a critical
        // error has occured.
        //
        Update_Output(OUT_B01_status_bicolor_red_led, OUTPUT_ON);
        Update_Output(OUT_B02_status_bicolor_green_led, OUTPUT_OFF);

    }
    else
    {
        //
        // No error has occured. Enable green. Also, enable the RED
        // along with the green if the voltage of the 12V system is
        // dropping below 12V.
        //
        Update_Output(OUT_B02_status_bicolor_green_led, OUTPUT_ON);

        if(IN_A11_keyswitch < 12000)
        {
            //
            // Will turn the led yellow, indicating the 12V battery is
            // below 12V
            //
            Update_Output(OUT_B01_status_bicolor_red_led, OUTPUT_ON);

        }
        else
        {

            // disable the red led
            Update_Output(OUT_B01_status_bicolor_red_led, OUTPUT_OFF);

        }

    }

}

//=============================================================================
//
// stop_charge_button_led_control()
//
//=============================================================================
//
static OUTPUT_STATUS_ stop_charge_button_led_control()
{

    //
    // Check to see if charger is plugged in
    //
    bool_t charger_plugged_in =
    		get_charger_plugged_in_status();

    //
    // Check to see if there is the plug is live
    //
    bool_t charger_plug_live =
    		get_charger_plug_live_status();

    //
    // Check to see if high voltage in ON.
    //
    bool_t enable_high_voltage =
        get_sm_status_enable_hv_systems();

    //
    // Blink or steady on or off depending on various conditions.
    //
    if (charger_plug_live && enable_high_voltage)
    {

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_ON_FLASH);

    }
    else if (charger_plugged_in && enable_high_voltage)
    {

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_ON);

    }
    else
    {

        Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_OFF);

    }

    return (OUTPUT_STATUS_)OUT_B03_stop_charge_button_led_STATUS;

}

//=============================================================================
//
// seat_belt_light_pwr_control()
//
//=============================================================================
//
static OUTPUT_STATUS_ seat_belt_light_pwr_control()
{

    //
    // Check the murphys analog input to see if the seat belt is in
    // use.
    //
    uint8_t seat_belt_signal = pdm_2_get_seat_belt_switch_signal();

    if(seat_belt_signal != FALSE)
    {

        //
        // Turn off the light to indicate the seat_belt has been
        // plugged in.
        //
        Update_Output(OUT_C09_seat_belt_light_pwr, OUTPUT_OFF);

    }
    else
    {

        //
        // Turn on the seat belt LED to indicate the seat belt must be
        // used. Also makes it easy to find the seat belt when the
        // buzzer goes off.
        //
        Update_Output(OUT_C09_seat_belt_light_pwr, OUTPUT_ON);

    }

    return (OUTPUT_STATUS_)OUT_C09_seat_belt_light_pwr_STATUS;
}

//=============================================================================
//
// seat_belt_buzzer_pwr_control()
//
//=============================================================================
//
static OUTPUT_STATUS_ seat_belt_buzzer_pwr_control()
{

    bool_t seatbelt_plugged_in = FALSE;

    //
    // Check to see if high voltage in enabled.
    //
    bool_t enable_high_voltage =
        get_sm_status_enable_hv_systems();

    //
    // Check the murphys analog input to see if the seat belt is in
    // use.
    //
    uint8_t seat_belt_signal = pdm_2_get_seat_belt_switch_signal();

    //
    // Enable the seat belt buzzer if high voltage is ON and the
    // brakes are disabled. This indicates the vehilce is ready to
    // move. At this point, check to ensure that the seat belt is
    // plugged in. If its not plugged in, enable the buzzer. The dude
    // needs to know he must be wearing a seatbelt.
    //
    if(enable_high_voltage && !brakes_desired && (seat_belt_signal == 0))
    {
        //
        // Vehicle is ready to move.
        //

        //
        // Enable seat belt buzzer and scare the crap out of the
        // operator. Instill the fear of God in the dude.
        //
        Update_Output(OUT_C10_seat_belt_buzzer_pwr, OUTPUT_ON);

    }
    else
    {

        //
        // The dudes wearing a seatbelt. Good on him. No scare tatics
        // to used at this moment.
        //
        Update_Output(OUT_C10_seat_belt_buzzer_pwr, OUTPUT_OFF);

    }

    return (OUTPUT_STATUS_)OUT_C10_seat_belt_buzzer_pwr_STATUS;
}

//=============================================================================
//
// aba_brake_button_led_pwr_control()
//
//=============================================================================
//
static OUTPUT_STATUS_ aba_brake_button_led_pwr_control()
{

    static timer_t shuttle_shift_timer;
    static bool_t first_pass = TRUE;
    static bool_t shuttle_shift = FALSE;
    static bool_t set_bit = FALSE;
    static bool_t reset_bit = FALSE;

    if(first_pass)
    {
        //
        // Initialize Timer
        //
        shuttle_shift_timer =
            timer_setup(get_FIVE_SECONDS(), FALLING);

        first_pass = FALSE;

    }

    //
    // Check to see if the state machine is in brakes desired state.
    //
    bool_t sm_brakes_desired =
        get_sm_status_brakes_desired();

    uint16_t front_accum_pressure =
        get_front_accum_pressure();

    uint16_t rear_accum_pressure =
        get_rear_accum_pressure();

    uint16_t transmission_pressure =
        get_transmission_pressure();

    //
    // Check to see if the vehicle is in forward , reverse or neutral.
    //
    shifter_position_t direction =
        get_shifter_direction();

    //
    // Shuttle_shift timer
    //
    //
    // Shuttle shift is true if the vehicle is in F or R, and stays
    // true for a time after the vehicle is taken out of gear.
    //
    shuttle_shift = timer_operate(
        &shuttle_shift_timer,
        (direction == SHIFTER_FORWARD)
        || (direction == SHIFTER_REVERSE));

    set_bit = (sm_brakes_desired || (IN_C15_aba_brake_button_signal == INPUT_OFF) || !shuttle_shift);

    reset_bit = (IN_C16_aba_brake_momentary_signal == INPUT_ON
                 && (front_accum_pressure > 1150)
                 && (rear_accum_pressure > 1150)
                 && (transmission_pressure > 175));

    // State machine desires it.
    // Brake button is pressed.
    // Shuttle shift expires
    brakes_desired = set_reset(brakes_desired,
                               set_bit,
                               reset_bit);

    //
    // Get the rpm of the traction drive inverter. If the traction
    // drive gets above the set max limit by 300rpm. It means the
    // carrier runs away. Therefore, apply the front driveline brakes
    // and rear driveline brakes.
    //
    uint16_t traction_drive_rpm =
       (uint16_t)abs((int16_t)skai_get_vissim_motor_rpm(ONE));

    if((brakes_desired == TRUE) || (traction_drive_rpm > 2300) || (front_accum_pressure < 1100) || (rear_accum_pressure < 1100))
    {

        //
        // Brakes are desired. Therefore, disable power to the brake
        // release solenoids and enable power to the service brake
        // solenoid.
        //
        // Also to indicate the Brakes are enabled, Enable power to
        // brake button.
        //
        Update_Output(OUT_C04_aba_brake_button_led_pwr, OUTPUT_ON);

        pdm_1_command_brake_release_solenoid(FALSE);
        pdm_2_command_brake_release_solenoid_pwr(FALSE);
        pdm_2_command_service_brake_solenoid_pwr(TRUE);
                
        //
        // Reset these bits
        //
        set_bit = FALSE;
        reset_bit = FALSE;
        brakes_desired = TRUE;


    }
    else
    {

        //
        // The vehilce is safe and ready to drive. Therefore enable
        // power to the brake solenoids. Disable power to the service
        // brake solenoid.
        //
        // Also to indicate its safe to drive and disable led power to
        // the aba_brake_button.
        //
        Update_Output(OUT_C04_aba_brake_button_led_pwr, OUTPUT_OFF);

        //
        // enable  brakes
        //
        pdm_1_command_brake_release_solenoid(TRUE);
        pdm_2_command_brake_release_solenoid_pwr(TRUE);
        pdm_2_command_service_brake_solenoid_pwr(FALSE);

    }

    return (OUTPUT_STATUS_)OUT_C04_aba_brake_button_led_pwr_STATUS;

}


//=============================================================================
//
// enable_hydraulic_sprayer_motor_solenoid()
//
//=============================================================================
//
static void enable_hydraulic_sprayer_motor_solenoid()
{

    //
    // enable the hydraulic solenoid if ay of the sprayer solenoids
    // are commanded to be turned ON.
    //
    bool_t solenoids_status =
        sprayer_solenoids_status();

    //
    // Enable the hydraulic sprayer motor solenoid if any one of the
    // three solenoids are commanded to be turned on.
    //
    pdm_2_command_selector_solenoid_pwr(solenoids_status);
    pdm_1_command_left_spray_solenoid(left_solenoid_sprayer_status);
    pdm_1_command_center_spray_solenoid(middle_solenoid_sprayer_status);
    pdm_1_command_spare_output(middle_solenoid_sprayer_status);
    pdm_1_command_right_spray_solenoid(right_solenoid_sprayer_status);

}


//=============================================================================
//
// Disable all the murphy and cvc outputs
//
//=============================================================================
//
void disable_outputs()
{

    //
    // CVC Outputs diasble
    //
    Update_Output(OUT_B10_brake_impending_light, OUTPUT_OFF);
    Update_Output(OUT_A14_power_for_charge_box_estop_led, OUTPUT_OFF);
    Update_Output(OUT_B09_low_accum_light_pwr, OUTPUT_OFF);
    Update_Output(OUT_B10_brake_impending_light, OUTPUT_OFF);
    Update_Output(OUT_B03_stop_charge_button_led, OUTPUT_OFF);
    Update_Output(OUT_C09_seat_belt_light_pwr, OUTPUT_OFF);
    Update_Output(OUT_C10_seat_belt_buzzer_pwr, OUTPUT_OFF);
    Update_Output(OUT_C04_aba_brake_button_led_pwr, OUTPUT_OFF);
    Update_Output(OUT_A14_power_for_charge_box_estop_led, OUTPUT_OFF);
    Update_Output(OUT_D14_dash_e_stop_led_pwr, OUTPUT_OFF);

    //
    // Murphy Accessories
    //
    pdm_3_command_head_light_pwr(FALSE);
    pdm_3_command_beacon_pwr(FALSE);
    pdm_3_command_horn_pwr(FALSE);
    pdm_1_command_reverse_lights_output(FALSE);

}


//=============================================================================
//
// Odometer -> Cyrus
//
//=============================================================================
//
void odometer_calculate()
{
    uint16_t groundspeed_cmps = 0;

    //
    // A running counter of the number of cm travelled in total.
    //
    static uint16_t running_cm_counter          = 0;

    //
    // A counter to keep track of how long it's been.
    //
    static uint16_t time_interval_counter       = 1;

    //
    // status of HV on the system
    //
    bool_t enable_hv_system =
        get_sm_status_enable_hv_systems();

    //
    // traction motor rpm
    //
    int16_t traction_motor_rpm =
        (int16_t)(skai_get_vissim_motor_rpm(ONE));
    uint16_t abs_traction_motor_rpm = abs(traction_motor_rpm);


    shifter_position_t gear_position = get_shifter_gear_position();


    if (enable_hv_system)
    {

        //
        // Calculate the groundspeed of the machine depending on the
        //
        if (gear_position == SHIFTER_1st_GEAR)
        {
            groundspeed_cmps = (uint16_t)((float)abs_traction_motor_rpm * 0.0529);
        }
        else if (gear_position == SHIFTER_2nd_GEAR)
        {
            groundspeed_cmps = (uint16_t)((float)abs_traction_motor_rpm * 0.110216);
        }
        else if (gear_position == SHIFTER_3rd_GEAR)
        {
            groundspeed_cmps = (uint16_t)((float)abs_traction_motor_rpm * 0.191754);
        }
        else
        {
            groundspeed_cmps = 0;
        }

    }
    else
    {
        //
        // Set ground speed to 0 if high voltage is disabled.
        //
        groundspeed_cmps = 0;

    }

    //
    // Decrement the counter, and if it's zero, tally the cm travelled
    //
    if (--time_interval_counter == 0)
    {

        //
        // The number of cm travelled in the last 250 msecs.
        //
        uint16_t cm_travelled_in_250_msecs   = 0;
        time_interval_counter =
            get_TWO_HUNDRED_FIFTY_MSECS();

        // Calculate the distance travelled in the last 250msecs based
        // on current speed
        cm_travelled_in_250_msecs =
            (uint16_t)divide(groundspeed_cmps, 4);

        // Add it to the running total of cm travelled
        running_cm_counter +=
            cm_travelled_in_250_msecs;

        //
        // Remove cm from the running total in meter (100cm) increments.
        //
        while (running_cm_counter >= 100)
        {

            //
            // Remove 1 meter from the running total.
            //
            running_cm_counter -= 100;

            //
            // Add 1 meter to the odometer.
            EEVAR_ODOMETER_METERS += 1;

        }
    }
}

void set_can_switch_led_initial_state()
{


    pku2400_set_change_led_colour(
        ONE,
        SWITCH_BOTTOM_1_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        ONE,
        SWITCH_BOTTOM_2_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        ONE,
        SWITCH_BOTTOM_3_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        ONE,
        SWITCH_BOTTOM_4_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        ONE,
        SWITCH_TOP_1_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        ONE,
        SWITCH_TOP_2_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        ONE,
        SWITCH_TOP_3_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        ONE,
        SWITCH_TOP_4_BIT,
        BROWN);

    //
    // Transmit the change LED colour command.
    //
    pku2400_tx_led_controls(ONE, TX_SEND_EACH_CALL);

}

static void enable_reverse_beeper_becaon()
{

	shifter_position_t direction = get_shifter_direction();

	if(direction == SHIFTER_REVERSE)
	{

		pdm_1_command_reverse_beeper_output(TRUE);
	    pdm_1_command_reverse_lights_output(TRUE);
		pdm_1_command_reverse_beacon_output(TRUE);

	}
	else if (bottom_switch_1_status == TRUE)
	{
		pdm_1_command_reverse_beeper_output(FALSE);
	    pdm_1_command_reverse_lights_output(TRUE);
		pdm_1_command_reverse_beacon_output(FALSE);
	}
	else
	{
		pdm_1_command_reverse_beeper_output(FALSE);
	    pdm_1_command_reverse_lights_output(FALSE);
		pdm_1_command_reverse_beacon_output(FALSE);
	}

}
