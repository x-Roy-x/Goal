/*******************************************************************************
* File Name: Status_LED_Blue.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Status_LED_Blue_ALIASES_H) /* Pins Status_LED_Blue_ALIASES_H */
#define CY_PINS_Status_LED_Blue_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Status_LED_Blue_0			(Status_LED_Blue__0__PC)
#define Status_LED_Blue_0_PS		(Status_LED_Blue__0__PS)
#define Status_LED_Blue_0_PC		(Status_LED_Blue__0__PC)
#define Status_LED_Blue_0_DR		(Status_LED_Blue__0__DR)
#define Status_LED_Blue_0_SHIFT	(Status_LED_Blue__0__SHIFT)
#define Status_LED_Blue_0_INTR	((uint16)((uint16)0x0003u << (Status_LED_Blue__0__SHIFT*2u)))

#define Status_LED_Blue_INTR_ALL	 ((uint16)(Status_LED_Blue_0_INTR))


#endif /* End Pins Status_LED_Blue_ALIASES_H */


/* [] END OF FILE */
