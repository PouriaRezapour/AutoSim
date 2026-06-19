/**
 * @file    DevTst.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Development test framework — top-level public interface.
 *
 * @details
 * This is the sole entry point into the entire DevTst framework from the
 * RTE layer.  It exposes one init function and one periodic run function per
 * supported task rate.  All implementation is delegated to DevTstMgr; this
 * file adds no logic of its own.
 *
 * The only file that should include this header is Rte_DevTst.c.
 * No other module should depend on DevTst.h directly.
 *
 * Compile-time gate: DEVTST_ENABLED must be 1 in DevTst_Config.h for any
 * code in this module to be compiled.  When 0, all public functions become
 * empty macros so that call sites require no conditional compilation.
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

#ifndef DEVTST_H
#define DEVTST_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "DevTst_Config.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DEVTST_VENDOR_ID               0xFFFFu   /* Project vendor identifier  */
#define DEVTST_MODULE_ID               2050u      /* DevTst framework module ID */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DEVTST_MAJOR_VERSION           0x00u
#define DEVTST_MINOR_VERSION           0x02u
#define DEVTST_PATCH_VERSION           0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* None */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/* None */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

#if DEVTST_ENABLED

/**
 * @brief  Initialise the entire DevTst framework.
 * @note   Called once from Rte_DevTst_Init().
 *         Delegates to DevTstMgr_Init().
 */
void DevTst_Init(void);

/**
 * @brief  10 ms periodic execution entry point for the DevTst framework.
 * @note   Called every 10 ms from Rte_Runnable_DevTst_10ms().
 *         Delegates to DevTstMgr_Run_10ms().
 */
void DevTst_Run_10ms(void);

#else /* DEVTST_ENABLED == 0 */

#define DevTst_Init()              ((void)0)
#define DevTst_Run_10ms()          ((void)0)

#endif /* DEVTST_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* DEVTST_H */
