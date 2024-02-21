/******************************************************************************
 *
 *        Name: gears_and_transmission.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Monday, 26 August 2019
 *
 ******************************************************************************
 */

#ifndef GEARS_AND_TRANSMISSION_H_
#define GEARS_AND_TRANSMISSION_H_

typedef enum
{
    LOW_TRANSMISSION_SYSTEM_PRESSURE = -1,
    SHIFTER_FORWARD,
    SHIFTER_REVERSE,
    SHIFTER_NEUTRAL,
    SHIFTER_1st_GEAR,
    SHIFTER_2nd_GEAR,
    SHIFTER_3rd_GEAR,
} shifter_position_t;

//=============================================================================
//
// Gears and Transmission Controls
//
//=============================================================================
//
void gears_and_transmission_control();


//=============================================================================
//
// Getters
//
//=============================================================================
//
shifter_position_t get_shifter_direction();
shifter_position_t get_shifter_gear_position();



#endif
