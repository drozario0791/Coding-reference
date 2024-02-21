/******************************************************************************
 *
 *        Name: sevcon_hvlp10_device.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
#include "sevcon_hvlp10_device_private.h"

//
// Private member functions
//
// These functions are private in that they are called only from HED's
// User_Can_Receive() via a function pointer that is obtained within a
// call to fvt_can_process_rx_message().
//
static void parameters_rx_timeout(
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
static void sevcon_hvlp10_rx_can_parameters(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);

static void sevcon_hvlp10_rx_can_motor_rpm(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_timeout_counter);


typedef enum {
    SEVCON_HVLP10_RXID_PARAMETERS = 0x331,
    SEVCON_HVLP10_RXID_MOTOR_RPM = 0x214,
} SEVCON_HVLP10_RECEIVE_IDS;

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
typedef enum {
    SEVCON_HVLP10_TXID_MSG1 = 0x191071FA,
    SEVCON_HVLP10_TXID_MSG2 = 0x191171FA,
    SEVCON_HVLP10_TXID_MSG3 = 0x191271FA,
    SEVCON_HVLP10_TXID_MSG4 = 0x80,
} DEVICE_TRANSMIT_ID;

//
// A pointer to the first structure in the linked list compound
// structures that represent all of the Bel received data messages and
// any other constructs, such as timers, required for each Bel charger
// instance.
//
device_data_t *first_sevcon_device_data_ptr = NULL;

//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;

static uint32_t get_instance_offset(device_instances_t device)
{
    return (uint32_t)(device - 1);
}


/******************************************************************************
 *
 *        Name: sevcon_hvlp10_name_init()
 *
 * Description: Initialize an instance of the sevcon hvlp10.
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void
sevcon_hvlp10_init(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line)
{
    device_data_t *device_data_ptr;

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
        		&first_sevcon_device_data_ptr,
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
        SEVCON_HVLP10_RXID_PARAMETERS + get_instance_offset(device),
        sevcon_hvlp10_rx_can_parameters,
        parameters_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SEVCON_HVLP10_RXID_MOTOR_RPM + get_instance_offset(device),
        sevcon_hvlp10_rx_can_motor_rpm,
        parameters_rx_timeout);

    //
    // Register an intent to transmit the
    // BEL_TX_CHARGER_SETPOINT_J1939 CAN message.
    //
    device_data_ptr->send_data1_can_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SEVCON_HVLP10_TXID_MSG1 + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_data2_can_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SEVCON_HVLP10_TXID_MSG2 + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_data3_can_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SEVCON_HVLP10_TXID_MSG3 + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_data4_can_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SEVCON_HVLP10_TXID_MSG4 + get_instance_offset(device),
            EXTENDED);

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
 *        Name: parameters_rx_timeout()
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
static void parameters_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->parameters_rx_ok = FALSE;

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
 *        Name: sevcon_hvlp10_rx_can_name()
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
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
static void sevcon_hvlp10_rx_can_parameters(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_timeout_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    sevcon_hvlp10_rx_parameters_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->parameters);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(sevcon_hvlp10_rx_parameters_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->sevcon_current = BYTE_SWAP16(dest_ptr->sevcon_current);
    dest_ptr->sevcon_dc_link_voltage = BYTE_SWAP16(dest_ptr->sevcon_dc_link_voltage);
    dest_ptr->sevcon_heatsink_temperature = BYTE_SWAP16(dest_ptr->sevcon_heatsink_temperature);
    dest_ptr->stepper_motor_temperature = BYTE_SWAP16(dest_ptr->stepper_motor_temperature);

	*receive_timeout_counter = NO_TIME_OUT;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->parameters_rx_ok = TRUE;
}

/******************************************************************************
 *
 *        Name: sevcon_hvlp10_rx_can_motor_rpm()
 *
 * Description:
 *
 *        Date: Sunday, 22 September 2019
 *
 ******************************************************************************
 */
static void sevcon_hvlp10_rx_can_motor_rpm(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_timeout_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    sevcon_hvlp10_rx_motor_rpm_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device,
                                                   first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->motor_rpm);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(sevcon_hvlp10_rx_motor_rpm_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->motor_rpm =
        BYTE_SWAP16(dest_ptr->motor_rpm);

	*receive_timeout_counter = NO_TIME_OUT;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->parameters_rx_ok = TRUE;
}


//////////////////////////////////////////////////////////////////////////////
//
// These are the public functions that are called to transmit a
// registered CAN transmit message.
//
//////////////////////////////////////////////////////////////////////////////


/******************************************************************************
 *
 *        Name: device_tx_can_name()
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
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */

/******************************************************************************
 *
 *        Name: sevcon_hvlp10_tx_can_set_drive_limits_and_inverter_enable()
 *
 * Description: Comments? Deepak?
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_tx_can_set_drive_limits_and_inverter_enable(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    sevcon_hvlp10_tx_data1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_data1_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit obkect.
    //
    sevcon_hvlp10_tx_data1_t *can_data_ptr =
        (sevcon_hvlp10_tx_data1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->data1);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(sevcon_hvlp10_tx_data1_t));

        can_data_ptr->set_output_torque =
            BYTE_SWAP16(can_data_ptr->set_output_torque);

        can_data_ptr->sevcon_inverter_enable =
            BYTE_SWAP16(can_data_ptr->sevcon_inverter_enable);

        can_data_ptr->set_torque_drive_limit =
            BYTE_SWAP16(can_data_ptr->set_torque_drive_limit);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

/******************************************************************************
 *
 *        Name: sevcon_hvlp10_tx_can_set_operational_limits
 *
 * Description: Comments?
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_tx_can_set_operational_limits(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    sevcon_hvlp10_tx_data2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_data2_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit obkect.
    //
    sevcon_hvlp10_tx_data2_t *can_data_ptr =
        (sevcon_hvlp10_tx_data2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->data2);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(sevcon_hvlp10_tx_data2_t));

        can_data_ptr->set_torque_regen_limit =
            BYTE_SWAP16(can_data_ptr->set_torque_regen_limit);

        can_data_ptr->set_forward_speed_limit =
            BYTE_SWAP16(can_data_ptr->set_forward_speed_limit);

        can_data_ptr->set_reverse_speed_limit =
            BYTE_SWAP16(can_data_ptr->set_reverse_speed_limit);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: sevcon_hvlp10_tx_can_battery_current_limits_and_voltage_limits()
 *
 * Description: Deepak! Comments, please!
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_tx_can_battery_current_limits_and_voltage_limits(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    sevcon_hvlp10_tx_data3_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_data3_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit obkect.
    //
    sevcon_hvlp10_tx_data3_t *can_data_ptr =
        (sevcon_hvlp10_tx_data3_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->data3);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(sevcon_hvlp10_tx_data3_t));

        can_data_ptr->set_battery_discharge_limit = BYTE_SWAP16(can_data_ptr->set_battery_discharge_limit);
        can_data_ptr->set_battery_recharge_limit = BYTE_SWAP16(can_data_ptr->set_battery_recharge_limit);
        can_data_ptr->set_voltage_capacitor_limit = BYTE_SWAP16(can_data_ptr->set_voltage_capacitor_limit);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

/******************************************************************************
 *
 *        Name: sevcon_hvlp10_tx_can_heartbeat()
 *
 * Description: Deepak! Please comment this!
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_tx_can_heartbeat(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    sevcon_hvlp10_tx_data4_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            first_sevcon_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_data4_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    //
    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to be of the type that's associated with this
    // function's transmit obkect.
    //
    sevcon_hvlp10_tx_data4_t *can_data_ptr =
        (sevcon_hvlp10_tx_data4_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >=
        tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->data4);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(sevcon_hvlp10_tx_data4_t));

        can_data_ptr->unused1 = 0;
        can_data_ptr->umused2 = 0;

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//////////////////////////////////////////////////////////////////////////////
//
// These are the PDM's public timeout setter functions for each of the
// registered receive CAN messages. They are placed here because each
// requires arguments that are very similar to the arguments used in
// the pdm_init() function during receive CAN message registration.
//
//////////////////////////////////////////////////////////////////////////////


/******************************************************************************
 *
 *        Name: sevcon_hvlp10_set_timeout_parameters() *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void sevcon_hvlp10_set_timeout_parameters(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SEVCON_HVLP10_RXID_PARAMETERS + get_instance_offset(device),
        receive_timeout_counter_limit);
}
