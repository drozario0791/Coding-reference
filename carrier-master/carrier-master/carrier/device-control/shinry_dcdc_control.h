/******************************************************************************
 *
 *        Name: shinry_dcdc_control.h
 *
 * Description:
 *
 *      Author: Deepak
 *        Date: Wednesday, 04 September 2019
 *
 ******************************************************************************
 */

#ifndef SHINRY_DCDC_CONTROL_H_
#define SHINRY_DCDC_CONTROL_H_

/******************************************************************************
 *
 *        Name: shinry_dc_dc_control();
 *
 * Description: The DC-DC converter charges the 12V LVbattery on the
 *              vehicle. The DC-DC gets its input from the high
 *              voltage battery. Its it important for the shinry DC-DC
 *              converter to be on as soon as contactors are
 *              closed. The reason being, the pumps, fans, BMS,
 *              inverters are powered off the 12V supply. The DC-DC
 *              needs to ensure the 12V battery is always charged to
 *              keep the rest of the system up and functional.
 *
 *
 ******************************************************************************
 */
bool_t shinry_dc_dc_control();


/******************************************************************************
 *
 *        Name: shinry_dcdc_error_status()
 *
 * Description: returns True if the the DCDC flags an error.
 *
 *
 ******************************************************************************
 */
bool_t shinry_dcdc_error_status();



#endif
