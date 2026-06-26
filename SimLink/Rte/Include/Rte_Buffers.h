/**
 * @file    Rte_Buffers.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR RTE Shared Signal Buffers
 *
 * @details
 * Declares and manages all inter-SWC signal buffers shared between two or
 * more RTE modules. This is the ONLY file that owns shared signal storage;
 * no individual Rte_<Swc>.c file may declare inter-SWC variables directly.
 *
 * Currently managed signals
 * ─────────────────────────
 *   BtnValue        — Written by Rte_Hmi      (Hmi PpBtnValue provide port).
 *                     Read    by Rte_PcComMgr  (PcComMgr RpBtnValue require port).
 *
 *   ValveStatus     — Written by Rte_VlvSigObs (VlvSigObs PpValveStatusPacked provide port).
 *                     Read    by any consumer   via Rte_Buffers_Get_ValveStatus().
 *
 *   CommandSignals  — Written by any producer   via Rte_Buffers_Set_CommandSignals().
 *                     Read    by Rte_CmdSigMgr  (CmdSigMgr RpCommandSignalsPacked require port).
 *
 * Access rules
 * ─────────────
 *   BtnValue
 *     Write : Rte_Buffers_Set_BtnValue()       — called exclusively from Rte_Hmi.c
 *     Read  : Rte_Buffers_Get_BtnValue()       — called exclusively from Rte_PcComMgr.c
 *
 *   ValveStatus
 *     Write : Rte_Buffers_Set_ValveStatus()    — called exclusively from Rte_VlvSigObs.c
 *     Read  : Rte_Buffers_Get_ValveStatus()    — called by any consumer SWC RTE module
 *
 *   CommandSignals
 *     Write : Rte_Buffers_Set_CommandSignals() — called by the command-word producer
 *     Read  : Rte_Buffers_Get_CommandSignals() — called exclusively from Rte_CmdSigMgr.c
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial release (BtnValue only)      |
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Added ValveStatus and CommandSignals |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_BUFFERS_H
#define RTE_BUFFERS_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define RTE_BUFFERS_VENDOR_ID           0xFFFFu
#define RTE_BUFFERS_MODULE_ID           3u

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define RTE_BUFFERS_SW_MAJOR_VERSION    1u
#define RTE_BUFFERS_SW_MINOR_VERSION    1u
#define RTE_BUFFERS_SW_PATCH_VERSION    0u

/*==============================================================================
 *  SHARED SIGNAL TYPE DEFINITIONS
 *============================================================================*/

/**
 * @brief  Type of the BtnValue inter-SWC signal buffer.
 *         Hmi provides this; PcComMgr requires it.
 */
typedef float   Rte_BT_BtnValue_t;

/**
 * @brief  Type of the ValveStatus inter-SWC signal buffer.
 *         VlvSigObs provides this; any consumer may require it.
 *
 *         Bit layout (14 bits used, bits 14-15 always 0):
 *           Bit  0     — HWAB_DIO_SIGNAL_VALVE_ON
 *           Bits 1-12  — HWAB_DIO_SIGNAL_VALVE1 … HWAB_DIO_SIGNAL_VALVE12
 *           Bit  13    — HWAB_DIO_MOTOR_STATUS
 *           Bits 14-15 — Reserved, always 0
 */
typedef uint16  Rte_BT_ValveStatus_t;

/**
 * @brief  Type of the CommandSignals inter-SWC signal buffer.
 *         The command-word producer writes this; CmdSigMgr requires it.
 *
 *         Bit layout (7 bits used, bit 7 always masked to 0):
 *           Bit 0 — HWAB_DIO_CMD_VESC
 *           Bit 1 — HWAB_DIO_CMD_BLS
 *           Bit 2 — HWAB_DIO_CMD_ITPMS
 *           Bit 3 — HWAB_DIO_CMD_PARK_BRAKE
 *           Bit 4 — HWAB_DIO_CMD_ESC_TCS
 *           Bit 5 — HWAB_DIO_CMD_WAKEUP
 *           Bit 6 — HWAB_DIO_CMD_VMOT
 *           Bit 7 — Reserved, masked to 0 before use
 */
typedef uint8   Rte_BT_CommandSignals_t;

/*==============================================================================
 *  PUBLIC API
 *============================================================================*/

/**
 * @brief  Initialise all shared signal buffers to their safe defaults.
 * @note   Must be called once at system startup before any SWC init.
 */
void Rte_Buffers_Init(void);

/* ─── BtnValue ──────────────────────────────────────────────────────────── */

/**
 * @brief  Write the BtnValue shared signal buffer.
 * @param  value  New accumulated button value from the Hmi SWC.
 */
void Rte_Buffers_Set_BtnValue(Rte_BT_BtnValue_t value);

/**
 * @brief  Read the BtnValue shared signal buffer.
 * @return Current accumulated button value.
 */
Rte_BT_BtnValue_t Rte_Buffers_Get_BtnValue(void);

/* ─── ValveStatus ───────────────────────────────────────────────────────── */

/**
 * @brief  Write the ValveStatus shared signal buffer.
 * @param  value  Packed uint16 valve/motor status word from VlvSigObs SWC.
 *                Bits 14-15 must already be cleared by the caller.
 */
void Rte_Buffers_Set_ValveStatus(Rte_BT_ValveStatus_t value);

/**
 * @brief  Read the ValveStatus shared signal buffer.
 * @return Current packed uint16 valve/motor status word.
 *         Bits 14-15 are always 0.
 */
Rte_BT_ValveStatus_t Rte_Buffers_Get_ValveStatus(void);

/* ─── CommandSignals ────────────────────────────────────────────────────── */

/**
 * @brief  Write the CommandSignals shared signal buffer.
 * @param  value  Packed uint8 command word.
 *                Bit 7 is masked to 0 by CmdSigMgr before use.
 */
void Rte_Buffers_Set_CommandSignals(Rte_BT_CommandSignals_t value);

/**
 * @brief  Read the CommandSignals shared signal buffer.
 * @return Current packed uint8 command word as last written.
 */
Rte_BT_CommandSignals_t Rte_Buffers_Get_CommandSignals(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_BUFFERS_H */
