/******************************************************************************
 *
 *        Name: hydraulic_control.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Wednesday, 28 August 2019
 *
 ******************************************************************************
 */

#ifndef HYDRAULIC_CONTROL_H_
#define HYDRAULIC_CONTROL_H_

typedef enum
{
    BAD     = -1,
    WARNING = 0,
    GOOD    = 1,
} hydraulic_system_pressure_t;



//=============================================================================
//
// A function that control the working of the hydraulic system
// Hydraulic Inverter, Start Drain Solenoid, Tranmsission Inverter.
//
//=============================================================================
//
void hydraulic_system_control(device_instances_t device_hydraulic_inverter,
                              device_instances_t device_transmission_inverter);

bool_t brake_control();



//=============================================================================
//
// Getters
//
//=============================================================================
//
uint16_t get_rear_accum_pressure();
uint16_t get_transmission_pressure();
uint16_t get_front_axil_parking_brake_pressure();
uint16_t get_rear_service_brake();
uint16_t get_rear_service_brake_pressure();
uint16_t get_drive_line_brake_pressure();
uint16_t get_front_accum_pressure();
uint16_t get_front_service_brake_pressure();
uint16_t main_sys_pressure();

uint16_t convert_adc_to_psi(uint16_t raw_adc_value);


#endif
