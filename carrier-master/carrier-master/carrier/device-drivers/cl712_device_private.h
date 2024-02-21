/******************************************************************************
 *
 *        Name: cl712_device_private.h
 *
 * Description: This is the private header file for the device_name
 *              device driver.
 *
 *      Author: Tom
 *        Date: Tuesday, 25 June 2019
 *
 ******************************************************************************
 */

#ifndef CL712_DEVICE_PRIVATE_H_
#define CL712_NAME_DEVICE_PRIVATE_H_

#include <string.h>
#include "can_service_devices.h"
#include "timer_service.h"

//////////////////////////////////////////////////////////////////////////////
//
// NOTE: There is a required #include file and two function prototypes
// at the end of this head file. This is required for all device
// drivers.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct {
    uint32_t hydraulic_motor_pump_override: 8;
    uint32_t traction_motor_pump_override: 8;
    uint32_t pe_pump_override: 8;
    uint32_t charger_pump_override: 8;
    uint32_t charger_fan_override:8;
    uint32_t pe_fan_override: 8;
    uint32_t unused1: 8;
    uint32_t unused2: 8;
} cl712_rx_message1_t;

typedef struct {
    uint32_t throttle_depressed: 16;
    uint32_t throttle_released: 16;
    uint32_t data3: 16;
    uint32_t data4: 16;
} cl712_rx_message2_t;

typedef struct {
    uint32_t service_time_setting: 32;
    uint32_t data2: 32;
} cl712_rx_message3_t;

//=============================================================================
//
// Main Screen Messages
//
//=============================================================================
//
typedef struct {
    uint32_t odometer_m: 32;
    uint32_t hourmeter_s: 32;
} cl712_tx_main_screen_message1_t;

typedef struct {
    uint32_t time_until_service_s: 32;
    uint32_t groundspeed_cmps: 16;
    uint32_t soc_percent: 8;
    uint32_t gear_direction: 8;
} cl712_tx_main_screen_message2_t;

typedef struct {
    uint32_t gear_ratio: 8;
    uint32_t vehicle_state: 8;
    uint32_t misc_status: 8;
    uint32_t can1_rx_4_spare_1: 8;
    uint32_t yellow_warning_reason: 32;
} cl712_tx_main_screen_message3_t;

typedef struct {
    uint32_t crit_fail_hv_off_reason: 32;
    uint32_t crit_fail_hv_on_reason: 32;
} cl712_tx_main_screen_message4_t;


//=============================================================================
//
// BMS DATA
//
//=============================================================================
//
typedef struct {
    uint32_t battery_1_high_cell_voltage_mv: 16;
    uint32_t battery_1_low_cell_voltage_mv: 16;
    uint32_t battery_1_cell_voltage_delta_mv: 16;
    uint32_t battery_1_pack_voltage_v10: 16;
} cl712_tx_bms_message1_t;

typedef struct {
    uint32_t battery_1_temperature_c: 16;
    uint32_t battery_1_pack_current_v10: 16;
    uint32_t battery_1_isolation_fault_status: 8;
    uint32_t battery_1_precharge_status: 8;
    uint32_t battery_1_positive_contactor_status: 8;
    uint32_t battery_1_negative_contactor_status: 8;
} cl712_tx_bms_message2_t;

typedef struct {
    uint32_t battery_2_high_cell_voltage_mv: 16;
    uint32_t battery_2_low_cell_voltage_mv: 16;
    uint32_t battery_2_cell_voltage_delta_mv: 16;
    uint32_t battery_2_pack_voltage_v10: 16;
} cl712_tx_bms_message3_t;

typedef struct {
    uint32_t battery_2_temperature_c: 16;
    uint32_t battery_2_pack_current_v10: 16;
    uint32_t battery_2_isolation_fault_status: 8;
    uint32_t battery_2_precharge_status: 8;
    uint32_t battery_2_positive_contactor_status: 8;
    uint32_t battery_2_negative_contactor_status: 8;
} cl712_tx_bms_message4_t;

typedef struct {
    uint32_t battery_3_high_cell_voltage_mv: 16;
    uint32_t battery_3_low_cell_voltage_mv: 16;
    uint32_t battery_3_cell_voltage_delta_mv: 16;
    uint32_t battery_3_pack_voltage_v10: 16;
} cl712_tx_bms_message5_t;

typedef struct {
    uint32_t battery_3_temperature_c: 16;
    uint32_t battery_3_pack_current_v10: 16;
    uint32_t battery_3_isolation_fault_status: 8;
    uint32_t battery_3_precharge_status: 8;
    uint32_t battery_3_positive_contactor_status: 8;
    uint32_t battery_3_negative_contactor_status: 8;
} cl712_tx_bms_message6_t;

//=============================================================================
//
// PDM Supply Voltage and STATUS
//
//=============================================================================
//
typedef struct {

    uint32_t pdm_1_supply_voltage_v10: 16;
    uint32_t pdm_2_supply_voltage_v10: 16;
    uint32_t pdm_3_supply_voltage_v10: 16;
    uint32_t pdm_1_status: 8;
    uint32_t pdm_2_status: 8;

} cl712_tx_pdm_voltage_status_message1_t;

typedef struct {
    uint32_t pdm_3_status: 8;
    uint32_t data2: 8;
    uint32_t data3: 8;
    uint32_t data4: 8;
    uint32_t data5: 8;
    uint32_t data6: 8;
    uint32_t data7: 8;
    uint32_t data8: 8;
} cl712_tx_pdm_voltage_status_message2_t;

//=============================================================================
//
// PDM 1
//
//=============================================================================
//
typedef struct {

    uint32_t pdm_1_reverse_lights_status: 8;
    uint32_t pdm_1_brake_lights_status: 8;
    uint32_t pdm_1_deck_lights_status: 8;
    uint32_t pdm_1_brake_release_status: 8;
    uint32_t pdm_1_reverse_beeper_status: 8;
    uint32_t pdm_1_reverse_beacon_status: 8;
    uint32_t pdm_1_right_spray_selenoid_status: 8;
    uint32_t pdm_1_center_spray_selenoid_status: 8;

} cl712_tx_pdm_1_status_message1_t;

typedef struct {
    uint32_t pdm_1_left_spray_selenoid_status: 8;
    uint32_t pdm_1_spare_status: 8;
    uint32_t pdm_1_channel_11_status: 8;
    uint32_t pdm_1_channel_12_status: 8;
    uint32_t pdm_1_reverse_lights_current: 8;
    uint32_t pdm_1_brake_lights_current: 8;
    uint32_t pdm_1_deck_lights_current: 8;
    uint32_t pdm_1_brake_release_current: 8;
} cl712_tx_pdm_1_status_and_current_message2_t;

typedef struct {
    uint32_t pdm_1_reverse_beeper_current: 8;
    uint32_t pdm_1_reverse_beacon_current: 8;
    uint32_t pdm_1_right_spray_selenoid_current: 8;
    uint32_t pdm_1_center_spray_selenoid_current: 8;
    uint32_t pdm_1_left_spray_selenoid_current: 8;
    uint32_t pdm_1_spare_current: 8;
    uint32_t pdm_1_channel_11_current: 8;
    uint32_t pdm_1_channel_12_current: 8;
} cl712_tx_pdm_1_current_message3_t;

//=============================================================================
//
// PDM 2
//
//=============================================================================
//
typedef struct {

    uint32_t     pdm_2_start_drain_selenoid_status: 8;
    uint32_t     pdm_2_aux_hydraulics_12v_bussbar_status: 8;
    uint32_t     pdm_2_gear_1_selenoid_status: 8;
    uint32_t     pdm_2_gear_2_selenoid_status: 8;
    uint32_t     pdm_2_gear_3_selenoid_status: 8;
    uint32_t     pdm_2_gear_4_selenoid_status: 8;
    uint32_t     pdm_2_power_electronics_pump_status: 8;
    uint32_t     pdm_2_selector_selenoid_status: 8;

} cl712_tx_pdm_2_status_message1_t;

typedef struct {
    uint32_t     pdm_2_brake_release_selenoid_status: 8;
    uint32_t     pdm_2_service_brake_selenoid_status: 8;
    uint32_t     pdm_2_hydraulic_motor_cooling_pump_status: 8;
    uint32_t     pdm_2_hydraulic_motor_radiator_cooling_fan_status: 8;
    uint32_t     pdm_2_start_drain_selenoid_current: 8;
    uint32_t     pdm_2_aux_hydraulics_12v_bussbar_current: 8;
    uint32_t     pdm_2_gear_1_selenoid_current: 8;
    uint32_t     pdm_2_gear_2_selenoid_current: 8;
} cl712_tx_pdm_2_status_and_current_message2_t;

typedef struct {
    uint32_t     pdm_2_gear_3_selenoid_current: 8;
    uint32_t     pdm_2_gear_4_selenoid_current: 8;
    uint32_t     pdm_2_power_electronics_pump_current: 8;
    uint32_t     pdm_2_selector_selenoid_current: 8;
    uint32_t     pdm_2_brake_release_selenoid_current: 8;
    uint32_t     pdm_2_service_brake_selenoid_current: 8;
    uint32_t     pdm_2_hydraulic_motor_cooling_pump_current: 8;
    uint32_t     pdm_2_hydraulic_motor_radiator_cooling_fan_current: 8;
} cl712_tx_pdm_2_current_message3_t;

//=============================================================================
//
// PDM 3
//
//=============================================================================
//
typedef struct {
    uint32_t     pdm_3_work_lights_status: 8;
    uint32_t     pdm_3_beacon_status: 8;
    uint32_t     pdm_3_head_light_status: 8;
    uint32_t     pdm_3_horn_status: 8;
    uint32_t     pdm_3_charger_1_bussbar_status: 8;
    uint32_t     pdm_3_drive_motor_cooling_pump_status: 8;
    uint32_t     pdm_3_imt_cooling_pump_status: 8;
    uint32_t     pdm_3_fan_radiator_1_2_status: 8;
} cl712_tx_pdm_3_status_message1_t;

typedef struct {
    uint32_t     pdm_3_pump_charger_1_cooling_status: 8;
    uint32_t     pdm_3_pump_charger_2_cooling_status: 8;
    uint32_t     pdm_3_main_bussbar_supply_status: 8;
    uint32_t     pdm_3_hydraulic_fan_selenoid_status: 8;
    uint32_t     pdm_3_work_lights_current: 8;
    uint32_t     pdm_3_beacon_current: 8;
    uint32_t     pdm_3_head_light_current: 8;
    uint32_t     pdm_3_horn_current: 8;
} cl712_tx_pdm_3_status_and_current_message2_t;

typedef struct {
    uint32_t     pdm_3_charger_1_bussbar_current: 8;
    uint32_t     pdm_3_drive_motor_cooling_pump_current: 8;
    uint32_t     pdm_3_imt_cooling_pump_current: 8;
    uint32_t     pdm_3_fan_radiator_1_2_current: 8;
    uint32_t     pdm_3_pump_charger_1_cooling_current: 8;
    uint32_t     pdm_3_pump_charger_2_cooling_current: 8;
    uint32_t     pdm_3_main_bussbar_supply_current: 8;
    uint32_t     pdm_3_hydraulic_fan_selenoid_current: 8;
} cl712_tx_pdm_3_current_message3_t;

//=============================================================================
//
// Pump and Fan Status
//
//=============================================================================
//
typedef struct {
    uint32_t traction_pump_status: 8;
    uint32_t hydraulic_pump_status: 8;
    uint32_t pe_fan_status: 8;
    uint32_t pe_pump_status: 8;
    uint32_t charger_fan_status: 8;
    uint32_t charger_pump_status: 8;
    uint32_t unused: 16;
} cl712_tx_pump_fan_status;


//=============================================================================
//
// Traction Drive Inverter
//
//=============================================================================
//
typedef struct {
    uint32_t drive_dc_voltage_v10: 16;
    uint32_t drive_motor_rpm: 16;
    uint32_t drive_throttle_percentage: 8;
    uint32_t drive_inverter_pcb_temp_c: 8;
    uint32_t drive_inverter_temp_c: 8;
    uint32_t drive_motor_temp_c: 8;
} cl712_tx_traction_drive_message1_t;

typedef struct {
    uint32_t throttle_adc_raw: 16;
    uint32_t throttle_released: 16;
    uint32_t throttle_depressed: 16;
    uint32_t unused1: 16;
} cl712_tx_traction_drive_message2_t;

//=============================================================================
//
// Hydraulic Drive Inverter
//
//=============================================================================
//
typedef struct {
    uint32_t    ehs_dc_voltage_v10: 16;
    uint32_t    ehs_motor_rpm: 16;
    uint32_t    ehs_inverter_pcb_temp_c: 8;
    uint32_t    ehs_inverter_temp_c: 8;
    uint32_t    ehs_motor_temp_c: 8;
    uint32_t    can1_rx_25_spare_1: 8;
} cl712_tx_hydraulic_drive_message1_t;

//=============================================================================
//
// System Pressures
//
//=============================================================================
//
typedef struct {
    uint32_t    main_pressure_psi: 16;
    uint32_t    front_brake_pressure_psi: 16;
    uint32_t    rear_brake_pressure_psi: 16;
    uint32_t    driveline_pressure_psi: 16;
}cl712_tx_system_pressure_message1_t;

typedef struct {
    uint32_t    front_parking_brake_psi: 16;
    uint32_t    front_accumulator_pressure_psi: 16;
    uint32_t    rear_accumulator_pressure_psi: 16;
    uint32_t    transmission_pressure_10x_psi: 16;
}cl712_tx_system_pressure_message2_t;

//=============================================================================
//
// Pump and Fan OVERRIDE Status
//
//=============================================================================
//
typedef struct {
    uint32_t traction_pump_override_status: 8;
    uint32_t hydraulic_pump_override_status: 8;
    uint32_t pe_fan_override_status: 8;
    uint32_t pe_pump_override_status: 8;
    uint32_t charger_fan_override_status: 8;
    uint32_t charger_pump_override_status: 8;
    uint32_t unused: 16;
} cl712_tx_pump_fan_override_status;

//
// A compound structure that encompasses all the RX CAN messages
// related to the Bel charger. One of these is allocated for each
// instantiation of a Bel Charger.
//
typedef struct device_data_s {
    struct device_data_s *next_ptr;
    uint8_t device_instance_number;

    // These are tx message data buffers
    cl712_tx_main_screen_message1_t tx_message1;
    cl712_tx_main_screen_message2_t tx_message2;
    cl712_tx_main_screen_message3_t tx_message3;
    cl712_tx_main_screen_message4_t tx_message4;

    cl712_tx_bms_message1_t tx_message5;
    cl712_tx_bms_message2_t tx_message6;
    cl712_tx_bms_message3_t tx_message7;
    cl712_tx_bms_message4_t tx_message8;
    cl712_tx_bms_message5_t tx_message9;
    cl712_tx_bms_message6_t tx_message10;

    cl712_tx_pdm_voltage_status_message1_t tx_message11;
    cl712_tx_pdm_voltage_status_message2_t tx_message12;

    cl712_tx_pdm_1_status_message1_t tx_message13;
    cl712_tx_pdm_1_status_and_current_message2_t tx_message14;
    cl712_tx_pdm_1_current_message3_t tx_message15;

    cl712_tx_pdm_2_status_message1_t tx_message16;
    cl712_tx_pdm_2_status_and_current_message2_t tx_message17;
    cl712_tx_pdm_2_current_message3_t tx_message18;

    cl712_tx_pdm_3_status_message1_t tx_message19;
    cl712_tx_pdm_3_status_and_current_message2_t tx_message20;
    cl712_tx_pdm_3_current_message3_t tx_message21;

    cl712_tx_traction_drive_message1_t tx_message22;
    cl712_tx_traction_drive_message2_t tx_message23;

    cl712_tx_hydraulic_drive_message1_t tx_message24;

    cl712_tx_system_pressure_message1_t tx_message25;
    cl712_tx_system_pressure_message2_t tx_message26;

    cl712_tx_pump_fan_status tx_message27;
    cl712_tx_pump_fan_override_status tx_message28;

    // These are pointers to CAN tx registration records
    // Main Screen Elements
    can_tx_registration_t *send_tx_data1_can_ptr;
    can_tx_registration_t *send_tx_data2_can_ptr;
    can_tx_registration_t *send_tx_data3_can_ptr;
    can_tx_registration_t *send_tx_data4_can_ptr;
    can_tx_registration_t *send_tx_data5_can_ptr;
    can_tx_registration_t *send_tx_data6_can_ptr;
    can_tx_registration_t *send_tx_data7_can_ptr;
    can_tx_registration_t *send_tx_data8_can_ptr;
    can_tx_registration_t *send_tx_data9_can_ptr;
    can_tx_registration_t *send_tx_data10_can_ptr;
    can_tx_registration_t *send_tx_data11_can_ptr;
    can_tx_registration_t *send_tx_data12_can_ptr;
    can_tx_registration_t *send_tx_data13_can_ptr;
    can_tx_registration_t *send_tx_data14_can_ptr;
    can_tx_registration_t *send_tx_data15_can_ptr;
    can_tx_registration_t *send_tx_data16_can_ptr;
    can_tx_registration_t *send_tx_data17_can_ptr;
    can_tx_registration_t *send_tx_data18_can_ptr;
    can_tx_registration_t *send_tx_data19_can_ptr;
    can_tx_registration_t *send_tx_data20_can_ptr;
    can_tx_registration_t *send_tx_data21_can_ptr;
    can_tx_registration_t *send_tx_data22_can_ptr;
    can_tx_registration_t *send_tx_data23_can_ptr;
    can_tx_registration_t *send_tx_data24_can_ptr;
    can_tx_registration_t *send_tx_data25_can_ptr;
    can_tx_registration_t *send_tx_data26_can_ptr;
    can_tx_registration_t *send_tx_data27_can_ptr;
    can_tx_registration_t *send_tx_data28_can_ptr;

    // Each of the SEVCON CAN receive functions sets bool associated with
    // itself to TRUE when its message is received.
    cl712_rx_message1_t rx_message_1;
    cl712_rx_message2_t rx_message_2;
    cl712_rx_message3_t rx_message_3;

    bool_t rx_message_1_ok;
    bool_t rx_message_2_ok;
    bool_t rx_message_3_ok;

} device_data_t;

//////////////////////////////////////////////////////////////////////////////
//
// Note: These two include files must be located here. This is the
// structure we are following with the idea that all of these device
// drivers will be built into a libray.
//
//////////////////////////////////////////////////////////////////////////////
//
#include "cl712_device.h"
#include "Prototypes_Time.h"


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


#endif


