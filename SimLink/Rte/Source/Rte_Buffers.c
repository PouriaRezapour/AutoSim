/**
 * @file    Rte_Buffers.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR RTE Shared Signal Buffers — Implementation
 *
 * @details
 * Single point of truth for all inter-SWC signal storage. This module owns
 * the static variables that cross SWC boundaries and exposes them only
 * through typed accessor functions declared in Rte_Buffers.h.
 *
 * No other module may declare a variable that holds an inter-SWC signal.
 *
 * Currently managed signals
 * ─────────────────────────
 *   BtnValue        — Written by Rte_Hmi;       read by Rte_PcComMgr.
 *   ValveStatus     — Written by Rte_VlvSigObs; read by any consumer.
 *   CommandSignals  — Written by the command-word producer;
 *                     read by Rte_CmdSigMgr.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial release (BtnValue only)      |
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Added ValveStatus, CommandSignals    |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Buffers.h"

/* ─── Private Variables (shared signal storage) ───────────────────────────── */

/**
 * @brief  BtnValue inter-SWC signal buffer.
 *         Written by Rte_Hmi; read by Rte_PcComMgr.
 */
static Rte_BT_BtnValue_t s_btnValue = 0.0f;

/**
 * @brief  ValveStatus inter-SWC signal buffer.
 *         Written by Rte_VlvSigObs (PpValveStatusPacked provide port).
 *         Read by any consumer SWC via Rte_Buffers_Get_ValveStatus().
 *         Bits 14-15 are always 0 (cleared by VlvSigObs before writing).
 */
static Rte_BT_ValveStatus_t s_valveStatus = 0x0000u;

/**
 * @brief  CommandSignals inter-SWC signal buffer.
 *         Written by the command-word producer via Rte_Buffers_Set_CommandSignals().
 *         Read by Rte_CmdSigMgr (RpCommandSignalsPacked require port).
 *         Bit 7 is masked to 0 by CmdSigMgr before use.
 */
static Rte_BT_CommandSignals_t s_commandSignals = 0x00u;

/* ─── Public Function Implementations ────────────────────────────────────── */

/**
 * @brief  Initialise all shared signal buffers to their safe defaults.
 */
void Rte_Buffers_Init(void)
{
    s_btnValue       = 0.0f;
    s_valveStatus    = 0x0000u;
    s_commandSignals = 0x00u;
}

/**
 * @brief  Write the BtnValue shared signal buffer.
 * @param  value  New accumulated button value from the Hmi SWC.
 */
void Rte_Buffers_Set_BtnValue(Rte_BT_BtnValue_t value)
{
    s_btnValue = value;
}

/**
 * @brief  Read the BtnValue shared signal buffer.
 * @return Current accumulated button value.
 */
Rte_BT_BtnValue_t Rte_Buffers_Get_BtnValue(void)
{
    return s_btnValue;
}

/* ─── ValveStatus ─────────────────────────────────────────────────────────── */

/**
 * @brief  Write the ValveStatus shared signal buffer.
 * @param  value  Packed uint16 valve/motor status word from VlvSigObs SWC.
 *                Bits 14-15 must already be cleared by the caller.
 */
void Rte_Buffers_Set_ValveStatus(Rte_BT_ValveStatus_t value)
{
    s_valveStatus = value;
}

/**
 * @brief  Read the ValveStatus shared signal buffer.
 * @return Current packed uint16 valve/motor status word.
 *         Bits 14-15 are always 0.
 */
Rte_BT_ValveStatus_t Rte_Buffers_Get_ValveStatus(void)
{
    return s_valveStatus;
}

/* ─── CommandSignals ──────────────────────────────────────────────────────── */

/**
 * @brief  Write the CommandSignals shared signal buffer.
 * @param  value  Packed uint8 command word.
 *                Bit 7 is masked to 0 by CmdSigMgr before use.
 */
void Rte_Buffers_Set_CommandSignals(Rte_BT_CommandSignals_t value)
{
    s_commandSignals = value;
}

/**
 * @brief  Read the CommandSignals shared signal buffer.
 * @return Current packed uint8 command word as last written.
 */
Rte_BT_CommandSignals_t Rte_Buffers_Get_CommandSignals(void)
{
    return s_commandSignals;
}