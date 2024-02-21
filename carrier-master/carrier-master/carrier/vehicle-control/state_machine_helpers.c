/******************************************************************************
 *
 *        Name: state_machine_helpers.c
 *
 * Description: Consist of all the getters and setters required for
 *              the state_machine.
 *
 *      Author: Deepak
 *        Date: Saturday, 07 September 2019
 *
 ******************************************************************************
 */
#include <stdio.h>
#include "reserved.h"
#include "Prototypes_CAN.h"
#include "Constants.h"
#include "typedefs.h"
#include "Prototypes.h"
#include "User_App.h"
#include "User_Init.h"
#include "can_service.h"
#include "skai2_inverter.h"
#include "bel_charger_device.h"
#include "pdm_device.h"
#include "orion_device.h"
#include "pku2400_device.h"
#include "shinry_dcdc_device.h"
#include "shinry_dcdc_control.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "pku2400_control_1.h"
#include "sevcon_hvlp10_device.h"
#include "skai2_inverter_vissim.h"
#include "can_service.h"
#include "cooling_lubrication.h"
#include "state_machine.h"
#include "timer_service.h"
#include "fvt_library.h"
#include "orion_control.h"

#define BATTERY_CRITICAL_FAULT_THRESHOLD_MV        8000
#define MOTOR_CRITICAL_FAULT_TEMPERATURE_THRESHOLD 150
#define INVERTER_CRITICAL_FAULT_TEMPERATURE_THRESHOLD 125
#define EEVAR_NO_OF_BATTERY_PACKS                  3

static uint32_t critical_hv_off_fault = 0;
static uint32_t critical_hv_on_fault = 0;


/******************************************************************************
 *
 *        Name: check_critical_fault_high_voltage_off()
 *
 * Description: A function that checks all the critical faults on the
 *              vehicle. If any of these faults occur. The vehicle
 *              would turn off the high voltage.
 *
 *        Date: Monday, 09 September 2019
 *
 ******************************************************************************
 */
bool_t check_critical_fault_high_voltage_off()
{

    //
    // Small delay in the aux battery dropping low before error is
    // signalled.
    //
    timer_t aux_battery_dying_timer;

    static bool_t first_pass = TRUE;

    bool_t critical_fault_high_voltage_off = FALSE;

    //
    // A structure provided by HED, the member elements of the
    // structure give indication on the state of health of the hed
    // module.
    //
    hed_system_status_t system_status =
        {.in_safe_mode = FALSE};

    if(first_pass)
    {

        //
        // Initialize timer
        //
        aux_battery_dying_timer =
            timer_setup(get_TWO_AND_A_HALF_SECONDS(), RISING);

        first_pass = FALSE;

    }

    //
    // Check to see if battery power supply voltage is less than
    // 11V. If this condition is true. Start a timer, for two and half
    // mins (just to see if the DC-DC starts up to charge the 12V
    // battery). If the voltage is lower after that. Flag a critical
    // fault.
    //
    bool_t enable_hv_system =
    		get_sm_status_enable_hv_systems();


    bool_t aux_battery_dying =
        timer_operate(&aux_battery_dying_timer,
                      (IN_A11_keyswitch <= 11000) &&
                      enable_hv_system &&
                      (get_sm_pos_contactor_status() == 1000));

    //
    // Check to see if the 12V battery is above 8V. if its under 8V
    // flag a critical fault.
    //
    bool_t auxilary_voltage_under_voltage_fault =
        (IN_A11_keyswitch < BATTERY_CRITICAL_FAULT_THRESHOLD_MV);

    //
    // Check to see if a battery fault has occured
    //
    bool_t battery_failure =
        battery_pack_failure_status();

    //
    // Check to see if an traction drive inverter fault has occured.
    //
    bool_t traction_inverter_critical_fault =
        skai_get_vissim_error_status(ONE);

    //
    // Check to see if an hydraulic inverter fault has occured
    //
    bool_t hydraulic_inverter_critical_fault =
        skai_get_vissim_error_status(TWO);

    //
    // Check to see if a precharge failure has occured.
    //
    bool_t precharge_failure =
        get_sm_precharge_failure_status();

    //
    // Check DCDC fault
    //
    bool_t dcdc_critical_fault = FALSE;
    
    //
    // Check to see if any HED faults have occured.
    //
    system_status = get_system_status();

    bool_t can_1_error_status =
        (system_status.can1_status == CAN_ERROR_STATUS);

    bool_t can_1_bus_off =
        (system_status.can1_status == CAN_BUSOFF_STATUS);

    bool_t can_2_error_status =
        (system_status.can2_status == CAN_ERROR_STATUS);

    bool_t can_2_bus_off =
        (system_status.can2_status == CAN_BUSOFF_STATUS);

    bool_t can_3_error_status =
        (system_status.can3_status == CAN_ERROR_STATUS);

    bool_t can_3_bus_off =
        (system_status.can3_status == CAN_BUSOFF_STATUS);

    bool_t master_running_state =
        (system_status.master_module_state != MODULE_RUNNING_STATE);

    bool_t exceeded_system_error_count =
        (system_status.number_of_system_errors != 0);

    bool_t traction_inverter_can_time_out = FALSE;

    bool_t charger_failure = FALSE;

    //
    // Check to see if a critical fault has occured
    //
    critical_fault_high_voltage_off = (aux_battery_dying
                                       || auxilary_voltage_under_voltage_fault
                                       || battery_failure
                                       || traction_inverter_critical_fault
                                       || hydraulic_inverter_critical_fault
                                       || precharge_failure
                                       || dcdc_critical_fault
                                       || can_1_error_status
                                       || can_1_bus_off
                                       || can_2_error_status
                                       || can_2_bus_off
                                       || can_3_error_status
                                       || can_3_bus_off
                                       || master_running_state
                                       || exceeded_system_error_count);


    critical_hv_off_fault = (uint32_t)(
        (aux_battery_dying && enable_hv_system    << 0)
        | (auxilary_voltage_under_voltage_fault   << 1)
        | (battery_failure                        << 2)
        | (traction_inverter_critical_fault       << 3)
        | (hydraulic_inverter_critical_fault      << 4)
        | ((can_1_error_status || can_1_bus_off)  << 5)
        | ((can_2_error_status || can_2_bus_off)  << 6)
        | ((can_3_error_status || can_3_bus_off)  << 7)
        | (master_running_state                   << 8)
        | (exceeded_system_error_count            << 9)
        | (precharge_failure                      << 10)
        | (charger_failure                        << 11)
        | (dcdc_critical_fault                    << 12)
        );

    //
    // return the critical_fault_state
    //
    return critical_fault_high_voltage_off;

}


/******************************************************************************
 *
 *        Name: check_critical_fault_high_voltage_on()
 *
 * Description: A function that checks all the critcal faults on the
 *              vehicle. If any of these faults occur. The vehicle
 *              would be kept the high voltage ON.
 *
 *       0 Date: Monday, 09 September 2019
 *
 ******************************************************************************
 */
bool_t check_critical_fault_high_voltage_on()
{

    static bool_t critical_fault_high_voltage_on =
        FALSE;

    //
    // Check to see if the motor temperature has exceeded 150 Degree
    // Celcius.
    //
    uint16_t traction_motor_temp =
        skai_get_vissim_motor_temp_C(ONE);

    uint16_t hydraulic_motor_temp =
        skai_get_vissim_motor_temp_C(TWO);


    uint16_t traction_inverter_temp = max_of_three_uint16_t_values(skai_get_vissim_dcb_phase1_temperature(ONE),
                                                                   skai_get_vissim_dcb_phase2_temperature(ONE),
                                                                   skai_get_vissim_dcb_phase3_temperature(ONE));

    traction_inverter_temp = traction_inverter_temp / 10;


    uint16_t hydraulic_inverter_temp = max_of_three_uint16_t_values(skai_get_vissim_dcb_phase1_temperature(ONE),
                                                                    skai_get_vissim_dcb_phase2_temperature(ONE),
                                                                    skai_get_vissim_dcb_phase3_temperature(ONE));

    hydraulic_inverter_temp = hydraulic_inverter_temp / 10;

    //
    // Check to see if any cooling element has faulted.
    //
    bool_t cooling_device_fault = FALSE;

    critical_fault_high_voltage_on = (cooling_device_fault
    		|| (traction_motor_temp > 150)
    		|| (hydraulic_motor_temp > 150)
    		|| (traction_inverter_temp > 125)
    		|| (hydraulic_inverter_temp > 125));

    critical_hv_on_fault = (uint32_t)(
        ((traction_motor_temp > 150)             << 0)
        | ((hydraulic_motor_temp > 150)          << 1)
        | ((traction_inverter_temp > 125)        << 2)
        | ((hydraulic_inverter_temp > 125)       << 3)
        | (cooling_device_fault                  << 4));

    return critical_fault_high_voltage_on;
}


/******************************************************************************
 *
 *        Name: get_system_status()
 *
 * Description: Checks to see if the CVC has encountered errors
 *
 *      Author: Cyrus
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
hed_system_status_t get_system_status()
{
    hed_system_status_t system_status;

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

    //
    // There are no submodules in this vehicle.
    //
    system_status.io_modules_in_running_state = TRUE;
    system_status.in_safe_mode =
        set_reset(system_status.in_safe_mode,
        		IN_A11_keyswitch <= 8000,
        		IN_A11_keyswitch >= 12000);

    return system_status;
}

/******************************************************************************
 *
 *        Name: battery_pack_failure_status()
 *
 * Description: Flags a bettery pack fault if more than two battery
 *              packs have been faulted.
 *
 *        Date: Sunday, 15 September 2019
 *
 ******************************************************************************
 */
bool_t battery_pack_failure_status()
{

    bool_t error_status = FALSE;
    uint8_t max_allowabled_working_packs = 1;
    
    //
    // Check for fresh data updates
    //
    bool_t fresh_data_1 = bms_fresh_data_1();
    bool_t fresh_data_2 = bms_fresh_data_2();
    bool_t fresh_data_3 = bms_fresh_data_3();

    uint8_t bms_pack_fault_1_status = (uint8_t)get_battery_pack_error_status(ONE) || !fresh_data_1;
    uint8_t bms_pack_fault_2_status = (uint8_t)get_battery_pack_error_status(TWO) || !fresh_data_2;
    uint8_t bms_pack_fault_3_status = (uint8_t)get_battery_pack_error_status(THREE) || !fresh_data_3;

    uint8_t total_faulted_packs = (bms_pack_fault_1_status
    		+ bms_pack_fault_2_status
    		+ bms_pack_fault_3_status);

    //
    // Flag a fault if two battery packs have faulted
    //
    if(total_faulted_packs >= 1)
    {

        //
        // error has occured
        //
        error_status = TRUE;

    }
    else
    {

        //
        // no error
        //
        error_status = FALSE;
    }

    return error_status;

}


//=============================================================================
//
// Getters for critical faults()
//
//=============================================================================
//
uint32_t get_critical_fault_hv_off()
{

    return critical_hv_off_fault;

}

uint32_t get_critical_fault_hv_on()
{

    return critical_hv_on_fault;

}

uint32_t yellow_warning_message()
{

    uint32_t yellow_warning_light = 0;

    //
    // battery warm: high cell is above 50 degrees
    //
    bool_t battery_warm =
        (get_battery_pack_high_cell_max_temperature() >= 50);

    //
    // cell delta: (highcell - low_cell) > 1000
    //
    bool_t pack1_cell_delta =
        ((orion_get_pack_high_cell_voltage(ONE) - orion_get_pack_low_cell_voltage(ONE)) > 1750);

    bool_t pack2_cell_delta =
        ((orion_get_pack_high_cell_voltage(TWO) - orion_get_pack_low_cell_voltage(TWO)) > 1750);

    bool_t pack3_cell_delta =
        ((orion_get_pack_high_cell_voltage(THREE) - orion_get_pack_low_cell_voltage(THREE)) > 1750);

    bool_t cell_delta = (pack1_cell_delta || pack2_cell_delta || pack3_cell_delta);

    //
    // Traction Inverter Warm > 100
    //
    //
    // Check to see if the motor temperature has exceeded 150 Degree
    // Celcius.
    //
    uint16_t traction_motor_temp =
        skai_get_vissim_motor_temp_C(ONE);

    uint16_t hydraulic_motor_temp =
        skai_get_vissim_motor_temp_C(TWO);


    uint16_t traction_inverter_temp = max_of_three_uint16_t_values(skai_get_vissim_dcb_phase1_temperature(ONE),
                                                                   skai_get_vissim_dcb_phase2_temperature(ONE),
                                                                   skai_get_vissim_dcb_phase3_temperature(ONE));

    traction_inverter_temp = traction_inverter_temp / 10;


    uint16_t hydraulic_inverter_temp = max_of_three_uint16_t_values(skai_get_vissim_dcb_phase1_temperature(ONE),
                                                                    skai_get_vissim_dcb_phase2_temperature(ONE),
                                                                    skai_get_vissim_dcb_phase3_temperature(ONE));

    hydraulic_inverter_temp = hydraulic_inverter_temp / 10;

    //
    // high voltage isolation fault
    //
    bool_t iso_flt_pack1 = orion_get_flag2_high_voltage_isolation_fault(ONE);
    bool_t iso_flt_pack2 = orion_get_flag2_high_voltage_isolation_fault(TWO);
    bool_t iso_flt_pack3 = orion_get_flag2_high_voltage_isolation_fault(THREE);

    bool_t hv_isolation_fault = (iso_flt_pack1
                                 || iso_flt_pack2
                                 || iso_flt_pack3);

    //
    // power limited battery low: soc < 30%
    //
    bool_t low_soc = (bool_t)(get_battery_pack_SOC() < 30);

    yellow_warning_light = ((battery_warm                         << 0)
                            | (cell_delta                         << 1)
                            | ((traction_motor_temp > 125)        << 2)
                            | ((hydraulic_motor_temp > 125)       << 3)
                            | ((traction_inverter_temp > 100)     << 4)
                            | ((hydraulic_inverter_temp > 100)    << 5)
                            | (hv_isolation_fault                 << 6)
                            | (low_soc                            << 7));

    return yellow_warning_light;

}
