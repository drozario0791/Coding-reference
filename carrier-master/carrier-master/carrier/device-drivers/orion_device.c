/******************************************************************************
 *
 *        Name: orion_device.c
 *
 * Description: This source file consists of the device driver for the
 *              orion BMS.
 *
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
#include "orion_device_private.h"

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
void bms_data1_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void bms_data2_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void bms_data3_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void custom_faults_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void bms_data5_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void bms_data6_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void bms_data7_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void bms_data8_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

void bms_data9_rx_timeout(
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
static void rx_orion_inst_data(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_dcl_ccl_temp(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_bms_data(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_custom_faults(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_faults_with_identifiers(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_cell_data_1(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_cell_data_2(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_pack_isolation_fault(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

static void rx_orion_pack_cycle_data(
    device_instances_t device,
    can_data_t *can_data_ptr,
    int16_t *receive_counter);

//
// A pointer to the first structure in the linked list compound
// structures that represent all of the skai inverter received data
// messages and any other constructs, such as timers, required for
// each orion bms instance.
//
device_data_t *first_orion_device_data_ptr = NULL;

//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;

//
// Recieve CAN ID's
//
typedef enum {
	ORION_BMS_INST_DATA = 0x001B5001,
	ORION_BMS_DCL_CCL_TEMP = 0x001B5002,
	ORION_BMS_MISC_DATA = 0x001B5003,
	ORION_BMS_CUSTOM_FAULTS = 0x001B5004,
	ORION_BMS_FAULTS_WITH_IDENTIFIERS = 0x001B5005,
	ORION_BMS_CELL_DATA_1 = 0x001B5006,
	ORION_BMS_CELL_DATA_2 = 0x001B5007,
	ORION_BMS_PACK_ISO_FAULT = 0x001B5008,
	ORION_BMS_CYCLE_DATA = 0x001B5009,
} ORION_BMS_RECEIVE_IDS;


/******************************************************************************
 *
 *        Name: get_instance_offset()
 *
 * Description: The function returns the offset based on the device
 *              instance provided to the function. This is used when
 *              multiple of the same devices are used in the system.
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static uint32_t get_instance_offset(device_instances_t device)
{
	// Calculate the offset for the inverters based on the device
	// instance provided.
	// Relation between canIDs are as follows:
	// Orion BMS 0 : 0x001B5001
	// Orion BMS 1 : 0x002B5001
	// Orion BMS 2 : 0x003B5001

	// formula = (device - 1) * 0x100

	uint32_t offset = (uint32_t)((device - 1)* 0x00100000);

    return offset;
}


/******************************************************************************
 *
 *        Name: init_orion_bms()
 *
 * Description: Register interest in CAN transmit and receive
 *              messages. Create records to store the data associated
 *              with each of the RX and TX messsages.
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
void init_orion_bms(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line)
{
    device_data_t *device_data_ptr;


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
        		&first_orion_device_data_ptr,
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
        ORION_BMS_INST_DATA + get_instance_offset(device),
        rx_orion_inst_data,
        bms_data1_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_DCL_CCL_TEMP + get_instance_offset(device),
        rx_orion_dcl_ccl_temp,
        bms_data2_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_MISC_DATA + get_instance_offset(device),
        rx_orion_bms_data,
        bms_data3_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_CUSTOM_FAULTS + get_instance_offset(device),
        rx_orion_custom_faults,
        custom_faults_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_FAULTS_WITH_IDENTIFIERS + get_instance_offset(device),
        rx_orion_faults_with_identifiers,
        bms_data5_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_CELL_DATA_1 + get_instance_offset(device),
        rx_orion_cell_data_1,
        bms_data6_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_CELL_DATA_2 + get_instance_offset(device),
        rx_orion_cell_data_2,
        bms_data7_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_PACK_ISO_FAULT + get_instance_offset(device),
        rx_orion_pack_isolation_fault,
        bms_data8_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        ORION_BMS_CYCLE_DATA + get_instance_offset(device),
        rx_orion_pack_cycle_data,
        bms_data9_rx_timeout);


}


/******************************************************************************
 *
 *        Name: bms_data1_rx_timeout()
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
void bms_data1_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data1_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: bms_data2_rx_timeout()
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
void bms_data2_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data2_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: bms_data3_rx_timeout()
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
void bms_data3_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data3_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: custom_faults_rx_timeout()
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
void custom_faults_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->custom_faults_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: bms_data5_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message associated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 *      Author: Tom
 *        Date: Saturday, 10 August 2019
 *
 ******************************************************************************
 */
void bms_data5_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data5_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: bms_data6_rx_timeout()
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
void bms_data6_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data6_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: bms_data7_rx_timeout()
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
void bms_data7_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data7_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: bms_data8_rx_ok_timeout()
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
void bms_data8_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data8_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: bms_data9_rx_timeout()
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
void bms_data9_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->bms_data9_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}






/******************************************************************************
 *
 *        Name: rx_orion_inst_data()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_inst_data(device_instances_t device,
								can_data_t *can_data_ptr,
								int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data1_t *dest_ptr = NULL;
    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data1);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data1_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->instantaneous_pack_current = BYTE_SWAP16(dest_ptr->instantaneous_pack_current);
	dest_ptr->instantaneous_pack_voltage = BYTE_SWAP16(dest_ptr->instantaneous_pack_voltage);
	dest_ptr->pack_high_cell_voltage = BYTE_SWAP16(dest_ptr->pack_high_cell_voltage);
	dest_ptr->pack_low_cell_voltage = BYTE_SWAP16(dest_ptr->pack_low_cell_voltage);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data1_rx_ok = TRUE;

}


/******************************************************************************
 *
 *        Name: rx_orion_dcl_ccl_temp()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_dcl_ccl_temp(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data2_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data2);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data2_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->pack_discharge_current_limit = BYTE_SWAP16(dest_ptr->pack_discharge_current_limit);
	dest_ptr->pack_charge_current_limit = BYTE_SWAP16(dest_ptr->pack_charge_current_limit);

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data2_rx_ok = TRUE;

}

/******************************************************************************
 *
 *        Name: rx_orion_bms_data()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_bms_data(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data3_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data3);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data3_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->pack_total_resiatance = BYTE_SWAP16(dest_ptr->pack_total_resiatance);
	dest_ptr->pack_open_voltage = BYTE_SWAP16(dest_ptr->pack_open_voltage);
	dest_ptr->pack_amp_hours = BYTE_SWAP16(dest_ptr->pack_amp_hours);

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data3_rx_ok = TRUE;

}



/******************************************************************************
 *
 *        Name: rx_orion_faults_with_identifiers
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_faults_with_identifiers(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data5_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data5);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data5_t));

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data5_rx_ok = TRUE;

}

/******************************************************************************
 *
 *        Name: rx_orion_cell_data_1()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_cell_data_1(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data6_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data6);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data6_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->average_cell_voltage = BYTE_SWAP16(dest_ptr->average_cell_voltage);
	dest_ptr->high_cell_resistance = BYTE_SWAP16(dest_ptr->high_cell_resistance);
	dest_ptr->low_cell_resistance = BYTE_SWAP16(dest_ptr->low_cell_resistance);
	dest_ptr->average_cell_resistance = BYTE_SWAP16(dest_ptr->average_cell_resistance);

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data6_rx_ok = TRUE;

}


/******************************************************************************
 *
 *        Name: rx_orion_cell_data_2()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_cell_data_2(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data7_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data7);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data7_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->high_open_cell_voltage = BYTE_SWAP16(dest_ptr->high_open_cell_voltage);
	dest_ptr->low_open_cell_voltage = BYTE_SWAP16(dest_ptr->low_open_cell_voltage);
	dest_ptr->low_power_supply_indicator = BYTE_SWAP16(dest_ptr->low_power_supply_indicator);

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data7_rx_ok = TRUE;

}

/******************************************************************************
 *
 *        Name: rx_orion_pack_isolation_fault()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_pack_isolation_fault(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data8_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data8);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data8_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->pack_isolation_adc = BYTE_SWAP16(dest_ptr->pack_isolation_adc);
	dest_ptr->pack_shortest_wave = BYTE_SWAP16(dest_ptr->pack_shortest_wave);
	dest_ptr->pack_isolation_clipping = BYTE_SWAP16(dest_ptr->pack_isolation_clipping);
	dest_ptr->pack_isolation_threshold = BYTE_SWAP16(dest_ptr->pack_isolation_threshold);

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data8_rx_ok = TRUE;

}

/******************************************************************************
 *
 *        Name: rx_orion_pack_cycle_data()
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */
static void rx_orion_pack_cycle_data(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data9_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->bms_data9);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data9_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->pack_total_cycles = BYTE_SWAP16(dest_ptr->pack_total_cycles);

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->bms_data9_rx_ok = TRUE;


}

static void rx_orion_custom_faults(device_instances_t device,
								  can_data_t *can_data_ptr,
								  int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    bms_pack_data4_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->custom_faults);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(bms_pack_data4_t));

	*receive_counter = 0;
    //
    // Indicate that this message has been received.
    //
    device_data_ptr->custom_faults_rx_ok = TRUE;

}


/******************************************************************************
 *
 *        Name: Set_RX_CAN_TIMEOUT
 *
 * Description: These public functions may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
void orion_bms_set_timeout_orion_bms_inst_data(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_INST_DATA + get_instance_offset(device),
        receive_timeout_counter_limit);
}


void orion_bms_set_timeout_orion_bms_dcl_ccl_temp(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_DCL_CCL_TEMP + get_instance_offset(device),
        receive_timeout_counter_limit);
}


void orion_bms_set_timeout_orion_bms_misc_data(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_MISC_DATA + get_instance_offset(device),
        receive_timeout_counter_limit);
}


void orion_bms_set_timeout_orion_bms_custom_faults(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_CUSTOM_FAULTS + get_instance_offset(device),
        receive_timeout_counter_limit);
}

void orion_bms_set_timeout_orion_bms_faults_with_identifiers(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_FAULTS_WITH_IDENTIFIERS + get_instance_offset(device),
        receive_timeout_counter_limit);
}

void orion_bms_set_timeout_orion_bms_cell_data_1(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_CELL_DATA_1 + get_instance_offset(device),
        receive_timeout_counter_limit);
}

void orion_bms_set_timeout_orion_bms_cell_data_2(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_CELL_DATA_2 + get_instance_offset(device),
        receive_timeout_counter_limit);
}

void orion_bms_set_timeout_orion_bms_pack_iso_fault(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_PACK_ISO_FAULT + get_instance_offset(device),
        receive_timeout_counter_limit);
}

void orion_bms_set_timeout_orion_bms_cycle_data(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
    fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        ORION_BMS_CYCLE_DATA + get_instance_offset(device),
        receive_timeout_counter_limit);
}
