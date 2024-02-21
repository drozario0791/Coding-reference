/*
 ** ###################################################################
 **     Filename  : User_App.c
 **     Processor : Not applicable
 **     FileFormat: V2.00
 **     Abstract  :
 **          This is where the user application software is located
 **          This function gets called once a loop
 **
 **     (c) Copyright HED,Inc. 2006-2016
 **     HED,Inc.
 **     2120 Constitution Ave.
 **     Hartford, WI 53027
 **     United States
 **     http      : www.hedonline.com
 **     mail      : support@hedonline.com
 ** ###################################################################
 */

/* Include Header Files */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "reserved.h"
#include "Prototypes.h"
#include "User_App.h"
#include "Constants.h"
#include "fvt_general_functions.h"
#include "fvt_vehicle_state_machine.h"
#include "fvt_murphy_ix3212.h"
#include "fvt_orion_bms2.h"
#include "fvt_razorback_charger.h"
#include "fvt_display.h"
#include "fvt_skai2_rtos_inverter.h"
#include "fvt_volt_liquid_cooled_dcdc.h"
#include "vehicle_constants.h"

static OUTPUT_STATUS_ hvil_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
stop_charge_btn_led_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t battery_current);

static OUTPUT_STATUS_
startco_relay_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    bool_t razorback_input_overvoltage,
    bool_t startco_cable_detected);

static OUTPUT_STATUS_
skai_hsp_pwr1_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack_1_pwr_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
skai_hsp_pwr2_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
dcdc_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    bool_t dcdc_working);

static OUTPUT_STATUS_
razorback_pwr_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
dash_pwr_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
skai_hyd_pwr1_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack_2_pwr_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
skai_hyd_pwr2_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
e_stop_led_pwr_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack1_pos_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack1_neg_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack1_pre_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack1_bms_charge_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t pack_current);

static OUTPUT_STATUS_
pack2_pos_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack2_neg_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack2_pre_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack2_bms_charge_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t pack_current);

static OUTPUT_STATUS_
brake_override_out_update(
    const state_machine_output_data_t *state_machine,
    display_output_override_t brake_override);

static OUTPUT_STATUS_
pack3_pos_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack3_neg_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack3_pre_contactor_out_update(
    const state_machine_output_data_t *state_machine);

static OUTPUT_STATUS_
pack3_bms_charge_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t pack_current);

static OUTPUT_STATUS_
welding_plug_contactor_out_update(
    const state_machine_output_data_t *state_machine,
    bool_t razorback_input_overvoltage,
    bool_t welding_cable_detected);

static OUTPUT_STATUS_
dcdc_control_signal_out_update(
    const state_machine_output_data_t *state_machine,
    uint16_t duty_cycle_percent_10);

//
// Settings from vehicle_configuration
//
static bool_t pack_3_present;


/******************************************************************************
 *
 *        Name: User_App()
 *
 * Description: This function gets called by the firmware once every
 *              loop. Loop speed controlled by Orchestra Project
 *              settings
 *
 *      Author: Cyrus
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void User_App(void)
{
    //
    // Code modules
    //
    static murphy_pdm_data_t murphy_pdm_data;
    static orion_bms_2_data_t pack_1_data;
    static orion_bms_2_data_t pack_2_data;
    static orion_bms_2_data_t pack_3_data;
    static razorback_data_t razorback_data;
    static data_from_display_t display_data;
    static data_to_display_t screen_output;
    static skai2_rtos_inverter_data_t hsp_inverter_data;
    static skai2_rtos_inverter_data_t hyd_inverter_data;
    static volt_liquid_cooled_dcdc_data_t dcdc_data;

    //
    // Other local variables
    //
    static int8_t temperature_atms_c;
    static int8_t temperature_wtms_motor_oil_c;
    static int8_t temperature_wtms_hyd_fluid_c;
    static int8_t temperature_wtms_pe_coolant_c;
    //
    // Signed to prevent weird overflow while calculating between s1
    // and s2.
    //
    uint8_t throttle_percentage;

    //
    // State machine i/O
    //
    static state_machine_input_data_t sm_input_data;
    static state_machine_output_data_t sm_output_data;

    //
    // System data structure
    //
    static hed_system_status_t system_status = { .in_safe_mode = FALSE };

    //
    // Timers
    //
    static timer_fvt_t plugged_in_timer;
    static timer_fvt_t motor_pre_oil_timer;
    static timer_fvt_t aux_battery_dying_timer;
    static timer_fvt_t lubricant_pump_running_dry_timer;
    static timer_fvt_t temp_sensor_sample_timer;
    static timer_fvt_t pack_1_cell_delta_timer;
    static timer_fvt_t pack_2_cell_delta_timer;
    static timer_fvt_t pack_3_cell_delta_timer;
    static timer_fvt_t vehicle_not_charging_timer;

    //
    // Boolean helpers
    //
    static bool_t first_run_initialization = TRUE;
    static bool_t hyd_motor_pump_hyst = FALSE;
    static bool_t hsp_motor_pump_hyst = FALSE;
    static bool_t ignition_interlock_flip_flop = FALSE;

    //
    //
    //
    if (first_run_initialization)
    {
        plugged_in_timer = timer_setup(get_THREE_SECONDS(), RISING);
        temp_sensor_sample_timer = timer_setup(get_ONE_SECOND(), RISING);
        motor_pre_oil_timer = timer_setup(get_TEN_SECONDS(), PULSE);
        aux_battery_dying_timer = timer_setup(get_TWO_AND_A_HALF_SECONDS(), RISING);
        lubricant_pump_running_dry_timer = timer_setup(get_THIRTY_SECONDS(), RISING);
        pack_1_cell_delta_timer = timer_setup(get_TEN_SECONDS(), RISING);
        pack_2_cell_delta_timer = timer_setup(get_TEN_SECONDS(), RISING);
        pack_3_cell_delta_timer = timer_setup(get_TEN_SECONDS(), RISING);
        vehicle_not_charging_timer = timer_setup(get_TEN_SECONDS(), RISING);

        first_run_initialization = FALSE;
    }

    //
    // Parse vehicle_configuration
    // 0th bit determines if pack 3 is present.
    //
    pack_3_present = ((vehicle_configuration >> 0) & 0x0001) == 0x0001;

    //
    // Process all code module input data.
    //

    //
    // MURPHY PDM****
    //
    // MURPHY OUTPUTS:
    // Out 1: PE Pump (inverters + charger)
    // Out 2: Hydraulic Motor Cooling Pump (hydraulic motor)
    // Out 3: Hydrostatic Drive Motor Oil (hydrostatic motor)
    // Out 4: PE + Hydraulic Motor Fan (cools coolant for pe loop and hydraulic motor)
    // Out 5: Hydrostatic Drive Motor Fan (cools hydrostatic motor oil)
    // Out 6: Hydraulic Oil Fan (hydraulic oil)
    //

    //////////////////////////////////////////////////////////////////////////////
    //
    // Control pump logic first.
    //
    // Pre oil cycle right after precharge is done to lubricate motors
    // prior to running.
    //
    bool_t pre_oil_motors =
        timer_operate(&motor_pre_oil_timer,
                      sm_output_data.enable_hv_systems);

    //
    // Temperature control to run pumps while motors are hot.
    //
    hsp_motor_pump_hyst =
        hysteresis(hsp_motor_pump_hyst,
                   hsp_inverter_data.motor_temperature_c,
                   70, 60);

    hyd_motor_pump_hyst =
        hysteresis(hyd_motor_pump_hyst,
                   hyd_inverter_data.motor_temperature_c,
                   60, 50);

    //
    // Run hsp cooling pump pump if: inverters enabled OR pre oil
    // cycle OR motor is moving OR fan is running.
    //
    bool_t run_hsp_motor_pump = (hsp_inverter_data.enabled
            || pre_oil_motors
            || (hsp_inverter_data.rpm != 0)
            || hsp_motor_pump_hyst
            || murphy_pdm_data.output_commanded_duty_cycle[4] != 0);

    //
    // Hydraulic motor is on same loop as PE, no need to pre-oil, no
    // need to run when fan is active.
    //
    bool_t run_hyd_motor_pump = (hyd_inverter_data.enabled
            || (hyd_inverter_data.rpm != 0)
            || hyd_motor_pump_hyst);

    //////////////////////////////////////////////////////////////////////////////
    //
    // Control fans next.
    //
    static bool_t hyd_inverter_fan_request = FALSE;
    static bool_t hsp_inverter_fan_request = FALSE;
    static bool_t hyd_motor_fan_request = FALSE;
    static bool_t hsp_motor_fan_request = FALSE;
    static bool_t razorback_fan_request = FALSE;

    hyd_inverter_fan_request =
        hysteresis(hyd_inverter_fan_request,
                   hyd_inverter_data.max_switch_temperature_c,
                   80, 70);

    hsp_inverter_fan_request =
        hysteresis(hsp_inverter_fan_request,
                   hsp_inverter_data.max_switch_temperature_c,
                   80, 70);

    hyd_motor_fan_request =
        hysteresis(hyd_motor_fan_request,
                   hsp_inverter_data.motor_temperature_c,
                   70, 60);

    hsp_motor_fan_request =
        hysteresis(hsp_motor_fan_request,
                   hsp_inverter_data.motor_temperature_c,
                   80, 70);

    razorback_fan_request =
        hysteresis(razorback_fan_request,
                   razorback_data.max_pe_temperature_c,
                   80, 70);

    //
    // Hydraulic oil fan starts turns on at 65, increases linearly to
    // 130.
    //
    uint8_t hyd_oil_fan_duty_cycle = 0;
    if (temperature_wtms_hyd_fluid_c >= 130)
    {
        hyd_oil_fan_duty_cycle = 100;
    }
    else if (temperature_wtms_hyd_fluid_c >= 65)
    {
        hyd_oil_fan_duty_cycle =
            (uint8_t)(round((1.535 * temperature_wtms_hyd_fluid_c) - 9.762));

        if (hyd_oil_fan_duty_cycle < 20)
        {
            hyd_oil_fan_duty_cycle = 20;
        }
    }

    uint8_t murphy_duty_cycles[NUM_MURPHY_OUTPUTS] = {
            ((hsp_inverter_data.enabled ||
              hyd_inverter_data.enabled ||
              razorback_data.power_enable ||
              murphy_pdm_data.output_commanded_duty_cycle[3] != 0) &&
             sm_output_data.enable_hv_systems) ? 100 : 0,
            (run_hyd_motor_pump && sm_output_data.enable_hv_systems) ? 100 : 0,
            (run_hsp_motor_pump && sm_output_data.enable_hv_systems) ? 100 : 0,
            sm_output_data.enable_hv_systems & (
                hyd_inverter_fan_request ||
                hsp_inverter_fan_request ||
                hyd_motor_fan_request ||
                razorback_fan_request) ? 100 : 0,
            sm_output_data.enable_hv_systems & hsp_motor_fan_request ? 100 : 0,
            sm_output_data.enable_hv_systems ? hyd_oil_fan_duty_cycle : 0,
            0, 0, 0, 0, 0, 0};

    //
    // Last run overrides for pumps in the case of the screen
    // overriding them.
    //
    for (uint8_t i = 0; i < NUM_MURPHY_OUTPUTS; i++)
    {
        if (display_data.pdm_override_output[i] == OVERRIDE_OFF)
        {
            murphy_duty_cycles[i] = 0;
        }
        else if (display_data.pdm_override_output[i] == OVERRIDE_ON)
        {
            murphy_duty_cycles[i] = 100;
        }
    }

    //
    // Calculate throttle percentage.
    //
    int32_t throttle_percent_temp =
        divide((divide((acc_pedal_pot_s1_in + acc_pedal_pot_s2_in)*100, 2) - 0x61A*100),
               (0x1127 - 0x61A));

    if (throttle_percent_temp < 0)
    {
        throttle_percent_temp = 0;
    }

    if (throttle_percent_temp > 100)
    {
        throttle_percent_temp = 100;
    }
    throttle_percentage = (uint8_t)throttle_percent_temp;

    //
    // Scale throttle percentage in case of low battery or overheating
    // devices.
    //
    if ((screen_output.soc_percent_2/2.0 <= 15) ||
        (hsp_inverter_data.pcb_temperature_c > 100) ||
        (hyd_inverter_data.pcb_temperature_c > 100))
    {
        throttle_percentage = (uint8_t)divide(throttle_percentage, 2);
    }

    if (screen_output.soc_percent_2/2.0 <= 7)
    {
        throttle_percentage = (uint8_t)divide(throttle_percentage, 2);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Run all code modules
    //
    murphy_pdm_data =
        murphy_pdm_control(murphy_duty_cycles);

    pack_1_data =
        orion_bms_2_battery_control(
            PACK_ONE,
            FALSE,
            pack_1_pwr_out_VALUE == 1000);

    pack_2_data =
        orion_bms_2_battery_control(
            PACK_TWO,
            FALSE,
            pack_2_pwr_out_VALUE == 1000);

    //
    // Check if pack 3 is present.
    //
    // Only run this module if pack3 is there. Otherwise skip it,
    // pack3 will have garbage data.
    //
    if (pack_3_present)
    {
        pack_3_data = orion_bms_2_battery_control(PACK_THREE, FALSE, pack_3_pwr_out_VALUE == 1000);
    }

    //
    // If brake_button_in is INPUT_ON, then the brakes are RELEASED.
    // If the brake button is INPUT_OFF, then the brakes are APPLIED.
    //
    ignition_interlock_flip_flop =
        set_reset(ignition_interlock_flip_flop,
                  ignition_interlock_in ==
                  INPUT_ON && key_on_in ==
                  INPUT_OFF, key_on_in ==
                  INPUT_OFF);

    hsp_inverter_data =
        skai2_rtos_inverter_control(
            HYDROSTATIC_INVERTER,
            sm_output_data.enable_hv_systems &&
            sm_output_data.current_state != CRITICAL_FAILURE_HV_ON &&
            sm_output_data.current_state != CHARGING &&
            sm_output_data.current_state != POST_CHARGE_IDLE &&
            ignition_interlock_flip_flop,
            hsp_idle_rpm,
            hsp_max_rpm,
            throttle_percentage,
            RTOS_SKAI_NEUTRAL,
            pack_3_present ?
            (uint16_t)(max_3(pack_1_data.high_cell_voltage_mv_10,
                             pack_2_data.high_cell_voltage_mv_10,
                             pack_3_data.high_cell_voltage_mv_10)) :
            (uint16_t)(max_2(pack_1_data.high_cell_voltage_mv_10,
                             pack_2_data.high_cell_voltage_mv_10)),
            pack_3_present ?
            (uint16_t)(max_3(pack_1_data.pack_current_a_10,
                             pack_2_data.pack_current_a_10,
                             pack_3_data.pack_current_a_10)) :
            (uint16_t)(max_2(pack_1_data.pack_current_a_10,
                             pack_2_data.pack_current_a_10)),
            FALSE,
            FALSE);

    static uint8_t hydraulic_throttle_ramp = 0;
    static uint16_t hydraulic_ramp_running_variable = 0;

    if (hyd_inverter_data.enabled && hydraulic_throttle_ramp < 100)
    {
        hydraulic_throttle_ramp =
            (uint8_t)(round((100.0 / get_TWO_SECONDS()) *
                            hydraulic_ramp_running_variable));

        if (hydraulic_throttle_ramp > 100)
        {
            hydraulic_throttle_ramp = 100;
        }
        hydraulic_ramp_running_variable++;
    }
    else if (!hyd_inverter_data.enabled)
    {
        hydraulic_throttle_ramp = 0;
        hydraulic_ramp_running_variable = 0;
    }

    hyd_inverter_data = skai2_rtos_inverter_control(
        HYDROSTATIC_INVERTER,
        sm_output_data.enable_hv_systems &&
        sm_output_data.current_state != CRITICAL_FAILURE_HV_ON &&
        sm_output_data.current_state != CHARGING &&
        sm_output_data.current_state != POST_CHARGE_IDLE &&
        ignition_interlock_flip_flop,
        hyd_idle_rpm,
        hyd_max_rpm,
        hydraulic_throttle_ramp,
        RTOS_SKAI_NEUTRAL,
        pack_3_present ?
        (uint16_t)(max_3(pack_1_data.high_cell_voltage_mv_10,
                         pack_2_data.high_cell_voltage_mv_10,
                         pack_3_data.high_cell_voltage_mv_10)) :
        (uint16_t)(max_2(pack_1_data.high_cell_voltage_mv_10,
                         pack_2_data.high_cell_voltage_mv_10)),
        pack_3_present ?
        (uint16_t)(max_3(pack_1_data.pack_current_a_10,
                         pack_2_data.pack_current_a_10,
                         pack_3_data.pack_current_a_10)) :
        (uint16_t)(max_2(pack_1_data.pack_current_a_10,
                         pack_2_data.pack_current_a_10)),
        FALSE,
        FALSE);


    dcdc_data = volt_liquid_cooled_dcdc_control(
        sm_output_data.enable_hv_systems,
        144, dcdc_feedback);

    razorback_data =
        razorback_charger_control(
            sm_output_data.charging_desired,
            240, sm_output_data.charge_current_factor);



    screen_output.soc_percent_2 = pack_3_present ?
              (uint8_t)(divide((pack_1_data.pack_soc_percent_2 + pack_2_data.pack_soc_percent_2 + pack_3_data.pack_soc_percent_2), 3))
            : (uint8_t)(divide((pack_1_data.pack_soc_percent_2 + pack_2_data.pack_soc_percent_2), 2));

    screen_output.vehicle_state = sm_output_data.current_state;

    screen_output.battery_current_a_10 = pack_3_present ?
              pack_1_data.pack_current_a_10 + pack_2_data.pack_current_a_10 + pack_3_data.pack_current_a_10
            : pack_1_data.pack_current_a_10 + pack_2_data.pack_current_a_10;

    screen_output.battery_voltage_v_10 = pack_3_present ?
              (uint16_t)(divide(pack_1_data.pack_inst_voltage_v_10 + pack_2_data.pack_inst_voltage_v_10 + pack_3_data.pack_inst_voltage_v_10, 3))
            : (uint16_t)(divide(pack_1_data.pack_inst_voltage_v_10 + pack_2_data.pack_inst_voltage_v_10, 2));

    screen_output.inverter_1_temp_c = (int8_t)(hsp_inverter_data.max_switch_temperature_c);

    screen_output.inverter_2_temp_c = (int8_t)(hyd_inverter_data.max_switch_temperature_c);

    screen_output.motor_1_temp_c = (int16_t)(hsp_inverter_data.motor_temperature_c);

    screen_output.motor_2_temp_c = (int16_t)(hyd_inverter_data.motor_temperature_c);

    screen_output.charger_temp_c = (int8_t)(razorback_data.max_pe_temperature_c);

    screen_output.high_cell_temp_c = pack_3_present ?
              (int8_t)max_3_s(pack_1_data.high_cell_temp_c, pack_2_data.high_cell_temp_c, pack_3_data.high_cell_temp_c)
            : (int8_t)max_2_s(pack_1_data.high_cell_temp_c, pack_2_data.high_cell_temp_c);

    screen_output.low_cell_temp_c = pack_3_present ?
              (int8_t)min_3_s(pack_1_data.low_cell_temp_c, pack_2_data.low_cell_temp_c, pack_3_data.low_cell_temp_c)
            : (int8_t)min_2_s(pack_1_data.low_cell_temp_c, pack_2_data.low_cell_temp_c);

    screen_output.coolant_temp_1_c = temperature_wtms_pe_coolant_c;

    screen_output.coolant_temp_2_c = temperature_wtms_motor_oil_c;

    screen_output.coolant_temp_3_c = temperature_wtms_hyd_fluid_c;

    screen_output.coolant_temp_4_c = 0;

    screen_output.coolant_temp_5_c = 0;

    screen_output.ambient_air_temp_c = temperature_atms_c;

    screen_output.yellow_warnings = (((pack1_neg_contactor_fdbk_in == INPUT_ON) && (pack1_neg_contactor_out_VALUE != 1000)) << 0)
            | (((pack2_neg_contactor_fdbk_in == INPUT_ON) && (pack2_neg_contactor_out_VALUE != 1000)) << 1)
            | (pack_1_data.non_critical_fault << 2)
            | (pack_2_data.non_critical_fault << 3)
            | ((pack_1_data.high_cell_temp_c >= 50) << 4)
            | ((pack_2_data.high_cell_temp_c >= 50) << 5)
            | (timer_operate(&pack_1_cell_delta_timer, (divide((pack_1_data.high_cell_voltage_mv_10 - pack_1_data.low_cell_voltage_mv_10), 10) > 100)) << 6)
            | (timer_operate(&pack_2_cell_delta_timer, (divide((pack_2_data.high_cell_voltage_mv_10 - pack_2_data.low_cell_voltage_mv_10), 10) > 100)) << 7)
            | (hsp_inverter_data.thermal_warning || (hsp_inverter_data.pcb_temperature_c > 100) << 8)
            | (hyd_inverter_data.thermal_warning || (hyd_inverter_data.pcb_temperature_c > 100) << 9)
            | ((hsp_inverter_data.motor_temperature_c >= 120) << 10)
            | ((hyd_inverter_data.motor_temperature_c >= 85) << 11)
            | ((pack_3_present ? ((pack3_neg_contactor_fdbk_in == INPUT_ON) && (pack1_neg_contactor_out_VALUE != 1000)) : FALSE) << 12)
            | ((pack_3_present ? (pack_3_data.non_critical_fault) : FALSE) << 13)
            | ((pack_3_present ? (pack_3_data.high_cell_temp_c >= 50) : FALSE) << 14)
            | ((pack_3_present ? (timer_operate(&pack_3_cell_delta_timer, (divide((pack_3_data.high_cell_voltage_mv_10 - pack_3_data.low_cell_voltage_mv_10), 10) > 100))) : FALSE) << 15);

    screen_output.misc_status = (uint8_t)(
            ((timer_operate(&vehicle_not_charging_timer, sm_output_data.charging_desired && (screen_output.battery_current_a_10) > 0)) << 0)
            | ((brake_override_out_VALUE == 0) << 1)
            | ((sm_output_data.current_state == CHARGING) && (!sm_output_data.charging_desired) << 2));

    for (uint8_t i = 0; i < NUM_MURPHY_OUTPUTS; i++)
    {
        screen_output.pdm_output_currents[i] = murphy_pdm_data.output_current[i];
        screen_output.pdm_output_statuses[i] = murphy_pdm_data.output_diagnostic[i];
    }
    display_data = display_control(&screen_output);

    //////////////////////////////////////////////////////////////////////////////
    //
    // Run the state machine
    // Get all State Machine Input Data
    //
    sm_input_data.master_closed = (master_discon_open_in == INPUT_OFF) && (switched_battery_voltage_in > 5000);

    sm_input_data.e_stop = (!(dash_e_stop_in == INPUT_ON) || !(charger_e_stop_in == INPUT_ON))
                    && ((sm_output_data.current_state != ZERO_ENERGY) && sm_input_data.master_closed);

    sm_input_data.ignition_on = key_on_in == INPUT_ON;

    sm_input_data.plugged_in = timer_operate(&plugged_in_timer,
               ((75 < charge_cable_sensor_in) && (charge_cable_sensor_in < 125))
            || ((175 < charge_cable_sensor_in) && (charge_cable_sensor_in < 225)));

    sm_input_data.in_neutral = TRUE; //TODO fix this

    sm_input_data.critical_fault_hv_off =
        (uint32_t)((((pack1_pos_contactor_fdbk_in == INPUT_ON) &&
                     (pack1_pos_contactor_out_VALUE != 1000)) << 0) | //pack 1 + contactor welded
                   (((pack2_pos_contactor_fdbk_in == INPUT_ON) &&
                     (pack2_pos_contactor_out_VALUE != 1000)) << 1) | // pack 2 + contactor welded
                   ((timer_operate(&aux_battery_dying_timer, (switched_battery_voltage_in <= 11000)
                                   && sm_output_data.enable_hv_systems)) << 2) | // 12V battery dying while dcdc should be running
                   ((switched_battery_voltage_in < 8000) << 3) | //12v battery gotten too low to properly control contactors
                   (pack_1_data.critical_fault << 4) | // critical battery fault
                   (pack_2_data.critical_fault << 5) | // critical battery fault
                   (hsp_inverter_data.critical_fault << 6) | //critical inverter fault
                   (hyd_inverter_data.critical_fault << 7) | //critical inverter fault
                   (hsp_inverter_data.can_receive_timeout << 8) |
                   (hyd_inverter_data.can_receive_timeout << 9) |
                   (pack_1_data.can_receive_timeout << 10) |
                   (pack_2_data.can_receive_timeout << 11) |
                   ((system_status.can1_status == CAN_ERROR_STATUS) << 12) |  // CAN fault
                   ((system_status.can1_status == CAN_BUSOFF_STATUS) << 13) | // CAN fault
                   ((system_status.can2_status == CAN_ERROR_STATUS) << 14) |  // CAN fault
                   ((system_status.can2_status == CAN_BUSOFF_STATUS) << 15) | // CAN fault
                   ((system_status.can3_status == CAN_ERROR_STATUS) << 16) |  // CAN fault
                   ((system_status.can3_status == CAN_BUSOFF_STATUS) << 17) | // CAN fault
                   (!system_status.io_modules_in_running_state << 18) |  // IO Module has faulted
                   ((system_status.master_module_state != MODULE_RUNNING_STATE) << 19) |  // Master module has faulted
                   ((system_status.number_of_system_errors != 0) << 20) |  // There is a system error
                   (sm_output_data.contactors.precharge_failure << 21) | // Precharge failed
            (((hvil_out_VALUE == 1000) && (hvil_out_STATUS == STATUS_OPEN)) << 22)) //hvil failure
            | ((pack_3_present ? ((pack3_pos_contactor_fdbk_in == INPUT_ON) &&
                                  (pack3_pos_contactor_out_VALUE != 1000)) : FALSE) << 23) //pack 3 + contactor welded
            | (pack_3_present ? pack_3_data.critical_fault : FALSE << 24) // pack 3 critical fault
            | (pack_3_present ? pack_3_data.can_receive_timeout : FALSE << 25); // pack 3 can timeout

    sm_input_data.critical_fault_hv_on =
        (uint32_t)((hsp_inverter_data.thermal_critical << 0) |
                   (hyd_inverter_data.thermal_critical << 1) |
                   ((hsp_inverter_data.motor_temperature_c > 150) << 2) |
                   ((hyd_inverter_data.motor_temperature_c > 95) << 3) |
                   (timer_operate(&lubricant_pump_running_dry_timer,
                    murphy_pdm_data.output_running_dry[1] ||
                             murphy_pdm_data.output_running_dry[2])) << 4);

    sm_input_data.stop_charging_button = (stop_charge_btn_in == INPUT_ON);

    sm_input_data.brake_test_mode = FALSE; //not used in this vehicle

    sm_input_data.diagnostic_mode = diag_mode_in == INPUT_ON;

    sm_input_data.max_cell_voltage_uv = pack_3_present ?
              max_3(pack_1_data.high_cell_voltage_mv_10,
                    pack_2_data.high_cell_voltage_mv_10,
                    pack_3_data.high_cell_voltage_mv_10)*100
            : max_2(pack_1_data.high_cell_voltage_mv_10,
                    pack_2_data.high_cell_voltage_mv_10)*100;

    sm_input_data.min_cell_voltage_uv = pack_3_present ?
              min_3(pack_1_data.low_cell_voltage_mv_10,
                    pack_2_data.low_cell_voltage_mv_10,
                    pack_3_data.low_cell_voltage_mv_10)*100
            : min_2(pack_1_data.low_cell_voltage_mv_10,
                    pack_2_data.low_cell_voltage_mv_10)*100;

    sm_input_data.battery_soc = pack_3_present ?
              (uint8_t)(divide(pack_1_data.pack_soc_percent_2 +
                               pack_2_data.pack_soc_percent_2 +
                               pack_3_data.pack_soc_percent_2, 6))
            : (uint8_t)(divide(pack_1_data.pack_soc_percent_2 +
                               pack_2_data.pack_soc_percent_2, 4));

    sm_input_data.dc_bus_voltage_v = (hsp_inverter_data.dc_link_voltage_v);

    sm_input_data.battery_voltage_v = pack_3_present ?
              (uint16_t)(divide(pack_1_data.pack_inst_voltage_v_10 +
                                pack_2_data.pack_inst_voltage_v_10 +
                                pack_3_data.pack_inst_voltage_v_10, 30))
            : (uint16_t)(divide(pack_1_data.pack_inst_voltage_v_10 +
                                pack_2_data.pack_inst_voltage_v_10, 20));

    sm_input_data.traction_rpm = hsp_inverter_data.rpm;

    sm_input_data.setting_max_charging_cell_voltage_uv = max_charging_cell_voltage_uv;

    sm_output_data = run_state_machine(&sm_input_data);

    //
    // Check status of HED system as a whole
    //
    system_status = get_system_status();

    //
    // If conductor is connected, set the eeprom values from the IO
    // map to their proper locations.
    //
    if (system_status.conductor_mode == CONDUCTOR_RUNNING_MODE)
    {
        ;
    }

    //
    // Sample temperature sensors.
    //
    if (timer_operate(&temp_sensor_sample_timer, TRUE))
    {
        static uint8_t i = 0;
        if (i == 1)
        {
            uint16_t resistance_atms =
                murphy_pdm_convert_analog_input_to_resistance(murphy_pdm_data.analog_input_1);
            temperature_atms_c =
                (int8_t)h_atms_temp_sensor_r_to_temp_c(resistance_atms);
        }
        else if (i == 3)
        {
            temperature_wtms_pe_coolant_c =
                (int8_t)h_wtms_temp_sensor_r_to_temp_c(temp_sensor_pe_coolant_in);
        }
        else if (i == 5)
        {
            temperature_wtms_motor_oil_c =
                (int8_t)h_wtms_temp_sensor_r_to_temp_c(temp_sensor_motor_oil_in);
        }
        else if (i == 7)
        {
            temperature_wtms_hyd_fluid_c =
                h_wtms_temp_sensor_r_to_temp_c(temp_sensor_hyd_fluid_in);
        }
        else if (i == 9)
        {
            i = 0;
            timer_operate(&temp_sensor_sample_timer, FALSE);
        }
        i++;
    }

    send_stock_can_messages(
        temperature_atms_c,
        (uint8_t)(throttle_percentage),
        hsp_inverter_data.percent_load,
        hsp_inverter_data.rpm,
        divide((hsp_max_rpm - hsp_idle_rpm)*throttle_percentage, 100) + hsp_idle_rpm,
        temperature_wtms_motor_oil_c);

    //
    // Control all outputs based on state machine.
    //
    stop_charge_btn_led_out_update(
        &sm_output_data,
        pack_1_data.pack_current_a_10 + pack_2_data.pack_current_a_10);

    hvil_out_update(&sm_output_data);

    brake_override_out_update(
        &sm_output_data, display_data.brake_override);

    startco_relay_pwr_out_update(
        &sm_output_data, razorback_data.input_overvoltage,
        (75 < charge_cable_sensor_in) && (charge_cable_sensor_in < 125));

    skai_hsp_pwr1_out_update(&sm_output_data);

    pack_1_pwr_out_update(&sm_output_data);

    skai_hsp_pwr2_out_update(&sm_output_data);

    dcdc_pwr_out_update(&sm_output_data,
                        dcdc_data.working);

    razorback_pwr_out_update(&sm_output_data);

    dash_pwr_out_update(&sm_output_data);

    skai_hyd_pwr1_out_update(&sm_output_data);

    pack_2_pwr_out_update(&sm_output_data);

    skai_hyd_pwr2_out_update(&sm_output_data);

    e_stop_led_pwr_out_update(&sm_output_data);

    pack1_pos_contactor_out_update(&sm_output_data);

    pack1_neg_contactor_out_update(&sm_output_data);

    pack1_pre_contactor_out_update(&sm_output_data);

    pack1_bms_charge_pwr_out_update(
        &sm_output_data,
        (int16_t)divide(pack_1_data.pack_current_a_10, 10));

    pack2_pos_contactor_out_update(&sm_output_data);

    pack2_neg_contactor_out_update(&sm_output_data);

    pack2_pre_contactor_out_update(&sm_output_data);

    pack2_bms_charge_pwr_out_update(
        &sm_output_data,
        (int16_t)divide(pack_2_data.pack_current_a_10, 10));

    if (pack_3_present)
    {
        pack3_pos_contactor_out_update(&sm_output_data);

        pack3_neg_contactor_out_update(&sm_output_data);

        pack3_pre_contactor_out_update(&sm_output_data);

        pack3_bms_charge_pwr_out_update(
            &sm_output_data,
            (int16_t)divide(pack_3_data.pack_current_a_10, 10));
    }

    welding_plug_contactor_out_update(
        &sm_output_data,
        razorback_data.input_overvoltage,
        (175 < charge_cable_sensor_in) &&
        (charge_cable_sensor_in < 225));

    dcdc_control_signal_out_update(
        &sm_output_data,
        dcdc_data.desired_duty_cycle_percent_10);

    status_led_control(
        sm_input_data.critical_fault_hv_off != 0);
}



/******************************************************************************
 *
 *        Name: get_system_status()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
hed_system_status_t get_system_status()
{
    static hed_system_status_t system_status;

    //
    // Get all system status information.
    //
    system_status.firmware_version =
        Get_Firmware_Version(Module1);

    system_status.application_version =
        Get_Application_Version();

    system_status.number_of_system_errors =
        GetNumberOfSystemErrors();

    system_status.system_error =
        GetSystemError(0);

    system_status.conductor_mode =
        Get_Conductor_Status();

    system_status.conductor_access_level =
        Get_Conductor_Access_Level();

    system_status.can1_status =
        Check_For_CAN_Status(CAN1);

    system_status.can2_status =
        Check_For_CAN_Status(CAN2);

    system_status.can3_status =
        Check_For_CAN_Status(CAN3);

    system_status.can1_baudrate =
        GetCanBaudRate(Module1, CAN1);

    system_status.can2_baudrate =
        GetCanBaudRate(Module1, CAN2);

    system_status.can3_baudrate =
        GetCanBaudRate(Module1, CAN3);

    system_status.can1_missed_status =
        Get_CAN_Missing_Messages_Status(CAN1);

    system_status.can2_missed_status =
        Get_CAN_Missing_Messages_Status(CAN2);

    system_status.can3_missed_status =
        Get_CAN_Missing_Messages_Status(CAN3);

    system_status.master_module_state =
        (MODULE_STATE_) (Module1_STATUS);

    system_status.io_modules_in_running_state =
        ((Module2_STATUS == MODULE_RUNNING_STATE) &&
         (Module3_STATUS == MODULE_RUNNING_STATE)) &&
        (pack_3_present ? (Module4_STATUS == MODULE_RUNNING_STATE) : TRUE);

    system_status.in_safe_mode =
        set_reset(system_status.in_safe_mode,
                  switched_battery_voltage_in <= 8000,
                  switched_battery_voltage_in >= 12000);

    return system_status;

}


/******************************************************************************
 *
 *        Name: stop_charge_btn_led_out_update()
 *
 * Description:
 *
 *      Author: Cyrus
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
stop_charge_btn_led_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t battery_current)
{
    if (battery_current < 0 && state_machine->charging_desired)
    {
        Update_Output(stop_charge_btn_led_out, OUTPUT_ON_FLASH);
    }
    else if (state_machine->current_state == CHARGING)
    {
        Update_Output(stop_charge_btn_led_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(stop_charge_btn_led_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(stop_charge_btn_led_out_STATUS);
}


/******************************************************************************
 *
 *        Name: hvil_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
hvil_out_update(
    const state_machine_output_data_t *state_machine)
{
    if ((state_machine->current_state != ZERO_ENERGY) &&
        (state_machine->current_state != MASTER_CLOSED_NO_HV) &&
        (state_machine->current_state != CRITICAL_FAILURE_HV_OFF) &&
        (state_machine->current_state != E_STOP))
    {
        Update_Output(hvil_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(hvil_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(hvil_out_STATUS);
}


/******************************************************************************
 *
 *        Name: brake_override_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
brake_override_out_update(
    const state_machine_output_data_t *state_machine,
    display_output_override_t brake_override)
{
    if (brake_override == OVERRIDE_ON)
    {
        Update_Output(brake_override_out, OUTPUT_OFF); //turn the brakes on
    }
    else if (brake_override == OVERRIDE_OFF)
    {
        Update_Output(brake_override_out, OUTPUT_ON); //turn the brakes off
    }
    else if (!state_machine->brakes_desired)
    {
        Update_Output(brake_override_out, OUTPUT_ON); //turn the brakes off
    }
    else
    {
        Update_Output(brake_override_out, OUTPUT_OFF); //turn the brakes on
    }

    return (OUTPUT_STATUS_)(brake_override_out_STATUS);
}


/******************************************************************************
 *
 *        Name: startco_relay_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
startco_relay_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    bool_t razorback_input_overvoltage,
    bool_t startco_cable_detected)
{
    static timer_fvt_t contactor_open_timer;
    static bool_t first_run_init = TRUE;

    if (first_run_init)
    {
        first_run_init = FALSE;
        contactor_open_timer = timer_setup(
            get_SEVEN_HUNDRED_MSECS(), FALLING);
    }

    // This timer allows the contactor to be opened after
    // charging_desired has gone false (and charging has stopped).
    bool_t close_contactor =
        timer_operate(&contactor_open_timer,
                      state_machine->charging_desired);

    if (close_contactor &&
        !razorback_input_overvoltage &&
        startco_cable_detected)
    {
        Update_Output(startco_relay_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(startco_relay_pwr_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(startco_relay_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: skai_hsp_pwr1_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
skai_hsp_pwr1_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(skai_hsp_pwr1_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(skai_hsp_pwr1_out, OUTPUT_OFF);
    }
    return (OUTPUT_STATUS_)(skai_hsp_pwr1_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack_1_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack_1_pwr_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(pack_1_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack_1_pwr_out, OUTPUT_OFF);
    }
    return (OUTPUT_STATUS_)(pack_1_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: skai_hsp_pwr2_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
skai_hsp_pwr2_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(skai_hsp_pwr2_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(skai_hsp_pwr2_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(skai_hsp_pwr2_out_STATUS);
}


/******************************************************************************
 *
 *        Name: dcdc_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
dcdc_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    bool_t dcdc_working)
{
    static uint32_t pulse_offset_counter = 1;
    static bool_t first_run = TRUE;
    static timer_fvt_t dcdc_kickstart_timer;
    bool_t pulse_output = FALSE;

    if (first_run)
    {
        first_run = FALSE;
        dcdc_kickstart_timer =
            timer_setup(get_TWO_HUNDRED_FIFTY_MSECS(), PULSE);
    }

    if (pulse_offset_counter < get_FOUR_SECONDS())
    {
        pulse_offset_counter++;
    }
    else
    {
        pulse_offset_counter = 0;
    }

    pulse_output =
        timer_operate(&dcdc_kickstart_timer,
                      pulse_offset_counter == 0);

    if (state_machine->enable_hv_systems &&
        !dcdc_working && pulse_output)
    {
        Update_Output(dcdc_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(dcdc_pwr_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(dcdc_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: razorback_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
razorback_pwr_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(razorback_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(razorback_pwr_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(razorback_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: dash_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
dash_pwr_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(dash_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(dash_pwr_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(dash_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: skai_hyd_pwr1_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
skai_hyd_pwr1_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(skai_hyd_pwr1_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(skai_hyd_pwr1_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(skai_hyd_pwr1_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack_2_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack_2_pwr_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(pack_2_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack_2_pwr_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack_2_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: skai_hyd_pwr2_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
skai_hyd_pwr2_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state != ZERO_ENERGY &&
        state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(skai_hyd_pwr2_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(skai_hyd_pwr2_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(skai_hyd_pwr2_out_STATUS);
}


/******************************************************************************
 *
 *        Name: e_stop_led_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
e_stop_led_pwr_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->current_state == E_STOP)
    {
        Update_Output(e_stop_led_pwr, OUTPUT_ON_FLASH);
    }
    else if (state_machine->current_state != ZERO_ENERGY &&
             state_machine->current_state != MASTER_CLOSED_NO_HV)
    {
        Update_Output(e_stop_led_pwr, OUTPUT_ON);
    }
    else
    {
        Update_Output(e_stop_led_pwr, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(e_stop_led_pwr_STATUS);
}


/******************************************************************************
 *
 *        Name: pack1_pos_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack1_pos_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.pos_contactor)
    {
        Update_Output(pack1_pos_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack1_pos_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack1_pos_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack1_neg_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack1_neg_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.neg_contactor)
    {
        Update_Output(pack1_neg_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack1_neg_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack1_neg_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack1_pre_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack1_pre_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.pre_contactor)
    {
        Update_Output(pack1_pre_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack1_pre_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack1_pre_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack1_bms_charge_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static OUTPUT_STATUS_
pack1_bms_charge_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t pack_current)
{
    if (((!state_machine->charging_desired ||
          abs(pack_current) < 5) &&
         (state_machine->current_state == CHARGING ||
          state_machine->current_state == POST_CHARGE_IDLE)) ||
        state_machine->current_state == DIAGNOSTIC_MODE)
    {
        Update_Output(pack1_bms_charge_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack1_bms_charge_pwr_out, OUTPUT_OFF);
    }
    return (OUTPUT_STATUS_)(pack1_bms_charge_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack2_pos_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack2_pos_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.pos_contactor)
    {
        Update_Output(pack2_pos_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack2_pos_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack2_pos_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack2_neg_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack2_neg_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.neg_contactor)
    {
        Update_Output(pack2_neg_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack2_neg_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack2_neg_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack2_pre_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
OUTPUT_STATUS_
pack2_pre_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{

    if (state_machine->contactors.pre_contactor)
    {
        Update_Output(pack2_pre_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack2_pre_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack2_pre_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack2_bms_charge_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
pack2_bms_charge_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t pack_current)
{
    if (((!state_machine->charging_desired || abs(pack_current) < 5) &&
         (state_machine->current_state == CHARGING ||
          state_machine->current_state == POST_CHARGE_IDLE)) ||
        state_machine->current_state == DIAGNOSTIC_MODE)
    {
        Update_Output(pack2_bms_charge_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack2_bms_charge_pwr_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack2_bms_charge_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack3_pos_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
pack3_pos_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.pos_contactor)
    {
        Update_Output(pack3_pos_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack3_pos_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack3_pos_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack3_neg_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
pack3_neg_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.neg_contactor)
    {
        Update_Output(pack3_neg_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack3_neg_contactor_out, OUTPUT_OFF);
    }
    return (OUTPUT_STATUS_)(pack3_neg_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack3_pre_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
pack3_pre_contactor_out_update(
    const state_machine_output_data_t *state_machine)
{
    if (state_machine->contactors.pre_contactor)
    {
        Update_Output(pack3_pre_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack3_pre_contactor_out, OUTPUT_OFF);
    }
    return (OUTPUT_STATUS_)(pack3_pre_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: pack3_bms_charge_pwr_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
pack3_bms_charge_pwr_out_update(
    const state_machine_output_data_t *state_machine,
    int16_t pack_current)
{
    if (((!state_machine->charging_desired ||
          abs(pack_current) < 5) &&
         (state_machine->current_state == CHARGING ||
          state_machine->current_state == POST_CHARGE_IDLE)) ||
        state_machine->current_state == DIAGNOSTIC_MODE)
    {
        Update_Output(pack3_bms_charge_pwr_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(pack3_bms_charge_pwr_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(pack3_bms_charge_pwr_out_STATUS);
}


/******************************************************************************
 *
 *        Name: welding_plug_contactor_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
welding_plug_contactor_out_update(
    const state_machine_output_data_t * state_machine,
    bool_t razorback_input_overvoltage,
    bool_t welding_cable_detected)
{
    static timer_fvt_t contactor_open_timer;
    static bool_t first_run_init = TRUE;

    if (first_run_init)
    {
        first_run_init = FALSE;
        contactor_open_timer =
            timer_setup(get_SEVEN_HUNDRED_MSECS(), FALLING);
    }

    //
    // This timer allows the contactor to be opened after
    // charging_desired has gone false (and charging has stopped).
    //
    bool_t close_contactor =
        timer_operate(&contactor_open_timer,
                      state_machine->charging_desired);

    if (close_contactor &&
        !razorback_input_overvoltage &&
        welding_cable_detected)
    {
        Update_Output(welding_plug_contactor_out, OUTPUT_ON);
    }
    else
    {
        Update_Output(welding_plug_contactor_out, OUTPUT_OFF);
    }

    return (OUTPUT_STATUS_)(welding_plug_contactor_out_STATUS);
}


/******************************************************************************
 *
 *        Name: dcdc_control_signal_out_update()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static
OUTPUT_STATUS_
dcdc_control_signal_out_update(
    const state_machine_output_data_t *state_machine,
    uint16_t duty_cycle_percent_10)
{
    if (state_machine->enable_hv_systems)
    {
        Update_Output(dcdc_control_signal_out,
                      duty_cycle_percent_10, FALSE);
    }
    else
    {
        Update_Output(dcdc_control_signal_out, 0, FALSE);
    }
    return (OUTPUT_STATUS_)(dcdc_control_signal_out_STATUS);
}


/******************************************************************************
 *
 *        Name: status_led_control()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void
status_led_control(
    bool_t critical_error)
{
    static uint16_t led_count = 0;

    if (led_count < ConvertMsecToLoops(500))
    {
        if (critical_error)
        {
            // if there are errors, LED is red
            Update_Output(status_led_red_out, OUTPUT_ON);
            Update_Output(status_led_green_out, OUTPUT_OFF);
        }
        else
        {
            //
            // Green is on for both yellow and green (only time it
            // isn't on is for red).
            //
            Update_Output(status_led_green_out, OUTPUT_ON);

            if (switched_battery_voltage_in < 12000)
            {
                //
                // If battery is below 12V, turn on red as well as
                // green (which is yellow).
                //
                Update_Output(status_led_red_out, OUTPUT_ON);
            }
            else
            {
                Update_Output(status_led_red_out, OUTPUT_OFF);
            }
        }

        led_count++;
    }
    else if (led_count < ConvertMsecToLoops(1000)) // LED count is not below 500, it should be OFF
    {
        // turn it off
        Update_Output(status_led_green_out, OUTPUT_OFF);
        Update_Output(status_led_red_out, OUTPUT_OFF);
        led_count++;                                // count up
    }

    // if the counter makes it to 1000 ms (one second), reset it to zero.
    if (led_count >= ConvertMsecToLoops(1000))
    {
        led_count = 0;
    }
}

void
send_stock_can_messages(
    int16_t ambient_air_temp_c,
    uint8_t throttle_pedal_percentage,
    uint8_t hsp_inverter_percent_load,
    int16_t hsp_inverter_rpm,
    int16_t hsp_inverter_target_rpm,
    int16_t hsp_motor_coolant_temp_c)
{
    static bool_t first_time_running = TRUE;

    static fvt_can_message_tx_t amb;
    static fvt_can_message_tx_t propb_50;
    static fvt_can_message_tx_t lfe1;
    static fvt_can_message_tx_t eec2;
    static fvt_can_message_tx_t efl_p1;
    static fvt_can_message_tx_t eec1;
    static fvt_can_message_tx_t egf1;
    static fvt_can_message_tx_t ic1;
    static fvt_can_message_tx_t eec3;
    static fvt_can_message_tx_t at1ig2;
    static fvt_can_message_tx_t eoi;
    static fvt_can_message_tx_t a1doc1;
    static fvt_can_message_tx_t shutdn;
    static fvt_can_message_tx_t at1img;
    static fvt_can_message_tx_t dpf1s;
    static fvt_can_message_tx_t at1og2;
    static fvt_can_message_tx_t lfc1;
    static fvt_can_message_tx_t efl_p2;
    static fvt_can_message_tx_t et1;
    static fvt_can_message_tx_t propb_51;
    static fvt_can_message_tx_t dm01;
    static fvt_can_message_tx_t dpfc1;
    static fvt_can_message_tx_t vep1;
    static fvt_can_message_tx_t oi;

    if (first_time_running)
    {
        first_time_running = FALSE;

        amb = fvt_can_tx_init(8, EXTENDED, 0x18FEF500, get_ONE_SECOND());
        amb.message.data[0] = 0xC7; //barometric pressure, put in dummy value to match previous trace
        amb.message.data[1] = 0xFF;
        amb.message.data[2] = 0xFF;
        amb.message.data[5] = 0x3E; //engine air intake temp (C) +40 dummy
        amb.message.data[6] = 0xFF;
        amb.message.data[7] = 0xFF;
        propb_50 = fvt_can_tx_init(8, EXTENDED, 0x18FF5000, get_TEN_MSECS());
        lfe1 = fvt_can_tx_init(8, EXTENDED, 0x18FEF200, get_ONE_HUNDRED_MSECS());
        lfe1.message.data[0] = 0x32; //engine fuel rate dummy value
        lfe1.message.data[1] = 0x0;  //engine fuel rate dummy value
        lfe1.message.data[2] = 0xFF;
        lfe1.message.data[3] = 0xFF;
        lfe1.message.data[4] = 0xFF;
        lfe1.message.data[5] = 0xFF;
        lfe1.message.data[6] = 0x53;  //engine throttle value position dummy value
        lfe1.message.data[7] = 0xFF;
        eec2 = fvt_can_tx_init(8, EXTENDED, 0x0CF00300, get_FIFTY_MSECS());
        eec2.message.data[0] = 0xFF;
        eec2.message.data[3] = 0xFF;
        eec2.message.data[4] = 0xFF;
        eec2.message.data[5] = 0xFF;
        eec2.message.data[6] = 0xFF;
        eec2.message.data[7] = 0xFF;
        efl_p1 = fvt_can_tx_init(8, EXTENDED, 0x18FEEF00, get_TEN_SECONDS());
        efl_p1.message.data[0] = 0xFF;
        efl_p1.message.data[1] = 0xFF;
        efl_p1.message.data[2] = 0xFF;
        efl_p1.message.data[3] = 0x32;  //engine oil pressure dummy value
        efl_p1.message.data[4] = 0xFF;
        efl_p1.message.data[5] = 0xFF;
        efl_p1.message.data[6] = 0xFF;
        efl_p1.message.data[7] = 0xFF;
        eec1 = fvt_can_tx_init(8, EXTENDED, 0x0CF00400, get_FIFTY_MSECS());
        eec1.message.data[0] = 0xFF;
        eec1.message.data[5] = 0;       //Source Address of Controlling Device for Engine Control (0 for Engine ECU, copied from logs)
        eec1.message.data[6] = 0xF4;    //engine starter mode, copied from logs
        eec1.message.data[7] = 0xFF;
        egf1 = fvt_can_tx_init(8, EXTENDED, 0x0CF00A00, get_FIFTY_MSECS());
        egf1.message.data[0] = 0xFF;
        egf1.message.data[1] = 0xFF;
        egf1.message.data[2] = 0x71;    //engine intake air mass flow rate dummy value
        egf1.message.data[2] = 0x09;    //engine intake air mass flow rate dummy value
        egf1.message.data[4] = 0xFF;
        egf1.message.data[5] = 0xFF;
        egf1.message.data[6] = 0xFF;
        egf1.message.data[7] = 0xFF;
        ic1 = fvt_can_tx_init(8, EXTENDED, 0x18FEF600, get_FIVE_HUNDRED_MSECS());
        ic1.message.data[0] = 0xFF;
        ic1.message.data[1] = 0xFF;
        ic1.message.data[2] = 0x36;     //engine intake manifold temperature dummy value
        ic1.message.data[3] = 0x3E;     //engine intake air pressure dummy value
        ic1.message.data[4] = 0xFF;
        ic1.message.data[5] = 0xFF;
        ic1.message.data[6] = 0xFF;
        ic1.message.data[7] = 0xFF;
        eec3 = fvt_can_tx_init(8, EXTENDED, 0x18FEDF00, get_TWO_HUNDRED_FIFTY_MSECS());
        eec3.message.data[0] = 0xFF;
        eec3.message.data[3] = 0xFF;
        eec3.message.data[4] = 0xFF;
        eec3.message.data[5] = 0xFF;
        eec3.message.data[6] = 0xFF;
        eec3.message.data[7] = 0xFF;
        at1ig2 = fvt_can_tx_init(8, EXTENDED, 0x18FDB400, get_FIVE_HUNDRED_MSECS());
        at1ig2.message.data[0] = 0xFF;
        at1ig2.message.data[1] = 0xFF;
        at1ig2.message.data[2] = 0xAA;  //aftertreatment 1 intake gas 2 dummy value
        at1ig2.message.data[3] = 0x2C;  //aftertreatment 1 intake gas 2 dummy value
        at1ig2.message.data[4] = 0xFF;
        at1ig2.message.data[5] = 0xFF;
        at1ig2.message.data[6] = 0xFF;
        at1ig2.message.data[7] = 0xFF;
        eoi = fvt_can_tx_init(8, EXTENDED, 0x0CFD9200, get_TWO_HUNDRED_FIFTY_MSECS());
        eoi.message.data[0] = 0xFF;
        eoi.message.data[1] = 0xFF;
        eoi.message.data[2] = 0xFF;
        eoi.message.data[3] = 0xFF;
        eoi.message.data[4] = 0xFF;
        eoi.message.data[5] = 0xFF;
        eoi.message.data[6] = 0xCF;         //copied from logs
        eoi.message.data[7] = 0xFF;
        a1doc1 = fvt_can_tx_init(8, EXTENDED, 0x18FD2000, get_FIVE_HUNDRED_MSECS());
        a1doc1.message.data[0] = 0x12;  //Aftertreatment 1 Diesel Oxidation Catalyst Intake Temperature dummy value
        a1doc1.message.data[1] = 0x30;  //Aftertreatment 1 Diesel Oxidation Catalyst Intake Temperature dummy value
        a1doc1.message.data[2] = 0xFF;
        a1doc1.message.data[3] = 0xFF;
        a1doc1.message.data[4] = 0xFF;
        a1doc1.message.data[5] = 0xFF;
        a1doc1.message.data[6] = 0xFF;
        a1doc1.message.data[7] = 0xFF;
        shutdn = fvt_can_tx_init(8, EXTENDED, 0x18FEE400, get_ONE_SECOND());
        shutdn.message.data[0] = 0xFF;
        shutdn.message.data[1] = 0xFF;
        shutdn.message.data[2] = 0xFF;
        shutdn.message.data[3] = 0xFC;
        shutdn.message.data[4] = 0xFC;
        shutdn.message.data[5] = 0xFF;
        shutdn.message.data[6] = 0xFF;
        shutdn.message.data[7] = 0xFF;
        at1img = fvt_can_tx_init(8, EXTENDED, 0x18FDB200, get_FIVE_HUNDRED_MSECS());
        at1img.message.data[0] = 0xFF;
        at1img.message.data[1] = 0xFF;
        at1img.message.data[2] = 0xFF;
        at1img.message.data[3] = 0xFF;
        at1img.message.data[4] = 0x0C;  //Aftertreatment 1 Diesel Particulate Filter Differential Pressure dummy value
        at1img.message.data[5] = 0x00;  //Aftertreatment 1 Diesel Particulate Filter Differential Pressure dummy value
        at1img.message.data[6] = 0xFF;
        at1img.message.data[7] = 0xFF;
        dpf1s = fvt_can_tx_init(8, EXTENDED, 0x18FD1C00, get_ONE_SECOND());
        dpf1s.message.data[0] = 0x05;   //Aftertreatment 1 Diesel Particulate Filter Soot Mass dummy value
        dpf1s.message.data[1] = 0xFF;
        dpf1s.message.data[2] = 0xFF;
        dpf1s.message.data[3] = 0xFF;
        dpf1s.message.data[4] = 0xFF;
        dpf1s.message.data[5] = 0xFF;
        dpf1s.message.data[6] = 0xFF;
        dpf1s.message.data[7] = 0xFF;
        at1og2 = fvt_can_tx_init(8, EXTENDED, 0x18FDB300, get_FIVE_HUNDRED_MSECS());
        at1og2.message.data[0] = 0xFF;
        at1og2.message.data[1] = 0xFF;
        at1og2.message.data[2] = 0xF1;  //Aftertreatment 1 Diesel Particulate Filter Outlet Temperature dummy value
        at1og2.message.data[3] = 0x25;  //Aftertreatment 1 Diesel Particulate Filter Outlet Temperature dummy value
        at1og2.message.data[4] = 0xFF;
        at1og2.message.data[5] = 0xFF;
        at1og2.message.data[6] = 0xFF;
        at1og2.message.data[7] = 0xFF;
        lfc1 = fvt_can_tx_init(8, EXTENDED, 0x18FEE900, get_ONE_SECOND());
        lfc1.message.data[0] = 0xFF;
        lfc1.message.data[1] = 0xFF;
        lfc1.message.data[2] = 0xFF;
        lfc1.message.data[3] = 0xFF;
        lfc1.message.data[4] = 0x0;     //Engine Total Fuel Used dummy value
        lfc1.message.data[5] = 0x0;     //Engine Total Fuel Used dummy value
        lfc1.message.data[6] = 0x0;     //Engine Total Fuel Used dummy value
        lfc1.message.data[7] = 0x0;     //Engine Total Fuel Used dummy value
        efl_p2 = fvt_can_tx_init(8, EXTENDED, 0x18FEDB00, get_FIVE_HUNDRED_MSECS());
        efl_p2.message.data[0] = 0xFF;
        efl_p2.message.data[0] = 0xFF;
        efl_p2.message.data[2] = 0x1D;  //Engine Fuel 1 Injector Metering Rail 1 Pressure dummy value
        efl_p2.message.data[3] = 0x35;  //Engine Fuel 1 Injector Metering Rail 1 Pressure dummy value
        efl_p2.message.data[0] = 0xFF;
        efl_p2.message.data[0] = 0xFF;
        efl_p2.message.data[0] = 0xFF;
        efl_p2.message.data[0] = 0xFF;
        et1 = fvt_can_tx_init(8, EXTENDED, 0x18FEEE00, get_ONE_SECOND());
        et1.message.data[1] = 0x3A;     //Engine Fuel 1 Temperature 1 dummy value
        et1.message.data[2] = 0xFF;
        et1.message.data[3] = 0xFF;
        et1.message.data[4] = 0xFF;
        et1.message.data[5] = 0xFF;
        et1.message.data[6] = 0xFF;
        et1.message.data[7] = 0xFF;
        propb_51 = fvt_can_tx_init(8, EXTENDED, 0x18FF5100, get_ONE_SECOND());
        propb_51.message.data[0] = 0xFF;
        propb_51.message.data[1] = 0x3C;    //prop value taken from logs, unknown meaning
        propb_51.message.data[2] = 0x0;     //prop value taken from logs, unknown meaning
        propb_51.message.data[3] = 0xFF;
        propb_51.message.data[4] = 0xFF;
        propb_51.message.data[5] = 0x0;    //prop value taken from logs, unknown meaning
        propb_51.message.data[6] = 0xC0;   //prop value taken from logs, unknown meaning
        propb_51.message.data[7] = 0x0;    //prop value taken from logs, unknown meaning
        dm01 = fvt_can_tx_init(8, EXTENDED, 0x18FECA00, get_ONE_SECOND());
        dm01.message.data[0] = 0;           //copied from logs - used for error codes in engine
        dm01.message.data[1] = 0xFF;
        dm01.message.data[2] = 0;           //copied from logs - used for error codes in engine
        dm01.message.data[3] = 0;           //copied from logs - used for error codes in engine
        dm01.message.data[4] = 0;           //copied from logs - used for error codes in engine
        dm01.message.data[5] = 0;           //copied from logs - used for error codes in engine
        dm01.message.data[6] = 0xFF;
        dm01.message.data[7] = 0xFF;
        dpfc1 = fvt_can_tx_init(8, EXTENDED, 0x18FD7C00, get_ONE_SECOND());
        dpfc1.message.data[0] = 0xF8;       //copied from logs - all do do with diesel regeneration
        dpfc1.message.data[1] = 0x83;       //copied from logs - all do do with diesel regeneration
        dpfc1.message.data[2] = 0xF0;       //copied from logs - all do do with diesel regeneration
        dpfc1.message.data[3] = 0xC3;       //copied from logs - all do do with diesel regeneration
        dpfc1.message.data[4] = 0x0;        //copied from logs - all do do with diesel regeneration
        dpfc1.message.data[5] = 0xC3;       //copied from logs - all do do with diesel regeneration
        dpfc1.message.data[6] = 0xE3;       //copied from logs - all do do with diesel regeneration
        dpfc1.message.data[7] = 0xFF;       //copied from logs - all do do with diesel regeneration
        vep1 = fvt_can_tx_init(8, EXTENDED, 0x18FEF700, get_ONE_SECOND());
        vep1.message.data[0] = 0xFF;
        vep1.message.data[1] = 0xFF;
        vep1.message.data[2] = 0xFF;
        vep1.message.data[3] = 0xFF;
        vep1.message.data[6] = 0xFF;
        vep1.message.data[7] = 0xFF;
        oi = fvt_can_tx_init(8, EXTENDED, 0x18FEFF00, get_TEN_SECONDS());
        oi.message.data[0] = 0xFC;      //taken from logs: water in fuel indicator and "operator shift prompt"
        oi.message.data[1] = 0xFF;
        oi.message.data[2] = 0xFF;
        oi.message.data[3] = 0xFF;
        oi.message.data[4] = 0xFF;
        oi.message.data[5] = 0xFF;
        oi.message.data[6] = 0xFF;
        oi.message.data[7] = 0xFF;
    }

    amb.counter--;
    propb_50.counter--;
    lfe1.counter--;
    eec2.counter--;
    efl_p1.counter--;
    eec1.counter--;
    egf1.counter--;
    ic1.counter--;
    eec3.counter--;
    at1ig2.counter--;
    eoi.counter--;
    a1doc1.counter--;
    shutdn.counter--;
    at1img.counter--;
    dpf1s.counter--;
    at1og2.counter--;
    lfc1.counter--;
    efl_p2.counter--;
    et1.counter--;
    propb_51.counter--;
    dm01.counter--;
    dpfc1.counter--;
    vep1.counter--;
    oi.counter--;

    if (amb.counter == 0)
    {
        amb.counter = amb.period;

        uint16_t ambient_air_transmit = (uint16_t)((ambient_air_temp_c + 273) * 32);

        amb.message.data[3] = (uint8_t)(ambient_air_transmit >> 8); //ambient air temperature -> (X(degrees C) + 273) / 0.03125
        amb.message.data[4] = (uint8_t)(ambient_air_transmit & 0xFF); //ambient air temperature

        Send_CAN_Message(0, CAN2, amb.message);
    }

    if (propb_50.counter == 0)
    {
        propb_50.counter = propb_50.period;

        Send_CAN_Message(0, CAN2, propb_50.message);
    }

    if (lfe1.counter == 0)
    {
        lfe1.counter = lfe1.period;

        Send_CAN_Message(0, CAN2, lfe1.message);
    }

    if (eec2.counter == 0)
    {
        eec2.counter = eec2.period;
        uint8_t throttle_percent_transmit = (uint8_t)(round((throttle_pedal_percentage * 2.5)));

        uint8_t engine_percent_load = hsp_inverter_percent_load;

        eec2.message.data[1] = throttle_percent_transmit;
        eec2.message.data[2] = engine_percent_load;

        Send_CAN_Message(0, CAN2, eec2.message);
    }

    if (efl_p1.counter == 0)
    {
        efl_p1.counter = efl_p1.period;

        Send_CAN_Message(0, CAN2, efl_p1.message);
    }

    if (eec1.counter == 0)
    {
        eec1.counter = eec1.period;

        uint8_t engine_percent_load_at_current_speed = hsp_inverter_percent_load + 125; //the 100 is the current torque of the motor in newton meters //if torque is less than zero, make it zero

        uint16_t rpm_transmit = (uint16_t)(abs(hsp_inverter_rpm) * 8);

        eec1.message.data[1] = engine_percent_load_at_current_speed;
        eec1.message.data[2] = engine_percent_load_at_current_speed;
        eec1.message.data[4] = (uint8_t)(rpm_transmit >> 8);
        eec1.message.data[3] = (uint8_t)(rpm_transmit & 0xFF);

        Send_CAN_Message(0, CAN2, eec1.message);
    }

    if (egf1.counter == 0)
    {
        egf1.counter = egf1.period;

        Send_CAN_Message(0, CAN2, egf1.message);
    }

    if (ic1.counter == 0)
    {
        ic1.counter = ic1.period;

        Send_CAN_Message(0, CAN2, ic1.message);
    }

    if (eec3.counter == 0)
    {
        eec3.counter = eec3.period;

        uint16_t desired_rpm = (uint16_t)(hsp_inverter_target_rpm * 8); //value is already *1000. divide by 1000 then multiply by 2000 to get desired rpm, then by 8 for scaling

        eec3.message.data[1] = (uint8_t)(desired_rpm >> 8);
        eec3.message.data[2] = (uint8_t)(desired_rpm & 0xFF);

        Send_CAN_Message(0, CAN2, eec3.message);
    }

    if (at1ig2.counter == 0)
    {
        at1ig2.counter = at1ig2.period;

        Send_CAN_Message(0, CAN2, at1ig2.message);
    }

    if (eoi.counter == 0)
    {
        eoi.counter = eoi.period;

        Send_CAN_Message(0, CAN2, eoi.message);
    }

    if (a1doc1.counter == 0)
    {
        a1doc1.counter = a1doc1.period;

        Send_CAN_Message(0, CAN2, a1doc1.message);
    }

    if (shutdn.counter == 0)
    {
        shutdn.counter = shutdn.period;

        Send_CAN_Message(0, CAN2, shutdn.message);
    }

    if (at1img.counter == 0)
    {
        at1img.counter = at1img.period;

        Send_CAN_Message(0, CAN2, at1img.message);
    }

    if (dpf1s.counter == 0)
    {
        dpf1s.counter = dpf1s.period;

        Send_CAN_Message(0, CAN2, dpf1s.message);
    }

    if (at1og2.counter == 0)
    {
        at1og2.counter = at1og2.period;



        Send_CAN_Message(0, CAN2, at1og2.message);
    }

    if (lfc1.counter == 0)
    {
        lfc1.counter = lfc1.period;

        Send_CAN_Message(0, CAN2, lfc1.message);
    }

    if (efl_p2.counter == 0)
    {
        efl_p2.counter = efl_p2.period;

        Send_CAN_Message(0, CAN2, efl_p2.message);
    }

    if (et1.counter == 0)
    {
        et1.counter = et1.period;

        uint8_t motor_coolant_temp_transmit = (uint8_t)(hsp_motor_coolant_temp_c + 40);

        et1.message.data[0] = motor_coolant_temp_transmit;

        Send_CAN_Message(0, CAN2, et1.message);
    }

    if (propb_51.counter == 0)
    {
        propb_51.counter = propb_51.period;

        Send_CAN_Message(0, CAN2, propb_51.message);
    }

    if (dm01.counter == 0)
    {
        dm01.counter = dm01.period;

        Send_CAN_Message(0, CAN2, dm01.message);
    }

    if (dpfc1.counter == 0)
    {
        dpfc1.counter = dpfc1.period;

        Send_CAN_Message(0, CAN2, dpfc1.message);
    }

    if (vep1.counter == 0)
    {
        vep1.counter = vep1.period;

        uint16_t aux_battery_voltage_transmit = (uint16_t)(divide(switched_battery_voltage_in * 20, 1000));


        vep1.message.data[4] = (uint8_t)(aux_battery_voltage_transmit >> 8);
        vep1.message.data[5] = (uint8_t)(aux_battery_voltage_transmit & 0xFF);


        Send_CAN_Message(0, CAN2, vep1.message);
    }

    if (oi.counter == 0)
    {
        oi.counter = oi.period;

        Send_CAN_Message(0, CAN2, oi.message);
    }
}
