#include "smartboy.h"

void protocol_init(void) {
    adc_init();
    lcd_init();
    Button_Reset_Int_StartEx(Button_Reset_Int_Handler);
    Button_Scan_Int_StartEx(Button_Scan_Int_Handler);
    
	/* Start BLE component with appropriate Event handler function */
	CyBle_Start(BleCallBack);
    //CyBle_Start(BleCallBackServer);
    
    Timer_Int_StartEx( Timer_Int_Handler );
    Timer_Update_Start();
    Timer_Update_Int_StartEx( Timer_Update_Int_Handler );
	
	/* Set the BD address of peripheral device to which we have to connect */
	set_initial_peripheral_address(); 
    
    lcd_data_screen();
}
