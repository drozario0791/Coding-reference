/******************************************************************************
 *
 *        Name: traction_inverter_control.h
 *
 * Description: A module that controls the working of the traction
 *              inverter on the vehicle. The module also takes care of
 *              updating the eeproms for the traction drive inverter
 *              when orchestra is active.
 *
 *              NOTE: This traction_inverter_control_module is written
 *              for traction drive code generated by VISSIM. The
 *              vissim and rtos differ drastically with respect to
 *              eeproms and can messages.
 *
 *      Author: Deepak
 *        Date: Thursday, 05 September 2019
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
#include "skai2_inverter_vissim.h"
#include "cl712_device.h"
#include "traction_inverter_control.h"
#include "orion_control.h"
#include "gears_and_transmission.h"
#include "cvc_input_control.h"
#include "state_machine.h"
#include "can_switches.h"

static uint16_t traction_inverter_enable = FALSE;

//=============================================================================
//
// Static Function Prototypes
//
//=============================================================================
//

static uint16_t inverter_throttle_control();
uint8_t throttle_scaling(uint16_t raw_adc_value);
static void update_traction_inverter_eeproms(device_instances_t device);


/******************************************************************************
 *
 *        Name: traction_inverter_control()
 *
 * Description: The function gathers data from the state machine,
 *              orion BMS and the shifter. Based on the information
 *              obtained, the control function controls the working of
 *              the skai2 traction drive inverter.
 *
 *        Date: Thursday, 05 September 2019
 *
 ******************************************************************************
 */
void traction_inverter_control(
    device_instances_t device)
{

    uint16_t direction = 0;
    uint16_t max_allowable_battery_current = 0;
    uint16_t max_motor_rpm = 0;

    //
    // The function gathers data from the state machine, orion BMS and
    // the shifter. Based on the information obtained, the control
    // function controls the working of the skai2 traction drive
    // inverter.
    //

    //
    // MAXIMUM ALLOWABLE BATTERY CURRENT
    //
    // Is the maximum allowable battery current the inverter can
    // draw. If the current draw is greater than this set value derate
    // the torque output of the motor to keep the current under this
    // limit.
    bool_t brake_test_mode = brake_test_mode_status();

    if(brake_test_mode == TRUE)
    {
        max_allowable_battery_current =
            (EEVAR_TRACTION_SKAI_max_battery_current/2);
    }
    else
    {
        max_allowable_battery_current =
            EEVAR_TRACTION_SKAI_max_battery_current;
    }
    //
    // PACK_STATE_OF_CHARGE
    //
    // The pack state of charge is one of the factors that determines
    // the max allowable regen limit of the motor. When the Pack SOC
    // is above 90 percent, the amount of regen is limited. This is
    // done to stop large amounts of currents sunk into the battery
    // packs.
    //
    uint16_t pack_state_of_charge =  (uint16_t)get_battery_pack_SOC();


    //
    // HIGH CELL VOLTAGE
    //
    // The second factor that would limit the amount of regen
    // avaliable is the high cell voltage of the pack.
    uint16_t pack_high_cell_voltage = get_pack_high_cell_voltage();


    //
    // DETERMINE THE DIRECTION OF OPERATION
    //
    //
    // Get the direction of the shifter. Forward, reverse or neutral.
    //
    shifter_position_t shifter_direction =  get_shifter_direction();
    //
    // Determine the direction of movement based on the position of
    // the shifter.
    //
    if (shifter_direction == SHIFTER_FORWARD)
    {
        //
        // Set direction to forward
        //
        uint16_t forward = 255;
        direction = forward;

    }
    else if (shifter_direction == SHIFTER_REVERSE)
    {
        //
        // Set direction to reverse
        //
        uint16_t reverse = 255;
        // revrse = 255
        direction = reverse;
    }
    else
    {
        //
        // Else the shifter is in neutral - Report neutral (0) only if
        // the absolute value of the traction drive motor RPM is less
        // than 10. This should fix issue #28.
        //
        // Issue #28:
        //
        // It appears that the CVC is turning off the traction drive
        // switches when the transmission is switched into
        // neutral. This is causing a problem when a driver is
        // switching directions. Brad proposed a solution where the
        // traction inverter switches would be turned off if in
        // neutral when the value of the traction motor RPM
        // is less than 10.
        //
        if (skai_get_vissim_motor_rpm(ONE) <= 10)
        {
            // Report neutral.
            // The SKAI2 switches WILL be turned off.
            direction = 0;
        }
        else
        {
            // Do not report neutral.
            // The traction SKAI2 switches WILL NOT be turned off.
            direction = 255;
        }
    }

    //
    // If the gear is in 3rd, reduce the rpm by 150
    //
    shifter_position_t gear = get_shifter_gear_position();

    if ((gear == SHIFTER_3rd_GEAR))
    {
        max_motor_rpm = EEVAR_TRACTION_SKAI_max_motor_rpm - 350;
    }
    else
    {
        max_motor_rpm = EEVAR_TRACTION_SKAI_max_motor_rpm;
    }

    //
    // INVERTER ENABLE/DISABLE
    //
    // This information is obtained from the state machine. The state
    // machine would determine if the inverter must be enabled or
    // disabled.
    //
    state_t sm_state = get_sm_current_state();
    bool_t sm_enable_hv_system =  get_sm_status_enable_hv_systems();

    traction_inverter_enable = (sm_state == READY_TO_DRIVE
                                && !get_sm_precharge_failure_status()
                                && get_sm_precharge_success_status()
                                && (sm_state != POST_CHARGE_IDLE)
                                && (sm_state != CHARGING)
                                && (sm_state != CRITICAL_FAILURE_HV_ON));

    //
    // INSTANTANEOUS BATTERY CURRENT
    //
    // The total current sourced by the battery pack. This is used to
    // derate the torque output if the max current limit of the
    // inverter exceeded.
    //
    uint16_t instantaneous_current =
    		(uint16_t)(abs(get_instantaneous_battery_pack_current()));


    //
    // THROTTLE
    // A value from 0 to 100% that would control the motion of the
    // vehicle.
    //
    uint16_t throttle_percent = inverter_throttle_control(device);


    //
    // MODE SWITCH
    //
    // Set to zero
    uint16_t mode_switch = 0;

    //
    //    The traction inverter control firmware is made up of  various
    //    parameters. These parameters are crucial for the smooth
    //    operation of the motors. The eeproms are responsible for the
    //    tuning of the motor(gains), determining the amount of regen
    //    or motoring torque of the motor and the filters for the
    //    throttle. These gain would be written to the appropriate can
    //    structures when only of the two conditions are met. The
    //    first condition is during first pass (the first time the
    //    program ran) and the second condition is when orchestra is
    //    opened. Update the TX structures when one of the above
    //    conditions have been met.
    //
    update_traction_inverter_eeproms(device);

    //
    // Update values of the Skai2 CAN TX structure
    //
    skai2_set_vissim_tx_msg5_enable_max_current_soc_high_cell(
        device,
        traction_inverter_enable,
        max_allowable_battery_current,
        pack_state_of_charge,
        pack_high_cell_voltage);

    skai2_set_vissim_tx_msg9_battery_current_mode_direction_hyd_throttle(
        device,
        instantaneous_current,
        mode_switch,
        direction,
        throttle_percent);

    skai2_set_vissim_tx_msg12_rpm_outgain_ramptime(
        device,
        0,
        max_motor_rpm,
        1000,
        0);

    //
    // Transmit CAN Messages
    //
    tx_skai2_enable_maxcurrent_soc_highcell(device, EVERY_10MS);

    tx_skai2_vissim_battery_current_mode_direction_hyd_throttle(device, EVERY_10MS);

    tx_skai2_vissim_rpm_settings_outgain_pgainramptime(device, EVERY_50MS);

}


/******************************************************************************
 *
 *        Name: update_traction_inverter_eeproms()
 *
 * Description: The traction inverter control firmware is made up of
 *              various parameters. These parameters are crucial for
 *              the smooth operation of the motors. The eeproms are
 *              responsible for the tuning of the motor(gains),
 *              determining the amount of regen or motoring torque of
 *              the motor and the filters for the throttle. These gain
 *              would be written to the appropriate can structures
 *              when only of the two conditions are met. The first
 *              condition is during first pass (the first time the
 *              program ran) and the second condition is when
 *              orchestra is opened. Update the TX structures when one
 *              of the above conditions have been met.
 *
 *        Date: Thursday, 05 September 2019
 *
 ******************************************************************************
 */
static void update_traction_inverter_eeproms(
    device_instances_t device)
{

    //
    // Update the inverter eeproms when one of the two conditions have
    // been met
    //

    // 1st Condition: A static variable which is used to indicate the
    // first time this program is run.
    //
    static bool_t first_pass = TRUE;

    //
    // 2nd Condition: Check to see if conductor is running. If
    // Conductor is running, EEPROM IO values may get updated Get all
    // necessary IO values continuously and parse them into relevant
    // positions.
    CONDUCTOR_MODES_ conductor_status =
        Get_Conductor_Status();

    if((first_pass == TRUE) || (conductor_status == CONDUCTOR_RUNNING_MODE))
    {

        //
        // Update all the transmit structure elements. When one of
        // the conditions is true.
        //
        //
        skai2_set_vissim_tx_msg1_data_d_gains(
            device,
            EEVAR_TRACTION_SKAI_dp_gain,
	        EEVAR_TRACTION_SKAI_di_gain,
	        0,
	        0);

        skai2_set_vissim_tx_msg2_data_q_gains(
            device,
            EEVAR_TRACTION_SKAI_qp_gain,
            EEVAR_TRACTION_SKAI_qi_gain,
            0,
            0);

        skai2_set_vissim_tx_msg3_data_speed_gains(
            device,
            EEVAR_TRACTION_SKAI_speed_p_gain,
            0,
            EEVAR_TRACTION_SKAI_speed_i_gain,
            0);

        skai2_set_vissim_tx_msg4_data_electrical_angle(
            device,
            EEVAR_TRACTION_SKAI_elec_angle_fwd,
            EEVAR_TRACTION_SKAI_elec_angle_rvs,
            EEVAR_TRACTION_SKAI_elec_fwd_regen,
            EEVAR_TRACTION_SKAI_elec_rev_regen);

        skai2_set_vissim_tx_msg6_motor_scale(
            device,
            EEVAR_TRACTION_SKAI_motor_scale_p1,
            EEVAR_TRACTION_SKAI_motor_scale_p2,
            EEVAR_TRACTION_SKAI_motor_scale_l1,
            EEVAR_TRACTION_SKAI_motor_scale_l2);

        skai2_set_vissim_tx_msg7_regen_scale(
            device,
            EEVAR_TRACTION_SKAI_regen_scale_p1,
            EEVAR_TRACTION_SKAI_regen_scale_p2,
            EEVAR_TRACTION_SKAI_regen_scale_l1,
            EEVAR_TRACTION_SKAI_regen_scale_l2);

        skai2_set_vissim_tx_msg8_throttle_filter_dqf_gains(
            device,
            EEVAR_TRACTION_SKAI_throttle_gain_1,
            EEVAR_TRACTION_SKAI_throttle_gain_2,
            0,
            0);

        skai2_set_vissim_tx_msg10_pedal_calibration_motor_scale(
            device,
            EEVAR_TRACTION_SKAI_throttle_released,
            EEVAR_TRACTION_SKAI_throttle_depressed,
            0,
            EEVAR_TRACTION_SKAI_battery_current_i_gain);

        skai2_set_vissim_tx_msg12_rpm_outgain_ramptime(
            device,
            0,
            EEVAR_TRACTION_SKAI_max_motor_rpm,
            1000,
            0);

        //
        // The program has been run once and all the public setter
        // functions of skai2_inverter_vissim have seen set. Do not
        // update the eeprom values until CONDUCTOR_RUNNING_MODE is
        // enabled.
        //
        first_pass = FALSE;
    }

    //
    // Transmit the CAN Messages associated to the SKAI2 inverter
    // control. Transmit these messages every 100MS.
    //
    tx_skai2_vissim_d_gains(device, EVERY_1000MS);

    tx_skai2_vissim_q_gains(device, EVERY_1000MS);

    tx_skai2_vissim_speed_gains(device, EVERY_1000MS);

    tx_skai2_vissim_electrical_angle(device, EVERY_1000MS);

    tx_skai2_vissim_motor_scale(device, EVERY_1000MS);

    tx_skai2_vissim_regen_scale(device, EVERY_1000MS);

    tx_skai2_vissim_throttle_filter_dq_df_gains(device, EVERY_1000MS);

    tx_skai2_vissim_rpm_settings_outgain_pgainramptime(device, EVERY_1000MS);

    tx_skai2_vissim_pedal_calibration_battery_gain(device, EVERY_1000MS);

}


/******************************************************************************
 *
 *        Name: inverter_throttle_control()
 *
 * Description: Every traction drive gets its throttle command from a
 *              william controls throttle pedal. The pedal is nothing
 *              but a potentiometer. The output from the pedal is a 0
 *              - 5V signal. The signal is fed into port E of the
 *              CVC.
 *q
 *              This module is responsible for the calibration of the
 *              william controls pedal. It is also responsible to
 *              scale the input signal to a value between 0 - 100%,
 *              which can be used by the inverter. This scaled value
 *              is transmitted over can to the inverter driving the
 *              traction motor.
 *
 *
 *        Date: Thursday, 05 September 2019
 *
 ******************************************************************************
 */
static uint16_t inverter_throttle_control()
{

	static uint16_t analog_reading = 0;
    static uint16_t throttle_percent = 0;

    //
    // Call this function to read analog value of input
    // software_input_number is the input software number of the input
    // you want to read (since it is on master module it lines up with
    // the IOMap address) frequency sets the amount of time the input
    // is sampled over.  For example 50 Hz -> 20 msec, 100 Hz, 10
    // msec, 200 Hz 5 msec return the Analog_Values_ structure NOTE:
    // if the frequency is set such that it takes longer to sample the
    // input than the loop time, the previous value will be reported
    // until the sampling is finished.  In some cases the inputs on
    // the module are run through a multiplexer, in these cases having
    // one long sample time will affect all the inputs since the
    // multiplexer cannot be flipped until all the inputs have
    // finished sampling
    //
    //
    // Read the analog result
    //
    analog_reading = IN_E03_throttle_position_sensor_signal;

    //
    // Convert the received analog value to a value usable by the
    // inverter to drive the motor.
    //
    // f(x) = 0.0473260x - 23.663038
    //
    throttle_percent = (uint16_t)((analog_reading * 0.03278) - 22.95);

    if(throttle_percent > 100)
    {

    	//
    	// force throttle percent to 100
    	//
    	throttle_percent = 100;

    }

    if(throttle_percent <= 0)
    {
    	//
    	// force throttle percent to 0
    	//
    	throttle_percent = 0;

    }

    //
    // return the calculated throttle percent
    //
    return throttle_percent;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t traction_inverter_state()
{
    return (bool_t)traction_inverter_enable;
}

#warning "Comment this a bit better"

//=============================================================================
//
// Throttle Scaling: depending on throttle released and depressed values
//
//=============================================================================
//
uint8_t throttle_scaling(uint16_t raw_adc_value)
{

    double raw_adc_input = (((double)raw_adc_value)/4096.0);

//    double throttle_released_value = 625 * 1.0;
//    double throttle_depressed_value = 3846 * 1.0;

    double throttle_released_value = (double)EEVAR_THROTTLE_RELEASED * 1.0;
    double throttle_depressed_value = (double)EEVAR_THROTTLE_DEPRESSED * 1.0;

    double throttle_released = (throttle_released_value /4096.0);
    double throttle_depressed = throttle_depressed_value /4096.0;
    double denominator = 0;

    //
    // Default Settings
    //
    if(throttle_released == 0)
    {
        throttle_released = 0.781;
    }

    if(throttle_depressed == 0)
    {
        throttle_depressed = 0.253;
    }

    //
    // Calculations
    //
    double numerator =
        (double)raw_adc_input - throttle_released - 0.00976565;

    //
    // Calculation for the denominator
    //
    double temp1 =  throttle_released - throttle_depressed;

    if(temp1 < 0.0)
    {
    	temp1 = temp1 * -1.0;
    }
    else
    {
    	temp1 = temp1 * 1.0;
    }
    double temp2 = temp1 - 0.0245;

    if(temp2 > 0.0)
    {
        denominator = temp2;
    }
    else
    {
        denominator = 0.571;
    }

    double output = numerator / denominator;

    uint8_t throttle = (uint8_t)(output * 100);

    if (throttle > 100)
    {
        throttle = 100;
    }

    if (throttle < 0)
    {
        throttle = 0;
    }

    return throttle;

}
