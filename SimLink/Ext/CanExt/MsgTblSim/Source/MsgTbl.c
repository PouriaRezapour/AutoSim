/**
 * @file    MsgTbl.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-23
 * @version 0.2.4
 * @brief   Implementation of the generic CAN message/signal table framework.
 *
 * @details
 * Implements the bit-level, scaling, signal, frame, and lookup layers
 * declared (to be prototyped) in MsgTbl.h.  All operations are purely
 * frame-format-agnostic: they depend only on CanSignalDef_t /
 * CanFrameDef_t and carry no project-specific knowledge.
 *
 * Layer overview (bottom to top):
 *   1. Bit-level  : MsgTbl_GetRawBits / MsgTbl_SetRawBits
 *   2. Scaling    : MsgTbl_RawToPhysical / MsgTbl_PhysicalToRaw
 *   3. Signal     : MsgTbl_GetSignal / MsgTbl_SetSignal
 *   4. Frame      : MsgTbl_InitFrame
 *   5. Utilities  : MsgTbl_IsValueInRange
 *
 * @par Revision History:
 * |---------|------------|--------------|--------------------------------------|
 * | Version | Date       | Author       | Description                          |
 * |---------|------------|--------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour   | Initial release                      |
 * |---------|------------|--------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "MsgTbl.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define LOG(fmt, ...)  /* expand to nothing in production; hook a logger here */

/** Maximum number of data bytes in a classic CAN frame. */
#define MSGTBL_MAX_DLC  8U

/* ─── Private Types ───────────────────────────────────────────────────────── */

/* (none at this layer) */

/* ─── Private Variables ───────────────────────────────────────────────────── */

/* (none at this layer) */

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static uint64 ExtractLE(const uint8 data[MSGTBL_MAX_DLC],
                           uint8 startByte, uint8 startBit,
                           uint8 length);

static void     InsertLE(uint8 data[MSGTBL_MAX_DLC],
                          uint8 startByte, uint8 startBit,
                          uint8 length, uint64 rawValue);

static uint64 ExtractBE(const uint8 data[MSGTBL_MAX_DLC],
                           uint8 startByte, uint8 startBit,
                           uint8 length);

static void     InsertBE(uint8 data[MSGTBL_MAX_DLC],
                          uint8 startByte, uint8 startBit,
                          uint8 length, uint64 rawValue);

/* ─── Public Functions ────────────────────────────────────────────────────── */

/* ── 1. Bit-level layer ───────────────────────────────────────────────────── */

/**
 * @brief  Extract an N-bit field from an 8-byte CAN data buffer.
 *
 * @param  data       8-byte frame payload.
 * @param  startByte  Index of the byte that contains the signal's start bit.
 * @param  startBit   Bit offset within startByte (0 = LSB of that byte).
 * @param  length     Number of bits to extract (1-64).
 * @param  byteOrder  CAN_BYTEORDER_LITTLE_ENDIAN (Intel) or
 *                    CAN_BYTEORDER_BIG_ENDIAN (Motorola).
 * @return Extracted raw value, zero-extended into a uint64.
 */
uint64 MsgTbl_GetRawBits(const uint8        data[8],
                            uint8              startByte,
                            uint8              startBit,
                            uint8              length,
                            CanByteOrder_t       byteOrder)
{
    if (byteOrder == CAN_BYTEORDER_BIG_ENDIAN)
    {
        return ExtractBE(data, startByte, startBit, length);
    }
    return ExtractLE(data, startByte, startBit, length);
}

/**
 * @brief  Insert an N-bit field into an 8-byte CAN data buffer.
 *
 * @param  data       8-byte frame payload (modified in place).
 * @param  startByte  Index of the byte that contains the signal's start bit.
 * @param  startBit   Bit offset within startByte (0 = LSB of that byte).
 * @param  length     Number of bits to insert (1-64).
 * @param  byteOrder  CAN_BYTEORDER_LITTLE_ENDIAN (Intel) or
 *                    CAN_BYTEORDER_BIG_ENDIAN (Motorola).
 * @param  rawValue   Value to insert (upper bits beyond length are ignored).
 */
void MsgTbl_SetRawBits(uint8        data[8],
                        uint8        startByte,
                        uint8        startBit,
                        uint8        length,
                        CanByteOrder_t byteOrder,
                        uint64       rawValue)
{
    if (byteOrder == CAN_BYTEORDER_BIG_ENDIAN)
    {
        InsertBE(data, startByte, startBit, length, rawValue);
    }
    else
    {
        InsertLE(data, startByte, startBit, length, rawValue);
    }
}

/* ── 2. Scaling layer ─────────────────────────────────────────────────────── */

/**
 * @brief  Convert a raw integer value to a physical float64.
 *
 * physical = (raw * scale) + offset.
 * The result is clamped to [pSig->min, pSig->max] when clamp != 0.
 *
 * @param  pSig   Signal definition supplying scale, offset, min, max.
 * @param  raw    Raw integer value extracted from the frame.
 * @param  clamp  Non-zero to clamp the result to [min, max].
 * @return Physical value as a float64.
 */
float64 MsgTbl_RawToPhysical(const CanSignalDef_t *pSig,
                             uint64              raw,
                             int                   clamp)
{
    float64 physical;

    if (pSig == NULL)
    {
        return 0.0;
    }

    if (pSig->type == CAN_SIGTYPE_SIGNED)
    {
        int64_t signedRaw = (int64_t)raw;
        if ((uint8)pSig->length > 0U &&
            (uint8)pSig->length < 64U &&
            ((raw >> (pSig->length - 1U)) & 1U) != 0U)
        {
            signedRaw |= (int64_t)(~0ULL << pSig->length);
        }
        physical = ((float64)signedRaw * pSig->scale) + pSig->offset;
    }
    else
    {
        physical = ((float64)raw * pSig->scale) + pSig->offset;
    }

    if (clamp)
    {
        if (physical < pSig->min) { physical = pSig->min; }
        if (physical > pSig->max) { physical = pSig->max; }
    }

    return physical;
}

/**
 * @brief  Convert a physical float64 to a raw integer value.
 *
 * raw = round((physical - offset) / scale).
 * The result is clamped so it fits in pSig->length bits.
 *
 * @param  pSig      Signal definition supplying scale, offset, length, type.
 * @param  physical  Physical value to convert.
 * @return Raw integer value ready to be packed into the frame.
 */
uint64 MsgTbl_PhysicalToRaw(const CanSignalDef_t *pSig,
                               float64                physical)
{
    float64   rawfloat64;
    uint64 raw;
    uint64 maxRaw;

    if (pSig == NULL || pSig->scale == 0.0)
    {
        return 0U;
    }

    rawfloat64 = round((physical - pSig->offset) / pSig->scale);

    if (pSig->type == CAN_SIGTYPE_SIGNED)
    {
        if (pSig->length > 0U && pSig->length < 64U)
        {
            float64 minSigned = -(float64)(1ULL << (pSig->length - 1U));
            float64 maxSigned =  (float64)((1ULL << (pSig->length - 1U)) - 1ULL);

            if (rawfloat64 < minSigned) { rawfloat64 = minSigned; }
            if (rawfloat64 > maxSigned) { rawfloat64 = maxSigned; }
        }

        /* Convert to int64_t then mask to the signal width so only the
         * relevant N bits are returned (upper bits are zeroed). */
        int64_t  signedRaw = (int64_t)rawfloat64;
        uint64 widthMask = (pSig->length < 64U)
                             ? ((1ULL << pSig->length) - 1ULL)
                             : UINT64_MAX;
        raw = (uint64)signedRaw & widthMask;
    }
    else
    {
        /* Unsigned/enum/boolean/float: clamp to [0, 2^N - 1]. */
        maxRaw = (pSig->length < 64U) ? ((1ULL << pSig->length) - 1ULL) : UINT64_MAX;

        if (rawfloat64 < 0.0)                   { raw = 0U; }
        else if (rawfloat64 > (float64)maxRaw)   { raw = maxRaw; }
        else                                   { raw = (uint64)rawfloat64; }
    }

    return raw;
}

/* ── 3. Signal layer ──────────────────────────────────────────────────────── */

/**
 * @brief  Read one signal's physical value from a frame payload.
 *
 * @param  data    8-byte frame payload.
 * @param  pSig    Signal definition.
 * @param  pValue  Output: physical value (must not be NULL).
 * @return 0 on success, -1 if any pointer is NULL.
 */
int MsgTbl_GetSignal(const uint8        data[8],
                     const CanSignalDef_t *pSig,
                     float64               *pValue)
{
    uint64 raw;

    if (data == NULL || pSig == NULL || pValue == NULL)
    {
        return -1;
    }

    raw     = MsgTbl_GetRawBits(data, pSig->startByte, pSig->startBit,
                                 pSig->length, pSig->byteOrder);
    *pValue = MsgTbl_RawToPhysical(pSig, raw, 0 /* no clamping on read */);

    return 0;
}

/**
 * @brief  Write one signal's physical value into a frame payload.
 *
 * @param  data   8-byte frame payload (modified in place).
 * @param  pSig   Signal definition.
 * @param  value  Physical value to write.
 * @return 0 on success, -1 if any pointer is NULL.
 */
int MsgTbl_SetSignal(uint8              data[8],
                     const CanSignalDef_t *pSig,
                     float64                value)
{
    uint64 raw;

    if (data == NULL || pSig == NULL)
    {
        return -1;
    }

    raw = MsgTbl_PhysicalToRaw(pSig, value);
    MsgTbl_SetRawBits(data, pSig->startByte, pSig->startBit,
                      pSig->length, pSig->byteOrder, raw);

    return 0;
}

/* ── 4. Frame layer ───────────────────────────────────────────────────────── */

/**
 * @brief  Zero a frame buffer then write every signal's initialValue.
 *
 * @param  data    8-byte frame payload to initialise (modified in place).
 * @param  pFrame  Frame definition supplying the signal list.
 * @return 0 on success, -1 if any pointer is NULL.
 */
int MsgTbl_InitFrame(uint8              data[8],
                     const CanFrameDef_t *pFrame)
{
    uint8 i;

    if (data == NULL || pFrame == NULL || pFrame->pSignals == NULL)
    {
        return -1;
    }

    (void)memset(data, 0, MSGTBL_MAX_DLC);

    for (i = 0U; i < pFrame->signalCount; i++)
    {
        const CanSignalDef_t *pSig = &pFrame->pSignals[i];
        MsgTbl_SetRawBits(data, pSig->startByte, pSig->startBit,
                          pSig->length, pSig->byteOrder, pSig->initialValue);
    }

    return 0;
}

/* ── 5. Utility layer ─────────────────────────────────────────────────────── */

/**
 * @brief  Check whether a physical value lies within a signal's declared range.
 *
 * @param  pSig   Signal definition supplying min and max.
 * @param  value  Physical value to test.
 * @return 1 if value is in [min, max], 0 otherwise (or if pSig is NULL).
 */
int MsgTbl_IsValueInRange(const CanSignalDef_t *pSig, float64 value)
{
    if (pSig == NULL)
    {
        return 0;
    }
    return (value >= pSig->min && value <= pSig->max) ? 1 : 0;
}

/* ─── Private Functions ───────────────────────────────────────────────────── */

/**
 * @brief  Extract a little-endian (Intel) bit field from the payload.
 *
 * The start bit is the LSB of the field within startByte.
 * Bits spill into successive higher-index bytes.
 */
static uint64 ExtractLE(const uint8 data[MSGTBL_MAX_DLC],
                           uint8 startByte, uint8 startBit,
                           uint8 length)
{
    uint64 result   = 0U;
    uint32 bitPos   = (uint32)startByte * 8U + (uint32)startBit;
    uint8  bitsLeft = length;
    uint8  outBit   = 0U;

    while (bitsLeft > 0U)
    {
        uint8 byteIdx    = (uint8)(bitPos / 8U);
        uint8 bitInByte  = (uint8)(bitPos % 8U);
        uint8 bitsInByte = (uint8)(8U - bitInByte);

        if (byteIdx >= MSGTBL_MAX_DLC)
        {
            break;
        }

        if (bitsInByte > bitsLeft) { bitsInByte = bitsLeft; }

        uint8 mask = (uint8)((1UL << bitsInByte) - 1UL);
        uint8 bits = (uint8)((data[byteIdx] >> bitInByte) & mask);

        result  |= (uint64)bits << outBit;
        outBit  += bitsInByte;
        bitPos  += bitsInByte;
        bitsLeft = (uint8)(bitsLeft - bitsInByte);
    }

    return result;
}

/**
 * @brief  Insert a little-endian (Intel) bit field into the payload.
 */
static void InsertLE(uint8 data[MSGTBL_MAX_DLC],
                      uint8 startByte, uint8 startBit,
                      uint8 length, uint64 rawValue)
{
    uint32 bitPos   = (uint32)startByte * 8U + (uint32)startBit;
    uint8  bitsLeft = length;
    uint8  srcBit   = 0U;

    while (bitsLeft > 0U)
    {
        uint8 byteIdx    = (uint8)(bitPos / 8U);
        uint8 bitInByte  = (uint8)(bitPos % 8U);
        uint8 bitsInByte = (uint8)(8U - bitInByte);

        if (byteIdx >= MSGTBL_MAX_DLC)
        {
            break;
        }

        if (bitsInByte > bitsLeft) { bitsInByte = bitsLeft; }

        uint8 mask = (uint8)((1UL << bitsInByte) - 1UL);
        uint8 bits = (uint8)((rawValue >> srcBit) & mask);

        data[byteIdx] = (uint8)(  (data[byteIdx] & (uint8)(~((uint16)mask << bitInByte)))
                                   | (uint8)((uint16)bits << bitInByte) );

        srcBit  += bitsInByte;
        bitPos  += bitsInByte;
        bitsLeft = (uint8)(bitsLeft - bitsInByte);
    }
}

/**
 * @brief  Extract a big-endian (Motorola) bit field from the payload.
 *
 * The start bit is the MSB of the field within startByte.
 * Bits spill into successive higher-index bytes.
 */
static uint64 ExtractBE(const uint8 data[MSGTBL_MAX_DLC],
                           uint8 startByte, uint8 startBit,
                           uint8 length)
{
    uint64 result   = 0U;
    uint8  byteIdx  = startByte;
    int8_t   bitIdx   = (int8_t)startBit;   /* counts down within byte */
    uint8  bitsLeft = length;

    while (bitsLeft > 0U)
    {
        if (byteIdx >= MSGTBL_MAX_DLC)
        {
            break;
        }

        uint8 bitsInByte = (uint8)(bitIdx + 1);  /* bits remaining in current byte */
        if (bitsInByte > bitsLeft) { bitsInByte = bitsLeft; }

        uint8 shift = (uint8)(bitIdx + 1U - bitsInByte);
        uint8 mask  = (uint8)((1UL << bitsInByte) - 1UL);
        uint8 bits  = (uint8)((data[byteIdx] >> shift) & mask);

        result   = (result << bitsInByte) | (uint64)bits;
        bitsLeft = (uint8)(bitsLeft - bitsInByte);

        byteIdx++;
        bitIdx = 7;   /* subsequent bytes start from their MSB */
    }

    return result;
}

/**
 * @brief  Insert a big-endian (Motorola) bit field into the payload.
 */
static void InsertBE(uint8 data[MSGTBL_MAX_DLC],
                      uint8 startByte, uint8 startBit,
                      uint8 length, uint64 rawValue)
{
    uint8 byteIdx  = startByte;
    int8_t  bitIdx   = (int8_t)startBit;
    uint8 bitsLeft = length;
    uint8 srcShift = length;   /* we consume MSB-first */

    while (bitsLeft > 0U)
    {
        if (byteIdx >= MSGTBL_MAX_DLC)
        {
            break;
        }

        uint8 bitsInByte = (uint8)(bitIdx + 1);
        if (bitsInByte > bitsLeft) { bitsInByte = bitsLeft; }

        srcShift = (uint8)(srcShift - bitsInByte);
        uint8 shift = (uint8)(bitIdx + 1U - bitsInByte);
        uint8 mask  = (uint8)((1UL << bitsInByte) - 1UL);
        uint8 bits  = (uint8)((rawValue >> srcShift) & mask);

        data[byteIdx] = (uint8)(  (data[byteIdx] & (uint8)(~((uint16)mask << shift)))
                                   | (uint8)((uint16)bits << shift) );

        bitsLeft = (uint8)(bitsLeft - bitsInByte);
        byteIdx++;
        bitIdx = 7;
    }
}
