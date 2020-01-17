#include "smartboy.h"

void protocol_init(void) {
    adc_init();
    lcd_init();
    rtc_init();
    Button_Back_Int_StartEx(Button_Back_Int_Handler);
    Button_Ok_Int_StartEx(Button_Ok_Int_Handler);
    Button_Up_Int_StartEx(Button_Up_Int_Handler);
    Button_Down_Int_StartEx(Button_Down_Int_Handler);
    
	/* Start BLE component with appropriate Event handler function */
	CyBle_Start(BleCallBack);
    
    Timer_Int_StartEx( Timer_Int_Handler );
    Timer_Update_Start();
    Timer_Update_Int_StartEx( Timer_Update_Int_Handler );
	
	/* Set the BD address of peripheral device to which we have to connect */
	set_initial_peripheral_address(); 
    
    lcd_main_screen();
}
