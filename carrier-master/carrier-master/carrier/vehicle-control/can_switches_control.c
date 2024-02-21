/******************************************************************************
 *
 *        Name: can_switches_control.c
 *
 * Description: A module that controls the working of the can switches
 *              on the carrier.
 *
 *
 *      Author: Deepak
 *        Date: Friday, 27 September 2019
 *
 ******************************************************************************
 */
#include <math.h>
#include <stdlib.h>
#include "Prototypes.h"
#include "Prototypes_CAN.h"
#include "Constants.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "pku2400_control_1.h"
#include "can_switches.h"
#include "cvc_debug_msgs.h"

static bool_t ignition_state = FALSE;
static bool_t charger_enable = FALSE;
static bool_t brake_test_mode = FALSE;
 bool_t left_solenoid_sprayer_status = FALSE;
 bool_t middle_solenoid_sprayer_status = FALSE;
 bool_t right_solenoid_sprayer_status = FALSE;
 bool_t bottom_switch_1_status  = FALSE;

//=============================================================================
//
// Front Lights
//
//=============================================================================
//
void top_switch_1(bool_t switch_state)
{

    //
    // Command Head Lights
    //
    pdm_3_command_head_light_pwr(switch_state);
    pdm_3_command_work_lights_pwr(switch_state);
}

//=============================================================================
//
// Ignition
//
//=============================================================================
//
void top_switch_2(bool_t switch_state)
{

    //
    // Still to be decided
    //
    // ignition_state = switch_state;

    // Brake test mode
    brake_test_mode = switch_state;

}

//=============================================================================
//
// Beacon
//
//=============================================================================
//
void top_switch_3(bool_t switch_state)
{

    //
    // enable\disable beacon based on state
    //
    // pdm_3_command_beacon_pwr(switch_state);

    //
    // Remapped_4th_gear_solenoid to beacon
    //
    pdm_2_command_4th_gear_solenoid_pwr(switch_state);

    //
    // enable_disable_charger
    //
    // charger_enable = switch_state;
}


//=============================================================================
//
// Horn
//
//=============================================================================
//
void top_switch_4(bool_t switch_state)
{
    //
    // Enables/Disables the Horn on the carrier
    //
    pdm_3_command_beacon_pwr(switch_state);

}

//=============================================================================
//
// Rear Lights
//
//=============================================================================
//
void bottom_switch_1(bool_t switch_state)
{

    //
    // Rear lights
    //
    pdm_1_command_reverse_lights_output(switch_state);
    pdm_1_command_deck_lights_output(switch_state);
    bottom_switch_1_status = switch_state;

    
}

//=============================================================================
//
// Left Sprayer Solenoid
//
//=============================================================================
//
void bottom_switch_2(bool_t switch_state)
{

    //
    // Enable/disable left sprayer
    //
    pdm_1_command_left_spray_solenoid(switch_state);
    left_solenoid_sprayer_status = switch_state;

}

//=============================================================================
//
// Middle Sprayer Solenoid
//
//=============================================================================
//
void bottom_switch_3(bool_t switch_state)
{

    //
    // Center Solenoid
    //
    pdm_1_command_center_spray_solenoid(switch_state);

    middle_solenoid_sprayer_status = switch_state;

}

//=============================================================================
//
// Right Sprayer Solenoid
//
//=============================================================================
//
void bottom_switch_4(bool_t switch_state)
{

    //
    // Right Sprayer Solenoid
    //
    pdm_1_command_right_spray_solenoid(switch_state);

    right_solenoid_sprayer_status = switch_state;

}


//=============================================================================
//
//
//
//=============================================================================
//
bool_t ignition()
{

    return ignition_state;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t enable_charger_can_switch()
{

    return charger_enable;

}


//=============================================================================
//
//
//
//=============================================================================
//
bool_t sprayer_solenoids_status()
{

    bool_t solenoid_status = (left_solenoid_sprayer_status
                              || middle_solenoid_sprayer_status
                              || right_solenoid_sprayer_status);

    return solenoid_status;

}

//=============================================================================
//
//
//
//=============================================================================
//
bool_t brake_test_mode_status()
{
    return brake_test_mode;
}
