#include "goal.h"

CY_ISR(button_handler){
    signalization_state(1);
    //PWM_Servo_WriteCompare(DEGREE_0);
    Button_ClearInterrupt();
}
