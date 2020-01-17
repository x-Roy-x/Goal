/*******************************************************************************
* \file MQ_9_Configuration.h
* \version 6.0
*
* \brief
*   This file provides the customizer parameters definitions.
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

#if !defined(CY_SENSE_MQ_9_CONFIGURATION_H)
#define CY_SENSE_MQ_9_CONFIGURATION_H

#include <cytypes.h>

/*******************************************************************************
* Customizer-generated defines
*******************************************************************************/
#define MQ_9_ENABLE                             (1u)
#define MQ_9_DISABLE                            (0u)

#define MQ_9_THIRD_GENERATION_BLOCK             (1u)
#define MQ_9_FOURTH_GENERATION_BLOCK            (2u)

#define MQ_9_GENERATION_BLOCK_VERSION           (2u)

/*******************************************************************************
* HW IP block global defines
*******************************************************************************/

#if (MQ_9_GENERATION_BLOCK_VERSION == MQ_9_THIRD_GENERATION_BLOCK)
    #define MQ_9_CSDV1                          (MQ_9_ENABLE)
    
    #ifdef CYIPBLOCK_m0s8csd_VERSION
        #if (0u == CYIPBLOCK_m0s8csd_VERSION)
            #define MQ_9_CSDV1_VER1             (MQ_9_ENABLE)
            #define MQ_9_CSDV1_VER2             (MQ_9_DISABLE)
        #else
            #define MQ_9_CSDV1_VER1             (MQ_9_DISABLE)
            #define MQ_9_CSDV1_VER2             (MQ_9_ENABLE)
        #endif
    #else
        #error "HW IP block version is not specified"
    #endif
#else
    #define MQ_9_CSDV1                          (MQ_9_DISABLE)
    #define MQ_9_CSDV1_VER1                     (MQ_9_DISABLE)
    #define MQ_9_CSDV1_VER2                     (MQ_9_DISABLE)
#endif

#if (MQ_9_GENERATION_BLOCK_VERSION == MQ_9_FOURTH_GENERATION_BLOCK)
    #define MQ_9_CSDV2                          (MQ_9_ENABLE)
    
    #ifdef CYIPBLOCK_m0s8csdv2_VERSION
        #if (1u == CYIPBLOCK_m0s8csdv2_VERSION)
            #define MQ_9_CSDV2_VER1             (MQ_9_ENABLE)
        #else
            #define MQ_9_CSDV2_VER1             (MQ_9_DISABLE)
        #endif
        #if (2u == CYIPBLOCK_m0s8csdv2_VERSION)
            #define MQ_9_CSDV2_VER2             (MQ_9_ENABLE)
        #else
            #define MQ_9_CSDV2_VER2             (MQ_9_DISABLE)
        #endif
    #else
        #error "HW IP block version is not specified"
    #endif
#else
    #define MQ_9_CSDV2                          (MQ_9_DISABLE)
    #define MQ_9_CSDV2_VER1                     (MQ_9_DISABLE)
    #define MQ_9_CSDV2_VER2                     (MQ_9_DISABLE)
#endif

/*******************************************************************************
* Project-global defines
*******************************************************************************/

#define MQ_9_2000_MV                            (2000u)

#ifdef CYDEV_VDDA_MV
    #define MQ_9_CYDEV_VDDA_MV                  (CYDEV_VDDA_MV)
#else
    #ifdef CYDEV_VDD_MV
        #define MQ_9_CYDEV_VDDA_MV              (CYDEV_VDD_MV)
    #endif
#endif

#define MQ_9_BAD_CONVERSIONS_NUM                (1u)
#define MQ_9_RESAMPLING_CYCLES_MAX_NUMBER       (1u)


/*******************************************************************************
* Enabled Scan Methods
*******************************************************************************/
#define MQ_9_CSD_EN                             (0u)
#define MQ_9_CSX_EN                             (0u)
#define MQ_9_ISX_EN                             (0u)
#define MQ_9_CSD_CSX_EN                         (MQ_9_CSD_EN && MQ_9_CSX_EN)

#define MQ_9_MANY_SENSE_MODES_EN                ((MQ_9_CSD_EN && MQ_9_CSX_EN) || \
                                                             (MQ_9_CSD_EN && MQ_9_ISX_EN) || \
                                                             (MQ_9_CSX_EN && MQ_9_ISX_EN) || \
                                                             (MQ_9_SELF_TEST_EN))

#define MQ_9_MANY_WIDGET_METHODS_EN             ((MQ_9_CSD_EN && MQ_9_CSX_EN) || \
                                                             (MQ_9_CSD_EN && MQ_9_ISX_EN) || \
                                                             (MQ_9_CSX_EN && MQ_9_ISX_EN))

#define MQ_9_CSD2X_EN                           (0u)
#define MQ_9_CSX2X_EN                           (0u)

/*******************************************************************************
* Definitions for number of widgets and sensors
*******************************************************************************/
#define MQ_9_TOTAL_WIDGETS                      (0u)
#define MQ_9_TOTAL_CSD_WIDGETS                  (0u)
#define MQ_9_TOTAL_CSD_SENSORS                  (0u)
#define MQ_9_TOTAL_CSX_WIDGETS                  (0u)
#define MQ_9_TOTAL_ISX_WIDGETS                  (0u)
#define MQ_9_TOTAL_CSX_NODES                    (0u)
#define MQ_9_TOTAL_ISX_NODES                    (0u)

/*******************************************************************************
* Total number of CSD sensors + CSX nodes
*******************************************************************************/
#define MQ_9_TOTAL_SENSORS            (MQ_9_TOTAL_CSD_SENSORS + \
                                                   MQ_9_TOTAL_CSX_NODES+ \
                                                   MQ_9_TOTAL_ISX_NODES)

/*******************************************************************************
* Total number of scan slots (used only when dual-channel scan is enabled)
*******************************************************************************/
#define MQ_9_TOTAL_SCAN_SLOTS         (0u)

/*******************************************************************************
* Defines widget IDs
*******************************************************************************/

/*******************************************************************************
* Defines sensor IDs
*******************************************************************************/


/*******************************************************************************
* Defines ADC channel IDs
*******************************************************************************/
#define MQ_9_CHANNEL_0                          (0u)


/*******************************************************************************
* Enabled widget types
*******************************************************************************/
#define MQ_9_BUTTON_WIDGET_EN         (0u)
#define MQ_9_SLIDER_WIDGET_EN         (0u)
#define MQ_9_MATRIX_WIDGET_EN         (0u)
#define MQ_9_PROXIMITY_WIDGET_EN      (0u)
#define MQ_9_TOUCHPAD_WIDGET_EN       (0u)
#define MQ_9_ENCODERDIAL_WIDGET_EN    (0u)

#define MQ_9_CSD_MATRIX_WIDGET_EN     (0u)
#define MQ_9_CSD_TOUCHPAD_WIDGET_EN   (0u)

#define MQ_9_CSX_MATRIX_WIDGET_EN     (0u)
#define MQ_9_CSX_TOUCHPAD_WIDGET_EN   (0u)

/*******************************************************************************
* Centroid APIs
*******************************************************************************/
#define MQ_9_CENTROID_EN              (0u)
#define MQ_9_TOTAL_DIPLEXED_SLIDERS   (0u)
#define MQ_9_TOTAL_LINEAR_SLIDERS     (0u)
#define MQ_9_TOTAL_RADIAL_SLIDERS     (0u)
#define MQ_9_TOTAL_TOUCHPADS          (0u)
#define MQ_9_MAX_CENTROID_LENGTH      (0u)
#define MQ_9_SLIDER_MULT_METHOD       (0u)
#define MQ_9_TOUCHPAD_MULT_METHOD     (0u)

/*******************************************************************************
* Enabled sensor types
*******************************************************************************/
#define MQ_9_REGULAR_SENSOR_EN        (0u)
#define MQ_9_PROXIMITY_SENSOR_EN      (0u)

/*******************************************************************************
* Sensor ganging
*******************************************************************************/
#define MQ_9_GANGED_SNS_EN            (0u)
#define MQ_9_CSD_GANGED_SNS_EN        (0u)
#define MQ_9_CSX_GANGED_SNS_EN        (0u)

/*******************************************************************************
* Max number of sensors used among all the widgets
*******************************************************************************/
#define MQ_9_MAX_SENSORS_PER_WIDGET   (0u)
#define MQ_9_MAX_SENSORS_PER_5X5_TOUCHPAD (1u)

/*******************************************************************************
* Total number of all used electrodes (NOT unique)
*******************************************************************************/
#define MQ_9_TOTAL_ELECTRODES         (0u)
/* Obsolete */
#define MQ_9_TOTAL_SENSOR_IOS         MQ_9_TOTAL_ELECTRODES

/*******************************************************************************
* Total number of used physical IOs (unique)
*******************************************************************************/
#define MQ_9_TOTAL_IO_CNT             (0u)

/*******************************************************************************
* Array length for widget status registers
*******************************************************************************/
#define MQ_9_WDGT_STATUS_WORDS        \
                        (((uint8)((MQ_9_TOTAL_WIDGETS - 1u) / 32u)) + 1u)


/*******************************************************************************
* Auto-tuning mode selection
*******************************************************************************/
#define MQ_9_CSD_SS_DIS         (0x00ul)
#define MQ_9_CSD_SS_HW_EN       (0x01ul)
#define MQ_9_CSD_SS_TH_EN       (0x02ul)
#define MQ_9_CSD_SS_HWTH_EN     (MQ_9_CSD_SS_HW_EN | \
                                             MQ_9_CSD_SS_TH_EN)

#define MQ_9_CSD_AUTOTUNE       MQ_9_CSD_SS_DIS


/*******************************************************************************
* General settings
*******************************************************************************/

#define MQ_9_AUTO_RESET_METHOD_LEGACY (0u)
#define MQ_9_AUTO_RESET_METHOD_SAMPLE (1u)

#define MQ_9_MULTI_FREQ_SCAN_EN       (0u)
#define MQ_9_SENSOR_AUTO_RESET_EN     (0u)
#define MQ_9_SENSOR_AUTO_RESET_METHOD (0u)
#define MQ_9_NUM_CENTROIDS            (1u)
#define MQ_9_4PTS_LOCAL_MAX_EN        (0u)
#define MQ_9_OFF_DEBOUNCE_EN          (0u)
#define MQ_9_CUSTOM_DS_RAM_SIZE       (0u)

/* Defines power status of HW block after scanning */
#define MQ_9_BLOCK_OFF_AFTER_SCAN_EN  (0u)

/* Defines number of scan frequencies */
#if (MQ_9_DISABLE != MQ_9_MULTI_FREQ_SCAN_EN)
    #define MQ_9_NUM_SCAN_FREQS       (3u)
#else
    #define MQ_9_NUM_SCAN_FREQS       (1u)
#endif /* #if (MQ_9_DISABLE != MQ_9_MULTI_FREQ_SCAN_EN) */

/* Data size for thresholds / low baseline reset */
#define MQ_9_SIZE_8BITS               (8u)
#define MQ_9_SIZE_16BITS              (16u)

#define MQ_9_THRESHOLD_SIZE           MQ_9_SIZE_16BITS
typedef uint16 MQ_9_THRESHOLD_TYPE;

#if (MQ_9_AUTO_RESET_METHOD_LEGACY == MQ_9_SENSOR_AUTO_RESET_METHOD)
    #define MQ_9_LOW_BSLN_RST_SIZE        MQ_9_SIZE_8BITS
    typedef uint8 MQ_9_LOW_BSLN_RST_TYPE;
#else
    #define MQ_9_LOW_BSLN_RST_SIZE    (16u)
    typedef uint16 MQ_9_LOW_BSLN_RST_TYPE;
#endif /* #if (MQ_9_AUTO_RESET_METHOD_LEGACY == MQ_9_SENSOR_AUTO_RESET_METHOD) */

/* Coefficient to define touch threshold for proximity sensors */
#define MQ_9_PROX_TOUCH_COEFF         (300u)

/*******************************************************************************
* General Filter Constants
*******************************************************************************/

/* Baseline algorithm options */
#define MQ_9_IIR_BASELINE                 (0u)
#define MQ_9_BUCKET_BASELINE              (1u)

#define MQ_9_BASELINE_TYPE                MQ_9_IIR_BASELINE

/* IIR baseline filter algorithm for regular sensors*/
#define MQ_9_REGULAR_IIR_BL_TYPE          MQ_9_IIR_FILTER_PERFORMANCE

/* IIR baseline coefficients for regular sensors */
#define MQ_9_REGULAR_IIR_BL_N             (1u)
#define MQ_9_REGULAR_IIR_BL_SHIFT         (8u)

/* IIR baseline filter algorithm for proximity sensors*/
#define MQ_9_PROX_IIR_BL_TYPE             MQ_9_IIR_FILTER_PERFORMANCE

/* IIR baseline coefficients for proximity sensors */
#define MQ_9_PROX_IIR_BL_N                (1u)
#define MQ_9_PROX_IIR_BL_SHIFT            (8u)


/* IIR filter constants */
#define MQ_9_IIR_COEFFICIENT_K            (256u)

/* IIR filter type */
#define MQ_9_IIR_FILTER_STANDARD          (1u)
#define MQ_9_IIR_FILTER_PERFORMANCE       (2u)
#define MQ_9_IIR_FILTER_MEMORY            (3u)

/* Regular sensor raw count filters */
#define MQ_9_REGULAR_RC_FILTER_EN         (0u)
#define MQ_9_REGULAR_RC_IIR_FILTER_EN     (0u)
#define MQ_9_REGULAR_RC_MEDIAN_FILTER_EN  (0u)
#define MQ_9_REGULAR_RC_AVERAGE_FILTER_EN (0u)
#define MQ_9_REGULAR_RC_CUSTOM_FILTER_EN  (0u)
#define MQ_9_REGULAR_RC_ALP_FILTER_EN     (0u)

/* Proximity sensor raw count filters */
#define MQ_9_PROX_RC_FILTER_EN            (0u)
#define MQ_9_PROX_RC_IIR_FILTER_EN        (0u)
#define MQ_9_PROX_RC_MEDIAN_FILTER_EN     (0u)
#define MQ_9_PROX_RC_AVERAGE_FILTER_EN    (0u)
#define MQ_9_PROX_RC_CUSTOM_FILTER_EN     (0u)
#define MQ_9_PROX_RC_ALP_FILTER_EN        (0u)

#define MQ_9_ALP_FILTER_EN                (0u)
#define MQ_9_REGULAR_RC_ALP_FILTER_COEFF  (2u)
#define MQ_9_PROX_RC_ALP_FILTER_COEFF     (2u)

/* Raw count filters */
#define MQ_9_RC_FILTER_EN                 (MQ_9_REGULAR_RC_FILTER_EN || MQ_9_PROX_RC_FILTER_EN)

/* IIR raw count filter algorithm for regular sensors */
#define MQ_9_REGULAR_IIR_RC_TYPE          (MQ_9_IIR_FILTER_STANDARD)

/* IIR raw count filter coefficients for regular sensors */
#define MQ_9_REGULAR_IIR_RC_N             (128u)
#define MQ_9_REGULAR_IIR_RC_SHIFT         (0u)

/* IIR raw count filter algorithm for proximity sensors*/
#define MQ_9_PROX_IIR_RC_TYPE             (MQ_9_IIR_FILTER_STANDARD)

/* IIR raw count filter coefficients for proximity sensors */
#define MQ_9_PROX_IIR_RC_N                (128u)
#define MQ_9_PROX_IIR_RC_SHIFT            (0u)

/* Median filter constants */

/* Order of regular sensor median filter */
#define MQ_9_REGULAR_MEDIAN_LEN           (2u)

/* Order of proximity sensor median filter */
#define MQ_9_PROX_MEDIAN_LEN              (2u)

/* Average filter constants*/
#define MQ_9_AVERAGE_FILTER_LEN_2         (1u)
#define MQ_9_AVERAGE_FILTER_LEN_4         (3u)

/* Order of regular sensor average filter */
#define MQ_9_REGULAR_AVERAGE_LEN          (MQ_9_AVERAGE_FILTER_LEN_4)

/* Order of proximity sensor average filter */
#define MQ_9_PROX_AVERAGE_LEN             (MQ_9_AVERAGE_FILTER_LEN_4)

/* Widget baseline coefficient enable */
#define MQ_9_WD_BSLN_COEFF_EN             (0u)

/* Centroid position filters */
#define MQ_9_POSITION_FILTER_EN           (0u)
#define MQ_9_POS_MEDIAN_FILTER_EN         (0u)
#define MQ_9_POS_IIR_FILTER_EN            (0u)
#define MQ_9_POS_ADAPTIVE_IIR_FILTER_EN   (0u)
#define MQ_9_POS_AVERAGE_FILTER_EN        (0u)
#define MQ_9_POS_JITTER_FILTER_EN         (0u)
#define MQ_9_BALLISTIC_MULTIPLIER_EN      (0u)
#define MQ_9_CENTROID_3X3_CSD_EN          (0u)
#define MQ_9_CENTROID_5X5_CSD_EN          (0u)
#define MQ_9_CSD_5X5_MAX_FINGERS          (1u)

#define MQ_9_POS_IIR_COEFF                (128u)
#define MQ_9_POS_IIR_RESET_RADIAL_SLIDER  (35u)

#define MQ_9_CSX_TOUCHPAD_UNDEFINED       (40u)

/* IDAC options */

/* Third-generation HW block IDAC gain */
#define MQ_9_IDAC_GAIN_4X                 (4u)
#define MQ_9_IDAC_GAIN_8X                 (8u)

/* Fourth-generation HW block IDAC gain */
#define MQ_9_IDAC_GAIN_LOW                (0uL)
#define MQ_9_IDAC_GAIN_MEDIUM             (1uL)
#define MQ_9_IDAC_GAIN_HIGH               (2uL)

#define MQ_9_IDAC_SOURCING                (0u)
#define MQ_9_IDAC_SINKING                 (1u)

/* Shield tank capacitor precharge source */
#define MQ_9_CSH_PRECHARGE_VREF           (0u)
#define MQ_9_CSH_PRECHARGE_IO_BUF         (1u)

/* Shield electrode delay */
#define MQ_9_NO_DELAY                     (0u)

#if(MQ_9_ENABLE == MQ_9_CSDV2)
    #define MQ_9_SH_DELAY_5NS             (1u)
    #define MQ_9_SH_DELAY_10NS            (2u)
    #define MQ_9_SH_DELAY_20NS            (3u)
#else
    #if(MQ_9_ENABLE == MQ_9_CSDV1_VER2)
        #define MQ_9_SH_DELAY_10NS        (3u)
        #define MQ_9_SH_DELAY_50NS        (2u)
    #else
        #define MQ_9_SH_DELAY_1CYCLES     (1u)
        #define MQ_9_SH_DELAY_2CYCLES     (2u)
    #endif /* (MQ_9_ENABLE == MQ_9_CSDV1_VER2) */
#endif /* (MQ_9_ENABLE == MQ_9_CSDV2) */

/* Inactive sensor connection options */
#define MQ_9_SNS_CONNECTION_GROUND        (0x00000006Lu)
#define MQ_9_SNS_CONNECTION_HIGHZ         (0x00000000Lu)
#define MQ_9_SNS_CONNECTION_SHIELD        (0x00000002Lu)

/* Sense clock selection options */
#if defined(MQ_9_TAPEOUT_STAR_USED)
    #define MQ_9_CSDV2_REF9P6UA_EN            (0u)
#else
    #define MQ_9_CSDV2_REF9P6UA_EN            (1u)
#endif /* defined(MQ_9_TAPEOUT_STAR_USED) */

#define MQ_9_CLK_SOURCE_DIRECT            (0x00000000Lu)

#define MQ_9_CLK_SOURCE_SSC1              (0x01u)
#define MQ_9_CLK_SOURCE_SSC2              (0x02u)
#define MQ_9_CLK_SOURCE_SSC3              (0x03u)
#define MQ_9_CLK_SOURCE_SSC4              (0x04u)

#define MQ_9_CLK_SOURCE_PRS8              (0x05u)
#define MQ_9_CLK_SOURCE_PRS12             (0x06u)
#define MQ_9_CLK_SOURCE_PRSAUTO           (0xFFu)

#define MQ_9_MFS_IMO                      (0u)
#define MQ_9_MFS_SNS_CLK                  (1u)

/* Defines scan resolutions */
#define MQ_9_RES6BIT                      (6u)
#define MQ_9_RES7BIT                      (7u)
#define MQ_9_RES8BIT                      (8u)
#define MQ_9_RES9BIT                      (9u)
#define MQ_9_RES10BIT                     (10u)
#define MQ_9_RES11BIT                     (11u)
#define MQ_9_RES12BIT                     (12u)
#define MQ_9_RES13BIT                     (13u)
#define MQ_9_RES14BIT                     (14u)
#define MQ_9_RES15BIT                     (15u)
#define MQ_9_RES16BIT                     (16u)

/* Fourth-generation HW block: Initialization switch resistance */
#define MQ_9_INIT_SW_RES_LOW              (0x00000000Lu)
#define MQ_9_INIT_SW_RES_MEDIUM           (0x00000001Lu)
#define MQ_9_INIT_SW_RES_HIGH             (0x00000002Lu)

/* Fourth-generation HW block: Initialization switch resistance */
#define MQ_9_SCAN_SW_RES_LOW              (0x00000000Lu)
#define MQ_9_SCAN_SW_RES_MEDIUM           (0x00000001Lu)
#define MQ_9_SCAN_SW_RES_HIGH             (0x00000002Lu)

/* Fourth-generation HW block: CSD shield switch resistance */
#define MQ_9_SHIELD_SW_RES_LOW            (0x00000000Lu)
#define MQ_9_SHIELD_SW_RES_MEDIUM         (0x00000001Lu)
#define MQ_9_SHIELD_SW_RES_HIGH           (0x00000002Lu)
#define MQ_9_SHIELD_SW_RES_LOW_EMI        (0x00000003Lu)

/* Fourth-generation HW block: CSD shield switch resistance */
#define MQ_9_INIT_SHIELD_SW_RES_LOW       (0x00000000Lu)
#define MQ_9_INIT_SHIELD_SW_RES_MEDIUM    (0x00000001Lu)
#define MQ_9_INIT_SHIELD_SW_RES_HIGH      (0x00000002Lu)
#define MQ_9_INIT_SHIELD_SW_RES_LOW_EMI   (0x00000003Lu)

/* Fourth-generation HW block: CSD shield switch resistance */
#define MQ_9_SCAN_SHIELD_SW_RES_LOW       (0x00000000Lu)
#define MQ_9_SCAN_SHIELD_SW_RES_MEDIUM    (0x00000001Lu)
#define MQ_9_SCAN_SHIELD_SW_RES_HIGH      (0x00000002Lu)
#define MQ_9_SCAN_SHIELD_SW_RES_LOW_EMI   (0x00000003Lu)

/* Sensing method */
#define MQ_9_SENSING_LEGACY               (0x00000000Lu)
#define MQ_9_SENSING_LOW_EMI              (0x00000001Lu)
#define MQ_9_SENSING_FULL_WAVE            (0x00000002Lu)


/*******************************************************************************
* CSD/CSX Common settings
*******************************************************************************/

#define MQ_9_BLOCK_ANALOG_WAKEUP_DELAY_US (0u)

#define MQ_9_MFS_METHOD                   (0u)
#define MQ_9_IMO_FREQUENCY_OFFSET_F1      (20u)
#define MQ_9_IMO_FREQUENCY_OFFSET_F2      (20u)

/*******************************************************************************
* CSD Specific settings
*******************************************************************************/

/* CSD scan method settings */
#define MQ_9_CSD_IDAC_AUTOCAL_EN          (0u)
#define MQ_9_CSD_IDAC_GAIN                (MQ_9_IDAC_GAIN_HIGH)
#define MQ_9_CSD_SHIELD_EN                (0u)
#define MQ_9_CSD_SHIELD_TANK_EN           (0u)
#define MQ_9_CSD_CSH_PRECHARGE_SRC        (MQ_9_CSH_PRECHARGE_VREF)
#define MQ_9_CSD_SHIELD_DELAY             (MQ_9_NO_DELAY)
#define MQ_9_CSD_TOTAL_SHIELD_COUNT       (0u)
#define MQ_9_CSD_SCANSPEED_DIVIDER        (2u)
#define MQ_9_CSD_COMMON_SNS_CLK_EN        (0u)
#define MQ_9_CSD_SNS_CLK_SOURCE           (MQ_9_CLK_SOURCE_PRSAUTO)
#define MQ_9_CSD_SNS_CLK_DIVIDER          (4u)
#define MQ_9_CSD_INACTIVE_SNS_CONNECTION  (MQ_9_SNS_CONNECTION_GROUND)
#define MQ_9_CSD_IDAC_COMP_EN             (0u)
#define MQ_9_CSD_IDAC_CONFIG              (MQ_9_IDAC_SOURCING)
#define MQ_9_CSD_RAWCOUNT_CAL_LEVEL       (85u)
#define MQ_9_CSD_DUALIDAC_LEVEL           (50u)
#define MQ_9_CSD_PRESCAN_SETTLING_TIME    (5u)
#define MQ_9_CSD_SNSCLK_R_CONST           (1000u)
#define MQ_9_CSD_VREF_MV                  (2021u)

/* CSD settings - Fourth-generation HW block */
#define MQ_9_CSD_ANALOG_STARTUP_DELAY_US  (10u)
#define MQ_9_CSD_FINE_INIT_TIME           (10u)
#define MQ_9_CSD_DEDICATED_IDAC_COMP_EN   (0u)
#define MQ_9_CSD_AUTO_ZERO_EN             (0u)
#define MQ_9_CSD_AUTO_ZERO_TIME           (15Lu)
#define MQ_9_CSD_NOISE_METRIC_EN          (0u)
#define MQ_9_CSD_NOISE_METRIC_TH          (1Lu)
#define MQ_9_CSD_INIT_SWITCH_RES          (MQ_9_INIT_SW_RES_MEDIUM)
#define MQ_9_CSD_SENSING_METHOD           (0)
#define MQ_9_CSD_SHIELD_SWITCH_RES        (MQ_9_SHIELD_SW_RES_MEDIUM)
#define MQ_9_CSD_GAIN                     (18Lu)

#define MQ_9_CSD_MFS_METHOD               (u)
#define MQ_9_CSD_MFS_DIVIDER_OFFSET_F1    (1u)
#define MQ_9_CSD_MFS_DIVIDER_OFFSET_F2    (2u)

/*******************************************************************************
* CSX Specific settings
*******************************************************************************/

/* CSX scan method settings */
#define MQ_9_CSX_SCANSPEED_DIVIDER        (2u)
#define MQ_9_CSX_COMMON_TX_CLK_EN         (0u)
#define MQ_9_CSX_TX_CLK_SOURCE            (MQ_9_CLK_SOURCE_PRSAUTO)
#define MQ_9_CSX_TX_CLK_DIVIDER           (40u)
#define MQ_9_CSX_MAX_FINGERS              (1u)
#define MQ_9_CSX_MAX_LOCAL_PEAKS          (5u)
#define MQ_9_CSX_IDAC_AUTOCAL_EN          (0u)
#define MQ_9_CSX_IDAC_BITS_USED           (7u)
#define MQ_9_CSX_RAWCOUNT_CAL_LEVEL       (40u)
#define MQ_9_CSX_IDAC_GAIN                (MQ_9_IDAC_GAIN_MEDIUM)
#define MQ_9_CSX_SKIP_OVSMPL_SPECIFIC_NODES (0u)
#define MQ_9_CSX_MULTIPHASE_TX_EN         (0u)
#define MQ_9_CSX_MAX_TX_PHASE_LENGTH      (0u)

/* CSX settings - Fourth-generation HW block */
#define MQ_9_CSX_ANALOG_STARTUP_DELAY_US  (10u)
#define MQ_9_CSX_AUTO_ZERO_EN             (0u)
#define MQ_9_CSX_AUTO_ZERO_TIME           (15u)
#define MQ_9_CSX_FINE_INIT_TIME           (4u)
#define MQ_9_CSX_NOISE_METRIC_EN          (0u)
#define MQ_9_CSX_NOISE_METRIC_TH          (1u)
#define MQ_9_CSX_INIT_SWITCH_RES          (MQ_9_INIT_SW_RES_MEDIUM)
#define MQ_9_CSX_SCAN_SWITCH_RES          (MQ_9_SCAN_SW_RES_LOW)
#define MQ_9_CSX_INIT_SHIELD_SWITCH_RES   (MQ_9_INIT_SHIELD_SW_RES_HIGH)
#define MQ_9_CSX_SCAN_SHIELD_SWITCH_RES   (MQ_9_SCAN_SHIELD_SW_RES_LOW)

#define MQ_9_CSX_MFS_METHOD               (u)
#define MQ_9_CSX_MFS_DIVIDER_OFFSET_F1    (1u)
#define MQ_9_CSX_MFS_DIVIDER_OFFSET_F2    (2u)

/* Gesture parameters */
#define MQ_9_GES_GLOBAL_EN                (0u)

/*******************************************************************************
* ISX Specific settings
*******************************************************************************/

/* ISX scan method settings */
#define MQ_9_ISX_SCANSPEED_DIVIDER        (2u)
#define MQ_9_ISX_LX_CLK_DIVIDER           (40u)
#define MQ_9_ISX_IDAC_AUTOCAL_EN          (0u)
#define MQ_9_ISX_IDAC_BITS_USED           (7u)
#define MQ_9_ISX_RAWCOUNT_CAL_LEVEL       (30u)
#define MQ_9_ISX_IDAC_GAIN                (MQ_9_IDAC_GAIN_MEDIUM)
#define MQ_9_ISX_SKIP_OVSMPL_SPECIFIC_NODES (0u)
#define MQ_9_ISX_MAX_TX_PHASE_LENGTH      (0u)
#define MQ_9_ISX_PIN_COUNT_LX             (u)
/* ISX settings - Fourth-generation HW block */
#define MQ_9_ISX_AUTO_ZERO_EN             (0u)
#define MQ_9_ISX_AUTO_ZERO_TIME           (15u)
#define MQ_9_ISX_FINE_INIT_TIME           (20u)
#define MQ_9_ISX_NOISE_METRIC_EN          (0u)
#define MQ_9_ISX_NOISE_METRIC_TH          (1u)
#define MQ_9_ISX_INIT_SWITCH_RES          (MQ_9_INIT_SW_RES_MEDIUM)
#define MQ_9_ISX_SCAN_SWITCH_RES          (MQ_9_SCAN_SW_RES_LOW)
#define MQ_9_ISX_INIT_SHIELD_SWITCH_RES   (MQ_9_INIT_SHIELD_SW_RES_HIGH)
#define MQ_9_ISX_SCAN_SHIELD_SWITCH_RES   (MQ_9_SCAN_SHIELD_SW_RES_LOW)
#define MQ_9_ISX_SAMPLE_PHASE_DEG         (30u)

/*******************************************************************************
* Global Parameter Definitions
*******************************************************************************/

/* Compound section definitions */
#define MQ_9_ANY_NONSS_AUTOCAL ((0u != MQ_9_CSX_IDAC_AUTOCAL_EN) || \
                                       (0u != MQ_9_ISX_IDAC_AUTOCAL_EN) || \
                                      ((MQ_9_CSD_AUTOTUNE == MQ_9_CSD_SS_DIS) && (0u != MQ_9_CSD_IDAC_AUTOCAL_EN)))
#define MQ_9_ANYMODE_AUTOCAL (((0u != MQ_9_CSX_IDAC_AUTOCAL_EN) \
                                       || (0u != MQ_9_ISX_IDAC_AUTOCAL_EN)) \
                                       || (0u != MQ_9_CSD_IDAC_AUTOCAL_EN))
/* RAM Global Parameters Definitions */


/* RAM Sensor Parameters Definitions */


/*******************************************************************************
* ADC Specific Macros
*******************************************************************************/
#define MQ_9_ADC_RES8BIT                  (8u)
#define MQ_9_ADC_RES10BIT                 (10u)

#define MQ_9_ADC_FULLRANGE_MODE           (0u)
#define MQ_9_ADC_VREF_MODE                (1u)

#define MQ_9_ADC_MIN_CHANNELS             (1u)
#define MQ_9_ADC_EN                       (1u)
#define MQ_9_ADC_STANDALONE_EN            (1u)
#define MQ_9_ADC_TOTAL_CHANNELS           (1u)
#define MQ_9_ADC_RESOLUTION               (MQ_9_ADC_RES10BIT)
#define MQ_9_ADC_AMUXB_INPUT_EN           (0u)
#define MQ_9_ADC_SELECT_AMUXB_CH          (0u)
#define MQ_9_ADC_AZ_EN                    (1Lu)
#define MQ_9_ADC_AZ_TIME                  (5u)
#define MQ_9_ADC_VREF_MV                  (2133u)
#define MQ_9_ADC_GAIN                     (17Lu)
#define MQ_9_ADC_IDAC_DEFAULT             (15u)
#define MQ_9_ADC_MODCLK_DIV_DEFAULT       (1u)
#define MQ_9_ADC_MEASURE_MODE             (MQ_9_ADC_FULLRANGE_MODE)
#define MQ_9_ADC_ANALOG_STARTUP_DELAY_US  (5u)
#define MQ_9_ADC_ACQUISITION_TIME_US      (10u)

/*******************************************************************************
* Built-In Self-Test Configuration
*******************************************************************************/
#define MQ_9_SELF_TEST_EN                   (0Lu)
#define MQ_9_TST_GLOBAL_CRC_EN              (0Lu)
#define MQ_9_TST_WDGT_CRC_EN                (0Lu)
#define MQ_9_TST_BSLN_DUPLICATION_EN        (0Lu)
#define MQ_9_TST_BSLN_RAW_OUT_RANGE_EN      (0Lu)
#define MQ_9_TST_SNS_SHORT_EN               (0Lu)
#define MQ_9_TST_SNS2SNS_SHORT_EN           (0Lu)
#define MQ_9_TST_SNS_CAP_EN                 (0Lu)
#define MQ_9_TST_SH_CAP_EN                  (0Lu)
#define MQ_9_TST_EXTERNAL_CAP_EN            (0Lu)
#define MQ_9_TST_INTERNAL_CAP_EN            (0Lu)
#define MQ_9_TST_VDDA_EN                    (0Lu)
#define MQ_9_TST_FINE_INIT_TIME             (10Lu)


#define MQ_9_TST_ANALOG_STARTUP_DELAY_US    (23u)

/*******************************************************************************
* Gesture Configuration
*******************************************************************************/
#define MQ_9_TIMESTAMP_INTERVAL             (1Lu)
#define MQ_9_GESTURE_EN_WIDGET_ID           (0Lu)
#define MQ_9_BALLISTIC_EN_WIDGET_ID         (0Lu)


#endif /* CY_SENSE_MQ_9_CONFIGURATION_H */


/* [] END OF FILE */
