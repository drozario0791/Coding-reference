/*
** ###################################################################
**     Filename  : Prototypes_USB.h
**     Processor : Not applicable
**     FileFormat: V2.00
**     Abstract  :
**         This file contains all the function and structure prototypes that are required for 
**         USB operation - if supported by module hardware
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

#ifndef PROTOTYPES_USB_H_
#define PROTOTYPES_USB_H_

#include "typedefs.h"

/* Values for CoProcStatus internal input*/
typedef enum
{
	CSB_SYSTEM_READY, 		 		/* Set when everything is running fine */
	CSB_MODEM_NOT_RESPONDING,		/* Set when modem is not responding -
										may not be an error if no modem is present*/
	CSB_SD_CARD_FAIL,         		/* Set when SD Card fails initialization */
	/* Only one of the following bit will ever be set at a time */
	CSB_USB_DISABLED, 				/* Set when the USB is Disabled*/
	CSB_USB_VIRTUAL_COM_PORT, 		/* Set when the USB is VCom Mode*/
	CSB_USB_MASS_STORAGE_DEVICE, 	/* Set when the USB is Mass Storage Mode*/
	CSB_USB_HOST,					/* Set when the USB is Host*/
	
	RESERVED_SD_CARD_FORMAT,  /* Set when SD Card is auto formatted due to a failed initialization - 
									   this is a drastic response to an otherwise very specific unrecoverable error
									   the only known way to possibly achieve this is to pull power on the module 
									   while reformatting in mass storage device mode - even that is really hard to time correctly  
									 */
	CSB_TELEMATICS_CACHING_FAIL	    /* Set when the telematics caching function detects a failure*/
	
}COPROC_STATUS_BITS_;


/* Values to control the USB operation */
/* NOTE** MASS Storage mode disable application write access to the internal SD card - this prevents
 * Telematics, and data logging from occurring.
 */
typedef enum {
	USB_DISABLED,							/* Disabled all activity from USB port -- good way to prevent unauthorized access */
	USB_VIRTUAL_COM_PORT,					/* USB default mode - allows Conductor, or Serial Downloader, or Terminal debugger access*/
	USB_MASS_STORAGE_DEVICE,				/* USB Mass Storage Mode - looks like a USB Flash drive when connected to a PC*/
	USB_HOST,								/* USB Host Mode- will read USB flash drives connected to it*/
	USB_HOST_CHECK_FOR_VALID_FTDI_SOFTWARE, /* queries the thumb drive to determine if there is a software file for the FTDI 
											 * USB processor*/
	USB_HOST_REPROGRAM_FTDI,                /* re-programs the FTDI USB processor with the software on the thumb drive, 
											 * NOTE** USB re-program takes a couple minutes, the USB will not be responsive during this time, 
											 * do not turn the unit off during programming*/
	USB_HOST_CHECK_FOR_VALID_CPU_SOFTWARE , /* queries the thumb drive to determine if there is a software file for the main processor*/
	USB_HOST_REPROGRAM_CPU                  /* Do not change to this mode.  If you want to re-program the unit, call the jump to boot code function*/
}USB_MODE_;

typedef enum {
	USB_MODE_STATUS_IDLE,					/* USB not changing modes*/
	USB_MODE_STATUS_WORKING,				/* USB in process of changing modes*/
	USB_MODE_STATUS_SUCCESS,				/* Commanded USB mode is now active*/
	USB_MODE_STATUS_FAIL,					/* Commanded USB mode failed to change*/
	USB_MODE_NOT_SUPPORTED,					/* Commanded USB mode not supported by hardware*/
	USB_MODE_STATUS_HOST_DRIVE_NOT_FOUND,   /* searched for a thumb drive but did not find one*/
	USB_MODE_STATUS_HOST_NO_FOLDER_FOUND,   /* the specific folder that the software is supposed to be
	                                         * in was not found on the connected thumb drive*/
	USB_MODE_STATUS_HOST_DRIVE_BOMS_FAILED, /* the USB processor failed when loading the BOMS 
	                                         *(Bulk Only Mass Storage) driver*/
	USB_MODE_STATUS_HOST_DRIVE_FAT_FAILED,  /* the USB processor failed when loading the FAT (File Allocation Table) driver*/
	USB_MODE_STATUS_HOST_NO_SOFTWARE_FOUND, /* the software file could not be located*/
	USB_MODE_STATUS_HOST_SOFTWARE_FOUND,    /* the software file was located*/
	USB_MODE_STATUS_HOST_SOFTWARE_INVALID   /* the software file was located but it was invalid*/
	
}USB_MODE_STATUS_;

/*
 * This function switches the USB port between different modes of operation
 * it is paired with GetUSBModeStatus() to determine when it is finished
 * Parameters:
 * module_id is the module id of the unit to set the USB on. see Constants.h for valid IDs
 * mode = the new mode requested < See USB_MODE enumeration for details>
 */
void SetUSBMode(uint8_t module_id, USB_MODE_ mode);

/*
 * This function reports the status of SetUSBMode call it could take up to 11 seconds to 
 * succeed
 * Return:
 * < See USB_MODE_STATUS_ enumeration for details>
 */
USB_MODE_STATUS_ GetUSBModeStatus();

/*
 * This function interrogates the module to see if a USB mode is supported
 * Return:
 * TRUE if the module supports the USB mode passed in
 */
bool_t USBModeSupported(USB_MODE_ mode);
/*
 * This will get the current version of the FTDI software on the chip
 * Return: example 0x0102 
 */
uint16_t GetFTDIVersion(void);

#endif /* PROTOTYPES_USB_H_ */
