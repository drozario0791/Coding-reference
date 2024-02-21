#include <stdio.h>
#include <math.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "typedefs.h"
#include "can_service.h"
#include "pku2400_device.h"
#include "can_buttons_test.h"

/******************************************************************************
 *
 *        Name: test_can_button_leds
 *
 * Description: A test to light up the can buttons in a particular
 *              fashion. This is also used to verify the program in
 *              the can buttons. Also verifies the transmit structure
 *              of the device driver.
 *
 *              TEST PASSES
 *
 *      Author: Deepak
 *        Date: Thursday, 08 August 2019
 *
 ******************************************************************************
 */
void test_can_button_leds(device_instances_t device)
{
    int firsttime = 0;

    set_led_colour_top_switch_1(device, GREEN, LED_ON);
    set_led_colour_top_switch_2(device, RED, LED_ON);
    set_led_colour_top_switch_3(device, GREEN, LED_ON);
    set_led_colour_top_switch_4(device, RED, LED_ON);

    set_led_colour_bottom_switch_1(device, GREEN, LED_ON);
    set_led_colour_bottom_switch_2(device, RED, LED_ON);
    set_led_colour_bottom_switch_3(device, GREEN, LED_ON);
    set_led_colour_bottom_switch_4(device, RED, LED_ON);

    pku2400_tx_led_controls(device, EVERY_2S);

}

/******************************************************************************
 *
 *        Name: test_rx_messages_blink_parallel_led()
 *
 * Description: A function to blink the parallel led of the switch pressed.
 *              This function was written as Deepak had a problem with turning
 *              a particular switch on.
 *
 *              This function verifies the get_status_switch_toggle_state(device, switch_top_1)
 *
 *      Author: Tom
 *        Date: Friday, 09 August 2019
 *
 ******************************************************************************
 */
void test_rx_messages_blink_parallel_led(device_instances_t device)
{
    bool_t top1 = get_status_switch_toggle_state(device, SWITCH_TOP_1);
    bool_t top2 = get_status_switch_toggle_state(device, SWITCH_TOP_2);
    bool_t top3 = get_status_switch_toggle_state(device, SWITCH_TOP_3);
    bool_t top4 = get_status_switch_toggle_state(device, SWITCH_TOP_4);

    bool_t bottom1 = get_status_switch_toggle_state(device, SWITCH_BOTTOM_1);
    bool_t bottom2 = get_status_switch_toggle_state(device, SWITCH_BOTTOM_2);
    bool_t bottom3 = get_status_switch_toggle_state(device, SWITCH_BOTTOM_3);
    bool_t bottom4 = get_status_switch_toggle_state(device, SWITCH_BOTTOM_4);

    if(top1)
    {
    		set_led_colour_bottom_switch_1(device, GREEN, LED_ON);
    		set_led_colour_bottom_switch_1(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }
    if(top1 == 0)
    {
    	set_led_colour_bottom_switch_1(device, GREEN, LED_OFF);
    	set_led_colour_bottom_switch_1(device, RED, LED_ON);
    }

    if(top2)
    {
    		set_led_colour_bottom_switch_2(device, GREEN, LED_ON);
    		set_led_colour_bottom_switch_2(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }

    if(top3)
    {
    		set_led_colour_bottom_switch_3(device, GREEN, LED_ON);
    		set_led_colour_bottom_switch_3(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }

    if(top4)
    {
    		set_led_colour_bottom_switch_4(device, GREEN, LED_ON);
    		set_led_colour_bottom_switch_4(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }

    if(bottom1)
    {
    		set_led_colour_top_switch_1(device, GREEN, LED_ON);
    		set_led_colour_top_switch_1(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }

    if(bottom2)
    {
    		set_led_colour_top_switch_2(device, GREEN, LED_ON);
    		set_led_colour_top_switch_2(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }

    if(bottom3)
    {
    		set_led_colour_top_switch_3(device, GREEN, LED_ON);
    		set_led_colour_top_switch_3(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }

    if(bottom4)
    {
    		set_led_colour_top_switch_4(device, GREEN, LED_ON);
    		set_led_colour_top_switch_4(device, RED, LED_OFF);
    		pku2400_tx_led_controls(device, EVERY_50MS);
    }

}
