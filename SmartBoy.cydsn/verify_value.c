#include "smartboy.h"

void sensor_signalization() {
    uint16 *adc_data_list = get_adc_list();
    
    for(int sensor = 0; sensor < NUMBERS_OF_SENSORS; sensor++){
        if(adc_data_list[sensor] > 700){
            if(ble_state == BLE_DISCONNECTED) {
                handle_scan();
                connect();
            }
        }
    }
    free(adc_data_list);
}

