/***************************************************************************//**
* \file MQ_9_Structure.c
* \version 6.0
*
* \brief
*   This file defines the data structure global variables and provides implementation
*   for the high-level and low-level APIs of the Data Structure module.
*
* \see MQ_9 v6.0 Datasheet
*
*//*****************************************************************************
* Copyright (2016-2018), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#include <string.h>
#include <cytypes.h>
#include "CyLib.h"
#include "MQ_9_Structure.h"
#include "MQ_9_Configuration.h"

#if (MQ_9_ENABLE == MQ_9_ADC_EN)
    #include "MQ_9_Adc.h"
#endif /* (MQ_9_ENABLE == MQ_9_ADC_EN) */

/*******************************************************************************
* Defines the RAM Data Structure variables and their init data in flash
*******************************************************************************/

/**
* \cond SECTION_GLOBAL_VARIABLES
* \addtogroup group_global_variables
* \{
*/

/**
* The variable that contains the MQ_9 configuration, settings and
* scanning results. MQ_9_dsRam represents RAM Data Structure.
*/
MQ_9_RAM_STRUCT MQ_9_dsRam;

/** \}
* \endcond */

/**
* \cond SECTION_API_CONSTANTS
* \addtogroup group_api_constants
* \{
*/


/**
 *  The array of the pointers to the ADC input channels specific register.
 */
const MQ_9_FLASH_IO_STRUCT MQ_9_adcIoList[MQ_9_ADC_TOTAL_CHANNELS] =
{
    {
        (reg32 *)MQ_9_AdcInput__0__HSIOM,
        (reg32 *)MQ_9_AdcInput__0__PC,
        (reg32 *)MQ_9_AdcInput__0__DR,
        (reg32 *)MQ_9_AdcInput__0__PS,
        MQ_9_AdcInput__0__HSIOM_MASK,
        MQ_9_AdcInput__0__MASK,
        MQ_9_AdcInput__0__HSIOM_SHIFT,
        (uint8)MQ_9_AdcInput__0__SHIFT,
        (uint8)MQ_9_AdcInput__0__SHIFT * 3u,
    },
};



/** \}
* \endcond */


/*******************************************************************************
* Function Name: MQ_9_DsInitialize
****************************************************************************//**
*
* \brief
*   This function initializes the Data Structure storage.
*
* \details
*   Configures the initial Adc datastructure members.
*
*******************************************************************************/
void MQ_9_DsInitialize(void)
{
    /* Reset RAM data structure content */
    (void)memset(&MQ_9_dsRam, 0, sizeof(MQ_9_dsRam));

    MQ_9_dsRam.adcResolution = MQ_9_ADC_RESOLUTION;
    MQ_9_dsRam.adcIdac = (uint8)(MQ_9_ADC_IDAC_DEFAULT);
    MQ_9_dsRam.adcActiveCh = MQ_9_NO_CHANNEL;
}

/* [] END OF FILE */
