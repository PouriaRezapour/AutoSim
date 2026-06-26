/**
 * @file    DevTstMgr.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   Development test manager — implementation.
 *
 * @details
 * Sequences the initialisation and periodic execution of every DevTst_*
 * sub-module that is compiled in.  Each sub-module is guarded by its own
 * DEVTST_<MODULE>_ENABLED flag from DevTst_Cfg.h so individual tests
 * can be switched on or off without touching this file.
 *
 * Current sub-modules registered:
 *   - DevTst_CanTx    (DEVTST_CANTX_ENABLED)   — 10 ms runnable
 *   - DevTst_DioFlip  (DEVTST_DIOFLIP_ENABLED) — 100 ms runnable
 *
 * To register a future module, follow the pattern in the #if blocks below.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | Initial release                      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the project's    |
 * |         |            |                  | standard types defined in            |
 * |         |            |                  | Std_Types.h.                         |
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Registered DevTst_DioFlip with       |
 * |         |            |                  | 100 ms runnable.                     |
 * |---------|------------|------------------|--------------------------------------|
 */

#include "Std_Types.h"
#include "DevTst_Cfg.h"

#if DEVTST_ENABLED

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "DevTstMgr.h"

#if DEVTST_CANTX_ENABLED
#include "DevTst_CanTx.h"
#endif

#if DEVTST_DIOFLIP_ENABLED
#include "DevTst_DioFlip.h"
#endif

/* Add future sub-module includes here, e.g.:
 * #if DEVTST_SPI_ENABLED
 * #include "DevTst_Spi.h"
 * #endif
 */

/* ─── Public Functions ────────────────────────────────────────────────────── */

void DevTstMgr_Init(void)
{
#if DEVTST_CANTX_ENABLED
    DevTst_CanTx_Init();
#endif

#if DEVTST_DIOFLIP_ENABLED
    DevTst_DioFlip_Init();
#endif

    /* Add future sub-module initialisers here, e.g.:
     * #if DEVTST_SPI_ENABLED
     *     DevTst_Spi_Init();
     * #endif
     */
}

void DevTstMgr_Run_10ms(void)
{
#if DEVTST_CANTX_ENABLED
    DevTst_CanTx_Run_10ms();
#endif

    /* Add future 10 ms sub-module runnables here, e.g.:
     * #if DEVTST_SPI_ENABLED
     *     DevTst_Spi_Run_10ms();
     * #endif
     */
}

void DevTstMgr_Run_100ms(void)
{
#if DEVTST_DIOFLIP_ENABLED
    DevTst_DioFlip_Run_100ms();
#endif

    /* Add future 100 ms sub-module runnables here, e.g.:
     * #if DEVTST_SOMEMODULE_ENABLED
     *     DevTst_SomeModule_Run_100ms();
     * #endif
     */
}

#endif /* DEVTST_ENABLED */
