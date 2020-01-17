#ifndef BUTTONS_H
#define BUTTONS_H
#include "smartboy.h"
    
    CY_ISR_PROTO(Button_Back_Int_Handler);
    CY_ISR_PROTO(Button_Ok_Int_Handler);
    CY_ISR_PROTO(Button_Up_Int_Handler);
    CY_ISR_PROTO(Button_Down_Int_Handler);
    
    void scroll_pages(void);
    void button_back_switch_screen(void);
    void button_ok_switch_screen(void);
    
    void counter_set_back_stack_value(uint8 backStackLevel);
    int16 get_counter_up_down(void);
    uint8 get_counter_ok_back(void);
    void set_counter_up_down(int16 counter);
    
#endif
    