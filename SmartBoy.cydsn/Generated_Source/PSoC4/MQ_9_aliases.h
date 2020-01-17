/*******************************************************************************
* File Name: MQ_9.h  
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

#if !defined(CY_PINS_MQ_9_ALIASES_H) /* Pins MQ_9_ALIASES_H */
#define CY_PINS_MQ_9_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define MQ_9_0			(MQ_9__0__PC)
#define MQ_9_0_PS		(MQ_9__0__PS)
#define MQ_9_0_PC		(MQ_9__0__PC)
#define MQ_9_0_DR		(MQ_9__0__DR)
#define MQ_9_0_SHIFT	(MQ_9__0__SHIFT)
#define MQ_9_0_INTR	((uint16)((uint16)0x0003u << (MQ_9__0__SHIFT*2u)))

#define MQ_9_INTR_ALL	 ((uint16)(MQ_9_0_INTR))


#endif /* End Pins MQ_9_ALIASES_H */


/* [] END OF FILE */
