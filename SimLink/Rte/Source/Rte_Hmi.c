/**
 * @file    Rte_Hmi.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR RTE Implementation for SWC: Hmi
 *
 * @details
 * Single consolidated RTE implementation for the Hmi software component.
 *
 * This is the only file in the Hmi domain permitted to include DioIf.h;
 * the Hmi SWC (Hmi.c) must not include hardware abstraction headers directly.
 *
 * Signal storage
 * ──────────────
 *   RpDio      :: UsrBtn   — targets a Dio pin through the BSW (DioIf),
 *                            not another SWC. Exposed via Rte_Call, not
 *                            Rte_Read, so that Rte_Read/Rte_Write remain
 *                            reserved strictly for SWC-to-SWC communication.
 *
 *   PpBtnValue :: BtnValue — stored in Rte_Buffers (Rte_Buffers_Set/Get_BtnValue).
 *                            Rte_Hmi.c writes via Rte_Buffers_Set_BtnValue().
 *                            Rte_PcComMgr.c reads via Rte_Buffers_Get_BtnValue().
 *                            Neither RTE module includes the other's RTE header.
 *                            This is genuine SWC-to-SWC communication, so it
 *                            correctly remains an Rte_Write.
 *
 * Port naming
 * ───────────
 *   Ports follow the convention: no underscore after Pp/Rp prefix.
 *   RpDio, PpBtnValue — not Rp_Dio, Pp_PcCom.
 *
 * @par Revision History:
 * |---------|------------|------------------|---------------------------------------|
 * | Version | Date       | Author           | Description                           |
 * |---------|------------|------------------|---------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial consolidated release          |
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | RpDio.UsrBtn is BSW-bound Dio pin     |
 * |         |            |                  | access, not SWC-to-SWC communication: |
 * |         |            |                  | Rte_Read_Hmi_RpDio_UsrBtn renamed to  |
 * |         |            |                  | Rte_Call_Hmi_RpDio_UsrBtn.            |
 * |---------|------------|------------------|---------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"
#include "Rte_Hmi.h"
#include "Rte_Buffers.h"    /* Shared inter-SWC signal storage              */
#include "Hmi.h"
#include "DioIf.h"       /* Only Rte_Hmi.c is allowed to include this    */

/* ─── Require Port Implementations (RpDio) ───────────────────────────────── */

/**
 * @brief  Call (read) UsrBtn state from the DIO hardware abstraction layer.
 * @details
 * RpDio.UsrBtn targets a Dio pin through the BSW (DioIf), not another
 * SWC; this access is therefore implemented via Rte_Call, not Rte_Read.
 * @param  data  Pointer to receive the boolean button state.
 * @return RTE_E_OK always (DioIf_Read does not signal errors here).
 */
Std_ReturnType Rte_Call_Hmi_RpDio_UsrBtn(Rte_DT_RpDio_UsrBtn_t *data)
{
    *data = DioIf_Read(HWAB_DIO_BLUE_USER_BUTTON);
    return RTE_E_OK;
}

/* ─── Provide Port Implementations (PpBtnValue) ──────────────────────────── */

/**
 * @brief  Write accumulated BtnValue to the shared signal buffer.
 * @details
 * Delegates to Rte_Buffers_Set_BtnValue(). Rte_PcComMgr reads the same
 * buffer via Rte_Buffers_Get_BtnValue() — no cross-RTE-header dependency.
 * @param  data  New button value.
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Write_Hmi_PpBtnValue_BtnValue(Rte_DT_PpBtnValue_BtnValue_t data)
{
    Rte_Buffers_Set_BtnValue(data);
    return RTE_E_OK;
}

/* ─── OS / Scheduler Interface ────────────────────────────────────────────── */

/**
 * @brief  Initialize all Hmi domain software components.
 * @details
 * Wraps Hmi_Init(). The scheduler calls this once at startup without
 * needing to include Hmi.h directly.
 */
void Rte_Hmi_Init(void)
{
    Hmi_Init();
}

/**
 * @brief  10 ms cyclic runnable for Hmi user-button processing.
 * @details
 * Wraps Hmi_Run_10ms(). The scheduler calls this every 10 ms without
 * needing to include Hmi.h directly.
 */
void Rte_Runnable_Hmi_10ms(void)
{
    Hmi_Run_10ms();
}
