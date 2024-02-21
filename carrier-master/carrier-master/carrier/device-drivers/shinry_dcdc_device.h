/******************************************************************************
 *
 *        Name: shinry_dcdc_device.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 30 July 2019
 *
 ******************************************************************************
 */

#ifndef SHINRY_DCDC_DEVICE_H_
#define SHINRY_DCDC_DEVICE_H_


typedef enum
{
    DCDC_10V_OUTPUT_VOLTAGE = 10,
    DCDC_11V_OUTPUT_VOLTAGE = 11,
    DCDC_12V_OUTPUT_VOLTAGE = 12,
    DCDC_13V_OUTPUT_VOLTAGE = 13,
    DCDC_14V_OUTPUT_VOLTAGE = 14,
} dcdc_output_voltage_t;


typedef enum
{
    STANDBY = 0,
    WORKING = 1,
    FAILURE = 2,
    RESERVED = 3,
} dcdc_mode_t;

/******************************************************************************
 *
 * Description: DCDC Initialisation Functions
 *
 ******************************************************************************
 */
void init_shinry_dcdc(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    uint8_t dcdc_enable_pin);


/******************************************************************************
 *
 * Description: DCDC Transmit Messages
 *
 *
 ******************************************************************************
 */
void tx_shinry_dcdc_control(
    device_instances_t device,
    can_rate_t transmit_counter_limit);


/******************************************************************************
 *
 * Description: Public Setter Function
 *
 *
 ******************************************************************************
 */
void shinry_set_dcdc_control(
    device_instances_t device,
    float vcu_dcdc_voltage_req,
    bool_t dcdc_enable);


//=============================================================================
//
// SET Rx time out count
//
//=============================================================================
//
void shinry_dcdc_set_rx_timeout_contol(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);



/******************************************************************************
 *
 * Description: Public Getter Function
 *
 *
 ******************************************************************************
 */


/******************************************************************************
 * Description: Controls the enable signal to Pin3 on connector.
 ******************************************************************************
 */
void
shinry_dcdc_enable_signal_to_pin3_on_connector(
    device_instances_t device, uint16_t state);


/******************************************************************************
 * Description: Returns the software version.
 *              if Changed, the version is incremented 1 from 1 everytime.
 ******************************************************************************
 */
uint8_t
shinry_get_software_version(
    device_instances_t device);


/******************************************************************************
 * Description: Returns the input voltage to the DC-Dc converter.
 *              min value: 0
 *              max value: 1024
 ******************************************************************************
 */
uint16_t
shinry_get_instantaneous_input_voltage(
    device_instances_t device);


/******************************************************************************
 * Description: Returns the output voltage from the DC-Dc converter.
 *              min value: 0
 *              max value: 50
 ******************************************************************************
 */
uint16_t
shinry_get_instantaneous_output_voltage(
    device_instances_t device);


/******************************************************************************
 * Description: Returns the instantaneous temperature of the DC-Dc converter.
 *              min value: -40
 *              max value: 120
 ******************************************************************************
 */
uint16_t
shinry_get_instantaneous_temperature(
    device_instances_t device);


/******************************************************************************
 * Description: Returns the output current from the DCDC converter.
 *              min value: 0
 *              max value: 250
 ******************************************************************************
 */
uint16_t
shinry_get_instantaneous_current_voltage(
    device_instances_t device);


/******************************************************************************
 * Description: returns true if a over current fault has occured.
 ******************************************************************************
 */
bool_t
shinry_get_over_current_fault(
    device_instances_t device);


/******************************************************************************
 * Description: returns true if a over temperature fault has occured.
 ******************************************************************************
 */
bool_t
shinry_get_over_temperature_fault(
    device_instances_t device);


/******************************************************************************
 * Description: returns true if the dcdc has derated its output power.
 ******************************************************************************
 */
bool_t
shinry_get_derating(
    device_instances_t device);


/******************************************************************************
 *
 * Description: returns the operation mode of the shinry.
 *
 ******************************************************************************
 */
dcdc_mode_t shinry_get_operation_mode(device_instances_t device);

/******************************************************************************
 * Description: returns true if can comminucations is lost.
 ******************************************************************************
 */
bool_t shinry_get_can_cmd_lost(device_instances_t device);

/******************************************************************************
 * Description: returns true if fault occured.
 ******************************************************************************
 */
bool_t shinry_get_input_over_voltage(device_instances_t device);

/******************************************************************************
 * Description: returns true if fault occured.
 ******************************************************************************
 */
bool_t shinry_get_input_under_voltage(device_instances_t device);

/******************************************************************************
 * Description: returns true if fault occured.
 ******************************************************************************
 */
bool_t shinry_get_output_over_voltage(device_instances_t device);

/******************************************************************************
 * Description: returns true if fault occured.
 ******************************************************************************
 */
bool_t shinry_get_output_under_voltage(device_instances_t device);

bool_t shinry_get_can_rx_ok(device_instances_t device);





#endif // SHINRY_DCDC_DEVICE_H_
