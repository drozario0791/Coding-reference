/******************************************************************************
 *
 *        Name: fvt_bel_charger.c
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */
#include "bel_charger_device_private.h"


//
// Private member functions
//
// These functions are called by can_rx_check_message_timeouts() in
// can_service.c. There is one function for each registered receive
// CAN message for this driver. There are associated bool_t's
// associated with each registered receive CAN message that are
// defined in this devices device_data_t structure. These functions
// are called when a CAN message has not been received for a period
// longer than the set timeout_limit value. Registered receive
// messages, by default, do not have timeouts enabled.
//
void status_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void measured_values_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void primary_values_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void secondary_values_rx_timeout(
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
static void bel_rx_can_status(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);

static void bel_rx_can_measured_values(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);

static void bel_rx_can_primary_values(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);

static void bel_rx_can_secondary_values(
		device_instances_t device,
		can_data_t *can_data_ptr,
		int16_t *receiver_counter);

typedef enum {
    BEL_RXID_CHARGER_STATUS = 0x18ffd400,
    BEL_RXID_MEASURED_VALUES = 0x18ffd500,
    BEL_RXID_PRIMARY_VALUES = 0x18ffd600,
    BEL_RXID_SECONDARY_VALUES = 0X18ffd700,
    BEL_RXID_VERSION_ID = 0x18feda00,
} BEL_RECEIVE_IDS;

//
// These are the tx message types for the Bel charger.  Just as in
// receive messages, transmit messages are registered. In the case of
// receive messages, the 'keys' into the the registered link list of
// messages are the CAN ID and the CAN LINE (CAN1, CAN2 or CAN3). In
// the case of the transmit registered messages, the 'key' into the
// message to transmit will be one of the enums listed below. These
// enums are used to both register a TX CAN message and to search for
// (through linked tx registration records) the TX CAN message to
// send.
//
typedef enum { BEL_TX_CHARGER_SETPOINT_J1939 = 0xfa00,
               BEL_TX_GPIO_SETTING_J1939 = 0xfa01,
               BEL_TX_LED_SETTING_J1939 = 0xfa02,
               BEL_TX_BATTERY_VOLATAGE_LIMITS_J1939 = 0xfa03,
} bel_charger_tx_j1939_words_t;

const uint32_t BEL_TXID_J1939_BASE = 0x18ef0000;

//
// A pointer to the first structure in the linked list compound
// structures that represent all of the Bel received data messages and
// any other constructs, such as timers, required for each Bel charger
// instance.
//
device_data_t *bel_charger_first_device_data_ptr = NULL;

//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;

static uint32_t get_tx_instance_offset(device_instances_t device)
{
    // Calculate the charger instance offset of this instance of the charger.
    //
    // modulo means relationship between charger number and expected source address is:
    //  charger_num     % 4     source_address
    //  0               0       0xD4
    //  1               1       0xD3
    //  2               2       0xD2
    //  3               3       0xD1
    return (uint32_t)((0xD4 - ((device - 1) % 4)) * 0x100);
}

static uint32_t get_rx_instance_offset(device_instances_t device)
{
    // Calculate the charger instance offset of this instance of the charger.
    //
    // modulo means relationship between charger number and expected source address is:
    //  charger_num     % 4     source_address
    //  0               0       0xD4
    //  1               1       0xD3
    //  2               2       0xD2
    //  3               3       0xD1
    return (uint32_t)(0xD4 - ((device - 1) % 4));
}

/******************************************************************************
 *
 *        Name: bel_charger_init()
 *
 * Description: Initialize an instance of the bel charger.
 *
 *      Author: Tom & Deepak
 *        Date: Monday, 17 June 2019
 *
 ******************************************************************************
 */
void
bel_charger_init(
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
    // instance, is referred to as a device_linked_data_record.
    //
    // Create the first or the next device_linked_data_record. This
    // record is created before registering an interest in this device
    // instance's CAN messages. There must be a place to store the
    // data before we receive the data.
    //
    device_data_ptr =
        create_device_linked_data_record(
        		device,
        		&bel_charger_first_device_data_ptr,
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
        BEL_RXID_CHARGER_STATUS + get_rx_instance_offset(device),
        bel_rx_can_status,
        status_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        BEL_RXID_MEASURED_VALUES + get_rx_instance_offset(device),
        bel_rx_can_measured_values,
        measured_values_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        BEL_RXID_PRIMARY_VALUES + get_rx_instance_offset(device),
        bel_rx_can_primary_values,
        primary_values_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        BEL_RXID_SECONDARY_VALUES + get_rx_instance_offset(device),
        bel_rx_can_secondary_values,
        secondary_values_rx_timeout);

    //
    // Register an intent to transmit the
    // BEL_TX_CHARGER_SETPOINT_J1939 CAN message.
    //
    device_data_ptr->send_setpoint_j1939_can_ptr =
        fvt_can_register_transmit_id_j1939(
            device,
            module_id,
            CAN3,
            BEL_TXID_J1939_BASE + get_tx_instance_offset(device),
            EXTENDED,
            BEL_TX_CHARGER_SETPOINT_J1939);

    //
    // Register an intent to transmit the BEL_TX_GPIO_SETTING_J1939.
    //
    device_data_ptr->send_gpio_setting_j1939_can_ptr =
        fvt_can_register_transmit_id_j1939(
            device,
            module_id,
            CAN3,
            BEL_TXID_J1939_BASE + get_tx_instance_offset(device),
            EXTENDED,
            BEL_TX_GPIO_SETTING_J1939);

    //
    // Register an interest to transmit the
    // BEL_TX_BATTERY_VOLATAGE_LIMITS_J1939 CAN message.
    //
    device_data_ptr->send_battery_voltage_limits_j1939_can_ptr =
        fvt_can_register_transmit_id_j1939(
            device,
            module_id,
            CAN3,
            BEL_TXID_J1939_BASE + get_tx_instance_offset(device),
            EXTENDED,
            BEL_TX_BATTERY_VOLATAGE_LIMITS_J1939);

    //
    // Register interest to transmit CAN messages to control LED
    // settings.
    //
    device_data_ptr->send_led_setting_j1939_can_ptr =
        fvt_can_register_transmit_id_j1939(
            device,
            module_id,
            CAN3,
            BEL_TXID_J1939_BASE + get_tx_instance_offset(device),
            EXTENDED,
            BEL_TX_LED_SETTING_J1939);


}


//////////////////////////////////////////////////////////////////////////////
//
// These functions are called by can_rx_check_message_timeouts() in
// can_service.c. There is one function for each registered receive
// CAN message for this driver. There are associated bool_t's
// associated with each registered receive CAN message that are
// defined in this devices device_data_t structure. These functions
// are called when a CAN message has not been received for a period
// longer than the set timeout_limit value. Registered receive
// messages, by default, do not have timeouts enabled.
//
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
void status_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
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


/******************************************************************************
 *
 *        Name: measured_values_timeout()
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
void measured_values_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->measured_values_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: primary_values_rx_timeout()
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
void primary_values_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->primary_values_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: secondary_values_rx_timeout()
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
void secondary_values_rx_timeout(
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
            bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->secondary_values_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}



//////////////////////////////////////////////////////////////////////////////
//
// These are the Bel Charger's receive functions that are called when
// one of the Bel's registered received CAN messages are received and
// processed via a call to fvt_can_process_rx_message()
//
//////////////////////////////////////////////////////////////////////////////




/******************************************************************************
 *
 *        Name: bel_rx_can_status()
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
 *      Author: Tom
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */
static void bel_rx_can_status(
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
    bel_charger_status_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
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
           sizeof(bel_charger_status_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->not_used =
        BYTE_SWAP16(dest_ptr->not_used);

	*receive_timeout_counter = NO_TIME_OUT;
    device_data_ptr->status_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: bel_rx_can_measured_values()
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
 *      Author: Tom
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */
static void bel_rx_can_measured_values(
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
    bel_charger_measured_values_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->measured_values);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bel_charger_measured_values_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->pin_17_vtd_v_20 =
        BYTE_SWAP16(dest_ptr->pin_17_vtd_v_20);

    dest_ptr->max_charging_current_ava =
        BYTE_SWAP16(dest_ptr->max_charging_current_ava);

    dest_ptr->buck_current_a_20 =
        BYTE_SWAP16(dest_ptr->buck_current_a_20);

	*receive_timeout_counter = NO_TIME_OUT;
    device_data_ptr->measured_values_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: bel_rx_can_primary_values()
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
 *      Author: Tom
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */
static void bel_rx_can_primary_values(
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
    bel_charger_primary_values_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->primary_values);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bel_charger_primary_values_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->input_voltage_rms_phase_1_2_v =
        BYTE_SWAP16(dest_ptr->input_voltage_rms_phase_1_2_v);

    dest_ptr->input_voltage_rms_phase_2_3_v =
        BYTE_SWAP16(dest_ptr->input_voltage_rms_phase_2_3_v);

    dest_ptr->input_voltage_rms_phase_3_1_v =
        BYTE_SWAP16(dest_ptr->input_voltage_rms_phase_3_1_v);

    dest_ptr->input_voltage_frequency_hz_20 =
        BYTE_SWAP16(dest_ptr->input_voltage_frequency_hz_20);

	*receive_timeout_counter = NO_TIME_OUT;
    device_data_ptr->primary_values_rx_ok = TRUE;

}


/******************************************************************************
 *
 *        Name: bel_rx_can_secondary_values()
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
 *      Author: Tom
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */
static void bel_rx_can_secondary_values(
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
    bel_charger_secondary_values_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->secondary_values);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bel_charger_secondary_values_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->output_voltage_v_20 =
        BYTE_SWAP16(dest_ptr->output_voltage_v_20);

    dest_ptr->output_current_a_20 =
        BYTE_SWAP16(dest_ptr->output_current_a_20);

    dest_ptr->aux_battery_voltage_v_20 =
        BYTE_SWAP16(dest_ptr->aux_battery_voltage_v_20);

	*receive_timeout_counter = NO_TIME_OUT;
    device_data_ptr->secondary_values_rx_ok = TRUE;
}





//////////////////////////////////////////////////////////////////////////////
//
// These are the public functions that are called to transmit a
// registered CAN transmit message.
//
//////////////////////////////////////////////////////////////////////////////





/******************************************************************************
 *
 *        Name: bel_tx_can_setpoint()
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
 *        Date: Friday, 28 June 2019
 *
 ******************************************************************************
 */
void bel_tx_can_setpoint(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    bel_charger_tx_setpoint_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_setpoint_j1939_can_ptr;
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
    bel_charger_tx_setpoint_t *can_data_ptr =
        (bel_charger_tx_setpoint_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->setpoints);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(bel_charger_tx_setpoint_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: bel_tx_can_gpio()
 *
 * Description: Refer to the description for bel_tx_can_setpoint()
 *
 *      Author: Tom
 *        Date: Friday, 28 June 2019
 *
 ******************************************************************************
 */
void bel_tx_can_gpio(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    bel_charger_tx_gpio_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_gpio_setting_j1939_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to the Bel charger's 8-byte structure that
    // represents the data that this function is sending over CAN.
    bel_charger_tx_gpio_t *can_data_ptr =
        (bel_charger_tx_gpio_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}


    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->gpios);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(bel_charger_tx_gpio_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: bel_tx_can_under_over_voltage_limits()
 *
 * Description: Refer to the description for tx_bel_can_charger_setpoint()
 *
 *      Author: Tom
 *        Date: Friday, 28 June 2019
 *
 ******************************************************************************
 */
void bel_tx_can_battery_under_over_voltage_limits(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    bel_charger_tx_battery_under_over_voltage_limits_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_battery_voltage_limits_j1939_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to the Bel charger's 8-byte structure that
    // represents the data that this function is sending over CAN.
    bel_charger_tx_battery_under_over_voltage_limits_t *can_data_ptr =
        (bel_charger_tx_battery_under_over_voltage_limits_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->limits);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(bel_charger_tx_battery_under_over_voltage_limits_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

void bel_tx_can_led_duty_cycle(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    bel_charger_tx_led_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_led_setting_j1939_can_ptr;
    if (tx_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Update the limit that the transmit counter is compared to.
    //
    tx_ptr->transmit_counter_limit = transmit_counter_limit;

    // Cast our pointer to the 8-byte Can_Message_ tx message data to
    // be a pointer to the Bel charger's 8-byte structure that
    // represents the data that this function is sending over CAN.
    bel_charger_tx_led_t *can_data_ptr =
        (bel_charger_tx_led_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->leds);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(bel_charger_tx_led_t));

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
 *        Name: bel_set_timeout_status()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *      Author: Tom
 *        Date: Thursday, 25 July 2019
 *
 ******************************************************************************
 */
void bel_set_timeout_status(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        BEL_RXID_CHARGER_STATUS + get_rx_instance_offset(device),
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: bel_set_timeout_measured_values()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *
 *
 *      Author: Tom
 *        Date: Thursday, 25 July 2019
 *
 ******************************************************************************
 */
void bel_set_timeout_measured_values(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        BEL_RXID_MEASURED_VALUES + get_rx_instance_offset(device),
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: bel_set_timeout_primary_values()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *      Author: Tom
 *        Date: Thursday, 25 July 2019
 *
 ******************************************************************************
 */
void bel_set_timeout_primary_values(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        BEL_RXID_PRIMARY_VALUES + get_rx_instance_offset(device),
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: bel_set_timeout_secondary_values()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *      Author: Tom
 *        Date: Thursday, 25 July 2019
 *
 ******************************************************************************
 */
void bel_set_timeout_secondary_values(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        BEL_RXID_SECONDARY_VALUES + get_rx_instance_offset(device),
        receive_timeout_counter_limit);
}
