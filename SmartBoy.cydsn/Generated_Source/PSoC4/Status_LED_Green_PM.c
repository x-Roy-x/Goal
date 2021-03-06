/*******************************************************************************
* File Name: Status_LED_Green.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Status_LED_Green.h"

static Status_LED_Green_BACKUP_STRUCT  Status_LED_Green_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Status_LED_Green_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Status_LED_Green_SUT.c usage_Status_LED_Green_Sleep_Wakeup
*******************************************************************************/
void Status_LED_Green_Sleep(void)
{
    #if defined(Status_LED_Green__PC)
        Status_LED_Green_backup.pcState = Status_LED_Green_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Status_LED_Green_backup.usbState = Status_LED_Green_CR1_REG;
            Status_LED_Green_USB_POWER_REG |= Status_LED_Green_USBIO_ENTER_SLEEP;
            Status_LED_Green_CR1_REG &= Status_LED_Green_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Status_LED_Green__SIO)
        Status_LED_Green_backup.sioState = Status_LED_Green_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Status_LED_Green_SIO_REG &= (uint32)(~Status_LED_Green_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Status_LED_Green_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Status_LED_Green_Sleep() for an example usage.
*******************************************************************************/
void Status_LED_Green_Wakeup(void)
{
    #if defined(Status_LED_Green__PC)
        Status_LED_Green_PC = Status_LED_Green_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Status_LED_Green_USB_POWER_REG &= Status_LED_Green_USBIO_EXIT_SLEEP_PH1;
            Status_LED_Green_CR1_REG = Status_LED_Green_backup.usbState;
            Status_LED_Green_USB_POWER_REG &= Status_LED_Green_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Status_LED_Green__SIO)
        Status_LED_Green_SIO_REG = Status_LED_Green_backup.sioState;
    #endif
}


/* [] END OF FILE */
