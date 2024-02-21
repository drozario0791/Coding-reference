/*
** ###################################################################
**     Filename  : reserved.h
**     Processor : Not applicable
**     FileFormat: V1.00
**     Compiler  : Composer Version XX.XX.XX
**     Database  : Name.cdb_00_00
**     IOC       : Name.ioc
**     Date/Time : 07/11/08, 1:05
**     Abstract  :
**         This file defines values that are also defined in the precompiled firmware
**         Editing these values will cause unexpected operation
**
**     (c) Copyright HED, inc. 2006-2008
**     HED, inc.
**     2120 Constitution Ave.
**     Hartford, WI 53089
**     United States
**     http      : www.hedonline.com
**     mail      : support@hedonline.com
** ###################################################################
*/
#define PROTOTYPES_DM1_DM2_BAM_HEADER_FILE          "Prototypes_DM1_DM2_BAM.h"
#define PROTOTYPES_CAN_HEADER_FILE                  "Prototypes_CAN.h"
#define PROTOTYPES_SERIAL_HEADER_FILE               "Prototypes_Serial.h"
#define PROTOTYPES_APP_CONFIG_HEADER_FILE           "Prototypes_App_Configurator.h"
#define PROTOTYPES_TIME_HEADER_FILE                 "Prototypes_Time.h"
#define PROTOTYPES_J1708_HEADER_FILE                "Prototypes_J1708.h"
#define PROTOTYPES_LOW_POWER_HEADER_FILE    		"Prototypes_Low_Power.h"


/*---------------------------------------------------------------------------
*  Serial Flash RAM
*---------------------------------------------------------------------------
*/
#define EXTERNAL_MEMORY_BASE_ADDRESS               0x80000000
#define EXTERNAL_MEMORY_SECTOR_SIZE                256
#define NUMBER_OF_EXTERNAL_MEMORY_SECTORS          8		

#define  IO_MAP_SIZE                         2048     /*This is how big the IOmap is, holds all ins, outs, module status, J1939, J1939 Bam, Variables, EEPROMS*/
#define  CONFIGURATOR_FILES_WRITTEN_WORD     0x1111
#define  MAX_J1708_LENGTH                    30
#define  NUMBER_OF_STATE_MACHINES            100       /*Maximum number of State Machines that can be used */
#define  MAX_RX_MESSAGE_SIZE                 150      /*biggest message it can recieve, Conductor Max = , Programmer Max = */
#define  NUMBER_OF_BAM_MESSAGES              8
#define  NUMBER_SPN_PER_BAM                  16
#define UNUSED_PARAMETER  (void)
#define PARAMETER_NOT_APPLICABLE             0	
#define _PTR_ *
#define  FLASH_POINTER_TYPE             uint32_t _PTR_
#ifdef USE_PAGED_RAM 
#define USER_RAM 		#pragma DATA_SEG __RPAGE_SEG PAGED_RAM_USER
#define DEFAULT_RAM  	#pragma DATA_SEG DEFAULT
#else
#define USER_RAM     	/* Leave Blank*/
#define DEFAULT_RAM 	/* Leave Blank*/
#endif
