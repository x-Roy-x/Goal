/*******************************************************************************
* File Name: Button_Back.h  
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

#if !defined(CY_PINS_Button_Back_ALIASES_H) /* Pins Button_Back_ALIASES_H */
#define CY_PINS_Button_Back_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Button_Back_0			(Button_Back__0__PC)
#define Button_Back_0_PS		(Button_Back__0__PS)
#define Button_Back_0_PC		(Button_Back__0__PC)
#define Button_Back_0_DR		(Button_Back__0__DR)
#define Button_Back_0_SHIFT	(Button_Back__0__SHIFT)
#define Button_Back_0_INTR	((uint16)((uint16)0x0003u << (Button_Back__0__SHIFT*2u)))

#define Button_Back_INTR_ALL	 ((uint16)(Button_Back_0_INTR))


#endif /* End Pins Button_Back_ALIASES_H */


/* [] END OF FILE */
