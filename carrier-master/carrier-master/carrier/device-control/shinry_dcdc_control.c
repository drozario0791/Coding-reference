/******************************************************************************
 *
 *        Name: shiney_dcdc_control.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */

#include "Prototypes.h"
#include "Constants.h"
#include "Prototypes_CAN.h"
#include "can_service.h"
#include "shinry_dcdc_device.h"
#include "timer_service.h"
#include "shinry_dcdc_control.h"
#include "state_machine.h"
#include "pdm_control_2.h"

/******************************************************************************
 *
 *        Name: shinry_dc_dc_control();
 *
 * Description: The DC-DC converter charges the 12V LVbattery on the
 *              vehicle. The DC-DC gets its input from the high
 *              voltage battery. Its it important for the shinry DC-DC
 *              converter to be on as soon as contactors are
 *              closed. The reason being, the pumps, fans, BMS,
 *              inverters are powered off the 12V supply. The DC-DC
 *              needs to ensure the 12V battery is always charged to
 *              keep the rest of the system up and functional.
 *
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */
bool_t shinry_dc_dc_control()
{

    static timer_t enable_dcdc_timer;
    static bool_t first_pass = TRUE;

    if(first_pass == TRUE)
    {
        //
        // five_seconds
        //
        enable_dcdc_timer = timer_setup(get_TWO_SECONDS(), RISING);
        first_pass = FALSE;

    }


    //
    // The enable bit for the dc-dc is obtained from the fvt state
    // machine. If the vehicle has been precharged safely without any
    // faults, the state machine sets a bit that provide the
    // enable/disable state for the dc-dc.
    //
    //
    // Set the Output Voltage of the dc-dc converter
    //
    float vcu_dcdc_voltage_req = 0.0;

	state_t state = get_sm_current_state();
	bool_t precharge_successful = get_sm_precharge_success_status();

    bool_t dcdc_enable = get_sm_status_enable_hv_systems() && precharge_successful;

    bool_t enable = timer_operate(&enable_dcdc_timer,
                                  dcdc_enable);

    if(enable == TRUE)
    {

    	vcu_dcdc_voltage_req = 14.5;

        shinry_set_dcdc_control(ONE,
                                vcu_dcdc_voltage_req,
                                TRUE);
        // REWIRED
        // Note: The soft start solenoid provides the 12V to the
        // enable of the DCDC connector
        //
    	pdm_2_command_start_drain_solenoid_pwr(TRUE);

    }
    else
    {

    	vcu_dcdc_voltage_req = 0.0;

        shinry_set_dcdc_control(ONE,
                                vcu_dcdc_voltage_req,
                                FALSE);

        // REWIRED
        // Note: The soft start solenoid provides the 12V to the
        // enable of the DCDC connector
        //
    	pdm_2_command_start_drain_solenoid_pwr(FALSE);

    }


    //
    // Call the setter to set the elements of the shinry tx data
    // structure.
    //
    shinry_set_dcdc_control(ONE,
                            vcu_dcdc_voltage_req,
                            dcdc_enable);

    //
    // Transmit Can Messages required for the device to function.
    //
    tx_shinry_dcdc_control(ONE, EVERY_100MS);

    bool_t error_status = shinry_dcdc_error_status();

    return error_status;

}


/******************************************************************************
 *
 *        Name: shinry_dcdc_error_status()
 *
 * Description: returns True if the the DCDC flags an error.
 *
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */
bool_t shinry_dcdc_error_status()
{

    //
    // Check the various error states thet could occur during the
    // working of the CVC
    //
    bool_t over_current_fault = shinry_get_over_current_fault(ONE);
    bool_t can_cmd_lost = shinry_get_can_cmd_lost(ONE);
    bool_t output_over_voltage = shinry_get_output_over_voltage(ONE);
    bool_t input_over_voltage = shinry_get_input_over_voltage(ONE);
    bool_t output_under_voltage = shinry_get_output_under_voltage(ONE);
    bool_t input_under_voltage = shinry_get_input_under_voltage(ONE);

    //
    // Check to see if any of the above faults have occured
    //
    bool_t dcdc_error_status = ( can_cmd_lost
                                || input_over_voltage
                                || input_under_voltage
                                || over_current_fault
                                || output_over_voltage
                                || output_under_voltage);

    //
    // return the dcdc_error_status
    //
    return dcdc_error_status;

}
