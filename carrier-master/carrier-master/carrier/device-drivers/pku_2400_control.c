/******************************************************************************
 *
 *        Name: can_switch_service.h
 *
 * Description: A virtual device driver that register interest in all
 *              the can buttons used on the machine. A CAN button can
 *              be configured to operate either as a toggle button or
 *              momemtary switch.
 *
 *              Using this function, the CAN button is operation is
 *              defined, along with the function to be called when the
 *              can button changes state.
 *
 *
 *      Author: Deepak
 *        Date: Thursday, 08 August 2019
 *
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "typedefs.h"
#include "Prototypes.h"
#include "Prototypes_CAN.h"
#include "can_service_private.h"
#include "can_service_devices.h"
#include "pku2400_device.h"


/******************************************************************************
 *
 * A pointer to the first device data pointer of the CAN switches
 *
 ******************************************************************************
 */
extern device_data_t *can_switches_pku2400_first_device_data_ptr;

/******************************************************************************
 *
 * Create a struct for the can buttons. These structs are linked
 * together to form a linked list of structs. The linked list of
 * structs is called a registration record. A registration record is
 * created for every CAN switched that is used on the machine. The
 * linked list stores information regarding a particular switch. This
 * information is used later to check the status of the switches and
 * to decide the appropriate action to be taken if a switch changes
 * state.
 *
 * The registeration record also hold a function pointer. The function
 * to be called if a CAN switch changes state is stored in this
 * function pointer.
 *
 ******************************************************************************
 */
typedef struct can_buttons_registration_s
{
    //
    // These structure records are linked from one to another.  The
    // last structure will have it's next_ptr == NULL.
    //
    struct can_buttons_registration_s *next_ptr;

    // Register the name of the switch
    can_switch_t can_switch_name;

    // The device_index is zero-indexed. Use the enums ONE, TWO,
    // THREE, ... which equate to 0, 1, 2, ...
    device_instances_t device;

    // Configure the can button to either momentary type or toggle
    // type.
    can_button_config_t button_configuration;

    // function pointer
    p_can_buttons_f_t handler_function_p;

} can_buttons_registration_t;


/******************************************************************************
 *
 * A pointer to the first registeration record of the linked list of
 * registration records (can_buttons_registration_t)
 *
 ******************************************************************************
*/
can_buttons_registration_t *first_can_buttons_registration_record_p;


/******************************************************************************
 *
 * Function Calls to support the registeration process
 *
 ******************************************************************************
 */
static can_buttons_registration_t
    *get_last_can_buttons_registration_record_pointer();

static can_buttons_registration_t
    * search_can_buttons_registration_records_for_switch_configuration(
    device_instances_t device,
    can_switch_t switch_name);


//bool_t
//can_buttons_register_functions(
//    device_instances_t device_instance,
//    can_button_config_t button_config,
//    can_switch_t can_switch_name,
//    p_can_buttons_f_t handler_function_p);


/******************************************************************************
 *
 *        Name: can_buttons_register_functions()
 *
 * Description: This function, typically called from the can_switches
 *              device driver, registers an interest in a can_switches
 *              with a operation. These registrations are stored in a
 *              linked list of can_rx_registration_t structs. Each
 *              can_id registered must be unique in its composition of
 *              device_instances, button_configuration, handler_function
 *
 *
 *               Used to initialise the a CAN switches. The function
 *                      takes the name of CAN switch, along with the
 *                      desired can_switch_configuration, the can line
 *                      the switch is present and the function to be
 *                      called when a state change occurs in the
 *                      switch.
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
bool_t
pku2400_configure_switches(
    device_instances_t device_instance,
    can_button_config_t button_config,
    can_switch_t can_switch_name,
    p_can_buttons_f_t handler_function_p)
{
	can_buttons_registration_t *can_buttons_registration_record_p;

    //
    // Check if this can_switch_name has already been
    // registered.
    //
    if (search_can_buttons_registration_records_for_switch_configuration(device_instance, can_switch_name) != NULL)
    {
        //
        // Report an error. The specified can_switch has already been
        // registered.
        //
        DEBUG("Registration record already exists!");
        return FALSE;
    }
    else
    {
        //
        // Either the can_switch was not found in an existing record or
        // there are no records yet. Allocate and populate the next
        // registration record.
        //

        //
        // Get the last registration record address in the linked list
        // or return NULL if no records exist yet.
        //
        can_buttons_registration_record_p = get_last_can_buttons_registration_record_pointer();

        //
        // Check if NULL returned, indicating that no records exist yet.
        //
        if (can_buttons_registration_record_p == NULL)
        {
            //
            // No records exist in our linked list. Create the first
            // record.
            //
            first_can_buttons_registration_record_p =
                (can_buttons_registration_t *)malloc(sizeof(can_buttons_registration_t));
            can_buttons_registration_record_p = first_can_buttons_registration_record_p;
        }
        else
        {
            //
            // At least one record already exists. Create the new
            // record and link it with the current record.
            //
            can_buttons_registration_record_p->next_ptr =
                (can_buttons_registration_t *)malloc(sizeof(can_buttons_registration_t));
            can_buttons_registration_record_p = can_buttons_registration_record_p->next_ptr;
        }

        //
        // Check the return value from malloc.
        //
        if (can_buttons_registration_record_p == NULL)
        {
            //
            // Not enough space on the heap to satisfy our
            // request. This never returns.
            //
            DEBUG("Malloc error, no space on heap!");
            return FALSE;
        }

        //
        // Populate the newly created registration record.
        //
        can_buttons_registration_record_p->next_ptr = NULL;
        can_buttons_registration_record_p->can_switch_name = can_switch_name;
        can_buttons_registration_record_p->device = device_instance;
		can_buttons_registration_record_p->button_configuration = button_config;
        can_buttons_registration_record_p->handler_function_p = handler_function_p;
    }
    return TRUE;
}


/******************************************************************************
 *
 *        Name: search_can_buttons_registration_records_for_can_id()
 *
 * Description: Search through the linked list of
 *              can_button_registeration_t structures looking for a
 *              matching can_id.
 *
 *              Returns a pointer to the matching record or NULL if no
 *              match was found.
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
static can_buttons_registration_t *
search_can_buttons_registration_records_for_switch_configuration(
    device_instances_t device,
    can_switch_t switch_name)
{
    can_buttons_registration_t *can_buttons_registration_record_p;

    //
    // Point to the first registration record in the linked list of
    // records. Searches always start from the first record and work
    // their way through all of the linked records.
    //
    can_buttons_registration_record_p = first_can_buttons_registration_record_p;

    //
    // Check if the first record has been created. If not,
    // can_buttons_registration_record_p will be NULL and function returns.
    //
    if (can_buttons_registration_record_p != NULL)
    {
        //
        // The first registration record was not NULL. Proceed to
        // iterate through the linked list of records, looking for a
        // record with a matching can_id.
        //
        do
        {
            //
            // Check if the current registration record matches the
            // module_id and can_id.
            //
            if((can_buttons_registration_record_p->can_switch_name == switch_name) &&
               (can_buttons_registration_record_p->device == device))
            {
                //
                // A matching record has been found, break out of the
                // loop. Can_Buttons_Registration_record_p points to the matching
                // record.
                //
                break;
            }

            //
            // A matching record has not yet been found. Point to the
            // next record.
            //
            can_buttons_registration_record_p = can_buttons_registration_record_p->next_ptr;

            //
            // Loop until the end of the linked list of records.
            //
        } while(can_buttons_registration_record_p != NULL);
    }

    //
    // Return either a pointer to the matching record in the linked
    // list or NULL if no records have yet been created or a match was
    // not found.
    //
    return can_buttons_registration_record_p;
}


/******************************************************************************
 *
 *        Name: get_last_can_buttons_registration_record_pointer()
 *
 * Description: Search through the linked list of
 *              can_button_registration records looking for the last
 *              record. Return a pointer to the last record so that a
 *              new record may be added.
 *
 *      Author: Deepak
 *      Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
static
can_buttons_registration_t *
get_last_can_buttons_registration_record_pointer()
{
    can_buttons_registration_t *can_buttons_registration_record_p;

    //
    // Point to the first registration record in the linked list of
    // records. Searches always start from the first record and work
    // their way through all of the linked records.
    //
    can_buttons_registration_record_p = first_can_buttons_registration_record_p;

    //
    // Check if the first record has been created. If not,
    // rx_registration_record_p will be NULL and function returns.
    //
    if (can_buttons_registration_record_p != NULL)
    {
        //
        // The first registration record was not NULL.  Iterate
        // through the registration records until the record with the
        // next_ptr == NULL is found. This will be the last record in
        // the linked list.
        //
        while (can_buttons_registration_record_p->next_ptr != NULL)
        {
            can_buttons_registration_record_p = can_buttons_registration_record_p->next_ptr;
        }
    }

    //
    // Return either a pointer to the last record in the linked list
    // or NULL if no records have yet been created.
    //
    return can_buttons_registration_record_p;

}

/******************************************************************************
 *
 *        Name: check_can_switches_state()
 *
 * Description: The function checks the state of the switches. Calls
 *              the respective handler function. it also handles the
 *              change of LED backlight colours(tranmits the message
 *              to take care of the change.)
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void pku2400_check_switches(device_instances_t device)
{
    can_buttons_registration_t *can_buttons_registration_record_p = NULL;

    //
    // Search our linked list of can_buttons_registration_t records
    // for a record that matches the provided CAN identifier.
    // The CAN identifier is a member of the
    // received_can_messate structure.
    //
    can_buttons_registration_record_p =
        first_can_buttons_registration_record_p;

    //
    // Loop through all of the registration records for the specified
    // (device) PKU.
    //
    do
    {
        //
        // Depending on the switch configuration execute the
        // approprite section
        //
        switch(can_buttons_registration_record_p->button_configuration)
        {

        case MOMENTARY_SWITCH:
            //
            // If the switch was configured as a momentary
            // switch. execute the hander function every
            // time we come into this loop.
            //
            bool_t result = FALSE;

            result =
                pku2400_get_status_momentary_input_state(
                    device,
                    can_buttons_registration_record_p->can_switch_name);

            can_buttons_registration_record_p->handler_function_p(result);

            //
            // Update the switch's LED backlight based on
            // result ON/OFF.
            //
            if (result == TRUE)
            {
                pku2400_set_change_led_colour(
                    device,
                    can_buttons_registration_record_p->can_switch_name,
                    GREEN);
            }
            else
            {
                if(IN_A11_keyswitch < 4000)
                {
                    pku2400_set_change_led_colour(
                        device,
                        can_buttons_registration_record_p->can_switch_name ,
                        OFF);

                }
                else
                {

                    pku2400_set_change_led_colour(
                        device,
                        can_buttons_registration_record_p->can_switch_name ,
                        BROWN);

                }

            }

            // Once configuration of the LED backlights have taken place
            // Transmit the can message.
            pku2400_tx_led_controls(device, EVERY_100MS);

            break;

        case TOGGLE_SWITCH:

            if (pku2400_get_switch_toggled_state(
                    device,
                    can_buttons_registration_record_p->can_switch_name) == TRUE)
            {
                //
                // Clear this switches toggled state to FASLE so that
                // execution will not come back to this point until
                // there's been a change in the toggled state as set
                // in the device driver receive CAN function.
                //
                pku2400_set_switch_toggled_state(
                    device,
                    can_buttons_registration_record_p->can_switch_name,
                    FALSE);

                //
                // Check the toggle state of the switch and store
                // the result. Compare the present state to the
                // previous state. If there is a change in
                // state. Then change the colour of the led
                // backlight. Transmit a message to carry out the
                // action.
                //
                bool_t result =
                    pku2400_get_status_switch_toggle_state(
                        device,
                        can_buttons_registration_record_p->can_switch_name);

                //
                // Have the new switch state, check to see if it
                // has toggled. If it has, call the registered
                // handler function for this switch. Based on
                // whether the result is TRUE (switch ON) or false
                // (switch OFF), update the state of the switch's
                // LED backlight.
                //

                can_buttons_registration_record_p->handler_function_p(result);

                //
                // Update the switch's LED backlight based on
                // result ON/OFF.
                //
                if (result == TRUE)
                {
                    pku2400_set_change_led_colour(
                        device,
                        can_buttons_registration_record_p->can_switch_name,
                        GREEN);
                    //
                    // Transmit the new LED backlight state to the PKU CAN switches.
                    //
                    pku2400_tx_led_controls(device, TX_SEND_EACH_CALL);
                }
                else
                {
                    pku2400_set_change_led_colour(
                        device,
                        can_buttons_registration_record_p->can_switch_name ,
                        BROWN);

                    //
                    // Transmit the new LED backlight state to the PKU CAN switches.
                    //
                    pku2400_tx_led_controls(device, TX_SEND_EACH_CALL);

                }
            }
            break;

        default:
            return;

        }

        //
        // Point to the next CAN buttons registration record.
        //
        can_buttons_registration_record_p = can_buttons_registration_record_p->next_ptr;

        //
        // Loop through the records until the end (NULL next_ptr) is reached.
        //
    } while(can_buttons_registration_record_p != NULL);

    //
    // Thransmit the new LED backlight state to the PKU CAN switches.
    //
    return;
}
