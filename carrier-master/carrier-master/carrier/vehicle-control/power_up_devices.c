/******************************************************************************
 *
 *        Name: power_up_devices()
 *
 * Description: A module that controls power to the inverters and DCDC
 *              on the carrier.
 *
 *      Author: Deepak
 *        Date: Monday, 16 September 2019
 *
 ******************************************************************************
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "typedefs.h"
#include "can_service.h"
#include "cvc_input_control.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "power_up_devices.h"
#include "state_machine.h"

/******************************************************************************
 *
 *        Name:
 *
 * Description: The goal is to power the devices the
 *              skai2_hydraulic_inverter, the chargers and the sevcon
 *              once precharge is complete. This is done to reduce the
 *              draw on the 12V battery. Therefore before precharge,
 *              only the traction inverter is powered.
 *
 *        Date: Monday, 16 September 2019
 *
 ******************************************************************************
 */
void power_up_devices()
{

    //
    // Enable power to the main bus bar
    //
    // The main bus bar supplys power to the SKAI2 Traction Inverter.
    // Powered by Murphy 3
    pdm_3_command_main_bussbar_supply_pwr(TRUE);

    //
    // Check to see the status of the precharge.
    //
    bool_t precharge_successful =
        get_sm_precharge_success_status();

    //
    // if pre charge is successful, enable power to the rest of the devices
    //
    if(precharge_successful == TRUE)
    {

        //
        // Enable power to the aux hydraulics system This would power
        // the Skai2_hydraulic_inverter.
        pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(TRUE);

        //
        // Enable power to the two bel charges on the carrier.
        //
        pdm_3_command_charger_1_busbar_pwr(TRUE);

    }
    else
    {

        //
        // Disable power to the devices
        //
        pdm_2_command_aux_hydraulicss_12v_bussbar_pwr(FALSE);
        pdm_3_command_charger_1_busbar_pwr(FALSE);

    }

}
