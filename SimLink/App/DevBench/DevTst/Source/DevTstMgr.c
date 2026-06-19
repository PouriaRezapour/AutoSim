/**
 * @file    DevTstMgr.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Development test manager — implementation.
 *
 * @details
 * Sequences the initialisation and periodic execution of every DevTst_*
 * sub-module that is compiled in.  Each sub-module is guarded by its own
 * DEVTST_<MODULE>_ENABLED flag from DevTst_Config.h so individual tests
 * can be switched on or off without touching this file.
 *
 * Current sub-modules registered:
 *   - DevTst_CanTx  (DEVTST_CANTX_ENABLED)
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
 * |---------|------------|------------------|--------------------------------------|
 */

#include "Std_Types.h"
#include "DevTst_Config.h"

#if DEVTST_ENABLED

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "DevTstMgr.h"

#if DEVTST_CANTX_ENABLED
#include "DevTst_CanTx.h"
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

#endif /* DEVTST_ENABLED */
