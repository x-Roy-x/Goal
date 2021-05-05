#include "smartboy.h"

void adc_init(void) {
    ADC_Start();
    ADC_StartConvert();
}

uint16 get_adc_by_channel(uint8 channel) {
    uint16 adc_value = ADC_GetResult16(channel);
    return adc_value;
}

uint16 * get_adc_list(void) {
    uint16 *adc_values = (uint16 *)malloc(NUMBERS_OF_SENSORS*sizeof(uint16));
    for(int channel = 0; channel < NUMBERS_OF_SENSORS; channel++){
        adc_values[channel] = get_adc_by_channel(channel);
    }
    return adc_values;
}
