/*
** ###################################################################
**     Filename  : User_Init.c
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**          This is where the user can initialize their application variables
**          This function gets called once on startup prior to the main loop
**
**     (c) Copyright HED,Inc. 2006-2016
**     HED,Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** ###################################################################
*/

#include <stdio.h>
#include "reserved.h"
#include "Prototypes_CAN.h"
#include "Constants.h"
#include "typedefs.h"
#include "Prototypes.h"
#include "User_App.h"
#include "User_Init.h"
#include "can_service.h"
#include "skai2_inverter.h"
#include "bel_charger_device.h"
#include "pdm_device.h"
#include "skai2_test_device.h"
#include "orion_device.h"
#include "pku2400_device.h"
#include "shinry_dcdc_device.h"
#include "pdm_control_1.h"
#include "pdm_control_2.h"
#include "pdm_control_3.h"
#include "pku2400_control_1.h"
#include "pdm_test_harness.h"
#include "sevcon_hvlp10_device.h"
#include "skai2_inverter_vissim.h"
#include "cl712_device.h"

/******************************************************************************
 *
 *        Name: User_Init()
 *
 * Description: This function gets called once on startup prior to the
 *              main loop and User_App executes.
 *
 *      Author: Tom
 *        Date: Monday, 12 August 2019
 *
 ******************************************************************************
 */
void User_Init(void)
{

    //////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the Carrier's CAN devices.
    //
    //////////////////////////////////////////////////////////////////////////////

    //
    // Initialize the three PDMs on modules 0 (this one) using the
    // 250kbps CAN interface. This initializes each device's receive
    // and transmit CAN messages.
    //
    // Initialize the PDMs
    //
	pdm_init(ONE, 0, CAN2);
	pdm_init(TWO, 0, CAN2);
    pdm_init(THREE, 0, CAN2);


    //=============================================================================
    //
    // pku2400
    //
    //=============================================================================
    pku2400_init(ONE, 0, CAN3);

    //////////////////////////////////////////////////////////////////////////////
    //
    // Initialize the Carrier's vehicle_control functions. These are
    // functional control blocks built on top of the device drivers.
    //
    //////////////////////////////////////////////////////////////////////////////
    pku2400_control_init_1();

    pdm_control_1_init();
    pdm_control_2_init();
    pdm_control_3_init();

    //=============================================================================
    //
    // Skai2 Inverter : Traction Drive Inverter
    //
    //=============================================================================
    //
    init_skai2_inverter_vissim(ONE, 0, CAN3, 0);

    //=============================================================================
    //
    // Skai2 Inverter : Hydraulic Drive Inverter
    //
    //=============================================================================
    //
    init_skai2_inverter_vissim(TWO, 0, CAN3, 0);


    //=============================================================================
    //
    // Shinry DC-DC Converter
    //
    //=============================================================================
    //
    init_shinry_dcdc(ONE, 0, CAN3, 0);

    //=============================================================================
    //
    // Sevcon HVLP10 Air Cooled Inverter
    //
    //=============================================================================
    //
    sevcon_hvlp10_init(ONE, 0, CAN3);

    //=============================================================================
    //
    // Orion BMS 1
    //
    //=============================================================================
    //
    init_orion_bms(ONE, 0, CAN3);
    init_orion_bms(TWO, 0, CAN3);
    init_orion_bms(THREE, 0, CAN3);

    //=============================================================================
    //
    // Bel Charger Control
    //
    //=============================================================================
    //
    bel_charger_init(ONE, 0, CAN3);
    bel_charger_init(TWO, 0, CAN3);


    //=============================================================================
    //
    // CL712 HED Screen
    //
    //=============================================================================
    //
    cl712_init(ONE, 0, CAN1);

}
