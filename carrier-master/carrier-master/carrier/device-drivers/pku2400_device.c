/******************************************************************************
 *
 *        Name: pku2400_device.c
 *
 * Description: This module implements the PKU2400 CAN Dash Switches
 *              device driver.
 *
 *      Author: Deepak
 *        Date: Tuesday, 6 Aug 2019
 *
 ******************************************************************************
 */
#include "pku2400_device_private.h"


//=============================================================================
// Private member functions
//
// These functions are private in that they are called only from HED's
// User_Can_Receive() via a pointer that is obtained from a call to
// fvt_can_process_rx_message().
//
//=============================================================================
//
static void rx_can_switch_status_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void rx_can_switch_status(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);


//=============================================================================
// A private function that returns the state of the can buttons, when
// configured as a push button. The toggle_switch_state makes the
// state decision based on the rising edge.
// =============================================================================
//
static bool_t pku2400_determine_switch_state(device_instances_t device, bool_t input, can_switch_t can_switch);

// CAN Receive Message Identifier
typedef enum {
    PKU2400_RXID_STATUS = 0x18EFFF00,
} PKU2400_RECEIVE_IDS;


//=============================================================================
//
// These are the tx message types for the device.  Just as in
// receive messages, transmit messages are registered. In the case of
// receive messages, the 'keys' into the the registered link list of
// messages are the CAN ID and the CAN LINE (CAN1, CAN2 or CAN3). In
// the case of the transmit registered messages, the 'key' into the
// message to transmit will be one of the enums listed below. These
// enums are used to both register a TX CAN message and to search for
// (through linked tx registration records) the TX CAN message to
// send.
//
//=============================================================================
//
typedef enum {
    PKU2400_TXID_LED_CONTROLS = 0x18EFFF01,
} PKU2400_TRANSMIT_ID;


//=============================================================================
//
// A pointer to the first structure in the linked list compound
// structures that represent all of the Bel received data messages and
// any other constructs, such as timers, required for each Bel charger
// instance.
//
//=============================================================================
//
device_data_t *can_switches_pku2400_first_device_data_ptr = NULL;


//=============================================================================
//
// This is incremented each for each device instance
//
//=============================================================================
//
static uint8_t device_instance_counter = 0;


//=============================================================================
//
// Returns the offset for a device based on the instance provided.
// In pku2400 the RX and TX devices have the same device offset.
//
//=============================================================================
//
static uint32_t get_instance_offset(device_instances_t device)
{
    // Device offsets for the can switches
    // device 1: 18EFFF00
    // device 2: 18EFFF10
    // device 3: 18EFFF20
    // and so on..
    return (uint32_t)((device - 1) * 0x10);
}


/******************************************************************************
 *
 *        Name: can_switches_pku2400_init()
 *
 * Description: Initialize an instance of the can_switches_pku2400
 *              device.
 *
 *      Author: Deepak
 *        Date: Tuesday, 6 Aug 2019
 *
 ******************************************************************************
 */
void
pku2400_init(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line)
{

    device_data_t *device_data_ptr;
    uint16_t i = 0;

#warning "Limit the number of instances that may be initialized"

    //
    // Each instance of an FVT device driver, such as for the Bel,
    // BMS, etc., that receives data from the physical device over one
    // of the three CAN interfaces, must have memory allocated for the
    // storage of its received CAN data.
    //
    // Each instance may register an interest in receiving one or more
    // CAN messages, each with a unique ID. For each message
    // registered, there is an associated structure in the device
    // driver's header file that describes the message's data. This
    // structure makes use of the native data types as well as bit
    // fields where applicable. This insures the ease of retreiving
    // data from a 8-byte buffer without the use of bit shifting and
    // masking.
    //
    // All of the message structures, each describing a CAN message,
    // are combined into a compound structure. Other constructs that
    // may be required for each device instance, such as timers, may
    // also be added to the compound structure. To facilitate multiple
    // instances of a device, such as two Bel chargers, the compound
    // structure also contains a pointer to the next structure of this
    // same type. Each compound structure in this linked list of
    // structures represents a single instance of all the data
    // collected from the CAN messages associated with this device
    // instance. Each of these linked structures, one for each
    // instance, is refered to as a device_linked_data_record.
    //
    // Create the first or the next device_linked_data_record. This
    // record is created before registering an interest in this device
    // instance's CAN messages. There must be a place to store the
    // data before we receive the data.
    //
    device_data_ptr =
        create_device_linked_data_record(
            device,
            &can_switches_pku2400_first_device_data_ptr,
            &device_instance_counter,
            sizeof(device_data_t));

    //
    // Register an interest in this device instance's receive CAN
    // messages.
    //
    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        PKU2400_RXID_STATUS + get_instance_offset(device),
        rx_can_switch_status,
        rx_can_switch_status_rx_timeout);

    //
    // Register an intent to transmit the
    // BEL_TX_CHARGER_SETPOINT_J1939 CAN message.
    //
    device_data_ptr->send_can_switch_led_control_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            can_line,
            PKU2400_TXID_LED_CONTROLS + get_instance_offset(device),
            EXTENDED);

    // During initialise set all LED's on the pku to a known state.
    // This was included, as the first transmitted can message
    // contained garbage.
    pku2400_set_led_initial_state(device, LED_OFF);

    // Initialze the rest of the structures to a known state
    for(i = 0; i <= 7; i ++)
    {
        device_data_ptr->switch_states[i].momentary_input_switch_state = FALSE;
        device_data_ptr->switch_states[i].output_toggle_switch_state = FALSE;
        device_data_ptr->switch_states[i].previous_state = FALSE;
    }

    // Send it once to set all LED backlights to an of state.
    pku2400_tx_led_controls(device, TX_SEND_EACH_CALL);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// These are the receive CAN message timeout functions that are
// sutomatically called when a receive CAN message has not been
// received within the timeout limit that has optionally been set.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: rx_can_switch_status_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 *      Author: Tom
 *        Date: Saturday, 10 August 2019
 *
 ******************************************************************************
 */
static void rx_can_switch_status_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->can_switch_status_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// These are the receive CAN message functions that are sutomatically
// called when a matching CAN message is received.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: rx_can_switch_status()
 *
 * Description: This function is called by
 *              fvt_can_process_rx_message(). fvt_can_process_rx_message()
 *              is called by User_Can_Receive() whenever a new CAN
 *              message is received into the HED Rx fifo.
 *
 *              Use the device number to find this instance's
 *              device_data_t structure in a linked list of these
 *              structures. Copy the received CAN message data to the
 *              appropriate message structure within the device_data_t
 *              structure for this device instance.
 *
 *      Author: Deepak
 *        Date: Tuesday, 6 Aug 2019
 *
 ******************************************************************************
 */
static void rx_can_switch_status(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_timeout_counter)
{
    //
    // Check can_data_ptr is not NULL.
    //
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    can_switches_pku2400_rx_status_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->can_switch_status);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //

    memcpy(dest_ptr, can_data_ptr,
           sizeof(can_switches_pku2400_rx_status_t));


    uint8_t switch_state = (uint8_t)can_data_ptr->MDL.bit8.BYTE0;
    uint8_t i = 0;

    for (i = 0; i < PKU_SWITCH_CNT; ++i)
    {
        //
        // Use i to index into the device's switch_state array.
        //
        // Check if the switch (see can_switch_t) is pressed. This
        // will be determined by the fact that the new state of the
        // switch is greater than the previous state. This works since
        // we are, in effect, looking for a rising edge on the switch.
        //
        if ( ((switch_state >> i) & 0x1) >
             device_data_ptr->switch_states[i].previous_state)
        {
            //
            // The indexed switch is pressed. Invert (toggle) its
            // output_toggle_switch_state to indicate the state
            // change. The PKU switches configured as toggle switches
            // operate in a press ON, press OFF toggle state.
            //
            device_data_ptr->switch_states[i].output_toggle_switch_state ^= 1;

            //
            // Set this switch's toggled to TRUE to indicate to
            // pku2400_check_switches() that there's been a change in
            // the toggle state and it will set it to FALSE after it
            // processes the new state.
            //
            device_data_ptr->switch_states[i].toggled = TRUE;
        }

        //
        // Save the new previous state of the switch.
        //
        device_data_ptr->switch_states[i].previous_state = ((switch_state >> i) & 1);

    }

	*receive_timeout_counter = NO_TIME_OUT;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->can_switch_status_rx_ok = TRUE;

}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// These are the public functions that are called to transmit a
// registered CAN transmit message.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



/******************************************************************************
 *
 *        Name: pku2400_tx_led_controls()
 *
 * Description: This is a public function that is called to transmit a
 *              specific pre-registered CAN message, at a periodic
 *              rate, to the specified physical instance of this
 *              device.
 *
 *              The source of the CAN data to transmit is retrieved
 *              from a 8-byte width structure. This structure, with
 *              named uint32_t bitfields, matches precisely the
 *              pattern defined in this devices technical data sheet
 *              or user manual. The members of this structure are
 *              written to by one or more of the public setter
 *              functions designed to access each defined bitfield by
 *              name. Note: this processor is big endian and CAN is
 *              little endian. Data is always written to this source
 *              structure, by the public setter functions, in this
 *              processor's native big endian format. During the copy
 *              of source structure data to the TX CAN destination
 *              message structure, 16-bit values are byte-swapped.
 *
 *              The destination TX CAN message structure, which is
 *              passed to HED's Send_CAN_Message(), is allocated as a
 *              part of this this CAN messages transmit registration
 *              record. This TX CAN message structure has a 8-byte CAN
 *              data member. The source data, described above, must be
 *              copied to this destination 8-byte CAN message
 *              buffer. As described above, during the copy of data
 *              from the source to the destination, or after the copy,
 *              16-bit values must be byte-swapped prior to sending
 *              the CAN message.
 *
 *              The device parameter, provided by the caller, is used
 *              to retrieve a pointer into the correct element in the
 *              device_data_t linked list of structures. For example,
 *              if there are four instances of a particular device,
 *              such as a digital fuse box, each device has its own
 *              device_data_t structure. The device_data_ptr points to
 *              the correct device structure which is referred to, in
 *              subsequent comments, as the device data structure.
 *
 *              The device data structure, a compound structure,
 *              contains the source structure for this CAN transmit
 *              operation as well as a pointer to this CAN transmit
 *              message's registration record. The CAN transmit
 *              registration record has a CAN_Message_ structure. The
 *              CAN_Message_ structure contains the destination 8-byte
 *              destination message buffer.
 *
 *      Author: Deepak
 *        Date: Tuesday, 6 Aug 2019
 *
 ******************************************************************************
 */
void pku2400_tx_led_controls(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    can_switches_pku2400_tx_led_control_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_can_switch_led_control_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Handle the transmit_counter_limit so that this function's CAN
    // message is sent the first time it is called. And then sent
    // again at the rate specified in the transmit_counter_limit
    // parameter.
    //
    // NOTE: In order for the repeat transmit time to work properly,
    // the transmit function, such as this one, must be called at the
    // User_App() loop rate.
    //
    if (tx_ptr->transmit_message_sent_first_time == FALSE)
    {
        //
        // In all cases, regardless of what the transmit_counter_limit
        // parameter value is, send this function's CAN message
        // immediately the very first time this function is
        // called. After this initial send of the CAN message, the
        // next send of the CAN message will not happen until this
        // function has been called at least the number of times
        // specified in the transmit_counter_limit parameter.
        //
        tx_ptr->transmit_counter_limit = TX_SEND_EACH_CALL;
        tx_ptr->transmit_message_sent_first_time = TRUE;
    }
    else
    {
        //
        // Update the limit that the transmit counter is compared to.
        //
        tx_ptr->transmit_counter_limit = transmit_counter_limit;
    }

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit obkect.
    //
    can_switches_pku2400_tx_led_control_t *can_data_ptr =
        (can_switches_pku2400_tx_led_control_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) > tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->can_switch_led_control);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}


        source_ptr->unused1 = 0x0;
        source_ptr->unused2 = 0x0;

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(can_switches_pku2400_tx_led_control_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);

    }
}


//=============================================================================
//
// Name: can_switches_pku2400_set_timeout_switch_status()
//
// These are the PDM's public timeout setter functions for each of the
// registered receive CAN messages. They are placed here because each
// requires arguments that are very similar to the arguments used in
// the pdm_init() function during receive CAN message registration.
//
//=============================================================================
//
void pku2400_set_timeout_switch_status(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        PKU2400_RXID_STATUS + get_instance_offset(device),
        receive_timeout_counter_limit);
}


//=============================================================================
//
// pku2400_set_led_initial_state()
//
//=============================================================================
//
void pku2400_set_led_initial_state(device_instances_t device, led_state_t state)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            can_switches_pku2400_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    device_data_ptr->can_switch_led_control.green_led_top_switch_1 = state;
    device_data_ptr->can_switch_led_control.green_led_top_switch_2 = state;
    device_data_ptr->can_switch_led_control.green_led_top_switch_3 = state;
    device_data_ptr->can_switch_led_control.green_led_top_switch_4 = state;

    device_data_ptr->can_switch_led_control.red_led_top_switch_1 = state;
    device_data_ptr->can_switch_led_control.red_led_top_switch_2 = state;
    device_data_ptr->can_switch_led_control.red_led_top_switch_3 = state;
    device_data_ptr->can_switch_led_control.red_led_top_switch_4 = state;

    device_data_ptr->can_switch_led_control.green_led_bottom_switch_1 = state;
    device_data_ptr->can_switch_led_control.green_led_bottom_switch_2 = state;
    device_data_ptr->can_switch_led_control.green_led_bottom_switch_3 = state;
    device_data_ptr->can_switch_led_control.green_led_bottom_switch_4 = state;

    device_data_ptr->can_switch_led_control.red_led_bottom_switch_1 = state;
    device_data_ptr->can_switch_led_control.red_led_bottom_switch_2 = state;
    device_data_ptr->can_switch_led_control.red_led_bottom_switch_3 = state;
    device_data_ptr->can_switch_led_control.red_led_bottom_switch_4 = state;

    device_data_ptr->can_switch_led_control.blue_led_bottom_switch_4 = state;
    device_data_ptr->can_switch_led_control.blue_led_bottom_switch_3 = state;
    device_data_ptr->can_switch_led_control.blue_led_bottom_switch_2 = state;
    device_data_ptr->can_switch_led_control.blue_led_bottom_switch_1 = state;

    device_data_ptr->can_switch_led_control.blue_led_top_switch_4 = state;
    device_data_ptr->can_switch_led_control.blue_led_top_switch_3 = state;
    device_data_ptr->can_switch_led_control.blue_led_top_switch_2 = state;
    device_data_ptr->can_switch_led_control.blue_led_top_switch_1 = state;

}
