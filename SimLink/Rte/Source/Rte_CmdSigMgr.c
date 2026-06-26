/**
 * @file    Rte_CmdSigMgr.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR RTE Implementation for SWC: CmdSigMgr
 *
 * @details
 * Single consolidated RTE implementation for the CmdSigMgr software
 * component.
 *
 * This is the only file in the CmdSigMgr domain permitted to include
 * DioIf.h; the CmdSigMgr SWC (CmdSigMgr.c) must not include hardware
 * abstraction headers directly.
 *
 * Signal storage
 * ──────────────
 *   RpCommandSignalsPacked ::    Packed — read from Rte_Buffers
 *                                (Rte_Buffers_Get_CommandSignals()). The
 *                                producer of this buffer is outside the
 *                                scope of this revision; any future SWC
 *                                that decodes an incoming command word
 *                                writes it via
 *                                Rte_Buffers_Set_CommandSignals(). This
 *                                module does not include any other SWC's
 *                                RTE header. This is genuine SWC-to-SWC
 *                                communication, so it correctly remains
 *                                an Rte_Read.
 *   PpCommandSignals  :: *     — each signal is written straight through
 *                                to the matching DioIf output channel.
 *                                Each signal targets a Dio pin through the
 *                                BSW (not another SWC) and is therefore
 *                                written via Rte_Call, not Rte_Write.
 *
 * Port naming
 * ───────────
 *   Ports follow the convention: no underscore after Pp/Rp prefix.
 *   RpCommandSignalsPacked, PpCommandSignals — same base name, Pp/Rp prefix only.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"
#include "Rte_CmdSigMgr.h"
#include "Rte_Buffers.h"    /* Shared inter-SWC signal storage              */
#include "CmdSigMgr.h"
#include "DioIf.h"       /* Only Rte_CmdSigMgr.c is allowed to include this */

/* ─── Require Port Implementations (RpCommandSignalsPacked) ────────────────────── */

/**
 * @brief  Read the packed command word from the shared inter-SWC signal buffer.
 * @details
 * Delegates to Rte_Buffers_Get_CommandSignals() — the single authoritative
 * accessor for the shared CommandSignals signal. No other SWC's RTE header
 * is included; cross-SWC access is mediated exclusively by Rte_Buffers.
 * @param  data  Pointer to receive the packed uint8 command word.
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Read_CmdSigMgr_RpCommandSignalsPacked_Packed(Rte_DT_RpCommandSignalsPacked_Packed_t *data)
{
    *data = Rte_Buffers_Get_CommandSignals();
    return RTE_E_OK;
}

/* ─── Provide Port Implementations (PpCommandSignals) ────────────────────── */

/**
 * @brief  Call (write) the Vesc command state to the DIO hardware abstraction layer.
 * @details
 * PpCommandSignals.Vesc targets a Dio pin through the BSW (DioIf), not
 * another SWC; this is therefore implemented via Rte_Call, not Rte_Write.
 * @param  data  Boolean command state.
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Vesc(Rte_DT_PpCommandSignals_Vesc_t data)
{
    DioIf_Write(HWAB_DIO_CMD_VESC, data);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Bls(Rte_DT_PpCommandSignals_Bls_t data)
{
    DioIf_Write(HWAB_DIO_CMD_BLS, data);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Itpms(Rte_DT_PpCommandSignals_Itpms_t data)
{
    DioIf_Write(HWAB_DIO_CMD_ITPMS, data);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_ParkBrake(Rte_DT_PpCommandSignals_ParkBrake_t data)
{
    DioIf_Write(HWAB_DIO_CMD_PARK_BRAKE, data);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_EscTcs(Rte_DT_PpCommandSignals_EscTcs_t data)
{
    DioIf_Write(HWAB_DIO_CMD_ESC_TCS, data);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Wakeup(Rte_DT_PpCommandSignals_Wakeup_t data)
{
    DioIf_Write(HWAB_DIO_CMD_WAKEUP, data);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Vmot(Rte_DT_PpCommandSignals_Vmot_t data)
{
    DioIf_Write(HWAB_DIO_CMD_VMOT, data);
    return RTE_E_OK;
}

/* ─── OS / Scheduler Interface ────────────────────────────────────────────── */

/**
 * @brief  Initialize all CmdSigMgr domain software components.
 * @details
 * Wraps CmdSigMgr_Init(). The scheduler calls this once at startup without
 * needing to include CmdSigMgr.h directly.
 */
void Rte_CmdSigMgr_Init(void)
{
    CmdSigMgr_Init();
}

/**
 * @brief  50 ms cyclic runnable for CmdSigMgr command signal unpacking.
 * @details
 * Wraps CmdSigMgr_Run_50ms(). The scheduler calls this every 50 ms without
 * needing to include CmdSigMgr.h directly.
 */
void Rte_Runnable_CmdSigMgr_50ms(void)
{
    CmdSigMgr_Run_50ms();
}
