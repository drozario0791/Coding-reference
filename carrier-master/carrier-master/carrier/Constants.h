/*
** #########################################################################
**     File Name   : Constants.h
**     Processor   : Not applicable
**     File Format : Version 00.01
**     Composer    : Version 19.12.2.1055
**     Database    : carrier.sdf
**     IOC         : carrier.ioc
**     App version : Version 00.00
**     Date/Time   : 2020-06-10, 5:20 PM UTC
**     Abstract    :
**     This header has all the #defines for the system inputs, outputs, modules, etc.
**
**     (c) Copyright HED, Inc. 2006-2009
**     HED, Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53089
**     United States
**     http        : www.hedonline.com
**     mail        : support@hedonline.com
** #########################################################################
**                            !!!  WARNING !!!
**     ALTERING THIS FILE IN ANY WAY MAY CAUSE PERSONAL INJURY AND/OR DEATH.
** #########################################################################
*/

/*  Module Numbers */
#define Module1                0     /*CL-450-101-10*/

/*  IOMap locations */

/*  Inputs assigned to module Module1 */
#define IN_E11_master_switch_signal      IOMap[0]        /*Range 0 - 2*/
#define IN_E01_cvc_zener_input           IOMap[4]        /*Range 0 - 5000*/
#define IN_E03_throttle_position_sensor_signal IOMap[6]        /*Range 0 - 5000*/
#define IN_E10_main_sys_pressure_sensor_signal IOMap[11]       /*Range 0 - 5000*/
#define IN_A11_keyswitch                 IOMap[16]       /*Range 0 - 39000*/
#define IN_A15_stop_charging_button_signal IOMap[25]       /*Range 0 - 2*/
#define IN_A16_dash_e_stop_signal        IOMap[26]       /*Range 0 - 2*/
#define IN_A17_charge_box_e_stop_signal  IOMap[27]       /*Range 0 - 2*/
#define IN_A18_diagnositic_switch_signal IOMap[28]       /*Range 0 - 2*/
#define IN_B15_forward_switch            IOMap[38]       /*Range 0 - 2*/
#define IN_B16_1st_gear_switch           IOMap[39]       /*Range 0 - 2*/
#define IN_B17_2nd_gear_switch           IOMap[40]       /*Range 0 - 2*/
#define IN_B18_3rd_gear_switch           IOMap[41]       /*Range 0 - 2*/
#define IN_C11_neutral_switch            IOMap[48]       /*Range 0 - 2*/
#define IN_C15_aba_brake_button_signal   IOMap[51]       /*Range 0 - 2*/
#define IN_C16_aba_brake_momentary_signal IOMap[52]       /*Range 0 - 2*/
#define IN_C18_reverse_switch            IOMap[54]       /*Range 0 - 2*/

/*  Value Outputs assigned to module Module1 */
#define OUT_A02_screen_constant_12v_pwr_VALUE IOMap[72]       /*Range 0 - 1000*/
#define OUT_A01_telematics_constant_12v_pwr_VALUE IOMap[73]       /*Range 0 - 1000*/
#define OUT_A14_power_for_charge_box_estop_led_VALUE IOMap[74]       /*Range 0 - 1000*/
#define OUT_A13_turn_on_balancing_signal_VALUE IOMap[75]       /*Range 0 - 1000*/
#define OUT_B09_low_accum_light_pwr_VALUE IOMap[77]       /*Range 0 - 1000*/
#define OUT_B10_brake_impending_light_VALUE IOMap[78]       /*Range 0 - 1000*/
#define OUT_B02_status_bicolor_green_led_VALUE IOMap[81]       /*Range 0 - 1000*/
#define OUT_B03_stop_charge_button_led_VALUE IOMap[82]       /*Range 0 - 1000*/
#define OUT_D01_battery_1_positive_contactor_VALUE IOMap[83]       /*Range 0 - 1000*/
#define OUT_C09_seat_belt_light_pwr_VALUE IOMap[84]       /*Range 0 - 1000*/
#define OUT_C10_seat_belt_buzzer_pwr_VALUE IOMap[85]       /*Range 0 - 1000*/
#define OUT_C04_aba_brake_button_led_pwr_VALUE IOMap[86]       /*Range 0 - 1000*/
#define OUT_A10_blue_shutdown_light_signal_VALUE IOMap[87]       /*Range 0 - 1000*/
#define OUT_A09_master_contactor_signal_VALUE IOMap[88]       /*Range 0 - 1000*/
#define OUT_C13_spare_VALUE              IOMap[89]       /*Range 0 - 1000*/
#define OUT_C14_battery_3_precharge_contactor_VALUE IOMap[90]       /*Range 0 - 1000*/
#define OUT_A03_charge_box_relay_signal_from_cvc_VALUE IOMap[92]       /*Range 0 - 1000*/
#define OUT_D04_battery_2_positive_contactor_VALUE IOMap[93]       /*Range 0 - 1000*/
#define OUT_D13_battery_3_negative_contactor_VALUE IOMap[94]       /*Range 0 - 1000*/
#define OUT_D10_battery_3_positive_contactor_VALUE IOMap[95]       /*Range 0 - 1000*/
#define OUT_D09_battery_2_precharge_contactor_VALUE IOMap[96]       /*Range 0 - 1000*/
#define OUT_D02_battery_1_negative_contactor_VALUE IOMap[97]       /*Range 0 - 1000*/
#define OUT_D03_battery_1_precharge_contactor_VALUE IOMap[98]       /*Range 0 - 1000*/
#define OUT_C02_reverse_gear_solenoid_VALUE IOMap[99]       /*Range 0 - 1000*/
#define OUT_D14_dash_e_stop_led_pwr_VALUE IOMap[101]      /*Range 0 - 1000*/
#define OUT_C01_forward_gear_solenoid_VALUE IOMap[102]      /*Range 0 - 1000*/
#define OUT_B01_status_bicolor_red_led_VALUE IOMap[103]      /*Range 0 - 1000*/
#define OUT_D07_battery_2_negative_contactor_VALUE IOMap[104]      /*Range 0 - 1000*/

/*  Flash Outputs assigned to module Module1 */
#define OUT_A02_screen_constant_12v_pwr_FLASH IOMap[105]      /*Range 0 - 1*/
#define OUT_A01_telematics_constant_12v_pwr_FLASH IOMap[106]      /*Range 0 - 1*/
#define OUT_A14_power_for_charge_box_estop_led_FLASH IOMap[107]      /*Range 0 - 1*/
#define OUT_A13_turn_on_balancing_signal_FLASH IOMap[108]      /*Range 0 - 1*/
#define OUT_B09_low_accum_light_pwr_FLASH IOMap[110]      /*Range 0 - 1*/
#define OUT_B10_brake_impending_light_FLASH IOMap[111]      /*Range 0 - 1*/
#define OUT_B02_status_bicolor_green_led_FLASH IOMap[114]      /*Range 0 - 1*/
#define OUT_B03_stop_charge_button_led_FLASH IOMap[115]      /*Range 0 - 1*/
#define OUT_D01_battery_1_positive_contactor_FLASH IOMap[116]      /*Range 0 - 1*/
#define OUT_C09_seat_belt_light_pwr_FLASH IOMap[117]      /*Range 0 - 1*/
#define OUT_C10_seat_belt_buzzer_pwr_FLASH IOMap[118]      /*Range 0 - 1*/
#define OUT_C04_aba_brake_button_led_pwr_FLASH IOMap[119]      /*Range 0 - 1*/
#define OUT_A10_blue_shutdown_light_signal_FLASH IOMap[120]      /*Range 0 - 1*/
#define OUT_A09_master_contactor_signal_FLASH IOMap[121]      /*Range 0 - 1*/
#define OUT_C13_spare_FLASH              IOMap[122]      /*Range 0 - 1*/
#define OUT_C14_battery_3_precharge_contactor_FLASH IOMap[123]      /*Range 0 - 1*/
#define OUT_A03_charge_box_relay_signal_from_cvc_FLASH IOMap[125]      /*Range 0 - 1*/
#define OUT_D04_battery_2_positive_contactor_FLASH IOMap[126]      /*Range 0 - 1*/
#define OUT_D13_battery_3_negative_contactor_FLASH IOMap[127]      /*Range 0 - 1*/
#define OUT_D10_battery_3_positive_contactor_FLASH IOMap[128]      /*Range 0 - 1*/
#define OUT_D09_battery_2_precharge_contactor_FLASH IOMap[129]      /*Range 0 - 1*/
#define OUT_D02_battery_1_negative_contactor_FLASH IOMap[130]      /*Range 0 - 1*/
#define OUT_D03_battery_1_precharge_contactor_FLASH IOMap[131]      /*Range 0 - 1*/
#define OUT_C02_reverse_gear_solenoid_FLASH IOMap[132]      /*Range 0 - 1*/
#define OUT_D14_dash_e_stop_led_pwr_FLASH IOMap[134]      /*Range 0 - 1*/
#define OUT_C01_forward_gear_solenoid_FLASH IOMap[135]      /*Range 0 - 1*/
#define OUT_B01_status_bicolor_red_led_FLASH IOMap[136]      /*Range 0 - 1*/
#define OUT_D07_battery_2_negative_contactor_FLASH IOMap[137]      /*Range 0 - 1*/

/*  Status Outputs assigned to module Module1 */
#define OUT_A02_screen_constant_12v_pwr_STATUS IOMap[138]      /*Range 0 - 4*/
#define OUT_A01_telematics_constant_12v_pwr_STATUS IOMap[139]      /*Range 0 - 4*/
#define OUT_A14_power_for_charge_box_estop_led_STATUS IOMap[140]      /*Range 0 - 4*/
#define OUT_A13_turn_on_balancing_signal_STATUS IOMap[141]      /*Range 0 - 4*/
#define OUT_B09_low_accum_light_pwr_STATUS IOMap[143]      /*Range 0 - 4*/
#define OUT_B10_brake_impending_light_STATUS IOMap[144]      /*Range 0 - 4*/
#define OUT_B02_status_bicolor_green_led_STATUS IOMap[147]      /*Range 0 - 4*/
#define OUT_B03_stop_charge_button_led_STATUS IOMap[148]      /*Range 0 - 4*/
#define OUT_D01_battery_1_positive_contactor_STATUS IOMap[149]      /*Range 0 - 4*/
#define OUT_C09_seat_belt_light_pwr_STATUS IOMap[150]      /*Range 0 - 4*/
#define OUT_C10_seat_belt_buzzer_pwr_STATUS IOMap[151]      /*Range 0 - 4*/
#define OUT_C04_aba_brake_button_led_pwr_STATUS IOMap[152]      /*Range 0 - 4*/
#define OUT_A10_blue_shutdown_light_signal_STATUS IOMap[153]      /*Range 0 - 4*/
#define OUT_A09_master_contactor_signal_STATUS IOMap[154]      /*Range 0 - 4*/
#define OUT_C13_spare_STATUS             IOMap[155]      /*Range 0 - 4*/
#define OUT_C14_battery_3_precharge_contactor_STATUS IOMap[156]      /*Range 0 - 4*/
#define OUT_A03_charge_box_relay_signal_from_cvc_STATUS IOMap[158]      /*Range 0 - 4*/
#define OUT_D04_battery_2_positive_contactor_STATUS IOMap[159]      /*Range 0 - 4*/
#define OUT_D13_battery_3_negative_contactor_STATUS IOMap[160]      /*Range 0 - 4*/
#define OUT_D10_battery_3_positive_contactor_STATUS IOMap[161]      /*Range 0 - 4*/
#define OUT_D09_battery_2_precharge_contactor_STATUS IOMap[162]      /*Range 0 - 4*/
#define OUT_D02_battery_1_negative_contactor_STATUS IOMap[163]      /*Range 0 - 4*/
#define OUT_D03_battery_1_precharge_contactor_STATUS IOMap[164]      /*Range 0 - 4*/
#define OUT_C02_reverse_gear_solenoid_STATUS IOMap[165]      /*Range 0 - 4*/
#define OUT_D14_dash_e_stop_led_pwr_STATUS IOMap[167]      /*Range 0 - 4*/
#define OUT_C01_forward_gear_solenoid_STATUS IOMap[168]      /*Range 0 - 4*/
#define OUT_B01_status_bicolor_red_led_STATUS IOMap[169]      /*Range 0 - 4*/
#define OUT_D07_battery_2_negative_contactor_STATUS IOMap[170]      /*Range 0 - 4*/

/*  Current Outputs assigned to module Module1 */
#define OUT_A02_screen_constant_12v_pwr_CURRENT IOMap[171]      /*Range 0 - 3000*/
#define OUT_A01_telematics_constant_12v_pwr_CURRENT IOMap[172]      /*Range 0 - 3000*/
#define OUT_A14_power_for_charge_box_estop_led_CURRENT IOMap[173]      /*Range 0 - 3000*/
#define OUT_A13_turn_on_balancing_signal_CURRENT IOMap[174]      /*Range 0 - 3000*/
#define OUT_B09_low_accum_light_pwr_CURRENT IOMap[176]      /*Range 0 - 3000*/
#define OUT_B10_brake_impending_light_CURRENT IOMap[177]      /*Range 0 - 3000*/
#define OUT_B02_status_bicolor_green_led_CURRENT IOMap[180]      /*Range 0 - 3000*/
#define OUT_B03_stop_charge_button_led_CURRENT IOMap[181]      /*Range 0 - 3000*/
#define OUT_D01_battery_1_positive_contactor_CURRENT IOMap[182]      /*Range 0 - 3000*/
#define OUT_C09_seat_belt_light_pwr_CURRENT IOMap[183]      /*Range 0 - 3000*/
#define OUT_C10_seat_belt_buzzer_pwr_CURRENT IOMap[184]      /*Range 0 - 3000*/
#define OUT_C04_aba_brake_button_led_pwr_CURRENT IOMap[185]      /*Range 0 - 3000*/
#define OUT_A10_blue_shutdown_light_signal_CURRENT IOMap[186]      /*Range 0 - 3000*/
#define OUT_A09_master_contactor_signal_CURRENT IOMap[187]      /*Range 0 - 3000*/
#define OUT_C13_spare_CURRENT            IOMap[188]      /*Range 0 - 3000*/
#define OUT_C14_battery_3_precharge_contactor_CURRENT IOMap[189]      /*Range 0 - 3000*/
#define OUT_A03_charge_box_relay_signal_from_cvc_CURRENT IOMap[191]      /*Range 0 - 3000*/
#define OUT_D04_battery_2_positive_contactor_CURRENT IOMap[192]      /*Range 0 - 3000*/
#define OUT_D13_battery_3_negative_contactor_CURRENT IOMap[193]      /*Range 0 - 3000*/
#define OUT_D10_battery_3_positive_contactor_CURRENT IOMap[194]      /*Range 0 - 3000*/
#define OUT_D09_battery_2_precharge_contactor_CURRENT IOMap[195]      /*Range 0 - 3000*/
#define OUT_D02_battery_1_negative_contactor_CURRENT IOMap[196]      /*Range 0 - 3000*/
#define OUT_D03_battery_1_precharge_contactor_CURRENT IOMap[197]      /*Range 0 - 3000*/
#define OUT_C02_reverse_gear_solenoid_CURRENT IOMap[198]      /*Range 0 - 3000*/
#define OUT_D14_dash_e_stop_led_pwr_CURRENT IOMap[200]      /*Range 0 - 3000*/
#define OUT_C01_forward_gear_solenoid_CURRENT IOMap[201]      /*Range 0 - 3000*/
#define OUT_B01_status_bicolor_red_led_CURRENT IOMap[202]      /*Range 0 - 3000*/
#define OUT_D07_battery_2_negative_contactor_CURRENT IOMap[203]      /*Range 0 - 3000*/

/* State Machines Enumeration */

/* Timers */
#define LoopCounter_VALUE                *((uint32_t *FAR_T) &IOMap[252])  /*Range 0 - 4294967295*/
#define EEVAR_HOURMETER_SECONDS_VALUE    *((uint32_t *FAR_T) &IOMap[254])  /*Range 0 - 4294967295*/
#define EEVAR_TIME_UNTIL_SERVICE_VALUE   *((uint32_t *FAR_T) &IOMap[256])  /*Range 0 - 4294967295*/
#define LoopCounter_STATUS               IOMap[258]      /*Range 0 - 1*/
#define EEVAR_HOURMETER_SECONDS_STATUS   IOMap[259]      /*Range 0 - 1*/
#define EEVAR_TIME_UNTIL_SERVICE_STATUS  IOMap[260]      /*Range 0 - 1*/

/* Eeproms */
#define EEVAR_ODOMETER_METERS            *((uint32_t *FAR_T) &IOMap[206])  /*Range 0 - 4294967295*/
#define EEVAR_HYDRAULIC_SKAI_dp_gain     IOMap[208]      /*Range 0 - 10000*/
#define EEVAR_HYDRAULIC_SKAI_di_gain     IOMap[209]      /*Range 0 - 1000*/
#define EEVAR_HYDRAULIC_SKAI_qp_gain     IOMap[210]      /*Range 0 - 10000*/
#define EEVAR_HYDRAULIC_SKAI_qi_gain     IOMap[211]      /*Range 0 - 10000*/
#define EEVAR_HYDRAULIC_SKAI_speed_p_gain IOMap[212]      /*Range 0 - 10000*/
#define EEVAR_HYDRAULIC_SKAI_speed_i_gain IOMap[213]      /*Range 0 - 12800*/
#define EEVAR_HYDRAULIC_SKAI_elec_angle_fwd IOMap[214]      /*Range 0 - 1023*/
#define EEVAR_HYDRAULIC_SKAI_elec_angle_rvs IOMap[215]      /*Range 0 - 1023*/
#define EEVAR_HYDRAULIC_SKAI_max_battery_current IOMap[216]      /*Range 0 - 10000*/
#define EEVAR_HYDRAULIC_SKAI_battery_current_i_gain IOMap[217]      /*Range 0 - 10000*/
#define EEVAR_HYDRAULIC_SKAI_max_motor_rpm IOMap[218]      /*Range 0 - 3000*/
#define EEVAR_HYDRAULIC_SKAI_out_gain    IOMap[219]      /*Range 0 - 1000*/
#define EEVAR_HYDRAULIC_SKAI_motor_direction IOMap[220]      /*Range 0 - 255*/
#define EEVAR_TRACTION_SKAI_dp_gain      IOMap[221]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_di_gain      IOMap[222]      /*Range 0 - 1000*/
#define EEVAR_TRACTION_SKAI_qp_gain      IOMap[223]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_qi_gain      IOMap[224]      /*Range 0 - 1000*/
#define EEVAR_TRACTION_SKAI_speed_p_gain IOMap[225]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_speed_i_gain IOMap[226]      /*Range 0 - 12800*/
#define EEVAR_TRACTION_SKAI_elec_angle_fwd IOMap[227]      /*Range 0 - 1023*/
#define EEVAR_TRACTION_SKAI_elec_angle_rvs IOMap[228]      /*Range 0 - 1023*/
#define EEVAR_TRACTION_SKAI_max_battery_current IOMap[229]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_motor_scale_p1 IOMap[230]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_motor_scale_p2 IOMap[231]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_motor_scale_l1 IOMap[232]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_motor_scale_l2 IOMap[233]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_regen_scale_p1 IOMap[234]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_regen_scale_p2 IOMap[235]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_regen_scale_l1 IOMap[236]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_regen_scale_l2 IOMap[237]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_throttle_gain_1 IOMap[238]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_throttle_gain_2 IOMap[239]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_throttle_released IOMap[240]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_throttle_depressed IOMap[241]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_battery_current_i_gain IOMap[242]      /*Range 0 - 5000*/
#define EEVAR_TRACTION_SKAI_max_motor_rpm IOMap[243]      /*Range 0 - 3000*/
#define EEVAR_TRACTION_SKAI_outgain      IOMap[244]      /*Range 0 - 1000*/
#define EEVAR_CHARGER_no_of_chargers     IOMap[245]      /*Range 0 - 4*/
#define EEVAR_CHARGER_total_current_command IOMap[246]      /*Range 0 - 75*/
#define EEVAR_HYDRAULIC_SKAI_speed_p_gain_time_ramp IOMap[247]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_elec_fwd_regen IOMap[248]      /*Range 0 - 10000*/
#define EEVAR_TRACTION_SKAI_elec_rev_regen IOMap[249]      /*Range 0 - 10000*/
#define EEVAR_THROTTLE_DEPRESSED         IOMap[250]      /*Range 0 - 4095*/
#define EEVAR_THROTTLE_RELEASED          IOMap[251]      /*Range 0 - 4095*/

/* Module Information */
#define Module1_STATUS                   IOMap[204]      /*Range 0 - 10*/
#define Module1_CAN_STATUS               IOMap[205]      /*Range 0 - 3*/

/* Defines for Update Output function calls */
#define OUT_A02_screen_constant_12v_pwr  (uint8_t)Module1       , (uint8_t)0
#define OUT_A01_telematics_constant_12v_pwr (uint8_t)Module1       , (uint8_t)1
#define OUT_A14_power_for_charge_box_estop_led (uint8_t)Module1       , (uint8_t)2
#define OUT_A13_turn_on_balancing_signal (uint8_t)Module1       , (uint8_t)3
#define OUT_B09_low_accum_light_pwr      (uint8_t)Module1       , (uint8_t)5
#define OUT_B10_brake_impending_light    (uint8_t)Module1       , (uint8_t)6
#define OUT_B02_status_bicolor_green_led (uint8_t)Module1       , (uint8_t)9
#define OUT_B03_stop_charge_button_led   (uint8_t)Module1       , (uint8_t)10
#define OUT_D01_battery_1_positive_contactor (uint8_t)Module1       , (uint8_t)11
#define OUT_C09_seat_belt_light_pwr      (uint8_t)Module1       , (uint8_t)12
#define OUT_C10_seat_belt_buzzer_pwr     (uint8_t)Module1       , (uint8_t)13
#define OUT_C04_aba_brake_button_led_pwr (uint8_t)Module1       , (uint8_t)14
#define OUT_A10_blue_shutdown_light_signal (uint8_t)Module1       , (uint8_t)15
#define OUT_A09_master_contactor_signal  (uint8_t)Module1       , (uint8_t)16
#define OUT_C13_spare                    (uint8_t)Module1       , (uint8_t)17
#define OUT_C14_battery_3_precharge_contactor (uint8_t)Module1       , (uint8_t)18
#define OUT_A03_charge_box_relay_signal_from_cvc (uint8_t)Module1       , (uint8_t)20
#define OUT_D04_battery_2_positive_contactor (uint8_t)Module1       , (uint8_t)21
#define OUT_D13_battery_3_negative_contactor (uint8_t)Module1       , (uint8_t)22
#define OUT_D10_battery_3_positive_contactor (uint8_t)Module1       , (uint8_t)23
#define OUT_D09_battery_2_precharge_contactor (uint8_t)Module1       , (uint8_t)24
#define OUT_D02_battery_1_negative_contactor (uint8_t)Module1       , (uint8_t)25
#define OUT_D03_battery_1_precharge_contactor (uint8_t)Module1       , (uint8_t)26
#define OUT_C02_reverse_gear_solenoid    (uint8_t)Module1       , (uint8_t)27
#define OUT_D14_dash_e_stop_led_pwr      (uint8_t)Module1       , (uint8_t)29
#define OUT_C01_forward_gear_solenoid    (uint8_t)Module1       , (uint8_t)30
#define OUT_B01_status_bicolor_red_led   (uint8_t)Module1       , (uint8_t)31
#define OUT_D07_battery_2_negative_contactor (uint8_t)Module1       , (uint8_t)32

/* IOMap Indexes */
typedef enum
{
	IOMapIndex_IN_E11_master_switch_signal   = 0,
	IOMapIndex_IN_E01_cvc_zener_input        = 4,
	IOMapIndex_IN_E03_throttle_position_sensor_signal = 6,
	IOMapIndex_IN_E10_main_sys_pressure_sensor_signal = 11,
	IOMapIndex_IN_A11_keyswitch              = 16,
	IOMapIndex_IN_A15_stop_charging_button_signal = 25,
	IOMapIndex_IN_A16_dash_e_stop_signal     = 26,
	IOMapIndex_IN_A17_charge_box_e_stop_signal = 27,
	IOMapIndex_IN_A18_diagnositic_switch_signal = 28,
	IOMapIndex_IN_B15_forward_switch         = 38,
	IOMapIndex_IN_B16_1st_gear_switch        = 39,
	IOMapIndex_IN_B17_2nd_gear_switch        = 40,
	IOMapIndex_IN_B18_3rd_gear_switch        = 41,
	IOMapIndex_IN_C11_neutral_switch         = 48,
	IOMapIndex_IN_C15_aba_brake_button_signal = 51,
	IOMapIndex_IN_C16_aba_brake_momentary_signal = 52,
	IOMapIndex_IN_C18_reverse_switch         = 54,
	IOMapIndex_OUT_A02_screen_constant_12v_pwr_VALUE = 72,
	IOMapIndex_OUT_A01_telematics_constant_12v_pwr_VALUE = 73,
	IOMapIndex_OUT_A14_power_for_charge_box_estop_led_VALUE = 74,
	IOMapIndex_OUT_A13_turn_on_balancing_signal_VALUE = 75,
	IOMapIndex_OUT_B09_low_accum_light_pwr_VALUE = 77,
	IOMapIndex_OUT_B10_brake_impending_light_VALUE = 78,
	IOMapIndex_OUT_B02_status_bicolor_green_led_VALUE = 81,
	IOMapIndex_OUT_B03_stop_charge_button_led_VALUE = 82,
	IOMapIndex_OUT_D01_battery_1_positive_contactor_VALUE = 83,
	IOMapIndex_OUT_C09_seat_belt_light_pwr_VALUE = 84,
	IOMapIndex_OUT_C10_seat_belt_buzzer_pwr_VALUE = 85,
	IOMapIndex_OUT_C04_aba_brake_button_led_pwr_VALUE = 86,
	IOMapIndex_OUT_A10_blue_shutdown_light_signal_VALUE = 87,
	IOMapIndex_OUT_A09_master_contactor_signal_VALUE = 88,
	IOMapIndex_OUT_C13_spare_VALUE           = 89,
	IOMapIndex_OUT_C14_battery_3_precharge_contactor_VALUE = 90,
	IOMapIndex_OUT_A03_charge_box_relay_signal_from_cvc_VALUE = 92,
	IOMapIndex_OUT_D04_battery_2_positive_contactor_VALUE = 93,
	IOMapIndex_OUT_D13_battery_3_negative_contactor_VALUE = 94,
	IOMapIndex_OUT_D10_battery_3_positive_contactor_VALUE = 95,
	IOMapIndex_OUT_D09_battery_2_precharge_contactor_VALUE = 96,
	IOMapIndex_OUT_D02_battery_1_negative_contactor_VALUE = 97,
	IOMapIndex_OUT_D03_battery_1_precharge_contactor_VALUE = 98,
	IOMapIndex_OUT_C02_reverse_gear_solenoid_VALUE = 99,
	IOMapIndex_OUT_D14_dash_e_stop_led_pwr_VALUE = 101,
	IOMapIndex_OUT_C01_forward_gear_solenoid_VALUE = 102,
	IOMapIndex_OUT_B01_status_bicolor_red_led_VALUE = 103,
	IOMapIndex_OUT_D07_battery_2_negative_contactor_VALUE = 104,
	IOMapIndex_OUT_A02_screen_constant_12v_pwr_FLASH = 105,
	IOMapIndex_OUT_A01_telematics_constant_12v_pwr_FLASH = 106,
	IOMapIndex_OUT_A14_power_for_charge_box_estop_led_FLASH = 107,
	IOMapIndex_OUT_A13_turn_on_balancing_signal_FLASH = 108,
	IOMapIndex_OUT_B09_low_accum_light_pwr_FLASH = 110,
	IOMapIndex_OUT_B10_brake_impending_light_FLASH = 111,
	IOMapIndex_OUT_B02_status_bicolor_green_led_FLASH = 114,
	IOMapIndex_OUT_B03_stop_charge_button_led_FLASH = 115,
	IOMapIndex_OUT_D01_battery_1_positive_contactor_FLASH = 116,
	IOMapIndex_OUT_C09_seat_belt_light_pwr_FLASH = 117,
	IOMapIndex_OUT_C10_seat_belt_buzzer_pwr_FLASH = 118,
	IOMapIndex_OUT_C04_aba_brake_button_led_pwr_FLASH = 119,
	IOMapIndex_OUT_A10_blue_shutdown_light_signal_FLASH = 120,
	IOMapIndex_OUT_A09_master_contactor_signal_FLASH = 121,
	IOMapIndex_OUT_C13_spare_FLASH           = 122,
	IOMapIndex_OUT_C14_battery_3_precharge_contactor_FLASH = 123,
	IOMapIndex_OUT_A03_charge_box_relay_signal_from_cvc_FLASH = 125,
	IOMapIndex_OUT_D04_battery_2_positive_contactor_FLASH = 126,
	IOMapIndex_OUT_D13_battery_3_negative_contactor_FLASH = 127,
	IOMapIndex_OUT_D10_battery_3_positive_contactor_FLASH = 128,
	IOMapIndex_OUT_D09_battery_2_precharge_contactor_FLASH = 129,
	IOMapIndex_OUT_D02_battery_1_negative_contactor_FLASH = 130,
	IOMapIndex_OUT_D03_battery_1_precharge_contactor_FLASH = 131,
	IOMapIndex_OUT_C02_reverse_gear_solenoid_FLASH = 132,
	IOMapIndex_OUT_D14_dash_e_stop_led_pwr_FLASH = 134,
	IOMapIndex_OUT_C01_forward_gear_solenoid_FLASH = 135,
	IOMapIndex_OUT_B01_status_bicolor_red_led_FLASH = 136,
	IOMapIndex_OUT_D07_battery_2_negative_contactor_FLASH = 137,
	IOMapIndex_OUT_A02_screen_constant_12v_pwr_STATUS = 138,
	IOMapIndex_OUT_A01_telematics_constant_12v_pwr_STATUS = 139,
	IOMapIndex_OUT_A14_power_for_charge_box_estop_led_STATUS = 140,
	IOMapIndex_OUT_A13_turn_on_balancing_signal_STATUS = 141,
	IOMapIndex_OUT_B09_low_accum_light_pwr_STATUS = 143,
	IOMapIndex_OUT_B10_brake_impending_light_STATUS = 144,
	IOMapIndex_OUT_B02_status_bicolor_green_led_STATUS = 147,
	IOMapIndex_OUT_B03_stop_charge_button_led_STATUS = 148,
	IOMapIndex_OUT_D01_battery_1_positive_contactor_STATUS = 149,
	IOMapIndex_OUT_C09_seat_belt_light_pwr_STATUS = 150,
	IOMapIndex_OUT_C10_seat_belt_buzzer_pwr_STATUS = 151,
	IOMapIndex_OUT_C04_aba_brake_button_led_pwr_STATUS = 152,
	IOMapIndex_OUT_A10_blue_shutdown_light_signal_STATUS = 153,
	IOMapIndex_OUT_A09_master_contactor_signal_STATUS = 154,
	IOMapIndex_OUT_C13_spare_STATUS          = 155,
	IOMapIndex_OUT_C14_battery_3_precharge_contactor_STATUS = 156,
	IOMapIndex_OUT_A03_charge_box_relay_signal_from_cvc_STATUS = 158,
	IOMapIndex_OUT_D04_battery_2_positive_contactor_STATUS = 159,
	IOMapIndex_OUT_D13_battery_3_negative_contactor_STATUS = 160,
	IOMapIndex_OUT_D10_battery_3_positive_contactor_STATUS = 161,
	IOMapIndex_OUT_D09_battery_2_precharge_contactor_STATUS = 162,
	IOMapIndex_OUT_D02_battery_1_negative_contactor_STATUS = 163,
	IOMapIndex_OUT_D03_battery_1_precharge_contactor_STATUS = 164,
	IOMapIndex_OUT_C02_reverse_gear_solenoid_STATUS = 165,
	IOMapIndex_OUT_D14_dash_e_stop_led_pwr_STATUS = 167,
	IOMapIndex_OUT_C01_forward_gear_solenoid_STATUS = 168,
	IOMapIndex_OUT_B01_status_bicolor_red_led_STATUS = 169,
	IOMapIndex_OUT_D07_battery_2_negative_contactor_STATUS = 170,
	IOMapIndex_OUT_A02_screen_constant_12v_pwr_CURRENT = 171,
	IOMapIndex_OUT_A01_telematics_constant_12v_pwr_CURRENT = 172,
	IOMapIndex_OUT_A14_power_for_charge_box_estop_led_CURRENT = 173,
	IOMapIndex_OUT_A13_turn_on_balancing_signal_CURRENT = 174,
	IOMapIndex_OUT_B09_low_accum_light_pwr_CURRENT = 176,
	IOMapIndex_OUT_B10_brake_impending_light_CURRENT = 177,
	IOMapIndex_OUT_B02_status_bicolor_green_led_CURRENT = 180,
	IOMapIndex_OUT_B03_stop_charge_button_led_CURRENT = 181,
	IOMapIndex_OUT_D01_battery_1_positive_contactor_CURRENT = 182,
	IOMapIndex_OUT_C09_seat_belt_light_pwr_CURRENT = 183,
	IOMapIndex_OUT_C10_seat_belt_buzzer_pwr_CURRENT = 184,
	IOMapIndex_OUT_C04_aba_brake_button_led_pwr_CURRENT = 185,
	IOMapIndex_OUT_A10_blue_shutdown_light_signal_CURRENT = 186,
	IOMapIndex_OUT_A09_master_contactor_signal_CURRENT = 187,
	IOMapIndex_OUT_C13_spare_CURRENT         = 188,
	IOMapIndex_OUT_C14_battery_3_precharge_contactor_CURRENT = 189,
	IOMapIndex_OUT_A03_charge_box_relay_signal_from_cvc_CURRENT = 191,
	IOMapIndex_OUT_D04_battery_2_positive_contactor_CURRENT = 192,
	IOMapIndex_OUT_D13_battery_3_negative_contactor_CURRENT = 193,
	IOMapIndex_OUT_D10_battery_3_positive_contactor_CURRENT = 194,
	IOMapIndex_OUT_D09_battery_2_precharge_contactor_CURRENT = 195,
	IOMapIndex_OUT_D02_battery_1_negative_contactor_CURRENT = 196,
	IOMapIndex_OUT_D03_battery_1_precharge_contactor_CURRENT = 197,
	IOMapIndex_OUT_C02_reverse_gear_solenoid_CURRENT = 198,
	IOMapIndex_OUT_D14_dash_e_stop_led_pwr_CURRENT = 200,
	IOMapIndex_OUT_C01_forward_gear_solenoid_CURRENT = 201,
	IOMapIndex_OUT_B01_status_bicolor_red_led_CURRENT = 202,
	IOMapIndex_OUT_D07_battery_2_negative_contactor_CURRENT = 203,
	IOMapIndex_LoopCounter_VALUE            = 252,
	IOMapIndex_EEVAR_HOURMETER_SECONDS_VALUE = 254,
	IOMapIndex_EEVAR_TIME_UNTIL_SERVICE_VALUE = 256,
	IOMapIndex_LoopCounter_STATUS           = 258,
	IOMapIndex_EEVAR_HOURMETER_SECONDS_STATUS = 259,
	IOMapIndex_EEVAR_TIME_UNTIL_SERVICE_STATUS = 260,
	IOMapIndex_EEVAR_ODOMETER_METERS        = 206,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_dp_gain = 208,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_di_gain = 209,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_qp_gain = 210,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_qi_gain = 211,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_speed_p_gain = 212,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_speed_i_gain = 213,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_elec_angle_fwd = 214,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_elec_angle_rvs = 215,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_max_battery_current = 216,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_battery_current_i_gain = 217,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_max_motor_rpm = 218,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_out_gain = 219,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_motor_direction = 220,
	IOMapIndex_EEVAR_TRACTION_SKAI_dp_gain  = 221,
	IOMapIndex_EEVAR_TRACTION_SKAI_di_gain  = 222,
	IOMapIndex_EEVAR_TRACTION_SKAI_qp_gain  = 223,
	IOMapIndex_EEVAR_TRACTION_SKAI_qi_gain  = 224,
	IOMapIndex_EEVAR_TRACTION_SKAI_speed_p_gain = 225,
	IOMapIndex_EEVAR_TRACTION_SKAI_speed_i_gain = 226,
	IOMapIndex_EEVAR_TRACTION_SKAI_elec_angle_fwd = 227,
	IOMapIndex_EEVAR_TRACTION_SKAI_elec_angle_rvs = 228,
	IOMapIndex_EEVAR_TRACTION_SKAI_max_battery_current = 229,
	IOMapIndex_EEVAR_TRACTION_SKAI_motor_scale_p1 = 230,
	IOMapIndex_EEVAR_TRACTION_SKAI_motor_scale_p2 = 231,
	IOMapIndex_EEVAR_TRACTION_SKAI_motor_scale_l1 = 232,
	IOMapIndex_EEVAR_TRACTION_SKAI_motor_scale_l2 = 233,
	IOMapIndex_EEVAR_TRACTION_SKAI_regen_scale_p1 = 234,
	IOMapIndex_EEVAR_TRACTION_SKAI_regen_scale_p2 = 235,
	IOMapIndex_EEVAR_TRACTION_SKAI_regen_scale_l1 = 236,
	IOMapIndex_EEVAR_TRACTION_SKAI_regen_scale_l2 = 237,
	IOMapIndex_EEVAR_TRACTION_SKAI_throttle_gain_1 = 238,
	IOMapIndex_EEVAR_TRACTION_SKAI_throttle_gain_2 = 239,
	IOMapIndex_EEVAR_TRACTION_SKAI_throttle_released = 240,
	IOMapIndex_EEVAR_TRACTION_SKAI_throttle_depressed = 241,
	IOMapIndex_EEVAR_TRACTION_SKAI_battery_current_i_gain = 242,
	IOMapIndex_EEVAR_TRACTION_SKAI_max_motor_rpm = 243,
	IOMapIndex_EEVAR_TRACTION_SKAI_outgain  = 244,
	IOMapIndex_EEVAR_CHARGER_no_of_chargers = 245,
	IOMapIndex_EEVAR_CHARGER_total_current_command = 246,
	IOMapIndex_EEVAR_HYDRAULIC_SKAI_speed_p_gain_time_ramp = 247,
	IOMapIndex_EEVAR_TRACTION_SKAI_elec_fwd_regen = 248,
	IOMapIndex_EEVAR_TRACTION_SKAI_elec_rev_regen = 249,
	IOMapIndex_EEVAR_THROTTLE_DEPRESSED     = 250,
	IOMapIndex_EEVAR_THROTTLE_RELEASED      = 251,
	IOMapIndex_Module1_STATUS               = 204,
	IOMapIndex_Module1_CAN_STATUS           = 205
} enum_IOMapIndexes;

/* Timer Indexes */
typedef enum
{
	TM_LOC_LoopCounter_VALUE                = 0,
	TM_LOC_EEVAR_HOURMETER_SECONDS_VALUE    = 1,
	TM_LOC_EEVAR_TIME_UNTIL_SERVICE_VALUE   = 2,
	TM_LOC_LoopCounter_STATUS               = 3,
	TM_LOC_EEVAR_HOURMETER_SECONDS_STATUS   = 4,
	TM_LOC_EEVAR_TIME_UNTIL_SERVICE_STATUS  = 5
} enum_TimerIndexes;

/* EEProm Indexes */
typedef enum
{
	EE_LOC_EEVAR_ODOMETER_METERS            = 0,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_dp_gain     = 1,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_di_gain     = 2,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_qp_gain     = 3,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_qi_gain     = 4,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_speed_p_gain = 5,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_speed_i_gain = 6,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_elec_angle_fwd = 7,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_elec_angle_rvs = 8,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_max_battery_current = 9,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_battery_current_i_gain = 10,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_max_motor_rpm = 11,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_out_gain    = 12,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_motor_direction = 13,
	EE_LOC_EEVAR_TRACTION_SKAI_dp_gain      = 14,
	EE_LOC_EEVAR_TRACTION_SKAI_di_gain      = 15,
	EE_LOC_EEVAR_TRACTION_SKAI_qp_gain      = 16,
	EE_LOC_EEVAR_TRACTION_SKAI_qi_gain      = 17,
	EE_LOC_EEVAR_TRACTION_SKAI_speed_p_gain = 18,
	EE_LOC_EEVAR_TRACTION_SKAI_speed_i_gain = 19,
	EE_LOC_EEVAR_TRACTION_SKAI_elec_angle_fwd = 20,
	EE_LOC_EEVAR_TRACTION_SKAI_elec_angle_rvs = 21,
	EE_LOC_EEVAR_TRACTION_SKAI_max_battery_current = 22,
	EE_LOC_EEVAR_TRACTION_SKAI_motor_scale_p1 = 23,
	EE_LOC_EEVAR_TRACTION_SKAI_motor_scale_p2 = 24,
	EE_LOC_EEVAR_TRACTION_SKAI_motor_scale_l1 = 25,
	EE_LOC_EEVAR_TRACTION_SKAI_motor_scale_l2 = 26,
	EE_LOC_EEVAR_TRACTION_SKAI_regen_scale_p1 = 27,
	EE_LOC_EEVAR_TRACTION_SKAI_regen_scale_p2 = 28,
	EE_LOC_EEVAR_TRACTION_SKAI_regen_scale_l1 = 29,
	EE_LOC_EEVAR_TRACTION_SKAI_regen_scale_l2 = 30,
	EE_LOC_EEVAR_TRACTION_SKAI_throttle_gain_1 = 31,
	EE_LOC_EEVAR_TRACTION_SKAI_throttle_gain_2 = 32,
	EE_LOC_EEVAR_TRACTION_SKAI_throttle_released = 33,
	EE_LOC_EEVAR_TRACTION_SKAI_throttle_depressed = 34,
	EE_LOC_EEVAR_TRACTION_SKAI_battery_current_i_gain = 35,
	EE_LOC_EEVAR_TRACTION_SKAI_max_motor_rpm = 36,
	EE_LOC_EEVAR_TRACTION_SKAI_outgain      = 37,
	EE_LOC_EEVAR_CHARGER_no_of_chargers     = 38,
	EE_LOC_EEVAR_CHARGER_total_current_command = 39,
	EE_LOC_EEVAR_HYDRAULIC_SKAI_speed_p_gain_time_ramp = 40,
	EE_LOC_EEVAR_TRACTION_SKAI_elec_fwd_regen = 41,
	EE_LOC_EEVAR_TRACTION_SKAI_elec_rev_regen = 42,
	EE_LOC_EEVAR_THROTTLE_DEPRESSED         = 43,
	EE_LOC_EEVAR_THROTTLE_RELEASED          = 44
} enum_EEPromIndexes;

