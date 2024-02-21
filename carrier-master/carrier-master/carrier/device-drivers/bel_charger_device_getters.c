/******************************************************************************
 *
 *        Name: fvt_bel_charger_getters.c
 *
 * Description: All of the getter functions for the Bel Charger.
 *
 *      Author: Tom
 *        Date: Thursday, 04 July 2019
 *
 ******************************************************************************
 */
#include "bel_charger_device_private.h"

extern device_data_t *bel_charger_first_device_data_ptr;

//=============================================================================
//
// Getters for Bel Charger measured_values.
//
//=============================================================================
//
uint16_t bel_get_pin_17_vtd_v_20(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_measured_values_t *source_ptr = &(device_data_ptr->measured_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->pin_17_vtd_v_20;
}

uint16_t bel_get_max_charging_current_ava(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_measured_values_t *source_ptr = &(device_data_ptr->measured_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}


    return (uint8_t)source_ptr->max_charging_current_ava;
}

//=============================================================================
//
// Getters for Bel Charger primary_values.
//
//=============================================================================
//
uint16_t bel_get_input_voltage_rms_phase_1_2_v(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_primary_values_t *source_ptr = &(device_data_ptr->primary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->input_voltage_rms_phase_1_2_v;
}

uint16_t bel_get_input_voltage_rms_phase_2_3_v(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_primary_values_t *source_ptr = &(device_data_ptr->primary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->input_voltage_rms_phase_2_3_v;
}

uint16_t bel_get_input_voltage_rms_phase_3_1_v(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_primary_values_t *source_ptr = &(device_data_ptr->primary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->input_voltage_rms_phase_3_1_v;
}

uint16_t bel_get_input_voltage_frequency_hz_20(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_primary_values_t *source_ptr = &(device_data_ptr->primary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->input_voltage_frequency_hz_20;
}

//=============================================================================
//
// Getters for Bel Charger secondary_values.
//
//=============================================================================
//
uint16_t bel_get_output_voltage_v_20(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_secondary_values_t *source_ptr = &(device_data_ptr->secondary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->output_voltage_v_20;
}

uint16_t bel_get_output_current_a_20(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_secondary_values_t *source_ptr = &(device_data_ptr->secondary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->output_current_a_20;
}

int8_t bel_get_chassis_temperature_c(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_secondary_values_t *source_ptr = &(device_data_ptr->secondary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (int8_t)(source_ptr->chassis_temperature_c - 40);
}

uint16_t bel_get_aux_battery_voltage_v_20(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_secondary_values_t *source_ptr = &(device_data_ptr->secondary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint16_t)source_ptr->aux_battery_voltage_v_20;
}

int8_t bel_get_internal_ambient_temperature_c(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_secondary_values_t *source_ptr = &(device_data_ptr->secondary_values);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (int8_t)(source_ptr->internal_ambient_temperature_c - 40);
}

//=============================================================================
//
// Getters for Bel Charger status.
//
//=============================================================================
//
bool_t bel_get_input_voltage_ok(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->input_voltage_ok;
}

bool_t bel_get_input_frequency_ok(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->input_frequency_ok;
}

bool_t bel_get_hv_battery_undervoltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->hv_battery_undervoltage;
}

bool_t bel_get_hv_battery_overvoltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->hv_battery_overvoltage;
}

bool_t bel_get_hv_battery_voltage_in_range(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->hv_battery_voltage_in_range;
}

bool_t bel_get_output_overcurrent(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->output_overcurrent;
}

bool_t bel_get_unit_overtemperature(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->unit_overtemperature;
}

bool_t bel_get_i2c_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->i2c_fault;
}

bool_t bel_get_can_bus_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->can_bus_fault;
}

bool_t bel_get_eeprom_memory_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->eeprom_memory_fault;
}

bool_t bel_get_unit_thermistor_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->unit_thermistor_fault;
}

bool_t bel_get_lv_battery_in_range(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->lv_battery_in_range;
}

bool_t bel_get_unit_enabled(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->unit_enabled;
}

bool_t bel_get_key_switch_voltage_in_range(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->key_switch_voltage_in_range;
}

bool_t bel_get_output_off_fault_bit_set(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->output_off_fault_bit_set;
}

bool_t bel_get_converter_latched_off_due_to_fault(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->converter_latched_off_due_to_fault;
}

bool_t bel_get_converter_commanded_off(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->converter_commanded_off;
}

bool_t bel_get_start_up_init_done_self_test_passed(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->start_up_init_done_self_test_passed;
}

bool_t bel_get_output_off_control_dsp_module_failure(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->output_off_control_dsp_module_failure;
}

bool_t bel_get_control_dsp_in_bootloader(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->control_dsp_in_bootloader;
}

bool_t bel_get_proprietary_a(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->proprietary_a;
}

bool_t bel_get_signal_dsp_uploading_firmware_to_control_dsp(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->signal_dsp_uploading_firmware_to_control_dsp;
}

bool_t bel_get_gpi_signal_on_external_signal_conenctor(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->gpi_signal_on_external_signal_conenctor;
}

bool_t bel_get_wake_up_power_switch_failure(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->wake_up_power_switch_failure;
}

bool_t bel_get_gpo_up_power_switch_failure(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->gpo_up_power_switch_failure;
}

bool_t bel_get_hvil_loop_disconnected(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->hvil_loop_disconnected;
}

bool_t bel_get_buck_undervoltage(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->buck_undervoltage;
}

bool_t bel_get_proximity_and_pilot_enabled(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->proximity_and_pilot_enabled;
}

bool_t bel_get_pilot_signal_ok(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->pilot_signal_ok;
}

bool_t bel_get_proximity_signal_ok(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (bool_t)source_ptr->proximity_signal_ok;
}

uint8_t bel_get_chg_state(device_instances_t device)
{
    device_data_t *device_data_ptr =
        get_device_linked_data_record_instance_ptr(device, bel_charger_first_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

    //
    // Have the pointer to this device's data record, now get a
    // pointer to the appropriate receive data structure.
    //
    bel_charger_status_t *source_ptr = &(device_data_ptr->status);
    if (source_ptr == NULL) {DEBUG("NULL Pointer");}

    return (uint8_t)source_ptr->chg_state;
}
