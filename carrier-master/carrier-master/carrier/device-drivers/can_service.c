/******************************************************************************
 *
 *        Name: can_service.c
 *
 * Description: The FVT CAN service device driver provides member
 *              functions that are called from each of the FVT device
 *              specific drivers (Bel, BMS, etc.) which facilitate
 *              such services as the registration of an interest in a
 *              particular CAN ID, the searching of the linked list of
 *              can_rx_registration_t structures after having received
 *              a new CAN message which calls the FVT device's
 *              specific member function to handle that message.
 *
 *              The FVT CAN service device driver also provides member
 *              functions that are called from each of the FVT device
 *              specific drivers (Bel, BMS, etc.) which facilitate
 *              such services as the registration of an intent to
 *              transmit a message with a particular CAN ID, the
 *              searching of the linked list of can_tx_registration_t
 *              structures when given a CAN ID to transmit and then
 *              calling the FVT device's specific member function to
 *              handle the population of the CAN ID's data structure
 *              with the matching device's message data.
 *
 *
 *      Author: Tom & Deepak
 *        Date: Wednesday, 19 June 2019
 *
 ******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "typedefs.h"
#include "Prototypes.h"
#include "Prototypes_CAN.h"
#include "can_service_private.h"
#include "can_service_devices.h"
#include "string.h"

//
// The FVT CAN receive messaging device driver provides member
// functions that are called from each of the FVT device specific
// drivers (Bel, BMS, etc.) which facilitate such services as the
// registration of an interest in a particular CAN ID, the searching
// of the linked list of can_rx_registration_t structures after having
// received a new CAN message which calls the FVT device's specific
// member function to handle that message.
//
typedef struct can_rx_registration_s
{
    // These structure records are linked from one to another.  The
    // last structure will have it's next_ptr == NULL.
    struct can_rx_registration_s *next_ptr;
    // ID of the Module that will receive the message
    uint8_t module_id;
    // One of three CAN interfaces
    CANLINE_ can_line;
    // ID of CAN message. Either extended or standard are acceptable.
    uint32_t can_id;
	// A counter that is incremented every 10ms from userApp.
    int16_t receive_timeout_counter;
    // This value is set for each particular CAN receive message, with
    // the receive_timeout_counter_limit argument, to any of the device's CAN
    // transmit methods.
    can_rate_t receive_timeout_counter_limit;
    bool_t timeout_enabled;
    // J1939 byte (optional)
    uint8_t j1939_byte;
    // The device_index is zero-indexed. Use the enums ONE, TWO,
    // THREE, ... which equate to 0, 1, 2, ...
    device_instances_t device;
    // FVT device's specific member function pointer. This function
    // will be called when the rx CAN message is received.
    p_rx_f_t handler_function_p;
    p_rx_not_ok timeout_function_p;
} can_rx_registration_t;

//
// NOTE: Although there is only one can_rx_registration_t structure
// definition, there must be two linked lists of registration
// records. One is for the registration records that support lookup by
// can_id, module_id, and can_line. In addition to these lookup items,
// the second linked list adds a J1939 byte. The PDM is an example of
// a device that uses a single CAN id (as well as can_line and
// module_id) for multiple messages, each with a unique byte 0
// payload. We call this byte 0 payload, in this case, the j1939_byte.
//
// The private pointers to each of these two lists are declared below.
//
static can_rx_registration_t *first_j1939_byte_rx_registration_record_p;
static can_rx_registration_t *first_rx_registration_record_p;


//
// Private functions for internal use only
//
// For handling rx_registration_records.
//
static
can_rx_registration_t *
get_last_rx_registration_record_pointer(
    can_rx_registration_t *first_rx_registration_record_p);

static can_rx_registration_t *
search_rx_registration_records_for_can_id(
    uint32_t can_id,
    uint8_t module_id,
    CANLINE_ can_line);

static can_rx_registration_t *
search_rx_registration_records_for_can_id_j1939_byte(
    uint32_t can_id,
    uint8_t j1939_byte,
    uint8_t module_id,
    CANLINE_ can_line);

static can_rx_registration_t *
search_matching_rx_registration_record(
    Can_Message_ received_can_message,
    uint8_t module_id,
    CANLINE_ can_line);

//
// For handling device_data records.
//
static device_data_t *get_last_device_linked_data_record_ptr(
    device_data_t *first_device_data_p);

device_data_t *
create_device_linked_data_record(
    device_instances_t device_instance,
    device_data_t **first_device_data_ptr,
    uint8_t *device_instance_counter,
    uint32_t sizeof_device_data_t);

device_data_t *
get_device_linked_data_record_instance_ptr(
    device_instances_t device,
    device_data_t *first_device_data_ptr);




/******************************************************************************
 *
 *        Name: fvt_can_register_receive_id()
 *
 * Description: This function, typically called from a FVT device
 *              driver, registers an interest in a CAN message with a
 *              specific ID. These registrations are stored in a
 *              linked list of can_rx_registration_t structs. At this
 *              point in time, each can_id registered must be unique.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
bool_t
fvt_can_register_receive_id(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    p_rx_f_t handler_function_p,
    p_rx_not_ok timeout_function_p)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Check if this (can_id AND module_id AND can_line) has already
    // been registered.
    //
    if (search_rx_registration_records_for_can_id(can_id, module_id, can_line) != NULL)
    {
        //
        // Report an error. The specified can_id has already been
        // registered.
        //
        DEBUG("Registration record already exists!");
        return FALSE;
    }
    else
    {
        //
        // Either the can_id was not found in an existing record or
        // there are no records yet. Allocate and populate the next
        // registration record.
        //
        // Get the last registration record address in the linked list
        // or return NULL if no records exist yet.
        //
        rx_registration_record_p =
            get_last_rx_registration_record_pointer(
                first_rx_registration_record_p);

        //
        // Check if NULL returned, indicating that no records exist yet.
        //
        if (rx_registration_record_p == NULL)
        {
            //
            // No records exist in our linked list. Create the first
            // record.
            //
            first_rx_registration_record_p =
                (can_rx_registration_t *)malloc(sizeof(can_rx_registration_t));
            rx_registration_record_p = first_rx_registration_record_p;
        }
        else
        {
            //
            // At least one record already exists. Create the new
            // record and link it with the current record.
            //
            rx_registration_record_p->next_ptr =
                (can_rx_registration_t *)malloc(sizeof(can_rx_registration_t));
            rx_registration_record_p = rx_registration_record_p->next_ptr;
        }

        //
        // Check the return value from malloc.
        //
        if (rx_registration_record_p == NULL)
        {
            //
            // Not enough space on the heap to satisfy our
            // request. This never returns.
            //
            DEBUG("Malloc error, no space on heap!");
            return FALSE;
        }

        //
        // Populate the newly created record.
        //
        rx_registration_record_p->next_ptr = NULL;
        rx_registration_record_p->module_id = module_id;
        rx_registration_record_p->can_line = can_line;
        rx_registration_record_p->can_id = can_id;
        rx_registration_record_p->j1939_byte = 0;
        rx_registration_record_p->device = device_instance;
        rx_registration_record_p->handler_function_p = handler_function_p;
        rx_registration_record_p->timeout_function_p = timeout_function_p;
		rx_registration_record_p->receive_timeout_counter = 0;
		rx_registration_record_p->receive_timeout_counter_limit = NO_TIME_OUT;
		rx_registration_record_p->timeout_enabled = FALSE;
    }

    return TRUE;
}


/******************************************************************************
 *
 *        Name: fvt_can_register_receive_id_j1939_byte()
 *
 * Description: This function, typically called from a FVT device
 *              driver, registers an interest in a CAN message with a
 *              specific ID. These registrations are stored in a
 *              linked list of can_rx_registration_t structs. At this
 *              point in time, each can_id registered must be unique.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
bool_t
fvt_can_register_receive_id_j1939_byte(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte,
    p_rx_f_t handler_function_p,
    p_rx_not_ok timeout_function_p)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Check if this (can_id AND module_id AND can_line) has already
    // been registered.
    //
    if (search_rx_registration_records_for_can_id_j1939_byte(
            can_id, j1939_byte, module_id, can_line) != NULL)
    {
        //
        // Report an error. The specified can_id has already been
        // registered.
        //
        DEBUG("Registration record already exists!");
        return FALSE;
    }
    else
    {
        //
        // Either the can_id was not found in an existing record or
        // there are no records yet. Allocate and populate the next
        // registration record.
        //
        // Get the last registration record address in the linked list
        // or return NULL if no records exist yet.
        //
        rx_registration_record_p =
            get_last_rx_registration_record_pointer(
                first_j1939_byte_rx_registration_record_p);

        //
        // Check if NULL returned, indicating that no records exist yet.
        //
        if (rx_registration_record_p == NULL)
        {
            //
            // No records exist in our linked list. Create the first
            // record.
            //
            first_j1939_byte_rx_registration_record_p =
                (can_rx_registration_t *)malloc(sizeof(can_rx_registration_t));
            rx_registration_record_p = first_j1939_byte_rx_registration_record_p;
        }
        else
        {
            //
            // At least one record already exists. Create the new
            // record and link it with the current record.
            //
            rx_registration_record_p->next_ptr =
                (can_rx_registration_t *)malloc(sizeof(can_rx_registration_t));
            rx_registration_record_p = rx_registration_record_p->next_ptr;
        }

        //
        // Check the return value from malloc.
        //
        if (rx_registration_record_p == NULL)
        {
            //
            // Not enough space on the heap to satisfy our
            // request. This never returns.
            //
            DEBUG("Malloc error, no space on heap!");
            return FALSE;
        }

        //
        // Populate the newly created record.
        //
        rx_registration_record_p->next_ptr = NULL;
        rx_registration_record_p->module_id = module_id;
        rx_registration_record_p->can_line = can_line;
        rx_registration_record_p->can_id = can_id;
        rx_registration_record_p->j1939_byte = j1939_byte;
        rx_registration_record_p->device = device_instance;
        rx_registration_record_p->handler_function_p = handler_function_p;
        rx_registration_record_p->timeout_function_p = timeout_function_p;
		rx_registration_record_p->receive_timeout_counter = 0;
		rx_registration_record_p->receive_timeout_counter_limit = NO_TIME_OUT;
		rx_registration_record_p->timeout_enabled = FALSE;
    }


    return TRUE;
}


/******************************************************************************
 *
 *        Name: fvt_can_process_rx_message()
 *
 * Description: The HED underlying system calls User_Can_Receive(), in
 *              User_Can_Receive.c, located in the Sources directory,
 *              when it has one or more CAN messages ready for us to
 *              process. The call to User_Can_Receive() occurs one or
 *              more times only while User_App() is not
 *              executing. User_Can_Receive() is an HED provided
 *              skeleton which is generated by Orchestra.
 *
 *              This function is to be called from within
 *              User_Can_Receive() and will search both of our linked
 *              list of can_rx_registration_t records for a matching
 *              can_id. If a matching can_id is found, the record's
 *              registered handler_function is called with arguments
 *              of Can_Message structure, the device_instance and a
 *              pointer to the registered record's
 *              receive_timeout_counter.
 *
 *              This function simply returns without error if a
 *              matching record was not found.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
void
fvt_can_process_rx_message(
    Can_Message_ received_can_message,
    uint8_t module_id,
    CANLINE_ can_line)
{
    can_data_t *can_data_ptr;
    can_rx_registration_t *rx_registration_record_p = NULL;

    //
    // Search our linked list of can_rx_registration_t records for a
    // record that matches the provided CAN identifier and
    // can_line. The CAN identifier is a member of the
    // received_can_messate structure.
    //
    rx_registration_record_p =
        search_matching_rx_registration_record(
            received_can_message,
            module_id,
            can_line);

    //
    // Check if a matching registration record was found.
    //
    if (rx_registration_record_p != NULL)
    {
        //
        // Matching record found, get a pointer to its data and call
        // its handler_function. The handler function will copy data
        // from the received CAN message to the appropriate message
        // structure within the approptiate device driver's data
        // structure.
        //
        can_data_ptr = (can_data_t *)&(received_can_message.data);
        rx_registration_record_p->handler_function_p(
            rx_registration_record_p->device,
            can_data_ptr,
            &(rx_registration_record_p->receive_timeout_counter));
     }
}


/******************************************************************************
 *
 *        Name: get_last_rx_registration_record_pointer()
 *
 * Description: Search through a list of can_rx_registration records
 *              looking for the last record. Return a pointer to the
 *              last record so that a new record may be added.
 *
 *      Author: Tom & Deepak Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
static
can_rx_registration_t *
get_last_rx_registration_record_pointer(
    can_rx_registration_t *first_rx_registration_record_p)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Get a pointer to the first registration record in the linked
    // list of records. Searches always start from the first record
    // and work their way through all of the linked records.
    //
    rx_registration_record_p = first_rx_registration_record_p;

    //
    // Check if the first record has been created. If not,
    // rx_registration_record_p will be NULL and function returns.
    //
    if (rx_registration_record_p != NULL)
    {
        //
        // The first registration record was not NULL.  Iterate
        // through the registration records until the record with the
        // next_ptr == NULL is found. This will be the last record in
        // the linked list.
        //
        while (rx_registration_record_p->next_ptr != NULL)
        {
            rx_registration_record_p = rx_registration_record_p->next_ptr;
        }
    }

    //
    // Return either a pointer to the last record in the linked list
    // or NULL if no records have yet been created.
    //
    return rx_registration_record_p;
}


/******************************************************************************
 *
 *        Name: search_rx_registration_records_for_can_id()
 *
 * Description: Search through the linked list of can_registeration_t
 *              structures looking for a matching can_id.
 *
 *              Returns a pointer to the matching record or NULL if no
 *              match was found.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
static can_rx_registration_t *
search_rx_registration_records_for_can_id(
    uint32_t can_id,
    uint8_t module_id,
    CANLINE_ can_line)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Point to the first registration record in the linked list of
    // records. Searches always start from the first record and work
    // their way through all of the linked records.
    //
    rx_registration_record_p = first_rx_registration_record_p;

    //
    // Check if the first record has been created. If not,
    // rx_registration_record_p will be NULL and function returns.
    //
    if (rx_registration_record_p != NULL)
    {
        //
        // The first registration record was not NULL. Proceed to
        // iterate through the linked list of records, looking for a
        // record with a matching can_id and can_line.
        //
        do
        {
            //
            // Check if the current registration record matches the
            // module_id and can_id and can_line.
            //
            if((rx_registration_record_p->can_id == can_id) &&
               (rx_registration_record_p->module_id == module_id) &&
               (rx_registration_record_p->can_line == can_line))
            {
                //
                // A matching record has been found, break out of the
                // loop. Rx_Registration_record_p points to the
                // matching record.
                //
                break;
            }

            //
            // A matching record has not yet been found. Point to the
            // next record.
            //
            rx_registration_record_p = rx_registration_record_p->next_ptr;

            //
            // Loop until the end of the linked list of records.
            //
        } while(rx_registration_record_p != NULL);
    }

    //
    // Return either a pointer to the matching record in the linked
    // list or NULL if no records have yet been created or a match was
    // not found.
    //
    return rx_registration_record_p;
}


/******************************************************************************
 *
 *        Name: search_rx_registration_records_for_can_id_j1939_byte()
 *
 * Description: Search through the linked list of can_registeration_t
 *              structures looking for a matching record.
 *
 *              Returns a pointer to the matching record or NULL if no
 *              match was found.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
static can_rx_registration_t *
search_rx_registration_records_for_can_id_j1939_byte(
    uint32_t can_id,
    uint8_t j1939_byte,
    uint8_t module_id,
    CANLINE_ can_line)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Point to the first registration record in the linked list of
    // records. Searches always start from the first record and work
    // their way through all of the linked records.
    //
    rx_registration_record_p = first_j1939_byte_rx_registration_record_p;

    //
    // Check if the first record has been created. If not,
    // rx_registration_record_p will be NULL and function returns.
    //
    if (rx_registration_record_p != NULL)
    {
        //
        // The first registration record was not NULL. Proceed to
        // iterate through the linked list of records, looking for a
        // record with a matching can_id and can_line.
        //
        do
        {
            //
            // Check if the current registration record matches the
            // module_id and can_id and j1939_byte and can_line.
            //
            if((rx_registration_record_p->can_id == can_id) &&
               (rx_registration_record_p->j1939_byte == j1939_byte) &&
               (rx_registration_record_p->module_id == module_id) &&
               (rx_registration_record_p->can_line == can_line))
            {
                //
                // A matching record has been found, break out of the
                // loop. Rx_Registration_record_p points to the matching
                // record.
                //
                break;
            }

            //
            // A matching record has not yet been found. Point to the
            // next record.
            //
            rx_registration_record_p = rx_registration_record_p->next_ptr;

            //
            // Loop until the end of the linked list of records.
            //
        } while(rx_registration_record_p != NULL);
    }

    //
    // Return either a pointer to the matching record in the linked
    // list or NULL if no records have yet been created or a match was
    // not found.
    //
    return rx_registration_record_p;
}


/******************************************************************************
 *
 *        Name: search_matching_rx_registration_record()
 *
 * Description: Search through a list of can_registeration records for
 *              a matching can_id and can_line.
 *
 *              For each record in linked list, first checks if record
 *              matches the received message, including the
 *              j1939_byte. If not, it also checks for a match with
 *              only the can_id and can_line.
 *
 *              If a matching registration is found return a pointer
 *              to the registration record, otherwise return NULL.
 *
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
static
can_rx_registration_t *
search_matching_rx_registration_record(
    Can_Message_ received_can_message,
    uint8_t module_id,
    CANLINE_ can_line)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Point to the first registration record in the j1939_byte linked
    // list of records. Searches always start from the first record
    // and work their way through all of the linked records.
    //
    rx_registration_record_p = first_j1939_byte_rx_registration_record_p;

    //
    // Check if the first record has been created. If not,
    // rx_registration_record_p will be NULL and function returns.
    //
    if (rx_registration_record_p != NULL)
    {
        //
        // The first registration record was not NULL. Proceed to
        // iterate through the linked list of records, looking for a
        // record with a matching can_id and can_line.
        //
        do
        {
            //
            // Check if the current registration record matches the
            // module_id and can_id and j1939_byte and can_line.
            //
            if ((rx_registration_record_p->can_id ==
                 received_can_message.identifier) &&
                (rx_registration_record_p->j1939_byte ==
                 received_can_message.data[0]) &&
                (rx_registration_record_p->module_id == module_id) &&
                (rx_registration_record_p->can_line == can_line))
            {
                //
                // A matching record has been found, break out of the
                // loop. Rx_Registration_record_p points to the
                // matching record.
                //
                return rx_registration_record_p;
            }

            //
            // A matching record has not yet been found. Point to the
            // next record.
            //
            rx_registration_record_p =
                rx_registration_record_p->next_ptr;

            //
            // Loop until the end of the linked list of records.
            //
        } while(rx_registration_record_p != NULL);
    }


    //
    // Point to the first registration record in linked list of
    // records. Searches always start from the first record and work
    // their way through all of the linked records.
    //
    rx_registration_record_p = first_rx_registration_record_p;

    //
    // Check if the first record has been created. If not,
    // rx_registration_record_p will be NULL and function returns.
    //
    if (rx_registration_record_p != NULL)
    {
        //
        // The first registration record was not NULL. Proceed to
        // iterate through the linked list of records, looking for a
        // record with a matching can_id and can_line.
        //
        do
        {
            //
            // Check if the current registration record matches the
            // module_id and can_id and can_line.
            //
            if ((rx_registration_record_p->can_id ==
                 received_can_message.identifier) &&
                (rx_registration_record_p->module_id == module_id) &&
                (rx_registration_record_p->can_line == can_line))
            {
                //
                // A matching record has been found, break out of the
                // loop. Rx_Registration_record_p points to the
                // matching record.
                //
                return rx_registration_record_p;
            }

            //
            // A matching record has not yet been found. Point to the
            // next record.
            //
            rx_registration_record_p =
                rx_registration_record_p->next_ptr;

            //
            // Loop until the end of the linked list of records.
            //
        } while(rx_registration_record_p != NULL);
    }

    //
    // A matching record was not found, return NULL.
    //
    return NULL;
}


/******************************************************************************
 *
 *        Name: fvt_can_set_timeout_receive_id_j1939_byte()
 *
 * Description: Each CAN ID that is registered for receiving has a
 *              timeout limit associated with it. It is set to
 *              NO_TIME_OUT by default, when the Rx CAN message is
 *              registered.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void
fvt_can_set_timeout_receive_id_j1939_byte(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte,
    can_rate_t timeout_limit)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Serch for a matching receive registration record.
    //
	rx_registration_record_p =
        search_rx_registration_records_for_can_id_j1939_byte(
            can_id,
            j1939_byte,
            module_id,
            can_line);

    //
    // Check for a NULL pointer, in the case of the record not being
    // found. The DEBUG statement will only be of help during testing.
    //
	if (rx_registration_record_p == NULL)
    {
        DEBUG("NULL Pointer");
    }
    else
    {
        rx_registration_record_p->receive_timeout_counter_limit =
            timeout_limit;

        if (timeout_limit >= 1)
        {
            rx_registration_record_p->timeout_enabled = TRUE;
        }
        else
        {
            rx_registration_record_p->timeout_enabled = FALSE;
        }
    }
}


/******************************************************************************
 *
 *        Name: fvt_can_get_timeout_status_for_receive_id_j1939_byte()
 *
 * Description: Each CAN ID that is registered for receiving has a
 *              timeout limit associated with it. By default, it is
 *              set to NO_TIME_OUT when the Rx CAN message is
 *              registered. Return TRUE if there has been a timeout
 *              for the specified message. Otherwise, return FALSE.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
bool_t
fvt_can_get_timeout_status_for_receive_id_j1939_byte(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte)
{
    bool_t timed_out = FALSE;
    can_rx_registration_t *rx_registration_record_p;

    //
    // Serch for a matching receive registration record.
    //
	rx_registration_record_p =
        search_rx_registration_records_for_can_id_j1939_byte(
            can_id,
            j1939_byte,
            module_id,
            can_line);

    //
    // Check for a NULL pointer, in the case of the record not being
    // found. The DEBUG statement will only be of help during testing.
    //
	if (rx_registration_record_p == NULL)
    {
        DEBUG("NULL Pointer");
    }

    //
    // Check if this message has timed out. Note that this TIMED_OUT
    // state is reset whenever the message is received again.
    //
    else if (rx_registration_record_p->receive_timeout_counter ==
             TIMED_OUT)
    {
        timed_out = TRUE;
    }

    return timed_out;
}


/******************************************************************************
 *
 *        Name: fvt_can_set_timeout_receive_id()
 *
 * Description: Each CAN ID that is registered for receiving has a
 *              timeout limit associated with it. By default, it is
 *              set to NO_TIME_OUT when the Rx CAN message is
 *              registered.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void
fvt_can_set_timeout_receive_id(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    can_rate_t timeout_limit)
{
    can_rx_registration_t *rx_registration_record_p;

    //
    // Serch for a matching receive registration record.
    //
	rx_registration_record_p =
        search_rx_registration_records_for_can_id(
            can_id,
            module_id,
            can_line);

    //
    // Check for a NULL pointer, in the case of the record not being
    // found. The DEBUG statement will only be of help during testing.
    //
    if (rx_registration_record_p == NULL)
    {
        DEBUG("NULL POINTER");
    }
    else
    {
        rx_registration_record_p->receive_timeout_counter_limit =
            timeout_limit;
        if (timeout_limit >= 1)
        {
            rx_registration_record_p->timeout_enabled = TRUE;
        }
        else
        {
            rx_registration_record_p->timeout_enabled = FALSE;
        }
    }
}


/******************************************************************************
 *
 *        Name: fvt_can_get_timeout_status_for_receive_id()
 *
 * Description: Each CAN ID that is registered for receiving has a
 *              timeout limit associated with it. By default, it is
 *              set to NO_TIME_OUT when the Rx CAN message is
 *              registered. Return FALSE if there has been a timeout
 *              for the specified message. Otherwise, return TRUE.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
bool_t
fvt_can_get_timeout_status_for_receive_id(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id)
{
    bool_t timed_out = FALSE;
    can_rx_registration_t *rx_registration_record_p;

    //
    // Search for a matching receive registration record.
    //
	rx_registration_record_p =
        search_rx_registration_records_for_can_id(
            can_id,
            module_id,
            can_line);

    //
    // Check for a NULL pointer, in the case of the record not being
    // found. The DEBUG statement will only be of help during testing.
    //
	if (rx_registration_record_p == NULL)
    {
        DEBUG("NULL Pointer");
    }

    //
    // Check if this message has timed out. Note that this TIMED_OUT
    // state is reset whenever the message is received again.
    //
    else if (rx_registration_record_p->receive_timeout_counter == TIMED_OUT)
    {
        timed_out = TRUE;
    }

    return timed_out;
}


/******************************************************************************
 *
 *        Name: can_rx_check_message_timeouts()
 *
 * Description: This function is to be called from within User_App()
 *              each time it executes. The FVT default timing for
 *              User_App() to be called, set in Orchestra, is once
 *              every 10ms.
 *
 *              This function finds and returns any registered CAN
 *              receive message that has not received a message within
 *              its receive timeout limit. Note that the receive
 *              timeout limit, for each registered CAN receive message
 *              may be set by calling one of the fvt CAN set timeout
 *              receive functions listed below.
 *
 *                  fvt_can_set_timeout_receive_id_j1939_byte()
 *                  fvt_can_set_timeout_receive_id()
 *
 *              This function exists because there are vehicle CAN
 *              devices under control of the CVC, such as the BMS or
 *              the SKAI inverters, that if not heard from, due to a
 *              broken or intermittent CAN connection, must cause the
 *              vehicle to shut down or some other action to be taken.
 *
 *              Each time this function is called, it walks through
 *              both the j1939_byte linked list and the normal linked
 *              list of CAN receive registration records, incrementing
 *              each record's receive_timeout_counter. If the record's
 *              receive_timeout_counter is greater than the
 *              receive_timeout_counter_limit, a structure identifying
 *              the receive CAN message is returned. It is then up to
 *              User_App() code to take the appropriate action.
 *
 *              Note that each time a registered CAN receive message
 *              is received, its registered receive function zeros its
 *              receive timeout counter. In this way, the limit will
 *              only be reached if the message is not received.
 *
 *              The caller should first look at the can_id of the
 *              returned timeout status structure. The following
 *              describes the returned can_id:
 *
 *                  A can_id of NO_RX_TIMEOUT_FOUND - indicates that a
 *                  receive timeout has not occured on any of the
 *                  registered CAN receive messages. Other members of
 *                  the returned timeout status structure are not
 *                  valid.
 *
 *                  A can_id of NO_RX_MESSAGES_REGISTERED - indicates
 *                  that there have not yet been any receive CAN
 *                  messages registered. Other members of the returned
 *                  timeout status structure are not valid.
 *
 *                  Any other valid can_id indicates that a timeout
 *                  has been found and the remaining members of the
 *                  timeout status structure are valid.
 *
 *      Author: Deepak
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void can_rx_check_message_timeouts()
{
    //
    // There are two linked lists of receive CAN registration records.
    //
    const int linked_list_cnt = 2;

    //
    // Initialize our array of linked list pointers.
    //
    can_rx_registration_t *rx_registration_record_p[linked_list_cnt];
    rx_registration_record_p[0] = first_rx_registration_record_p;
    rx_registration_record_p[1] = first_j1939_byte_rx_registration_record_p;

    //
    // Prepare the return structure.
    //
    can_rx_timeout_status_t can_rx_timeout_status;
	can_rx_timeout_status.can_id = NO_RX_TIMEOUT_FOUND;

    //
    // Walk through both linked lists of registration records.
    //
    for (int i = 0; i < linked_list_cnt; ++i)
    {
        while (rx_registration_record_p[i] != NULL)
        {
            //
            // Starting with the very first CAN Rx registration record,
            // walk through the linked list, looking for, marking, and
            // returning a structure identifying a registered receive
            // message that has timed out.
            //
            // NOTE: When a Rx CAN message is registered, the
            //       following assgnments are made to its
            //       rx_registration_record:
            //
            //          receive_timeout_counter = 0
            //          receive_timeout_counter_limit = NO_TIME_OUT
            //
            if (rx_registration_record_p[i]->receive_timeout_counter <
                rx_registration_record_p[i]->receive_timeout_counter_limit)
            {
                //
                // Check if this record has previously been marked as
                // TIMED_OUT and has already had its information returned
                // in a can_rx_timeout_status structure. If it has, then
                // skip to the next record.
                //
                if (rx_registration_record_p[i]->receive_timeout_counter != TIMED_OUT)
                {
                    //
                    // This record has not been marked as
                    // TIMED_OUT. Increment its receive_timeout_counter.
                    //
                    rx_registration_record_p[i]->receive_timeout_counter++;
                }

            }
            else
            {
                if (rx_registration_record_p[i]->timeout_enabled == TRUE)
                {
                    //
                    // A registered CAN receive record has been found
                    // which has a timeout condition. Populate the return
                    // timeout status structure with the appropriate
                    // information.
                    //
                    can_rx_timeout_status.module_id =
                        rx_registration_record_p[i]->module_id;

                    can_rx_timeout_status.can_id =
                        rx_registration_record_p[i]->can_id;

                    can_rx_timeout_status.can_line =
                        rx_registration_record_p[i]->can_line;

                    can_rx_timeout_status.device =
                        rx_registration_record_p[i]->device;

                    can_rx_timeout_status.j1939_byte =
                        rx_registration_record_p[i]->j1939_byte;

                    //
                    // Call this record's timeout_function.
                    //
                    rx_registration_record_p[i]->timeout_function_p(
                        rx_registration_record_p[i]->device,
                        rx_registration_record_p[i]->module_id,
                        rx_registration_record_p[i]->can_line,
                        rx_registration_record_p[i]->can_id,
                        rx_registration_record_p[i]->j1939_byte);

                    //
                    // Mark this record's receive timeout counter as
                    // TIMED_OUT, break out of the loop, and return the
                    // populated timeout status structure.
                    //
                    rx_registration_record_p[i]->receive_timeout_counter = TIMED_OUT;

                    return;
                }
            }
            //
            // Point to the next record in the linked list of
            // records. Loop until the end of the list.
            //
            rx_registration_record_p[i] = rx_registration_record_p[i]->next_ptr;

        }
    }

    // Update_Output(OUT_A1, OUTPUT_OFF);
}


/******************************************************************************
 *
 *        Name: rx_message_timeout()
 *
 * Description: This function may be called from within a device
 *              driver's registered receive CAN message timeout
 *              functions. If called from all device drivers, it
 *              provides a common location to set a breakpoint in
 *              order to call all such timeouts in the system.
 *
 *      Author: Tom
 *        Date: Saturday, 10 August 2019
 *
 ******************************************************************************
 */
void rx_message_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte)
{
    device_instances_t local_device;
    uint8_t local_module_id;
    CANLINE_ local_can_line;
    uint32_t local_can_id;
    uint8_t local_j1939_byte;

    local_device = device;
    local_module_id = module_id;
    local_can_line = can_line;
    local_can_id = can_id;
    local_j1939_byte = j1939_byte;

    // Update_Output(OUT_A1, OUTPUT_ON);
}



//=============================================================================
//=============================================================================
//=============================================================================
//
// Tx Registration Record Support Functions
//
//=============================================================================
//=============================================================================
//=============================================================================






/******************************************************************************
 *
 *        Name: fvt_can_register_transmit_id()
 *
 * Description: This function, typically called from a FVT device
 *              driver, registers an interest in a CAN message with a
 *              specific ID. These registrations are stored in a
 *              linked list of can_tx_registration_t structs. At this
 *              point in time, each can_id registered must be unique.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
can_tx_registration_t *
fvt_can_register_transmit_id(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    IDENTIFIER_TYPE_ type)
{
    can_tx_registration_t *tx_registration_record_p;

    tx_registration_record_p =
        (can_tx_registration_t *)malloc(sizeof(can_tx_registration_t));

    //
    // Check the return value from malloc.
    //
    if (tx_registration_record_p == NULL)
    {
        //
        // Not enough space on the heap to satisfy our
        // request. This never returns.
        //
        DEBUG("Malloc error, no space on heap!");
        return NULL;
    }

    //
    // Populate the newly created record.
    //
    tx_registration_record_p->module_id = module_id;
    tx_registration_record_p->can_line = can_line;
    tx_registration_record_p->tx_message.identifier = can_id;
    tx_registration_record_p->tx_message.type = type;
    tx_registration_record_p->tx_message.length = 8;
    tx_registration_record_p->device = device_instance;
    tx_registration_record_p->transmit_counter_limit = EVERY_100MS;
    tx_registration_record_p->transmit_counter = 0;
    tx_registration_record_p->transmit_message_sent_first_time = FALSE;

    return tx_registration_record_p;
}


/******************************************************************************
 *
 *        Name: fvt_can_register_transmit_id_j1939()
 *
 * Description: This function, typically called from a FVT device
 *              driver, registers an interest in a CAN message with a
 *              specific ID. These registrations are stored in a
 *              linked list of can_tx_registration_t structs. At this
 *              point in time, each can_id registered must be unique.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */
can_tx_registration_t *
fvt_can_register_transmit_id_j1939(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    IDENTIFIER_TYPE_ type,
    uint16_t j1939_word)
{
    can_tx_registration_t *tx_registration_record_p;

    tx_registration_record_p =
        (can_tx_registration_t *)malloc(sizeof(can_tx_registration_t));

    //
    // Check the return value from malloc.
    //
    if (tx_registration_record_p == NULL)
    {
        //
        // Not enough space on the heap to satisfy our request. This
        // never returns.
        //
        DEBUG("Malloc error, no space on heap!");
        return NULL;
    }

    //
    // Populate the newly created record.
    //
    tx_registration_record_p->module_id = module_id;
    tx_registration_record_p->can_line = can_line;
    tx_registration_record_p->tx_message.identifier = can_id;
    tx_registration_record_p->tx_message.type = type;
    tx_registration_record_p->tx_message.length = 8;
    tx_registration_record_p->device = device_instance;
    tx_registration_record_p->j1939_word = j1939_word;

    //
    // A counter that is incremented when any of a device's CAN
    // transmit methods are called. This will occur at the rate that
    // User_App() is called by the underlying HED system services. The
    // specific message registered will be transmitted only when this
    // counter exceeds the transmit_counter_limit.
    //
    tx_registration_record_p->transmit_counter = 0;

    //
    // Setting to a default value, but this value is set for each
    // particular CAN transmit message, with the
    // transmit_counter_limit argument, to any of the device's CAN
    // transmit methods.
    //
    tx_registration_record_p->transmit_counter_limit = EVERY_100MS;
    tx_registration_record_p->transmit_message_sent_first_time = FALSE;

    return tx_registration_record_p;
}


/******************************************************************************
 *
 *        Name: fvt_can_register_transmit_id_j1939()
 *
 * Description: This function, typically called from a FVT device
 *              driver, registers an interest in a CAN message with a
 *              specific ID. These registrations are stored in a
 *              linked list of can_tx_registration_t structs. At this
 *              point in time, each can_id registered must be unique.
 *
 *      Author: Tom & Deepak
 *        Date: Thursday, 20 June 2019
 *
 ******************************************************************************
 */

can_tx_registration_t *
fvt_can_register_transmit_id_j1939_byte(
    device_instances_t device_instance,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    IDENTIFIER_TYPE_ type,
    uint8_t j1939_byte)
{
    can_tx_registration_t *tx_registration_record_p;

    tx_registration_record_p =
        (can_tx_registration_t *)malloc(sizeof(can_tx_registration_t));

    //
    // Check the return value from malloc.
    //
    if (tx_registration_record_p == NULL)
    {
        //
        // Not enough space on the heap to satisfy our request. This
        // never returns.
        //
        DEBUG("Malloc error, no space on heap!");
        return NULL;
    }

    //
    // Populate the newly created record.
    //
    tx_registration_record_p->module_id = module_id;
    tx_registration_record_p->can_line = can_line;
    tx_registration_record_p->tx_message.identifier = can_id;
    tx_registration_record_p->tx_message.type = type;
    tx_registration_record_p->tx_message.length = 8;
    tx_registration_record_p->device = device_instance;
    tx_registration_record_p->j1939_byte = j1939_byte;

    //
    // A counter that is incremented when any of a device's CAN
    // transmit methods are called. This will occur at the rate that
    // User_App() is called by the underlying HED system services. The
    // specific message registered will be transmitted only when this
    // counter exceeds the transmit_counter_limit.
    //
    tx_registration_record_p->transmit_counter = 0;

    //
    // Setting to a default value, but this value is set for each
    // particular CAN transmit message, with the
    // transmit_counter_limit argument, to any of the device's CAN
    // transmit methods.
    //
    tx_registration_record_p->transmit_counter_limit = EVERY_100MS;
    tx_registration_record_p->transmit_message_sent_first_time = FALSE;

    return tx_registration_record_p;
}


/******************************************************************************
 *
 *        Name: canPrintf()
 *
 * Description: A version of printf() that 'prints' to the specified
 *              CAN interface.
 *
 *      Author: Tom
 *        Date: Oct 17, 2017 Time: 5:32:03 PM
 *
 ******************************************************************************
 */
int
canPrintf(
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    char *format, ...)
{
    Can_Message_ tx_message;
    char buffer[40][8];
    va_list aptr;
    int len, numBufs;

    can_data_t *data_ptr = (can_data_t *)tx_message.data;

    tx_message.identifier = can_id;
    tx_message.length = 8;
    tx_message.type = EXTENDED;

    //
    // Compose a string of text using the provided format string
    //
    va_start(aptr, format);
    len = vsnprintf((char *)buffer, sizeof(buffer), format, aptr);
    va_end(aptr);

#warning "Need to handle this error!"
    // Brute force error handling during debug phase
//    if ((len < 0) || (len > sizeof(buffer)))
//        asm ("    ESTOP0");

    //
    // Get the number of 8 byte buffers to transmit over CAN
    //
    numBufs = len / 8;

    //
    // Full 8-byte buffers are always sent. If the len of the
    // formatted buffer string is not a multiple of 8, then the last
    // transmitted message will have fewer than 8 significant
    // characters. Ensure that the last significant character is
    // followed by a null character.
    //
    buffer[numBufs][len % 8] = 0;

    //
    // Adjust the number of buffers to transmit if the len of the
    // formatted buffer is not a multiple of 8.
    //
    numBufs += (len % 8) ? 1 : 0;

    //
    // Send the formatted string in the buffer over one or more 8-byte
    // CAN messages/
    //
    for (int i = 0; i < numBufs; ++i)
    {

        data_ptr->MDL.bit8.BYTE0 = buffer[i][0];
        data_ptr->MDL.bit8.BYTE1 = buffer[i][1];
        data_ptr->MDL.bit8.BYTE2 = buffer[i][2];
        data_ptr->MDL.bit8.BYTE3 = buffer[i][3];

        data_ptr->MDH.bit8.BYTE0 = buffer[i][4];
        data_ptr->MDH.bit8.BYTE1 = buffer[i][5];
        data_ptr->MDH.bit8.BYTE2 = buffer[i][6];
        data_ptr->MDH.bit8.BYTE3 = buffer[i][7];

        //
        // Send the tx_message to the specified CAN port.
        //
        CAN_WRITE_STATUS status;
        do
        {
            //
            // Check if the CAN Tx buffer is full.
            //
            status = Send_CAN_Message(module_id, can_line, tx_message);

            //
            // Keep attempting to send until successful.
            //
        } while (status == CAN_WRITE_BUFFER_FULL);
    }

    return(len);
}







//=============================================================================
//=============================================================================
//=============================================================================
//
// Device instance's device_data_t Record Support Functions
//
//=============================================================================
//=============================================================================
//=============================================================================




/******************************************************************************
 *
 *        Name: create_device_linked_data_record()
 *
 * Description: Each device module has a complex structure associated
 *              with itself. The received CAN messages are stored in
 *              this compound structure. Also, the device instance
 *              structs for each of the transmit messages are also in
 *              this compound structure.
 *
 *              When this function is called is called for the first
 *              time, memory is allocated dynamically for the
 *              bel_rx_data_t. After the allocation of memory is done,
 *              set the next_ptr of the structure to NULL. To indicate
 *              the end of the linked list.
 *
 *              When this function is called again. The pointer of the
 *              existing bel_rx_data_t is obtained by searching for
 *              the record whose next_ptr is set to NULL. Once, the
 *              last bel_rx_data_t is found. Dynamically allocate
 *              memory to store a new bel_rx_data_t.
 *
 *              In the end we would end up with a list of linked
 *              device_data_t records.
 *
 *
 *      Author: Tom & Deepak
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */

device_data_t *
create_device_linked_data_record(
    device_instances_t device_instance,
    device_data_t **first_device_data_ptr,
    uint8_t *device_instance_counter,
    uint32_t sizeof_device_data_t)
{
    device_data_t *device_data_ptr;

    //
    // Check if a linked data record for this device has already been
    // created.
    //
    if ((device_data_ptr =
         get_device_linked_data_record_instance_ptr(
             device_instance,
             *first_device_data_ptr)) != NULL)
    {
        //
        // A linked data record for this device has already been
        // created. Return the pointer to the record.
        //
        return device_data_ptr;
    }

    //
    // Get a pointer to last registration record address in the linked
    // list.  The return pointer will be NULL if no records exist yet.
    //
    device_data_ptr =
        get_last_device_linked_data_record_ptr(
            *first_device_data_ptr);

    //
    // Have any linked data records been created yet?
    //
    if (device_data_ptr == NULL)
    {
        //
        // No records exist in our linked list. Create the first
        // record. Set the device module's global first record pointer
        // to the newly allocated record.
        //
        device_data_ptr =
            (device_data_t *)malloc(sizeof_device_data_t);
        memset((uint8_t *)device_data_ptr, 0, sizeof_device_data_t);

        *first_device_data_ptr = device_data_ptr;
    }
    else
    {
        //
        // At least one record already exists. Create the new
        // record and link it with the current record.
        //
        device_data_ptr->next_ptr =
            (device_data_t *)malloc(sizeof_device_data_t);

        memset((uint8_t *)device_data_ptr->next_ptr, 0, sizeof_device_data_t);

        //
        // Set out data record pointer to point to this newly created
        // record.
        //
        device_data_ptr = device_data_ptr->next_ptr;
        //
        // Set this new records next pointer to NULL.
        //
    }

    //
    // At this point, either a new record has been allocated, either
    // because it is the first record or a second or greater
    // record. If there was no space on the heap then the record
    // pointer will be pointing to NULL.
    //
    if(device_data_ptr == NULL)
    {
        //
        // Not enough space on the heap to satisfy our request.
        //
        DEBUG("Malloc error, no space on heap!");
        return NULL;
    }

    //
    // The device rx data record pointer is now pointing to a newly
    // created record. Since a 'new device' has been instantiated,
    // update the device module's global device instance counter and
    // save its value in the device instance's data record.
    //
    ++*device_instance_counter;
    device_data_ptr->device_instance_number =
        *device_instance_counter;

    //
    // Set this newly created data record's next pointer to NULL.
    //
    device_data_ptr->next_ptr = NULL;

    return device_data_ptr;
}


/******************************************************************************
 *
 *        Name: get_last_device_linked_data_record_ptr()
 *
 * Description: Search through a list of device_data_t linked records.
 *              Look for a device_data_t linked record with its next
 *              pointer initialised to NULL. Once the record is found,
 *              return a pointer to the HEAD of that particular
 *              record.
 *
 *
 *      Author: Tom & Deepak
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */

static device_data_t *
get_last_device_linked_data_record_ptr(
    device_data_t *first_device_data_ptr)
{
    device_data_t *device_data_ptr;

    //
    // Point to the first device_rx_data linked record in the linked
    // list of records.
    //
	device_data_ptr = first_device_data_ptr;

    //
    // Check if the first record has been created. If not,
    // device_data_ptr will be NULL and function returns.
    //
	if (device_data_ptr != NULL)
	{
        //
        // The first device_data_t linked record was not NULL. Proceed
        // to iterate through the linked list of records, looking for
        // a record with its next pointer initialised to NULL.
        //
        // Iterate through the registration records until one with the
        // next_ptr == NULL is found. This finds the last record in
        // the linked list.
        //
        while (device_data_ptr->next_ptr != NULL)
        {
            device_data_ptr = device_data_ptr->next_ptr;
        }
    }

    //
    // Return either a pointer to a matching record or NULL if no
    // match found.
    //
	return device_data_ptr;

}

/******************************************************************************
 *
 *        Name: get_device_linked_data_record_instance_ptr()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */
device_data_t *
get_device_linked_data_record_instance_ptr(
    device_instances_t device,
    device_data_t *first_device_data_ptr)

{
    device_data_t *device_data_ptr;

    //
    // Point to the first device_data_t linked record in the linked list
    // of records.
    //
	device_data_ptr = first_device_data_ptr;

    //
    // Check if the first record has been created. If not,
    // device_data_t pointer will be NULL and function returns.
    //
	if (device_data_ptr != NULL)
	{
        //
        // If there is one Bel charger, its device number would be
        // 1. If there are 4 Bel chargers, the device numbers for each
        // instance would be 1, 2, 3 & 4. The device_instance_number
        // is assigned when the divice instance is created. Iterate
        // through the link list of records while looking for the
        // record that corresponds to the device function paramter
        // provided. If a match is found, return a pointer to the
        // record. Return NULL if a match is not found.
        //
        while (device_data_ptr != NULL)
        {
            //
            // Check if this record's device instance matches the
            // device in question.
            //
            if (device == device_data_ptr->device_instance_number)
            {
                //
                // This device already has a linked data
                // record. Return a pointer to this record.
                //
                break;
            }
            else
            {
                //
                // The device does not match the current record's
                // device instance. Point to the next one in the
                // linked list of records. Loop until the end of the
                // list.
                //
            	device_data_ptr = device_data_ptr->next_ptr;
            }
        }
    }

    //
    // Return either a pointer to a matching record or NULL if no
    // match found.
    //
	return device_data_ptr;

}
