/***************************************************************************//**
* \file MQ_9_Adc_INT.c
* \version 6.0
*
* \brief
*   This file contains the source code for implementation of the MQ_9
*   Component ADC functionality Interrupt Service Routine (ISR).
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
#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"
#include "MQ_9_Adc.h"
#include "MQ_9_Sensing.h"
#include "MQ_9_Configuration.h"
#include "cyapicallbacks.h"

/**
* \cond SECTION_ADC_INTERRUPT
* \addtogroup group_c_interrupt
* \{
*/

#if (MQ_9_ADC_EN)


/*******************************************************************************
* Function Name: MQ_9_IntrHandler
****************************************************************************//**
*
* \brief
*   This is an internal ISR function for the ADC implementation.
*
* \details
*   This ISR is triggered after a measurement completes or during the calibration
*   phases.
*
*   To use the entry or exit callbacks, define
*   MQ_9_[ENTRY|EXIT]_CALLBACK
*   and define the corresponding function,
*   MQ_9_[Entry|Exit]Callback().
*
*******************************************************************************/
CY_ISR(MQ_9_IntrHandler)
{
    static uint16 tVssa2Vref;
    static uint16 tFull;
    static uint16 vMaxMv;

    uint32 tmpResult;
    uint32 polarity;
    uint8 adcFsmStatus;
    uint8 tmpChId;

    #if (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE)
        uint8  interruptState;
        uint32 newRegValue;

        static uint16 tVdda2Vref;
        static uint16 tRecover;
    #endif /* (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE) */

    #ifdef MQ_9_ENTRY_CALLBACK
        MQ_9_EntryCallback();
    #endif /* MQ_9_ENTRY_CALLBACK */

    /* Mask all CSD block interrupts (disable all interrupts) */
    CY_SET_REG32(MQ_9_INTR_MASK_PTR, MQ_9_INTR_MASK_CLEAR_MASK);

    /* Clear all pending interrupts of CSD block */
    CY_SET_REG32(MQ_9_INTR_PTR, MQ_9_INTR_ALL_MASK);

    /* Read ADC result and check for ADC_ABORT or ADC_OVERFLOW flags */
    tmpResult = CY_GET_REG32(MQ_9_ADC_RES_PTR);
    if (0uL == (tmpResult & MQ_9_ADC_RES_ABORT_MASK))
    {
        if (0uL == (tmpResult & MQ_9_ADC_RES_OVERFLOW_MASK))
        {
            /* Read ADC status, define polarity, value and ChId */
            adcFsmStatus = (uint8)(MQ_9_dsRam.adcStatus & MQ_9_STATUS_FSM_MASK);
            /* Select the polarity bit */
            polarity = tmpResult & MQ_9_ADC_RES_HSCMPPOL_MASK;
            /* Select the result value */
            tmpResult &= MQ_9_ADC_RES_VALUE_MASK;
            tmpChId = MQ_9_dsRam.adcStatus & (uint8)(MQ_9_STATUS_LASTCHAN_MASK);

            /* ADC could have been converting or calibrating; handle each differently. */
            switch (adcFsmStatus)
            {
            case MQ_9_STATUS_CONVERTING:
                /*
                * After the converting will calculate an ADC result in mV depending on
                * sourcing or sinking mode. Checks for a saturation in all modes.
                */
                /* HSCMP polarity is 0:sink, 1:source */
                if(0uL != polarity) /* Sourcing */
                {
                    /* Saturate result at tVssa2Vref */
                    tmpResult = (tmpResult > (uint32)tVssa2Vref) ? (uint32)tVssa2Vref : tmpResult;
                    /* Scale result to Resolution range with rounding*/
                    tmpResult = ((((uint32)tVssa2Vref - tmpResult) * MQ_9_RES_MAX) +
                                 ((uint32)tFull >> 1uL)) / (uint32)tFull;
                }
                else /* Sinking */
                {
                    #if (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE)
                        /* Scale result with sink/source mismatch with rounding */
                        tmpResult = (((uint32)((uint32)tRecover << 1u) * tmpResult) + ((uint32)tVssa2Vref >> 1u)) / (uint32)tVssa2Vref;
                        /* Saturate result at t_Vdda2Vref*/
                        tmpResult = (tmpResult > (uint32)tVdda2Vref) ? (uint32)tVdda2Vref : tmpResult;
                        /* Scale result to Resolution range with rounding */
                        tmpResult = ((((uint32)tVssa2Vref + tmpResult) * MQ_9_RES_MAX)  +
                                     ((uint32)tFull >> 1uL)) / (uint32)tFull;
                    #else /* (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE) */
                        /* In vref mode, we are not supposed to be sinking. Saturate */
                        tmpResult = ((uint32)tVssa2Vref * MQ_9_RES_MAX) / (uint32)tFull;
                    #endif /* (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE) */
                }
                /* Store ADC result code */
                MQ_9_dsRam.adcCode [tmpChId] = (uint16)(tmpResult);        
                /* Scale result to mV with rounding and store it */
                tmpResult = ((uint32)vMaxMv * tmpResult) / MQ_9_RES_MAX;
                MQ_9_dsRam.adcResult[tmpChId] = (uint16)(tmpResult);
                MQ_9_dsRam.adcStatus = MQ_9_STATUS_IDLE;
                break;

            case MQ_9_STATUS_CALIBPH1:
                /*
                * After the calibration phase 1 will define a time to charge Cref1 and Cref2 from Vssa to Vref in
                * clock cycles. In the full-range mode starts the next calibration phase 2. In the Vref mode checks for a target
                * and recalibrate Idac if necessary.
                */
                tVssa2Vref = (uint16)tmpResult;

                #if (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE)
                    /* Full range mode */
                    MQ_9_dsRam.adcStatus = MQ_9_STATUS_CALIBPH2;
                    /* Un-mask ADC_RES interrupt (enable interrupt) */
                    CY_SET_REG32(MQ_9_INTR_MASK_PTR, MQ_9_INTR_MASK_ADC_RES_MASK);
                    MQ_9_StartAdcFSM(MQ_9_MEASMODE_VREFBY2);
                    
                #else /* (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE) */
                    /* Vref range mode */
                    vMaxMv = MQ_9_adcVrefMv;
                    tFull = tVssa2Vref;
                    MQ_9_dsRam.adcStatus = MQ_9_STATUS_IDLE;
                    
                #endif /* (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE) */

                break;

            /* In vref mode not need further calibration */
            #if (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE)

                case MQ_9_STATUS_CALIBPH2:
                    /*
                    * After the calibration phase 2 will define a time to charge Cref1 and Cref2 to Vref by a sourcing after
                    * a discharging them by a sinking from Vref during tVssa2Vref/2 time. This recharge time is called tRecover
                    * and is proporional to an Idac_sourcing/Idac_sinking mismatch. Then charges capacitors to Vdda and starts
                    * the next calibration phase 3.
                    */
                    tRecover = (uint16)tmpResult;
                    
                    /* Disconnect amuxbusB, Connect VDDA to csdbusB */
                    interruptState = CyEnterCriticalSection();
                    newRegValue = CY_GET_REG32(MQ_9_SW_BYP_SEL_PTR);
                    newRegValue &= (uint32)(~MQ_9_SW_BYP_DEFAULT);
                    CY_SET_REG32(MQ_9_SW_BYP_SEL_PTR, newRegValue);
                    CyExitCriticalSection(interruptState);
                    CY_SET_REG32(MQ_9_SW_SHIELD_SEL_PTR, MQ_9_SW_SHIELD_VDDA2CSDBUSB);

                    MQ_9_dsRam.adcStatus = MQ_9_STATUS_CALIBPH3;

                    /* Un-mask ADC_RES interrupt (enable interrupt) */
                    CY_SET_REG32(MQ_9_INTR_MASK_PTR, MQ_9_INTR_MASK_ADC_RES_MASK);
                    MQ_9_StartAdcFSM(MQ_9_MEASMODE_VIN);
                    break;
                    
                case MQ_9_STATUS_CALIBPH3:
                    /*
                    * After the calibration phase 3 will define a time to discharge Cref1 and Cref2 from Vdda to Vref by
                    * a sinking. This time must be corrected to an Idac_sourcing/Idac_sinking mismatch defined in the phase 2.
                    * Calculates t_full, checks it for a target and recalibrate Idac if necessary. Then calculates Vdda.
                    */
                    /* Reconnect amuxbusB, disconnect VDDA */
                    CY_SET_REG32(MQ_9_SW_SHIELD_SEL_PTR, MQ_9_SW_SHIELD_DEFAULT);
                    interruptState = CyEnterCriticalSection();
                    newRegValue = CY_GET_REG32(MQ_9_SW_BYP_SEL_PTR);
                    newRegValue |= MQ_9_SW_BYP_DEFAULT;
                    CY_SET_REG32(MQ_9_SW_BYP_SEL_PTR, newRegValue);
                    CyExitCriticalSection(interruptState);
                    
                    tVdda2Vref = (uint16)tmpResult;
                    /* Calibrate tVdda2Vref with Sink/Source mismatch with rounding */
                    tVdda2Vref = (uint16)(((((uint32)tVdda2Vref << 1u) * tRecover) + ((uint32)tVssa2Vref >> 1u)) / tVssa2Vref);
                    /* Test whether the full range is 2-bits higher than adc resolution */
                    tFull = (tVssa2Vref + tVdda2Vref);
                    vMaxMv = MQ_9_adcVrefMv +
                            (((MQ_9_adcVrefMv * tVdda2Vref) + (tVssa2Vref >> 1uL)) / tVssa2Vref);

                    MQ_9_dsRam.adcStatus = MQ_9_STATUS_IDLE;

                    break;

            #endif /* (MQ_9_ADC_FULLRANGE_MODE == MQ_9_ADC_MEASURE_MODE) */

            default:
                /* If interrupt is called without defined ADC state, do nothing. */
                break;
            }
        }
        else
        {
            MQ_9_dsRam.adcStatus = MQ_9_STATUS_OVERFLOW;
        }
    }
    else
    {
        MQ_9_dsRam.adcStatus = MQ_9_STATUS_IDLE;
    }
    #ifdef MQ_9_EXIT_CALLBACK
        MQ_9_ExitCallback();
    #endif /* MQ_9_EXIT_CALLBACK */
}


#endif /* MQ_9_ADC_EN */

/** \}
 * \endcond */


/* [] END OF FILE */
