/******************************************************************************
 *
 *        Name: shinry_dc-dc_device.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
#include "shinry_dcdc_device_private.h"

//
// Private member functions
//
// These functions are private in that they are called only from HED's
// User_Can_Receive() via a function pointer that is obtained within a
// call to fvt_can_process_rx_message().
//
static void status_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

//
// These functions are private in that they are called only from HED's
// User_Can_Receive() via a pointer that is obtained from a call to
// fvt_can_process_rx_message().
//
static void rx_shinry_dcdc_status(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);


//
// Shinry follows the intel byte order standard (little endian).
//
//

// Received every 20ms
const uint32_t SHINRY_BASE_RXID = 0x18F8622B;

// Must be transmitted every 100ms
const uint32_t SHINRY_BASE_TXID = 0x10262B2;

//
// A pointer to the first structure in the linked list compound
// structures that represent all of the shinry received data messages
// and any other constructs.
//
device_data_t *first_shinry_device_data_ptr = NULL;

//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;


/******************************************************************************
 *
 *        Name: init_shinry_dcdc()
 *
 * Description: Register interest in CAN transmit and receive
 *              messages. Create records to store the data associated
 *              with each of the RX and TX messsages.
 *
 *      Author: Deepak
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
void
init_shinry_dcdc(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
	uint8_t dcdc_enable_pin)
{
    device_data_t *device_data_ptr;

#warning "Limit the number of instances that may be initialized"

    // Each instance of an FVT device driver, such as for the Dc-Dc,
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
    // instances of a device, such as two Dc-Dc chargers, the compound
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

    // According to the datasheet provided by shinry. Only one device
    // shinry should be present on the CAN bus. Shniry does not
    // support multiple devices on the same bus. Therefore, if device
    // is not ONE, return after sending a debug message.
    if (device != ONE)
    {
        return;
    }

    device_data_ptr =
        create_device_linked_data_record(
        		device,
        		&first_shinry_device_data_ptr,
        		&device_instance_counter,
                sizeof(device_data_t));

    //
    // Register an interest to receive the following CAN messages from
    // the skai2 device instance.
    //
    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SHINRY_BASE_RXID,
        rx_shinry_dcdc_status,
        status_rx_timeout);

    //
    // Register an intent to transmit the following CAN messages to
    // the skai2 device instance.
    //
    device_data_ptr->send_dcdc_control_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SHINRY_BASE_TXID,
            EXTENDED);

    //
    // Controls the power/enable signal to the DC DC converter.
    // This signal is an output controlled by the CVC.
    //
    device_data_ptr->dcdc_power_pin = dcdc_enable_pin;
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
 *        Name: status_rx_timeout()
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
static void status_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_shinry_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->status_rx_ok = FALSE;

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
 *        Name: rx_shinry_dcdc_status()
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
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
static void rx_shinry_dcdc_status(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    shinry_dcdc_rx_status_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device, first_shinry_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->status);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(shinry_dcdc_rx_status_t));

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->status_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: tx_shinry_dcdc_control()
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
 *      Author: Tom
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
void tx_shinry_dcdc_control(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    shinry_dcdc_tx_control_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            first_shinry_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_dcdc_control_ptr;

    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit object.
    //
    shinry_dcdc_tx_control_t *can_data_ptr =
        (shinry_dcdc_tx_control_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Send the CAN message if time to do so.
    //
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get
        // a pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->control);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(shinry_dcdc_tx_control_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}


/******************************************************************************
 *
 *        Name: shinry_dcdc_set_rx_timeout_contol()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *
 ******************************************************************************
 */
void shinry_dcdc_set_rx_timeout_contol(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SHINRY_BASE_RXID,
        receive_timeout_counter_limit);
}
