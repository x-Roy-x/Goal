#ifndef ADC_READ_H
#define ADC_READ_H
#include "smartboy.h"

    void adc_init(void);

    uint16 get_adc_by_channel(uint8 channel);

    uint16 * get_adc_list(void);

#endif
    