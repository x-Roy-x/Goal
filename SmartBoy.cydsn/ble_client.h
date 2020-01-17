/******************************************************************************
* Project Name		: PSoC_4_BLE_Central_IAS
* File Name			: BLEclient.h
* Version 			: 1.0
* Device Used		: CY8C4247LQI-BL483
* Software Used		: PSoC Creator 3.1
* Compiler    		: ARM GCC 4.8.4, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT-042-BLE Bluetooth Low Energy Pioneer Kit 
* Owner				: ROIT
*
********************************************************************************
* Copyright (2014-15), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

/********************************************************************************
*	Contains all macros and function declaration used in the BLEclient.c file 
********************************************************************************/
#ifndef BLECLIENT_H
#define BLECLIENT_H
#include "smartboy.h"

/* Maximum number of devices this project will keep track of */
#define CYBLE_MAX_ADV_DEVICES        10u
	
/* BD Address Length*/
#define ADV_ADDR_LEN				0x06
	
/* Public Address indication used for Scanning */
#define PUBLIC_ADDR_TYPE			0x00
	
/* Random Address indication used for Scanning */
#define RANDOM_ADDR_TYPE			0x01
    
extern uint8 restartScanning;
    
extern uint8 ble_state;
    
extern CYBLE_CONN_HANDLE_T connHandle;

extern uint8 goalDevices;
    
extern bool isDataRead;
extern bool isConnecting;

/**************************Function Declarations*****************************/
void HandleScanDevices(CYBLE_GAPC_ADV_REPORT_T* scanReport);
void BleCallBack(uint32 event, void *eventparam);
void set_initial_peripheral_address(void);
void set_peripheral_address_to_connect(uint8 deviceIndex);
CY_ISR_PROTO(Timer_Int_Handler);
void handle_scan(void);
void handle_disconnection(void);
void handle_connection(void);
void handle_scan_results(void);
char * get_device_name(uint8 deviceIndex);
uint8 get_connected_device_id(void);
void check_device_availability_and_connect(uint8 deviceIndex);
void save_data(uint8 data);
uint8 get_data(void);
/****************************************************************************/

#endif
/* [] END OF FILE */
