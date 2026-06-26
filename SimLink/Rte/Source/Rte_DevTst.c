/**
 * @file    Rte_DevTst.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   RTE OS Interface for the DevTst domain.
 *
 * @details
 * This is the only file permitted to include DevTst.h directly.
 * The init function calls DevTst_Init() to bring up the full test framework.
 * Each runnable maps one scheduler slot to its corresponding DevTst periodic
 * entry point.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * When DEVTST_ENABLED is not defined, DevTst_Init(), DevTst_Run_10ms(), and
 * DevTst_Run_100ms() expand to no-ops (defined in DevTst.h), so this file
 * compiles cleanly without any conditional compilation at the call sites.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | Initial release                      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h                |
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Added Rte_Runnable_DevTst_100ms()    |
 * |         |            |                  | for the DioFlip module.              |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_DevTst.h"
#include "DevTst.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Rte_DevTst_Init(void)
{
    DevTst_Init();
}

void Rte_Runnable_DevTst_10ms(void)
{
    DevTst_Run_10ms();
}

void Rte_Runnable_DevTst_100ms(void)
{
    DevTst_Run_100ms();
}
