/**
 * @file    DevTst_CanTx.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Development test module for CAN TX verification — implementation.
 *
 * @details
 * Transmits a burst of three Classic CAN frames (IDs 0x1, 0x2, and 0x3,
 * each with a distinct 8-byte payload) every time DevTst_CanTx_Run_10ms()
 * is called.  The burst is issued through DevTst_CanTx_TransmitBurst(),
 * which enqueues all frames into the FDCAN Tx FIFO queue back-to-back in
 * a single call.
 *
 * Layering exception: this file intentionally includes BSW headers (Can.h)
 * directly.  This is permitted exclusively for DevTst_* modules whose purpose
 * is cross-layer testing.  No other application module may do the same.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | Initial release                      |
 * | 0.2.2   | 2025-06-01 | A.Rezapour       | Replace single-frame Can_Transmit    |
 * |         |            |                  | with Can_TransmitBurst; remove       |
 * |         |            |                  | Can_HwAb.h include (no longer needed)|
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the project's    |
 * |         |            |                  | standard types defined in            |
 * |         |            |                  | Std_Types.h.                         |
 * |---------|------------|------------------|--------------------------------------|
 */

#include "Std_Types.h"
#include "DevTst_Cfg.h"

#if DEVTST_ENABLED && DEVTST_CANTX_ENABLED

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "DevTst_CanTx.h"

/* BSW direct include — permitted for DevTst_* modules only */
#include "Can.h"

#include <string.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define LOG(fmt, ...)  /* Swap for your project logger if available */

/** Number of frames transmitted in a single DevTst_CanTx_TransmitBurst() call */
#define DEVTST_CANTX_BURST_FRAME_COUNT     3U

/* ─── Public Functions ────────────────────────────────────────────────────── */

void DevTst_CanTx_Init(void)
{
    /* No state to initialise for a pure TX test */
}

/**
 * @brief  Transmit a burst of DEVTST_CANTX_BURST_FRAME_COUNT Classic CAN frames.
 *
 * @details
 * Enqueues three frames with IDs 1, 2, and 3 back-to-back into the FDCAN
 * Tx FIFO queue.  Each frame carries a distinct 8-byte payload chosen to
 * produce varied bit patterns that are easily identified on a logic analyser
 * without decoding firmware.  The function is non-blocking: if the FIFO is
 * full when a frame is attempted, that frame is skipped and the next one is
 * tried, matching the non-blocking contract of DevTst_CanTx_Run_10ms().
 *
 * @note  Must be called only after Can_Init() and Can_Start() have both
 *        returned CAN_OK.
 * @return CAN_OK if every frame was queued successfully,
 *         CAN_NOT_INIT if the CAN module is not running,
 *         CAN_ERROR if at least one HAL call failed.
 */
CanStatus_t DevTst_CanTx_TransmitBurst(void)
{
    /* Burst frame definitions ──────────────────────────────────────────── */

    /* Frame 0: ID 1 — ramp pattern, easy to verify byte order on analyser */
    static const uint8 s_burstPayload0[CAN_MAX_DATA_BYTES] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
    };

    /* Frame 1: ID 2 — alternating 0xAA / 0x55 stress pattern */
    static const uint8 s_burstPayload1[CAN_MAX_DATA_BYTES] = {
        0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55
    };

    /* Frame 2: ID 3 — legacy bring-up signature retained for regression */
    static const uint8 s_burstPayload2[CAN_MAX_DATA_BYTES] = {
        0xDE, 0xAD, 0xBE, 0xEF, 0x0A, 0x0B, 0x0C, 0x0D
    };

    /* ────────────────────────────────────────────────────────────────────
     * Burst transmission table
     * Each entry holds { identifier, pointer-to-payload }.
     * Adding or removing frames requires only a change to this table and
     * the DEVTST_CANTX_BURST_FRAME_COUNT macro above.
     * ──────────────────────────────────────────────────────────────────── */
    struct {
        uint32       id;
        const uint8 *pPayload;
    } const burstFrames[DEVTST_CANTX_BURST_FRAME_COUNT] = {
        { 0x001U, s_burstPayload0 },
        { 0x002U, s_burstPayload1 },
        { 0x003U, s_burstPayload2 },
    };

    CanStatus_t status = CAN_OK;

    for (uint8 i = 0U; i < DEVTST_CANTX_BURST_FRAME_COUNT; i++)
    {
        CanTxHeader_t txHeader = Can_BuildTxHeader(burstFrames[i].id, FDCAN_DLC_BYTES_8);

        CanStatus_t frameStatus = Can_Transmit(&txHeader, burstFrames[i].pPayload);

        /*
         * CAN_TX_PENDING means the FIFO was momentarily full; the frame is
         * skipped but subsequent frames still get a chance.  Any other
         * non-OK code is a hard HAL error and causes the function to report
         * failure after finishing the remaining frames.
         */
        if ((frameStatus != CAN_OK) && (frameStatus != CAN_TX_PENDING))
        {
            LOG("DevTst_CanTx_TransmitBurst: frame %u failed (status %d)", i, frameStatus);
            status = CAN_ERROR;
        }
    }

    return status;
}

void DevTst_CanTx_Run_10ms(void)
{
    /*
     * Transmit three frames (IDs 1, 2, 3) back-to-back via the Tx FIFO queue.
     * CAN_TX_PENDING (FIFO full) and individual frame errors are handled
     * internally by DevTst_CanTx_TransmitBurst(), keeping this function
     * non-blocking.
     */
    DevTst_CanTx_TransmitBurst();
}

#endif /* DEVTST_ENABLED && DEVTST_CANTX_ENABLED */
