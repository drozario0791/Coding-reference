/******************************************************************************
 *
 *        Name: state_machine.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Saturday, 07 September 2019
 *
 ******************************************************************************
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

//=============================================================================
//
// A struct whose member elements keep track of HED related errors
//
//=============================================================================
//
typedef struct
{
	uint16_t						firmware_version;
	uint16_t						application_version;
	uint8_t							number_of_system_errors;
	System_Error_t					system_error;
	CONDUCTOR_MODES_				conductor_mode;
	Orch_NV_Access_Levels_t			conductor_access_level;
	CAN_STATUS_						can1_status;
	CAN_STATUS_						can2_status;
	CAN_STATUS_						can3_status;
	Can_Baud_Rate_					can1_baudrate;
	Can_Baud_Rate_					can2_baudrate;
	Can_Baud_Rate_					can3_baudrate;
	Can_Missing_Message_Status_t	can1_missed_status;
	Can_Missing_Message_Status_t	can2_missed_status;
	Can_Missing_Message_Status_t	can3_missed_status;
	MODULE_STATE_					master_module_state;
	bool_t							io_modules_in_running_state;
	bool_t							in_safe_mode;
} hed_system_status_t;

 //
 // These are all the states that the vehicle can be in
 //
typedef enum
{

	ZERO_ENERGY,                        //base state, at startup or shutdown                                        0x00
	MASTER_CLOSED_NO_HV,                //right after master has been closed                                        0x01
	STARTUP,                            //aux systems on, right before precharge                                    0x02
	CHARGING,                           //run the charger                                                           0x03
	POST_CHARGE_IDLE,                   //vehicle has been unplugged and stays on                                   0x04
	BRAKE_TEST_MODE,                    //pre-op brake test                                                         0x05
	DIAGNOSTIC_MODE,                    //aux systems on, hv off                                                    0x06
	READY_TO_DRIVE,                     //normal operation                                                          0x07
	TOW_MODE,                           //safe mode to tow the vehicle                                              0x08
	DEAD_BATTERY,                       //battery is too low for operation                                          0x09
	CRITICAL_FAILURE_HV_OFF,            //critical failure opening contactors and shutting vehicle down             0x0A
	CRITICAL_FAILURE_HV_ON,             //critical failure leaving contactors closed and disabling drive system     0x0B
	E_STOP,                             //e-stop has been pressed and vehicle is still on                           0x0C
	SHUTDOWN,                           //vehicle is shutting down                                                  0x0D
	NUM_STATES                          //an enumeration that will automatically be equal to the number of states   0x0E

} state_t;

 //
 // It is expected that the programmer will make sure all these values
 // are properly filled in and passed into the state machine for proper
 // operation.
 //
typedef struct
{

    //
	// Begin booleans that can be set at any time by the user drive
	// system
    //
    bool_t		master_closed;
	bool_t		e_stop;
	bool_t		ignition_on;
	bool_t		plugged_in;
	bool_t		in_neutral;
	uint32_t	critical_fault_hv_off;
	uint32_t	critical_fault_hv_on;
	bool_t		stop_charging_button;
	bool_t		brake_test_mode;
	bool_t		diagnostic_mode;
    bool_t      exit_post_charge_idle;

	// End
	uint32_t	max_cell_voltage_uv;
	uint32_t	min_cell_voltage_uv;
	uint8_t		battery_soc;
	uint16_t	dc_bus_voltage_v;
	uint16_t	battery_voltage_v;
	int16_t		traction_rpm;
	uint32_t	setting_max_charging_cell_voltage_uv;
} state_machine_input_data_t;

//
// The state of the contactors, as desired by the state machine.
//
typedef struct
{
	bool_t	pos_contactor;
	bool_t	neg_contactor;
	bool_t	pre_contactor;
	bool_t	precharge_failure;
	bool_t	precharge_success;
} state_machine_contactor_status_t;

/*
 * The data output by the state machine, used to control the rest of
 * the system.
 */
typedef struct
{
	state_t								current_state;
	bool_t								brakes_desired;
	bool_t								charging_desired;
	bool_t                              enable_hv_systems;
	float								charge_current_factor;
	state_machine_contactor_status_t	contactors;
} state_machine_output_data_t;

//
// Type definition: state_function_t can now be used as a type
// (pointer to a function that takes data and returns a state)
//
typedef state_t (*state_function_t)(const state_machine_input_data_t *input_data,
	state_machine_output_data_t *output_data);
//
// Type definition: transition_function: future
// use. transition_function_t can now be used as a type (pointer to a
// function that takes *data and returns void)
//
typedef void (*transition_function_t)(const state_machine_input_data_t *data);

//
// This function is exptected to be called by the controlling software
// every loop all input data should be properly populated prior to
// passing it to this function
//
void run_state_machine();

//=============================================================================
//
// check_critical_fault_high_voltage_off()
//
//=============================================================================
//
bool_t check_critical_fault_high_voltage_off();

//=============================================================================
//
// battery_pack_failure_status()
//
//=============================================================================
//
bool_t battery_pack_failure_status();

//=============================================================================
//
// check_critical_fault_high_voltage_on()
//
//=============================================================================
//
bool_t check_critical_fault_high_voltage_on();

//=============================================================================
//
// HED System Status
//
//=============================================================================
//
hed_system_status_t get_system_status();

//=============================================================================
//
// Populate State Machine
//
//=============================================================================
//
void
populate_state_machine_member_elements();

//=============================================================================
//
// Getters of the State Machine
//
//=============================================================================
//
state_t get_sm_current_state();
bool_t get_sm_status_brakes_desired();
bool_t get_sm_status_charging_desired();
bool_t get_sm_status_enable_hv_systems();
float get_sm_charge_current_factor();
bool_t get_sm_pos_contactor_status();
bool_t get_sm_neg_contactor_status();
bool_t get_sm_pre_contactor_status();
bool_t get_sm_precharge_failure_status();
bool_t get_sm_precharge_success_status();

uint32_t get_critical_fault_hv_off();
uint32_t get_critical_fault_hv_on();
uint32_t yellow_warning_message();

void set_state_machine_state(state_t state);
void set_positive_contactor(bool_t state);
void set_negative_contactor(bool_t state);
void set_pre_contactor(bool_t state);

#endif // STATE_MACHINE_H_
