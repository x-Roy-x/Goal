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
    
extern uint8 ble_state;
    
extern CYBLE_CONN_HANDLE_T connHandle;

extern uint8 goalDevices;
    
extern bool isDataRead;

/**************************Function Declarations*****************************/
void HandleScanDevices(CYBLE_GAPC_ADV_REPORT_T* scanReport);
void BleCallBack(uint32 event, void *eventparam);
void set_initial_peripheral_address(void);
CY_ISR_PROTO(Timer_Int_Handler);

void start_scan(void);
void stop_scan(void);
void connect(void);
void disconnect(void);
void start_advertisement(void);
void stop_advertisement(void);
void change_mode(void);

void handle_scan(void);
void handle_scan_results(void);
void save_data(uint8 data);
uint8 get_data(void);
/****************************************************************************/

#endif
/* [] END OF FILE */
