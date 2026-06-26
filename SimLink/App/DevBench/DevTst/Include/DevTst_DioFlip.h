/**
 * @file    DevTst_DioFlip.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   Development test module for DIO toggle verification.
 *
 * @details
 * Toggles LED1 (HWAB_DIO_LED1) via DioIf_Toggle() every time
 * DevTst_DioFlip_Run_100ms() is called.  The visible blink on the
 * on-board LED provides a low-cost, no-instrument confirmation that
 * the DIO hardware abstraction layer, the AUTOSAR DIO driver, and the
 * GPIO peripheral are all functioning end-to-end.
 *
 * This module is part of the DevTst framework and is exempt from the
 * project layering rule that restricts application modules from
 * including BSW headers directly.  DevTst_* modules may include any
 * layer.
 *
 * Compile-time gate: both DEVTST_ENABLED and DEVTST_DIOFLIP_ENABLED
 * must be set to 1 in DevTst_Cfg.h for this module to be compiled in.
 * When either flag is 0 all public functions become no-ops via the
 * macros below and no object code is emitted.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef DEVTST_DIOFLIP_H
#define DEVTST_DIOFLIP_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "DevTst_Cfg.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DEVTST_DIOFLIP_VENDOR_ID        0xFFFFu   /**< Project vendor identifier  */
#define DEVTST_DIOFLIP_MODULE_ID        2050u      /**< DevTst framework module ID */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DEVTST_DIOFLIP_MAJOR_VERSION    0x00u
#define DEVTST_DIOFLIP_MINOR_VERSION    0x01u
#define DEVTST_DIOFLIP_PATCH_VERSION    0x00u

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

#if DEVTST_ENABLED && DEVTST_DIOFLIP_ENABLED

/**
 * @brief  Initialise the DIO flip test module.
 * @note   Called once by DevTstMgr_Init().
 *         Currently a no-op; provided for symmetry with other DevTst
 *         sub-modules and for future use (e.g. setting a known LED state
 *         before the periodic runnable starts).
 */
void DevTst_DioFlip_Init(void);

/**
 * @brief  100 ms periodic execution: toggle LED1 via the DIO HwAb layer.
 * @note   Called every 100 ms by DevTstMgr_Run_100ms().
 *         Calls DioIf_Toggle(HWAB_DIO_LED1) once per invocation,
 *         producing a 200 ms period (5 Hz) blink visible on the board.
 */
void DevTst_DioFlip_Run_100ms(void);

#else /* DEVTST_ENABLED && DEVTST_DIOFLIP_ENABLED not satisfied */

#define DevTst_DioFlip_Init()           ((void)0)
#define DevTst_DioFlip_Run_100ms()      ((void)0)

#endif /* DEVTST_ENABLED && DEVTST_DIOFLIP_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* DEVTST_DIOFLIP_H */
