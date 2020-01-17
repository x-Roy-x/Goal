#include "smartboy.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    protocol_init();
    
    for(;;) {
        /* Handle BLE Status LED */
		HandleLEDs(ble_state);
        
        handle_connection();
        
        CyBle_ProcessEvents();
    }
}

/* [] END OF FILE */
