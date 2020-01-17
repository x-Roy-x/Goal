#include "goal.h"

uint8 state = 1;

uint16 gas_sensor_signalization() {
    uint16 data = ADC_ReadResult_mVolts(0)/10;
    if(data >= 150){
        state = 0;
    }
    if(state == 0){
        PWM_Buzzer_WriteCompare(BUZEER_ON);
        PWM_Servo_WriteCompare(DEGREE_90);
    } else{
        PWM_Buzzer_WriteCompare(BUZEER_OFF);
    }
        
    return data;
}

void signalization_state(uint8 value){
    state = value;
}
