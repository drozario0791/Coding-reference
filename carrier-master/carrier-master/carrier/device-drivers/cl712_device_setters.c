/******************************************************************************
 *
 *        Name: device_name_setters.c
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Thursday, 04 July 2019
 *
 ******************************************************************************
 */
#include "Prototypes.h"
#include "cl712_device_private.h"

extern device_data_t *first_cl712_device_data_ptr;

/******************************************************************************
 *
 *        Name: cl712_set_tx_message1()
 *
 * Description:
 *
 *
 *      Author: Deepak
 *        Date: Thursday, 19 July 2019
 *
 ******************************************************************************
 */
void cl712_set_tx_main_screen_message1(
	device_instances_t device,
	uint32_t odometer_m,
	uint32_t hourmeter_s)
{

	cl712_tx_main_screen_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message1);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->odometer_m = BYTE_SWAP32(odometer_m);
    dest_ptr->hourmeter_s = BYTE_SWAP32(hourmeter_s);

}

//=============================================================================
//
// cl712_set_tx_message2
//
//=============================================================================
//
void cl712_set_tx_main_screen_message2(
	device_instances_t device,
    uint32_t time_until_service_s,
    uint16_t groundspeed_cmps,
    uint8_t soc_percent,
    uint8_t gear_direction)
{

	cl712_tx_main_screen_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message2);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


    dest_ptr->time_until_service_s = BYTE_SWAP32(time_until_service_s);
    dest_ptr->groundspeed_cmps = BYTE_SWAP16(groundspeed_cmps);
    dest_ptr->soc_percent = soc_percent;
    dest_ptr->gear_direction = gear_direction;

}

//=============================================================================
//
// cl712_set_tx_message3()
//
//=============================================================================
//
void cl712_set_tx_main_screen_message3(
	device_instances_t device,
	uint8_t gear_ratio,
	uint8_t vehicle_state,
	uint8_t misc_status,
	uint8_t can1_rx_4_spare_1,
    uint32_t yellow_warning_reason)
{

	cl712_tx_main_screen_message3_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message3);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


    dest_ptr->gear_ratio = gear_ratio;
    dest_ptr->vehicle_state = vehicle_state;
    dest_ptr->misc_status = misc_status;
    dest_ptr->can1_rx_4_spare_1 = can1_rx_4_spare_1;
    dest_ptr->yellow_warning_reason = BYTE_SWAP32(yellow_warning_reason);


}

//=============================================================================
//
// cl712_set_tx_message4()
//
//=============================================================================
//
void cl712_set_tx_main_screen_message4(
	device_instances_t device,
	uint32_t crit_fail_hv_off_reason,
	uint32_t crit_fail_hv_on_reason)
{

	cl712_tx_main_screen_message4_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message4);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->crit_fail_hv_off_reason = BYTE_SWAP32(crit_fail_hv_off_reason);
    dest_ptr->crit_fail_hv_on_reason = BYTE_SWAP32(crit_fail_hv_on_reason);

}

//=============================================================================
//
// cl712_set_tx_bms_screen_message1
//
//=============================================================================
//
void cl712_set_tx_bms_screen_message1(
	device_instances_t device,
    uint16_t battery_1_high_cell_voltage_mv,
    uint16_t battery_1_low_cell_voltage_mv,
    uint16_t battery_1_cell_voltage_delta_mv,
    uint16_t battery_1_pack_voltage_v10)
{

	cl712_tx_bms_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message5);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->battery_1_high_cell_voltage_mv = BYTE_SWAP16(battery_1_high_cell_voltage_mv);
    dest_ptr->battery_1_low_cell_voltage_mv = BYTE_SWAP16(battery_1_low_cell_voltage_mv);
    dest_ptr->battery_1_cell_voltage_delta_mv = BYTE_SWAP16(battery_1_cell_voltage_delta_mv);
    dest_ptr->battery_1_pack_voltage_v10 = BYTE_SWAP16(battery_1_pack_voltage_v10);

}

//=============================================================================
//
// cl712_set_tx_bms_screen_message2()
//
//=============================================================================
//
void cl712_set_tx_bms_screen_message2(
	device_instances_t device,
    int16_t battery_1_temperature_c,
    int16_t battery_1_pack_current_v10,
    uint8_t battery_1_isolation_fault_status,
    uint8_t battery_1_precharge_status,
    uint8_t battery_1_positive_contactor_status,
    uint8_t battery_1_negative_contactor_status)
{

	cl712_tx_bms_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message6);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->battery_1_temperature_c = BYTE_SWAP16(battery_1_temperature_c);
    dest_ptr->battery_1_pack_current_v10 = BYTE_SWAP16(battery_1_pack_current_v10);
    dest_ptr->battery_1_isolation_fault_status = battery_1_isolation_fault_status;
    dest_ptr->battery_1_precharge_status = battery_1_precharge_status;
    dest_ptr->battery_1_positive_contactor_status = battery_1_positive_contactor_status;
    dest_ptr->battery_1_negative_contactor_status = battery_1_negative_contactor_status;

}


//=============================================================================
//
// cl712_set_tx_bms_screen_message3()
//
//=============================================================================
//
void cl712_set_tx_bms_screen_message3(
	device_instances_t device,
    uint16_t battery_2_high_cell_voltage_mv,
    uint16_t battery_2_low_cell_voltage_mv,
    uint16_t battery_2_cell_voltage_delta_mv,
    uint16_t battery_2_pack_voltage_v10)
{
	cl712_tx_bms_message3_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message7);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->battery_2_high_cell_voltage_mv = BYTE_SWAP16(battery_2_high_cell_voltage_mv);
    dest_ptr->battery_2_low_cell_voltage_mv = BYTE_SWAP16(battery_2_low_cell_voltage_mv);
    dest_ptr->battery_2_cell_voltage_delta_mv = BYTE_SWAP16(battery_2_cell_voltage_delta_mv);
    dest_ptr->battery_2_pack_voltage_v10 = BYTE_SWAP16(battery_2_pack_voltage_v10);

}

//=============================================================================
//
// cl712_set_tx_bms_screen_message4()
//
//=============================================================================
//
void cl712_set_tx_bms_screen_message4(
	device_instances_t device,
    int16_t battery_2_temperature_c,
    int16_t battery_2_pack_current_v10,
    uint8_t battery_2_isolation_fault_status,
    uint8_t battery_2_precharge_status,
    uint8_t battery_2_positive_contactor_status,
    uint8_t battery_2_negative_contactor_status)
{
	cl712_tx_bms_message4_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message8);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->battery_2_temperature_c = BYTE_SWAP16(battery_2_temperature_c);
    dest_ptr->battery_2_pack_current_v10 = BYTE_SWAP16(battery_2_pack_current_v10);
    dest_ptr->battery_2_isolation_fault_status = battery_2_isolation_fault_status;
    dest_ptr->battery_2_precharge_status = battery_2_precharge_status;
    dest_ptr->battery_2_positive_contactor_status = battery_2_positive_contactor_status;
    dest_ptr->battery_2_negative_contactor_status = battery_2_negative_contactor_status;

}


//=============================================================================
//
// cl712_set_tx_bms_screen_message5()
//
//=============================================================================
//
void cl712_set_tx_bms_screen_message5(
	device_instances_t device,
    uint16_t battery_3_high_cell_voltage_mv,
    uint16_t battery_3_low_cell_voltage_mv,
    uint16_t battery_3_cell_voltage_delta_mv,
    uint16_t battery_3_pack_voltage_v10)
{

	cl712_tx_bms_message5_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message9);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->battery_3_high_cell_voltage_mv = BYTE_SWAP16(battery_3_high_cell_voltage_mv);
    dest_ptr->battery_3_low_cell_voltage_mv = BYTE_SWAP16(battery_3_low_cell_voltage_mv);
    dest_ptr->battery_3_cell_voltage_delta_mv = BYTE_SWAP16(battery_3_cell_voltage_delta_mv);
    dest_ptr->battery_3_pack_voltage_v10 = BYTE_SWAP16(battery_3_pack_voltage_v10);

}


//=============================================================================
//
// cl712_set_tx_bms_screen_message6()
//
//=============================================================================
//
void cl712_set_tx_bms_screen_message6(
	device_instances_t device,
    int16_t battery_3_temperature_c,
    int16_t battery_3_pack_current_v10,
    uint8_t battery_3_isolation_fault_status,
    uint8_t battery_3_precharge_status,
    uint8_t battery_3_positive_contactor_status,
    uint8_t battery_3_negative_contactor_status)
{

	cl712_tx_bms_message6_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message10);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->battery_3_temperature_c = BYTE_SWAP16(battery_3_temperature_c);
    dest_ptr->battery_3_pack_current_v10 = BYTE_SWAP16(battery_3_pack_current_v10);
    dest_ptr->battery_3_isolation_fault_status = battery_3_isolation_fault_status;
    dest_ptr->battery_3_precharge_status = battery_3_precharge_status;
    dest_ptr->battery_3_positive_contactor_status = battery_3_positive_contactor_status;
    dest_ptr->battery_3_negative_contactor_status = battery_3_negative_contactor_status;

}

//=============================================================================
//
//
//
//=============================================================================
//
void cl712_set_tx_pdm_status_screen_message1(
	device_instances_t device,
    uint16_t pdm_1_supply_voltage_v10,
    uint16_t pdm_2_supply_voltage_v10,
    uint16_t pdm_3_supply_voltage_v10,
    uint8_t  pdm_1_status,
    uint8_t  pdm_2_status)
{

	cl712_tx_pdm_voltage_status_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message11);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->pdm_1_supply_voltage_v10 = BYTE_SWAP16(pdm_1_supply_voltage_v10);
    dest_ptr->pdm_2_supply_voltage_v10 = BYTE_SWAP16(pdm_2_supply_voltage_v10);
    dest_ptr->pdm_3_supply_voltage_v10 = BYTE_SWAP16(pdm_3_supply_voltage_v10);
    dest_ptr->pdm_1_status = pdm_1_status;
    dest_ptr->pdm_2_status = pdm_2_status;
}

//=============================================================================
//
//
//
//=============================================================================
//
void cl712_set_tx_pdm_status_screen_message2(
	device_instances_t device,
	uint8_t pdm_3_status,
	uint8_t arg2,
	uint8_t arg3,
	uint8_t arg4,
    uint8_t arg5,
	uint8_t arg6,
	uint8_t arg7,
	uint8_t arg8)
{

	cl712_tx_pdm_voltage_status_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message12);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

	dest_ptr->pdm_3_status = pdm_3_status;
    dest_ptr->data2 = arg2;
    dest_ptr->data3 = arg3;
    dest_ptr->data4 = arg4;
    dest_ptr->data5 = arg5;
    dest_ptr->data6 = arg6;
    dest_ptr->data7 = arg7;
    dest_ptr->data8 = arg8;

}

//=============================================================================
//
// cl712_set_tx_pdm_1_status_screen_message1()
//
//=============================================================================
//
void cl712_set_tx_pdm_1_status_screen_message1(
	device_instances_t device,
	uint8_t pdm_1_reverse_lights_status,
    uint8_t pdm_1_brake_lights_status,
    uint8_t pdm_1_deck_lights_status,
    uint8_t pdm_1_brake_release_status,
    uint8_t pdm_1_reverse_beeper_status,
    uint8_t pdm_1_reverse_beacon_status,
    uint8_t pdm_1_right_spray_selenoid_status,
    uint8_t pdm_1_center_spray_selenoid_status)
{

	cl712_tx_pdm_1_status_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message13);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


    dest_ptr->pdm_1_reverse_lights_status = pdm_1_reverse_lights_status;
    dest_ptr->pdm_1_brake_lights_status  = pdm_1_brake_lights_status ;
    dest_ptr->pdm_1_deck_lights_status = pdm_1_deck_lights_status;
    dest_ptr->pdm_1_brake_release_status = pdm_1_brake_release_status;
    dest_ptr->pdm_1_reverse_beeper_status = pdm_1_reverse_beeper_status;
    dest_ptr->pdm_1_reverse_beacon_status = pdm_1_reverse_beacon_status;
    dest_ptr->pdm_1_right_spray_selenoid_status = pdm_1_right_spray_selenoid_status;
    dest_ptr->pdm_1_center_spray_selenoid_status = pdm_1_center_spray_selenoid_status;
}

//=============================================================================
//
// cl712_set_tx_pdm_1_status_and_current_screen_message2()
//
//=============================================================================
//
void cl712_set_tx_pdm_1_status_and_current_screen_message2(
	device_instances_t device,
    uint8_t pdm_1_left_spray_selenoid_status,
    uint8_t pdm_1_spare_status,
    uint8_t pdm_1_channel_11_status,
    uint8_t pdm_1_channel_12_status,
    uint8_t pdm_1_reverse_lights_current,
    uint8_t pdm_1_brake_lights_current,
    uint8_t pdm_1_deck_lights_current,
    uint8_t pdm_1_brake_release_current)
{

	cl712_tx_pdm_1_status_and_current_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message14);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->pdm_1_left_spray_selenoid_status = pdm_1_left_spray_selenoid_status;
    dest_ptr->pdm_1_spare_status = pdm_1_spare_status;
    dest_ptr->pdm_1_channel_11_status = pdm_1_channel_11_status;
    dest_ptr->pdm_1_channel_12_status = pdm_1_channel_12_status;
    dest_ptr->pdm_1_reverse_lights_current = pdm_1_reverse_lights_current;
    dest_ptr->pdm_1_brake_lights_current = pdm_1_brake_lights_current;
    dest_ptr->pdm_1_deck_lights_current = pdm_1_deck_lights_current;
    dest_ptr->pdm_1_brake_release_current = pdm_1_brake_release_current;

}

//=============================================================================
//
// cl712_set_tx_pdm_1_current_screen_message3()
//
//=============================================================================
//
void cl712_set_tx_pdm_1_current_screen_message3(
	device_instances_t device,
    uint8_t pdm_1_reverse_beeper_current,
    uint8_t pdm_1_reverse_beacon_current,
    uint8_t pdm_1_right_spray_selenoid_current,
    uint8_t pdm_1_center_spray_selenoid_current,
    uint8_t pdm_1_left_spray_selenoid_current,
    uint8_t pdm_1_spare_current,
    uint8_t pdm_1_channel_11_current,
    uint8_t pdm_1_channel_12_current)
{

	cl712_tx_pdm_1_current_message3_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message15);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


    dest_ptr->pdm_1_reverse_beeper_current = pdm_1_reverse_beeper_current;
    dest_ptr->pdm_1_reverse_beacon_current = pdm_1_reverse_beacon_current;
    dest_ptr->pdm_1_right_spray_selenoid_current = pdm_1_right_spray_selenoid_current;
    dest_ptr->pdm_1_center_spray_selenoid_current = pdm_1_center_spray_selenoid_current;
    dest_ptr->pdm_1_left_spray_selenoid_current = pdm_1_left_spray_selenoid_current;
    dest_ptr->pdm_1_spare_current = pdm_1_spare_current;
    dest_ptr->pdm_1_channel_11_current = pdm_1_channel_11_current;
    dest_ptr->pdm_1_channel_12_current = pdm_1_channel_12_current;

}

//=============================================================================
//
// cl712_set_tx_pdm_2_status_screen_message1()
//
//=============================================================================
//
void cl712_set_tx_pdm_2_status_screen_message1(
	device_instances_t device,
    uint8_t pdm_2_start_drain_selenoid_status,
    uint8_t pdm_2_aux_hydraulics_12v_bussbar_status,
    uint8_t pdm_2_gear_1_selenoid_status,
    uint8_t pdm_2_gear_2_selenoid_status,
    uint8_t pdm_2_gear_3_selenoid_status,
    uint8_t pdm_2_gear_4_selenoid_status,
    uint8_t pdm_2_power_electronics_pump_status,
    uint8_t pdm_2_selector_selenoid_status)
{

	cl712_tx_pdm_2_status_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message16);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->pdm_2_start_drain_selenoid_status = pdm_2_start_drain_selenoid_status;
    dest_ptr->pdm_2_aux_hydraulics_12v_bussbar_status = pdm_2_aux_hydraulics_12v_bussbar_status;
    dest_ptr->pdm_2_gear_1_selenoid_status = pdm_2_gear_1_selenoid_status;
    dest_ptr->pdm_2_gear_2_selenoid_status = pdm_2_gear_2_selenoid_status;
    dest_ptr->pdm_2_gear_3_selenoid_status = pdm_2_gear_3_selenoid_status;
    dest_ptr->pdm_2_gear_4_selenoid_status = pdm_2_gear_4_selenoid_status;
    dest_ptr->pdm_2_power_electronics_pump_status = pdm_2_power_electronics_pump_status;
    dest_ptr->pdm_2_selector_selenoid_status = pdm_2_selector_selenoid_status;

}

//=============================================================================
//
// cl712_set_tx_pdm_2_status_and_current_screen_message2()
//
//=============================================================================
//
void cl712_set_tx_pdm_2_status_and_current_screen_message2(
	device_instances_t device,
    uint8_t pdm_2_brake_release_selenoid_status,
    uint8_t pdm_2_service_brake_selenoid_status,
    uint8_t pdm_2_hydraulic_motor_cooling_pump_status,
    uint8_t pdm_2_hydraulic_motor_radiator_cooling_fan_status,
    uint8_t pdm_2_start_drain_selenoid_current,
    uint8_t pdm_2_aux_hydraulics_12v_bussbar_current,
    uint8_t pdm_2_gear_1_selenoid_current,
    uint8_t pdm_2_gear_2_selenoid_current)
{

	cl712_tx_pdm_2_status_and_current_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message17);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->pdm_2_brake_release_selenoid_status = pdm_2_brake_release_selenoid_status;
    dest_ptr->pdm_2_service_brake_selenoid_status = pdm_2_service_brake_selenoid_status;
    dest_ptr->pdm_2_hydraulic_motor_cooling_pump_status = pdm_2_hydraulic_motor_cooling_pump_status;
    dest_ptr->pdm_2_hydraulic_motor_radiator_cooling_fan_status = pdm_2_hydraulic_motor_radiator_cooling_fan_status;
    dest_ptr->pdm_2_start_drain_selenoid_current = pdm_2_start_drain_selenoid_current;
    dest_ptr->pdm_2_aux_hydraulics_12v_bussbar_current = pdm_2_aux_hydraulics_12v_bussbar_current;
    dest_ptr->pdm_2_gear_1_selenoid_current = pdm_2_gear_1_selenoid_current;
    dest_ptr->pdm_2_gear_2_selenoid_current = pdm_2_gear_2_selenoid_current;
}

//=============================================================================
//
// cl712_set_tx_pdm_2_current_screen_message3()
//
//=============================================================================
//
void cl712_set_tx_pdm_2_current_screen_message3(
	device_instances_t device,
    uint8_t pdm_2_gear_3_selenoid_current,
    uint8_t pdm_2_gear_4_selenoid_current,
    uint8_t pdm_2_power_electronics_pump_current,
    uint8_t pdm_2_selector_selenoid_current,
    uint8_t pdm_2_brake_release_selenoid_current,
    uint8_t pdm_2_service_brake_selenoid_current,
    uint8_t pdm_2_hydraulic_motor_cooling_pump_current,
    uint8_t pdm_2_hydraulic_motor_radiator_cooling_fan_current)
{

	cl712_tx_pdm_2_current_message3_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message18);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}


    dest_ptr->pdm_2_gear_3_selenoid_current = pdm_2_gear_3_selenoid_current;
    dest_ptr->pdm_2_gear_4_selenoid_current = pdm_2_gear_4_selenoid_current;
    dest_ptr->pdm_2_power_electronics_pump_current = pdm_2_power_electronics_pump_current;
    dest_ptr->pdm_2_selector_selenoid_current = pdm_2_selector_selenoid_current;
    dest_ptr->pdm_2_brake_release_selenoid_current = pdm_2_brake_release_selenoid_current;
    dest_ptr->pdm_2_service_brake_selenoid_current = pdm_2_service_brake_selenoid_current;
    dest_ptr->pdm_2_hydraulic_motor_cooling_pump_current = pdm_2_hydraulic_motor_cooling_pump_current;
    dest_ptr->pdm_2_hydraulic_motor_radiator_cooling_fan_current = pdm_2_hydraulic_motor_radiator_cooling_fan_current;

}

//=============================================================================
//
// cl712_set_tx_pdm_3_status_screen_message1()
//
//=============================================================================
//
void cl712_set_tx_pdm_3_status_screen_message1(
	device_instances_t device,
    uint8_t pdm_3_work_lights_status,
    uint8_t pdm_3_beacon_status,
    uint8_t pdm_3_head_light_status,
    uint8_t pdm_3_horn_status,
    uint8_t pdm_3_charger_1_bussbar_status,
    uint8_t pdm_3_drive_motor_cooling_pump_status,
    uint8_t pdm_3_imt_cooling_pump_status,
    uint8_t pdm_3_fan_radiator_1_2_status)
{

	cl712_tx_pdm_3_status_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message19);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->pdm_3_work_lights_status = pdm_3_work_lights_status;
    dest_ptr->pdm_3_beacon_status = pdm_3_beacon_status;
    dest_ptr->pdm_3_head_light_status = pdm_3_head_light_status;
    dest_ptr->pdm_3_horn_status = pdm_3_horn_status;
    dest_ptr->pdm_3_charger_1_bussbar_status = pdm_3_charger_1_bussbar_status;
    dest_ptr->pdm_3_drive_motor_cooling_pump_status = pdm_3_drive_motor_cooling_pump_status;
    dest_ptr->pdm_3_imt_cooling_pump_status = pdm_3_imt_cooling_pump_status;
    dest_ptr->pdm_3_fan_radiator_1_2_status = pdm_3_fan_radiator_1_2_status;

}

//=============================================================================
//
// cl712_set_tx_pdm_3_status_and_current_screen_message2()
//
//=============================================================================
//
void cl712_set_tx_pdm_3_status_and_current_screen_message2(
	device_instances_t device,
    uint8_t pdm_3_pump_charger_1_cooling_status,
    uint8_t pdm_3_pump_charger_2_cooling_status,
    uint8_t pdm_3_main_bussbar_supply_status,
    uint8_t pdm_3_hydraulic_fan_selenoid_status,
    uint8_t pdm_3_work_lights_current,
    uint8_t pdm_3_beacon_current,
    uint8_t pdm_3_head_light_current,
    uint8_t pdm_3_horn_current)
{

	cl712_tx_pdm_3_status_and_current_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message20);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->pdm_3_pump_charger_1_cooling_status = pdm_3_pump_charger_1_cooling_status;
    dest_ptr->pdm_3_pump_charger_2_cooling_status = pdm_3_pump_charger_2_cooling_status;
    dest_ptr->pdm_3_main_bussbar_supply_status = pdm_3_main_bussbar_supply_status;
    dest_ptr->pdm_3_hydraulic_fan_selenoid_status = pdm_3_hydraulic_fan_selenoid_status;
    dest_ptr->pdm_3_work_lights_current = pdm_3_work_lights_current;
    dest_ptr->pdm_3_beacon_current = pdm_3_beacon_current;
    dest_ptr->pdm_3_head_light_current = pdm_3_head_light_current;
    dest_ptr->pdm_3_horn_current = pdm_3_horn_current;

}

//=============================================================================
//
// cl712_set_tx_pdm_3_current_screen_message3()
//
//=============================================================================
//
void cl712_set_tx_pdm_3_current_screen_message3(
	device_instances_t device,
    uint8_t pdm_3_charger_1_bussbar_current,
    uint8_t pdm_3_drive_motor_cooling_pump_current,
    uint8_t pdm_3_imt_cooling_pump_current,
    uint8_t pdm_3_fan_radiator_1_2_current,
    uint8_t pdm_3_pump_charger_1_cooling_current,
    uint8_t pdm_3_pump_charger_2_cooling_current,
    uint8_t pdm_3_main_bussbar_supply_current,
    uint8_t pdm_3_hydraulic_fan_selenoid_current)
{

	cl712_tx_pdm_3_current_message3_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message21);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->pdm_3_charger_1_bussbar_current = pdm_3_charger_1_bussbar_current;
    dest_ptr->pdm_3_drive_motor_cooling_pump_current = pdm_3_drive_motor_cooling_pump_current;
    dest_ptr->pdm_3_imt_cooling_pump_current = pdm_3_imt_cooling_pump_current;
    dest_ptr->pdm_3_fan_radiator_1_2_current = pdm_3_fan_radiator_1_2_current;
    dest_ptr->pdm_3_pump_charger_1_cooling_current = pdm_3_pump_charger_1_cooling_current;
    dest_ptr->pdm_3_pump_charger_2_cooling_current = pdm_3_pump_charger_2_cooling_current;
    dest_ptr->pdm_3_main_bussbar_supply_current = pdm_3_main_bussbar_supply_current;
    dest_ptr->pdm_3_hydraulic_fan_selenoid_current = pdm_3_hydraulic_fan_selenoid_current;

}



//=============================================================================
//
// void cl712_set_tx_drive_system_screen_message1()
//
//=============================================================================
//
void cl712_set_tx_drive_system_screen_message1(
	device_instances_t device,
    uint16_t drive_dc_voltage_v10,
    uint16_t drive_motor_rpm,
    uint8_t drive_throttle_percentage,
    uint8_t drive_inverter_pcb_temp_c,
    uint8_t drive_inverter_temp_c,
    uint8_t drive_motor_temp_c)
{

    cl712_tx_traction_drive_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message22);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->drive_dc_voltage_v10 = BYTE_SWAP16(drive_dc_voltage_v10);
    dest_ptr->drive_motor_rpm = BYTE_SWAP16(drive_motor_rpm);
    dest_ptr->drive_throttle_percentage = drive_throttle_percentage;
    dest_ptr->drive_inverter_pcb_temp_c = drive_inverter_pcb_temp_c;
    dest_ptr->drive_inverter_temp_c = drive_inverter_temp_c;
    dest_ptr->drive_motor_temp_c = drive_motor_temp_c;

}

//=============================================================================
//
// cl712_set_tx_drive_system_screen_message2()
//
//=============================================================================
//
void cl712_set_tx_drive_system_screen_message2(
    device_instances_t device,
    uint16_t throttle_adc_raw,
    uint16_t throttle_released,
    uint16_t throttle_depressed)
{

    cl712_tx_traction_drive_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message23);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->throttle_adc_raw = BYTE_SWAP16(throttle_adc_raw);
    dest_ptr->throttle_released = BYTE_SWAP16(throttle_released);
    dest_ptr->throttle_depressed = BYTE_SWAP16(throttle_depressed);
    dest_ptr->unused1 = 0x0000;
}

//=============================================================================
//
// cl712_set_tx_hydraulic_drive_system_screen_message1
//
//=============================================================================
//
void cl712_set_tx_hydraulic_drive_system_screen_message1(
	device_instances_t device,
    uint16_t ehs_dc_voltage_v10,
    uint16_t ehs_motor_rpm,
    uint8_t ehs_inverter_pcb_temp_c,
    uint8_t ehs_inverter_temp_c,
    uint8_t ehs_motor_temp_c,
    uint8_t can1_rx_25_spare_1)
{

    cl712_tx_hydraulic_drive_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message24);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->ehs_dc_voltage_v10 = BYTE_SWAP16(ehs_dc_voltage_v10);
    dest_ptr->ehs_motor_rpm = BYTE_SWAP16(ehs_motor_rpm);
    dest_ptr->ehs_inverter_pcb_temp_c = ehs_inverter_pcb_temp_c;
    dest_ptr->ehs_inverter_temp_c = ehs_inverter_temp_c;
    dest_ptr->ehs_motor_temp_c = ehs_motor_temp_c;
    dest_ptr->can1_rx_25_spare_1 = can1_rx_25_spare_1;

}

//=============================================================================
//
// cl712_set_tx_system_pressures_screen_message1()
//
//=============================================================================
//
void cl712_set_tx_system_pressures_screen_message1(
	device_instances_t device,
    uint16_t main_pressure_psi,
    uint16_t front_brake_pressure_psi,
    uint16_t rear_brake_pressure_psi,
    uint16_t driveline_pressure_psi)
{

    cl712_tx_system_pressure_message1_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message25);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->main_pressure_psi = BYTE_SWAP16(main_pressure_psi);
    dest_ptr->front_brake_pressure_psi = BYTE_SWAP16(front_brake_pressure_psi);
    dest_ptr->rear_brake_pressure_psi = BYTE_SWAP16(rear_brake_pressure_psi);
    dest_ptr->driveline_pressure_psi = BYTE_SWAP16(driveline_pressure_psi);

}

//=============================================================================
//
// cl712_set_tx_system_pressures_screen_message2()
//
//=============================================================================
//
void cl712_set_tx_system_pressures_screen_message2(
    device_instances_t device,
    uint16_t front_parking_brake_psi,
    uint16_t front_accumulator_pressure_psi,
    uint16_t rear_accumulator_pressure_psi,
    uint16_t transmission_pressure_10x_psi)
{

    cl712_tx_system_pressure_message2_t *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message26);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->front_parking_brake_psi = BYTE_SWAP16(front_parking_brake_psi);
    dest_ptr->front_accumulator_pressure_psi = BYTE_SWAP16(front_accumulator_pressure_psi);
    dest_ptr->rear_accumulator_pressure_psi = BYTE_SWAP16(rear_accumulator_pressure_psi);
    dest_ptr->transmission_pressure_10x_psi = BYTE_SWAP16(transmission_pressure_10x_psi);

}

//=============================================================================
//
// cl712_set_pump_fan_status_message1()
//
//=============================================================================
//
void cl712_set_pump_fan_status_message1(
    device_instances_t device,
    uint8_t traction_pump_status,
    uint8_t hydraulic_pump_status,
    uint8_t pe_fan_status,
    uint8_t pe_pump_status,
    uint8_t charger_fan_status,
    uint8_t charger_pump_status)
{

    cl712_tx_pump_fan_status *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message27);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->traction_pump_status = traction_pump_status;
    dest_ptr->hydraulic_pump_status = hydraulic_pump_status;
    dest_ptr->pe_fan_status = pe_fan_status;
    dest_ptr->pe_pump_status = pe_pump_status;
    dest_ptr->charger_fan_status = charger_fan_status;
    dest_ptr->charger_pump_status = charger_pump_status;

}

//=============================================================================
//
// cl712_set_pump_fan_override_status_message1()
//
//=============================================================================
//
void cl712_set_pump_fan_override_status_message1(
    device_instances_t device,
    uint8_t traction_pump_override_status,
    uint8_t hydraulic_pump_override_status,
    uint8_t pe_fan_override_status,
    uint8_t pe_pump_override_status,
    uint8_t charger_fan_override_status,
    uint8_t charger_pump_override_status)
{

    cl712_tx_pump_fan_override_status *dest_ptr = NULL;

	//
    // Get a pointer to the proper device data structure.
    //
	device_data_t *device_data_ptr =
		get_device_linked_data_record_instance_ptr(device, first_cl712_device_data_ptr);

    if (device_data_ptr == NULL) {DEBUG("NULL Pointer");}

	//
    // Get a pointer to the output command structure.
    //
    dest_ptr = &(device_data_ptr->tx_message28);
    if (dest_ptr == NULL) {DEBUG("NULL Pointer");}

    dest_ptr->traction_pump_override_status = traction_pump_override_status;
    dest_ptr->hydraulic_pump_override_status = hydraulic_pump_override_status;
    dest_ptr->pe_fan_override_status = pe_fan_override_status;
    dest_ptr->pe_pump_override_status = pe_pump_override_status;
    dest_ptr->charger_fan_override_status = charger_fan_override_status;
    dest_ptr->charger_pump_override_status = charger_pump_override_status;

}
