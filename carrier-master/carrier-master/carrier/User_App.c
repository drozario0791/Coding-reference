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

/*Include Header Files */
#include <stdio.h>
#include <math.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "typedefs.h"
#include "can_service.h"
#include "pdm_device.h"
#include "bel_charger_device.h"
#include "orion_device.h"
#include "pku2400_device.h"
#include "shinry_dcdc_device.h"
#include "cooling_lubrication.h"
#include "gears_and_transmission.h"
#include "hydraulic_system.h"
#include "orion_control.h"
#include "shinry_dcdc_control.h"
#include "traction_inverter_control.h"
#include "bel_charger_control.h"
#include "state_machine.h"
#include "contactor_control.h"
#include "cvc_power_sequencing.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "power_up_devices.h"
#include "shinry_dcdc_device.h"
#include "skai2_inverter_vissim.h"
#include "skai2_inverter.h"
#include "sevcon_hvlp10_device.h"
#include "cvc_debug_msgs.h"
#include "cvc_input_control.h"
#include "hydraulic_system.h"
#include "accessories_support.h"
#include "cvc_power_sequencing.h"
#include "timer_service.h"
#include "cl712_device_control.h"
#include "hydraulic_inverter_control.h"


/*
 * There are 4 different types of variables that can be used in a
 * Presto Project, they are listed in the order that you should try
 * use them below
 *
 * Local:
 *
 *      This is a variable that is defined within a function without
 *      the static keyword.
 *
 *      These variables are pushed onto the stack each time the
 *      function is called and loaded with the default value.
 *
 *      At the completed of the function they will be popped off the
 *      stack.
 *
 *      Note: If the local variable is a large structure you should
 *            consider making it a static local even if you don't
 *            need to maintain the value to ensure that you won't
 *            over run the stack size
 *
 * Static Local:
 *
 *      This is a variable that is defined within a function
 *      with the keyword static in front of it.
 *
 *      The first time the function is called it will be set to the
 *      default value.
 *
 *      After that the variable will hold the last value it was set
 *      to as it has a dedicated location in RAM.
 *
 *      It is similar to a local global but the scope is limited to a function
 *
 *      Example -> see heartbeat_count in heartbeatControl.
 *
 * Static Global:
 *
 *      This is a variable that is global but its scope is limited to
 *      the file it is defined in.
 *
 *      This is accomplished by putting static in front of the
 *      definition it can not be used by other C files.
 *
 *      In order to access this type of variable from other files
 *      getter and setters have to be created.
 *
 *      Example -> if you add static in front of App_Structure_t As,
 *                 you will get a compiler warning for the extern in
 *                 App.h and an error if you try to use it in another
 *                 file.
 *
 * Global:
 *
 *      This is a variable that can be used in any file in the project
 *      (good programming practice is to avoid these).
 *
 *      Variable is defined in a C file outside of a function.
 *
 *      Other files gain access by use of the extern keyword in front
 *      of the variable definition.
 *
 *      Example -> See App_Structure_t As
 */

//
// Processor specific # defines to control RAM page the variables
// stored in -if blank in reserved.h you may exclude.
//
USER_RAM

//
// Example global user structure
//
// App_Structure_t As;

//
// Processor specific # defines to control RAM page the variables
// stored in if blank in reserved.h you may exclude.
//
DEFAULT_RAM

/******************************************************************************
 *
 *        Name:User_App()
 *
 * Description: This function gets called by the firmware once every
 *              loop. Loop speed controlled by Orchestra Project
 *              settings.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
extern bool_t low_power_mode;

void User_App(void)
{

    static uint32_t loop_cnt = 0;
    static uint32_t first_pass = TRUE;
    static timer_t startup_timer;


    if(first_pass)
    {

        //
        // Initialise a three second timer.
        //
        startup_timer = timer_setup(get_TWO_SECONDS(), RISING);
        first_pass = FALSE;

    }

    //=============================================================================
    //
    // Provides 12V supply to all high voltages on the Carrier The
    // keyswitch is ON. Close the 12V contactor. This will provide
    // system wide 12V power. CVC connectors B, C, & D will also have
    // 12V applied.
    //
    //=============================================================================
    //
    //
    low_power_mode =
       cvc_power_sequencing_low_power_control();

    //=============================================================================
    //
    // Start up timer: If the vehicle is not in low power mode. enable
    // a 2 second timer. Which gives you enough time for devices to be
    // powered and instanteated.
    //
    //=============================================================================
    //
    bool_t startup_timer_state =  timer_operate(&startup_timer,
                                         !low_power_mode);

    //=============================================================================
    //
    // Enable the power to the three important bus-bars on the vehilcle
    //
    // 1) Main Bus Bar: Traction Inverter, Bel Charger 2
    // 2) Charger1 Bus Bar: Charger 1
    // 3) Aux Hydraulic: Hydraulic Inverter, Transmission Inverter
    // 4) Enable power to the rear driveline pressure sensor -> pdm1_channel_12
    //=============================================================================
    //
    pdm_3_command_main_bussbar_supply_pwr(TRUE);
    pdm_3_command_charger_1_busbar_pwr(TRUE);
    pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(TRUE);
    pdm_1_command_channel_12_pwr(TRUE);

    //=============================================================================
    //
    // Walk the linked lists of receive CAN messages, looking for any
    // that have timeouts enabled and have exceeded the timeout limit.
    //
    // Timeout limits, per message, may be set with calls to:
    // fvt_can_set_timeout_receive_id() or
    // fvt_can_set_timeout_receive_id_j1939_byte()
    //
    //=============================================================================
    //
    can_rx_check_message_timeouts();

    //=============================================================================
    //
    // Check PKU2400 switch states, calling any registered toggle of
    // momentary push button functions on each switch change.
    //
    //=============================================================================
    //
    pku2400_check_switches(ONE);

    //=============================================================================
    //
    // Checked the temperatures of the motors and power electronic
    // device connected on the vehicle. If temperatures exceed their
    // set thresholds, enable the appropriate the pumps and fans.
    //
    //=============================================================================
    //
    cooling_and_lubcrication_control();

    //=============================================================================
    //
    // The output from the shifter is connected as inputs to the
    // CVC. The function checks the input from the shifter and enables
    // the appropriate solenoids on the transmission. The shifter can
    // be placed in one of the following positions: forward, reverse,
    // first gear, second gear, third gear and fourth gear. Each signal
    // controls its corresponding transmission solenoid. The pdm
    // module provides power to the transmission solenoids.
    //
    //=============================================================================
    //
    gears_and_transmission_control();

    //=============================================================================
    //
    // The function gathers data from the state machine, orion BMS and
    // the shifter. Based on the information obtained, the control
    // function controls the working of the skai2 traction drive
    // inverter.
    //
    // NOTE: VISSIM CODE IS BRING USED ON THE SKAI2 TRACTION INVERTER.
    //
    //=============================================================================
    //
    traction_inverter_control(ONE);

    //=============================================================================
    //
    // A function that control the working of the hydraulic system
    // Hydraulic Inverter, Start Drain Solenoid, and Transmission
    // Inverter.
    //
    // Note: Hydraulic Inverter: FVT Motor Control RTOS
    //       Transmission Inverter: Sevcon Control
    //
    //=============================================================================
    //
    hydraulic_system_control(TWO, ONE);

    //=============================================================================
    //
    //
    // The DC-DC converter charges the 12V LVbattery on the
    // vehicle. The DC-DC gets its input from the high voltage
    // battery. Its it important for the shinry DC-DC converter to be
    // on as soon as contactors are closed. The reason being, the
    // pumps, fans, BMS, inverters are powered off the 12V supply. The
    // DC-DC needs to ensure the 12V battery is always charged to keep
    // the rest of the system up and functional.
    //
    //=============================================================================
    //
    bool_t status = shinry_dc_dc_control();

    //=============================================================================
    //
    // A function that enables/disables the bel charger based on
    // various conditions it receives from the state machine and BMS.
    //
    //=============================================================================
    //
    bel_charger_control(ONE);
    bel_charger_control(TWO);

    //=============================================================================
    //
    // populate state machine
    //
    //=============================================================================
    //
    populate_state_machine_member_elements();

    //=============================================================================
    //
    // State Machine: Determines the state the vehicle.
    //
    //=============================================================================
    //
    if(startup_timer_state == TRUE)
    {
        run_state_machine();
    }
    //=============================================================================
    //
    //  The contactor_control function controls the working of the
    //  contactors on the carrier. The input to the contactor control is
    //  obtained from the state machine. The state machine getters
    //  determine the opening and closing of contactors on the carrier.
    //
    //
    //=============================================================================
    //
    update_contactor_control_output(startup_timer_state);

    //=============================================================================
    //
    // Control the working of the accessories on the carrier.
    //
    //=============================================================================
    //
    accessories_support();

    //=============================================================================
    //
    // Transmit CAN messages to the screen
    //
    //=============================================================================
    //
    cl712_device_control();

    //============================================================================
    // An indication of loop count
    //===========================================================================
    loop_cnt++;

    state_t current_state = get_sm_current_state();
    bool_t pos_contactor = get_sm_pos_contactor_status();
    bool_t neg_contactor = get_sm_neg_contactor_status();
    bool_t pre_contactor = get_sm_pre_contactor_status();
    bool_t failure = get_sm_precharge_failure_status();
    bool_t success = get_sm_precharge_success_status();
    bool_t hv_enabled = get_sm_status_enable_hv_systems();
    bool_t brakes = get_sm_status_brakes_desired();
    bool_t charging = get_sm_status_charging_desired();

    send_debug_can_messages_8bits(0xF001,
                                  (uint8_t) current_state,
                                  (uint8_t) hv_enabled,
                                  (uint8_t) pos_contactor,
                                  (uint8_t) neg_contactor,
                                  (uint8_t) pre_contactor,
                                  (uint8_t) failure,
                                  (uint8_t) success,
                                  (uint8_t) charging);

    send_debug_can_messages_16bits(0xF006,
                                  (uint8_t)get_shifter_direction(),
                                  (uint8_t)get_shifter_gear_position(),
                                  0x0000,
                                  0x0000);

    send_debug_can_messages_16bits(0xF007,
                                    get_front_axil_parking_brake_pressure(),
                                    get_front_service_brake_pressure(),
                                    get_front_accum_pressure(),
                                    main_sys_pressure());

    send_debug_can_messages_16bits(0xF008,
    							  	  get_transmission_pressure(),
    							  	  get_rear_service_brake_pressure(),
                                      get_rear_accum_pressure(),
                                      get_drive_line_brake_pressure());

    send_debug_can_messages_16bits(0xF009,
                                  shinry_get_instantaneous_input_voltage(ONE),
                                  shinry_get_instantaneous_output_voltage(ONE),
                                  shinry_get_instantaneous_temperature(ONE),
                                  shinry_get_instantaneous_current_voltage(ONE));

}

/******************************************************************************
 *
 *        Name: heartbeat()
 *
 * Description:
 *
 *      Author: HED
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
static void hearbeat(uint32_t loop_count)
{
    static uint16_t  heartbeat_count = 0;

    if (heartbeat_count < ConvertMsecToLoops(loop_count >> 1))
    {
        heartbeat_count++;
    }
     else if(heartbeat_count < ConvertMsecToLoops(loop_count))
    {
        heartbeat_count++;
    }
    else
    {
        /* Skip*/
    }

    if(heartbeat_count >= ConvertMsecToLoops(loop_count))
    {
        heartbeat_count = 0;
    }
    else
    {
        /* Skip*/
    }

}
