#include "goal.h"
#include "stdio.h"

int main(void)
{
    CyGlobalIntEnable;
    uint8 value = 1;

    protocol_init();
    
//    UART_Start();

    for(;;)
    {    
        i2c_main_loop();
        
        uint16 gas_value = gas_sensor_signalization();
        set_data_from_mcu(1, gas_value); 
        
//        char buf[9];
//        sprintf(buf, "%u\r\n", gas_value);
//        UART_UartPutString(buf);
        
        value = get_data_from_ble(0);
        LED_Conect_Write(value);
        CyDelay(1);
    }
}
