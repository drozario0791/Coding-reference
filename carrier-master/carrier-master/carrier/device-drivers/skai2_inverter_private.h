/******************************************************************************
 *
 *        Name: skai2_inverter_private.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */

#ifndef  SKAI2_INVERTER_PRIVATE_H_
#define  SKAI2_INVERTER_PRIVATE_H_

#include <string.h>
#include "can_service_devices.h"


//////////////////////////////////////////////////////////////////////////////
//
// NOTE: There is a required #include file and two function prototypes
// at the end of this head file. This is required for all device
// drivers.
//
//////////////////////////////////////////////////////////////////////////////

//
// FVT Motor Control RTOS RX Messages
//
// Reference: fvt_inverter_control_skai/eeprom.c & processCanRXData
//
// Name: Inverter_Status
// CAN ID: 0x00101X00:
//
typedef struct {

    uint32_t error_feedback_1 : 16;
    uint32_t error_feedback_2 : 16;
	uint32_t MotorTemp_C : 16;
	uint32_t DCLink_Voltage : 16;
} skai2_inverter_status_t;

//
// Name: Inverter_Temperature
// CAN ID: 0x00101X01:
//
typedef struct {
	uint32_t temperature_dcb_phase_L1 : 16;
	uint32_t temperature_dcb_phase_L2 : 16;
	uint32_t temperature_dcb_phase_L3 : 16;
	uint32_t temperature_pcb : 16;
} skai2_inverter_temperature_t;


//
// Name: Electrical Angle and RPM
// CAN ID: 0x00101X05:
//
typedef struct {
	uint32_t electrical_offset_ccw : 16;
	uint32_t max_temperature : 16;
	uint32_t motor_rpm : 16;
	uint32_t electrical_offset_cw : 16;
} skai2_inverter_eleAngle_RPM_t;

//
// Name: Torque
// CAN ID: 0x00101X07:
//
typedef struct
{
	uint32_t unused_1 : 16;
	uint32_t unused_2 : 16;
	uint32_t motor_torque : 16;
	uint32_t unused_3 : 16;
} skai2_inverter_torque_t;


//
// FVT Motor Control RTOS TX Messages
//
// Name: Control
// Reference: fvt_inverter_control_skai/eeprom.c & processCanRXData
// CAN ID: 0x00111X00
//
typedef struct {
	uint32_t motor_switch_enable : 8;
	uint32_t commanded_motor_direction : 8;
	uint32_t diagnostic_mode : 8;
	uint32_t backdriving_mode : 8;
	uint32_t commanded_rpm : 16;
	uint32_t commanded_torque : 16;
} skai2_inverter_tx_control_t;


//
// Name: Inverter Limits
// CAN ID: 0x00111X00
//
typedef struct {
	uint32_t max_battery_current : 16;
	uint32_t instantaneous_battery_current : 16;
	uint32_t high_cell_voltage : 16;
	uint32_t mode : 16;
} skai2_inverter_tx_limits_t;


//
// A compound structure that encompasses all the RX CAN messages
// related to the FVT Skai. One of these is allocated for each
// instantiation of a Skai inverter.
//
typedef struct device_data_s {
	struct device_data_s *next_ptr;
	uint8_t device_instance_number;

	// These are receive CAN message data structures.
	skai2_inverter_status_t status;
	skai2_inverter_temperature_t temperature;
	skai2_inverter_eleAngle_RPM_t ElectricalAngle_RPM;
	skai2_inverter_torque_t motor_torque;

    // Each of the PDM CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    bool_t status_rx_ok;
    bool_t temperature_rx_ok;
    bool_t ElectricalAngle_RPM_rx_ok;
    bool_t motor_torque_rx_ok;

    // These are the transmit CAN message data structures.
	skai2_inverter_tx_control_t control;
	skai2_inverter_tx_limits_t limits;

    // These are pointers to CAN tx registration records.
	can_tx_registration_t *send_inverter_control_ptr;
	can_tx_registration_t *send_inverter_limits_ptr;

    //
    // Each of the BMS CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    //
    bool_t skai2_status_rx_ok;
    bool_t skai2_temperature_rx_ok;
    bool_t skai2_EA_RPM_rx_ok;
    bool_t skai2_torque_rx_ok;

	// power pin ->power pin
	uint8_t inverter_power_pin;
} device_data_t;

//
// Not required as of now.
//
inline bool_t getBit(uint32_t data, uint32_t bit_number)
{
	bool_t bit_status= (bool_t)(data >> bit_number) & 1;

	return bit_status;
}


#include "Prototypes_Time.h"
#include "skai2_inverter.h"

//////////////////////////////////////////////////////////////////////////////
//
// Note: These two function prototypes of functions defined in
// can_service.c are required here at this point in each device's
// private header file.
//
//////////////////////////////////////////////////////////////////////////////
//
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

#endif // SKAI2_INVERTER_PRIVATE_H_
