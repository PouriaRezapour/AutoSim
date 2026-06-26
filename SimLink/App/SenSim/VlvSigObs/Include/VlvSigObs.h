/**
 * @file    VlvSigObs.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR SWC Interface Header: VlvSigObs
 *
 * @details
 * Public interface for the Valve Signal Observer software component.
 *
 * The VlvSigObs SWC is responsible for:
 *   - Reading 14 individual DIO input pins via the RpValveStatus require port
 *   - Packing the 14 boolean pin states into a single uint16 status word
 *   - Explicitly clearing the two reserved bits (14-15) before output
 *   - Writing the packed status word to the PpValveStatusPacked provide port,
 *     which stores it in Rte_Buffers for downstream consumer SWCs
 *
 * Require Ports:
 *   RpValveStatus — MotorStatus  (boolean)  HWAB_DIO_MOTOR_STATUS pin state
 *                    ValveOn      (boolean)  HWAB_DIO_SIGNAL_VALVE_ON pin state
 *                    Valve1       (boolean)  HWAB_DIO_SIGNAL_VALVE1 pin state
 *                    Valve2       (boolean)  HWAB_DIO_SIGNAL_VALVE2 pin state
 *                    Valve3       (boolean)  HWAB_DIO_SIGNAL_VALVE3 pin state
 *                    Valve4       (boolean)  HWAB_DIO_SIGNAL_VALVE4 pin state
 *                    Valve5       (boolean)  HWAB_DIO_SIGNAL_VALVE5 pin state
 *                    Valve6       (boolean)  HWAB_DIO_SIGNAL_VALVE6 pin state
 *                    Valve7       (boolean)  HWAB_DIO_SIGNAL_VALVE7 pin state
 *                    Valve8       (boolean)  HWAB_DIO_SIGNAL_VALVE8 pin state
 *                    Valve9       (boolean)  HWAB_DIO_SIGNAL_VALVE9 pin state
 *                    Valve10      (boolean)  HWAB_DIO_SIGNAL_VALVE10 pin state
 *                    Valve11      (boolean)  HWAB_DIO_SIGNAL_VALVE11 pin state
 *                    Valve12      (boolean)  HWAB_DIO_SIGNAL_VALVE12 pin state
 *
 *                    All RpValveStatus signals target a Dio pin through the
 *                    BSW (not another SWC) and are therefore obtained via
 *                    Rte_Call, not Rte_Read.
 *
 * Provide Ports:
 *   PpValveStatusPacked — Packed       (uint16)   Packed valve/motor status word,
 *                    written via Rte_Write (genuine SWC-to-SWC communication)
 *
 * Bit mapping of the packed Pp status word:
 *   Bit  0     — ValveOn  (HWAB_DIO_SIGNAL_VALVE_ON)
 *   Bit  1     — Valve1   (HWAB_DIO_SIGNAL_VALVE1)
 *   Bit  2     — Valve2   (HWAB_DIO_SIGNAL_VALVE2)
 *   Bit  3     — Valve3   (HWAB_DIO_SIGNAL_VALVE3)
 *   Bit  4     — Valve4   (HWAB_DIO_SIGNAL_VALVE4)
 *   Bit  5     — Valve5   (HWAB_DIO_SIGNAL_VALVE5)
 *   Bit  6     — Valve6   (HWAB_DIO_SIGNAL_VALVE6)
 *   Bit  7     — Valve7   (HWAB_DIO_SIGNAL_VALVE7)
 *   Bit  8     — Valve8   (HWAB_DIO_SIGNAL_VALVE8)
 *   Bit  9     — Valve9   (HWAB_DIO_SIGNAL_VALVE9)
 *   Bit  10    — Valve10  (HWAB_DIO_SIGNAL_VALVE10)
 *   Bit  11    — Valve11  (HWAB_DIO_SIGNAL_VALVE11)
 *   Bit  12    — Valve12  (HWAB_DIO_SIGNAL_VALVE12)
 *   Bit  13    — MotorStatus (HWAB_DIO_MOTOR_STATUS)
 *   Bits 14-15 — Reserved, explicitly cleared to 0 before output
 *
 * Permitted includes: VlvSigObs.h, Rte_VlvSigObs.h
 * Forbidden:          DioIf.h, Eth_*.h, Mcal.h, HAL_*.h, Rte_Buffers.h
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef VLVSIGOBS_H
#define VLVSIGOBS_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_VlvSigObs.h"

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define VLVSIGOBS_SW_MAJOR_VERSION   1u
#define VLVSIGOBS_SW_MINOR_VERSION   0u
#define VLVSIGOBS_SW_PATCH_VERSION   0u

/* ─── Status Codes ────────────────────────────────────────────────────────── */

#define VLVSIGOBS_STATUS_OK          (0x00u)
#define VLVSIGOBS_STATUS_DEGRADED    (0x01u)
#define VLVSIGOBS_STATUS_FAULT       (0x02u)

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/** @brief  Input snapshot collected each runnable cycle. */
typedef struct
{
    /* --- RpValveStatus --- */
    Rte_DT_RpValveStatus_MotorStatus_t MotorStatus;  /**< HWAB_DIO_MOTOR_STATUS       */
    Rte_DT_RpValveStatus_ValveOn_t     ValveOn;      /**< HWAB_DIO_SIGNAL_VALVE_ON    */
    Rte_DT_RpValveStatus_Valve1_t      Valve1;       /**< HWAB_DIO_SIGNAL_VALVE1      */
    Rte_DT_RpValveStatus_Valve2_t      Valve2;       /**< HWAB_DIO_SIGNAL_VALVE2      */
    Rte_DT_RpValveStatus_Valve3_t      Valve3;       /**< HWAB_DIO_SIGNAL_VALVE3      */
    Rte_DT_RpValveStatus_Valve4_t      Valve4;       /**< HWAB_DIO_SIGNAL_VALVE4      */
    Rte_DT_RpValveStatus_Valve5_t      Valve5;       /**< HWAB_DIO_SIGNAL_VALVE5      */
    Rte_DT_RpValveStatus_Valve6_t      Valve6;       /**< HWAB_DIO_SIGNAL_VALVE6      */
    Rte_DT_RpValveStatus_Valve7_t      Valve7;       /**< HWAB_DIO_SIGNAL_VALVE7      */
    Rte_DT_RpValveStatus_Valve8_t      Valve8;       /**< HWAB_DIO_SIGNAL_VALVE8      */
    Rte_DT_RpValveStatus_Valve9_t      Valve9;       /**< HWAB_DIO_SIGNAL_VALVE9      */
    Rte_DT_RpValveStatus_Valve10_t     Valve10;      /**< HWAB_DIO_SIGNAL_VALVE10     */
    Rte_DT_RpValveStatus_Valve11_t     Valve11;      /**< HWAB_DIO_SIGNAL_VALVE11     */
    Rte_DT_RpValveStatus_Valve12_t     Valve12;      /**< HWAB_DIO_SIGNAL_VALVE12     */
} VlvSigObs_InputData_t;

/** @brief  Output snapshot written to provide ports each runnable cycle. */
typedef struct
{
    /* --- PpValveStatusPacked --- */
    Rte_DT_PpValveStatusPacked_Packed_t Packed;  /**< Packed valve/motor status (bits 14-15 = 0) */
} VlvSigObs_OutputData_t;

/* ─── Public Function Prototypes (Runnables) ─────────────────────────────── */

/**
 * @brief  VlvSigObs init runnable — called once at ECU startup.
 * @details
 * Zeroes all local structures and writes the safe default (all status bits
 * inactive / 0x0000) to the PpValveStatusPacked provide port.
 */
void VlvSigObs_Init(void);

/**
 * @brief  VlvSigObs 50 ms cyclic runnable — valve/motor status observation.
 * @details
 * Reads all 14 DIO input signals, packs them into a uint16, clears bits
 * 14-15, and writes the result to the PpValveStatusPacked provide port.
 */
void VlvSigObs_Run_50ms(void);

#endif /* VLVSIGOBS_H */
