/******************************************************************************
 *
 *        Name:
 *
 * Description:
 *
 *      Author: Tom
 *        Date: Wednesday, 14 August 2019
 *
 ******************************************************************************
 */

#include "Prototypes.h"
#include "Constants.h"
#include "Prototypes_CAN.h"
#include "can_service.h"
#include "pdm_device.h"
#include "pdm_control_1.h"
#include "pku2400_device.h"
#include "pku2400_control_1.h"

#include "can_switches.h"

device_instances_t pku_device_one = ONE;

//
// Local function prototypes.
//
// This is the default switch state change function.
//
void dummy(bool_t state);


/******************************************************************************
 *
 *        Name: pku2400_configure_switches()
 *
 * Description: Configure each of the PKU switches. The configuration
 *              for each switch sets the TOGGLE or MOMENTARY setting
 *              and the function to call when the switch state
 *              changes.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void pku2400_control_init_1()
{
    //
    // The rear deck of the carrier supports an optional sprayer
    // unit. The rear deck PDM has the ability to detect if the
    // sprayer unit is attached. When the sprayer is attached, three
    // of the pku switches control the state of each of the three
    // sprayers. Configure each of the three switches as toggle
    // switches.
    //
    pku2400_configure_switches(
        pku_device_one,
        TOGGLE_SWITCH,
        SWITCH_BOTTOM_1_BIT,
        bottom_switch_1);

    pku2400_configure_switches(
        pku_device_one,
        TOGGLE_SWITCH,
        SWITCH_BOTTOM_2_BIT,
        bottom_switch_2);

    pku2400_configure_switches(
        pku_device_one,
        TOGGLE_SWITCH,
        SWITCH_BOTTOM_3_BIT,
        bottom_switch_3);

    pku2400_configure_switches(
        pku_device_one,
        TOGGLE_SWITCH,
        SWITCH_BOTTOM_4_BIT,
        bottom_switch_4);

    pku2400_configure_switches(
        pku_device_one,
        TOGGLE_SWITCH,
        SWITCH_TOP_1_BIT,
        top_switch_1);

    pku2400_configure_switches(
        pku_device_one,
        TOGGLE_SWITCH,
        SWITCH_TOP_2_BIT,
        top_switch_2);

    pku2400_configure_switches(
        pku_device_one,
        TOGGLE_SWITCH,
        SWITCH_TOP_3_BIT,
        top_switch_3);

    pku2400_configure_switches(
        pku_device_one,
        MOMENTARY_SWITCH,
        SWITCH_TOP_4_BIT,
        top_switch_4);


    //
    // Set all of the back light LEDs to ORANGE (BROWN). The LED
    // back lights will switch to GREEN when the momentary switch is
    // pressed or when the toggle switch is ON.
    //
    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_BOTTOM_1_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_BOTTOM_2_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_BOTTOM_3_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_BOTTOM_4_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_TOP_1_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_TOP_2_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_TOP_3_BIT,
        BROWN);

    pku2400_set_change_led_colour(
        pku_device_one,
        SWITCH_TOP_4_BIT,
        BROWN);

    //
    // Transmit the change LED colour command.
    //
    pku2400_tx_led_controls(pku_device_one, TX_SEND_EACH_CALL);

}
