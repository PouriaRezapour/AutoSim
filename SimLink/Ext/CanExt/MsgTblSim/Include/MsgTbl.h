/**
 * @file    MsgTbl.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-23
 * @version 0.2.4
 * @brief   Generic CAN message/signal table framework.
 *
 * @details
 * This header provides the generic types and helper macros used to
 * describe CAN frames and the signals packed inside them.
 *
 * A frame is described by a CanFrameDef_t, which references an array
 * of CanSignalDef_t entries. Each CanSignalDef_t describes where a
 * signal lives inside the 8-byte data field and how to convert the
 * raw bit-field value to/from its physical value
 * (physical = raw * scale + offset).
 *
 * Per-message header files (e.g. AbsInformation1.h) use this framework
 * by:
 *   1) Defining a SignalIdx_t enum listing every signal in the frame,
 *      with the final entry named <MSG>_SIG_COUNT to represent the
 *      total number of signals (used for array sizing/iteration).
 *   2) Defining a static const CanSignalDef_t array, indexed by the
 *      SignalIdx_t enum, using the CAN_SIGNAL_xxx() macros below.
 *   3) Defining a static const CanFrameDef_t describing the frame
 *      itself and pointing to the signal array.
 *
 * @par Revision History:
 * |---------|------------|--------------|--------------------------------------|
 * | Version | Date       | Author       | Description                          |
 * |---------|------------|--------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour   | Initial release                      |
 * |---------|------------|--------------|--------------------------------------|
 */

#ifndef MSGTBL_H
#define MSGTBL_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include <stdint.h>
#include <stddef.h>

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

#define MSGTBL_VERSION      "0.2.4"

/** Sentinel CanFrameDef_t.period value for non-periodic (event-triggered) frames. */
#define CAN_PERIOD_EVENT    0U

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief Valid data length code (DLC) values for a classic CAN frame.
 *
 * Each enumerator's value equals the corresponding number of data bytes.
 */
typedef enum
{
    CAN_DLC_0 = 0, /**< 0 data bytes */
    CAN_DLC_1 = 1, /**< 1 data byte  */
    CAN_DLC_2 = 2, /**< 2 data bytes */
    CAN_DLC_3 = 3, /**< 3 data bytes */
    CAN_DLC_4 = 4, /**< 4 data bytes */
    CAN_DLC_5 = 5, /**< 5 data bytes */
    CAN_DLC_6 = 6, /**< 6 data bytes */
    CAN_DLC_7 = 7, /**< 7 data bytes */
    CAN_DLC_8 = 8  /**< 8 data bytes */
} CanDlc_t;

/**
 * @brief Bit/byte ordering convention for a signal within a frame.
 */
typedef enum
{
    CAN_BYTEORDER_LITTLE_ENDIAN = 0, /**< Intel: start bit = LSB of start byte */
    CAN_BYTEORDER_BIG_ENDIAN    = 1  /**< Motorola: start bit = MSB of start byte */
} CanByteOrder_t;

/**
 * @brief Raw data interpretation for a signal.
 */
typedef enum
{
    CAN_SIGTYPE_UNSIGNED = 0, /**< Unsigned integer */
    CAN_SIGTYPE_SIGNED   = 1, /**< Signed (two's complement) integer */
    CAN_SIGTYPE_FLOAT    = 2, /**< IEEE-754 encoded value */
    CAN_SIGTYPE_ENUM     = 3, /**< Enumerated/state value (no scaling) */
    CAN_SIGTYPE_BOOL     = 4  /**< Single-bit boolean flag */
} CanSignalType_t;

/**
 * @brief Describes a single signal's location and scaling within a frame.
 */
typedef struct
{
    uint8          startByte;  /**< Starting byte index (0 .. MSGTBL_MAX_DLC-1) */
    uint8          startBit;   /**< Starting bit within startByte (0-7) */
    uint8          length;     /**< Length in bits (1-64) */
    CanByteOrder_t   byteOrder;  /**< Bit numbering convention */
    CanSignalType_t  type;       /**< Raw data interpretation */
    uint64         initialValue; /**< Raw reset/default value loaded into the frame at init */
    float64           scale;      /**< physical = raw * scale + offset */
    float64           offset;     /**< physical = raw * scale + offset */
    float64           min;        /**< Physical minimum (for validation) */
    float64           max;        /**< Physical maximum (for validation) */
} CanSignalDef_t;

/**
 * @brief Describes a CAN frame (message) and its associated signal table.
 */
typedef struct
{
    uint16               id;          /**< CAN identifier (11-bit standard ID) */
    CanDlc_t               dlc;         /**< Data length code (number of valid data bytes, 0-8) */
    uint16               period;      /**< Transmission period in ms, or CAN_PERIOD_EVENT for event-triggered frames */
    const CanSignalDef_t*  pSignals;    /**< Pointer to array of signal defs */
    uint8                 signalCount; /**< Number of entries in pSignals */
} CanFrameDef_t;

/* ─── Signal Definition Macros ──────────────────────────────────────────────
 *
 * Used to populate a static const CanSignalDef_t array in a per-message
 * header. The array should be indexed by a matching SignalIdx_t enum,
 * whose final entry (<MSG>_SIG_COUNT) gives the array length.
 * ─────────────────────────────────────────────────────────────────────────
 */

/** Single-bit boolean flag signal, no scaling. */
#define CAN_SIGNAL_BOOL(BYTE, BIT, INIT) \
    { .startByte = (BYTE), .startBit = (BIT), .length = 1U, \
      .byteOrder = CAN_BYTEORDER_LITTLE_ENDIAN, .type = CAN_SIGTYPE_BOOL, \
      .initialValue = (INIT), \
      .scale = 1.0, .offset = 0.0, .min = 0.0, .max = 1.0 }

/** Generic unsigned-integer signal with scale/offset. */
#define CAN_SIGNAL_UNSIGNED(BYTE, BIT, LEN, INIT, SCALE, OFFSET, MIN, MAX) \
    { .startByte = (BYTE), .startBit = (BIT), .length = (LEN), \
      .byteOrder = CAN_BYTEORDER_LITTLE_ENDIAN, .type = CAN_SIGTYPE_UNSIGNED, \
      .initialValue = (INIT), \
      .scale = (SCALE), .offset = (OFFSET), .min = (MIN), .max = (MAX) }

/** Generic signed-integer signal with scale/offset. */
#define CAN_SIGNAL_SIGNED(BYTE, BIT, LEN, INIT, SCALE, OFFSET, MIN, MAX) \
    { .startByte = (BYTE), .startBit = (BIT), .length = (LEN), \
      .byteOrder = CAN_BYTEORDER_LITTLE_ENDIAN, .type = CAN_SIGTYPE_SIGNED, \
      .initialValue = (INIT), \
      .scale = (SCALE), .offset = (OFFSET), .min = (MIN), .max = (MAX) }

/**
 * Multi-bit enumerated/state signal (raw value = state, no scaling).
 */
#define CAN_SIGNAL_ENUM(BYTE, BIT, LEN, INIT) \
    { .startByte = (BYTE), .startBit = (BIT), .length = (LEN), \
      .byteOrder = CAN_BYTEORDER_LITTLE_ENDIAN, .type = CAN_SIGTYPE_ENUM, \
      .initialValue = (INIT), \
      .scale = 1.0, .offset = 0.0, .min = 0.0, \
      .max = (float64)((1ULL << (LEN)) - 1ULL) }

/**
 * Reserved/unused bit-field placeholder, documents gaps in the frame layout.
 */
#define CAN_SIGNAL_RESERVED(BYTE, BIT, LEN) \
    { .startByte = (BYTE), .startBit = (BIT), .length = (LEN), \
      .byteOrder = CAN_BYTEORDER_LITTLE_ENDIAN, .type = CAN_SIGTYPE_UNSIGNED, \
      .initialValue = 0U, \
      .scale = 1.0, .offset = 0.0, .min = 0.0, \
      .max = (float64)((1ULL << (LEN)) - 1ULL) }

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/*
 * Layer 1 — Bit-level
 * Lowest-level pack/unpack of an N-bit field at an arbitrary byte/bit
 * position within an 8-byte payload.  Both byte orders are handled here
 * and nowhere else.
 */

/**
 * @brief  Extract an N-bit field from an 8-byte CAN payload.
 *
 * @param  data       8-byte frame payload.
 * @param  startByte  Byte index of the field's start bit (0-7).
 * @param  startBit   Bit offset within startByte (0 = LSB of that byte).
 * @param  length     Number of bits to extract (1-64).
 * @param  byteOrder  CAN_BYTEORDER_LITTLE_ENDIAN or CAN_BYTEORDER_BIG_ENDIAN.
 * @return Raw extracted value, zero-extended into a uint64.
 */
uint64 MsgTbl_GetRawBits(const uint8  data[8],
                            uint8        startByte,
                            uint8        startBit,
                            uint8        length,
                            CanByteOrder_t byteOrder);

/**
 * @brief  Insert an N-bit field into an 8-byte CAN payload.
 *
 * @param  data       8-byte frame payload (modified in place).
 * @param  startByte  Byte index of the field's start bit (0-7).
 * @param  startBit   Bit offset within startByte (0 = LSB of that byte).
 * @param  length     Number of bits to insert (1-64).
 * @param  byteOrder  CAN_BYTEORDER_LITTLE_ENDIAN or CAN_BYTEORDER_BIG_ENDIAN.
 * @param  rawValue   Value to insert (bits above length are ignored).
 */
void MsgTbl_SetRawBits(uint8        data[8],
                        uint8        startByte,
                        uint8        startBit,
                        uint8        length,
                        CanByteOrder_t byteOrder,
                        uint64       rawValue);

/*
 * Layer 2 — Scaling
 * Convert between a raw integer and a physical float64 using the
 * scale/offset stored in a CanSignalDef_t.
 */

/**
 * @brief  Convert a raw integer value to a physical float64.
 *
 * physical = (raw * scale) + offset.
 * Signed signals are sign-extended before scaling.
 *
 * @param  pSig   Signal definition (scale, offset, min, max, type).
 * @param  raw    Raw integer value extracted from the frame.
 * @param  clamp  Non-zero to clamp the result to [pSig->min, pSig->max].
 * @return Physical value as a float64, or 0.0 if pSig is NULL.
 */
float64 MsgTbl_RawToPhysical(const CanSignalDef_t *pSig,
                             uint64              raw,
                             int                   clamp);

/**
 * @brief  Convert a physical float64 to a raw integer value.
 *
 * raw = round((physical - offset) / scale), clamped to fit in length bits.
 * For signed signals the result is the two's-complement bit pattern.
 *
 * @param  pSig      Signal definition (scale, offset, length, type).
 * @param  physical  Physical value to convert.
 * @return Raw integer, or 0 if pSig is NULL or scale is zero.
 */
uint64 MsgTbl_PhysicalToRaw(const CanSignalDef_t *pSig,
                               float64                physical);

/*
 * Layer 3 — Signal
 * Combine bit-level extraction with scaling to read/write one signal
 * using a CanSignalDef_t.
 */

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
                     float64               *pValue);

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
                     float64                value);

/*
 * Layer 4 — Frame
 * Operate on a complete CanFrameDef_t rather than individual signals.
 */

/**
 * @brief  Zero a frame buffer then write every signal's initialValue.
 *
 * @param  data    8-byte frame payload to initialise (modified in place).
 * @param  pFrame  Frame definition (signal list, DLC).
 * @return 0 on success, -1 if any pointer is NULL.
 */
int MsgTbl_InitFrame(uint8              data[8],
                     const CanFrameDef_t *pFrame);

/*
 * Layer 5 — Utilities
 * Range validation.
 */

/**
 * @brief  Check whether a physical value lies within a signal's declared range.
 *
 * @param  pSig   Signal definition (min, max).
 * @param  value  Physical value to test.
 * @return 1 if value is in [min, max], 0 otherwise (or if pSig is NULL).
 */
int MsgTbl_IsValueInRange(const CanSignalDef_t *pSig, float64 value);

#endif /* MSGTBL_H */