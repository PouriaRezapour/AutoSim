/**
 * @file    Rte_VlvSigObs.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR RTE Implementation for SWC: VlvSigObs
 *
 * @details
 * Single consolidated RTE implementation for the VlvSigObs (Valve Signal
 * Observer) software component.
 *
 * This is the only file in the VlvSigObs domain permitted to include
 * DioIf.h; the VlvSigObs SWC (VlvSigObs.c) must not include hardware
 * abstraction headers directly.
 *
 * Signal storage
 * ──────────────
 *   RpValveStatus :: * — each individual boolean signal targets a Dio pin
 *                        through the BSW (DioIf), not another SWC, and
 *                        is therefore read via Rte_Call, not Rte_Read.
 *
 *   PpValveStatusPacked :: Packed — the assembled uint16 status word is written
 *                             to Rte_Buffers (Rte_Buffers_Set_ValveStatus()).
 *                             Any future consumer SWC reads it via
 *                             Rte_Buffers_Get_ValveStatus(). This module
 *                             does not include any other SWC's RTE header.
 *                             This is genuine SWC-to-SWC communication, so
 *                             it correctly remains an Rte_Write.
 *
 * Port naming
 * ───────────
 *   Ports follow the convention: no underscore after Pp/Rp prefix.
 *   RpValveStatus, PpValveStatusPacked — same base name, Pp/Rp prefix only.
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
#include "Rte_VlvSigObs.h"
#include "Rte_Buffers.h"    /* Shared inter-SWC signal storage                */
#include "VlvSigObs.h"
#include "DioIf.h"       /* Only Rte_VlvSigObs.c is allowed to include this */

/* ─── Require Port Implementations (RpValveStatus) ──────────────────────── */

/**
 * @brief  Call (read) MotorStatus state from the DIO hardware abstraction layer.
 * @param  data  Pointer to receive the boolean pin state.
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_MotorStatus(Rte_DT_RpValveStatus_MotorStatus_t *data)
{
    *data = DioIf_Read(HWAB_DIO_MOTOR_STATUS);
    return RTE_E_OK;
}

/**
 * @brief  Call (read) ValveOn state from the DIO hardware abstraction layer.
 * @param  data  Pointer to receive the boolean pin state.
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_ValveOn(Rte_DT_RpValveStatus_ValveOn_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE_ON);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve1(Rte_DT_RpValveStatus_Valve1_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE1);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve2(Rte_DT_RpValveStatus_Valve2_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE2);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve3(Rte_DT_RpValveStatus_Valve3_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE3);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve4(Rte_DT_RpValveStatus_Valve4_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE4);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve5(Rte_DT_RpValveStatus_Valve5_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE5);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve6(Rte_DT_RpValveStatus_Valve6_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE6);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve7(Rte_DT_RpValveStatus_Valve7_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE7);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve8(Rte_DT_RpValveStatus_Valve8_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE8);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve9(Rte_DT_RpValveStatus_Valve9_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE9);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve10(Rte_DT_RpValveStatus_Valve10_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE10);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve11(Rte_DT_RpValveStatus_Valve11_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE11);
    return RTE_E_OK;
}

Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve12(Rte_DT_RpValveStatus_Valve12_t *data)
{
    *data = DioIf_Read(HWAB_DIO_SIGNAL_VALVE12);
    return RTE_E_OK;
}

/* ─── Provide Port Implementations (PpValveStatusPacked) ──────────────────────── */

/**
 * @brief  Write the packed valve/motor status word to the shared signal buffer.
 * @details
 * Delegates to Rte_Buffers_Set_ValveStatus() — the single authoritative
 * writer for the VlvSigObs→consumer inter-SWC signal. No other SWC's RTE
 * header is included; cross-SWC access is mediated exclusively by
 * Rte_Buffers.
 * @param  data  Packed uint16 status word (bits 14-15 already 0).
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Write_VlvSigObs_PpValveStatusPacked_Packed(Rte_DT_PpValveStatusPacked_Packed_t data)
{
    Rte_Buffers_Set_ValveStatus(data);
    return RTE_E_OK;
}

/* ─── OS / Scheduler Interface ────────────────────────────────────────────── */

/**
 * @brief  Initialize all VlvSigObs domain software components.
 * @details
 * Wraps VlvSigObs_Init(). The scheduler calls this once at startup without
 * needing to include VlvSigObs.h directly.
 */
void Rte_VlvSigObs_Init(void)
{
    VlvSigObs_Init();
}

/**
 * @brief  50 ms cyclic runnable for VlvSigObs valve/motor status observation.
 * @details
 * Wraps VlvSigObs_Run_50ms(). The scheduler calls this every 50 ms without
 * needing to include VlvSigObs.h directly.
 */
void Rte_Runnable_VlvSigObs_50ms(void)
{
    VlvSigObs_Run_50ms();
}
