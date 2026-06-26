/**
 * @file    DevTst_DioFlip.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   Development test module for DIO toggle verification — implementation.
 *
 * @details
 * Toggles LED1 (HWAB_DIO_LED1) via DioIf_Toggle() every 100 ms.
 * The resulting 5 Hz blink on the on-board LED provides a no-instrument
 * confirmation that the DIO HwAb layer, the AUTOSAR DIO driver, and the
 * GPIO peripheral are all operational end-to-end.
 *
 * Layering exception: this file intentionally includes HwAb headers
 * (DioIf.h) directly.  This is permitted exclusively for DevTst_*
 * modules whose purpose is cross-layer testing.  No other application
 * module may do the same.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#include "Std_Types.h"
#include "DevTst_Cfg.h"

#if DEVTST_ENABLED && DEVTST_DIOFLIP_ENABLED

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "DevTst_DioFlip.h"

/* HwAb direct include — permitted for DevTst_* modules only */
#include "DioIf.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Initialise the DIO flip test module.
 *
 * @details No state to initialise for a pure toggle test.  Function is
 *          provided for symmetry with other DevTst sub-modules and to
 *          allow future extensions (e.g. driving LED1 LOW to a known
 *          state before the periodic runnable begins).
 */
void DevTst_DioFlip_Init(void)
{
    /* No initialisation required */
}

/**
 * @brief  Toggle LED1 via the DIO hardware abstraction layer.
 *
 * @details Calls DioIf_Toggle(HWAB_DIO_LED1) once per 100 ms
 *          invocation.  The toggle produces a square wave with a 200 ms
 *          period (5 Hz) on the LED1 output, which is directly visible
 *          on the board and can also be captured on a logic analyser
 *          or oscilloscope for timing verification.
 *
 *          DioIf_Toggle() internally calls Dio_FlipChannel()
 *          ([SWS_Dio_00190]), which performs an atomic read-modify-write
 *          on the GPIO output register via BSRR.
 *
 * @note   Must be called only after DioIf_Init() has completed.
 */
void DevTst_DioFlip_Run_100ms(void)
{
    DioIf_Toggle(HWAB_DIO_LED1);
}

#endif /* DEVTST_ENABLED && DEVTST_DIOFLIP_ENABLED */
