/*******************************************************************************
* File Name: Debouncer_Clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Debouncer_Clock_H)
#define CY_CLOCK_Debouncer_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void Debouncer_Clock_StartEx(uint32 alignClkDiv);
#define Debouncer_Clock_Start() \
    Debouncer_Clock_StartEx(Debouncer_Clock__PA_DIV_ID)

#else

void Debouncer_Clock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void Debouncer_Clock_Stop(void);

void Debouncer_Clock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 Debouncer_Clock_GetDividerRegister(void);
uint8  Debouncer_Clock_GetFractionalDividerRegister(void);

#define Debouncer_Clock_Enable()                         Debouncer_Clock_Start()
#define Debouncer_Clock_Disable()                        Debouncer_Clock_Stop()
#define Debouncer_Clock_SetDividerRegister(clkDivider, reset)  \
    Debouncer_Clock_SetFractionalDividerRegister((clkDivider), 0u)
#define Debouncer_Clock_SetDivider(clkDivider)           Debouncer_Clock_SetDividerRegister((clkDivider), 1u)
#define Debouncer_Clock_SetDividerValue(clkDivider)      Debouncer_Clock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define Debouncer_Clock_DIV_ID     Debouncer_Clock__DIV_ID

#define Debouncer_Clock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define Debouncer_Clock_CTRL_REG   (*(reg32 *)Debouncer_Clock__CTRL_REGISTER)
#define Debouncer_Clock_DIV_REG    (*(reg32 *)Debouncer_Clock__DIV_REGISTER)

#define Debouncer_Clock_CMD_DIV_SHIFT          (0u)
#define Debouncer_Clock_CMD_PA_DIV_SHIFT       (8u)
#define Debouncer_Clock_CMD_DISABLE_SHIFT      (30u)
#define Debouncer_Clock_CMD_ENABLE_SHIFT       (31u)

#define Debouncer_Clock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << Debouncer_Clock_CMD_DISABLE_SHIFT))
#define Debouncer_Clock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << Debouncer_Clock_CMD_ENABLE_SHIFT))

#define Debouncer_Clock_DIV_FRAC_MASK  (0x000000F8u)
#define Debouncer_Clock_DIV_FRAC_SHIFT (3u)
#define Debouncer_Clock_DIV_INT_MASK   (0xFFFFFF00u)
#define Debouncer_Clock_DIV_INT_SHIFT  (8u)

#else 

#define Debouncer_Clock_DIV_REG        (*(reg32 *)Debouncer_Clock__REGISTER)
#define Debouncer_Clock_ENABLE_REG     Debouncer_Clock_DIV_REG
#define Debouncer_Clock_DIV_FRAC_MASK  Debouncer_Clock__FRAC_MASK
#define Debouncer_Clock_DIV_FRAC_SHIFT (16u)
#define Debouncer_Clock_DIV_INT_MASK   Debouncer_Clock__DIVIDER_MASK
#define Debouncer_Clock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_Debouncer_Clock_H) */

/* [] END OF FILE */
