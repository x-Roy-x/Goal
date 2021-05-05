#ifndef I2C_GOAL_H
#define I2C_GOAL_H
#include "goal.h"
    
    void i2c_init(void);
    void set_data_mcu(uint8 index, uint16 value);
    uint16 get_data_mcu(uint8 index);
       
#endif
    