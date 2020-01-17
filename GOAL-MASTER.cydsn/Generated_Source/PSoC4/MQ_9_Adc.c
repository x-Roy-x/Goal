/***************************************************************************//**
* \file MQ_9_Adc.c
* \version 6.0
*
* \brief
*   This file provides implementation for the ADC module of the MQ_9
*   Component.
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
#include <cytypes.h>
#include "CyLib.h"
#include "MQ_9_ISR.h"
#include "MQ_9_Configuration.h"
#include "MQ_9_Structure.h"
#include "MQ_9_Adc.h"
#include "MQ_9_Sensing.h"
#if (MQ_9_ENABLE == MQ_9_SELF_TEST_EN)
    #include "MQ_9_SelfTest.h"
#endif

#if (MQ_9_ADC_EN)

static uint8 MQ_9_initVar;
uint16 MQ_9_adcVrefMv = MQ_9_ADC_VREF_MV;


/*******************************************************************************
* Module local function declarations
*******************************************************************************/
/**
* \cond SECTION_ADC_INTERNAL
* \addtogroup group_adc_internal
* \{
*/

void MQ_9_SetModClkClockDivider(uint32 modClk);

/** \}
* \endcond */

/*******************************************************************************
* Local definition
*******************************************************************************/
#define MQ_9_INIT_DONE                    (1u)
#define MQ_9_INIT_NEEDED                  (0u)
#define MQ_9_CAL_WATCHDOG_CYCLES_NUM      (0x0000FFFFLu)

#if (MQ_9_ENABLE == MQ_9_ADC_STANDALONE_EN)
    /*******************************************************************************
    * Function Name: MQ_9_Start
    ****************************************************************************//**
    * \cond SECTION_STANDALONE_ADC
    * \brief
    *   Configures the hardware and performs calibration.
    *
    * \details
    *   Configures the hardware and performs calibration.
    * \endcond
    *
    *******************************************************************************/
    void MQ_9_Start(void)
    {
        MQ_9_ClearAdcChannels();

        if (MQ_9_INIT_DONE != MQ_9_initVar)
        {
            MQ_9_DsInitialize();
        }
        MQ_9_ConfigAdcResources();
        (void) MQ_9_Calibrate();
        while(MQ_9_IsBusy() != MQ_9_STATUS_IDLE)
        {
        }
    }

    /*******************************************************************************
    * Function Name: MQ_9_Sleep
    ****************************************************************************//**
    * \cond SECTION_STANDALONE_ADC
    * \brief
    *  Prepares the Component for deep sleep.
    *
    * \details
    *  Currently this function is empty and exists as a place for future updates,
    *  this function will be used to prepare the Component to enter deep sleep.
    * \endcond
    *
    *******************************************************************************/
    void MQ_9_Sleep(void)
    {
    }


    /*******************************************************************************
    * Function Name: MQ_9_Wakeup
    ****************************************************************************//**
    * \cond SECTION_STANDALONE_ADC
    * \brief
    *  This function resumes the Component after sleep.
    *
    * \details
    *  Currently this function is empty and exists as a place for future updates,
    *  this function will be used to resume the Component after exiting deep sleep.
    * \endcond
    *
    *******************************************************************************/
    void MQ_9_Wakeup(void)
    {
    }
#endif  /* (MQ_9_ENABLE != MQ_9_ADC_STANDALONE_EN) */

/*******************************************************************************
* Function Name: MQ_9_SetNonDedicatedAdcChannel
****************************************************************************//**
*
* \brief
*   Sets the non dedicated channel to the given state.
*
* \details
*   Connects/disconnects the pin and the analog muxbus B. Sets the drive mode
*   of the pin as well.
*
* \param chId  The ID of the non dedicated channel to be set.
* \param state The state in which the channel is to be put:
*         - (0) MQ_9_CHAN_DISCONNECT
*         - (1) MQ_9_CHAN_CONNECT
*
*******************************************************************************/
void MQ_9_SetNonDedicatedAdcChannel(uint8 chId, uint32 state)
{
    MQ_9_FLASH_IO_STRUCT const *ptr2adcIO;
    uint32 newRegisterValue;
    uint8  interruptState;
    uint32 pinHSIOMShift;
    uint32 pinModeShift;
    uint32 tmpVal;

    ptr2adcIO = &MQ_9_adcIoList[chId];
    pinHSIOMShift = (uint32)ptr2adcIO->hsiomShift;
    pinModeShift = (uint32)ptr2adcIO->shift;
    
    /* Clear port connections. */
    tmpVal = CY_GET_REG32(ptr2adcIO->hsiomPtr);
    tmpVal &= ~(MQ_9_HSIOM_SEL_MASK << pinHSIOMShift);
    
    interruptState = CyEnterCriticalSection();

    switch (state)
    {
    case MQ_9_CHAN_CONNECT:
        /* Connect AMuxBusB to the selected port */
        CY_SET_REG32(ptr2adcIO->hsiomPtr, (tmpVal | (MQ_9_HSIOM_SEL_AMUXB << pinHSIOMShift)));
        /* Update port configuration register (drive mode) to HiZ Analog */
        newRegisterValue = CY_GET_REG32(ptr2adcIO->pcPtr);
        newRegisterValue &= ~(MQ_9_GPIO_PC_MASK << pinModeShift);
        newRegisterValue |= (MQ_9_GPIO_PC_INPUT << pinModeShift);
        CY_SET_REG32(ptr2adcIO->pcPtr, newRegisterValue);
    
        /* Cmod and Ctank are not typical GPIO, require CSD setting. */
        if (0u != ((uint32)(ptr2adcIO->hsiomPtr) & MQ_9_SW_CMOD_PORT_MASK))
        {
            if (MQ_9_SW_CTANK_PINSHIFT == pinModeShift)
            {
                CY_SET_REG32(MQ_9_SW_DSI_SEL_PTR, MQ_9_SW_DSI_CTANK);
            }
            else if (MQ_9_SW_CMOD_PINSHIFT == pinModeShift)
            {
                CY_SET_REG32(MQ_9_SW_DSI_SEL_PTR, MQ_9_SW_DSI_CMOD);
            }
            else { /* No action */ }
        }
        break;
        
    /* Disconnection is a safe default state. Fall-through is intentional. */
    case MQ_9_CHAN_DISCONNECT:
    default:
        /* Disconnect AMuxBusB from the selected port */
        CY_SET_REG32(ptr2adcIO->hsiomPtr, tmpVal);
    
        /* Update port configuration register (drive mode) to HiZ input/output by clearing PC */
        newRegisterValue = CY_GET_REG32(ptr2adcIO->pcPtr);
        newRegisterValue &= ~(MQ_9_GPIO_PC_MASK << pinModeShift);
        CY_SET_REG32(ptr2adcIO->pcPtr, newRegisterValue);
        
        /* Cmod and Ctank are not typical GPIO, require CSD setting. */
        if (0u != ((uint32)(ptr2adcIO->hsiomPtr) & MQ_9_SW_CMOD_PORT_MASK))
        {
            if ((MQ_9_SW_CTANK_PINSHIFT == pinModeShift) ||
                (MQ_9_SW_CMOD_PINSHIFT == pinModeShift ))
            {
                CY_SET_REG32(MQ_9_SW_DSI_SEL_PTR, 0u);  
            }
        }
        break;
    }

    CyExitCriticalSection(interruptState);
    
    /* Set logic 0 to port data register */
    tmpVal = CY_GET_REG32(ptr2adcIO->drPtr);
    tmpVal &= (uint32)~(uint32)((uint32)1u << ptr2adcIO->drShift);
    CY_SET_REG32(ptr2adcIO->drPtr, tmpVal);
}


/*******************************************************************************
* Function Name: MQ_9_SetAdcChannel
****************************************************************************//**
*
* \brief
*   Sets the given channel to the given state.
*
* \details
*   Connects/disconnects the pin and the analog muxbus B.  Sets the drive mode
*   of the pin as well.
*
* \param chId  The ID of the channel to be set.
* \param state The state in which the channel is to be put:
*         - (0) MQ_9_CHAN_DISCONNECT
*         - (1) MQ_9_CHAN_CONNECT
*
*******************************************************************************/
void MQ_9_SetAdcChannel(uint8 chId, uint32 state)
{
    #if (0u != MQ_9_ADC_AMUXB_INPUT_EN)
        #if (MQ_9_ADC_MIN_CHANNELS < MQ_9_ADC_TOTAL_CHANNELS)
            if(MQ_9_ADC_SELECT_AMUXB_CH != chId)
            {
                MQ_9_SetNonDedicatedAdcChannel(chId, state);
            }
        #endif /* (0u != MQ_9_ADC_TOTAL_CHANNELS) */
    #else
        MQ_9_SetNonDedicatedAdcChannel(chId, state);
    #endif /* (0u != MQ_9_ADC_AMUXB_INPUT_EN) */
}


/*******************************************************************************
* Function Name: MQ_9_ConfigAdcResources
****************************************************************************//**
*
* \brief
*   Configures the CSD block to be used as an ADC.
*
* \details
*   Configures the IDACB, internal switches, REFGEN, HSCOMP, enables CSD
*   block interrupt and set interrupt vector to ADC sensing method.
*
*******************************************************************************/
void MQ_9_ConfigAdcResources(void)
{
    uint8  interruptState;
    uint32 newRegValue;

    /* Configure clocks */
    MQ_9_SetModClkClockDivider(MQ_9_ADC_MODCLK_DIV_DEFAULT);
    CY_SET_REG32(MQ_9_SENSE_PERIOD_PTR, (MQ_9_SENSE_DIV_DEFAULT - 1uL));

    /* Configure the IDAC */
    CY_SET_REG32(MQ_9_CONFIG_PTR, MQ_9_CONFIG_DEFAULT);
    CY_SET_REG32(MQ_9_IDACB_PTR, MQ_9_IDACB_CONFIG | MQ_9_dsRam.adcIdac);

    /* Configure AZ Time */
    CY_SET_REG32(MQ_9_SEQ_TIME_PTR, (MQ_9_SEQ_TIME_DEFAUL - 1uL));

    CY_SET_REG32(MQ_9_CSDCMP_PTR, 0uL);
    CY_SET_REG32(MQ_9_SW_DSI_SEL_PTR, 0uL);

    CY_SET_REG32(MQ_9_SENSE_DUTY_PTR, 0uL);
    CY_SET_REG32(MQ_9_SEQ_INIT_CNT_PTR, 1uL);
    CY_SET_REG32(MQ_9_SEQ_NORM_CNT_PTR, 2uL);

    /* Configure the block-level routing */
    CY_SET_REG32(MQ_9_SW_HS_P_SEL_PTR, MQ_9_SW_HSP_DEFAULT);
    CY_SET_REG32(MQ_9_SW_HS_N_SEL_PTR, MQ_9_SW_HSN_DEFAULT);
    CY_SET_REG32(MQ_9_SW_SHIELD_SEL_PTR, MQ_9_SW_SHIELD_DEFAULT);
    CY_SET_REG32(MQ_9_SW_CMP_P_SEL_PTR, MQ_9_SW_CMPP_DEFAULT);
    CY_SET_REG32(MQ_9_SW_CMP_N_SEL_PTR, MQ_9_SW_CMPN_DEFAULT);
    CY_SET_REG32(MQ_9_SW_FW_MOD_SEL_PTR, MQ_9_SW_FWMOD_DEFAULT);
    CY_SET_REG32(MQ_9_SW_FW_TANK_SEL_PTR, MQ_9_SW_FWTANK_DEFAULT);
    CY_SET_REG32(MQ_9_SW_REFGEN_SEL_PTR, MQ_9_SW_REFGEN_DEFAULT);

    interruptState = CyEnterCriticalSection();
    newRegValue = CY_GET_REG32(MQ_9_SW_BYP_SEL_PTR);
    newRegValue |= MQ_9_SW_BYP_DEFAULT;
    CY_SET_REG32(MQ_9_SW_BYP_SEL_PTR, newRegValue);
    CyExitCriticalSection(interruptState);

    /* Config RefGen */
    #if (MQ_9_CYDEV_VDDA_MV < MQ_9_LVTHRESH)
        /* The routing of the HS_N and AMUXBUF switches depend on RefGen */
        CY_SET_REG32(MQ_9_REFGEN_PTR, MQ_9_REFGEN_LV);
        CY_SET_REG32(MQ_9_SW_AMUXBUF_SEL_PTR, MQ_9_SW_AMUBUF_LV);
        CY_SET_REG32(MQ_9_AMBUF_PTR, MQ_9_AMBUF_LV);
    #else
        CY_SET_REG32(MQ_9_REFGEN_PTR, MQ_9_REFGEN_NORM);
        CY_SET_REG32(MQ_9_SW_AMUXBUF_SEL_PTR, MQ_9_SW_AMUBUF_NORM);
    #endif /* MQ_9__CYDEV_VDDA_MV < MQ_9_LVTHRESH */
    
    /* Configure HSCOMP */
    CY_SET_REG32(MQ_9_HSCMP_PTR, MQ_9_HSCMP_AZ_DEFAULT);

    /* Clear all pending interrupts of CSD block */
    CY_SET_REG32(MQ_9_INTR_PTR, MQ_9_INTR_ALL_MASK);
    
    /* Mask all CSD block interrupts (disable all interrupts) */
    CY_SET_REG32(MQ_9_INTR_MASK_PTR, MQ_9_INTR_MASK_CLEAR_MASK);

    /* Set the ISR vector */
    MQ_9_ISR_StartEx(&MQ_9_IntrHandler);

    /* Component is initialized */
    MQ_9_initVar = MQ_9_INIT_DONE;
}


/*******************************************************************************
* Function Name: MQ_9_StartAdcFSM
****************************************************************************//**
*
* \brief
*   Starts the CSD state machine with correct parameters to initialize an ADC
*   conversion.
*
* \details
*   Starts the CSD state machine with correct parameters to initialize an ADC
*   conversion.
*
* \param measureMode The FSM mode:
*        - (0) MQ_9_MEASMODE_OFF
*        - (1) MQ_9_MEASMODE_VREF
*        - (2) MQ_9_MEASMODE_VREFBY2
*        - (3) MQ_9_MEASMODE_VIN
*
*******************************************************************************/
void MQ_9_StartAdcFSM(uint32 measureMode)
{
    uint32 tmpStartVal;

    /* Set the mode and acquisition time */
    CY_SET_REG32(MQ_9_ADC_CTL_PTR, (measureMode | (MQ_9_ACQUISITION_BASE - 1u)));

    if(MQ_9_MEASMODE_VREF == measureMode)
    {
        tmpStartVal =
            MQ_9_FSMSETTING_AZSKIP_DEFAULT    |
            MQ_9_FSMSETTING_DSIIGNORE   |
            MQ_9_FSMSETTING_NOABORT     |
            MQ_9_FSMSETTING_SEQMODE     |
            MQ_9_FSMSETTING_START;
    }
    else if (MQ_9_MEASMODE_OFF == measureMode)
    {
        tmpStartVal = MQ_9_FSMSETTING_ABORT;
    }
    /* This setting is used for both MEASMODE_VREFBY2 and MEASMODE_VIN */
    else
    {
        tmpStartVal =
            MQ_9_FSMSETTING_AZSKIP_DEFAULT    |
            MQ_9_FSMSETTING_DSIIGNORE   |
            MQ_9_FSMSETTING_NOABORT     |
            MQ_9_FSMSETTING_SEQMODE     |
            MQ_9_FSMSETTING_START;
    }

    /* Enable HSComp */
    CY_SET_REG32(MQ_9_SEQ_START_PTR, tmpStartVal);
}

/*******************************************************************************
* Function Name: MQ_9_AdcCaptureResources
****************************************************************************//**
*
* \brief
*   Releases CSD resources from sensing mode, and sets it into ADC mode.
*
* \details
*   Releases CSD resources from sensing mode, and sets it into ADC mode.
*
* \return     The function returns cystatus of its operation.
*   - CYRET_LOCKED  - The sensing hardware is in-use and could not be released.
*   - CYRET_SUCCESS - Block is configured for ADC use.
*
*******************************************************************************/
cystatus MQ_9_AdcCaptureResources(void)
{
    cystatus tmpStatus = CYRET_SUCCESS;

    #if !(MQ_9_ENABLE == MQ_9_ADC_STANDALONE_EN)
        tmpStatus = MQ_9_SsReleaseResources();

        if (MQ_9_INIT_NEEDED == MQ_9_initVar)
        {
            if(CYRET_SUCCESS == tmpStatus)
            {
                MQ_9_ConfigAdcResources();
            }
            else
            {
                tmpStatus = CYRET_LOCKED;
            }
        }
    #else
        if (MQ_9_INIT_NEEDED == MQ_9_initVar)
        {
            MQ_9_ConfigAdcResources();
        }
    #endif /* !(MQ_9_ENABLE == MQ_9_ADC_STANDALONE_EN) */

    return tmpStatus;
}


/*******************************************************************************
* Function Name: MQ_9_AdcReleaseResources
****************************************************************************//**
*
* \brief
*   Releases CSD resources from ADC mode.
*
* \details
*   Releases CSD resources from ADC mode.
*
* \return     The function returns cystatus of its operation.
*   CYRET_SUCCESS   Block resources no longer in use.
*
*******************************************************************************/
cystatus MQ_9_AdcReleaseResources(void)
{
    uint8  interruptState;
    uint32 newRegValue;

    if (MQ_9_INIT_DONE == MQ_9_initVar)
    {
        /* If the FSM is running, shut it down. */
        if(MQ_9_STATUS_IDLE != (MQ_9_dsRam.adcStatus
                            & MQ_9_STATUS_FSM_MASK))
        {
            MQ_9_StartAdcFSM(MQ_9_MEASMODE_OFF);
            MQ_9_SetAdcChannel((MQ_9_dsRam.adcStatus
                            & (uint8)(MQ_9_STATUS_LASTCHAN_MASK)),
                            MQ_9_CHAN_DISCONNECT);
            MQ_9_dsRam.adcStatus = MQ_9_STATUS_IDLE;
        }

        /* Disable the subblocks. */
        CY_SET_REG32(MQ_9_IDACB_PTR, 0u);
        CY_SET_REG32(MQ_9_REFGEN_PTR, 0u);
        CY_SET_REG32(MQ_9_AMBUF_PTR, 0u);
        CY_SET_REG32(MQ_9_HSCMP_PTR, 0u);

        /* Reset the block-level routing */
        CY_SET_REG32(MQ_9_SW_HS_P_SEL_PTR, 0u);
        CY_SET_REG32(MQ_9_SW_HS_N_SEL_PTR, 0u);
        CY_SET_REG32(MQ_9_SW_SHIELD_SEL_PTR, 0u);
        CY_SET_REG32(MQ_9_SW_CMP_P_SEL_PTR, 0u);
        CY_SET_REG32(MQ_9_SW_CMP_N_SEL_PTR, 0u);
        CY_SET_REG32(MQ_9_SW_FW_MOD_SEL_PTR, 0u);
        CY_SET_REG32(MQ_9_SW_FW_TANK_SEL_PTR, 0u);

        interruptState = CyEnterCriticalSection();
        newRegValue = CY_GET_REG32(MQ_9_SW_BYP_SEL_PTR);
        newRegValue &= (uint32)(~MQ_9_SW_BYP_DEFAULT);
        CY_SET_REG32(MQ_9_SW_BYP_SEL_PTR, newRegValue);
        CyExitCriticalSection(interruptState);

         /* Disconnect all ADC channels */
        MQ_9_ClearAdcChannels();

        MQ_9_initVar = MQ_9_INIT_NEEDED;
    }

    return CYRET_SUCCESS;
}


/*******************************************************************************
* Function Name: MQ_9_StartConvert
****************************************************************************//**
*
* \brief
*  Initializes the hardware and initiates an analog-to-digital conversion on the
*  selected input channel.
*
* \details
*  Initializes the hardware and initiates an analog-to-digital conversion on the
*  selected input channel. This API only initiates a conversion and does not
*  wait for the conversion to be completed, therefore the
*  MQ_9_IsBusy() API must be used to check the
*  status and ensure that the conversion is complete prior to reading the result,
*  starting a new conversion with the same or a different channel, or reconfiguring
*  the hardware for different functionality.
*
* \param chId
*  The ID of the channel to be converted.
*
* \return 
*  The function returns cystatus of its operation.
*    - CYRET_SUCCESS - A conversion has started.
*    - CYRET_LOCKED - The hardware is already in-use by a previously initialized
*      conversion or other functionality. No new conversion is started by this API.
*    - CYRET_BAD_PARAM - An invalid channel Id. No conversion is started.
*
*******************************************************************************/
cystatus MQ_9_StartConvert(uint8 chId)
{
    uint32 tmpStatus = CYRET_SUCCESS;

    /* If non-standalone ADC, try to Capture resources */
    #if (MQ_9_ENABLE != MQ_9_ADC_STANDALONE_EN)
        tmpStatus = MQ_9_AdcCaptureResources();

        if (CYRET_SUCCESS == tmpStatus)
        {
    #else /* Otherwise, configure resources if needed. */
        if (MQ_9_INIT_NEEDED == MQ_9_initVar)
        {
            MQ_9_ConfigAdcResources();
        }
    #endif /* (MQ_9_ENABLE != MQ_9_ADC_STANDALONE_EN) */

    if(chId >= MQ_9_ADC_TOTAL_CHANNELS)
    {
        tmpStatus = CYRET_BAD_PARAM;
    }
    
    if (CYRET_SUCCESS == tmpStatus)
    {
        if(MQ_9_STATUS_IDLE != (MQ_9_dsRam.adcStatus & MQ_9_STATUS_FSM_MASK))
        {
            tmpStatus = CYRET_LOCKED;
        }

        if(CYRET_SUCCESS == tmpStatus)
        {
            #if(MQ_9_ADC_ANALOG_STARTUP_DELAY_US > 0uL)
                CyDelayUs(MQ_9_ADC_ANALOG_STARTUP_DELAY_US);
            #endif /* (MQ_9_ADC_ANALOG_STARTUP_DELAY_US > 0uL) */

            /* Set Component Status */
            MQ_9_dsRam.adcStatus = (MQ_9_STATUS_CONVERTING | chId);

            /* Configure a desired channel if needed */
            if (chId != MQ_9_dsRam.adcActiveCh)
            {
                if (MQ_9_NO_CHANNEL != MQ_9_dsRam.adcActiveCh)
                {
                    /* Disconnect existing channel */
                    MQ_9_SetAdcChannel(MQ_9_dsRam.adcActiveCh, MQ_9_CHAN_DISCONNECT);
                }
                /* Connect desired input */
                MQ_9_SetAdcChannel(chId, MQ_9_CHAN_CONNECT);
                MQ_9_dsRam.adcActiveCh = chId;
            }

            /* Un-mask ADC_RES interrupt (enable interrupt) */
            CY_SET_REG32(MQ_9_INTR_MASK_PTR, MQ_9_INTR_MASK_ADC_RES_MASK);
            MQ_9_StartAdcFSM(MQ_9_MEASMODE_VIN);
        }
    }
    
    #if (MQ_9_ENABLE != MQ_9_ADC_STANDALONE_EN)
        }
    #endif /* (MQ_9_ENABLE != MQ_9_ADC_STANDALONE_EN) */
    
    return tmpStatus;
}


/*******************************************************************************
* Function Name: MQ_9_IsBusy
****************************************************************************//**
*
* \brief
*   The function returns the status of the ADC's operation.
*
* \details
*   The function returns the status of the ADC's operation. A new conversion or
*   calibration must not be started unless the ADC is in the IDLE state.
*
* \return
*  The function returns the status of the ADC's operation.
*    - MQ_9_STATUS_IDLE - The ADC is not busy,
*       a new conversion can be initiated.
*    - MQ_9_STATUS_CONVERTING - A previously
*       initiated conversion is in progress.
*    - MQ_9_STATUS_CALIBPH1 - The ADC is in the
*      first phase (of 3) of calibration.
*    - MQ_9_STATUS_CALIBPH2 - The ADC is in the
*      second phase (of 3) of calibration.
*    - MQ_9_STATUS_CALIBPH3 - The ADC is in the
*      third phase (of 3) of calibration.
*    - MQ_9_STATUS_OVERFLOW - The most recent
*      measurement caused an overflow. The root cause of the overflow may be
*      the previous calibration values being invalid or the VDDA setting in cydwr
*      and hardware do not match. Perform re-calibration or set the
*      appropriate VDDA value in cydwr to avoid this error condition.
*
*******************************************************************************/
uint8 MQ_9_IsBusy(void)
{
    uint8 tmpStatus;

    if (0u != (MQ_9_ADC_RES_REG & MQ_9_ADC_RES_OVERFLOW_MASK))
    {
        tmpStatus = MQ_9_STATUS_OVERFLOW;
    }
    else
    {
        tmpStatus = (uint8)((*(volatile uint8 *)&MQ_9_dsRam.adcStatus) & MQ_9_STATUS_FSM_MASK);
    }

    return tmpStatus;
}


/*******************************************************************************
* Function Name: MQ_9_ReadResult_mVolts
****************************************************************************//**
*
* \brief
*  This is a blocking API. It initiates a conversion, waits for completion and
*  returns the result.
*
* \details
*  This is a blocking API. Internally, it starts a conversion using
*  MQ_9_StartConvert(), checks the status using
*  MQ_9_IsBusy(), waits until the conversion is
*  completed and returns the result.
*
* \param chId
*  The ID of the channel to be measured
*
* \return
*  The function returns voltage in millivolts or
*  MQ_9_VALUE_BAD_RESULT if:
*   - chId is invalid
*   - The ADC conversion is not started
*   - The ADC conversion watch-dog triggered.
*
*******************************************************************************/
uint16 MQ_9_ReadResult_mVolts(uint8 chId)
{
    cystatus convertStatus;
    uint16 tmpValue;
    uint32 watchdogAdcCounter;

    convertStatus = MQ_9_StartConvert(chId);
    if (CYRET_SUCCESS == convertStatus)
    {
        /* Initialize Watchdog Counter with time interval which is enough to ADC conversion is completed */
        watchdogAdcCounter = MQ_9_CAL_WATCHDOG_CYCLES_NUM;
        while ((MQ_9_IsBusy() != MQ_9_STATUS_IDLE) &&
               (0u != watchdogAdcCounter))
        {
            /* Wait until conversion complete and decrement Watchdog Counter to prevent unending loop */
            watchdogAdcCounter--;
        }
        if (0u != watchdogAdcCounter)
        {
            tmpValue = MQ_9_GetResult_mVolts(chId);
        }
        else
        {
            tmpValue = (uint16) MQ_9_VALUE_BAD_RESULT;
        }
    }
    else
    {
        tmpValue = (uint16) MQ_9_VALUE_BAD_RESULT;
    }

    return tmpValue;
}


/*******************************************************************************
* Function Name: MQ_9_GetResult_mVolts
****************************************************************************//**
*
* \brief
*  This API does not perform an ADC conversion and returns the last valid result
*  for the specified channel.
*
* \details
*  Returns the last valid result from the data structure for the specified channel.
*  This function can be used to read a previous result of any channel even if the
*  ADC is busy or a conversion is in progress. However, it is highly recommended
*  not to use this function with a channel that is in an active conversion.
*
* \param chId
*  The ID of the channel to be measured
*
* \return
*  The function returns a voltage in millivolts or
*  MQ_9_VALUE_BAD_CHAN_ID if chId is invalid.
*
*******************************************************************************/
uint16 MQ_9_GetResult_mVolts(uint8 chId)
{
    uint32 tmpRetVal = MQ_9_VALUE_BAD_CHAN_ID;

    if(chId < MQ_9_ADC_TOTAL_CHANNELS)
    {
        tmpRetVal = MQ_9_dsRam.adcResult[chId];
    }
    return (uint16)tmpRetVal;
}


/*******************************************************************************
* Function Name: MQ_9_Calibrate
****************************************************************************//**
*
* \brief
*  Performs calibration of the ADC module.
*
* \details
*  Performs calibration for the ADC to identify the appropriate hardware configuration
*  to produce accurate results. It is recommended to run the calibration
*  periodically (for example every 10 seconds) for accuracy and compensations.
*
* \return
*  The function returns cystatus of its operation.
*    - CYRET_SUCCESS - The block is configured for the ADC use.
*    - CYRET_LOCKED - The hardware is already in-use by a previously initialized
*      conversion or other functionality. No new conversion is started by this API.
*
*******************************************************************************/
cystatus MQ_9_Calibrate(void)
{
    uint32 tmpStatus;
    uint32 watchdogAdcCounter;
    uint32 tmpVrefCal;

    tmpStatus = MQ_9_AdcCaptureResources();

    if(MQ_9_STATUS_IDLE != (MQ_9_dsRam.adcStatus
        & MQ_9_STATUS_FSM_MASK))
    {
        tmpStatus = CYRET_LOCKED;
    }

    if (CYRET_SUCCESS == tmpStatus)
    {
        #if(MQ_9_ADC_ANALOG_STARTUP_DELAY_US > 0uL)
            CyDelayUs(MQ_9_ADC_ANALOG_STARTUP_DELAY_US);
        #endif /* (MQ_9_ADC_ANALOG_STARTUP_DELAY_US > 0uL) */

        /* Disconnect a channel if connected */
        if (MQ_9_NO_CHANNEL != MQ_9_dsRam.adcActiveCh)
        {
            MQ_9_SetAdcChannel(MQ_9_dsRam.adcActiveCh, MQ_9_CHAN_DISCONNECT);
            MQ_9_dsRam.adcActiveCh = MQ_9_NO_CHANNEL;
        }
        
        /* Get Vref trim-value */
        tmpVrefCal = (uint32)CY_GET_REG8(CYREG_SFLASH_CSDV2_CSD0_ADC_TRIM1) | 
            (((uint32)CY_GET_REG8(CYREG_SFLASH_CSDV2_CSD0_ADC_TRIM2)) << 8u);

        /* Update nominal Vref to real Vref */
        tmpVrefCal *= MQ_9_ADC_VREF_MV;
        tmpVrefCal /= MQ_9_VREFCALIB_BASE;

        MQ_9_adcVrefMv = (uint16)tmpVrefCal;

        MQ_9_dsRam.adcIdac = (uint8)MQ_9_ADC_IDAC_DEFAULT;
        CY_SET_REG32(MQ_9_IDACB_PTR, MQ_9_IDACB_CONFIG |
                                                     MQ_9_dsRam.adcIdac);

        MQ_9_dsRam.adcStatus = (MQ_9_STATUS_CALIBPH1);

        /* Un-mask ADC_RES interrupt (enable interrupt) */
        CY_SET_REG32(MQ_9_INTR_MASK_PTR, MQ_9_INTR_MASK_ADC_RES_MASK);
        MQ_9_StartAdcFSM(MQ_9_MEASMODE_VREF);

        /* Global CRC update */
        #if (MQ_9_ENABLE == MQ_9_SELF_TEST_EN)
            #if (MQ_9_ENABLE ==MQ_9_TST_GLOBAL_CRC_EN)
                MQ_9_DsUpdateGlobalCrc();
            #endif /* (MQ_9_ENABLE == MQ_9_TST_GLOBAL_CRC_EN) */
        #endif /* (MQ_9_ENABLE == MQ_9_SELF_TEST_EN) */

        /* Initialize Watchdog Counter with time interval which is enough to ADC calibration is completed */
        watchdogAdcCounter = MQ_9_CAL_WATCHDOG_CYCLES_NUM;
        while (((*(volatile uint8 *)&MQ_9_dsRam.adcStatus & MQ_9_STATUS_FSM_MASK)
                != 0u) &&  (0u != watchdogAdcCounter))
        {
            /* Wait until scan complete and decrement Watchdog Counter to prevent unending loop */
            watchdogAdcCounter--;
        }
    }

    return tmpStatus;
}


/*******************************************************************************
* Function Name: MQ_9_Initialize
****************************************************************************//**
*
* \brief
*   Configures the hardware to ADC mode and begins a calibration.
*
* \details
*   Configures the hardware to ADC mode and begins a calibration.
*
*******************************************************************************/
void MQ_9_Initialize(void)
{
    MQ_9_ConfigAdcResources();
    (void)MQ_9_Calibrate();
}


/*******************************************************************************
* Function Name: MQ_9_Stop
****************************************************************************//**
*
* \brief
*   Disables the hardware sub-blocks that are in use while in the ADC mode,
*   and frees the routing.
*
* \details
*   This function stops the Component operation. No ADC conversion can be
*   initiated when the Component is stopped. Once stopped, the hardware block
*   may be reconfigured by the application program for any other special usage.
*   The ADC operation can be resumed by calling the
*   MQ_9_Resume() function or the Component can
*   be reset by calling the MQ_9_Start() function.
*   This function is called when no ADC conversion is in progress.
*
*******************************************************************************/
void MQ_9_Stop(void)
{
    #if (MQ_9_ENABLE != MQ_9_ADC_STANDALONE_EN)
        (void)MQ_9_AdcReleaseResources();
    #endif /* (MQ_9_ENABLE != MQ_9_ADC_STANDALONE_EN) */
    MQ_9_initVar = MQ_9_INIT_NEEDED;
}


/*******************************************************************************
* Function Name: MQ_9_Resume
****************************************************************************//**
*
* \brief
*   Resumes the ADC operation after a stop call.
*
* \details
*   Resumes the ADC operation if the operation is stopped
*   previously by the MQ_9_Stop() API.
*
*******************************************************************************/
void MQ_9_Resume(void)
{
    MQ_9_Initialize();
}


/*******************************************************************************
* Function Name: MQ_9_ClearAdcChannels
****************************************************************************//**
*
* \brief
*  Resets all the ADC channels to disconnected state.
*
* \details
*   The function goes through all the ADC channels and disconnects the pin
*   and the analog muxbus B.  Sets the drive mode of the pin as well.
*
*******************************************************************************/
void MQ_9_ClearAdcChannels(void)
{
    uint32 chId;

    for (chId = 0u; chId < MQ_9_ADC_TOTAL_CHANNELS; chId++)
    {
        MQ_9_SetAdcChannel((uint8)chId, MQ_9_CHAN_DISCONNECT);
    }
    MQ_9_dsRam.adcActiveCh = MQ_9_NO_CHANNEL;
}

/*******************************************************************************
* Function Name: MQ_9_SetModClkClockDivider
****************************************************************************//**
*
* \brief
*   Sets the divider values for the modulator clock and then starts
*   the modulator clock.
*
* \details
*   It is not recommended to call this function directly by the application layer.
*   It is used by initialization to enable the clocks.
*
* \param
*   modClk The divider value for the modulator clock.
*
*******************************************************************************/
void MQ_9_SetModClkClockDivider(uint32 modClk)
{
    /* Stop modulator clock   */
    CY_SET_REG32(MQ_9_MODCLK_CMD_PTR,
                 ((uint32)MQ_9_ModClk__DIV_ID <<
                 MQ_9_MODCLK_CMD_DIV_SHIFT)|
                 ((uint32)MQ_9_MODCLK_CMD_DISABLE_MASK));

    /*
     * Set divider value for modClk.
     * 1u is subtracted from modClk because Divider register value 0 corresponds
     * to dividing by 1.
     */
    CY_SET_REG32(MQ_9_MODCLK_DIV_PTR, ((modClk - 1u) << 8u));

    /* Check whether previous modulator clock start command has finished. */
    while(0u != (CY_GET_REG32(MQ_9_MODCLK_CMD_PTR) & MQ_9_MODCLK_CMD_ENABLE_MASK))
    {
        /* Wait until previous modulator clock start command has finished. */
    }

    /* Start modulator clock, aligned to HFCLK */
    CY_SET_REG32(MQ_9_MODCLK_CMD_PTR,
                 (uint32)(((uint32)MQ_9_ModClk__DIV_ID << MQ_9_MODCLK_CMD_DIV_SHIFT) |
                  ((uint32)MQ_9_ModClk__PA_DIV_ID << MQ_9_MODCLK_CMD_PA_DIV_SHIFT) |
                  MQ_9_MODCLK_CMD_ENABLE_MASK));
}

#endif /* #if MQ_9_ADC_EN */


/* [] END OF FILE */
