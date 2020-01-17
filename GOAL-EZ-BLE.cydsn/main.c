#include "goal.h"

int main (void)
{
    CyGlobalIntEnable;
 
    protocol_init();
    
    for(;;)
    {
        main_loop();
        
        update_data_read();
    }
}
