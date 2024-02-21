/******************************************************************************
 *
 *        Name: pdm_test_harness.c
 *
 * Description: Use the top 4 PKU dash switches to set groups of four
 *              PDM 1, 2, 3 outputs that may be toggled with the
 *              bottom four dash switches.
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
#include "Prototypes.h"
#include "Prototypes_CAN.h"
#include "Constants.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "pku2400_control_1.h"
#include "pdm_test_harness.h"

uint8_t switch_index = 0;
uint8_t harness_index = 0;



/******************************************************************************
 *
 *        Name: switch_n()
 *
 * Description: These four functions are called by each of the lower
 *              four PKU2400 switches when any one of their switch
 *              states TOGGLE from ON to OFF or OFF to ON.
 *
 *              Each switch function sets or clears a bit (0, 1, 2, 4)
 *              of switch_index. Switch index is then used as an
 *              address of up to 16 four harness banks. Each addressed
 *              bank may control four outputs by mutually exclusive
 *              toggling the upper four PKU2400 switches.
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
void switch_8(bool_t switch_state)
{
    if (switch_state == TRUE)
    {
        switch_index |= 8;
    }
    else
    {
        switch_index &= 7;
    }
}

void switch_4(bool_t switch_state)
{
    if (switch_state == TRUE)
    {
        switch_index |= 4;
    }
    else
    {
        switch_index &= 0xb;
    }
}

void switch_2(bool_t switch_state)
{
    if (switch_state == TRUE)
    {
        switch_index |= 2;
    }
    else
    {
        switch_index &= 0xd;
    }
}

void switch_1(bool_t switch_state)
{
    if (switch_state == TRUE)
    {
        switch_index |= 1;
    }
    else
    {
        switch_index &= 0xe;
    }
}


void harness_8(bool_t harness_state)
{
    if (harness_state == TRUE)
    {
        harness_index |= 8;
    }
    else
    {
        harness_index &= 7;
    }
}

void harness_4(bool_t harness_state)
{
    if (harness_state == TRUE)
    {
        harness_index |= 4;
    }
    else
    {
        harness_index &= 0xb;
    }
}

void harness_2(bool_t harness_state)
{
    if (harness_state == TRUE)
    {
        harness_index |= 2;
    }
    else
    {
        harness_index &= 0xd;
    }
}

void harness_1(bool_t harness_state)
{
    if (harness_state == TRUE)
    {
        harness_index |= 1;
    }
    else
    {
        harness_index &= 0xe;
    }
}


/******************************************************************************
 *
 *        Name: harness_control()
 *
 * Description: Using the switch_index and the harness_index, which
 *              are updated by the PKU2400 calls to the above
 *              functions on a switch state change, the switch_index,
 *              set by the bottom four PKU switches in a binary
 *              counting pattern, is used to index into n groups of up
 *              to four PDM outputs that are each controlled by the
 *              state of the upper four PKU switches. Only one of the
 *              channel selection switches (upper switches) may be ON
 *              at a time. It is basically a test that turns ON/OFF on
 *              of the addressed PDM output channels at a time.
 *
 *              The state of the PDM outputs are controlled by calling
 *              the pdm_n_command_channel_() functions defined in
 *              pdm_control_1.c, pdm_control_2.c and pdm_control_3.c.
 *
 *              There are unused address position that may be used to
 *              call other control functions at the press of one of
 *              the PDK2400 switches.
 *
 *      Author: Tom
 *        Date: Friday, 16 August 2019
 *
 ******************************************************************************
 */
void harness_control()
{

    //
    // This is here to satisfy the requirement that each PDM receive a
    // command message at least once a second. The PDM will otherwise
    // go into low-power mode. All of the switches are OFF if the
    // harness_index is zero. Send an output command to the PDM to keep it
    // from going into loss of communication mode.
    //
    if (harness_index == 0)
    {
        pdm_1_command_spare_output(FALSE);
        pdm_2_command_start_drain_solenoid_pwr(FALSE);
        pdm_3_command_work_lights_pwr(FALSE);
    }

    //
    // Command the indexed PDM output to ON.
    //
    switch (switch_index)
    {
    case 0:
        switch (harness_index)
        {
            //
            // Harness address 0
            //
        case 1:
//            pdm_1_command_spare_output(TRUE);
//            pdm_1_command_left_spray_solenoid(FALSE);
//            pdm_1_command_center_spray_solenoid(FALSE);
//            pdm_1_command_right_spray_solenoid(FALSE);
            break;
        case 2:
//            pdm_1_command_spare_output(FALSE);
//            pdm_1_command_left_spray_solenoid(TRUE);
//            pdm_1_command_center_spray_solenoid(FALSE);
//            pdm_1_command_right_spray_solenoid(FALSE);
            break;
        case 4:
//            pdm_1_command_spare_output(FALSE);
//            pdm_1_command_left_spray_solenoid(FALSE);
//            pdm_1_command_center_spray_solenoid(TRUE);
//            pdm_1_command_right_spray_solenoid(FALSE);
            break;
        case 8:
//            pdm_1_command_spare_output(FALSE);
//            pdm_1_command_left_spray_solenoid(FALSE);
//            pdm_1_command_center_spray_solenoid(FALSE);
//            pdm_1_command_right_spray_solenoid(TRUE);
            break;
        default:
//            pdm_1_command_spare_output(FALSE);
//            pdm_1_command_left_spray_solenoid(FALSE);
//            pdm_1_command_center_spray_solenoid(FALSE);
//            pdm_1_command_right_spray_solenoid(FALSE);

        }
        break;
    case 1:
        switch (harness_index)
        {
            //
            // Harness address 1
            //
        case 1:
            pdm_1_command_reverse_lights_output(TRUE);
            pdm_1_command_brake_lights_output(FALSE);
            pdm_1_command_deck_lights_output(FALSE);
            pdm_1_command_brake_release_solenoid(FALSE);
                break;
        case 2:
            pdm_1_command_reverse_lights_output(FALSE);
            pdm_1_command_brake_lights_output(TRUE);
            pdm_1_command_deck_lights_output(FALSE);
            pdm_1_command_brake_release_solenoid(FALSE);
            break;
        case 4:
            pdm_1_command_reverse_lights_output(FALSE);
            pdm_1_command_brake_lights_output(FALSE);
            pdm_1_command_deck_lights_output(TRUE);
            pdm_1_command_brake_release_solenoid(FALSE);
            break;
        case 8:
            pdm_1_command_reverse_lights_output(FALSE);
            pdm_1_command_brake_lights_output(FALSE);
            pdm_1_command_deck_lights_output(FALSE);
            pdm_1_command_brake_release_solenoid(TRUE);
            break;
        default:
            pdm_1_command_reverse_lights_output(FALSE);
            pdm_1_command_brake_lights_output(FALSE);
            pdm_1_command_deck_lights_output(FALSE);
            pdm_1_command_brake_release_solenoid(FALSE);

        }
        break;
    case 2:
        switch (harness_index)
        {
            //
            // Harness address 2
            //
        case 1:
            pdm_1_command_reverse_beeper_output(TRUE);
            pdm_1_command_reverse_beacon_output(FALSE);
            break;
        case 2:
            pdm_1_command_reverse_beeper_output(FALSE);
            pdm_1_command_reverse_beacon_output(TRUE);
            break;
        case 4:
            pdm_1_command_reverse_beeper_output(FALSE);
            pdm_1_command_reverse_beacon_output(FALSE);
            break;
        case 8:
            pdm_1_command_reverse_beeper_output(FALSE);
            pdm_1_command_reverse_beacon_output(FALSE);
            break;
        default:
            pdm_1_command_reverse_beeper_output(FALSE);
            pdm_1_command_reverse_beacon_output(FALSE);

        }
        break;
    case 3:
        switch (harness_index)
        {
            //
            // Harness address 3
            //
        case 1:
            pdm_2_command_start_drain_solenoid_pwr(TRUE);
            pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(FALSE);
            pdm_2_command_1st_gear_solenoid_pwr(FALSE);
            pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
            break;
        case 2:
            pdm_2_command_start_drain_solenoid_pwr(FALSE);
            pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(TRUE);
            pdm_2_command_1st_gear_solenoid_pwr(FALSE);
            pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
            break;
        case 4:
            pdm_2_command_start_drain_solenoid_pwr(FALSE);
            pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(FALSE);
            pdm_2_command_1st_gear_solenoid_pwr(TRUE);
            pdm_2_command_2nd_gear_solenoid_pwr(FALSE);
            break;
        case 8:
            pdm_2_command_start_drain_solenoid_pwr(FALSE);
            pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(FALSE);
            pdm_2_command_1st_gear_solenoid_pwr(FALSE);
            pdm_2_command_2nd_gear_solenoid_pwr(TRUE);
            break;
        default:
            pdm_2_command_start_drain_solenoid_pwr(FALSE);
            pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(FALSE);
            pdm_2_command_1st_gear_solenoid_pwr(FALSE);
            pdm_2_command_2nd_gear_solenoid_pwr(FALSE);

        }
        break;
    case 4:
        switch (harness_index)
        {
            //
            // Harness address 4
            //
        case 1:
            pdm_2_command_3rd_gear_solenoid_pwr(TRUE);
            pdm_2_command_4th_gear_solenoid_pwr(FALSE);
            pdm_2_command_power_electronics_pump_pwr(FALSE);
            pdm_2_command_selector_solenoid_pwr(FALSE);
            break;
        case 2:
            pdm_2_command_3rd_gear_solenoid_pwr(FALSE);
            pdm_2_command_4th_gear_solenoid_pwr(TRUE);
            pdm_2_command_power_electronics_pump_pwr(FALSE);
            pdm_2_command_selector_solenoid_pwr(FALSE);
            break;
        case 4:
            pdm_2_command_3rd_gear_solenoid_pwr(FALSE);
            pdm_2_command_4th_gear_solenoid_pwr(FALSE);
            pdm_2_command_power_electronics_pump_pwr(TRUE);
            pdm_2_command_selector_solenoid_pwr(FALSE);
            break;
        case 8:
            pdm_2_command_3rd_gear_solenoid_pwr(FALSE);
            pdm_2_command_4th_gear_solenoid_pwr(FALSE);
            pdm_2_command_power_electronics_pump_pwr(FALSE);
            pdm_2_command_selector_solenoid_pwr(TRUE);
            break;
        default:
            pdm_2_command_3rd_gear_solenoid_pwr(FALSE);
            pdm_2_command_4th_gear_solenoid_pwr(FALSE);
            pdm_2_command_power_electronics_pump_pwr(FALSE);
            pdm_2_command_selector_solenoid_pwr(FALSE);

        }
        break;
    case 5:
        switch (harness_index)
        {
            //
            // Harness address 5
            //
        case 1:
            pdm_2_command_brake_release_solenoid_pwr(TRUE);
            pdm_2_command_service_brake_solenoid_pwr(FALSE);
            pdm_2_command_hydraulic_motor_cooling_pump_pwr(FALSE);
            pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(FALSE);
            break;
        case 2:
            pdm_2_command_brake_release_solenoid_pwr(FALSE);
            pdm_2_command_service_brake_solenoid_pwr(TRUE);
            pdm_2_command_hydraulic_motor_cooling_pump_pwr(FALSE);
            pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(FALSE);
            break;
        case 4:
            pdm_2_command_brake_release_solenoid_pwr(FALSE);
            pdm_2_command_service_brake_solenoid_pwr(FALSE);
            pdm_2_command_hydraulic_motor_cooling_pump_pwr(TRUE);
            pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(FALSE);
            break;
        case 8:
            pdm_2_command_brake_release_solenoid_pwr(FALSE);
            pdm_2_command_service_brake_solenoid_pwr(FALSE);
            pdm_2_command_hydraulic_motor_cooling_pump_pwr(FALSE);
            pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(TRUE);
            break;
        default:
            pdm_2_command_brake_release_solenoid_pwr(FALSE);
            pdm_2_command_service_brake_solenoid_pwr(FALSE);
            pdm_2_command_hydraulic_motor_cooling_pump_pwr(FALSE);
            pdm_2_command_hydraulic_motor_radiator_cooling_fan_pwr(FALSE);

        }
        break;
    case 6:
        switch (harness_index)
        {
            //
            // Harness address 6
            //
        case 1:
            pdm_3_command_work_lights_pwr(TRUE);
            pdm_3_command_beacon_pwr(FALSE);
            pdm_3_command_head_light_pwr(FALSE);
            pdm_3_command_horn_pwr(FALSE);
            break;
        case 2:
            pdm_3_command_work_lights_pwr(FALSE);
            pdm_3_command_beacon_pwr(TRUE);
            pdm_3_command_head_light_pwr(FALSE);
            pdm_3_command_horn_pwr(FALSE);
            break;
        case 4:
            pdm_3_command_work_lights_pwr(FALSE);
            pdm_3_command_beacon_pwr(FALSE);
            pdm_3_command_head_light_pwr(TRUE);
            pdm_3_command_horn_pwr(FALSE);
            break;
        case 8:
            pdm_3_command_work_lights_pwr(FALSE);
            pdm_3_command_beacon_pwr(FALSE);
            pdm_3_command_head_light_pwr(FALSE);
            pdm_3_command_horn_pwr(TRUE);
            break;
        default:
            pdm_3_command_work_lights_pwr(FALSE);
            pdm_3_command_beacon_pwr(FALSE);
            pdm_3_command_head_light_pwr(FALSE);
            pdm_3_command_horn_pwr(FALSE);

        }
        break;
    case 7:
        switch (harness_index)
        {
            //
            // Harness address 7
            //
        case 1:
            pdm_3_command_charger_1_busbar_pwr(TRUE);
            pdm_3_command_drive_motor_cooling_pump_pwr(FALSE);
            pdm_3_command_imt_cooling_pump_pwr(FALSE);
            pdm_3_command_fan_radiator_1_2_pwr(FALSE);
            break;
        case 2:
            pdm_3_command_charger_1_busbar_pwr(FALSE);
            pdm_3_command_drive_motor_cooling_pump_pwr(TRUE);
            pdm_3_command_imt_cooling_pump_pwr(FALSE);
            pdm_3_command_fan_radiator_1_2_pwr(FALSE);
            break;
        case 4:
            pdm_3_command_charger_1_busbar_pwr(FALSE);
            pdm_3_command_drive_motor_cooling_pump_pwr(FALSE);
            pdm_3_command_imt_cooling_pump_pwr(TRUE);
            pdm_3_command_fan_radiator_1_2_pwr(FALSE);
            break;
        case 8:
            pdm_3_command_charger_1_busbar_pwr(FALSE);
            pdm_3_command_drive_motor_cooling_pump_pwr(FALSE);
            pdm_3_command_imt_cooling_pump_pwr(FALSE);
            pdm_3_command_fan_radiator_1_2_pwr(TRUE);
            break;
        default:
            pdm_3_command_charger_1_busbar_pwr(FALSE);
            pdm_3_command_drive_motor_cooling_pump_pwr(FALSE);
            pdm_3_command_imt_cooling_pump_pwr(FALSE);
            pdm_3_command_fan_radiator_1_2_pwr(FALSE);

        }
        break;
    case 8:
        switch (harness_index)
        {
            //
            // Harness address 8
            //
        case 1:
            pdm_3_command_pump_charger_1_cooling_pwr(TRUE);
            pdm_3_command_pump_charger_2_cooling_pwr(FALSE);
            pdm_3_command_main_bussbar_supply_pwr(FALSE);
            pdm_3_command_hydraulic_fan_solenoid_pwr(FALSE);
            break;
        case 2:
            pdm_3_command_pump_charger_1_cooling_pwr(FALSE);
            pdm_3_command_pump_charger_2_cooling_pwr(TRUE);
            pdm_3_command_main_bussbar_supply_pwr(FALSE);
            pdm_3_command_hydraulic_fan_solenoid_pwr(FALSE);
            break;
        case 4:
            pdm_3_command_pump_charger_1_cooling_pwr(FALSE);
            pdm_3_command_pump_charger_2_cooling_pwr(FALSE);
            pdm_3_command_main_bussbar_supply_pwr(TRUE);
            pdm_3_command_hydraulic_fan_solenoid_pwr(FALSE);
            break;
        case 8:
            pdm_3_command_pump_charger_1_cooling_pwr(FALSE);
            pdm_3_command_pump_charger_2_cooling_pwr(FALSE);
            pdm_3_command_main_bussbar_supply_pwr(FALSE);
            pdm_3_command_hydraulic_fan_solenoid_pwr(TRUE);
            break;
        default:
            pdm_3_command_pump_charger_1_cooling_pwr(FALSE);
            pdm_3_command_pump_charger_2_cooling_pwr(FALSE);
            pdm_3_command_main_bussbar_supply_pwr(FALSE);
            pdm_3_command_hydraulic_fan_solenoid_pwr(FALSE);
        }
        break;
    case 9:
        switch (harness_index)
        {
            //
            // Harness address 9
            //
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        }
        break;
    case 10:
        switch (harness_index)
        {
            //
            // Harness address 10
            //
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        }
        break;
    case 11:
        switch (harness_index)
        {
            //
            // Harness address 11
            //
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        }
        break;
    case 12:
        switch (harness_index)
        {
            //
            // Harness address 12
            //
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        }
        break;
    case 13:
        switch (harness_index)
        {
            //
            // Harness address 13
            //
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        }
        break;
    case 14:
        switch (harness_index)
        {
            //
            // Harness address 14
            //
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        }
        break;
    case 15:
        switch (harness_index)
        {
            //
            // Harness address 15
            //
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        }
        break;
    }

}
