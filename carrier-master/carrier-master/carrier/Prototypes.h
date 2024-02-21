/*
** ###################################################################
**     Filename  : Prototypes.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**         This file contains all the miscellaneous standard functions and prototypes
**         available to the User
**
**     (c) Copyright HED,Inc. 2006-2016
**     HED,Inc.
**     2120 Constitution Ave.
**     Hartford, WI 53027
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** #########################################################################
**                            !!!  WARNING !!!
**     ALTERING THIS FILE IN ANY WAY MAY CAUSE PERSONAL INJURY AND/OR DEATH.
** #########################################################################
*/
#ifndef _PROTOTYPES_H_
#define _PROTOTYPES_H_
#include "reserved.h"
#include "typedefs.h"

/* Below is a list of Prototype files taht a included based on the feature set of the
 * module being used. The feature set can be found in reserved.h
 */
#ifdef PROTOTYPES_ACCEL_HEADER_FILE
#include PROTOTYPES_ACCEL_HEADER_FILE
#endif
#ifdef PROTOTYPES_DM1_DM2_BAM_HEADER_FILE
#include PROTOTYPES_DM1_DM2_BAM_HEADER_FILE
#endif
#ifdef PROTOTYPES_CAN_HEADER_FILE
#include PROTOTYPES_CAN_HEADER_FILE
#endif
#ifdef PROTOTYPES_DATALOGGER_HEADER_FILE
#include PROTOTYPES_DATALOGGER_HEADER_FILE
#endif
#ifdef PROTOTYPES_J1708_HEADER_FILE
#include PROTOTYPES_J1708_HEADER_FILE
#endif
#ifdef PROTOTYPES_OTA_HEADER_FILE
#include PROTOTYPES_OTA_HEADER_FILE
#endif
#ifdef PROTOTYPES_RTCC_HEADER_FILE
#include PROTOTYPES_RTCC_HEADER_FILE
#endif
#ifdef PROTOTYPES_SERIAL_HEADER_FILE
#include PROTOTYPES_SERIAL_HEADER_FILE
#endif
#ifdef PROTOTYPES_APP_CONFIG_HEADER_FILE
#include PROTOTYPES_APP_CONFIG_HEADER_FILE
#endif
#ifdef PROTOTYPES_TELEMATICS_HEADER_FILE
#include PROTOTYPES_TELEMATICS_HEADER_FILE
#endif
#ifdef PROTOTYPES_TIME_HEADER_FILE
#include PROTOTYPES_TIME_HEADER_FILE
#endif
#ifdef PROTOTYPES_USB_HEADER_FILE
#include PROTOTYPES_USB_HEADER_FILE
#endif
#ifdef PROTOTYPES_CUSTOM_HEADER_FILE
#include PROTOTYPES_CUSTOM_HEADER_FILE
#endif
#ifdef PROTOTYPES_LOW_POWER_HEADER_FILE
#include PROTOTYPES_LOW_POWER_HEADER_FILE
#endif

/* standard commands for digital outputs */
#define OUTPUT_OFF                                   0, 0
#define OUTPUT_ON                                    1000, 0
#define OUTPUT_ON_FLASH                              1000, 1

/*
 * When comparing an input’s IOMap value these enumerations are used
 */
typedef enum
{
    INPUT_INVALID = -1,
    INPUT_OFF,                    /* The digital input is off */
    INPUT_ON,                     /* The digital input is on */
    INPUT_DISABLED,               /* The digital input is either not configured, its debounce
                                     time not expired on boot up, or the i/o module it is assigned to is missing */
    INPUT_ANALOG_OR_FREQ          /* This value is for internal use only and can be ignored */
} Input_State_t;

typedef Input_State_t INPUT_STATE_;

/*
 * Output Status States
 * When comparing an output’s IOMap status these enumerations are used
 */
typedef enum
{
    STATUS_NORMAL,              /* the output does not have any faults*/
    STATUS_OPEN,                 /* the output does not have a load connected to it or the current is lower than the open circuit threshold*/
    STATUS_GROUND,               /* the output is connected directly to ground or for a sourcing output it doesn’t have its battery supply connected*/
    STATUS_BATTERY,              /* the output is connected directly to battery or for a sinking output it doesn’t have its ground supply connected*/
    STATUS_OVER_CURRENT,         /* the output is drawing more current (average or peak) then the output is rated/configured for*/
    STATUS_GROUNDED,             /* this is only applicable to servo outputs and one of the pins is connected to ground and should not be*/
    STATUS_STB_OR_OPEN,          /* for sourcing outputs this is a state where it is either shorted to battery or open –*/
    /*(some modules will switch to the state it is in, however some modules cannot detect the difference)*/
    STATUS_OVER_UNDER_VOLTAGE,   /* the voltage is either over or under the FET’s rated operating range.  */
    STATUS_OVER_TEMP,            /* the FET’s internal temperature is too high most likely do to driving too much current*/
    STATUS_STG_OR_OPEN,          /* for sinking outputs this is a state where it is either shorted to ground or open – */
    /*(some modules will switch to the state it is in, however some modules cannot detect the difference)*/
    FORCE_TO_16_BIT_SIZE = 0x100 /* for internal purposes only */
} OUTPUT_STATUS_;

/*
 * This sets the module’s mode for open circuit when off
 * NOTE** this is a hardware specific function, consult the module’s data sheet to determine if it is supported
 * NOTE** When enabled this may make LED’s glow and/or buzzer's buzz
 */
typedef enum
{
    DPU_ALWAYS_OFF = 1,         /* the open circuit when off is always disabled*/
    DPU_ALWAYS_ON,              /* the open circuit when off is always enabled
                                   NOTE** the module cannot determine the difference between Open and STB in this state)*/
    DPU_TOGGLE,                 /* the open circuit when off will toggle on and off every 500 milliseconds*/
    DPU_UNINITIALIZED = 0xff    /* for internal use only*/
} DPU_MODES_;

/* When comparing a module’s IOMap value these enumerations are used.
 * The easiest way to use this is S
 if (Module_State != MODULE_RUNNING_STATE)
 {
 (this module has an error)
 }
 else
 {
 (this module is operating properly)
 }
*/
typedef enum
{
    MODULE_MISSING_STATE,             /* the master cannot find the i/o module on the CAN bus*/
    MODULE_NEEDS_ASSIGNMENT_STATE,    /* the master found the i/o module on the CAN bus*/
    /* and is beginning the configuration process*/
    MODULE_WAKE_UP_STATE,             /* the master is continuing the configuration process*/
    MODULE_NEEDS_CONFIGS_STATE,       /* the master is actively configuring the i/o module*/
    MODULE_RUNNING_STATE,             /* the i/o module has finished getting configured and is now running*/
    MODULE_IN_COMMFAIL_STATE,         /* the i/o module was running but now is not available on the CAN bus*/
    MODULE_GOT_SOFT_RESET_STATE,      /* the module was running but got the soft reset message*/
    MODULE_IN_STUB_STATE,             /* the i/o module is waiting for reprogramming*/
    MODULE_NEEDS_SAVE_CONFIGS_STATE,  /* the i/o module has been configured and needs to be told */
    /* to save the configuration*/
    MODULE_DISABLED,                  /* this i/o module is not active based on EEPROM settings*/
    MODULE_ON_WRONG_CAN_LINE          /* the i/o module is in the master’s project, however it is not connected*/
    /* to the correct CAN line on the master*/
} MODULE_STATE_;

/*
 * This is an additional state of the i/o modules that should be checked if the module is in RUNNING Mode
 * The master module determines the state of the CAN lines on i/o modules
 * For an i/o module with only 1 CANLink line, it will always be SINGLE_CAN
 * For an i/o module with 2 CANLink Lines and configured as a Dual Can i/o module in composer it will be one of the other 3 states
 */
typedef enum
{
    SINGLE_CAN,
    DUAL_CAN_OK,     /* DUAL_CAN_OK -> both CANLink lines are operating properly*/
    DUAL_CAN_1_BAD,  /* DUAL_CAN_1_BAD -> the i/o module is running on the secondary
                      * line but the primary is not functioning*/
    DUAL_CAN_2_BAD   /* DUAL_CAN_2_BAD -> the i/o module is running on the primary line
                      * but the secondary is not functioning*/
} CAN_STATE_;

/* Conductor connection modes */
typedef enum
{
    CONDUCTOR_NO_COMM_MODE,      /* Conductor is not connected */
    CONDUCTOR_CHANGE_COMM_MODE,  /* Conductor is in the process of connecting */
    CONDUCTOR_WAIT_MODE,         /* Conductor is in the process of connecting */
    CONDUCTOR_RUNNING_MODE,      /* Conductor is currently connected */
    CONDUCTOR_DEBUG_MODE         /* Conductor is connected and debugging */
} CONDUCTOR_MODES_;


typedef enum
{
    REGULATED_SUPPLY_OFF, /*Turns Regulated Supply off */
    REGULATED_SUPPLY_ON,  /*Turns Regulated Supply on */
    REGULATED_SUPPLY_NOT_INITIALIZED  /* For internal use only */
} REGULATED_SUPPLY_SETTING_;

typedef enum
{
    /* status created by software */
    FLASH_COMMAND_SUCCESSFUL,           /* Flash command was successfully executed*/
    FLASH_DIVIDER_NOT_SET,              /* Internal error*/
    FLASH_COMMAND_IN_PROGRESS,          /* Flash command currently executing when a second command was received*/
    FLASH_NOT_START_OF_SECTOR_ERROR,     /* Internal error*/
    FLASH_ADDRESS_ERROR,                    /* Block Address invalid - not 8 bit aligned -
                                               NOTE** some modules may have different alignment requirements*/
    FLASH_NOT_ERASED,                   /* Flash tried to wirte over location that was already written to*/
    FLASH_NO_DATA,                      /* Data size zero*/
    FLASH_SIZE_TOO_LARGE,                /* Data size too large*/
    FLASH_PROGRAM_FAILED,               /* Flash command failed*/

    /* status created by hardware */
    FLASH_ACCESS_ERROR,                 /* Internal error*/
    FLASH_PROTECTION_ERROR,             /* Internal error*/
    FLASH_CORRECTABLE_READ_ERROR,       /* Internal error*/
    FLASH_NON_CORRECTABLE_READ_ERROR,   /* Internal error*/
    FLASH_SINGLE_BIT_FAULT,             /* Internal error*/
    FLASH_DOUBLE_BIT_FAULT,             /* Internal error*/
    EEE_SECTOR_FORMAT_ERROR,                /* Internal error*/
    EEE_SECTOR_STATE_CHANGE_ERROR,      /* Internal error*/
    EEE_PROTECTION_VIOLATION_ERROR,     /* Internal error*/
    EEE_PROGRAM_ERROR,                  /* Internal error*/
    EEE_ERASE_ERROR                     /* Internal error*/

} FLASH_COMMAND_STATUS_;

/* values for the user */
typedef struct
{
    uint16_t     ui16_analog_value;
    bool_t      available_analog_value_flag;     /* if flag false, the analog value return invalid; if flag ture, the analog value return valid */
} Analog_Values_;

/*Used to control an h-bridge output using the Update_H_Bridge() function*/
typedef enum
{
    HB_Forward = 0,
    HB_Reverse = 1,
    HB_SinkBrake = 2,
    HB_SourceBrake = 3,
    HB_Off = 4
} enum_HB_Control_t;

/*Used to identify which h-bridge output to control using the Update_H_Bridge() function*/
typedef enum
{
    HBRIDGE1 = 0,
    HBRIDGE2 = 1,
    HBRIDGE3 = 2,
    HBRIDGE4 = 3
}enum_HB_Numbers_t;

typedef enum
{
    SET_ALL,
    SET_ONLY_SAVE_ON_SHUTDOWN,
    SET_BASED_ON_ACCESS_LEVEL,
    SET_INDIVIDUAL
} Orch_NV_Set_Modes_t;

typedef enum
{
    WRITE_LEVELS_0_TO_3 = 0,
    WRITE_LEVELS_1_TO_3 = 1,
    WRITE_LEVELS_2_TO_3 = 2,
    WRITE_LEVEL_3 = 3,
    NUMBER_OF_VALID_ACCESS_LEVELS
}Orch_NV_Access_Levels_t;

/* errors logged by the system Firmware */
typedef enum
{
    NO_SYSTEM_ERROR,
    INVALID_ERROR_INDEX,
    EERPOM_TIMESTAMP_CHANGE_SET_TO_DEFAULTS,
    EEPROM_WRITE_ERROR,
    EEPROM_READ_ERROR,
    TIMER_TIMESTAMP_CHANGE_SET_TO_DEFAULTS,
    TIMER_WRITE_ERROR,
    TIMER_READ_ERROR,
    TIMESTAMPS_WRITE_ERROR,
    OUTPUT_CALIBRATION_READ_FAILURE,
    MEMORY_READ_ERROR
} System_Error_t;

/* states for the CAN receive status */
typedef enum
{
    CLEAR,
    RECEIVED
} CanRxStatus_t;

/*
 * Global variables used by the Firmware and User code
 * Temporary variable to set the state machines, the actual state machine value
 * is not updated until the end of the loop
 */
#if NUMBER_OF_BAM_MESSAGES >0
#ifdef USE_PAGED_RAM
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM_STATE_MACHINES
#endif
extern uint16_t  StateMachineTempState[NUMBER_OF_STATE_MACHINES];
#ifdef USE_PAGED_RAM
#pragma DATA_SEG DEFAULT
#endif
#endif
/*
 * IOMap is where all the data items values are stored, depending on processor type it
 * could move into paged memory
 */
#if IO_MAP_SIZE > 0
#ifdef USE_PAGED_RAM
#pragma DATA_SEG __RPAGE_SEG PAGED_RAM_IO_MAP
#endif

extern uint16_t IOMap[IO_MAP_SIZE];

#ifdef USE_PAGED_RAM
#pragma DATA_SEG DEFAULT
#endif
#endif
/*
 * This function updates the output command
 * Parameters:
 * module_id  and number = are combined into a single macro #define in constants.h
 * value = the value that you want to set the output command to
 * flash = TRUE if you want the output to flash
 */
void Update_Output(uint8_t module_id, uint8_t number, uint16_t value, bool_t flash);

/*
 * This function updates the heart beat
 * Parameters:
 * turn_on = TRUE lights up the heart beat LED
 */
void Heart_Beat_Control(bool_t turn_on);


/*
 * This function updates the 5 volt supply or the output diagnostic mode
 * Parameters:
 * module_id = which module you want to update, you can use #define for module number in Constants.h
 * dpu = sets the mode of the diagnostic, <see the Dpu_Modes_ enumeration>
 * five volts = turns the 5 volt supply on and off <see the Regulated_Supply_Setting_ enumeration>
 */
void Update_Dpu_5v(uint8_t module_id, DPU_MODES_ dpu, REGULATED_SUPPLY_SETTING_ five_volts);


/*
 * This function writes blocks of memory into flash
 * NOTE** that the words have to be erased prior to writing
 * NOTE** Due to hardware limitations all current modules have a minimum number of bytes
 *        that can be written at one time which is 8
 * Parameters:
 * address_source = the address of the data to be written into Flash
 * FAR_T_address_destination = the global address to write to
 * count = the number of bytes to write
 *
 * return
 * the status of the write <see Flash_Command_Status_ enumeration for details>
 */
FLASH_COMMAND_STATUS_ Flash_Write_Block(FLASH_POINTER_TYPE address_source,
                                        FLASH_POINTER_TYPE address_destination,
                                        uint32_t count);

/*
 * This function erases sectors of flash
 * NOTE** that the address needs to be at the start of the sector
 * NOTE** Due to hardware limitations all current modules have a minimum number of bytes
 *        that can be erased at one time, which is one full sector of FLASH - a end address shorter
 *        than the end of the sector will still result in a full sector erase
 *
 * Parameters:
 * start_address = the global address of the start of the sector to erase
 * end_address = the global address of the end of the sector to erase -
 *
 * return
 * the status of the erase <see Flash_Command_Status_ enumeration for details>
 */
FLASH_COMMAND_STATUS_ Flash_Erase_Block(FLASH_POINTER_TYPE start_address,
                                        FLASH_POINTER_TYPE end_address);

/*
 * This function reads blocks of memory from flash
 * On internal flash, this allows the module to use CPU error checking and correction if available
 * Will read data from external flash if available on the module
 * Parameters:
 * source = the address of the data to be read from Flash
 * destination = a pointer or array to read data into
 * size = the number of bytes to read
 *
 * return
 * the status of the write <see Flash_Command_Status_ enumeration for details>
 */
FLASH_COMMAND_STATUS_ Flash_Read_Block(FLASH_POINTER_TYPE source,
                                       uint8_t _PTR_ destination, uint32_t size);

/*
 * This function reads the flash and if there are any ECC errors detected, it erases sectors of flash to clear the errors
 * NOTE** that the address needs to be at the start of the sector
 * NOTE** Due to hardware limitations all current modules have a minimum number of bytes
 *        that can be erased at one time, which is one full sector of FLASH - a end address shorter
 *        than the end of the sector will still result in a full sector erase
 *
 * Parameters:
 * start_address = the  address of the start of the sector to erase
 * end_address = the  address of the end of the sector to erase -
 *
 * return
 * if an error was found or not
 */
bool_t Flash_Integrity_Check(FLASH_POINTER_TYPE start_address,
                             FLASH_POINTER_TYPE end_address);

/*
 * This function gets the conductor communication status
 * return:
 * status of Conductor <see Conductor_Modes_ enumeration for details>
 */
CONDUCTOR_MODES_ Get_Conductor_Status(void);

/*
 * This function gets the conductor access level
 * return:
 * user's access level 0 is most restricted
 */
Orch_NV_Access_Levels_t Get_Conductor_Access_Level(void);

/*
 * OrchestraDIEepromSetDefaultsToIomap
 * based on what is passed in it will load all, some or one iomap spot with the default value
 * mode -> sets which eeproms to update in the iomap
 * eeprom_number -> used only on individual, sets which individual eeprom should be modified
 * access_level -> sets the access level of the user trying to do the operation, only will update eeproms that the user has access to
 */
void OrchestraDIEepromSetDefaultsToIomap(Orch_NV_Set_Modes_t mode, uint16_t eeprom_number, Orch_NV_Access_Levels_t access_level);

/*
 * OrchestraDIEepromSetNVMemoryToIomap
 * based on what is passed in it will load all, some or one iomap spot with the saved value
 * mode -> sets which eeproms to update in the iomap
 * eeprom_number -> used only on individual, sets which individual eeprom should be modified
 * access_level -> sets the access level of the user trying to do the operation, only will update eeproms that the user has access to
 */
void OrchestraDIEepromSetNVMemoryToIomap(Orch_NV_Set_Modes_t mode, uint16_t eeprom_number, Orch_NV_Access_Levels_t access_level);

/*
 * OrchestraDIEepromWriteToNVMem
 * based on what is passed in it will update\write all, some or one iomap spot into nonvolatile memory
 * every write will write all the values, just a matter if it writes IOmap or current value in nonvolatile memory
 * mode -> sets which eeproms to update in the iomap
 * eeprom_number -> used only on individual, sets which individual eeprom should be modified
 * access_level -> sets the access level of the user trying to do the operation, only will update eeproms that the user has access to
 */
bool_t OrchestraDIEepromWriteToNVMem(Orch_NV_Set_Modes_t mode, uint16_t eeprom_number, Orch_NV_Access_Levels_t access_level);

/*
 * OrchestraDIEepromSetDefaultsToIomap
 * based on what is passed in it will load all, some or one iomap spot with the default value
 * mode -> sets which eeproms to update in the iomap
 * eeprom_number -> used only on individual, sets which individual eeprom should be modified
 * access_level -> sets the access level of the user trying to do the operation, only will update eeproms that the user has access to
 */
void OrchestraDITimerSetDefaultsToIomap(Orch_NV_Set_Modes_t mode, uint16_t timer_number, Orch_NV_Access_Levels_t access_level);

/*
 * OrchestraDIEepromSetNVMemoryToIomap
 * based on what is passed in it will load all, some or one iomap spot with the saved value
 * mode -> sets which eeproms to update in the iomap
 * eeprom_number -> used only on individual, sets which individual eeprom should be modified
 * access_level -> sets the access level of the user trying to do the operation, only will update eeproms that the user has access to
 */
void OrchestraDITimerSetNVMemoryToIomap(Orch_NV_Set_Modes_t mode, uint16_t timer_number, Orch_NV_Access_Levels_t access_level);

/*
 * OrchestraDIEepromWriteToNVMem
 * based on what is passed in it will update\write all, some or one iomap spot into nonvolatile memory
 * every write will write all the values, just a matter if it writes IOmap or current value in nonvolatile memory
 * mode -> sets which eeproms to update in the iomap
 * eeprom_number -> used only on individual, sets which individual eeprom should be modified
 * access_level -> sets the access level of the user trying to do the operation, only will update eeproms that the user has access to
 */
bool_t OrchestraDITimerWriteToNVMem(Orch_NV_Set_Modes_t mode, uint16_t timer_number, Orch_NV_Access_Levels_t access_level);


/*
 * This function gets firmware version number
 * module_id is the module to send the message out on (0-F) - This value is available in constant.h
 * return = the version number
 * example: version 2.01 will return 0x0201, version 3.20 will return 0x0320
 */
uint16_t Get_Firmware_Version(uint8_t module_id);

/*
 * This function gets application version number defined in Orchestra is
 * return = the version number
 * example: version 03-01 will return 0x0301, version 00-10 will return 0x0010
 */
uint16_t Get_Application_Version(void);

/*
 * This function calculates the CRC for the memory between Start and Finish
 * Return the CRC value
 */
uint16_t CRCcalc(uint16_t *FAR_T start, uint16_t *FAR_T finish);

/*
 * This function interpolates between points (X1, Y1) and (X2, Y2). The input value passed in
 * should be between X1 and X2, however it is not necessary since the input is plugged into the
 * equation of a line which extends beyond the points (X1, Y1) and (X2, Y2). The calculation used
 * here is derived from y = mx + b, where the input is x, and points (X1, Y1) and (X2, Y2) are
 * substituted into m and b
 */
int16_t Interpolate(int16_t si16_x_input,
                    int16_t si16_x1,
                    int16_t si16_x2,
                    int16_t si16_y1,
                    int16_t si16_y2);

/*
 * This function will cause the module to jump to boot code and wait for programming of new
 * firmware/application
 */
void JumpToBootCode(void);

/*
 * This function will perform integer division with a proper rounding of the result, it should be used
 * in place of / for averaging functions  x / y = return
 */
int32_t divide(int32_t x,int32_t y);

/*
 * Call this function to read analog value of input.
 *
 * Software_input_number is the input software number of the input you
 * want to read (since it is on master module it lines up with the
 * IOMap address).
 *
 * Frequency sets the amount of time the input is sampled over.  For
 * example 50 Hz -> 20 msec, 100 Hz, 10 msec, 200 Hz 5 msec.
 *
 * return the Analog_Values_ structure
 *
 * NOTE: If the frequency is set such that it takes longer to sample
 * the input than the loop time, the previous value will be reported
 * until the sampling is finished.
 *
 * In some cases the inputs on the module are run through a
 * multiplexer, in these cases having one long sample time will affect
 * all the inputs since the multiplexer cannot be flipped until all
 * the inputs have finished sampling.
 */
Analog_Values_  Read_Analog_Value_Inputs(int8_t software_input_number, int16_t frequency);

/*
 * This function is used to update an H-bridge output
 * module_id - the name of the module in the Orchestra project (see Constants.h)
 * h_bridge_number - which h-bridge to update (see the enum_HB_Numbers_t enumeration in this file)
 * control - the function to have the h-bridge perform (see the enum_HB_Control_t enumeration in this file)
 * duty_cycle - the PWM duty cycle to command the h-bridge
 */
bool_t Update_H_Bridge(uint8_t module_id, uint8_t h_bridge_number, enum_HB_Control_t control, uint16_t duty_cycle);

/*
 * This function returns the number of errors that are currently active in the firmware
 */
uint8_t GetNumberOfSystemErrors(void);

/*
 * This function returns the specific errors that are active.
 */
System_Error_t GetSystemError(uint8_t error_index);


/*
 * This function will clear the error
 * returns True if it cleared the error and false if the error was not found
 */
bool_t ClearSystemError(System_Error_t error);

#ifdef PROTOTYPES_CUSTOM_FLASH_HEADER_FILE
#include PROTOTYPES_CUSTOM_FLASH_HEADER_FILE
#endif

#endif  /*  _PROTOTYPES_H_  */
