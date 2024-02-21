/******************************************************************************
 *
 *        Name: pku2400_device.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Wednesday, 07 August 2019
 *
 ******************************************************************************
 */
#ifndef PKU2400_DEVICE_H_
#define PKU2400_DEVICE_H_

// Delete after testing
void test_button_state(bool_t state);

/******************************************************************************
 *
 * Function Prototype for the handler functions
 *
 ******************************************************************************
 */
typedef void (*p_can_buttons_f_t)(bool_t state);


/******************************************************************************
 *
 * A CAN switch can work be configured either as a momentary switch
 * switch or a toggle switch.
 *
 ******************************************************************************
 */
typedef enum {
    MOMENTARY_SWITCH,
    TOGGLE_SWITCH,
} can_button_config_t;

/******************************************************************************
 *
 * The PKU2400 can switches have eight CAN switches.
 *
 *
 ******************************************************************************
 */
typedef enum
{
    SWITCH_TOP_1_BIT = 0,
    SWITCH_TOP_2_BIT = 1,
    SWITCH_TOP_3_BIT = 2,
    SWITCH_TOP_4_BIT = 3,
    SWITCH_BOTTOM_1_BIT = 4,
    SWITCH_BOTTOM_2_BIT = 5,
    SWITCH_BOTTOM_3_BIT = 6,
    SWITCH_BOTTOM_4_BIT = 7,
    PKU_SWITCH_CNT,
} can_switch_t;


/******************************************************************************
 *
 * The avaliable colours on the PKU2400
 *
 *
 ******************************************************************************
 */
typedef enum
{
    OFF = 0,
    RED = 1,
    BLUE = 2,
    GREEN = 3,
    BROWN = 4,
} can_switch_colour_t;


/******************************************************************************
 *
 * Enable or disable the LEDs on the module
 *
 *
 ******************************************************************************
 */
typedef enum
{
    LED_ON = 1,
    LED_OFF = 0,
} led_state_t;


/******************************************************************************
 *
 * A function to initialise a CAN switch. Used to initialise the a CAN
 * switches. The function takes the name of CAN switch, along with the
 * desired can_switch_configuration, the can line the switch is
 * present and the function to be called when a state change occurs in
 * the switch.
 *
 *
 ******************************************************************************
 */
bool_t
pku2400_configure_switches(
    device_instances_t device_instance,
    can_button_config_t button_config,
    can_switch_t can_switch_name,
    p_can_buttons_f_t handler_function_p);


/******************************************************************************
 *
 * pku2400_check_switches(device_instances_t device)
 *
 * A function that walks the list of can switch registeration records
 * and calls the respective handler function if a state change had
 * occured.
 *
 ******************************************************************************
 */
void pku2400_check_switches(device_instances_t device);


/******************************************************************************
 *
 * A function to set the initial state of all the LED's on the module to
 * a particular state
 *
 ******************************************************************************
 */
void pku2400_set_led_initial_state(
    device_instances_t device,
    led_state_t state);


/******************************************************************************
 *
 * Public initialization function for PKU2400 CAN Switches
 *
 *
 *
 ******************************************************************************
 */
void pku2400_init(device_instances_t device,
                          uint8_t module_id,
                          CANLINE_ can_line);

/******************************************************************************
 *
 *  These functions transmit the registered module CAN
 *  messages. Use the setter functions to populate the structure data
 * that each one of these refers to.
 * Public initialization function for PKU2400 CAN Switches
 *
 ******************************************************************************
 */
void pku2400_tx_led_controls(
    device_instances_t device,
    can_rate_t transmit_counter_limit);


/******************************************************************************
 *
 * Public functions to set the timeout counter limit for each of the
 * can switches receive CAN messages.
 *
 *
 *
 ******************************************************************************
 */
void pku2400_set_timeout_switch_status(
    device_instances_t device,
    uint8_t module_id,
    CANLINE_ can_line,
    can_rate_t receive_timeout_counter_limit);

/******************************************************************************
 *
 * Getter Functions for the pku2400 device
 *
 *
 *
 ******************************************************************************
 */

bool_t pku2400_get_status_momentary_input_state(
    device_instances_t device,
    can_switch_t can_switch_number);

bool_t pku2400_get_status_switch_toggle_state(
    device_instances_t device,
    can_switch_t can_switch_number);

bool_t pku2400_switch_state(
    bool_t input,
    bool_t previous_state);

bool_t pku2400_get_switch_previous_state(
    device_instances_t device,
    can_switch_t can_switch_number);

bool_t pku2400_get_switch_toggled_state(
    device_instances_t device,
    can_switch_t can_switch_name);

/******************************************************************************
 *
 * Setter functions for the pku2400 device
 *
 *
 *
 ******************************************************************************
 */
//=============================================================================
//
// A function to change the colour of an individual switch
//
//=============================================================================
//

void pku2400_set_change_led_colour(
    device_instances_t device,
    can_switch_t switch_name,
    can_switch_colour_t colour);


void pku2400_set_switch_toggled_state(
    device_instances_t device,
    can_switch_t can_switch_name,
    bool_t state);

/******************************************************************************
 *
 * A helper function to TOGGLE any led whose state has changed when
 * the LED is configured as a TOGGLE SWITCH
 *
 ******************************************************************************
 */

void pku2400_toggle_led_based_on_state(device_instances_t device);

void pku2400_set_all_leds_to_off_state(device_instances_t device);

/******************************************************************************
 *
 * Lower level helper function that control each LED individually
 *
 ******************************************************************************
 */

void pku2400_set_led_colour_top_switch_1(
	device_instances_t device, can_switch_colour_t colour, led_state_t state);

void pku2400_set_led_colour_top_switch_2(
	device_instances_t device, can_switch_colour_t colour, led_state_t state);

void pku2400_set_led_colour_top_switch_3(
	device_instances_t device, can_switch_colour_t colour, led_state_t state);

void pku2400_set_led_colour_top_switch_4(
	device_instances_t device, can_switch_colour_t colour, led_state_t state);

void pku2400_set_led_colour_bottom_switch_1(
	device_instances_t device, can_switch_colour_t colour, led_state_t state);

void pku2400_set_led_colour_bottom_switch_2(
    device_instances_t device, can_switch_colour_t colour, led_state_t state);

void pku2400_set_led_colour_bottom_switch_3(
	device_instances_t device, can_switch_colour_t colour, led_state_t state);

void pku2400_set_led_colour_bottom_switch_4(
	device_instances_t device, can_switch_colour_t colour, led_state_t state);

#endif
