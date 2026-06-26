/**
 * @file    DevTst_CanTx.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Development test module for CAN TX verification.
 *
 * @details
 * Provides a periodic CAN TX burst test that transmits three Classic CAN
 * frames (IDs 0x1, 0x2, 0x3) at every run cycle via
 * DevTst_CanTx_TransmitBurst().  Originally developed to verify the CAN
 * physical layer against an oscilloscope; retained in the DevTst framework
 * for on-demand regression use.
 *
 * This module is part of the DevTst framework and is exempt from the
 * project layering rule that restricts application modules from including
 * BSW headers directly. DevTst_* modules may include any layer.
 *
 * Compile-time gate: both DEVTST_ENABLED and DEVTST_CANTX_ENABLED must be
 * set to 1 in DevTst_Cfg.h for this module to be compiled in.
 * When either flag is 0 all public functions become no-ops via the macros
 * below and no object code is emitted.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | Initial release                      |
 * | 0.2.2   | 2025-06-01 | A.Rezapour       | Update description: single-frame →   |
 * |         |            |                  | burst transmit via Can_TransmitBurst |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the project's    |
 * |         |            |                  | standard types defined in            |
 * |         |            |                  | Std_Types.h.                         |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef DEVTST_CANTX_H
#define DEVTST_CANTX_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "DevTst_Cfg.h"
#include "Can.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DEVTST_CANTX_VENDOR_ID         0xFFFFu   /* Project vendor identifier  */
#define DEVTST_CANTX_MODULE_ID         2050u      /* DevTst framework module ID */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DEVTST_CANTX_MAJOR_VERSION     0x00u
#define DEVTST_CANTX_MINOR_VERSION     0x02u
#define DEVTST_CANTX_PATCH_VERSION     0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* Base CAN identifier for the first TX burst frame */
#define DEVTST_CANTX_FRAME_ID          0x1U

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

#if DEVTST_ENABLED && DEVTST_CANTX_ENABLED

/**
 * @brief  Initialise the CAN TX test module.
 * @note   Called once by DevTstMgr_Init().
 */
void DevTst_CanTx_Init(void);

/**
 * @brief  Transmit a burst of three Classic CAN frames via the Tx FIFO queue.
 *
 * @details
 * Enqueues three frames with IDs 0x1, 0x2, and 0x3 back-to-back.  Each
 * frame carries a distinct 8-byte payload.  The function is non-blocking:
 * if the FIFO is full when a given frame is attempted that frame is silently
 * skipped, consistent with the non-blocking contract of the 10 ms task.
 *
 * @note   Must be called after Can_Init() and Can_Start().
 * @return CAN_OK if every frame was queued, CAN_NOT_INIT if the CAN module
 *         is not running, CAN_ERROR if at least one HAL call returned an error.
 */
CanStatus_t DevTst_CanTx_TransmitBurst(void);

/**
 * @brief  Periodic execution: transmit a burst of three test frames on the CAN bus.
 * @note   Called every 10 ms by DevTstMgr_Run_10ms().
 *         Non-blocking — frames are skipped if the Tx FIFO is full.
 */
void DevTst_CanTx_Run_10ms(void);

#else /* DEVTST_ENABLED && DEVTST_CANTX_ENABLED not satisfied */

#define DevTst_CanTx_Init()                ((void)0)
#define DevTst_CanTx_TransmitBurst()       ((void)0)
#define DevTst_CanTx_Run_10ms()            ((void)0)

#endif /* DEVTST_ENABLED && DEVTST_CANTX_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* DEVTST_CANTX_H */
