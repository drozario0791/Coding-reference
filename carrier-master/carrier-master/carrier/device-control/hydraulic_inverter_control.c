/******************************************************************************
 *
 *        Name: hydraulic_inverter_control.h
 *
 * Description: A module that controls the working of the
 *              skai2_inverter.
 *
 *      Author: Deepak
 *        Date: Thursday, 29 August 2019
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
#include "hydraulic_inverter_control.h"
#include "orion_control.h"

#define EEVAR_HYDRAULIC_SKAI_pack_soc 0x6400
#define EEVAR_HYDRAULIC_SKAI_high_cell 40000
#define EEVAR_HYDRAULIC_SKAI_motor_scale_p1 1000
#define EEVAR_HYDRAULIC_SKAI_motor_scale_p2 1650
#define EEVAR_HYDRAULIC_SKAI_motor_scale_l1 1000
#define EEVAR_HYDRAULIC_SKAI_motor_scale_l2 1000
#define EEVAR_HYDRAULIC_SKAI_regen_scale_p1 1200
#define EEVAR_HYDRAULIC_SKAI_regen_scale_p2 2150
#define EEVAR_HYDRAULIC_SKAI_regen_scale_l1 700
#define EEVAR_HYDRAULIC_SKAI_regen_scale_l2 900
#define EEVAR_HYDRAULIC_SKAI_throttle_gain_1 1300
#define EEVAR_HYDRAULIC_SKAI_throttle_gain_2 1300
#define EEVAR_HYDRAULIC_SKAI_throttle_released 750
#define EEVAR_HYDRAULIC_SKAI_throttle_depressed 3750

bool_t enable = FALSE;

static uint16_t hydraulic_inverter_enable = FALSE;

//=============================================================================
//
// Static Function Prototypes
//
//=============================================================================
//
static void
update_traction_inverter_eeproms(device_instances_t device);

/******************************************************************************
 *
 *        Name: hydraulic_inverter_control()
 *
 * Description: A function that is responsible to enable/disable the
 *              hydraulic inverter based on various conditions.
 *
 *        Date: Wednesday, 28 August 2019
 *
 ******************************************************************************
 */
void hydraulic_inverter_control(
    device_instances_t device,
    uint16_t inverter_enable)
{

    static uint16_t throttle_percent = 100;
    static uint16_t direction = 0;
    static uint16_t hydraulic_inverter_speed_P_gain = 0;

    //
    // The hydraulic_motor can be enabled or disabled from the screen
    // using the screen over-ride function. This is useful when
    // debugging issues.
    //
    bool_t screen_override_status = FALSE;

    bool_t hydraulic_motor_override_status = FALSE;

    //
    // Set the direction the motor needs to spin
    //
    // The hydraulics on the carrier is the first systems that comes
    // up when precharge is completed. The hydraulic pump in the side
    // cab of the carrier has to be spun in a specific
    // direction. Spinning it in the wrong direction would damage the
    // pump and therefore the direction that would needs to be
    // hard-coded in this section would have to be checked.
    //
    direction = EEVAR_HYDRAULIC_SKAI_motor_direction;

    //
    // Set the rpm of the hydraulic motor.
    //
    uint16_t commanded_rpm = EEVAR_HYDRAULIC_SKAI_max_motor_rpm;

    uint16_t instantaneous_current =
        (uint16_t)abs(get_instantaneous_battery_pack_current());

    //
    // THROTTLE
    // A value from 0 to 100% that would control the motion of the
    // vehicle.
    //
    if(inverter_enable == TRUE)
    {

        throttle_percent = 100;

    }
    else
    {

        throttle_percent = 0;

    }

    //
    // Enable the hydraulic inverter. Check to see of screen over-ride
    // is active. If screen_override is active, enable/disable the
    // motor based on the input from the screen. Else, use the
    // argument provided to enable or disable the motor. Also, check
    // to see if precharge is successful. Only enable the inverter is
    // precharge is successful and high_voltage is ON.
    //
    if((screen_override_status == TRUE))
    {

        //
        // Use the command from the screen over-ride
        //
        hydraulic_inverter_enable =
            hydraulic_motor_override_status;

    }
    else if(screen_override_status == FALSE)
    {

        //
        // Enable/ disable the motor based on the argument provided to
        // the function.
        //
        hydraulic_inverter_enable = inverter_enable;

    }
    else
    {

        //
        // If high_voltage_on is FALSE, disable the inverter.
        //
        hydraulic_inverter_enable = 0;

    }

    //
    // MAXIMUM ALLOWABLE BATTERY CURRENT
    //
    // Is the maximum allowable battery current the inverter can
    // draw. If the current draw is greater than this set value derate
    // the torque output of the motor to keep the current under this
    // limit.
    uint16_t max_allowable_battery_current =
        EEVAR_HYDRAULIC_SKAI_max_battery_current;

    //
    // Speed P gain
    //
    uint16_t hyd_motor_rpm =
        skai_get_vissim_motor_rpm(TWO);

    //
    // Check the rpm and change the speed gain after the rpm reaches a certain limit
    //
    hydraulic_inverter_speed_P_gain = EEVAR_HYDRAULIC_SKAI_speed_p_gain;

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
        hydraulic_inverter_enable,
        max_allowable_battery_current,
        0,
        0);

    skai2_set_vissim_tx_msg9_battery_current_mode_direction_hyd_throttle(
        device,
        instantaneous_current,
        mode_switch,
        direction,
        throttle_percent);

    skai2_set_vissim_tx_msg3_data_speed_gains(
        device,
        hydraulic_inverter_speed_P_gain,
        0,
        EEVAR_HYDRAULIC_SKAI_speed_i_gain,
        0);


    //
    // Transmit CAN Messages
    //
    tx_skai2_enable_maxcurrent_soc_highcell(device, EVERY_20MS);
    tx_skai2_vissim_battery_current_mode_direction_hyd_throttle(device, EVERY_20MS);
    tx_skai2_vissim_speed_gains(device, EVERY_50MS);

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
            EEVAR_HYDRAULIC_SKAI_dp_gain,
	        EEVAR_HYDRAULIC_SKAI_di_gain,
	        0,
	        0);

        skai2_set_vissim_tx_msg2_data_q_gains(
            device,
            EEVAR_HYDRAULIC_SKAI_qp_gain,
            EEVAR_HYDRAULIC_SKAI_qi_gain,
            0,
            0);

        skai2_set_vissim_tx_msg4_data_electrical_angle(
            device,
            EEVAR_HYDRAULIC_SKAI_elec_angle_fwd,
            EEVAR_HYDRAULIC_SKAI_elec_angle_rvs,
            EEVAR_HYDRAULIC_SKAI_elec_angle_fwd,
            EEVAR_HYDRAULIC_SKAI_elec_angle_rvs);

        skai2_set_vissim_tx_msg6_motor_scale(
            device,
            EEVAR_HYDRAULIC_SKAI_motor_scale_p1,
            EEVAR_HYDRAULIC_SKAI_motor_scale_p2,
            EEVAR_HYDRAULIC_SKAI_motor_scale_l1,
            EEVAR_HYDRAULIC_SKAI_motor_scale_l2);

        skai2_set_vissim_tx_msg7_regen_scale(
            device,
            EEVAR_HYDRAULIC_SKAI_regen_scale_p1,
            EEVAR_HYDRAULIC_SKAI_regen_scale_p2,
            EEVAR_HYDRAULIC_SKAI_regen_scale_l1,
            EEVAR_HYDRAULIC_SKAI_regen_scale_l2);

        skai2_set_vissim_tx_msg8_throttle_filter_dqf_gains(
            device,
            EEVAR_HYDRAULIC_SKAI_throttle_gain_1,
            EEVAR_HYDRAULIC_SKAI_throttle_gain_2,
            0,
            0);

        skai2_set_vissim_tx_msg10_pedal_calibration_motor_scale(
            device,
            EEVAR_HYDRAULIC_SKAI_throttle_released,
            EEVAR_HYDRAULIC_SKAI_throttle_depressed,
            0,
            EEVAR_HYDRAULIC_SKAI_battery_current_i_gain);

        skai2_set_vissim_tx_msg12_rpm_outgain_ramptime(
            device,
            0,
            EEVAR_HYDRAULIC_SKAI_max_motor_rpm,
            EEVAR_HYDRAULIC_SKAI_out_gain,
            EEVAR_HYDRAULIC_SKAI_speed_p_gain_time_ramp);

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
    // control. Transmit these messages every 1000MS.
    //
    tx_skai2_vissim_d_gains(device, EVERY_1000MS);

    tx_skai2_vissim_q_gains(device, EVERY_1000MS);

    tx_skai2_vissim_electrical_angle(device, EVERY_1000MS);

    tx_skai2_vissim_motor_scale(device, EVERY_1000MS);

    tx_skai2_vissim_regen_scale(device, EVERY_1000MS);

    tx_skai2_vissim_throttle_filter_dq_df_gains(device, EVERY_1000MS);

    tx_skai2_vissim_rpm_settings_outgain_pgainramptime(device, EVERY_1000MS);

    tx_skai2_vissim_pedal_calibration_battery_gain(device, EVERY_1000MS);

}


//=============================================================================
//
//
//
//=============================================================================
//
bool_t hydraulic_inverter_state()
{

    return (bool_t)hydraulic_inverter_enable;

}
