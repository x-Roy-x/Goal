/***************************************************************************//**
* \file MQ_9_Adc.h
* \version 6.0
*
* \brief
*   This file provides the sources of APIs specific to the ADC implementation.
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

#if !defined(CY_SENSE_MQ_9_ADC_H)
#define CY_SENSE_MQ_9_ADC_H

#include "cytypes.h"
#include "MQ_9_Configuration.h"
#include "MQ_9_Structure.h"

#if (MQ_9_ADC_EN)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/**
* \cond SECTION_STANDALONE_ADC
* \addtogroup group_adc_public
* \{
*/
#if (MQ_9_ENABLE == MQ_9_ADC_STANDALONE_EN)
    void MQ_9_Start(void);
    void MQ_9_Sleep(void);
    void MQ_9_Wakeup(void);
#endif  /* (MQ_9_ENABLE == MQ_9_ADC_STANDALONE_EN) */

/** \}
* \endcond */

/**
* \cond SECTION_ADC_PUBLIC
* \addtogroup group_adc_public
* \{
*/
cystatus MQ_9_StartConvert(uint8 chId);
uint8 MQ_9_IsBusy(void);
uint16 MQ_9_ReadResult_mVolts(uint8 chId);
uint16 MQ_9_GetResult_mVolts(uint8 chId);
cystatus MQ_9_Calibrate(void);

void MQ_9_Stop(void);
void MQ_9_Resume(void);

/** \}
* \endcond */

CY_ISR_PROTO(MQ_9_IntrHandler);

/**
* \cond SECTION_ADC_INTERNAL
* \addtogroup group_adc_internal
* \{
*/

void MQ_9_Initialize(void);
void MQ_9_SetAdcChannel(uint8 chId, uint32 state);
void MQ_9_ConfigAdcResources(void);
void MQ_9_StartAdcFSM(uint32 measureMode);
cystatus MQ_9_AdcCaptureResources(void);
cystatus MQ_9_AdcReleaseResources(void);
void MQ_9_ClearAdcChannels(void);
void MQ_9_SetNonDedicatedAdcChannel(uint8 chId, uint32 state);

/** \}
* \endcond */

/**************************************
* Global software/external variables
**************************************/

extern uint16 MQ_9_adcVrefMv;

/**************************************
*           API Constants
**************************************/

/* Error value if given bad channel ID. */
#define MQ_9_VALUE_BAD_CHAN_ID            (0x0000FFFFuL)
#define MQ_9_VALUE_BAD_RESULT             (0x0000FFFFuL)

/* Statuses defined for use with IsBusy */
#define MQ_9_STATUS_LASTCHAN_MASK         (0x0000000FuL)
#define MQ_9_STATUS_FSM_MASK              (0x000000F0uL)
#define MQ_9_STATUS_IDLE                  (0x00u)
#define MQ_9_STATUS_CALIBPH1              (0x10u)
#define MQ_9_STATUS_CALIBPH2              (0x20u)
#define MQ_9_STATUS_CALIBPH3              (0x30u)
#define MQ_9_STATUS_CONVERTING            (0x40u)
#define MQ_9_STATUS_OVERFLOW              (0x80u)

/* Potential channel states */
#define MQ_9_CHAN_CONNECT                 (1uL)
#define MQ_9_CHAN_DISCONNECT              (0uL)

/* Active channel when ADC is not configured */
#define MQ_9_NO_CHANNEL                   (0xFFu)

#define MQ_9_GPIO_PC_INPUT                (0x1uL)

/* Default filter delay */
#define MQ_9_FILTER_DELAY_DEFAULT         (2uL)

/* Adc Config */
#define MQ_9_CONFIG_DEFAULT               (MQ_9_CONFIG_ENABLE_MASK | \
                                                                            MQ_9_CONFIG_SAMPLE_SYNC_MASK | \
                                                                            MQ_9_CONFIG_SENSE_EN_MASK | \
                                                                            MQ_9_CONFIG_DSI_COUNT_SEL_MASK | \
                                                                            MQ_9_FILTER_DELAY_DEFAULT)

/* Measurement modes */
#define MQ_9_MEASMODE_OFF                 (0x0uL << CYFLD_CSD_ADC_MODE__OFFSET)
#define MQ_9_MEASMODE_VREF                (0x1uL << CYFLD_CSD_ADC_MODE__OFFSET)
#define MQ_9_MEASMODE_VREFBY2             (0x2uL << CYFLD_CSD_ADC_MODE__OFFSET)
#define MQ_9_MEASMODE_VIN                 (0x3uL << CYFLD_CSD_ADC_MODE__OFFSET)

/* Clock defines */
#define MQ_9_SENSE_DIV_DEFAULT            (0x4uL)
#define MQ_9_TOTAL_CLOCK_DIV              (MQ_9_ADC_MODCLK_DIV_DEFAULT * \
                                                                         MQ_9_SENSE_DIV_DEFAULT)

/* The MAX possible value of the AZ time in CSD_SENSE cycles. The value is limited by the width of the SEQ_TIME register */
#define MQ_9_ADC_SEQ_TIME_MAX                               (0x100u)

#define MQ_9_ADC_SEQ_TIME_CYCLES                            (((CYDEV_BCLK__HFCLK__HZ * MQ_9_ADC_AZ_TIME) / \
                                                                         MQ_9_TOTAL_CLOCK_DIV) / 1000000uL)

#if (MQ_9_ADC_SEQ_TIME_MAX < MQ_9_ADC_SEQ_TIME_CYCLES)
    #define MQ_9_SEQ_TIME_BASE            (MQ_9_ADC_SEQ_TIME_MAX)
#else
    #define MQ_9_SEQ_TIME_BASE            (MQ_9_ADC_SEQ_TIME_CYCLES)
#endif

#if (0 == MQ_9_SEQ_TIME_BASE)
    #define MQ_9_SEQ_TIME_DEFAUL          (1u)
#else
    #define MQ_9_SEQ_TIME_DEFAUL          (MQ_9_SEQ_TIME_BASE)
#endif

/* Acquisition time definitions: ADC_CTL */
#define MQ_9_ACQUISITION_BASE             ((MQ_9_ADC_ACQUISITION_TIME_US * \
                                                                        (CYDEV_BCLK__HFCLK__MHZ)) / \
                                                                         MQ_9_TOTAL_CLOCK_DIV)

/* SEQ_START field definitions */
#define MQ_9_FSMSETTING_ABORT             (0x1uL << CYFLD_CSD_ABORT__OFFSET)
#define MQ_9_FSMSETTING_DSI_START_EN      (0x1uL << CYFLD_CSD_DSI_START_EN__OFFSET)
#define MQ_9_FSMSETTING_AZ0SKIP           (0x1uL << CYFLD_CSD_AZ0_SKIP__OFFSET)
#define MQ_9_FSMSETTING_AZ1SKIP           (0x1uL << CYFLD_CSD_AZ1_SKIP__OFFSET)


#define MQ_9_FSMSETTING_NOAZSKIP          (0uL)
#define MQ_9_FSMSETTING_AZSKIP_DEFAULT    (MQ_9_FSMSETTING_AZ0SKIP \
                                                                            | ((0u != MQ_9_ADC_AZ_EN) \
                                                                            ? 0u \
                                                                            : MQ_9_FSMSETTING_AZ1SKIP))
#define MQ_9_FSMSETTING_DSIIGNORE         (0x00000000uL)
#define MQ_9_FSMSETTING_NOABORT           (0x00000000uL)
#define MQ_9_FSMSETTING_SEQMODE           (0x00000000uL)
#define MQ_9_FSMSETTING_START             (0x00000001uL)

/* IDACB definitions
*  The idac configuration for ADC use is mostly static, with only the VAL field varying.
*   Dynamic Polarity = 1 << 7
*   Polarity (normal) = 0 << 8
*   Balance, Leg1, Leg2 modes = don't care.
*   DSI Control Enable (no mix) = 0 << 21
*   Range (low) = 0 << 22
*   Leg1, Leg2 enable = 0
*   Leg3 enable = 1 << 26
*/
#define MQ_9_IDACB_CONFIG                 (0x04000080uL)
#define MQ_9_CLK16_MASK                   (0x0000FFFFuL)
#define MQ_9_CLK16VAL_SHIFT               (8uL)

/* Switch definitions */
#define MQ_9_SW_HSP_DEFAULT               (0x10000000uL)
#define MQ_9_SW_HSN_DEFAULT               (0x00100000uL)
#define MQ_9_SW_HSP_GETINPOL              (0x00010000uL)
#define MQ_9_SW_HSN_GETINPOL              (0x01000000uL)
#define MQ_9_SW_SHIELD_DEFAULT            (0x00000000uL)
#define MQ_9_SW_SHIELD_VDDA2CSDBUSB       (0x00000100uL)
#define MQ_9_SW_BYP_DEFAULT               (0x00110000uL)
#define MQ_9_SW_CMPP_DEFAULT              (0x00000000uL)
#define MQ_9_SW_CMPN_DEFAULT              (0x00000000uL)
#define MQ_9_SW_REFGEN_DEFAULT            (0x10000000uL)

#define MQ_9_SW_FWMOD_DEFAULT             (0x01100000uL)
#define MQ_9_SW_FWTANK_DEFAULT            (0x01100000uL)

/* The reference voltage macros */
#define MQ_9_SW_CTANK_PINSHIFT            (9uL)
#define MQ_9_SW_CMOD_PINSHIFT             (6uL)
#define MQ_9_SW_CMOD_PORT_MASK            (0x400uL)

#define MQ_9_LVTHRESH                     (2700uL)

/* The reference voltage is measured at nominal 2400 mV. Measured amount is stored in CYREG_SFLASH_CSDV2_CSD0_ADC_TRIM1 */
#define MQ_9_VREFCALIB_BASE               (2400uL)

/* RefGen settings */
#define MQ_9_REFGEN_GAIN_SHIFT            (CYFLD_CSD_GAIN__OFFSET)

/* At low voltage, REFGEN is enabled and bypassed. */
#define MQ_9_SW_AMUBUF_LV                 (0x01000100uL)
#define MQ_9_AMBUF_LV                     (0x00000002uL)
/* At normal voltage, REFGEN uses customizer-defined gain */
#define MQ_9_REFGEN_NORM                  (0x00000041uL | (MQ_9_ADC_GAIN << \
                                                                            MQ_9_REFGEN_GAIN_SHIFT))
#define MQ_9_SW_AMUBUF_NORM               (0x00000000uL)

/* HSCOMP definitions */
#define MQ_9_HSCMP_AZ_DEFAULT             (MQ_9_HSCMP_EN_MASK | \
                                                                            (MQ_9_ADC_AZ_EN << CYFLD_CSD_AZ_EN__OFFSET))

/* ADC_RES definitions */
#define MQ_9_RES_MAX                      ((1uL << MQ_9_ADC_RESOLUTION) - 1uL)
#define MQ_9_ADC_RES_OVERFLOW_MASK        (0x40000000uL)
#define MQ_9_ADC_RES_ABORT_MASK           (0x80000000uL)
#define MQ_9_ADC_RES_HSCMPPOL_MASK        (0x00010000uL)
#define MQ_9_ADC_RES_VALUE_MASK           (0x0000FFFFuL)

#endif  /* MQ_9_ADC_EN */

#endif  /* CY_SENSE_MQ_9_ADC_H */


/* [] END OF FILE */
