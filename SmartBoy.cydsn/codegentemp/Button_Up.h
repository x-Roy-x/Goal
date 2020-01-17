/*******************************************************************************
* File Name: Button_Up.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Button_Up_H) /* Pins Button_Up_H */
#define CY_PINS_Button_Up_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Button_Up_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} Button_Up_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Button_Up_Read(void);
void    Button_Up_Write(uint8 value);
uint8   Button_Up_ReadDataReg(void);
#if defined(Button_Up__PC) || (CY_PSOC4_4200L) 
    void    Button_Up_SetDriveMode(uint8 mode);
#endif
void    Button_Up_SetInterruptMode(uint16 position, uint16 mode);
uint8   Button_Up_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Button_Up_Sleep(void); 
void Button_Up_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Button_Up__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Button_Up_DRIVE_MODE_BITS        (3)
    #define Button_Up_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Button_Up_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Button_Up_SetDriveMode() function.
         *  @{
         */
        #define Button_Up_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Button_Up_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Button_Up_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Button_Up_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Button_Up_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Button_Up_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Button_Up_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Button_Up_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Button_Up_MASK               Button_Up__MASK
#define Button_Up_SHIFT              Button_Up__SHIFT
#define Button_Up_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Button_Up_SetInterruptMode() function.
     *  @{
     */
        #define Button_Up_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Button_Up_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Button_Up_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Button_Up_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Button_Up__SIO)
    #define Button_Up_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Button_Up__PC) && (CY_PSOC4_4200L)
    #define Button_Up_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Button_Up_USBIO_DISABLE              ((uint32)(~Button_Up_USBIO_ENABLE))
    #define Button_Up_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Button_Up_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Button_Up_USBIO_ENTER_SLEEP          ((uint32)((1u << Button_Up_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Button_Up_USBIO_SUSPEND_DEL_SHIFT)))
    #define Button_Up_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Button_Up_USBIO_SUSPEND_SHIFT)))
    #define Button_Up_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Button_Up_USBIO_SUSPEND_DEL_SHIFT)))
    #define Button_Up_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Button_Up__PC)
    /* Port Configuration */
    #define Button_Up_PC                 (* (reg32 *) Button_Up__PC)
#endif
/* Pin State */
#define Button_Up_PS                     (* (reg32 *) Button_Up__PS)
/* Data Register */
#define Button_Up_DR                     (* (reg32 *) Button_Up__DR)
/* Input Buffer Disable Override */
#define Button_Up_INP_DIS                (* (reg32 *) Button_Up__PC2)

/* Interrupt configuration Registers */
#define Button_Up_INTCFG                 (* (reg32 *) Button_Up__INTCFG)
#define Button_Up_INTSTAT                (* (reg32 *) Button_Up__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Button_Up_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Button_Up__SIO)
    #define Button_Up_SIO_REG            (* (reg32 *) Button_Up__SIO)
#endif /* (Button_Up__SIO_CFG) */

/* USBIO registers */
#if !defined(Button_Up__PC) && (CY_PSOC4_4200L)
    #define Button_Up_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Button_Up_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Button_Up_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Button_Up_DRIVE_MODE_SHIFT       (0x00u)
#define Button_Up_DRIVE_MODE_MASK        (0x07u << Button_Up_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Button_Up_H */


/* [] END OF FILE */
