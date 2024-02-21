/******************************************************************************
 *
 *        Name: can_switches_pku2400_device_getters.c
 *
 * Description: All of the getter functions for the device_name.
 *
 *      Author: Tom
 *        Date: Thursday, 04 July 2019
 *
 ******************************************************************************
 */
#include "pku2400_device_private.h"

//=============================================================================
//
// pointer of the first registration record of the PKU2400 module
//
//=============================================================================
//
extern device_data_t *can_switches_pku2400_first_device_data_ptr;

/******************************************************************************
 *
 *        Name: get_status_momemtary_input_state()
 *
 * Description: Returns the instananeous state of the switch. This
 *              function would be called when the can_button is
 *              configured as a momemtary switch. For Example: vehicle
 *              horn.
 *
 *      Author: Deepak
 *        Date: Tuesday, 06 August 2019
 *
 ******************************************************************************
 */
bool_t pku2400_get_status_momentary_input_state(device_instances_t device,
                                          can_switch_t can_switch_number)
{
	bool_t momentary_state = FALSE;

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Depending on the can_switch_number, return the instantenous  state
    // value of the CAN switch
    //
    switch (can_switch_number)
    {

    case SWITCH_BOTTOM_1_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.bottom_switch_1;
        break;

    case SWITCH_BOTTOM_2_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.bottom_switch_2;
        break;

    case SWITCH_BOTTOM_3_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.bottom_switch_3;
        break;

    case SWITCH_BOTTOM_4_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.bottom_switch_4;
        break;

    case SWITCH_TOP_1_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.top_switch_1;
        break;

    case SWITCH_TOP_2_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.top_switch_2;
        break;

    case SWITCH_TOP_3_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.top_switch_3;
        break;

    case SWITCH_TOP_4_BIT:
        momentary_state = (bool_t)device_data_ptr->can_switch_status.top_switch_4;
        break;

    default:
        momentary_state = 0;
        break;
    }

    return momentary_state;
}

/******************************************************************************
 *
 *        Name: get_status_switch_toggle_state()
 *
 * Description: Returns the toggle state of the switch based on the
 *              rising edge. This function would be called when the
 *              can_button is configured as a toggle switch. For
 *              Example: sprayer.
 *
 *      Author: Deepak
 *        Date: Tuesday, 06 August 2019
 *
 ******************************************************************************
 */
bool_t pku2400_get_status_switch_toggle_state(device_instances_t device,
                                      can_switch_t can_switch_number)
{
    bool_t toggle_state = 0;

    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Depending on the can_switch_number, return the toggle state
    // value of the CAN switch
    //
    switch (can_switch_number)
    {

        case SWITCH_BOTTOM_1_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_BOTTOM_1_BIT].output_toggle_switch_state;
            break;

        case SWITCH_BOTTOM_2_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_BOTTOM_2_BIT].output_toggle_switch_state;
            break;

        case SWITCH_BOTTOM_3_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_BOTTOM_3_BIT].output_toggle_switch_state;
            break;

        case SWITCH_BOTTOM_4_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_BOTTOM_4_BIT].output_toggle_switch_state;
            break;

        case SWITCH_TOP_1_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_TOP_1_BIT].output_toggle_switch_state;
            break;

        case SWITCH_TOP_2_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_TOP_2_BIT].output_toggle_switch_state;
            break;

        case SWITCH_TOP_3_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_TOP_3_BIT].output_toggle_switch_state;
            break;

        case SWITCH_TOP_4_BIT:
            toggle_state =
                device_data_ptr->switch_states[SWITCH_TOP_4_BIT].output_toggle_switch_state;
            break;

    default:
        toggle_state = FALSE;
        DEBUG("Something went wrong");
    }

    return toggle_state;
}


/******************************************************************************
 *
 *        Name: get_switch_previous_state()
 *
 * Description: return the previous state of the desired switch
 *
 *
 ******************************************************************************
 */
bool_t pku2400_get_switch_previous_state(
    device_instances_t device,
    can_switch_t can_switch_name)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    bool_t previous_state =
        device_data_ptr->switch_states[can_switch_name].previous_state;

    return previous_state;

}

/******************************************************************************
 *
 *        Name: pku2400_get_switch_toggle_state()
 *
 * Description:
 *
 ******************************************************************************
 */
bool_t pku2400_get_switch_toggled_state(
    device_instances_t device,
    can_switch_t can_switch_name)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    bool_t toggled_state =
        device_data_ptr->switch_states[can_switch_name].toggled;

    return toggled_state;
}

