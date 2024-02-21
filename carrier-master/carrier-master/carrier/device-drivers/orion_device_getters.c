/******************************************************************************
 *
 *        Name: orion_device_getters.c
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 22 July 2019
 *
 ******************************************************************************
 */


#include "orion_device_private.h"
#include "orion_device.h"

extern device_data_t *first_orion_device_data_ptr;

//=============================================================================
//
// CAN ID: 0x00N5001
//
//=============================================================================
//

//=============================================================================
//
// orion_get_instantaneous_pack_current()
//
//=============================================================================
//
int16_t orion_get_instantaneous_pack_current(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Units 0.1A
    //
    return (int16_t)((device_data_ptr->bms_data1.instantaneous_pack_current));
}


//=============================================================================
//
// orion_get_instantaneous_pack_voltage()
//
//=============================================================================
//
uint16_t orion_get_instantaneous_pack_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    //
    // Units = 0.1V
    //
    return (uint16_t)((device_data_ptr->bms_data1.instantaneous_pack_voltage));
}


//=============================================================================
//
// orion_get_pack_high_cell_voltage()
//
//=============================================================================
//
uint16_t orion_get_pack_high_cell_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.0001.
    return (uint16_t)((device_data_ptr->bms_data1.pack_high_cell_voltage));
}


//=============================================================================
//
// orion_get_low_cell_voltage()
//
//=============================================================================
//
uint16_t orion_get_pack_low_cell_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.0001.
    return (uint16_t)((device_data_ptr->bms_data1.pack_low_cell_voltage));
}


//=============================================================================
//
// CAN ID: 0x00NB002
//
//=============================================================================

//
//=============================================================================
//
// orion_get_pack_discharge_current_limit()
//
//=============================================================================
//
uint16_t orion_get_pack_discharge_current_limit(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint16_t)(device_data_ptr->bms_data2.pack_discharge_current_limit);
}

//=============================================================================
//
// orion_get_pack_charge_current_limit()
//
//=============================================================================
//
uint16_t orion_get_pack_charge_current_limit(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint16_t)(device_data_ptr->bms_data2.pack_charge_current_limit);
}

//=============================================================================
//
// orion_get_pack_high_cell_temperature()
//
//=============================================================================
//
int8_t orion_get_pack_high_cell_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (int8_t)(device_data_ptr->bms_data2.pack_high_temperature);
}

//=============================================================================
//
// orion_get_pack_low_cell_temperature()
//
//=============================================================================
//
int8_t orion_get_pack_low_cell_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (int8_t)(device_data_ptr->bms_data2.pack_low_temperature);
}

//=============================================================================
//
// orion_get_pack_average_cell_temperature()
//
//=============================================================================
//
int8_t orion_get_pack_average_cell_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (int8_t)(device_data_ptr->bms_data2.pack_average_temperature);
}

//=============================================================================
//
// orion_get_internal_bms_temperature()
//
//=============================================================================
//
int8_t orion_get_internal_bms_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (int8_t)(device_data_ptr->bms_data2.internal_temperature);
}

//=============================================================================
//
// CAN ID: 0x00NB003
//
//=============================================================================

//
//=============================================================================
//
// orion_get_state_of_charge()
//
//=============================================================================
//
uint8_t orion_get_state_of_charge(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multipled by 2, hence the received value
    // is multiplied by 0.5.
    return (uint8_t)((device_data_ptr->bms_data3.state_of_charge) * 0.5);
}

//=============================================================================
//
// orion_get_pack_total_resiatance()
//
//=============================================================================
//
uint16_t orion_get_pack_total_resiatance(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.0010.
    return (uint16_t)((device_data_ptr->bms_data3.pack_total_resiatance));
}

//=============================================================================
//
// orion_get_pack_open_voltage()
//
//=============================================================================
//
uint16_t orion_get_pack_open_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)((device_data_ptr->bms_data3.pack_open_voltage));
}

//=============================================================================
//
// orion_get_pack_amp_hours()
//
//=============================================================================
//
uint16_t orion_get_pack_amp_hours(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)((device_data_ptr->bms_data3.pack_amp_hours));
}

//=============================================================================
//
// CAN ID: 0x00BN004
//
//=============================================================================
//

//=============================================================================
//
// orion_get_pack_health()
//
//=============================================================================
//
uint8_t orion_get_pack_health(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->custom_faults.pack_health);
}

//=============================================================================
//
// orion_get_pack_relay_status()
//
//=============================================================================
//
uint8_t orion_get_pack_relay_status(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->custom_faults.pack_relay_status);
}

//=============================================================================
//
// CAN ID: 0x00BN006
//
//=============================================================================

//
//=============================================================================
//
// orion_get_average_cell_voltage()
//
//=============================================================================
//
uint16_t orion_get_average_cell_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.0001.
    return (uint16_t)((device_data_ptr->bms_data6.average_cell_voltage));
}

//=============================================================================
//
// orion_get_high_cell_resistance()
//
//=============================================================================
//
uint16_t orion_get_high_cell_resistance(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.0001.
    return (uint16_t)((device_data_ptr->bms_data6.high_cell_resistance));
}

//=============================================================================
//
// orion_get_low_cell_resistance()
//
//=============================================================================
//
uint16_t orion_get_low_cell_resistance(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.01.
    return (uint16_t)((device_data_ptr->bms_data6.low_cell_resistance));
}

//=============================================================================
//
// orion_get_average_cell_resistance()
//
//=============================================================================
//
uint16_t orion_get_average_cell_resistance(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.01.
    return (uint16_t)((device_data_ptr->bms_data6.average_cell_resistance));
}

//=============================================================================
//
// CAN ID: 0x00BN008
//
//=============================================================================

//=============================================================================
//
// orion_get_high_open_cell_voltage()
//
//=============================================================================
//
uint16_t orion_get_high_open_cell_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint16_t)(device_data_ptr->bms_data7.high_open_cell_voltage);
}

//=============================================================================
//
// orion_get_low_open_cell_voltage()
//
//=============================================================================
//
uint16_t orion_get_low_open_cell_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint16_t)(device_data_ptr->bms_data7.low_open_cell_voltage);
}

//=============================================================================
//
// orion_get_low_power_supply_indicator()
//
//=============================================================================
//
uint16_t orion_get_low_power_supply_indicator(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    // The transmitted value is multiplied by 10, hence the received value
    // is multiplied by 0.1.
    return (uint16_t)((device_data_ptr->bms_data7.low_power_supply_indicator) * 0.1);
}

//=============================================================================
//
// orion_get_maximum_number_of_cells()
//
//=============================================================================
//
uint8_t orion_get_maximum_number_of_cells(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data7.maximum_number_of_cells);
}

//=============================================================================
//
// orion_get_number_of_populated_cells()
//
//=============================================================================
//
uint8_t orion_get_number_of_populated_cells(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data7.number_of_populated_cells);
}

//=============================================================================
//
// CAN ID: 0x00NB009
//
//=============================================================================
//

//=============================================================================
//
// orion_get_pack_total_cycles()
//
//=============================================================================
//
uint16_t orion_get_pack_total_cycles(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint16_t)(device_data_ptr->bms_data9.pack_total_cycles);
}

//=============================================================================
//
// orion_get_rolling_counter()
//
//=============================================================================
//
uint8_t orion_get_rolling_counter(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data9.rolling_counter);
}


//=============================================================================
//
//
// ID refers to the cell number, ranging from 1 to 180 Each of these
// IDs tells which cell is the highest or lowest for the indicated
// parameter
//
// CAN ID: 0x00NB004
//=============================================================================
//

//=============================================================================
//
// orion_get_pack_high_temperature_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_high_temperature_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_high_temperature_ID);
}

//=============================================================================
//
// orion_get_pack_low_temperature_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_low_temperature_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_low_temperature_ID);
}

//=============================================================================
//
// orion_get_pack_high_cell_voltage_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_high_cell_voltage_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_high_cell_voltage_ID);
}

//=============================================================================
//
// orion_get_pack_low_cell_voltage_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_low_cell_voltage_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_low_cell_voltage_ID);
}

//=============================================================================
//
// orion_get_pack_high_internal_resistance_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_high_internal_resistance_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_high_internal_resistance_ID);
}

//=============================================================================
//
// orion_get_pack_low_internal_resistance_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_low_internal_resistance_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_low_internal_resistance_ID);
}

//=============================================================================
//
// orion_get_pack_high_open_cell_voltage_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_high_open_cell_voltage_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_high_open_cell_voltage_ID);
}

//=============================================================================
//
// orion_get_pack_low_open_cell_voltage_ID()
//
//=============================================================================
//
uint8_t orion_get_pack_low_open_cell_voltage_ID(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (uint8_t)(device_data_ptr->bms_data5.pack_low_open_cell_voltage_ID);
}

//=============================================================================
// Custom Flag 0 Status Bits
//=============================================================================
//
bool_t orion_get_flag0_discharge_relay(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_relay);
}

bool_t orion_get_flag0_charge_relay(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charge_relay);
}

bool_t orion_get_flag0_charger_safety(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charger_safety);
}

bool_t orion_get_flag0_voltage_fail_safe(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.voltage_fail_safe);
}

bool_t orion_get_flag0_current_fail_safe(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.current_fail_safe);
}

bool_t orion_get_flag0_power_supply_failure(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.power_supply_failure);
}

bool_t orion_get_flag0_multipurpose_input_state(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.multipurpose_input_state);
}

//=============================================================================
// Custom Flag 0 Status Bits
//=============================================================================
//
bool_t orion_get_flag1_internal_communication_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.internal_communication_fault);
}

bool_t orion_get_flag1_internal_conversion_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.internal_conversion_fault);
}

bool_t orion_get_flag1_weak_cell_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.weak_cell_fault);
}

bool_t orion_get_flag1_low_cell_voltage_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.low_cell_voltage_fault);
}

bool_t orion_get_flag1_open_cell_voltage_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.open_cell_voltage_fault);
}

bool_t orion_get_flag1_current_sensor_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.current_sensor_fault);
}

bool_t orion_get_flag1_voltage_redundancy_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.voltage_redundancy_fault);
}

//=============================================================================
// Custom Flag 2 Status Bits
//=============================================================================
//
bool_t orion_get_flag2_weak_pack_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.weak_pack_fault);
}

bool_t orion_get_flag2_fan_monitor_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.fan_monitor_fault);
}

bool_t orion_get_flag2_thermistor_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.thermistor_fault);
}

bool_t orion_get_flag2_communication_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.communication_fault);
}

bool_t orion_get_flag2_always_on_power_supply_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.always_on_power_supply_fault);
}

bool_t orion_get_flag2_high_voltage_isolation_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.high_voltage_isolation_fault);
}

bool_t orion_get_flag2_power_supply_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.power_supply_fault);
}

bool_t orion_get_flag2_charger_enable_relay_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charger_enable_relay_fault);
}

//=============================================================================
// Custom Flag 3 Status Bits
//=============================================================================
//
bool_t orion_get_flag3_discharge_enable_relay_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_enable_relay_fault);
}

bool_t orion_get_flag3_charger_safety_relay_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charger_safety_relay_fault);
}

bool_t orion_get_flag3_internal_thermistor_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.internal_thermistor_fault);
}

bool_t orion_get_flag3_internal_logic_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.internal_logic_fault);
}

bool_t orion_get_flag3_internal_memory_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.internal_memory_fault);

}

//=============================================================================
// Current Status
//=============================================================================
//
bool_t orion_get_current_status_discharge_current_limit_low_soc(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_current_limit_low_soc);
}

bool_t orion_get_current_status_discharge_current_limit_high_cell_resistance(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_current_limit_high_cell_resistance);
}

bool_t orion_get_current_status_discharge_current_limit_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_current_limit_temperature);
}

bool_t orion_get_current_status_discharge_current_limit_low_cell_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_current_limit_low_cell_voltage);
}

bool_t orion_get_current_status_discharge_current_limit_low_pack_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_current_limit_low_pack_voltage);
}

bool_t orion_get_current_status_discharge_current_limit_voltage_failsafe(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_current_limit_voltage_failsafe);
}

bool_t orion_get_current_status_discharge_current_limit_communication_failsafe(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.discharge_current_limit_communication_failsafe);
}

bool_t orion_get_current_status_charge_current_limit_high_soc(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charge_current_limit_high_soc);
}

bool_t orion_get_current_status_charge_current_limit_high_cell_resistance(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charge_current_limit_high_cell_resistance);
}

bool_t orion_get_current_status_charge_current_limit_high_cell_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charge_current_limit_high_cell_voltage);
}

bool_t orion_get_current_status_charge_current_limit_high_pack_voltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charge_current_limit_high_pack_voltage);
}

bool_t orion_get_current_status_charger_latch(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charger_latch);
}

bool_t orion_get_current_status_charge_current_limit_alternate_current_limit(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charge_current_limit_alternate_current_limit);
}


bool_t orion_get_current_status_charge_current_limit_temperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);

    if (device_data_ptr == NULL)
	{
		DEBUG("NULL Pointer");
		return FALSE;
	}

    return (bool_t)(device_data_ptr->custom_faults.charge_current_limit_temperature);
}


//////////////////////////////////////////////////////////////////////////////
//
// By default, receive CAN message timeouts are disabled when receive
// messsages are registered. The timeouts may be set with the
// pdm_set_timeout_ setter functions. Also, by default, the _rx_ok
// status is set to FALSE when the receive CAN messages are
// registered. This function will return FALSE until each of the
// devices receive CAN messagers have been received at least one time.
//
// If receive CAN message timeouts are set for one or more messages,
// this function returns TRUE if there are no timeouts and FALSE if
// one or more messages have timed out.
//
// In order for receive CAN message timeouts to be active, a call to
// CAN service's can_rx_check_message_timeouts() must be made from
// User_App().
//
//////////////////////////////////////////////////////////////////////////////
//
bool_t orion_get_can_rx_ok(device_instances_t device)
{
    //
    // Get a pointer to this device instance's device_data_t
    // structure.
    //
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, first_orion_device_data_ptr);
    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // By default, receive CAN message timeouts are disabled.
    //
    if ((device_data_ptr->bms_data1_rx_ok == TRUE) &&
        (device_data_ptr->bms_data2_rx_ok == TRUE) &&
        (device_data_ptr->bms_data3_rx_ok == TRUE) &&
        (device_data_ptr->custom_faults_rx_ok == TRUE) &&
        (device_data_ptr->bms_data5_rx_ok == TRUE) &&
        (device_data_ptr->bms_data6_rx_ok == TRUE) &&
        (device_data_ptr->bms_data7_rx_ok == TRUE) &&
        (device_data_ptr->bms_data8_rx_ok == TRUE) &&
        (device_data_ptr->bms_data9_rx_ok == TRUE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
