/**
 * @file    Rte_Hmi.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-17
 * @version 1.0.0
 * @brief   AUTOSAR RTE Implementation for SWC: Hmi
 *
 * @details
 * Single consolidated RTE implementation for the Hmi software component.
 *
 * This is the only file in the Hmi domain permitted to include Dio_HwAb.h;
 * the Hmi SWC (Hmi.c) must not include hardware abstraction headers directly.
 *
 * Signal storage
 * ──────────────
 *   PpBtnValue :: BtnValue — stored in Rte_Buffers (Rte_Buffers_Set/Get_BtnValue).
 *                            Rte_Hmi.c writes via Rte_Buffers_Set_BtnValue().
 *                            Rte_PcComMgr.c reads via Rte_Buffers_Get_BtnValue().
 *                            Neither RTE module includes the other's RTE header.
 *
 * Port naming
 * ───────────
 *   Ports follow the convention: no underscore after Pp/Rp prefix.
 *   RpDio, PpBtnValue — not Rp_Dio, Pp_PcCom.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 1.0.0   | 2025-06-17 | A.Rezapour       | Initial consolidated release         |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"
#include "Rte_Hmi.h"
#include "Rte_Buffers.h"    /* Shared inter-SWC signal storage              */
#include "Hmi.h"
#include "Dio_HwAb.h"       /* Only Rte_Hmi.c is allowed to include this    */

/* ─── Require Port Implementations (RpDio) ───────────────────────────────── */

/**
 * @brief  Read UsrBtn state from the DIO hardware abstraction layer.
 * @param  data  Pointer to receive the boolean button state.
 * @return RTE_E_OK always (Dio_HwAb_Read does not signal errors here).
 */
Std_ReturnType Rte_Read_Hmi_RpDio_UsrBtn(Rte_DT_RpDio_UsrBtn_t *data)
{
    *data = Dio_HwAb_Read(HWAB_DIO_BLUE_USER_BUTTON);
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
