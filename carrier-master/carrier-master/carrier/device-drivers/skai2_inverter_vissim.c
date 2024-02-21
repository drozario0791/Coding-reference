/******************************************************************************
 *
 *        Name: skai2_inverter_visim.c
 *
 * Description: This source file consists of the device driver for the
 *              skai2 inverter. The following actions are done by the
 *              driver:
 *
 *              i) initialztion of skai inverters : The initialize
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
 *
 *      Author: Deepak
 *        Date: Monday, 25 July 2019
 *
 ******************************************************************************
 */
#include "skai2_inverter_private_vissim.h"

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
static void skai2_vissim_msg1_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void skai2_vissim_msg2_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void skai2_vissim_msg3_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);


static void skai2_vissim_msg4_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void skai2_vissim_msg5_rx_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint32_t can_id,
    uint8_t j1939_byte);

static void skai2_vissim_msg9_rx_timeout(
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
static void rx_skai2_vissim_msg1(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

static void rx_skai2_vissim_msg2(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

static void rx_skai2_vissim_msg3(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

static void rx_skai2_vissim_msg4(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

static void rx_skai2_vissim_msg5(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);

static void rx_skai2_vissim_msg9(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter);


//
// A pointer to the first structure in the linked list compound
// structures that represent all of the skai inverter received data
// messages and any other constructs, such as timers, required for
// each Skai inverter instance.
//
device_data_t *first_skai2_vissim_device_data_ptr = NULL;

//
// This is incremented each for each device instance
//
static uint8_t device_instance_counter = 0;

// TX and RX CAN ID's
typedef enum {
	SKAI2_RXID_MSG1 = 0x00101001,
	SKAI2_RXID_MSG2 = 0x00101002,
	SKAI2_RXID_MSG3 = 0x00101003,
	SKAI2_RXID_MSG4 = 0x00101004,
	SKAI2_RXID_MSG5 = 0x00101005,
	SKAI2_RXID_MSG9 = 0x00101009,
} SKAI2_RECEIVE_IDS;

typedef enum {
    SKAI2_INVERTER_TX_D_GAINS = 0x00111001,
    SKAI2_INVERTER_TX_Q_GAINS = 0x00111002,
	SKAI2_INVERTER_TX_SPD_GAINS = 0x00111003,
	SKAI2_INVERTER_TX_ELECTRICAL_ANGLE = 0x00111004,
	SKAI2_INVERTER_TX_ENABLE_MAXCURRENT_SOC_HIGH_CELL = 0x00111005,
	SKAI2_INVERTER_TX_MOTOR_SCALE = 0x00111006,
	SKAI2_INVERTER_TX_REGEN_SCALE = 0x00111007,
	SKAI2_INVERTER_TX_THROTTLE_FILTER_DQF_GAINS = 0x00111008,
	SKAI2_INVERTER_TX_BAT_CURRENT_MODE_DIRECTION_HYDTHROTTLE = 0x00111009,
	SKAI2_INVERTER_TX_PEDAL_CALC_MOTOR_SCALE_BAT_I_GAIN = 0x00111010,
	SKAI2_INVERTER_TX_THROTTLE_OVER_RIDE_SCALE = 0x00111011,
	SKAI2_INVERTER_TX_RPM_OUTGAIN_RAMPTIME = 0x00111012,
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

	uint32_t offset = (uint32_t)(device * 0x100000);

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
init_skai2_inverter_vissim(
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
        		&first_skai2_vissim_device_data_ptr,
        		&device_instance_counter,
                sizeof(device_data_t));

    uint32_t foo = (uint32_t)get_instance_offset(device);

    //
    // Register an interest to receive the following CAN messages from
    // the skai2 device instance.
    //

	// Error Feedback1, Error Feedback2, Motor Temp, DC Link Voltage
    fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_MSG1 + get_instance_offset(device),
        rx_skai2_vissim_msg1,
        skai2_vissim_msg1_rx_timeout);

	// Aux input voltage, throttle (0 - 100), version number, direction
	fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_MSG2 + get_instance_offset(device),
        rx_skai2_vissim_msg2,
        skai2_vissim_msg2_rx_timeout);

	// Throttle Pedal ADC, IO status, Electricalangle (0 - 1024), pcb temp
	fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_MSG3 + get_instance_offset(device),
        rx_skai2_vissim_msg3,
        skai2_vissim_msg3_rx_timeout);

	// Torque, RPM, Encoder Feedback,
	fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_MSG4 + get_instance_offset(device),
        rx_skai2_vissim_msg4,
        skai2_vissim_msg4_rx_timeout);

	// Temperatures
	fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_MSG5 + get_instance_offset(device),
        rx_skai2_vissim_msg5,
        skai2_vissim_msg5_rx_timeout);

	// Power Consumed, Max Temperature, Inverter Enable
	fvt_can_register_receive_id(
        device,
        module_id,
        can_line,
        SKAI2_RXID_MSG9 + get_instance_offset(device),
        rx_skai2_vissim_msg9,
        skai2_vissim_msg9_rx_timeout);

    //
    // Register an intent to transmit the following CAN messages to
    // the skai2 device instance.
    //
    device_data_ptr->send_inverter_d_gains_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_D_GAINS + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_q_gains_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_Q_GAINS + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_spd_gains_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_SPD_GAINS + get_instance_offset(device),
            EXTENDED);

	device_data_ptr->send_inverter_electrical_angle_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_ELECTRICAL_ANGLE + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_enable_maxCurrent_soc_highCell_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_ENABLE_MAXCURRENT_SOC_HIGH_CELL + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_motor_scale_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_MOTOR_SCALE + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_regen_scale_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_REGEN_SCALE + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_throttle_filter_dq_df_gains_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_THROTTLE_FILTER_DQF_GAINS + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_battery_current_mode_direction_hyd_throttle_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_BAT_CURRENT_MODE_DIRECTION_HYDTHROTTLE +
            get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_pedal_calibration_battery_gain_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_PEDAL_CALC_MOTOR_SCALE_BAT_I_GAIN + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_throttle_override_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_THROTTLE_OVER_RIDE_SCALE + get_instance_offset(device),
            EXTENDED);

    device_data_ptr->send_inverter_rpm_outgain_ramptime_ptr =
        fvt_can_register_transmit_id(
            device,
            module_id,
            CAN3,
            SKAI2_INVERTER_TX_RPM_OUTGAIN_RAMPTIME + get_instance_offset(device),
            EXTENDED);


    //
    //
    //
#warning "Deepak, please explain what this is for."
    device_data_ptr->inverter_power_pin = inverter_power_pin;
}

/******************************************************************************
 *
 *        Name: skai2_vissim_msg1_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 ******************************************************************************
 */
static void skai2_vissim_msg1_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_vissim_msg1_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: skai2_vissim_msg2_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 ******************************************************************************
 */
static void skai2_vissim_msg2_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_vissim_msg2_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: skai2_vissim_msg3_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 ******************************************************************************
 */
static void skai2_vissim_msg3_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_vissim_msg3_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}


/******************************************************************************
 *
 *        Name: skai2_vissim_msg4_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 ******************************************************************************
 */
static void skai2_vissim_msg4_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_vissim_msg4_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}

/******************************************************************************
 *
 *        Name: skai2_vissim_msg5_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 ******************************************************************************
 */
static void skai2_vissim_msg5_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_vissim_msg5_rx_ok = FALSE;

    //
    // Call this function in can_service.c which is used as a common
    // location where a breakpoint may be placed to catch all receive
    // CAN message timeouts.
    //
    rx_message_timeout(device, module_id, can_line, can_id, j1939_byte);
}

/******************************************************************************
 *
 *        Name: skai2_vissim_msg9_rx_timeout()
 *
 * Description: This function, registered with can_service and will be
 *              called when the received CAN message assocoated with
 *              this function, by registration, times out due to not
 *              having been received with the the timeout limit, if
 *              one has been set.
 *
 ******************************************************************************
 */
static void skai2_vissim_msg9_rx_timeout(
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
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Mark this message as having timed out.
    //
    device_data_ptr->skai2_vissim_msg9_rx_ok = FALSE;

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
 *        Name: rx_skai2_vissim_msg1
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 25 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_vissim_msg1(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter)
{
    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_rx_msg1_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_msg1_data);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_rx_msg1_t));

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
    device_data_ptr->skai2_vissim_msg1_rx_ok = TRUE;

}

/******************************************************************************
 *
 *        Name: rx_skai2_vissim_msg2
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 25 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_vissim_msg2(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter)
{
	    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_rx_msg2_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_msg2_data);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr, sizeof(skai2_inverter_rx_msg2_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->auxilary_input_voltage = BYTE_SWAP16(dest_ptr->auxilary_input_voltage);
	dest_ptr->throttle = BYTE_SWAP16(dest_ptr->throttle);
	dest_ptr->version_number = BYTE_SWAP16(dest_ptr->version_number);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_vissim_msg2_rx_ok = TRUE;

}

/******************************************************************************
 *
 *        Name: rx_skai2_vissim_msg3
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 25 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_vissim_msg3(device_instances_t device,
							can_data_t *can_data_ptr,
								 int16_t *receive_counter)
{
	    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_rx_msg3_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_msg3_data);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_rx_msg3_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->encoder_feedback = BYTE_SWAP16(dest_ptr->encoder_feedback);
	dest_ptr->motor_rpm = BYTE_SWAP16(dest_ptr->motor_rpm);
	dest_ptr->torque = BYTE_SWAP16(dest_ptr->torque);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_vissim_msg3_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_skai2_vissim_msg4
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 25 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_vissim_msg4(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter)
{
	    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_rx_msg4_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_msg4_data);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_rx_msg4_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->IO_status = BYTE_SWAP16(dest_ptr->IO_status);
	dest_ptr->electrical_angle = BYTE_SWAP16(dest_ptr->electrical_angle);
	dest_ptr->throttle_pedal_adc = BYTE_SWAP16(dest_ptr->throttle_pedal_adc);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_vissim_msg4_rx_ok = TRUE;
}

/******************************************************************************
 *
 *        Name: rx_skai2_vissim_msg5
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 25 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_vissim_msg5(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter)
{
	    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_rx_msg5_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_msg5_data);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_rx_msg5_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->dcb_phase1_temperature = BYTE_SWAP16(dest_ptr->dcb_phase1_temperature);
	dest_ptr->dcb_phase2_temperature = BYTE_SWAP16(dest_ptr->dcb_phase2_temperature);
	dest_ptr->dcb_phase3_temperature = BYTE_SWAP16(dest_ptr->dcb_phase3_temperature);
	dest_ptr->pcb_temperature = BYTE_SWAP16(dest_ptr->pcb_temperature);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_vissim_msg5_rx_ok = TRUE;
}


/******************************************************************************
 *
 *        Name: rx_skai2_vissim_msg9
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 25 July 2019
 *
 ******************************************************************************
 */
static void rx_skai2_vissim_msg9(device_instances_t device,
							can_data_t *can_data_ptr,
							int16_t *receive_counter)
{
	    // Check can_data_ptr is not NULL.
    if (can_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // This will be a pointer to the device's receive data structure
    // that corresponds to this function.
    //
    skai2_inverter_rx_msg9_t *dest_ptr = NULL;

    //
    // Get a pointer to this device's data record.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, not get a
    // pointer to the appropriate receive data structure.
    //
    dest_ptr = &(device_data_ptr->rx_msg9_data);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Copy the data from the received CAN message to the designated
    // receive message structure in this device's data structure.
    //
    memcpy(dest_ptr, can_data_ptr,
           sizeof(skai2_inverter_rx_msg9_t));

    //
    // All 16-bit values need to be byte swapped as this is a big
    // endian uP and CAN is little endian.
    //
	dest_ptr->inverter_enable = BYTE_SWAP16(dest_ptr->inverter_enable);
	dest_ptr->max_temperature = BYTE_SWAP16(dest_ptr->max_temperature);
	dest_ptr->power_consumned = BYTE_SWAP16(dest_ptr->power_consumned);

	*receive_counter = 0;

    //
    // Indicate that this message has been received.
    //
    device_data_ptr->skai2_vissim_msg9_rx_ok = TRUE;
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

/******************************************************************************
 *
 *        Name: tx_skai2_vissim_d_gains()
 *
 * Description: Transmits the gains required for the direct axis
 *              controller.
 *
 *              canID: 0x00101001
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_d_gains(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{

    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg1_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_d_gains_ptr;

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
    skai2_inverter_tx_msg1_t *can_data_ptr =
        (skai2_inverter_tx_msg1_t *)tx_ptr->tx_message.data;
    
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
        source_ptr = &(device_data_ptr->tx_msg1_data_d_gains);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg1_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
		can_data_ptr->d_proportional_gain = BYTE_SWAP16(source_ptr->d_proportional_gain);
		can_data_ptr->d_integral_gain = BYTE_SWAP16(source_ptr->d_integral_gain);
		can_data_ptr->d_differential_gain = BYTE_SWAP16(source_ptr->d_differential_gain);
		can_data_ptr->d_time_constant = BYTE_SWAP16(source_ptr->d_time_constant);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}


/******************************************************************************
 *
 *        Name: tx_skai2_vissim_q_gains()
 *
 * Description: Transmits the gains required for the quadrature axis
 *              PID controller.
 *
 *              canID: 0x00101002
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_q_gains(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg2_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_q_gains_ptr;

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
    skai2_inverter_tx_msg2_t *can_data_ptr =
        (skai2_inverter_tx_msg2_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg2_data_q_gains);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg2_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
		can_data_ptr->q_proportional_gain = BYTE_SWAP16(source_ptr->q_proportional_gain);
		can_data_ptr->q_integral_gain = BYTE_SWAP16(source_ptr->q_integral_gain);
		can_data_ptr->q_differential_gain = BYTE_SWAP16(source_ptr->q_differential_gain);
		can_data_ptr->q_time_constant = BYTE_SWAP16(source_ptr->q_time_constant);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}


/******************************************************************************
 *
 *        Name: tx_skai2_vissim_speed_gains()
 *
 * Description: Transmits the gains required for the speed PID
 *              controller.
 *
 *              canID: 0x00101003
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_speed_gains(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg3_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_spd_gains_ptr;

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
    skai2_inverter_tx_msg3_t *can_data_ptr =
        (skai2_inverter_tx_msg3_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg3_data_speed_gains);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg3_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
		can_data_ptr->speed_proportional_gain = BYTE_SWAP16(source_ptr->speed_proportional_gain);
		can_data_ptr->speed_integral_gain = BYTE_SWAP16(source_ptr->speed_integral_gain);
		can_data_ptr->speed_differential_gain = BYTE_SWAP16(source_ptr->speed_differential_gain);
		can_data_ptr->speed_time_constant = BYTE_SWAP16(source_ptr->speed_time_constant);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}

/******************************************************************************
 *
 *        Name: tx_skai2_vissim_electrical_angle()
 *
 * Description: Transmits the electrical angle for the motor.
 *
 *              canID: 0x00101004
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_electrical_angle(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg4_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_electrical_angle_ptr;

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
    skai2_inverter_tx_msg4_t *can_data_ptr =
        (skai2_inverter_tx_msg4_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg4_data_electrical_angle);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg4_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
		can_data_ptr->electrical_angle_forward = BYTE_SWAP16(source_ptr->electrical_angle_forward);
		can_data_ptr->electrical_angle_reverse = BYTE_SWAP16(source_ptr->electrical_angle_reverse);
		can_data_ptr->electrical_angle_forward_regen = BYTE_SWAP16(source_ptr->electrical_angle_forward_regen);
		can_data_ptr->electrical_angle_reverse_regen = BYTE_SWAP16(source_ptr->electrical_angle_reverse_regen);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}

/******************************************************************************
 *
 *        Name: tx_skai2_enable_maxcurrent_soc_highcell()
 *
 * Description: Transmits the following parameters
 *              1) Inverter Enable
 *              2) Max Current limit
 *              3) State of Charge(SOC)
 *              4) High Cell Voltage
 *
 *              canID: 0x00101005
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_enable_maxcurrent_soc_highcell(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg5_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_enable_maxCurrent_soc_highCell_ptr;

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
    skai2_inverter_tx_msg5_t *can_data_ptr =
        (skai2_inverter_tx_msg5_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg5_enable_max_current_soc_high_cell);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg5_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
		can_data_ptr->inverter_enable = BYTE_SWAP16(source_ptr->inverter_enable);
		can_data_ptr->max_battery_current = BYTE_SWAP16(source_ptr->max_battery_current);
		can_data_ptr->pack_state_of_charge = BYTE_SWAP16(source_ptr->pack_state_of_charge);
		can_data_ptr->high_cell_voltage = BYTE_SWAP16(source_ptr->high_cell_voltage);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}


/******************************************************************************
 *
 *        Name: tx_skai2_vissim_motor_scale()
 *
 * Description: Transmits the parameters that determine the
 *              motorscale. The motor scale limits the motor torque in
 *              motoring operation.
 *
 *              canID: 0x00101006
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_motor_scale(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg6_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_motor_scale_ptr;

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
    skai2_inverter_tx_msg6_t *can_data_ptr =
        (skai2_inverter_tx_msg6_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg6_motor_scale);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg6_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
		can_data_ptr->motor_scale_P1 = BYTE_SWAP16(source_ptr->motor_scale_P1);
		can_data_ptr->motor_scale_P2 = BYTE_SWAP16(source_ptr->motor_scale_P2);
		can_data_ptr->motor_scale_L1 = BYTE_SWAP16(source_ptr->motor_scale_L1);
		can_data_ptr->motor_scale_L2 = BYTE_SWAP16(source_ptr->motor_scale_L2);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}

/******************************************************************************
 *
 *        Name: tx_skai2_vissim_regen_scale()
 *
 * Description: Transmits the parameters that determine the
 *              regenscale. The regen scale limits the motor torque in
 *              regenration operation.
 *
 *              canID: 0x00101007
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_regen_scale(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg7_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_regen_scale_ptr;

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
    skai2_inverter_tx_msg7_t *can_data_ptr =
        (skai2_inverter_tx_msg7_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg7_regen_scale);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg7_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
		can_data_ptr->regen_scale_P1 = BYTE_SWAP16(source_ptr->regen_scale_P1);
		can_data_ptr->regen_scale_P2 = BYTE_SWAP16(source_ptr->regen_scale_P2);
		can_data_ptr->regen_scale_L1 = BYTE_SWAP16(source_ptr->regen_scale_L1);
		can_data_ptr->regen_scale_L2 = BYTE_SWAP16(source_ptr->regen_scale_L2);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

/******************************************************************************
 *
 *        Name: tx_skai2_vissim_throttle_filter_dq_df_gains
 *
 * Description: The message transmits the following parameters
 *              1) Throttle Filter gain 1
 *              2) Throttle Filter gain2
 *              3) DF gain
 *              4) QF gain
 *
 *              canID: 0x00101008
 *
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_throttle_filter_dq_df_gains(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg8_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_throttle_filter_dq_df_gains_ptr;

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
    skai2_inverter_tx_msg8_t *can_data_ptr =
        (skai2_inverter_tx_msg8_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg8_throttle_filter_dqf_gains);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg8_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
		can_data_ptr->throttle_gain_1 = BYTE_SWAP16(source_ptr->throttle_gain_1);
		can_data_ptr->throttle_gain_2 = BYTE_SWAP16(source_ptr->throttle_gain_2);
		can_data_ptr->df_gain = BYTE_SWAP16(source_ptr->df_gain);
		can_data_ptr->qf_gain = BYTE_SWAP16(source_ptr->qf_gain);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}

/******************************************************************************
 *
 *        Name: tx_skai2_battery_current_mode_direction_hyd_throttle()
 *
 * Description: The message transmits the following parameters
 *              1) Instantaneous Battey Current
 *              2) Mode Switch
 *              3) Motor direction
 *              4) Hydraulic Throttle
 *
 *              canID: 0x00101009
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_battery_current_mode_direction_hyd_throttle(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg9_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_battery_current_mode_direction_hyd_throttle_ptr;

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
    skai2_inverter_tx_msg9_t *can_data_ptr =
        (skai2_inverter_tx_msg9_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg9_battery_current_mode_direction_hyd_throttle);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg9_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
		can_data_ptr->battery_current = BYTE_SWAP16(source_ptr->battery_current);
		can_data_ptr->mode_switch = BYTE_SWAP16(source_ptr->mode_switch);
		can_data_ptr->direction = BYTE_SWAP16(source_ptr->direction);
		can_data_ptr->hydraulic_throttle = BYTE_SWAP16(source_ptr->hydraulic_throttle);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}

/******************************************************************************
 *
 *        Name: tx_skai2_vissim_pedal_calibration_battery_gain()
 *
 * Description: The message transmits the following parameters
 *              1) Throttle depressed
 *              2) Throttle released
 *              3) Hydraulic motor scale settings
 *              4) Battery current integral gain
 *
 *              canID: 0x00101010
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_pedal_calibration_battery_gain(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg10_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_pedal_calibration_battery_gain_ptr;

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
    skai2_inverter_tx_msg10_t *can_data_ptr =
        (skai2_inverter_tx_msg10_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg10_pedal_calibration_motor_scale);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg10_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
		can_data_ptr->throttle_depressed = BYTE_SWAP16(source_ptr->throttle_depressed);
		can_data_ptr->throttle_released = BYTE_SWAP16(source_ptr->throttle_released);
		can_data_ptr->hydraulic_motor_scale_settings = BYTE_SWAP16(source_ptr->hydraulic_motor_scale_settings);
		can_data_ptr->battery_current_integral_gain = BYTE_SWAP16(source_ptr->battery_current_integral_gain);
        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

/******************************************************************************
 *
 *        Name: tx_skai2_vissim_throttle_override()
 *
 * Description: The message transmits the following parameters
 *              1) Unused1
 *              2) Unused2
 *              3) Throttle override
 *              4) Throttle scale
 *
 *              canID: 0x00101011
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_throttle_override(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg11_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_throttle_override_ptr;

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
    skai2_inverter_tx_msg11_t *can_data_ptr =
        (skai2_inverter_tx_msg11_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg11_throttle_override);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg11_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
		can_data_ptr->unused1 = BYTE_SWAP16(source_ptr->unused1);
		can_data_ptr->unused2 = BYTE_SWAP16(source_ptr->unused2);
		can_data_ptr->throttle_override = BYTE_SWAP16(source_ptr->throttle_override);
		can_data_ptr->throttle_scale = BYTE_SWAP16(source_ptr->throttle_scale);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }
}

/******************************************************************************
 *
 *        Name: tx_skai2_vissim_rpm_settings_outgain_pgainramptime
 *
 * Description: The message transmits the following parameters
 *              1) Idle Motor rpm
 *              2) Max motor rpm
 *              3) Out gain
 *              4) Speed proportional ramp timer
 *
 *              canID: 0x00101012
 *
 *        Date: Friday, 26 July 2019
 *
 ******************************************************************************
 */
void tx_skai2_vissim_rpm_settings_outgain_pgainramptime(
    device_instances_t device,
    can_rate_t transmit_counter_limit)
{
    //
    // This will be a pointer to the device's send data structure
    // that corresponds to this function.
    //
    skai2_inverter_tx_msg12_t *source_ptr = NULL;

    //
    // Get a pointer to this device instance's device_data_t
    // structure. This structure has two objects required for the
    // operation of sending the CAN message associated this this
    // function.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_skai2_vissim_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Get a pointer to the can tx registration record that contains
    // the information required to transmit its data.
    //
	can_tx_registration_t *tx_ptr =
        device_data_ptr->send_inverter_rpm_outgain_ramptime_ptr;

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
    skai2_inverter_tx_msg12_t *can_data_ptr =
        (skai2_inverter_tx_msg12_t *)tx_ptr->tx_message.data;
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
        source_ptr = &(device_data_ptr->tx_msg12_rpm_outgain_ramptime);
        if (source_ptr == NULL) {DEBUG("NULL Pointer");}

        //
        // Copy data from our local structure to the CAN transmit
        // buffer.
        //
        memcpy(can_data_ptr, source_ptr,
               sizeof(skai2_inverter_tx_msg12_t));

        //
        // All 16-bit values need to be byte swapped as this is a
        // big endian uP and CAN is little endian.
        //
		can_data_ptr->idle_motor_rpm = BYTE_SWAP16(source_ptr->idle_motor_rpm);
		can_data_ptr->max_motor_rpm = BYTE_SWAP16(source_ptr->max_motor_rpm);
		can_data_ptr->out_gain = BYTE_SWAP16(source_ptr->out_gain);
		can_data_ptr->speed_p_gain_ramp_timer = BYTE_SWAP16(source_ptr->speed_p_gain_ramp_timer);

        Send_CAN_Message(tx_ptr->module_id,
                         tx_ptr->can_line,
                         tx_ptr->tx_message);
    }

}

/******************************************************************************
 *
 *        Name: Set_RX_CAN_TIMEOUT
 *
 * Description:
 *
 *        Date: Thursday, 18 July 2019
 *
 ******************************************************************************
 */
/******************************************************************************
 *
 *        Name: skai2_vissim_set_timeout_skai2_rxid_msg1()
 *
 * Description: This public function may be called, after the
 *              registration of recieve CAN messages in the device's
 *              init function, to set this message's receive CAN
 *              timeout limit. By default, registered receive CAN
 *              messages do not timeout.
 *
 ******************************************************************************
 */
void skai2_vissim_set_timeout_skai2_rxid_msg1(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_MSG1 + get_instance_offset(device),
        receive_timeout_counter_limit);
}

/******************************************************************************
 *
 *        Name: skai2_vissim_set_timeout_skai2_rxid_msg2()
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
void skai2_vissim_set_timeout_skai2_rxid_msg2(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_MSG2 + get_instance_offset(device),
        receive_timeout_counter_limit);
}

/******************************************************************************
 *
 *        Name: skai2_vissim_set_timeout_skai2_rxid_msg3()
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
void skai2_vissim_set_timeout_skai2_rxid_msg3(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_MSG3 + get_instance_offset(device),
        receive_timeout_counter_limit);
}

/******************************************************************************
 *
 *        Name: skai2_vissim_set_timeout_skai2_rxid_msg4()
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
void skai2_vissim_set_timeout_skai2_rxid_msg4(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_MSG4 + get_instance_offset(device),
        receive_timeout_counter_limit);
}

/******************************************************************************
 *
 *        Name: skai2_vissim_set_timeout_skai2_rxid_msg5()
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
void skai2_vissim_set_timeout_skai2_rxid_msg5(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_MSG5 + get_instance_offset(device),
        receive_timeout_counter_limit);
}

/******************************************************************************
 *
 *        Name: skai2_vissim_set_timeout_skai2_rxid_msg9()
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
void skai2_vissim_set_timeout_skai2_rxid_msg9(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit)
{
	fvt_can_set_timeout_receive_id(
        module_id,
        can_line,
        SKAI2_RXID_MSG9 + get_instance_offset(device),
        receive_timeout_counter_limit);
}
