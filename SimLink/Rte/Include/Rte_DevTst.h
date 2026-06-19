/**
 * @file    Rte_DevTst.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   RTE-OS interface for DevTst domain initialization and runnables.
 *
 * @details
 * This header provides the OS/scheduler interface to the DevTst framework.
 * It exposes the initialization entry point and all periodic runnable entry
 * points that the scheduler invokes at the appropriate task rates.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * The scheduler must not include DevTst.h, DevTstMgr.h, or any DevTst_*
 * header directly; all access to the DevTst framework is routed exclusively
 * through this RTE interface layer.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | Initial release                      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h; add Module    |
 * |         |            |                  | Info and Version Info sections       |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_DEVTSTOSIF_H
#define RTE_DEVTSTOSIF_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define RTE_DEVTSTOSIF_VENDOR_ID            0xFFFFu

/** AUTOSAR module identifier for the RTE module */
#define RTE_DEVTSTOSIF_MODULE_ID            2u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define RTE_DEVTSTOSIF_SW_MAJOR_VERSION     0x00u

/** Software minor version number */
#define RTE_DEVTSTOSIF_SW_MINOR_VERSION     0x02u

/** Software patch version number */
#define RTE_DEVTSTOSIF_SW_PATCH_VERSION     0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* None */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/* None */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialise the DevTst framework via the RTE OS interface.
 * @note   Must be called once at system startup before any DevTst runnables.
 *         When DEVTST_ENABLED is not defined this function is a no-op.
 */
void Rte_DevTst_Init(void);

/**
 * @brief  10 ms cyclic runnable for the DevTst framework.
 * @note   Wraps DevTst_Run_10ms() and must be invoked by the scheduler
 *         every 10 milliseconds.
 *         When DEVTST_ENABLED is not defined this function is a no-op.
 */
void Rte_Runnable_DevTst_10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_DEVTSTOSIF_H */
