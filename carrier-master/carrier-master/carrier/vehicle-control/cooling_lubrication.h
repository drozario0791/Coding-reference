/******************************************************************************
 *
 *        Name: cooling_lubrication.h
 *
 * Description:
 *
 *        Date: Wednesday, 21 August 2019
 *
 ******************************************************************************
 */


//=============================================================================
//
// Defines
//
//=============================================================================
//

// Traction Motor Temperature Thresholds
#define MAX_DRIVE_MOTOR_TEMPERATURE_THRESHOLD_1        75
#define MAX_DRIVE_MOTOR_TEMPERATURE_THRESHOLD_2        90

// Hydraulic Motor Temperature Thresholds
#define MAX_HYDRAULIC_MOTOR_TEMPERATURE_THRESHOLD_1    75
#define MAX_HYDRAULIC_MOTOR_TEMPERATURE_THRESHOLD_2    90

// Traction Inverter Temperature Thresholds
#define MAX_TRACTION_INVERTER_TEMPERATURE_THRESHOLD_1  65
#define MAX_TRACTION_INVERTER_TEMPERATURE_THRESHOLD_2  75

// Hydraulic Inverter Temperature Thresholds
#define MAX_HYDRAULIC_INVERTER_TEMPERATURE_THRESHOLD_1 65
#define MAX_HYDRAULIC_INVERTER_TEMPERATURE_THRESHOLD_2 75

// DC_DC Temperature Threshold
#define MAX_DC_DC_TEMPERATURE_THRESHOLD                50

// Bel Charger Temperature Thresholds
#define MAX_CHARGER_THRESHOLD_1                        60
#define MAX_CHARGER_THRESHOLD_2                        80


//=============================================================================
//
// cooling_and_lubcrication_control()
//
//=============================================================================
//
void cooling_and_lubcrication_control();

//=============================================================================
//
// get_cooling_device_state_of_health()
//
//=============================================================================
//
bool_t get_cooling_device_state_of_health();
