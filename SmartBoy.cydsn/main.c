#include "smartboy.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    protocol_init();
    
    for(;;) {
        CyBle_ProcessEvents();
        //start_advertisement();
        /* Handle BLE Status LED */
        bool ble_mode = get_ble_mode();
        //change_mode();
        if(ble_mode){
		    //HandleLEDs(ble_mode);
            sensor_signalization();
        }
    }
}

/* [] END OF FILE */
