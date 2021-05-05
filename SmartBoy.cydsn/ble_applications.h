#if !defined (BLEAPPLICATIONS_H)
#define BLEAPPLICATIONS_H
#include <project.h>

#define BLE_DISCONNECTED				0x01
#define BLE_SCANNING					0x02
#define BLE_SERVICE_DISCOVERY			0x03
#define BLE_CONNECTED					0x04

/* LED State Macros*/
#define LED_OFF							0x01
#define LED_ON							0x00
	
/* Counter default values used for LED status update during various 
* states BLE */
#ifdef ENABLE_LOW_POWER_MODE
#define LED_SCANNING_COUNTER_VALUE		60
#define LED_DISCOVERY_COUNTER_VALUE		5
#else
#define LED_SCANNING_COUNTER_VALUE		10000
#define LED_DISCOVERY_COUNTER_VALUE		1000
#endif

/**************************Function Declarations*****************************/
void HandleLEDs(uint8 state);
/****************************************************************************/

#endif
/* [] END OF FILE */
