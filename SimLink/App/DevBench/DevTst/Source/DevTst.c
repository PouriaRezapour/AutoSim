/**
 * @file    DevTst.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Development test framework — top-level implementation.
 *
 * @details
 * Thin delegation layer between the RTE OS interface (Rte_DevTst) and
 * the test manager (DevTstMgr).  Contains no test logic; its sole purpose
 * is to provide a stable, named API boundary so that the RTE layer never
 * needs to know about individual test sub-modules or the manager internals.
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

#include "DevTst.h"
#include "DevTstMgr.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

void DevTst_Init(void)
{
    DevTstMgr_Init();
}

void DevTst_Run_10ms(void)
{
    DevTstMgr_Run_10ms();
}

#endif /* DEVTST_ENABLED */
