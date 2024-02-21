/******************************************************************************
 *
 *        Name: can_switches_pku2400_setters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Tuesday, 09 Aug 2019
 *
 ******************************************************************************
 */
#include "Prototypes.h"
#include "pku2400_device_private.h"


//=============================================================================
//
// A pointer to the first registeration record of the pku2400
//
//=============================================================================
//
extern device_data_t *can_switches_pku2400_first_device_data_ptr;


//=============================================================================
//
// pku2400_set_change_led_colour(): Changes the colour of the led for
//                                  a switch based for a particular
//                                  switch name.
//
//=============================================================================
//
void pku2400_set_change_led_colour(
    device_instances_t device,
    can_switch_t switch_name,
    can_switch_colour_t colour)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


    // Based on the switch, change colour of the desired LED
    switch (switch_name)
    {
    case SWITCH_BOTTOM_1_BIT:
        pku2400_set_led_colour_bottom_switch_1(device, RED, LED_OFF);
        pku2400_set_led_colour_bottom_switch_1(device, GREEN, LED_OFF);
        pku2400_set_led_colour_bottom_switch_1(device, colour, LED_ON);
        break;

    case SWITCH_BOTTOM_2_BIT:
        pku2400_set_led_colour_bottom_switch_2(device, RED, LED_OFF);
        pku2400_set_led_colour_bottom_switch_2(device, GREEN, LED_OFF);
        pku2400_set_led_colour_bottom_switch_2(device, colour, LED_ON);
        break;

    case SWITCH_BOTTOM_3_BIT:
        pku2400_set_led_colour_bottom_switch_3(device, RED, LED_OFF);
        pku2400_set_led_colour_bottom_switch_3(device, GREEN, LED_OFF);
        pku2400_set_led_colour_bottom_switch_3(device, colour, LED_ON);
        break;

    case SWITCH_BOTTOM_4_BIT:
        pku2400_set_led_colour_bottom_switch_4(device, RED, LED_OFF);
        pku2400_set_led_colour_bottom_switch_4(device, GREEN, LED_OFF);
        pku2400_set_led_colour_bottom_switch_4(device, colour, LED_ON);
        break;

    case SWITCH_TOP_1_BIT:
        pku2400_set_led_colour_top_switch_1(device, RED, LED_OFF);
        pku2400_set_led_colour_top_switch_1(device, GREEN, LED_OFF);
        pku2400_set_led_colour_top_switch_1(device, colour, LED_ON);
        break;

    case SWITCH_TOP_2_BIT:
        pku2400_set_led_colour_top_switch_2(device, RED, LED_OFF);
        pku2400_set_led_colour_top_switch_2(device, GREEN, LED_OFF);
        pku2400_set_led_colour_top_switch_2(device, colour, LED_ON);
        break;

    case SWITCH_TOP_3_BIT:
        pku2400_set_led_colour_top_switch_3(device, RED, LED_OFF);
        pku2400_set_led_colour_top_switch_3(device, GREEN, LED_OFF);
        pku2400_set_led_colour_top_switch_3(device, colour, LED_ON);
        break;

    case SWITCH_TOP_4_BIT:
        pku2400_set_led_colour_top_switch_4(device, RED, LED_OFF);
        pku2400_set_led_colour_top_switch_4(device, GREEN, LED_OFF);
        pku2400_set_led_colour_top_switch_4(device, colour, LED_ON);
        break;

    default:
        pku2400_set_all_leds_to_off_state(device);
    }
}


//=============================================================================
//
// pku2400_led_colour_top_switch_1
//
//=============================================================================
//
void pku2400_set_led_colour_top_switch_1(
    device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	if (state == LED_ON)
        	{
            	dest_ptr->red_led_top_switch_1 = 1;
        	}
        	else
        	{
            	dest_ptr->red_led_top_switch_1 = 0;
        	}
        	break;

        case BLUE:
//           	dest_ptr->blue_led_top_switch_1 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_top_switch_1 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_top_switch_1 = state;;
        	dest_ptr->green_led_top_switch_1 = state;
        	break;
    }
}

//=============================================================================
//
// pku2400_led_colour_top_switch_2
//
//=============================================================================
//
void pku2400_set_led_colour_top_switch_2(
	device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	dest_ptr->red_led_top_switch_2 = state;
        	break;

        case BLUE:
//           	dest_ptr->blue_led_top_switch_2 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_top_switch_2 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_top_switch_2 = state;
        	dest_ptr->green_led_top_switch_2 = state;
        	break;
    }
}


//=============================================================================
//
// pku2400_led_colour_top_switch_3
//
//=============================================================================
//
void pku2400_set_led_colour_top_switch_3(
	device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	dest_ptr->red_led_top_switch_3 = state;

        case BLUE:
           	// dest_ptr->blue_led_top_switch_3 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_top_switch_3 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_top_switch_3 = state;
        	dest_ptr->green_led_top_switch_3 = state;
        	break;
    }
}

//=============================================================================
//
// pku2400_led_colour_top_switch_4
//
//=============================================================================
//
void pku2400_set_led_colour_top_switch_4(
	device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	dest_ptr->red_led_top_switch_4 = state;
        	break;

        case BLUE:
//           	dest_ptr->blue_led_top_switch_4 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_top_switch_4 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_top_switch_4 = state;
        	dest_ptr->green_led_top_switch_4 = state;
        	break;
    }
}


//=============================================================================
//
// pku2400_led_colour_bottom_switch_1
//
//=============================================================================
//
void pku2400_set_led_colour_bottom_switch_1(
	device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	dest_ptr->red_led_bottom_switch_1 = state;
        	break;

        case BLUE:
//           	dest_ptr->blue_led_bottom_switch_1 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_bottom_switch_1 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_bottom_switch_1 = state;
        	dest_ptr->green_led_bottom_switch_1 = state;
        	break;
    }
}

//=============================================================================
//
// pku2400_led_colour_bottom_switch_2
//
//=============================================================================
//
void pku2400_set_led_colour_bottom_switch_2(
	device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	dest_ptr->red_led_bottom_switch_2 = state;
        	break;

        case BLUE:
//           	dest_ptr->blue_led_bottom_switch_2 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_bottom_switch_2 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_bottom_switch_2 = state;
        	dest_ptr->green_led_bottom_switch_2 = state;
        	break;
    }
}


//=============================================================================
//
// pku2400_led_colour_bottom_switch_3
//
//=============================================================================
//
void pku2400_set_led_colour_bottom_switch_3(
	device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	dest_ptr->red_led_bottom_switch_3 = state;
        	break;

        case BLUE:
//           	dest_ptr->blue_led_bottom_switch_3 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_bottom_switch_3 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_bottom_switch_3 = state;
        	dest_ptr->green_led_bottom_switch_3 = state;
        	break;
    }
}


//=============================================================================
//
// pku2400_led_colour_bottom_switch_4
//
//=============================================================================
//
void pku2400_set_led_colour_bottom_switch_4(
	device_instances_t device,
    can_switch_colour_t colour,
    led_state_t state)
{

	can_switches_pku2400_tx_led_control_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_led_control);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    switch (colour)
    {
        case RED:
        	dest_ptr->red_led_bottom_switch_4 = state;
        	break;

        case BLUE:
//           	dest_ptr->blue_led_bottom_switch_4 = state;
           	break;

        case GREEN:
           	dest_ptr->green_led_bottom_switch_4 = state;
            break;

        case BROWN:
        	dest_ptr->red_led_bottom_switch_4 = state;
        	dest_ptr->green_led_bottom_switch_4 = state;
        	break;
    }
}

//=============================================================================
//
// pku2400_all_leds_to_off_state(device_instances_t device)
//
//=============================================================================
//
void pku2400_set_all_leds_to_off_state(
    device_instances_t device)
{

    pku2400_set_led_initial_state(device, LED_OFF);

}

//=============================================================================
//
// A Helper Function to Toggle Any Led whose state has changed when
// configured as a TOGGLE SWITCH
//
//=============================================================================
//
void pku2400_toggle_led_based_on_state(
    device_instances_t device)
{

    bool_t top1 = pku2400_get_status_switch_toggle_state(device, SWITCH_TOP_1_BIT);
    bool_t top2 = pku2400_get_status_switch_toggle_state(device, SWITCH_TOP_2_BIT);
    bool_t top3 = pku2400_get_status_switch_toggle_state(device, SWITCH_TOP_3_BIT);
    bool_t top4 = pku2400_get_status_switch_toggle_state(device, SWITCH_TOP_4_BIT);

    bool_t bottom1 = pku2400_get_status_switch_toggle_state(device, SWITCH_BOTTOM_1_BIT);
    bool_t bottom2 = pku2400_get_status_switch_toggle_state(device, SWITCH_BOTTOM_2_BIT);
    bool_t bottom3 = pku2400_get_status_switch_toggle_state(device, SWITCH_BOTTOM_3_BIT);
    bool_t bottom4 = pku2400_get_status_switch_toggle_state(device, SWITCH_BOTTOM_4_BIT);

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    // Check to see if a change in state has occured
    if (device_data_ptr->switch_states[0].previous_state != bottom1)
    {
        if (bottom1 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_1_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_1_BIT, RED);
        }

    }


    if (device_data_ptr->switch_states[1].previous_state != bottom2)
    {
        if(bottom2 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_2_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_2_BIT, RED);
        }

    }

    if (device_data_ptr->switch_states[2].previous_state != bottom3)
    {
        if (bottom3 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_3_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_3_BIT, RED);
        }

    }

    if (device_data_ptr->switch_states[3].previous_state != bottom4)
    {
        if(bottom4 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_4_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_BOTTOM_4_BIT, RED);
        }

    }


    if (device_data_ptr->switch_states[4].previous_state != top1)
    {
        if (top1 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_1_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_1_BIT, RED);
        }

    }


    if (device_data_ptr->switch_states[5].previous_state != top2)
    {
        if (top2 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_2_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_2_BIT, RED);
        }

    }

    if (device_data_ptr->switch_states[6].previous_state != top3)
    {
        if (top3 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_3_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_3_BIT, RED);
        }

    }

    if (device_data_ptr->switch_states[7].previous_state != top4)
    {
        if (top4 == TRUE)
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_4_BIT, GREEN);
        }
        else
        {
            pku2400_set_change_led_colour(device, SWITCH_TOP_4_BIT, RED);
        }
    }
}


/******************************************************************************
 *
 *        Name: pku2400_set_switch_toggled_state()
 *
 * Description:
 *
 *
 ******************************************************************************
 */
void pku2400_set_switch_toggled_state(
    device_instances_t device,
    can_switch_t can_switch_name,
    bool_t state)
{

    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);

    device_data_ptr->switch_states[can_switch_name].toggled = state;

    return;


}
