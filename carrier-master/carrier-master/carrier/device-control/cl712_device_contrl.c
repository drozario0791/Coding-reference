/******************************************************************************
 *
 *        Name: cl712_device_control.c
 *
 * Description: Populates the getters of the cl712 device
 *
 *        Date: Sunday, 13 October 2019
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
#include "cl712_device.h"
#include "orion_control.h"
#include "skai2_inverter_vissim.h"
#include "gears_and_transmission.h"
#include "cl712_device_control.h"
#include "hydraulic_system.h"
#include "shinry_dcdc_device.h"
#include "orion_device.h"
#include "state_machine.h"
#include "pdm_device.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "fvt_library.h"
#include "cvc_debug_msgs.h"

#define UNUSED 0x00

static void screen_update_main_screen();
static void screen_update_battery_data();
static void screen_update_pdm_status_selection();
static void screen_update_pdm_1_rear();
static void screen_update_pdm_2_hydraulic_box();
static void screen_update_pdm_3_front();
static void screen_update_traction_inverter();
static void screen_update_hydraulic_inverter();
static void screen_update_system_pressure();
static void update_pump_fan_status();


/******************************************************************************
 *
 *        Name: cl712_device_control()
 *
 * Description:
 *
 *        Date: Sunday, 13 October 2019
 *
 ******************************************************************************
 */
void cl712_device_control()
{

    screen_update_main_screen();
    screen_update_battery_data();

    screen_update_pdm_status_selection();
    screen_update_pdm_1_rear();
    screen_update_pdm_2_hydraulic_box();
    screen_update_pdm_3_front();

    screen_update_traction_inverter();
    update_pump_fan_status();

    screen_update_hydraulic_inverter();
    screen_update_system_pressure();

}


//=============================================================================
//
// screen_update_main()
//
//=============================================================================
//
static void screen_update_main_screen()
{

    uint16_t groundspeed_cmps = 0;

    uint32_t odometer_reading = (uint32_t)EEVAR_ODOMETER_METERS;

    int16_t traction_motor_rpm = (int16_t)skai_get_vissim_motor_rpm(ONE);
    uint16_t abs_motor_rpm = (uint16_t)(abs(traction_motor_rpm));

    //
    // Convert (0 - 100) SOC to (0 - 200)
    //
    uint8_t screen_SOC = (get_battery_pack_SOC() * 2);

    uint8_t direction = (uint8_t)get_shifter_direction();

    uint8_t gear = (uint8_t)get_shifter_gear_position();

    state_t state = get_sm_current_state();

    uint32_t yellow_warning_reason = yellow_warning_message();
    uint32_t crit_fail_hv_off_reason = get_critical_fault_hv_off();
    uint32_t crit_fail_hv_on_reason = get_critical_fault_hv_on();

    // Hourmeter
    EEVAR_HOURMETER_SECONDS_STATUS = (abs(traction_motor_rpm) > 10) ? 1 : 0;;
    EEVAR_TIME_UNTIL_SERVICE_STATUS = (abs(traction_motor_rpm) > 10) ? 1 : 0;;

    //
    // SETTERS
    //
    cl712_set_tx_main_screen_message1(
        ONE,
        odometer_reading,
        EEVAR_HOURMETER_SECONDS_VALUE);

    cl712_set_tx_main_screen_message2(
        ONE,
        EEVAR_TIME_UNTIL_SERVICE_VALUE,
        abs_motor_rpm,
        screen_SOC,
        direction);

    cl712_set_tx_main_screen_message3(
        ONE,
        gear,
        state,
        UNUSED,
        UNUSED,
        yellow_warning_reason);

    cl712_set_tx_main_screen_message4(
        ONE,
        crit_fail_hv_off_reason,
        crit_fail_hv_on_reason);

    //
    // TRANSMITTERS
    //
    cl712_tx_can_message1(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message2(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message3(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message4(
        ONE,
        EVERY_100MS);

    //
    // Sending out debug messages for the carrier
    //
    uint16_t soc_being_displayed = (uint16_t)(((100 / 81) * (screen_SOC / 2)) - (500 / 81));

    // Ground Spped Calculations
    uint16_t ground_speed_of_vehicle = 0;
    if(gear == 3)
    {
        // 1st Gear
        ground_speed_of_vehicle = (uint16_t)((float)abs_motor_rpm * 0.0019043539);
    }
    else if(gear == 4)
    {
        // 2nd Gear
        ground_speed_of_vehicle = (uint16_t)((float)abs_motor_rpm * 0.0039677741);
    }
    else if (gear == 5)
    {
        // 3rd Gear
        ground_speed_of_vehicle = (uint16_t)((float)abs_motor_rpm * 0.0069031280);
    }
    else
    {
        ground_speed_of_vehicle = 0;
    }

    send_debug_can_messages_32bits(0xF003,
                                   EEVAR_HOURMETER_SECONDS_VALUE,
                                   EEVAR_TIME_UNTIL_SERVICE_VALUE);


    send_debug_can_messages_16bits(0xF004,
                                   soc_being_displayed,
                                   ground_speed_of_vehicle,
                                   0x00,
                                   0x00);

    send_debug_can_messages_32bits(0xF005,
                                   odometer_reading,
                                   0x00);



}


//=============================================================================
//
// screen_update_battery_data()
//
//=============================================================================
//
static void screen_update_battery_data()
{

    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    cl712_set_tx_bms_screen_message1(
        ONE,
        orion_get_pack_high_cell_voltage(ONE),
        orion_get_pack_low_cell_voltage(ONE),
        orion_get_pack_high_cell_voltage(ONE) - orion_get_pack_low_cell_voltage(ONE),
        orion_get_instantaneous_pack_voltage(ONE));

    cl712_set_tx_bms_screen_message2(
        ONE,
        (uint16_t)orion_get_pack_high_cell_temperature(ONE),
        orion_get_instantaneous_pack_current(ONE),
        (uint8_t)orion_get_flag2_high_voltage_isolation_fault(ONE),
        (uint8_t)get_sm_precharge_success_status(),
        (uint8_t)get_sm_pos_contactor_status(),
        (uint8_t)get_sm_neg_contactor_status());

    cl712_set_tx_bms_screen_message3(
        ONE,
        orion_get_pack_high_cell_voltage(TWO),
        orion_get_pack_low_cell_voltage(TWO),
        orion_get_pack_high_cell_voltage(TWO) - orion_get_pack_low_cell_voltage(TWO),
        orion_get_instantaneous_pack_voltage(TWO));

    cl712_set_tx_bms_screen_message4(
        ONE,
        (uint16_t)orion_get_pack_high_cell_temperature(TWO),
        orion_get_instantaneous_pack_current(TWO),
        (uint8_t)orion_get_flag2_high_voltage_isolation_fault(TWO),
        (uint8_t)get_sm_precharge_success_status(),
        (uint8_t)get_sm_pos_contactor_status(),
        (uint8_t)get_sm_neg_contactor_status());

    cl712_set_tx_bms_screen_message5(
        ONE,
        orion_get_pack_high_cell_voltage(THREE),
        orion_get_pack_low_cell_voltage(THREE),
        orion_get_pack_high_cell_voltage(THREE) - orion_get_pack_low_cell_voltage(THREE),
        orion_get_instantaneous_pack_voltage(THREE));

    cl712_set_tx_bms_screen_message6(
        ONE,
        (uint16_t)orion_get_pack_high_cell_temperature(THREE),
        orion_get_instantaneous_pack_current(THREE),
        (uint8_t)orion_get_flag2_high_voltage_isolation_fault(THREE),
        (uint8_t)get_sm_precharge_success_status(),
        (uint8_t)get_sm_pos_contactor_status(),
        (uint8_t)get_sm_neg_contactor_status());

    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message5(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message6(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message7(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message8(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message9(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message10(
        ONE,
        EVERY_100MS);

}


//=============================================================================
//
// screen_update_pdm_status_selection
//
//=============================================================================
//
static void screen_update_pdm_status_selection()
{

    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    cl712_set_tx_pdm_status_screen_message1(
        ONE,
        pdm_get_battery_voltage_mv(ONE),
        pdm_get_battery_voltage_mv(TWO),
        pdm_get_battery_voltage_mv(THREE),
        pdm_get_power_supply_status(ONE),
        pdm_get_power_supply_status(TWO));

    cl712_set_tx_pdm_status_screen_message2(
        ONE,
        pdm_get_power_supply_status(THREE),
        UNUSED,
        UNUSED,
        UNUSED,
        UNUSED,
        UNUSED,
        UNUSED,
        UNUSED);

    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message11(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message12(
        ONE,
        EVERY_100MS);

}


//=============================================================================
//
// screen_update_pdm_1_rear()
//
//=============================================================================
//
static void screen_update_pdm_1_rear()
{


    cl712_set_tx_pdm_1_status_screen_message1(
        ONE,
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_1),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_2),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_3),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_4),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_5),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_6),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_7),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_8));

    cl712_set_tx_pdm_1_status_and_current_screen_message2(
        ONE,
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_9),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_10),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_11),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_12),
        pdm_1_reverse_lights_output_current_feedback(),
        pdm_1_brake_lights_output_current_feedback(),
        pdm_1_deck_lights_output_current_feedback(),
        pdm_1_brake_release_solenoid_current_feedback());

    cl712_set_tx_pdm_1_current_screen_message3(
        ONE,
        pdm_1_reverse_beeper_output_current_feedback(),
        pdm_1_reverse_beacon_output_current_feedback(),
        pdm_1_right_spray_solenoid_current_feedback(),
        pdm_1_center_spray_solenoid_current_feedback(),
        pdm_1_left_spray_solenoid_current_feedback(),
        pdm_1_spare_output_current_feedback(),
        pdm_1_channel_11_current_feedback(),
        pdm_1_channel_12__current_feedback());


    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message13(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message14(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message15(
        ONE,
        EVERY_100MS);


}


//=============================================================================
//
// screen_update_pdm_2_hydraulic_box()
//
//=============================================================================
//
static void screen_update_pdm_2_hydraulic_box()
{

    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    cl712_set_tx_pdm_2_status_screen_message1(
        ONE,
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_1),
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_2),
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_3),
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_4),
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_5),
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_6),
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_7),
        pdm_get_digital_output_dignostic(TWO, PDM_CHANNEL_8));

    cl712_set_tx_pdm_2_status_and_current_screen_message2(
        ONE,
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_9),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_10),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_11),
        pdm_get_digital_output_dignostic(ONE, PDM_CHANNEL_12),
        pdm_2_get_start_drain_solenoid_current_feedback(),
        pdm_2_get_aux_hydraulicss_12v_bussbar_current_feedback(),
        pdm_2_get_1st_gear_solenoid_current_feedback(),
        pdm_2_get_2nd_gear_solenoid_current_feedback());

    cl712_set_tx_pdm_2_current_screen_message3(
        ONE,
        pdm_2_get_3rd_gear_solenoid_current_feedback(),
        pdm_2_get_4th_gear_solenoid_current_feedback(),
        pdm_2_get_power_electronics_pump_current_feedback(),
        pdm_2_get_selector_solenoid_current_feedback(),
        pdm_2_get_brake_release_solenoid_current_feedback(),
        pdm_2_get_service_brake_solenoid_current_feedback(),
        pdm_2_get_hydraulic_motor_cooling_pump_current_feedback(),
        pdm_2_get_hydraulic_motor_radiator_cooling_fan_current_feedback());

    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message16(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message17(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message18(
        ONE,
        EVERY_100MS);


}

//=============================================================================
//
// screen_update_pdm_3_front()
//
//=============================================================================
//
static void screen_update_pdm_3_front()
{

    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    cl712_set_tx_pdm_3_status_screen_message1(
        ONE,
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_1),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_2),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_3),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_4),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_5),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_6),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_7),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_8));

    cl712_set_tx_pdm_3_status_and_current_screen_message2(
        ONE,
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_9),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_10),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_11),
        pdm_get_digital_output_dignostic(THREE, PDM_CHANNEL_12),
        pdm_3_get_work_lights_current_feedback(),
        pdm_3_get_beacon_current_feedback(),
        pdm_3_get_head_light_current_feedback(),
        pdm_3_get_horn_current_feedback());

    cl712_set_tx_pdm_3_current_screen_message3(
        ONE,
        pdm_3_get_charger_1_busbar_current_feedback(),
        pdm_3_get_drive_motor_cooling_pump_current_feedback(),
        pdm_3_get_imt_cooling_pump_current_feedback(),
        pdm_3_get_fan_radiator_1_2_current_feedback(),
        pdm_3_get_pump_charger_1_cooling_current_feedback(),
        pdm_3_get_pump_charger_2_cooling_current_feedback(),
        pdm_3_get_main_bussbar_supply_current_feedback(),
        pdm_3_get_hydraulic_fan_solenoid_current_feedback());

    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message19(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message20(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message21(
        ONE,
        EVERY_100MS);


}

//=============================================================================
//
// screen_update_traction_inverter()
//
//=============================================================================
//
static void screen_update_traction_inverter()
{

    uint16_t dc_link_voltage = (skai_get_vissim_DCLink_Voltage(ONE) * 10);

    uint16_t throttle = (skai_get_vissim_throttle(ONE) / 10);

    uint16_t inverter_temp =
        max_of_three_uint16_t_values(skai_get_vissim_dcb_phase1_temperature(ONE),
                                     skai_get_vissim_dcb_phase2_temperature(ONE),
                                     skai_get_vissim_dcb_phase3_temperature(ONE));

    inverter_temp = inverter_temp/10;

    uint16_t pcb_temp = (skai_get_vissim_pcb_temperature(ONE) / 10 );

    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    cl712_set_tx_drive_system_screen_message1(
        ONE,
        dc_link_voltage,
        skai_get_vissim_motor_rpm(ONE),
        (uint8_t)throttle,
        (uint8_t)inverter_temp,
        (uint8_t)pcb_temp,
        (uint8_t)skai_get_vissim_motor_temp_C(ONE));

   cl712_set_tx_drive_system_screen_message2(
        ONE,
        IN_E03_throttle_position_sensor_signal,
        EEVAR_THROTTLE_RELEASED,
        EEVAR_THROTTLE_DEPRESSED);


    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message22(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message23(
        ONE,
        EVERY_100MS);
}

//=============================================================================
//
// screen_update_hydraulic_inverter()
//
//=============================================================================
//
static void screen_update_hydraulic_inverter()
{

    uint16_t dc_link_voltage = (skai_get_vissim_DCLink_Voltage(TWO) * 10);

    uint16_t inverter_temp =
        max_of_three_uint16_t_values(skai_get_vissim_dcb_phase1_temperature(TWO),
                                     skai_get_vissim_dcb_phase2_temperature(TWO),
                                     skai_get_vissim_dcb_phase3_temperature(TWO));

    inverter_temp = inverter_temp/10;

    uint16_t pcb_temp = (skai_get_vissim_pcb_temperature(TWO) / 10);

    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    cl712_set_tx_hydraulic_drive_system_screen_message1(
        ONE,
        dc_link_voltage,
        skai_get_vissim_motor_rpm(TWO),
        (uint8_t)inverter_temp,
        (uint8_t)pcb_temp,
        (uint8_t)skai_get_vissim_motor_temp_C(TWO),
        (uint8_t)0x00);

    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message24(
        ONE,
        EVERY_100MS);


}

//=============================================================================
//
// screen_update_system_pressure()
//
//=============================================================================
//
static void screen_update_system_pressure()
{


    //=============================================================================
    //
    // SETTERS
    //
    //=============================================================================
    //
    uint16_t rear_accum_pressure_psi = get_rear_accum_pressure();
    uint16_t transmission_pressure_psi = get_transmission_pressure() * 10;
    uint16_t front_axil_parking_brake_pressure_psi = get_front_axil_parking_brake_pressure();
    uint16_t main_pressure_psi = main_sys_pressure();

    uint16_t rear_brake_pressure_psi = get_rear_service_brake_pressure();
    uint16_t drive_line_brake_pressure_psi = get_drive_line_brake_pressure();
    uint16_t front_accum_pressure_psi = get_front_accum_pressure();
    uint16_t front_brake_pressure_psi = get_front_service_brake_pressure();


    cl712_set_tx_system_pressures_screen_message1(
        ONE,
        main_pressure_psi,
        front_brake_pressure_psi,
        rear_brake_pressure_psi,
        drive_line_brake_pressure_psi);

    cl712_set_tx_system_pressures_screen_message2(
        ONE,
        front_axil_parking_brake_pressure_psi,
        front_accum_pressure_psi,
        rear_accum_pressure_psi,
        transmission_pressure_psi);

    //=============================================================================
    //
    // Transmist CAN Messages
    //
    //=============================================================================
    //
    cl712_tx_can_message25(
        ONE,
        EVERY_100MS);

    cl712_tx_can_message26(
        ONE,
        EVERY_100MS);

}

//=============================================================================
//
// update_pump_fan_status()
//
//=============================================================================
//
static void update_pump_fan_status()
{

    //
    // Populate Data required to the drive data screen. This data is based on
    // the current drawn by the pump and fans.
    //

    uint8_t traction_pump_status =
        (pdm_3_get_drive_motor_cooling_pump_current_feedback() > 0);

    uint8_t hydraulic_pump_status =
        (pdm_2_get_hydraulic_motor_cooling_pump_current_feedback() > 0);

    uint8_t pe_pump_status =
        ((pdm_2_get_power_electronics_pump_current_feedback() > 0) ||
        (pdm_3_get_imt_cooling_pump_current_feedback() > 0));

    uint8_t pe_fan_status =
        (pdm_2_get_hydraulic_motor_radiator_cooling_fan_current_feedback() > 0);

    uint8_t charger_pump_status =
        ((pdm_3_get_pump_charger_1_cooling_current_feedback() > 0)
         || (pdm_3_get_pump_charger_2_cooling_current_feedback() > 0));

    uint8_t charger_fan_status =
        (pdm_3_get_fan_radiator_1_2_current_feedback() > 0);


    cl712_set_pump_fan_status_message1(
        ONE,
        traction_pump_status,
        hydraulic_pump_status,
        pe_fan_status,
        pe_pump_status,
        charger_fan_status,
        charger_pump_status);

    cl712_tx_can_message27(
        ONE,
        EVERY_100MS);


    //
    // Populate Override Status: data beign sent to the diagnostic screen
    // Data is based on the command to the pdm.
    //
    uint8_t traction_pump_override_status =
        pdm_get_output_channel_command_state(THREE, PDM_CHANNEL_6);

    uint8_t hydraulic_pump_override_status =
        pdm_get_output_channel_command_state(TWO, PDM_CHANNEL_11);

    uint8_t pe_pump_override_status =
        (pdm_get_output_channel_command_state(TWO, PDM_CHANNEL_7) || pdm_get_output_channel_command_state(THREE, PDM_CHANNEL_7));

    uint8_t pe_fan_override_status =
        pdm_get_output_channel_command_state(TWO, PDM_CHANNEL_12);

    uint8_t charger_pump_override_status =
        (pdm_get_output_channel_command_state(THREE,PDM_CHANNEL_9) || pdm_get_output_channel_command_state(THREE,PDM_CHANNEL_10));

    uint8_t charger_fan_override_status =
        pdm_get_output_channel_command_state(THREE, PDM_CHANNEL_8);


    cl712_set_pump_fan_override_status_message1(
        ONE,
        traction_pump_override_status,
        hydraulic_pump_override_status,
        pe_fan_override_status,
        pe_pump_override_status,
        charger_fan_override_status,
        charger_pump_override_status);

    cl712_tx_can_message28(
        ONE,
        EVERY_100MS);




}
