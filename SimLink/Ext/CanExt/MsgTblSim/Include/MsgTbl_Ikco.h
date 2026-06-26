/**
 * @file    MsgTbl_Ikco.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-23
 * @version 0.2.4
 * @brief   IKCO-2024 project-specific CAN message table extensions.
 *
 * @details
 * This header extends the generic MsgTbl.h framework with concepts that
 * are specific to the IKCO-2024 message table:
 *
 *   - Rolling counter / checksum signal helper macros
 *     (CAN_SIGNAL_ROLLING_COUNTER, CAN_SIGNAL_CHECKSUM), used by frames
 *     that carry SAE-J1850 CRC8 protection (e.g. *_RollCntr / *_Checksum).
 *
 *   - CanNode_t: bitmask enum for the 8 ECUs referenced by the
 *     "Monitored by" column of Supervision frames. Bit order matches the
 *     message table convention (left to right = MSB to LSB):
 *     TCU - EMS - TPMS - EPS - ABS - BCM - ICU - EGS.
 *
 *   - IkcoFrameDef_t: wraps a generic CanFrameDef_t with the additional
 *     "Supervision Frame" flag and "Monitored by" bitmask.
 *
 * Per-message header files for this project should include this header
 * (instead of MsgTbl.h directly) and use IkcoFrameDef_t for the frame
 * definition. Non-supervision frames simply set
 * .isSupervisionFrame = false and .monitoredBy = CAN_MONITORED_BY_NONE.
 *
 * @par Revision History:
 * |---------|------------|--------------|--------------------------------------|
 * | Version | Date       | Author       | Description                          |
 * |---------|------------|--------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour   | Initial release                      |
 * |---------|------------|--------------|--------------------------------------|
 */

#ifndef MSGTBL_IKCO_H
#define MSGTBL_IKCO_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include <stdint.h>
#include <stdbool.h>
#include "MsgTbl.h"

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/** monitoredBy value meaning "not a supervision frame / monitored by no node". */
#define CAN_MONITORED_BY_NONE   0x00U

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief Bitmask of ECUs referenced by a Supervision frame's "Monitored by" byte.
 *
 * Bit order (MSB to LSB) matches the message table convention:
 * TCU - EMS - TPMS - EPS - ABS - BCM - ICU - EGS.
 * Example: 0x44 (0b01000100) => CAN_NODE_EMS | CAN_NODE_BCM.
 */
typedef enum
{
    CAN_NODE_EGS  = 0x01, /**< Bit 0: EGS     */
    CAN_NODE_ICU  = 0x02, /**< Bit 1: ICU     */
    CAN_NODE_BCM  = 0x04, /**< Bit 2: BCM     */
    CAN_NODE_ABS  = 0x08, /**< Bit 3: ABS/ESC */
    CAN_NODE_EPS  = 0x10, /**< Bit 4: EPS     */
    CAN_NODE_TPMS = 0x20, /**< Bit 5: TPMS    */
    CAN_NODE_EMS  = 0x40, /**< Bit 6: EMS     */
    CAN_NODE_TCU  = 0x80  /**< Bit 7: TCU     */
} CanNode_t;

/**
 * @brief IKCO-2024 frame definition: a generic CanFrameDef_t plus
 *        Supervision-frame / network-management metadata and direct
 *        signal indices for the checksum and rolling-counter fields.
 *
 * Use IKCO_SIGNAL_IDX_NONE for checksumSignalIdx / rollCntrSignalIdx
 * when the frame does not carry that protection.
 */

/** Sentinel value meaning "this frame has no checksum / rolling-counter signal". */
#define IKCO_SIGNAL_IDX_NONE  0xFFU

typedef struct
{
    CanFrameDef_t  frame;              /**< Generic frame definition (id, dlc, period, signals) */
    boolean           isSupervisionFrame; /**< true if this frame is a network-management Supervision frame */
    uint8        monitoredBy;        /**< Bitmask of CanNode_t: nodes that monitor this Supervision frame */
    uint8        checksumSignalIdx;  /**< Index into frame.pSignals for the checksum byte, or IKCO_SIGNAL_IDX_NONE */
    uint8        rollCntrSignalIdx;  /**< Index into frame.pSignals for the rolling counter byte, or IKCO_SIGNAL_IDX_NONE */
} IkcoFrameDef_t;

/* ─── Signal Definition Macros ──────────────────────────────────────────────
 *
 * IKCO-specific signal helpers, built on top of CAN_SIGNAL_UNSIGNED from
 * MsgTbl.h. Both occupy a full byte (8 bits) starting at bit 0.
 * ─────────────────────────────────────────────────────────────────────────
 */

/**
 * Rolling counter signal (one full byte). MAX is the counter's wrap value
 * as specified in the message table (e.g. 14, 255).
 */
#define CAN_SIGNAL_ROLLING_COUNTER(BYTE, MAX) \
    CAN_SIGNAL_UNSIGNED((BYTE), 0U, 8U, 0U, 1.0, 0.0, 0.0, (MAX))

/**
 * Checksum signal (one full byte), SAE-J1850 CRC8 over the rest of the frame.
 */
#define CAN_SIGNAL_CHECKSUM(BYTE) \
    CAN_SIGNAL_UNSIGNED((BYTE), 0U, 8U, 0U, 1.0, 0.0, 0.0, 255.0)

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Calculate the SAE-J1850 CRC8 checksum over a CAN frame payload.
 *
 * Covers all bytes within dlc.  The caller must zero the checksum byte
 * in the buffer before passing it (MsgTblIkco_VerifyFrame handles this
 * automatically).
 *
 * @param  data  8-byte frame payload.
 * @param  dlc   Number of bytes to include in the checksum (0-8).
 * @return Computed CRC8 byte value.
 */
uint8 MsgTblIkco_CalculateChecksum8(const uint8 data[8], uint8 dlc);

/**
 * @brief  Increment a rolling counter with project-defined wrap-around.
 *
 * The counter increments by 1.  After reaching maxValue it wraps to 0
 * on the next call (maxValue itself is a valid transmitted value).
 *
 * @param  current   Current raw counter value.
 * @param  maxValue  Maximum raw value before wrap (from the signal definition,
 *                   e.g. 14.0 for BRAKE_RollCntr, 255.0 for others).
 * @return Next counter value.
 */
uint8 MsgTblIkco_NextRollingCounter(uint8 current, float64 maxValue);

/**
 * @brief  Verify a received frame: recompute checksum and check rolling counter.
 *
 * Locates the "*_Checksum" and "*_RollCntr" signals by name suffix.
 * If a signal is absent its check is silently skipped (returns true for
 * that check), so the function works on frames that carry only one
 * protection or neither.
 *
 * @param  data             8-byte received frame payload.
 * @param  pFrame           IKCO frame definition.
 * @param  expectedCounter  Counter value the receiver expects to see.
 * @return true  if all present checks pass; false if any check fails or
 *               a pointer is NULL.
 */
boolean MsgTblIkco_VerifyFrame(const uint8         data[8],
                              const IkcoFrameDef_t *pFrame,
                              uint8               expectedCounter);

/**
 * @brief  Test whether a given ECU node monitors a Supervision frame.
 *
 * Always returns false for non-supervision frames.
 *
 * @param  pFrame  IKCO frame definition.
 * @param  node    One of the CanNode_t enumerators.
 * @return true if the node's bit is set in pFrame->monitoredBy, false otherwise.
 */
boolean MsgTblIkco_IsMonitoredBy(const IkcoFrameDef_t *pFrame, CanNode_t node);

#endif /* MSGTBL_IKCO_H */
