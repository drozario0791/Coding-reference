/******************************************************************************
 *
 *        Name: contactor_control()
 *
 * Description: The contactor_control function controls the working of
 *              the contactors on the carrier. The input to the
 *              contactor control is obtained from the state
 *              machine. The state machine getters determine the
 *              opening and closing of contactors on the carrier.
 *
 *      Author: Deepak
 *        Date: Tuesday, 10 September 2019
 *
 ******************************************************************************
 */
#include "Prototypes_CAN.h"
#include "can_service.h"
#include "Constants.h"
#include "Prototypes.h"
#include "contactor_control.h"
#include "state_machine.h"

#define MAX_BATTERY_PACKS 3

static void positive_contactor_control(bool_t startup_timer);
static void negative_contactor_control(bool_t startup_timer);
static void precharge_contactor_control(bool_t startup_timer);

/******************************************************************************
 *
 *        Name: contactor_control()
 *
 * Description: The contactor_control function controls the working of
 *              the contactors on the carrier. The input to the
 *              contactor control is obtained from the state
 *              machine. The state machine getters determine the
 *              opening and closing of contactors on the carrier.
 *
 *        Date: Tuesday, 10 September 2019
 *
 ******************************************************************************
 */
void update_contactor_control_output(bool_t startup_timer)
{

    //
    // Positive Contactor Control
    //
    positive_contactor_control(startup_timer);

    //
    // Negative Contactor Control
    //
    negative_contactor_control(startup_timer);

    //
    // Precharge Contactor Control
    //
    precharge_contactor_control(startup_timer);

}


/******************************************************************************
 *
 *        Name: positive_contactor_control()
 *
 * Description: Enables/disables the contactor based on input from
 *              state machine
 *
 * The positive and precharge connectors are swapped on this battery
 * pack.
 *
 *        Date: Tuesday, 10 September 2019
 *
 ******************************************************************************
 */
static void positive_contactor_control(bool_t startup_timer)
{

    //
    // Check the status of the pos_contactor variable in the state
    // machine. Use the status bit to control the working of the
    // contactor based on the result from the function call.
    //
    bool_t sm_positive_contactor_state =
         get_sm_pos_contactor_status();

    if ((sm_positive_contactor_state == TRUE) && startup_timer)
    {

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_ON);

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_ON);

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_ON);

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_ON);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_ON);

    }
    else
    {

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

        Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
        Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

    }


}


/******************************************************************************
 *
 *        Name: negative_contactor_control()
 *
 * Description: Enables/disables the contactor based on input from
 *              state machine
 *
 *        Date: Tuesday, 10 September 2019
 *
 ******************************************************************************
 */
static void negative_contactor_control(bool_t startup_timer)
{

    //
    // Check the status of the pos_contactor variable in the state
    // machine. Use the status bit to control the working of the
    // contactor based on the result from the function call.
    //
    bool_t sm_negative_contactor_state =
         get_sm_neg_contactor_status();

    if ((sm_negative_contactor_state == TRUE) && startup_timer)
    {

        //
        // Enable Contactors
        //
        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_ON);

        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_ON);

        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_ON);

        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_ON);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_ON);

    }
    else
    {

        //
        // Keeps Contactors Turned OFF
        //
        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

        Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
        Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

    }

}


/******************************************************************************
 *
 *        Name: precharge_contactor_control()
 *
 * Description: Enables/disables the contactor based on input from
 *              state machine
 *
 *        Date: Tuesday, 10 September 2019
 *
 ******************************************************************************
 */
static void precharge_contactor_control(bool_t startup_timer)
{

    //
    // Check the status of the pos_contactor variable in the state
    // machine. Use the status bit to control the working of the
    // contactor based on the result from the function call.
    //
    bool_t sm_precharge_contactor_state =
         get_sm_pre_contactor_status();

    if ((sm_precharge_contactor_state == TRUE) && startup_timer)
    {
        //
        // Writing multiple times as a potential fix for an unknown,
        // possibly and electrical issue on the carrier.
        //
        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_ON);

        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_ON);

        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_ON);

        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_ON);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_ON);

    }
    else
    {
        //
        // Writing multiple times as a potential fix for an unknown,
        // possibly and electrical issue on the carrier.
        //
        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

        Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
        Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

    }

}


//=============================================================================
//
// disable_contactors()
//
//=============================================================================
//
void disable_contactors()
{
    Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

    Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

    Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

    Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

    Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

    Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

    Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

    Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

    Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

    Update_Output(OUT_D01_battery_1_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D04_battery_2_positive_contactor, OUTPUT_OFF);
    Update_Output(OUT_D10_battery_3_positive_contactor, OUTPUT_OFF);

    Update_Output(OUT_D02_battery_1_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D07_battery_2_negative_contactor, OUTPUT_OFF);
    Update_Output(OUT_D13_battery_3_negative_contactor, OUTPUT_OFF);

    Update_Output(OUT_D03_battery_1_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_D09_battery_2_precharge_contactor, OUTPUT_OFF);
    Update_Output(OUT_C14_battery_3_precharge_contactor, OUTPUT_OFF);

}
