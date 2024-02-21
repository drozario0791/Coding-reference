/******************************************************************************
 *
 *        Name: cvc_power_sequencing.c
 *
 * Description: The HED-CL-450-101, known as the CVC, is the central
 *              vehicle control computer for the Carrier. Primary
 *              power to the CVC, +12V battery power is provided to
 *              its pin-A6 (unswitched BAT+) when the master
 *              disconnect switch is closed. Power to its pin-A6 is
 *              the source of power for its outputs 1-8 which are on
 *              connector A. Each connector, A, B, C, D, & E, has a
 *              +12 battery power pin for its associated outputs. In
 *              addition, connector A has a switched +12V battery
 *              input pin 11 which may be used to control when the CVC
 *              enters and leaves its low-power mode. The state of the
 *              pin A11 input (WAKEUP_PIN) may be read and compared to
 *              INPUT_ON or INPUT_OFF to indicate when CVC low power
 *              mode should be entered. Refer to reference (1) and (6)
 *              for details. The pinout for connectors B, C, D and E
 *              are shown below in references (2, 3, 4, & 5).
 *
 *              The power-up of the CVC will start with the master
 *              switch being closed. This will apply +12VDC to the
 *              CVCs pin-A6. The CVC will not come up out of low-power
 *              mode until pin A11 is switched to the +12V battery
 *              voltage through the dash mounted ignition switch. When
 *              the ignition switch is closed, after the master
 *              disconnect is closed, the CVC will initialize its
 *              underlying system and then call the
 *              User_Low_Power_Init() and User_Init() functions. After
 *              this initialization, the CVC will enter its normal
 *              operation mode.
 *
 *              OUT_A10_blue_shutdown_light_signal
 *
 *  References:
 *
 *    (1) file:../../../../ref/images/CL-450-101-XX_ConnectorA_pinout.png
 *    (2) file:../../../../ref/images/CL-450-101-XX_ConnectorB_pinout.png
 *    (3) file:../../../../ref/images/CL-450-101-XX_ConnectorC_pinout.png
 *    (4) file:../../../../ref/images/CL-450-101-XX_ConnectorD_pinout.png
 *    (5) file:../../../../ref/images/CL-450-101-XX_ConnectorE_pinout.png
 *
 *    (6) file:../../../../ref/images/CL-450-101-XX_KeySwitch_Pin_A11.png
 *    (7) file:../../../../ref/images/CL-450_101-XX_BatteryPlus_Module_and_Outputs.png
 *    (8)
 *
 *      Author: Tom
 *        Date: Wednesday, 21 August 2019
 *
 ******************************************************************************
 */

#include "reserved.h"
#include "Prototypes.h"
#include "Constants.h"
#include "User_App.h"
#include "User_Low_Power.h"
#include "cvc_power_sequencing.h"
#include "contactor_control.h"
#include "timer_service.h"
#include "state_machine.h"

bool_t low_power_mode = FALSE;

/******************************************************************************
 *
 *        Name: cvc_power_sequencing_init_low_power()
 *
 * Description: Set the wakeup from low-power mode method. For the
 *              Carrier, we use only the WAKEUP_PIN.
 *
 *      Author: Tom
 *        Date: Wednesday, 21 August 2019
 *
 ******************************************************************************
 */
void cvc_power_sequencing_init_low_power()
{
    Set_Wakeup_Modes((wakeup_modes_t)(WAKEUP_PIN));
}


/******************************************************************************
 *
 *        Name: cvc_power_sequencing_low_power_control()
 *
 * Description: See the description in the file comment header and
 *              comments below.
 *
 *       Notes: (1) HED CL-450-101-100 Standby current is less than
 *                  100 uA.
 *
 *              (2) The threshold voltage of the WAKEUP (switched
 *                  battery) pin is ~4.8 VDC.
 *
 *              (3) The minimum pulse HIGH width of the WAKEUP pin is
 *                  300 mS.
 *
 *      Author: Tom
 *        Date: Wednesday, 21 August 2019
 *
 ******************************************************************************
 */
bool_t cvc_power_sequencing_low_power_control()
{
    int i = 0;
    bool_t ret_val = FALSE;
    uint16_t keyswitch_mv;
    uint16_t screen_ma = 0;
    uint16_t telematics_ma = 0;
    static bool_t first_pass = TRUE;
    static timer_t shutdown_timer;

    //
    // Read the keyswitch voltage input CVC pin A11. Read the source
    // current into the HED screen and the telematics units.
    //
    keyswitch_mv = IN_A11_keyswitch;
    screen_ma = OUT_A02_screen_constant_12v_pwr_CURRENT;
    telematics_ma = OUT_A01_telematics_constant_12v_pwr_CURRENT;

    if(first_pass)
    {

        //
        // Initialise a three second timer.
        //
        shutdown_timer = timer_setup(get_THREE_SECONDS(), RISING);
        first_pass = FALSE;
    }

    bool_t shutdown_timer_state =
        timer_operate(&shutdown_timer,
                      ((keyswitch_mv < KEYSWITCH_ON_TRHESHOLD_MV)
                      || (IN_E11_master_switch_signal == INPUT_OFF)));

    //
    // Check if the key switch is ON.
    //
    if (keyswitch_mv > KEYSWITCH_ON_TRHESHOLD_MV)
    {
        //
        // The keyswitch is ON. Close the 12V contactor. This will
        // provide system wide 12V power. CVC connectors B, C, & D
        // will also have 12V applied.
        //
        Update_Output(OUT_A09_master_contactor_signal, OUTPUT_ON);

        //
        // Switch ON the Screen and the Telematics.
        //
        Update_Output(OUT_A02_screen_constant_12v_pwr, OUTPUT_ON);
        Update_Output(OUT_A01_telematics_constant_12v_pwr, OUTPUT_ON);

        //
        // Turn on the blue light (located next to the master switch)
        // to indicate that the master switch should not be put into
        // the OFF position until the blue light is turned off. Note
        // that the blue LED is turned off automatically when the CVC
        // enters low-power mode.
        //
        Update_Output(OUT_A10_blue_shutdown_light_signal, OUTPUT_ON);
    }
    else
    {
        //
        // The keyswitch is OFF. Open the 12V contactor. This will
        // remove 12V from CVC connectors B, C, & D. The keyswitch
        // signal is also wired to the WAKEUP pin on both the HED
        // screen and the HED telematics unit. Once the keyswitch is
        // OFF, both the screen and the telematics (each are Linux
        // devices) will start their power-down process. Since the CVC
        // powers each of these devices, it will monitor the supply
        // current to each and will switch off power to each one once
        // it has entered sleep mode.
        //
        // Flash (250ms ON/OFF, handled by HED) the blue shutdown LED
        // to indicate that the system is in the process of shutting
        // down. Okay to turn off the master switch when the blue LED
        // is OFF.
        //
        Update_Output(OUT_A10_blue_shutdown_light_signal, OUTPUT_ON);

        //
        // Has the screen entered sleep mode?
        //
        if (shutdown_timer_state)
        {


            Update_Output(OUT_A09_master_contactor_signal, OUTPUT_OFF);

            if(screen_ma < 10)
            {

                //
                // If screen current draw is less than 10mA, it
                // indicates the screen has been turned off.
                //

                Update_Output(OUT_A02_screen_constant_12v_pwr, OUTPUT_OFF);

                if(telematics_ma < 10)
                {

                    //
                    // return value: True indicates the machine can go into
                    // low power mode.
                    //
                    ret_val = TRUE;

                    for(i = 0; i <= 3; i++)
                    {

                        //
                        // Set the following states as it would be the
                        // state, the machine starts up with.
                        //
                        set_state_machine_state(SHUTDOWN);
                        set_positive_contactor(FALSE);
                        set_negative_contactor(FALSE);
                        set_pre_contactor(FALSE);

                        //
                        // Disable the battery pack contactors
                        //
                        Update_Output(OUT_A01_telematics_constant_12v_pwr, OUTPUT_OFF);
                        disable_contactors();
                        Update_Output(OUT_A10_blue_shutdown_light_signal, OUTPUT_OFF);

                    }

                }

            }

        }
    }

    //
    // The blue LED (located near the master switch) is turned off
    // automatically when ret_val is TRUE and the CVC enters low-power
    // mode.
    //
    return (ret_val);
}
