#include "goal.h"

void protocol_init(void) {
    I2C_Master_Start();
    PWM_Buzzer_Start();
    PWM_Servo_Start();
    ADC_Start();
    Button_INT_StartEx(button_handler);
}
