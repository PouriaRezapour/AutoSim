/**
 * @file    DevTstMgr.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Development test manager — aggregates all DevTst sub-modules.
 *
 * @details
 * DevTstMgr is the single point of contact between the DevTst framework
 * and the rest of the application.  It sequences the initialisation and
 * periodic execution of every registered DevTst_* sub-module.
 *
 * Adding a new test module (e.g. DevTst_Spi):
 *   1. Create DevTst_Spi.c / .h following the DevTst_CanTx pattern.
 *   2. Add DEVTST_SPI_ENABLED to DevTst_Config.h.
 *   3. Include DevTst_Spi.h and call its functions in DevTstMgr.c under
 *      a matching #if DEVTST_ENABLED && DEVTST_SPI_ENABLED guard.
 *
 * Compile-time gate: DEVTST_ENABLED must be 1 in DevTst_Config.h for any
 * code in this module to be compiled.
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

#ifndef DEVTSTMGR_H
#define DEVTSTMGR_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "DevTst_Config.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DEVTSTMGR_VENDOR_ID            0xFFFFu   /* Project vendor identifier  */
#define DEVTSTMGR_MODULE_ID            2050u      /* DevTst framework module ID */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DEVTSTMGR_MAJOR_VERSION        0x00u
#define DEVTSTMGR_MINOR_VERSION        0x02u
#define DEVTSTMGR_PATCH_VERSION        0x03u

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
 * @brief  Initialise all registered DevTst sub-modules.
 * @note   Called once by DevTst_Init().
 */
void DevTstMgr_Init(void);

/**
 * @brief  10 ms periodic execution for all registered DevTst sub-modules.
 * @note   Called every 10 ms by DevTst_Run_10ms().
 */
void DevTstMgr_Run_10ms(void);

#else /* DEVTST_ENABLED == 0 */

#define DevTstMgr_Init()           ((void)0)
#define DevTstMgr_Run_10ms()       ((void)0)

#endif /* DEVTST_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* DEVTSTMGR_H */
