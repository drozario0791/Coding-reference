/******************************************************************************
 *
 *        Name: pdm_device.c
 *
 * Description: A device driver to support the PDM.
 *
 *      Author: Tom
 *        Date: Friday, 21 June 2019
 *
 ******************************************************************************
 */
#include "pdm_device_private.h"


//
// Private member functions
//
// These functions are private in that they are called only from HED's
// User_Can_Receive() via a function pointer that is obtained within a
// call to fvt_can_process_rx_message().
//
static void analog_in_1_2_digital_in_feedback_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void analog_in_3_4_digital_out_feedback_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void analog_in_5_6_battery_sensor_supply_feedback_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void analog_in_7_8_power_version_feedback_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void output_current_1_6_feedback_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void output_current_7_12_feedback_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void output_function_handshake_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void output_configuration_1_6_handshake_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void output_configuration_7_12_handshake_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);



static void rx_can_analog_in_1_2_digital_in_feedback(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_analog_in_3_4_digital_out_feedback(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_analog_in_5_6_battery_sensor_supply_feedback(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_analog_in_7_8_power_version_feedback(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_output_current_1_6_feedback(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_output_current_7_12_feedback(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_output_function_handshake(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_output_configuration_1_6_handshake(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);

static void rx_can_output_configuration_7_12_handshake(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter);


//
// This is the BASE CAN ID we use to receive messages from the
// pdm.  It is modified by summing with the result of
// get_rx_instance_offset(device). Note: SA is 00.
//
const uint32_t PDM_BASE_RXID = 0x14ef1100;


static uint32_t get_rx_instance_offset(device_instances_t device)
{
    // Calculate the charger instance offset of this instance of the charger.
    //
    //  charger_num   SA (source_address)
    //  ONE           0x1E
    //  TWO           0x1F
    //  THREE         0x20
    //  FOUR          0x21
    return (uint32_t)(0x1E + ((device - 1) % 4));
}


//
// This is the BASE CAN ID we use to send messages to the pdm. It
// is modified by summing with the result of
// get_tx_instance_offset(device). Note: SA is 00.
//
const uint32_t PDM_BASE_TXID = 0x14ef0011;


static uint32_t get_tx_instance_offset(device_instances_t device)
{
    // Calculate the charger instance offset of this instance of the charger.
    //
    //  charger_num   SA (source_address)
    //  ONE           0x1E
    //  TWO           0x1F
    //  THREE         0x20
    //  FOUR          0x21
    return (uint32_t)((0x1E + ((device - 1) % 4)) * 0x100);
}


//
// Unique identifier for each CAN message received, located in the
// first byte of the CAN message data. This is known as the
// j1939_byte.
//
typedef enum {
    ANALOG_IN_1_2_DIGITAL_IN_FEEDBACK = 0x80,
    ANALOG_IN_3_4_DIGITAL_OUT_FEEDBACK = 0x81,
    ANALOG_IN_5_6_BATTERY_SENSOR_SUPPLY_FEEDBACK = 0x82,
    ANALOG_IN_7_8_POWER_VERSION_FEEDBACK = 0X83,
    OUTPUT_CURRENT_1_6_FEEDBACK = 0x84,
    OUTPUT_CURRENT_7_12_FEEDBACK = 0x85,
    OUTPUT_FUNCTION_HANDSHAKE = 0x86,
    OUTPUT_CONFIGURATION_1_6_HANDSHAKE = 0x87,
    OUTPUT_CONFIGURATION_7_12_HANDSHAKE = 0x88,
} pdm_rx_j1939_bytes_t;


//
// These are the tx message types for the pdm. Just as in
// receive messages, transmit messages are registered. In the case of
// receive messages, the 'keys' into the the registered link list of
// messages are the CAN ID and the CAN LINE (CAN1, CAN2 or CAN3). In
// the case of the transmit registered messages, the 'key' into the
// message to transmit will be one of the enums listed below. These
// enums are used to both register a TX CAN message.
//
// Unique identifier for each CAN message transmitted, located in the
// first byte of the CAN message data. This is known as the
// j1939_byte.
//
typedef enum {
    CONFIGURE_OUTPUT_FUNCTION = 0,
    CONFIGURE_OUTPUT_CHANNELS_1_6 = 6,
    CONFIGURE_OUTPUT_CHANNELS_7_12 = 7,
    COMMAND_OUTPUT_CHANNELS_1_6 = 4,
    COMMAND_OUTPUT_CHANNELS_7_12 = 5,
} pdm_tx_j1939_bytes_t;



//
// A pointer to the first structure in the linked list compound
// structures that represent all of the Bel received data messages and
// any other constructs, such as timers, required for each Bel charger
// instance.
//
device_data_t *pdm_first_device_data_ptr = NULL;


//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;


/******************************************************************************
 *
 *        Name: pdm_init()
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 17 June 2019
 *
 ******************************************************************************
 */
void
pdm_init(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line)
{

    device_data_t *device_data_ptr;

#warning "Limit the number of instances that may be initialized?"

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
        		&pdm_first_device_data_ptr,
        		&device_instance_counter,
                sizeof(device_data_t));

    //
    // Register an interest to receive the following CAN messages from
    // the pdm device instance.
    //
    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_1_2_DIGITAL_IN_FEEDBACK,
        rx_can_analog_in_1_2_digital_in_feedback,
        analog_in_1_2_digital_in_feedback_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_3_4_DIGITAL_OUT_FEEDBACK,
        rx_can_analog_in_3_4_digital_out_feedback,
        analog_in_3_4_digital_out_feedback_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_5_6_BATTERY_SENSOR_SUPPLY_FEEDBACK,
        rx_can_analog_in_5_6_battery_sensor_supply_feedback,
        analog_in_5_6_battery_sensor_supply_feedback_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_7_8_POWER_VERSION_FEEDBACK,
        rx_can_analog_in_7_8_power_version_feedback,
        analog_in_7_8_power_version_feedback_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CURRENT_1_6_FEEDBACK,
        rx_can_output_current_1_6_feedback,
        output_current_1_6_feedback_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CURRENT_7_12_FEEDBACK,
        rx_can_output_current_7_12_feedback,
        output_current_7_12_feedback_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_FUNCTION_HANDSHAKE,
        rx_can_output_function_handshake,
        output_function_handshake_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CONFIGURATION_1_6_HANDSHAKE,
        rx_can_output_configuration_1_6_handshake,
        output_configuration_1_6_handshake_rx_timeout);

    fvt_can_register_receive_id_j1939_byte(
        device,
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CONFIGURATION_7_12_HANDSHAKE,
        rx_can_output_configuration_7_12_handshake,
        output_configuration_7_12_handshake_rx_timeout);

    //
    // Register an intent to transmit the following CAN messages to
    // the pdm device instance.
    //
    device_data_ptr->send_configure_output_function_ptr =
        fvt_can_register_transmit_id_j1939_byte(
            device,
            module_id,
            can_line,
            PDM_BASE_TXID + get_tx_instance_offset(device),
            EXTENDED,
            CONFIGURE_OUTPUT_FUNCTION);

    device_data_ptr->send_configure_output_channels_1_6_ptr =
        fvt_can_register_transmit_id_j1939_byte(
            device,
            module_id,
            can_line,
            PDM_BASE_TXID + get_tx_instance_offset(device),
            EXTENDED,
            CONFIGURE_OUTPUT_CHANNELS_1_6);

    device_data_ptr->send_configure_output_channels_7_12_ptr =
        fvt_can_register_transmit_id_j1939_byte(
            device,
            module_id,
            can_line,
            PDM_BASE_TXID + get_tx_instance_offset(device),
            EXTENDED,
            CONFIGURE_OUTPUT_CHANNELS_7_12);

    device_data_ptr->send_command_output_channels_1_6_ptr =
        fvt_can_register_transmit_id_j1939_byte(
            device,
            module_id,
            can_line,
            PDM_BASE_TXID + get_tx_instance_offset(device),
            EXTENDED,
            COMMAND_OUTPUT_CHANNELS_1_6);

    device_data_ptr->send_command_output_channels_7_12_ptr =
        fvt_can_register_transmit_id_j1939_byte(
            device,
            module_id,
            can_line,
            PDM_BASE_TXID + get_tx_instance_offset(device),
            EXTENDED,
            COMMAND_OUTPUT_CHANNELS_7_12);

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
 *        Name: analog_in_1_2_digital_in_feedback_rx_timeout()
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
static void analog_in_1_2_digital_in_feedback_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->analog_in_1_2_digital_in_feedback_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: analog_in_3_4_digital_out_feedback_rx_timeout()
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
static void analog_in_3_4_digital_out_feedback_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->analog_in_3_4_digital_out_feedback_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: analog_in_5_6_battery_sensor_supply_feedback_rx_timeout()
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
static void analog_in_5_6_battery_sensor_supply_feedback_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->
        analog_in_5_6_battery_sensor_supply_feedback_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(
        device,
        module_id,
        can_line,
        can_id,
        j1939_byte);
}


/******************************************************************************
 *
 *        Name: analog_in_7_8_power_version_feedback_rx_timeout()
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
static void analog_in_7_8_power_version_feedback_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->
        analog_in_7_8_power_version_feedback_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(
        device,
        module_id,
        can_line,
        can_id,
        j1939_byte);
}


/******************************************************************************
 *
 *        Name: output_current_1_6_feedback_rx_timeout()
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
static void output_current_1_6_feedback_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->
        output_current_1_6_feedback_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(
        device,
        module_id,
        can_line,
        can_id,
        j1939_byte);
}


/******************************************************************************
 *
 *        Name: output_current_7_12_feedback_rx_timeout()
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
static void output_current_7_12_feedback_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->
        output_current_7_12_feedback_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: output_function_handshake_rx_timeout()
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
static void output_function_handshake_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->
        output_function_handshake_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: output_configuration_1_6_handshake_rx_timeout()
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
static void output_configuration_1_6_handshake_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->
        output_configuration_1_6_handshake_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: output_configuration_7_12_handshake_rx_timeout()
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
static void output_configuration_7_12_handshake_rx_timeout(
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
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->
        output_configuration_7_12_handshake_rx_ok = FALSE;

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
 *        Name: rx_can_analog_in_1_2_digital_in_feedback()
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
static void rx_can_analog_in_1_2_digital_in_feedback(
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
    analog_in_1_2_digital_in_feedback_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->
                 analog_in_1_2_digital_in_feedback);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(
        dest_ptr,
        can_data_ptr,
        sizeof(analog_in_1_2_digital_in_feedback_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->analog_input_1 =
        BYTE_SWAP16(dest_ptr->analog_input_1);

    dest_ptr->analog_input_2 =
        BYTE_SWAP16(dest_ptr->analog_input_2);

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->analog_in_1_2_digital_in_feedback_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_analog_in_3_4_digital_out_feedback()
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
static void rx_can_analog_in_3_4_digital_out_feedback(
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
    analog_in_3_4_digital_out_feedback_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->analog_in_3_4_digital_out_feedback);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(analog_in_3_4_digital_out_feedback_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->analog_input_3 = BYTE_SWAP16(dest_ptr->analog_input_3);
    dest_ptr->analog_input_4 = BYTE_SWAP16(dest_ptr->analog_input_4);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->analog_in_3_4_digital_out_feedback_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_analog_in_5_6_battery_sensor_supply_feedback()
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
static void rx_can_analog_in_5_6_battery_sensor_supply_feedback(
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
    analog_in_5_6_battery_sensor_supply_feedback_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(analog_in_5_6_battery_sensor_supply_feedback_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->battery_voltage = BYTE_SWAP16(dest_ptr->battery_voltage);
    dest_ptr->analog_input_5 = BYTE_SWAP16(dest_ptr->analog_input_5);
    dest_ptr->analog_input_6 = BYTE_SWAP16(dest_ptr->analog_input_6);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->analog_in_5_6_battery_sensor_supply_feedback_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_analog_in_7_8_power_version_feedback()
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
static void rx_can_analog_in_7_8_power_version_feedback(
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
    analog_in_7_8_power_version_feedback_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->
                 analog_in_7_8_power_version_feedback);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(analog_in_7_8_power_version_feedback_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->software_version =
        BYTE_SWAP16(dest_ptr->software_version);

    dest_ptr->analog_input_7 =
        BYTE_SWAP16(dest_ptr->analog_input_7);

    dest_ptr->analog_input_8 =
        BYTE_SWAP16(dest_ptr->analog_input_8);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->analog_in_7_8_power_version_feedback_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_output_current_1_6_feedback()
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
static void rx_can_output_current_1_6_feedback(
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
    output_current_feedback_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->output_current_1_6_feedback);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(output_current_feedback_t));

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->output_current_1_6_feedback_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_output_current_7_12_feedback()
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
static void rx_can_output_current_7_12_feedback(
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
    output_current_feedback_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->output_current_7_12_feedback);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(output_current_feedback_t));

    *receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->output_current_7_12_feedback_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_output_function_handshake()
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
static void rx_can_output_function_handshake(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the CAN messsage data structure that
    // is cast as a pointer to a output_function_handshake_t structure
    // so that the channel_number may be easily read from the
    // can_message.
    //
    output_function_handshake_t *source_ptr = (output_function_handshake_t *)can_data_ptr;
    uint8_t channel = (uint8_t)(source_ptr->channel_number);

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    output_function_handshake_t *dest_ptr =
        &(device_data_ptr->output_function_handshake[channel]);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(output_function_handshake_t));

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->output_function_handshake_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_output_configuration_1_6_handshake()
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
static void rx_can_output_configuration_1_6_handshake(
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
    output_configuration_handshake_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->output_configuration_1_6_handshake);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(output_configuration_handshake_t));

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->output_configuration_1_6_handshake_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_can_output_configuration_7_12_handshake()
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
static void rx_can_output_configuration_7_12_handshake(
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
    output_configuration_handshake_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->output_configuration_7_12_handshake);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(output_configuration_handshake_t));

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->output_configuration_7_12_handshake_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: pdm_tx_can_configure_output_function()
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
 *
 *      Author: Tom
 *        Date: Wednesday, 10 July 2019
 *
 *******************************************************************************/
void pdm_tx_can_configure_output_function(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    configure_output_function_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_configure_output_function_ptr;
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
    // function's transmit object.
    //
    configure_output_function_t *can_data_ptr =
        (configure_output_function_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Send the CAN message if time to do so.
    //
    if (++(tx_ptr->transmit_counter) > tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        for (int channel = PDM_CHANNEL_1; channel <= PDM_CHANNEL_12 ; ++channel)
        {
            //
            // Have the pointer to this device's data record, not get a
            // pointer to the appropriate receive data structure.
            //
            source_ptr = &(device_data_ptr->configure_output_function[channel - 1]);
            if (source_ptr == NULL) {DEBUG("NULL Pointer");}

            //
            // Copy data from our local structure to the CAN transmit buffer.
            //
            memcpy(can_data_ptr, source_ptr,
                   sizeof(configure_output_function_t));

            Send_CAN_Message(tx_ptr->module_id,
                             tx_ptr->can_line,
                             tx_ptr->tx_message);
        }
    }
}


/******************************************************************************
 *
 *        Name: pdm_tx_can_configure_output_channels_1_6()
 *
 * Description: Refer to first Tx function description.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void pdm_tx_can_configure_output_channels_1_6(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    configure_output_channels_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    source_ptr = &(device_data_ptr->configure_output_channels_1_6);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_configure_output_channels_1_6_ptr;
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
    configure_output_channels_t *can_data_ptr =
        (configure_output_channels_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Send the CAN message if time to do so.
    //
    if (++(tx_ptr->transmit_counter) > tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = TX_SEND_EACH_CALL;

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(configure_output_channels_t));

        // Send the CAN message
        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: pdm_tx_can_configure_output_channels_7_12()
 *
 * Description: Refer to first Tx function description.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void pdm_tx_can_configure_output_channels_7_12(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    configure_output_channels_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    source_ptr = &(device_data_ptr->configure_output_channels_7_12);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_configure_output_channels_7_12_ptr;
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
    // function's transmit object.
    //
    configure_output_channels_t *can_data_ptr =
        (configure_output_channels_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) > tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(configure_output_channels_t));

        // Send the CAN message
        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}



/******************************************************************************
 *
 *        Name: pdm_tx_can_command_output_channels_1_6()
 *
 * Description: Refer to first Tx function description.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void pdm_tx_can_command_output_channels_1_6(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    command_output_channels_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    source_ptr = &(device_data_ptr->command_output_channels_1_6);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_command_output_channels_1_6_ptr;
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
    // function's transmit object.
    //
    command_output_channels_t *can_data_ptr =
        (command_output_channels_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) > tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(command_output_channels_t));

        // Send the CAN message
        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: pdm_tx_can_command_output_channels_7_12()
 *
 * Description: Refer to first Tx function description.
 *
 *      Author: Tom
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void pdm_tx_can_command_output_channels_7_12(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    command_output_channels_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, pdm_first_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    source_ptr = &(device_data_ptr->command_output_channels_7_12);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_command_output_channels_7_12_ptr;
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
    // function's transmit object.
    //
    command_output_channels_t *can_data_ptr =
        (command_output_channels_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) > tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;
        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(command_output_channels_t));

        // Send the CAN message
        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: Set_RX_CAN_TIMEOUT
 *
  ******************************************************************************
 */


/******************************************************************************
 *
 *        Name: pdm_set_timeout_analog_in_1_2_digital_in_feedback()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_analog_in_1_2_digital_in_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_1_2_DIGITAL_IN_FEEDBACK,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_analog_in_3_4_digital_in_feedback()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_analog_in_3_4_digital_out_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_3_4_DIGITAL_OUT_FEEDBACK,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_analog_in_5_6_battery_sensor_supply_feedback()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_analog_in_5_6_battery_sensor_supply_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_5_6_BATTERY_SENSOR_SUPPLY_FEEDBACK,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_analog_in_7_8_power_version_feedback()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_analog_in_7_8_power_version_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        ANALOG_IN_7_8_POWER_VERSION_FEEDBACK,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_output_current_1_6_feedback
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_output_current_1_6_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CURRENT_1_6_FEEDBACK,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_output_current_7_12_feedback()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_output_current_7_12_feedback(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CURRENT_7_12_FEEDBACK,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_analog_in_1_2_digital_in_feedback()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_output_function_handshake(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_FUNCTION_HANDSHAKE,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_output_configuration_1_6_handshake()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_output_configuration_1_6_handshake(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CONFIGURATION_1_6_HANDSHAKE,
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: pdm_set_timeout_output_configuration_7_12_handshake()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Tuesday, 30 July 2019
 *
 ******************************************************************************
 */
void pdm_set_timeout_output_configuration_7_12_handshake(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id_j1939_byte(
        module_id,
        can_line,
        PDM_BASE_RXID + get_rx_instance_offset(device),
        OUTPUT_CONFIGURATION_7_12_HANDSHAKE,
        receive_timeout_counter_limit);
}
