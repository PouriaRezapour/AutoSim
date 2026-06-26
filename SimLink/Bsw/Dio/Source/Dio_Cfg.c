/**
 * @file    Dio_Cfg.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   AUTOSAR DIO Driver – configuration data.
 *
 * @details
 * Contains the definitions of all channel group instances declared in
 * Dio_Cfg.h. Adapt / extend with your project's actual channel groups.
 *
 * Implements [SWS_Dio_00022] / [SRS_Dio_12355]: symbolic names for
 * channel groups are provided via Dio_ChannelGroup_* objects.
 *
 * AUTOSAR CP R24-11
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Dio.h"

/* ─── Version Check ───────────────────────────────────────────────────────── */

#if (DIO_AR_RELEASE_MAJOR_VERSION != DIO_CFG_AR_MAJOR_VERSION)
  #error "Dio_Cfg.c: AUTOSAR major version mismatch with Dio_Cfg.h"
#endif
#if (DIO_AR_RELEASE_MINOR_VERSION != DIO_CFG_AR_MINOR_VERSION)
  #error "Dio_Cfg.c: AUTOSAR minor version mismatch with Dio_Cfg.h"
#endif

/* ─── Channel Group Definitions ──────────────────────────────────────────── */

/*
 * Each Dio_ChannelGroupType contains:
 *   .mask   – 16-bit mask selecting which pins belong to the group.
 *             The selected pins MUST be contiguous (adjoining) within the port
 *             as required by [SWS_Dio_00056].
 *   .offset – Position of the LSB of the mask, counted from the port LSB.
 *             Calculated as: offset = position of first set bit in mask.
 *   .port   – Port that owns this group (use DioConf_DioPort_PORTx).
 *
 * Example: Pins PE2..PE5 of Port E
 *   mask   = 0x003CU  (bits 2,3,4,5 set)
 *   offset = 2U       (first set bit is bit 2)
 *   port   = DioConf_DioPort_PORTE
 *
 * When Dio_ReadChannelGroup() is called the raw IDR value is ANDed with mask
 * and then shifted right by offset, giving an LSB-aligned result per
 * [SWS_Dio_00092][SWS_Dio_00093].
 *
 * When Dio_WriteChannelGroup() is called the caller supplies an LSB-aligned
 * value; the implementation shifts it left by offset, applies the mask, and
 * writes only those bits to the port, leaving other channels untouched per
 * [SWS_Dio_00090][SWS_Dio_00091].
 */

/**
 * @brief  Example channel group: Port E, pins 2..5 (4 adjacent pins).
 * @details This is a placeholder. Replace with your board's actual groups.
 *          mask   = 0x003C (bits 2,3,4,5)
 *          offset = 2
 *          port   = PORTE (index 4)
 */
const Dio_ChannelGroupType Dio_ChannelGroup_Example =
{
    .mask   = 0x003CU,                  /* PE2, PE3, PE4, PE5 */
    .offset = 2U,
    .port   = DioConf_DioPort_PORTE
};

/*
 * Add further channel group definitions here, e.g.:
 *
 * const Dio_ChannelGroupType Dio_ChannelGroup_LedGroup =
 * {
 *     .mask   = 0x00F0U,   // PB4..PB7
 *     .offset = 4U,
 *     .port   = DioConf_DioPort_PORTB
 * };
 */