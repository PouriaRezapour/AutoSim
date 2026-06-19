/**
 * @file    MsgTbl_Ikco.c
 * @author  Ali Rezapour
 * @date    2025-06-13
 * @version 0.2.4
 * @brief   Implementation of IKCO-2024 project-specific CAN frame operations.
 *
 * @details
 * Implements the IKCO-2024-specific functions declared (to be prototyped)
 * in MsgTbl_Ikco.h.  Every function in this file operates on
 * IkcoFrameDef_t (which wraps CanFrameDef_t) and carries knowledge of
 * the project conventions that must not leak into the generic MsgTbl layer:
 *
 *   - SAE-J1850 CRC8 checksum computation over the relevant frame bytes.
 *   - Rolling-counter increment with project-defined per-frame wrap values.
 *   - Combined receive-side frame verification (checksum + counter).
 *   - Supervision-frame node-monitoring bitmask queries.
 *
 * Design notes
 * ─────────────
 * • MsgTblIkco_VerifyFrame() is the receive-side counterpart.  A matching
 *   MsgTblIkco_FinalizeFrame() (compute counter + checksum before TX) should
 *   be added once the transmit responsibility per node is confirmed.
 * • The checksum and rolling-counter signals are located via explicit indices
 *   (checksumSignalIdx, rollCntrSignalIdx) stored in IkcoFrameDef_t, with
 *   IKCO_SIGNAL_IDX_NONE (0xFF) as the sentinel for "not present".  This
 *   replaces the former name-suffix lookup and removes all string operations
 *   from the hot path.
 * • Rolling-counter wrap is per-signal (max from the signal definition),
 *   so different frames can cap at different values (e.g. 14 or 255).
 *
 * @par Revision History:
 * |---------|------------|--------------|--------------------------------------|
 * | Version | Date       | Author       | Description                          |
 * |---------|------------|--------------|--------------------------------------|
 * | 0.2.4   | 2025-06-13 | Ali Rezapour | Initial release                      |
 * |---------|------------|--------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "MsgTbl_Ikco.h"
#include "MsgTbl.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define LOG(fmt, ...)  /* expand to nothing in production; hook a logger here */

/**
 * SAE-J1850 CRC8 polynomial: x^8 + x^4 + x^3 + x^2 + 1 → 0x1D
 * reflected as 0x1D (no bit-reversal; J1850 operates MSB-first).
 */
#define J1850_CRC8_POLY  0x1DU

/* ─── Private Types ───────────────────────────────────────────────────────── */

/* (none) */

/* ─── Private Variables ───────────────────────────────────────────────────── */

/** Pre-computed SAE-J1850 CRC8 lookup table (populated once at first use). */
static uint8 s_crc8Table[256];
static int     s_crc8TableReady = 0;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static void    BuildCrc8Table(void);
static uint8 ComputeCrc8(const uint8 *pData, uint8 length);

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Calculate the SAE-J1850 CRC8 checksum over a CAN frame payload.
 *
 * The checksum covers all bytes within the DLC *except* the checksum byte
 * itself, matching the IKCO-2024 message table convention.  The caller is
 * responsible for excluding the checksum byte (i.e. pass the raw data[]
 * array and the checksum byte index separately, or call through
 * MsgTblIkco_VerifyFrame / MsgTblIkco_FinalizeFrame which handle this).
 *
 * @param  data  8-byte frame payload.
 * @param  dlc   Number of data bytes covered by the checksum (0–8).
 * @return Computed CRC8 byte value.
 */
uint8 MsgTblIkco_CalculateChecksum8(const uint8 data[8], uint8 dlc)
{
    if (!s_crc8TableReady)
    {
        BuildCrc8Table();
    }

    if (data == NULL || dlc == 0U || dlc > 8U)
    {
        return 0U;
    }

    return ComputeCrc8(data, dlc);
}

/**
 * @brief  Increment a rolling counter with project-defined wrap-around.
 *
 * The counter increments by 1 each call.  When it reaches maxValue it
 * wraps back to 0 on the *next* call (i.e. maxValue itself is a valid
 * transmitted value; the wrap occurs after maxValue).
 *
 * @param  current   Current raw counter value.
 * @param  maxValue  Maximum raw value before wrap (from the signal definition).
 * @return Next counter value.
 */
uint8 MsgTblIkco_NextRollingCounter(uint8 current, float64 maxValue)
{
    uint8 max = (maxValue >= 255.0) ? 255U : (uint8)maxValue;

    return (current >= max) ? 0U : (uint8)(current + 1U);
}

/**
 * @brief  Verify a received frame: recompute checksum and check rolling counter.
 *
 * Uses the checksumSignalIdx and rollCntrSignalIdx fields stored directly
 * in IkcoFrameDef_t to locate the relevant signals without any string lookup.
 * If an index is set to IKCO_SIGNAL_IDX_NONE the corresponding check is
 * skipped (the function still returns true for that check).
 *
 * @param  data             8-byte received frame payload.
 * @param  pFrame           IKCO frame definition (provides signal list + DLC).
 * @param  expectedCounter  The counter value the receiver expects to see.
 * @return true  if both the checksum and the rolling counter are valid
 *               (or if a signal is absent and the check is skipped);
 *         false if either check fails.
 */
boolean MsgTblIkco_VerifyFrame(const uint8          data[8],
                              const IkcoFrameDef_t  *pFrame,
                              uint8                expectedCounter)
{
    boolean checksumOk = true;
    boolean counterOk  = true;

    if (data == NULL || pFrame == NULL)
    {
        return false;
    }

    const CanFrameDef_t *pCanFrame = &pFrame->frame;
    uint8              dlc       = (uint8)pCanFrame->dlc;

    /* ── Checksum verification ────────────────────────────────────────── */
    if (pFrame->checksumSignalIdx != IKCO_SIGNAL_IDX_NONE &&
        pFrame->checksumSignalIdx < pCanFrame->signalCount)
    {
        const CanSignalDef_t *pCkSig = &pCanFrame->pSignals[pFrame->checksumSignalIdx];
        uint8               ckByte = pCkSig->startByte;

        /* Build a temporary buffer with the checksum byte zeroed */
        uint8 tmp[8];
        (void)memcpy(tmp, data, 8U);
        tmp[ckByte] = 0x00U;

        /* Compute CRC over the whole DLC (checksum byte is 0 during calc) */
        uint8 computed = MsgTblIkco_CalculateChecksum8(tmp, dlc);
        uint8 received = data[ckByte];

        checksumOk = (computed == received);
    }

    /* ── Rolling-counter verification ────────────────────────────────── */
    if (pFrame->rollCntrSignalIdx != IKCO_SIGNAL_IDX_NONE &&
        pFrame->rollCntrSignalIdx < pCanFrame->signalCount)
    {
        const CanSignalDef_t *pCntSig = &pCanFrame->pSignals[pFrame->rollCntrSignalIdx];
        float64                rxPhysical = 0.0;

        (void)MsgTbl_GetSignal(data, pCntSig, &rxPhysical);
        counterOk = ((uint8)rxPhysical == expectedCounter);
    }

    return checksumOk && counterOk;
}

/**
 * @brief  Test whether a given ECU node monitors a Supervision frame.
 *
 * Performs a simple bitmask AND between pFrame->monitoredBy and node.
 * Non-supervision frames always return false.
 *
 * @param  pFrame  IKCO frame definition.
 * @param  node    ECU node bitmask (one of the CanNode_t enumerators).
 * @return true if the node monitors this frame, false otherwise.
 */
boolean MsgTblIkco_IsMonitoredBy(const IkcoFrameDef_t *pFrame, CanNode_t node)
{
    if (pFrame == NULL)
    {
        return false;
    }

    if (!pFrame->isSupervisionFrame)
    {
        return false;
    }

    return (pFrame->monitoredBy & (uint8)node) != 0U;
}

/* ─── Private Functions ───────────────────────────────────────────────────── */

/**
 * @brief  Populate the SAE-J1850 CRC8 lookup table.
 *
 * Uses the standard table-driven approach: for each possible byte value,
 * iterate 8 bits and apply the polynomial.
 */
static void BuildCrc8Table(void)
{
    uint16 i;

    for (i = 0U; i < 256U; i++)
    {
        uint8 crc = (uint8)i;
        uint8 bit;

        for (bit = 0U; bit < 8U; bit++)
        {
            if ((crc & 0x80U) != 0U)
            {
                crc = (uint8)((crc << 1U) ^ J1850_CRC8_POLY);
            }
            else
            {
                crc = (uint8)(crc << 1U);
            }
        }
        s_crc8Table[i] = crc;
    }

    s_crc8TableReady = 1;
}

/**
 * @brief  Compute SAE-J1850 CRC8 over a byte array using the lookup table.
 *
 * @param  pData   Pointer to the first byte.
 * @param  length  Number of bytes to process.
 * @return CRC8 result.
 */
static uint8 ComputeCrc8(const uint8 *pData, uint8 length)
{
    uint8 crc = 0xFFU;   /* J1850 init value */
    uint8 i;

    for (i = 0U; i < length; i++)
    {
        crc = s_crc8Table[(uint8)(crc ^ pData[i])];
    }

    return crc;
}
