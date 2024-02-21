/******************************************************************************
 *
 *        Name: skai2_inverter.c
 *
 * Description: This source file consists of the device driver for the
 *              skai2 inverter. The following actions are done by the
 *              driver:
 *
 *              i) Initialztion of skai inverters : The initialize
 *              function is called from userInit.c. The function
 *              initializes the TX and RX messages accosicated with
 *              the SKAI2 inverter. Sets up the handler functions to
 *              be called when a message is received. Also, registers
 *              an intent to transmit the message.
 *
 *              ii) skai2_inverter_control(): determines the action to
 *              be taken based on information provided to the
 *              function. For example:Deals with enabling/disabling
 *              the inverter under safe conditions.
 *
 *              iii) Transmit and Receive helper function: A list of
 *              RX_Handler functions that are called when a message of
 *              a particular CAN ID is received. Also CAN TX functions
 *              that can be called to transmit a message.
 *
 *      Author: Deepak
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
#include "skai2_inverter_private.h"

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
//
static void skai2_status_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void skai2_temperature_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void skai2_EA_RPM_rx_timout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void skai2_torque_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

//
// Private member functions
//
// These functions are private in that they are called only from HED's
// User_Can_Receive() via a pointer that is obtained from a call to
// fvt_can_process_rx_message().
//

// Function Prototypes for can RX functions
static void rx_skai2_status(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

static void rx_skai2_temperature(device_instances_t device,
								 can_data_t *can_data_ptr,
								 int16_t *receive_counter);

static void rx_skai2_EA_RPM(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

static void rx_skai2_torque(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

//
// A pointer to the first structure in the linked list compound
// structures that represent all of the skai inverter received data
// messages and any other constructs, such as timers, required for
// each Skai inverter instance.
//
device_data_t *first_skai2_device_data_ptr = NULL;

//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;

// TX and RX CAN ID's
typedef enum {
	SKAI2_RXID_STATUS = 0x00101000,
	SKAI2_RXID_TEMPERATURE = 0x00101001,
	SKAI2_RXID_EA_RPM = 0x00101005,
	SKAI2_RXID_TORQUE = 0x00101007,
} SKAI2_RECEIVE_IDS;

typedef enum {
    SKAI2_INVERTER_TX_CONTROL = 0x00111000,
    SKAI2_INVERTER_TX_LIMITS =  0x00111001,
} SKAI2_TRANSMIT_IDS;

/******************************************************************************
 *
 *        Name: get_instance_offset()
 *
 * Description: The function returns the offset based on the device
 *              instance provided to the function. This is used when
 *              multiple of the same devices are used in the system.
 *
 *      Author: Deepak
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
static uint32_t get_instance_offset(device_instances_t device)
{
	// Calculate the offset for the inverters based on the device
	// instance provided.
	// Relation between canIDs are as follows:
	// Inverter 0 : 0x00111000
	// Inverter 1 : 0x00111100
	// Inverter 2 : 0x00111200
	// Inverter 3 : 0x00111300
	// Inverter 4 : 0x00111400
	// Inverter 5 : 0x00111500
	// formula = (device - 1) * 0x100

	uint32_t offset = (uint32_t)((device) * 0x100);

    return offset;
}

/******************************************************************************
 *
 *        Name: init_skai2_inverter()
 *
 * Description: Register interest in CAN transmit and receive
 *              messages. Create records to store the data associated
 *              with each of the RX and TX messsages.
 *
 *      Author: Tom
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
void
init_skai2_inverter(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
	uint8_t inverter_power_pin)
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
        		&first_skai2_device_data_ptr,
        		&device_instance_counter,
                sizeof(device_data_t));

    uint32_t foo = (uint32_t)get_instance_offset(device);

    //
    // Register an interest to receive the following CAN messages from
    // the skai2 device instance.
    //
    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_STATUS + get_instance_offset(device),
        rx_skai2_status,
        skai2_status_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_TEMPERATURE + get_instance_offset(device),
        rx_skai2_temperature,
        skai2_temperature_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_EA_RPM + get_instance_offset(device),
        rx_skai2_EA_RPM,
        skai2_EA_RPM_rx_timout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_TORQUE + get_instance_offset(device),
        rx_skai2_torque,
        skai2_torque_rx_timeout);

    //
    // Register an intent to transmit the following CAN messages to
    // the skai2 device instance.
    //
    device_data_ptr->send_inverter_control_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_CONTROL + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_limits_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_LIMITS + get_instance_offset(device),
            EXTENDED);


    device_data_ptr->inverter_power_pin = inverter_power_pin;
}

/******************************************************************************
 *
 *        Name: skai2_status_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 *
 ******************************************************************************
 */
static void skai2_status_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_status_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}

/******************************************************************************
 *
 *        Name: skai2_temperature_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 *
 ******************************************************************************
 */
static void skai2_temperature_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_temperature_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}

/******************************************************************************
 *
 *        Name: skai2_EA_RPM_rx_timout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 *
 ******************************************************************************
 */
static void skai2_EA_RPM_rx_timout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_EA_RPM_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name:
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 *
 ******************************************************************************
 */
static void skai2_torque_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_torque_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}

//=============================================================================
//
// Skai2 Receive Functions
//
//=============================================================================


/******************************************************************************
 *
 *        Name: rx_skai2_status
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
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_status(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t * receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_status_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
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
           sizeof(skai2_inverter_status_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->error_feedback_1 = BYTE_SWAP16(dest_ptr->error_feedback_1);
    dest_ptr->error_feedback_2 = BYTE_SWAP16(dest_ptr->error_feedback_2);
    dest_ptr->MotorTemp_C = BYTE_SWAP16(dest_ptr->MotorTemp_C);
    dest_ptr->DCLink_Voltage = BYTE_SWAP16(dest_ptr->DCLink_Voltage);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_status_rx_ok = TRUE;

}

/******************************************************************************
 *
 *        Name: rx_skai2_temperature
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
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_temperature(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t * receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_temperature_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->temperature);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_temperature_t));

	*receive_counter = 0;

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->temperature_dcb_phase_L1 =
        BYTE_SWAP16(dest_ptr->temperature_dcb_phase_L1);

    dest_ptr->temperature_dcb_phase_L2 =
        BYTE_SWAP16(dest_ptr->temperature_dcb_phase_L2);

    dest_ptr->temperature_dcb_phase_L3 =
        BYTE_SWAP16(dest_ptr->temperature_dcb_phase_L3);

    dest_ptr->temperature_pcb =
        BYTE_SWAP16(dest_ptr->temperature_pcb);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_temperature_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_skai2_EA_RPM()
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
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_EA_RPM(
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
    skai2_inverter_eleAngle_RPM_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->ElectricalAngle_RPM);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_eleAngle_RPM_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->electrical_offset_ccw =
        BYTE_SWAP16(dest_ptr->electrical_offset_ccw);

	dest_ptr->max_temperature =
        BYTE_SWAP16(dest_ptr->max_temperature);

	dest_ptr->motor_rpm =
        BYTE_SWAP16(dest_ptr->motor_rpm);

	dest_ptr->electrical_offset_cw =
        BYTE_SWAP16(dest_ptr->electrical_offset_cw);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_EA_RPM_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_skai2_torque
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
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_torque(
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
    skai2_inverter_torque_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(
            device,
            first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->motor_torque);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_torque_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->motor_torque = BYTE_SWAP16(dest_ptr->motor_torque);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_torque_rx_ok = TRUE;
}


//=============================================================================
//
// Skai2 Transmit Functions
//
//=============================================================================


/******************************************************************************
 *
 *        Name: tx_skai2_control()
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
void tx_skai2_control(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{

    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_control_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_control_ptr;

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
    skai2_inverter_tx_control_t *can_data_ptr =
        (skai2_inverter_tx_control_t *)tx_ptr->tx_message.data;
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
               sizeof(skai2_inverter_tx_control_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
        can_data_ptr->commanded_rpm = BYTE_SWAP16(source_ptr->commanded_rpm);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }


}

/******************************************************************************
 *
 *        Name: tx_skai2_limits()
 *
 * Description: Refer to the description for the tx_skai2_control().
 *
 *      Author: Deepak
 *        Date: Monday, 08 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_limits(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_limits_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_limits_ptr;
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
    skai2_inverter_tx_limits_t *can_data_ptr =
        (skai2_inverter_tx_limits_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->limits);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_limits_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
        can_data_ptr->max_battery_current = BYTE_SWAP16(source_ptr->max_battery_current);
        can_data_ptr->instantaneous_battery_current = BYTE_SWAP16(source_ptr->instantaneous_battery_current);
        can_data_ptr->high_cell_voltage = BYTE_SWAP16(source_ptr->high_cell_voltage);
        can_data_ptr->mode = BYTE_SWAP16(source_ptr->mode);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: skai2_set_timeout_status()
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
void skai2_set_timeout_status(device_instances_t device,
                              uint8_t module_id,
                              CANLINE_ can_line,
                              can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_STATUS + get_instance_offset(device),
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: skai2_set_timeout_temperature()
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
void skai2_set_timeout_temperature(device_instances_t device,
                                   uint8_t module_id,
                                   CANLINE_ can_line,
                                   can_rate_t receive_timeout_counter_limit)
{

	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_TEMPERATURE + get_instance_offset(device),
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: skai2_set_timeout_EA_RPM()
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
void skai2_set_timeout_EA_RPM(device_instances_t device,
                              uint8_t module_id,
                              CANLINE_ can_line,
                              can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_EA_RPM + get_instance_offset(device),
        receive_timeout_counter_limit);
}


/******************************************************************************
 *
 *        Name: skai2_set_timeout_torque()
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
void skai2_set_timeout_torque(device_instances_t device,
                              uint8_t module_id,
                              CANLINE_ can_line,
                              can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_TORQUE + get_instance_offset(device),
        receive_timeout_counter_limit);
}
