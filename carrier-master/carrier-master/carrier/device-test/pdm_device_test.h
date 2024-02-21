/******************************************************************************
 *
 *        Name: pdm_device_test.h
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Thursday, 25 July 2019
 *
 ******************************************************************************
 */
#ifndef PDM_DEVICE_TEST_H_
#define PDM_DEVICE_TEST_H_

//
// Public function prototypes.
//
void pdm_test_init(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line);

void pdm_test_set_timeout(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line);

void pdm_test_configure_output_function_all_channels(
    device_instances_t device);

void pdm_test_configure_all_output_channels(
    device_instances_t device);

void pdm_test_command_all_output_channel_leds_on(
    device_instances_t device);

void pdm_test_command_valve_off(
    device_instances_t device);

void pdm_test_command_valve_on(
    device_instances_t device);


#endif // PDM_DEVICE_TEST_H_
