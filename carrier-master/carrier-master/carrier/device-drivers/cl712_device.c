/******************************************************************************
 *
 *        Name: cl712_device.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Wednesday, 32 July 2019
 *
 ******************************************************************************
 */
#include "cl712_device_private.h"
#include "Prototypes.h"
#include "constants.h"

//
// Private member functions
//
// These functions are private in that they are called only from HED's
// User_Can_Receive() via a pointer that is obtained from a call to
// fvt_can_process_rx_message().
//
static void pump_fan_control_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void throttle_calibaration_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void service_timer_rx_timeout(
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
static void cl712_rx_pump_fan_control(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);

static void cl712_rx_throttle_calibaration(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);

static void cl712_rx_service_time_setting(
    device_instances_t device,
    can_data_t *can_data_ptr,
	int16_t *receiver_counter);


typedef enum {
    CL712_RXID_MSG1 = 0x15C43301,
    CL712_RXID_MSG2 = 0x15C43302,
    CL712_RXID_MSG3 = 0x15C43303,
} CL712_RECEIVE_IDS;

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
    CL712_TXID_MSG1 = 0x005C4001,
    CL712_TXID_MSG2 = 0x005C4002,
    CL712_TXID_MSG3 = 0x005C4003,
    CL712_TXID_MSG4 = 0x005C4004,
    CL712_TXID_MSG5 = 0x005C4005,
    CL712_TXID_MSG6 = 0x005C4006,
    CL712_TXID_MSG7 = 0x005C4007,
    CL712_TXID_MSG8 = 0x005C4008,
    CL712_TXID_MSG9 = 0x005C4009,
    CL712_TXID_MSG10 = 0x005C400A,
    CL712_TXID_MSG11 = 0x005C400B,
    CL712_TXID_MSG12 = 0x005C400C,
    CL712_TXID_MSG13 = 0x005C400D,
    CL712_TXID_MSG14 = 0x005C400E,
    CL712_TXID_MSG15 = 0x005C400F,
    CL712_TXID_MSG16 = 0x005C4010,
    CL712_TXID_MSG17 = 0x005C4011,
    CL712_TXID_MSG18 = 0x005C4012,
    CL712_TXID_MSG19 = 0x005C4013,
    CL712_TXID_MSG20 = 0x005C4014,
    CL712_TXID_MSG21 = 0x005C4015,
    CL712_TXID_MSG22 = 0x005C4016,
    CL712_TXID_MSG23 = 0x005C4017,
    CL712_TXID_MSG24 = 0x005C4018,
    CL712_TXID_MSG25 = 0x005C4019,
    CL712_TXID_MSG26 = 0x005C401A,
    CL712_TXID_MSG27 = 0x005C401B,
    CL712_TXID_MSG28 = 0x005C401C,
} CL712_TRANSMIT_ID;

//
// A pointer to the first structure in the linked list compound
// structures that represent all of the Bel received data messages and
// any other constructs, such as timers, required for each Bel charger
// instance.
//
device_data_t *first_cl712_device_data_ptr = NULL;

//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;

static uint32_t get_tx_instance_offset(device_instances_t device)
{
    // Example
    // Calculate the charger instance offset of this instance of the charger.
    //
    // modulo means relationship between charger number and expected source address is:
    //  charger_num     % 4     source_address
    //  0               0       0xD4
    //  1               1       0xD3
    //  2               2       0xD2
    //  3               3       0xD1
    return (uint32_t)(device * 0);

}


/******************************************************************************
 *
 *        Name: cl712_init()
 *
 * Description: Initialize an instance of the bel charger.
 *
 *      Author: Tom & Deepak
 *        Date: Monday, 17 June 2019
 *
 ******************************************************************************
 */
void
cl712_init(
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
        		&first_cl712_device_data_ptr,
        		&device_instance_counter,
                sizeof(device_data_t));

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        CL712_RXID_MSG1,
        cl712_rx_pump_fan_control,
        pump_fan_control_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        CL712_RXID_MSG2,
        cl712_rx_throttle_calibaration,
        throttle_calibaration_rx_timeout);

    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        CL712_RXID_MSG3,
        cl712_rx_service_time_setting,
        service_timer_rx_timeout);

    //
    // Register an intent to transmit the
    // BEL_TX_CHARGER_SETPOINT_J1939 CAN message.
    //
    device_data_ptr->send_tx_data1_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG1,
                                     EXTENDED);

    device_data_ptr->send_tx_data2_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG2,
                                     EXTENDED);

    device_data_ptr->send_tx_data3_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG3,
                                     EXTENDED);

    device_data_ptr->send_tx_data4_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG4,
                                     EXTENDED);

    device_data_ptr->send_tx_data5_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG5,
                                     EXTENDED);

    device_data_ptr->send_tx_data6_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG6,
                                     EXTENDED);

    device_data_ptr->send_tx_data7_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG7,
                                     EXTENDED);

    device_data_ptr->send_tx_data8_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG8,
                                     EXTENDED);

    device_data_ptr->send_tx_data9_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG9,
                                     EXTENDED);

    device_data_ptr->send_tx_data10_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG10,
                                     EXTENDED);

    device_data_ptr->send_tx_data11_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG11,
                                     EXTENDED);

    device_data_ptr->send_tx_data12_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG12,
                                     EXTENDED);

    device_data_ptr->send_tx_data13_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG13,
                                     EXTENDED);

    device_data_ptr->send_tx_data14_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG14,
                                     EXTENDED);

    device_data_ptr->send_tx_data15_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG15,
                                     EXTENDED);

    device_data_ptr->send_tx_data16_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG16,
                                     EXTENDED);

    device_data_ptr->send_tx_data17_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG17,
                                     EXTENDED);

    device_data_ptr->send_tx_data18_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG18,
                                     EXTENDED);

    device_data_ptr->send_tx_data19_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG19,
                                     EXTENDED);

    device_data_ptr->send_tx_data20_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG20,
                                     EXTENDED);

    device_data_ptr->send_tx_data21_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG21,
                                     EXTENDED);

    device_data_ptr->send_tx_data22_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG22,
                                     EXTENDED);

    device_data_ptr->send_tx_data23_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG23,
                                     EXTENDED);

    device_data_ptr->send_tx_data24_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG24,
                                     EXTENDED);

    device_data_ptr->send_tx_data25_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG25,
                                     EXTENDED);

    device_data_ptr->send_tx_data26_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG26,
                                     EXTENDED);

    device_data_ptr->send_tx_data27_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG27,
                                     EXTENDED);

    device_data_ptr->send_tx_data28_can_ptr =
        fvt_can_register_transmit_id(device,
                                     module_id,
                                     can_line,
                                     CL712_TXID_MSG28,
                                     EXTENDED);

}

static void pump_fan_control_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->rx_message_1_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);

}

static void throttle_calibaration_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->rx_message_2_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);

}

static void service_timer_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->rx_message_3_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);

}


/******************************************************************************
 *
 *        Name: cl712_rx_message1()
 *
 *    cl712_rx_message1_t rx_message_1;
 *
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
static void cl712_rx_pump_fan_control(
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
    cl712_rx_message1_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_message_1);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(cl712_rx_message1_t));

	*receive_timeout_counter = NO_TIME_OUT;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->rx_message_1_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: cl712_rx_message2()
 *
 *    cl712_rx_message1_t rx_message_1;
 *
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
static void cl712_rx_throttle_calibaration(
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
    cl712_rx_message2_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_message_2);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(cl712_rx_message2_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->throttle_depressed = BYTE_SWAP16(dest_ptr->throttle_depressed);
    dest_ptr->throttle_released = BYTE_SWAP16(dest_ptr->throttle_released);

    //
    // Write the recieved values to the eeprom variables only if they
    // non zero.
    //
    if(dest_ptr->throttle_depressed != 0)
    {
        //
        // Write eeprom variable and initiate the process of saving
        // the eeprom varaible
        //
        EEVAR_THROTTLE_DEPRESSED = dest_ptr->throttle_depressed;
        OrchestraDIEepromWriteToNVMem(SET_INDIVIDUAL, EE_LOC_EEVAR_THROTTLE_DEPRESSED, WRITE_LEVELS_0_TO_3);
    }

    //
    // Write the recieved values to the eeprom variables only if they
    // non zero.
    //
    if(dest_ptr->throttle_released != 0)
    {
        //
        // Write eeprom variable and initiate the process of saving
        // the eeprom varaible
        //
        EEVAR_THROTTLE_RELEASED = dest_ptr->throttle_released;
        OrchestraDIEepromWriteToNVMem(SET_INDIVIDUAL, EE_LOC_EEVAR_THROTTLE_RELEASED, WRITE_LEVELS_0_TO_3);
    }


	*receive_timeout_counter = NO_TIME_OUT;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->rx_message_2_ok = TRUE;
}

/******************************************************************************
 *
 *        Name: cl712_rx_message3()
 *
 *    cl712_rx_message1_t rx_message_1;
 *
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
static void cl712_rx_service_time_setting(
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
    cl712_rx_message3_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_message_3);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(cl712_rx_message3_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
    dest_ptr->service_time_setting = BYTE_SWAP32(dest_ptr->service_time_setting);
    EEVAR_TIME_UNTIL_SERVICE_VALUE = dest_ptr->service_time_setting;

    dest_ptr->data2 = BYTE_SWAP32(dest_ptr->data2);

	*receive_timeout_counter = NO_TIME_OUT;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->rx_message_3_ok = TRUE;
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
 *      Author: Tom
 *        Date: Friday, 28 June 2019
 *
 ******************************************************************************
 */
void cl712_tx_can_message1(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_main_screen_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data1_can_ptr;
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
    cl712_tx_main_screen_message1_t *can_data_ptr =
        (cl712_tx_main_screen_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message1);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_main_screen_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message2()
//
//=============================================================================
//
void cl712_tx_can_message2(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_main_screen_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data2_can_ptr;
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
    cl712_tx_main_screen_message2_t *can_data_ptr =
        (cl712_tx_main_screen_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message2);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_main_screen_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message3()
//
//=============================================================================
//
void cl712_tx_can_message3(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_main_screen_message3_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data3_can_ptr;
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
    cl712_tx_main_screen_message3_t *can_data_ptr =
        (cl712_tx_main_screen_message3_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message3);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_main_screen_message3_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message4()
//
//=============================================================================
//
void cl712_tx_can_message4(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_main_screen_message4_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data4_can_ptr;
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
    cl712_tx_main_screen_message4_t *can_data_ptr =
        (cl712_tx_main_screen_message4_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message4);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_main_screen_message4_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message5()
//
//=============================================================================
//
void cl712_tx_can_message5(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_bms_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data5_can_ptr;
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
    cl712_tx_bms_message1_t *can_data_ptr =
        (cl712_tx_bms_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message5);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_bms_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message6()
//
//=============================================================================
//
void cl712_tx_can_message6(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_bms_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data6_can_ptr;
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
    cl712_tx_bms_message2_t *can_data_ptr =
        (cl712_tx_bms_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message6);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_bms_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message7()
//
//=============================================================================
//
void cl712_tx_can_message7(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_bms_message3_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data7_can_ptr;
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
    cl712_tx_bms_message3_t *can_data_ptr =
        (cl712_tx_bms_message3_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message7);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_bms_message3_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message8()
//
//=============================================================================
//
void cl712_tx_can_message8(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_bms_message4_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data8_can_ptr;
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
    cl712_tx_bms_message4_t *can_data_ptr =
        (cl712_tx_bms_message4_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message8);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_bms_message4_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message9()
//
//=============================================================================
//
void cl712_tx_can_message9(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_bms_message5_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data9_can_ptr;
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
    cl712_tx_bms_message5_t *can_data_ptr =
        (cl712_tx_bms_message5_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message9);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_bms_message5_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message10()
//
//=============================================================================
//
void cl712_tx_can_message10(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_bms_message6_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data10_can_ptr;
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
    cl712_tx_bms_message6_t *can_data_ptr =
        (cl712_tx_bms_message6_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message10);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_bms_message6_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
//
//
//=============================================================================
//
void cl712_tx_can_message11(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_voltage_status_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data11_can_ptr;
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
    cl712_tx_pdm_voltage_status_message1_t *can_data_ptr =
        (cl712_tx_pdm_voltage_status_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message11);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_voltage_status_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// void cl712_tx_can_message12()
//
//=============================================================================
//
void cl712_tx_can_message12(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_voltage_status_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data12_can_ptr;
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
    cl712_tx_pdm_voltage_status_message2_t *can_data_ptr =
        (cl712_tx_pdm_voltage_status_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message12);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_voltage_status_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message13()
//
//=============================================================================
//
void cl712_tx_can_message13(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_1_status_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data13_can_ptr;
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
    cl712_tx_pdm_1_status_message1_t *can_data_ptr =
        (cl712_tx_pdm_1_status_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message13);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_1_status_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message14()
//
//=============================================================================
//
void cl712_tx_can_message14(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_1_status_and_current_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data14_can_ptr;
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
    cl712_tx_pdm_1_status_and_current_message2_t *can_data_ptr =
        (cl712_tx_pdm_1_status_and_current_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message14);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_1_status_and_current_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message15()
//
//=============================================================================
//
void cl712_tx_can_message15(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_1_current_message3_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data15_can_ptr;
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
    cl712_tx_pdm_1_current_message3_t *can_data_ptr =
        (cl712_tx_pdm_1_current_message3_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message15);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_1_current_message3_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message16()
//
//=============================================================================
//
void cl712_tx_can_message16(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_2_status_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data16_can_ptr;
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
    cl712_tx_pdm_2_status_message1_t *can_data_ptr =
        (cl712_tx_pdm_2_status_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message16);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_2_status_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message17()
//
//=============================================================================
//
void cl712_tx_can_message17(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_2_status_and_current_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data17_can_ptr;
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
    cl712_tx_pdm_2_status_and_current_message2_t *can_data_ptr =
        (cl712_tx_pdm_2_status_and_current_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message17);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_2_status_and_current_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message18()
//
//=============================================================================
//
void cl712_tx_can_message18(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_2_current_message3_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data18_can_ptr;
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
    cl712_tx_pdm_2_current_message3_t *can_data_ptr =
        (cl712_tx_pdm_2_current_message3_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message18);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_2_current_message3_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message19()
//
//=============================================================================
//
void cl712_tx_can_message19(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_3_status_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data19_can_ptr;
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
    cl712_tx_pdm_3_status_message1_t *can_data_ptr =
        (cl712_tx_pdm_3_status_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message19);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_3_status_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message20()
//
//=============================================================================
//
void cl712_tx_can_message20(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_3_status_and_current_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data20_can_ptr;
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
    cl712_tx_pdm_3_status_and_current_message2_t *can_data_ptr =
        (cl712_tx_pdm_3_status_and_current_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message20);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_3_status_and_current_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message21()
//
//=============================================================================
//
void cl712_tx_can_message21(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pdm_3_current_message3_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data21_can_ptr;
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
    cl712_tx_pdm_3_current_message3_t *can_data_ptr =
        (cl712_tx_pdm_3_current_message3_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message21);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pdm_3_current_message3_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message22()
//
//=============================================================================
//
void cl712_tx_can_message22(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_traction_drive_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data22_can_ptr;
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
    cl712_tx_traction_drive_message1_t *can_data_ptr =
        (cl712_tx_traction_drive_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message22);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_traction_drive_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message23()
//
//=============================================================================
//
void cl712_tx_can_message23(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_traction_drive_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data23_can_ptr;
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
    cl712_tx_traction_drive_message2_t *can_data_ptr =
        (cl712_tx_traction_drive_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message23);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_traction_drive_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message24()
//
//=============================================================================
//
void cl712_tx_can_message24(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_hydraulic_drive_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data24_can_ptr;
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
    cl712_tx_hydraulic_drive_message1_t *can_data_ptr =
        (cl712_tx_hydraulic_drive_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message24);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_hydraulic_drive_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message25()
//
//=============================================================================
//
void cl712_tx_can_message25(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_system_pressure_message1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data25_can_ptr;
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
    cl712_tx_system_pressure_message1_t *can_data_ptr =
        (cl712_tx_system_pressure_message1_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message25);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_system_pressure_message1_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message26()
//
//=============================================================================
//
void cl712_tx_can_message26(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_system_pressure_message2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data26_can_ptr;
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
    cl712_tx_system_pressure_message2_t *can_data_ptr =
        (cl712_tx_system_pressure_message2_t *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message26);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_system_pressure_message2_t));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


//=============================================================================
//
// cl712_tx_can_message27()
//
//=============================================================================
//
void cl712_tx_can_message27(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pump_fan_status *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data27_can_ptr;
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
    cl712_tx_pump_fan_status *can_data_ptr =
        (cl712_tx_pump_fan_status *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message27);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pump_fan_status));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

//=============================================================================
//
// cl712_tx_can_message28()
//
//=============================================================================
//
void cl712_tx_can_message28(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    cl712_tx_pump_fan_override_status *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
    can_tx_registration_t *tx_ptr =
        device_data_ptr->send_tx_data28_can_ptr;
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
    cl712_tx_pump_fan_override_status *can_data_ptr =
        (cl712_tx_pump_fan_override_status *)tx_ptr->tx_message.data;
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    // Send the CAN message if time to do so.
    if (++(tx_ptr->transmit_counter) >= tx_ptr->transmit_counter_limit)
    {
        tx_ptr->transmit_counter = 0;

        //
        // Have the pointer to this device's data record, not get a
        // pointer to the appropriate receive data structure.
        //
        source_ptr = &(device_data_ptr->tx_message28);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(cl712_tx_pump_fan_override_status));

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}
