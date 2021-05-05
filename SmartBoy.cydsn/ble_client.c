#include "smartboy.h"

/* 'list_of_devices' is an array of type 'CYBLE_GAPC_ADV_REPORT_T' (defined in 
 * BLE_StackGap.h) that is used to store the reports of the peripheral devices
* being scanned by this Central device*/
CYBLE_GAPC_ADV_REPORT_T 	list_of_devices[CYBLE_MAX_ADV_DEVICES];

/* 'connectPeriphDevice' is a variable of type 'CYBLE_GAP_BD_ADDR_T' (defined in 
* BLE_StackGap.h) and is used to store address of the connected device. */
CYBLE_GAP_BD_ADDR_T 		connectPeriphDevice;

/* 'connHandle' is a variable of type 'CYBLE_CONN_HANDLE_T' (defined in 
* BLE_StackGatt.h) and is used to store the connection handle parameters after
* connecting with the peripheral device. */
extern CYBLE_CONN_HANDLE_T connHandle;


/* 'ble_state' stores the state of connection which is used for updating LEDs */
uint8 ble_state = BLE_DISCONNECTED;

/* 'address_store' stores the addresses returned by Scanning results */
uint8 address_store[10][6];

/* 'peripheralAddress' stores the addresses of device presently connected to */
uint8 peripheralAddress[6];

/* Goal primary adress (from 2nd to 6th byte) for compare 
to determine is it goal device or no, first byte don't compare*/
uint8 goalCompareAddress[6] = {
    0x00, 0x34, 0x12, 0x50, 0xA0, 0x00
};

/* 'peripheralFound' flag tells whether the peripheral device with the required Address
 * has been found during scanning or not. */
uint8 peripheralFound = FALSE;

/* 'addedDevices' tells the number of devices that has been added to the list till now. */
uint8 addedDevices = FALSE;

uint8 savedData;

bool isDataRead = false;

bool isConnecting;
CY_ISR(Timer_Int_Handler) {
    Timer_Stop();
    stop_scan();
    handle_scan_results();
}

// Call back event function to handle various events from BLE stack 
void BleCallBack(uint32 event, void *eventparam) {
	/* 'apiResult' is a variable of type 'CYBLE_API_RESULT_T' (defined in 
	* BLE_StackTypes.h) and is used to store the return value from BLE APIs. */
	CYBLE_API_RESULT_T 			apiResult;
	
	/* 'scan_report' is a variable of type 'CYBLE_GAPC_ADV_REPORT_T' (defined in 
	* BLE_StackGap.h) and is used to store report retuned from Scan results. */
	CYBLE_GAPC_ADV_REPORT_T		scan_report;
    
    //CYBLE_GATTC_READ_BY_TYPE_REQ_T readByTypeReqParam;
	//CYBLE_GATT_ATTR_HANDLE_RANGE_T range;
    //CYBLE_UUID_T uuid;
    CYBLE_GATTC_READ_BY_GRP_RSP_PARAM_T readResponse;
	CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *wrReqParam;
    
    bool ble_mode = get_ble_mode();
    
	switch(event) {
		case CYBLE_EVT_STACK_ON:
			/* Set start scanning flag to allow calling the API in main loop */
            if(!ble_mode){
                start_advertisement();
                //break;
            }
		    break;

		case CYBLE_EVT_GAPC_SCAN_START_STOP:
			/* Add relevant code here pertaining to Starting/Stopping of Scan*/
			if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED) {
				ble_state = BLE_DISCONNECTED;
				
				if(!peripheralFound) {
					/* Restart Scanning */
					Status_LED_Red_Write(1);
				}
			} else {
				ble_state = BLE_SCANNING;
			}
		break;
			
		case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
			/* This event is generated whenever there is a device found*/
			if(CYBLE_STATE_CONNECTED != CyBle_GetState())	 {
				/* If we are not connected to any peripheral device, then save the new device  
					information so to add it to our list */
				scan_report = *(CYBLE_GAPC_ADV_REPORT_T*) eventparam;
				
				/* Add the new device to existing list if not done yet, and compare the address with our
					required address.*/
				HandleScanDevices(&scan_report);
			}
			/* Update the LED status for BLE scanning mode*/
			ble_state = BLE_SCANNING;
		break;
			
 		case CYBLE_EVT_GATT_CONNECT_IND:

			/* When the peripheral device is connected, store the connection handle.*/
            connHandle = *(CYBLE_CONN_HANDLE_T *)eventparam;
            break;
			
		case CYBLE_EVT_GATT_DISCONNECT_IND:
			/* When the peripheral device is disconnected, reset variables*/
			
			break;
			
		case CYBLE_EVT_GAP_DEVICE_CONNECTED:
			
			/* The Device is connected now. Start Attributes discovery process.*/
			apiResult = CyBle_GattcStartDiscovery(connHandle);
			
			if(apiResult != CYBLE_ERROR_OK) {

			}
			
			/* Update the LED status for BLE discovery mode*/
			ble_state = BLE_SERVICE_DISCOVERY;
            break;
			
		case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            //
            //
            if(ble_mode){
    			/* Reset all saved peripheral Addresses */
    			for(int i = 0; i < addedDevices; i++) {
    				list_of_devices[i].peerBdAddr[0] = FALSE;
    				list_of_devices[i].peerBdAddr[1] = FALSE;
    				list_of_devices[i].peerBdAddr[2] = FALSE;
    				list_of_devices[i].peerBdAddr[3] = FALSE;
    				list_of_devices[i].peerBdAddr[4] = FALSE;
    				list_of_devices[i].peerBdAddr[5] = FALSE;
    			}
    			
    			/* Reset application Flags on BLE Disconnect */
    			addedDevices = FALSE;
    			peripheralFound = FALSE;
    			ble_state = BLE_DISCONNECTED;
    			
    			
    			/* Update LED Status for Disconnection */
    			HandleLEDs(ble_state);
    			if(apiResult != CYBLE_ERROR_OK)
    			{
    			}
                //break;
            }else{
                start_advertisement();
                break;
            }
        break;
			
	
		case CYBLE_EVT_GATTC_DISCOVERY_COMPLETE:
			/* This event is generated whenever the discovery procedure is complete*/
            /*range.startHandle   = 0x0001;
            range.endHandle     = 0xFFFF;
            uuid.uuid16         = 0xb1b1;       //uuid of the characteristic to be read.

            readByTypeReqParam.range = range;
            readByTypeReqParam.uuid = uuid;
            readByTypeReqParam.uuidFormat = 0x01;

            apiResult = CyBle_GattcReadUsingCharacteristicUuid(connHandle, &readByTypeReqParam);

            if ( apiResult != CYBLE_ERROR_OK ) {
            }*/

			
			/* Update the LED status for BLE discovery mode*/
			ble_state = BLE_CONNECTED;
		    break;
        
        case CYBLE_EVT_GATTC_READ_BY_TYPE_RSP:
            readResponse = *(CYBLE_GATTC_READ_BY_TYPE_RSP_PARAM_T *) eventparam;
            save_data(readResponse.attrData.attrValue[2]);

            isDataRead = true;
            /*uint8 result[3];
            result[2] = readResponse.attrData.attrValue[2];
            LCD_Position(1, 11);
            char8 buffer[3];
            sprintf(buffer, "%u", result[2]);
            LCD_PrintString(buffer);*/
            break;
           
        case CYBLE_EVT_GATTS_WRITE_REQ:
            wrReqParam =(CYBLE_GATTS_WRITE_REQ_PARAM_T*) eventparam;
                                      
            /*write request is for custom characteristic*/
                
            
            //ClientCommand= wrReqParam->handleValPair.value.val[0];
            //ExecuteClientCommand(ClientCommand);
        
                
            /*Write request is for CCCD*/
            Status_LED_Red_Write(wrReqParam->handleValPair.value.val[0]);
     
            CyBle_GattsWriteRsp(wrReqParam->connHandle);/* response to write characterisitc from client*/
               
            break;
			
		default:
		    break;
	}
}

/*  This function checks for new devices that have been scanned and ads them
to its internal list. Also, if one of the scanned peripherals is Goal,
then it sets flag to allow connection with it. */
void HandleScanDevices(CYBLE_GAPC_ADV_REPORT_T* scanReport) {
	uint16 i;
	uint8 newDevice = TRUE;
	
	if((addedDevices < CYBLE_MAX_ADV_DEVICES)) {
		for(i = 0; i < addedDevices; i++) {
			/* Initialize the peerBdAddr element of our list.*/
			list_of_devices[i].peerBdAddr = &address_store[i][0];
			
			/* In this for loop, compare the new device address with the existing addresses in the list to 
				determine if the address is new or not. If the address exists in the list, then the device 
				is not new.*/
			if(FALSE == memcmp(list_of_devices[i].peerBdAddr, scanReport->peerBdAddr, ADV_ADDR_LEN)) {
				newDevice = FALSE;
				break;
			}
		}
		
		if(newDevice) {
			/* If the device address is new, then add the device to our existing list and compare the address
				with our expected address to see if the desired peripheral is advertising or not.*/
			
			list_of_devices[addedDevices].peerBdAddr = &address_store[addedDevices][0];
			
			/* Store the data*/
			list_of_devices[addedDevices].dataLen = scanReport->dataLen;
			
			list_of_devices[addedDevices].data = scanReport->data;
			
			list_of_devices[addedDevices].eventType = scanReport->eventType;
			/* Record the address type, Public or Random, of the advertising peripheral.*/
			list_of_devices[addedDevices].peerAddrType = scanReport->peerAddrType;
			
			/* Save the BD addresses */
            for(uint8 i = 0; i < 6; i++) {
			    list_of_devices[addedDevices].peerBdAddr[i] = scanReport->peerBdAddr[i];
            }
            
            /* Save address for compare */
            for(uint8 i = 0; i < 6; i++) {
                address_store[addedDevices][i] = scanReport->peerBdAddr[i];
            }
			
			list_of_devices[addedDevices].rssi = scanReport->rssi;
			
			/* If the new BD address found matches the desired BD address, the Peripheral has been found*/
			if(FALSE == memcmp(peripheralAddress, scanReport->peerBdAddr, ADV_ADDR_LEN)) {
				/* Save the connected device BD Address and Type*/
                for(uint8 i = 0; i < 6; i++) {
				    connectPeriphDevice.bdAddr[i] = scanReport->peerBdAddr[i];
                }
				
				connectPeriphDevice.type = list_of_devices[addedDevices].peerAddrType;
					
				/* Set the flag to notify application of a connected peripheral device */
				peripheralFound = TRUE;	
				
				/* Stop existing BLE Scan */
				CyBle_GapcStopScan();
                Timer_WriteCounter(0);
                Timer_Stop();
			}
			
			addedDevices++;
		}
		
		newDevice = TRUE;
	}
}

void set_initial_peripheral_address(void) {
	/* This stores the 6-byte BD address of peripheral device to which we have to connect.
	   This array is then used to compare the addresses of the advertising peripherals. 
       If one of the advertising peripherals has the same address, then we know it is 
       the device which we want to connect and initiate a connection request to it.*/
		peripheralAddress[5] = 0x00;
		peripheralAddress[4] = 0xA0;
		peripheralAddress[3] = 0x50;
		peripheralAddress[2] = 0x12;
		peripheralAddress[1] = 0x34;
		peripheralAddress[0] = 0x00;
}

void handle_scan(void) {
    /* If rescanning flag is TRUE, the restart the scan */
    if(ble_state == BLE_DISCONNECTED)
	{
        /* Reset flag to prevent calling multiple Start Scan commands */
		Timer_Start();
    	start_scan();
    } 
}

void start_scan(void){
    Status_LED_Red_Write(0);
    CyDelay(1);
    CyBle_GapcStartScan(CYBLE_SCANNING_FAST);
}

void stop_scan(void){
    Status_LED_Red_Write(1);
    CyDelay(1);
    CyBle_GapcStopScan();
}

void connect(void) {
    Status_LED_Green_Write(0);
    if(peripheralFound) {
        //stop_scan();
        /* If the desired peripheral is found, then connect to that peripheral */
        CyDelay(1);
		CyBle_GapcConnectDevice(&connectPeriphDevice);
			
		//CyBle_ProcessEvents();
		
		/* Reset flag to prevent resending the Connect command */
		peripheralFound = FALSE;
	}
}

void disconnect(void){
    Status_LED_Green_Write(1);
    CyDelay(1);
    CyBle_GapDisconnect(connHandle.bdHandle);
}

void start_advertisement(void){
    Status_LED_Blue_Write(0);
    CyDelay(1);
    CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
}

void stop_advertisement(void){
    Status_LED_Blue_Write(1);
    CyDelay(1);
    CyBle_GappStopAdvertisement();
}

void change_mode(void){
    bool status = get_ble_mode();
    if(status){
        disconnect();
        stop_advertisement();
        //sensor_signalization();
        start_scan();
    }else{
        stop_scan();
        disconnect();
        start_advertisement();
    }
}

void handle_scan_results(void) {
    // stores the addresses returned by Scannig results 
    uint8 scannedGoalAddresses[10][6];
    
    // Stores amount of goal devices about  
    uint8 goalDevices;
    
    uint8 emptyAddress[6];
    // Clear previous scanned goal addresses
    for(uint8 i = 0; i < goalDevices; i++) {
        for(uint8 j = 0; j < 6; j++) {
            scannedGoalAddresses[i][j] = 0;     
        }
    }
    goalDevices = 0;
    
    for(uint8 i = 0; i < CYBLE_MAX_ADV_DEVICES; i++) {
        // If compared address is empty, then next others will be empty too, so break
        if(0 == memcmp(emptyAddress, address_store[i], ADV_ADDR_LEN)) {
            break;
        }
        
        bool isGoalAddress = true;
        
        // Compare this address with primary goal address (first five bytes)
        for(uint8 j = 5; j > 0; j--) {
            if(address_store[i][j] != goalCompareAddress[j]) {
                isGoalAddress = false;
            }
        }
        
        if(isGoalAddress) {
            memcpy(&scannedGoalAddresses[goalDevices], &address_store[i], 6);
            goalDevices++;
        }
    }
    
    // Clear previous scanned address_store
    for(uint8 i = 0; i < 10; i++) {
        for(uint8 j = 0; j < 6; j++) {
            address_store[i][j] = 0;     
        }
    }
}

void save_data(uint8 data) {
    savedData = data;
}

uint8 get_data(void) {
    return savedData;
}
/* [] END OF FILE */
