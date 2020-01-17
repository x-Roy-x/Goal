#include "smartboy.h"

void adc_init(void) {
    ADC_Start();
    ADC_StartConvert();
}

uint16 adc_channel_value(uint8 channel) {
    uint16 result = ADC_GetResult16(channel);
    uint16 value = ADC_CountsTo_mVolts(channel, result);
    return value;
}
