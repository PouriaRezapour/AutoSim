/**
 * @file    DevTst_Config.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Compile-time configuration for the DevTst framework.
 *
 * @details
 * This header is the single location where DevTst modules are enabled or
 * disabled.  It follows the same pattern as Os_TasksConfig.h: each flag is
 * a named #define set to 1 (enabled) or 0 (disabled).
 *
 * Master switch:
 *   DEVTST_ENABLED — must be 1 for any test code to be compiled in.
 *   Set to 0 for production builds; the entire framework becomes zero
 *   object code without touching any other file.
 *
 * Per-module switches:
 *   Each DEVTST_<MODULE>_ENABLED flag activates one sub-module inside
 *   DevTstMgr.  A sub-module flag has no effect when DEVTST_ENABLED is 0.
 *
 * Adding a new test module:
 *   1. Add a DEVTST_<MODULE>_ENABLED entry in the sub-module section below.
 *   2. Create DevTst_<Module>.c / .h.
 *   3. Register the module in DevTstMgr.c.
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

#ifndef DEVTST_CONFIG_H
#define DEVTST_CONFIG_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DEVTST_VENDOR_ID               0xFFFFu   /* Project vendor identifier  */
#define DEVTST_MODULE_ID               2050u      /* DevTst framework module ID */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DEVTST_MAJOR_VERSION           0x00u
#define DEVTST_MINOR_VERSION           0x02u
#define DEVTST_PATCH_VERSION           0x03u

/* ─── Master Switch ───────────────────────────────────────────────────────── */

/**
 * @brief  Master enable for the entire DevTst framework.
 * 
 * |-------|---------------------------------------------------------------|
 * | Value | Effect                                                        |
 * |-------|---------------------------------------------------------------|
 * |   1   | DevTst framework compiled in; per-module flags take effect.  |
 * |   0   | No DevTst code is compiled; all entry points are no-ops.     |
 * |-------|---------------------------------------------------------------|
 */
#define DEVTST_ENABLED                 1

/* ─── Per-Module Switches ─────────────────────────────────────────────────── */

/**
 * @brief  Enable the CAN TX periodic test (DevTst_CanTx).
 *         Transmits a fixed 8-byte frame at ID 0x1 every 10 ms.
 *         Has no effect when DEVTST_ENABLED is 0.
 * 
 * |-------|----------------------------------|
 * | Value | Effect                           |
 * |-------|----------------------------------|
 * |   1   | DevTst_CanTx compiled in.       |
 * |   0   | DevTst_CanTx excluded.          |
 * |-------|----------------------------------|
 */
#define DEVTST_CANTX_ENABLED           1

/* Add future per-module switches here, e.g.:
 * #define DEVTST_SPI_ENABLED          0
 * #define DEVTST_CANTXRX_ENABLED      0
 * #define DEVTST_UART_ENABLED         0
 */

/* ─── Consistency Check ───────────────────────────────────────────────────── */

#if !DEVTST_ENABLED && DEVTST_CANTX_ENABLED
#undef DEVTST_CANTX_ENABLED
#define DEVTST_CANTX_ENABLED           0
#endif

#endif /* DEVTST_CONFIG_H */
