/**
 * @file    Rte_VlvSigObs.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR RTE Interface Header for SWC: VlvSigObs
 *
 * @details
 * Single consolidated RTE header for the VlvSigObs (Valve Signal Observer)
 * software component.
 *
 * Port naming follows the AUTOSAR convention: no underscore between the
 * Pp/Rp prefix and the port name. The Rp and Pp ports share the same base
 * name "ValveStatus", differentiated only by the Pp/Rp prefix:
 *   RpValveStatus — individual DIO input signals (motor + valve_on + 12 valves)
 *   PpValveStatusPacked — single packed uint16 status word
 *
 * Require Ports:
 *   RpValveStatus — MotorStatus   (boolean)  HWAB_DIO_MOTOR_STATUS pin state
 *                    ValveOn       (boolean)  HWAB_DIO_SIGNAL_VALVE_ON pin state
 *                    Valve1        (boolean)  HWAB_DIO_SIGNAL_VALVE1 pin state
 *                    Valve2        (boolean)  HWAB_DIO_SIGNAL_VALVE2 pin state
 *                    Valve3        (boolean)  HWAB_DIO_SIGNAL_VALVE3 pin state
 *                    Valve4        (boolean)  HWAB_DIO_SIGNAL_VALVE4 pin state
 *                    Valve5        (boolean)  HWAB_DIO_SIGNAL_VALVE5 pin state
 *                    Valve6        (boolean)  HWAB_DIO_SIGNAL_VALVE6 pin state
 *                    Valve7        (boolean)  HWAB_DIO_SIGNAL_VALVE7 pin state
 *                    Valve8        (boolean)  HWAB_DIO_SIGNAL_VALVE8 pin state
 *                    Valve9        (boolean)  HWAB_DIO_SIGNAL_VALVE9 pin state
 *                    Valve10       (boolean)  HWAB_DIO_SIGNAL_VALVE10 pin state
 *                    Valve11       (boolean)  HWAB_DIO_SIGNAL_VALVE11 pin state
 *                    Valve12       (boolean)  HWAB_DIO_SIGNAL_VALVE12 pin state
 *
 * Provide Ports:
 *   PpValveStatusPacked — Packed        (uint16)   Packed valve/motor status word
 *
 * Packed bit layout:
 *   Bit  0     — ValveOn  (HWAB_DIO_SIGNAL_VALVE_ON)
 *   Bits 1-12  — Valve1 … Valve12
 *   Bit  13    — MotorStatus (HWAB_DIO_MOTOR_STATUS)
 *   Bits 14-15 — Reserved, always 0
 *
 * BSW-bound access vs. SWC-to-SWC communication
 * ───────────────────────────────────────────────
 *   Every RpValveStatus signal targets a Dio pin through the BSW
 *   (DioIf), not another SWC. Per AUTOSAR RTE conventions, Rte_Read /
 *   Rte_Write are reserved for application-layer SWC-to-SWC communication;
 *   BSW-bound pin access must use Rte_Call instead. All 14 RpValveStatus
 *   signals are therefore exposed via Rte_Call_RpValveStatus_<Signal>(),
 *   not Rte_Read_RpValveStatus_<Signal>().
 *
 *   PpValveStatusPacked.Packed is genuine SWC-to-SWC communication (it is
 *   stored in Rte_Buffers for downstream consumer SWCs) and continues to
 *   use Rte_Write.
 *
 * Cross-SWC access
 * ─────────────────
 *   Rte_VlvSigObs.c writes the packed status word via
 *   Rte_Buffers_Set_ValveStatus() from Rte_Buffers. It does not include any
 *   other SWC's RTE header.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_VLVSIGOBS_H
#define RTE_VLVSIGOBS_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define RTE_VLVSIGOBS_VENDOR_ID               0xFFFFu

/** AUTOSAR module identifier */
#define RTE_VLVSIGOBS_MODULE_ID               6u

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define RTE_VLVSIGOBS_SW_MAJOR_VERSION        1u
#define RTE_VLVSIGOBS_SW_MINOR_VERSION        0u
#define RTE_VLVSIGOBS_SW_PATCH_VERSION        0u

/*==============================================================================
 *  DATA TYPES  (signal-level types used by the VlvSigObs SWC)
 *============================================================================*/

/* --- RpValveStatus signals ------------------------------------------------- */

typedef boolean Rte_DT_RpValveStatus_MotorStatus_t;  /**< HWAB_DIO_MOTOR_STATUS       */
typedef boolean Rte_DT_RpValveStatus_ValveOn_t;      /**< HWAB_DIO_SIGNAL_VALVE_ON    */
typedef boolean Rte_DT_RpValveStatus_Valve1_t;       /**< HWAB_DIO_SIGNAL_VALVE1      */
typedef boolean Rte_DT_RpValveStatus_Valve2_t;       /**< HWAB_DIO_SIGNAL_VALVE2      */
typedef boolean Rte_DT_RpValveStatus_Valve3_t;       /**< HWAB_DIO_SIGNAL_VALVE3      */
typedef boolean Rte_DT_RpValveStatus_Valve4_t;       /**< HWAB_DIO_SIGNAL_VALVE4      */
typedef boolean Rte_DT_RpValveStatus_Valve5_t;       /**< HWAB_DIO_SIGNAL_VALVE5      */
typedef boolean Rte_DT_RpValveStatus_Valve6_t;       /**< HWAB_DIO_SIGNAL_VALVE6      */
typedef boolean Rte_DT_RpValveStatus_Valve7_t;       /**< HWAB_DIO_SIGNAL_VALVE7      */
typedef boolean Rte_DT_RpValveStatus_Valve8_t;       /**< HWAB_DIO_SIGNAL_VALVE8      */
typedef boolean Rte_DT_RpValveStatus_Valve9_t;       /**< HWAB_DIO_SIGNAL_VALVE9      */
typedef boolean Rte_DT_RpValveStatus_Valve10_t;      /**< HWAB_DIO_SIGNAL_VALVE10     */
typedef boolean Rte_DT_RpValveStatus_Valve11_t;      /**< HWAB_DIO_SIGNAL_VALVE11     */
typedef boolean Rte_DT_RpValveStatus_Valve12_t;      /**< HWAB_DIO_SIGNAL_VALVE12     */

/* --- PpValveStatusPacked signals ------------------------------------------------- */

/**
 * @brief  Packed valve/motor status word.
 *  Bit  0     — ValveOn, Bits 1-12 — Valve1..12,
 *  Bit  13    — MotorStatus, Bits 14-15 — Reserved (always 0).
 */
typedef uint16  Rte_DT_PpValveStatusPacked_Packed_t;

/*==============================================================================
 *  REQUIRE PORT MACROS  — Rte_Read_<Port>_<Signal>()
 *============================================================================*/

/** @brief  Call (read) MotorStatus pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_MotorStatus(data) \
    Rte_Call_VlvSigObs_RpValveStatus_MotorStatus(data)

/** @brief  Call (read) ValveOn pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_ValveOn(data) \
    Rte_Call_VlvSigObs_RpValveStatus_ValveOn(data)

/** @brief  Call (read) Valve1 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve1(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve1(data)

/** @brief  Call (read) Valve2 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve2(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve2(data)

/** @brief  Call (read) Valve3 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve3(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve3(data)

/** @brief  Call (read) Valve4 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve4(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve4(data)

/** @brief  Call (read) Valve5 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve5(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve5(data)

/** @brief  Call (read) Valve6 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve6(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve6(data)

/** @brief  Call (read) Valve7 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve7(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve7(data)

/** @brief  Call (read) Valve8 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve8(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve8(data)

/** @brief  Call (read) Valve9 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve9(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve9(data)

/** @brief  Call (read) Valve10 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve10(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve10(data)

/** @brief  Call (read) Valve11 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve11(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve11(data)

/** @brief  Call (read) Valve12 pin state from the RpValveStatus require port. */
#define Rte_Call_RpValveStatus_Valve12(data) \
    Rte_Call_VlvSigObs_RpValveStatus_Valve12(data)

/*==============================================================================
 *  PROVIDE PORT MACROS  — Rte_Write_<Port>_<Signal>()
 *============================================================================*/

/**
 * @brief  Write the packed status word to the PpValveStatusPacked provide port.
 * @param  data  uint16 packed valve/motor status (bits 14-15 must be 0).
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Write_PpValveStatusPacked_Packed(data) \
    Rte_Write_VlvSigObs_PpValveStatusPacked_Packed(data)

/*==============================================================================
 *  INTERNAL FUNCTION DECLARATIONS
 *  Called via the macros above — not called directly by the SWC.
 *============================================================================*/

/* RpValveStatus */
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_MotorStatus(Rte_DT_RpValveStatus_MotorStatus_t *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_ValveOn    (Rte_DT_RpValveStatus_ValveOn_t     *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve1     (Rte_DT_RpValveStatus_Valve1_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve2     (Rte_DT_RpValveStatus_Valve2_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve3     (Rte_DT_RpValveStatus_Valve3_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve4     (Rte_DT_RpValveStatus_Valve4_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve5     (Rte_DT_RpValveStatus_Valve5_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve6     (Rte_DT_RpValveStatus_Valve6_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve7     (Rte_DT_RpValveStatus_Valve7_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve8     (Rte_DT_RpValveStatus_Valve8_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve9     (Rte_DT_RpValveStatus_Valve9_t      *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve10    (Rte_DT_RpValveStatus_Valve10_t     *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve11    (Rte_DT_RpValveStatus_Valve11_t     *data);
extern Std_ReturnType Rte_Call_VlvSigObs_RpValveStatus_Valve12    (Rte_DT_RpValveStatus_Valve12_t     *data);

/* PpValveStatusPacked */
extern Std_ReturnType Rte_Write_VlvSigObs_PpValveStatusPacked_Packed(Rte_DT_PpValveStatusPacked_Packed_t data);

/*==============================================================================
 *  OS / SCHEDULER INTERFACE
 *  The scheduler must not include VlvSigObs.h directly; all entry points
 *  are exposed here.
 *============================================================================*/

/**
 * @brief  Initialize all VlvSigObs domain software components.
 * @note   Must be called once at system startup before any VlvSigObs runnables.
 */
void Rte_VlvSigObs_Init(void);

/**
 * @brief  50 ms cyclic runnable for VlvSigObs valve/motor status observation.
 * @note   Must be called by the scheduler every 50 milliseconds.
 */
void Rte_Runnable_VlvSigObs_50ms(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_VLVSIGOBS_H */
