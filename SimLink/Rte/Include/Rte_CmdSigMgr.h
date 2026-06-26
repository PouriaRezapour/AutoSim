/**
 * @file    Rte_CmdSigMgr.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR RTE Interface Header for SWC: CmdSigMgr
 *
 * @details
 * Single consolidated RTE header for the CmdSigMgr software component.
 *
 * Port naming follows the AUTOSAR convention: no underscore between the
 * Pp/Rp prefix and the port name. The Rp and Pp ports share the same base
 * name "CommandSignals", differentiated only by the Pp/Rp prefix:
 *   RpCommandSignalsPacked — single packed uint8 command word
 *   PpCommandSignals — unpacked individual boolean command signals
 *
 * Require Ports:
 *   RpCommandSignalsPacked — Packed (uint8) Packed command bit field, sourced
 *                            from Rte_Buffers (Rte_Buffers_Get_CommandSignals).
 *                            This is genuine SWC-to-SWC communication and is
 *                            obtained via Rte_Read.
 *
 * Provide Ports:
 *   PpCommandSignals — Vesc, Bls, Itpms, ParkBrake, EscTcs, Wakeup, Vmot
 *                       (all boolean) — each drives the matching DIO
 *                       output channel via DioIf_Write(). Each signal
 *                       targets a Dio pin through the BSW (not another
 *                       SWC) and is therefore written via Rte_Call, not
 *                       Rte_Write.
 *
 * Cross-SWC access
 * ─────────────────
 *   Rte_CmdSigMgr.c reads the packed command word via
 *   Rte_Buffers_Get_CommandSignals() from Rte_Buffers. It does not include
 *   any other SWC's RTE header.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_CMDSIGMGR_H
#define RTE_CMDSIGMGR_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define RTE_CMDSIGMGR_VENDOR_ID               0xFFFFu

/** AUTOSAR module identifier */
#define RTE_CMDSIGMGR_MODULE_ID               5u

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define RTE_CMDSIGMGR_SW_MAJOR_VERSION        1u
#define RTE_CMDSIGMGR_SW_MINOR_VERSION        0u
#define RTE_CMDSIGMGR_SW_PATCH_VERSION        0u

/*==============================================================================
 *  DATA TYPES  (signal-level types used by the CmdSigMgr SWC)
 *============================================================================*/

/* --- RpCommandSignalsPacked signals ---------------------------------------------- */

/**
 * @brief  Packed command bit field.
 *  Bit 0 — Vesc, Bit 1 — Bls, Bit 2 — Itpms, Bit 3 — ParkBrake,
 *  Bit 4 — EscTcs, Bit 5 — Wakeup, Bit 6 — Vmot, Bit 7 — Reserved (masked).
 */
typedef uint8   Rte_DT_RpCommandSignalsPacked_Packed_t;

/* --- PpCommandSignals signals ----------------------------------------------- */

typedef boolean Rte_DT_PpCommandSignals_Vesc_t;
typedef boolean Rte_DT_PpCommandSignals_Bls_t;
typedef boolean Rte_DT_PpCommandSignals_Itpms_t;
typedef boolean Rte_DT_PpCommandSignals_ParkBrake_t;
typedef boolean Rte_DT_PpCommandSignals_EscTcs_t;
typedef boolean Rte_DT_PpCommandSignals_Wakeup_t;
typedef boolean Rte_DT_PpCommandSignals_Vmot_t;

/*==============================================================================
 *  REQUIRE PORT MACROS  — Rte_Read_<Port>_<Signal>()
 *============================================================================*/

/**
 * @brief  Read the packed command word from the RpCommandSignalsPacked require port.
 * @param  data  Pointer to receive the uint8 value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpCommandSignalsPacked_Packed(data) \
    Rte_Read_CmdSigMgr_RpCommandSignalsPacked_Packed(data)

/*==============================================================================
 *  PROVIDE PORT MACROS  — Rte_Call_<Port>_<Signal>()
 *  (BSW-bound: these target Dio pins through the BSW, not another SWC,
 *  so they use Rte_Call rather than Rte_Write.)
 *============================================================================*/

/**
 * @brief  Call (write) the Vesc command state to the PpCommandSignals provide port.
 * @param  data  Boolean command state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Call_PpCommandSignals_Vesc(data) \
    Rte_Call_CmdSigMgr_PpCommandSignals_Vesc(data)

/**
 * @brief  Call (write) the Bls command state to the PpCommandSignals provide port.
 * @param  data  Boolean command state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Call_PpCommandSignals_Bls(data) \
    Rte_Call_CmdSigMgr_PpCommandSignals_Bls(data)

/**
 * @brief  Call (write) the Itpms command state to the PpCommandSignals provide port.
 * @param  data  Boolean command state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Call_PpCommandSignals_Itpms(data) \
    Rte_Call_CmdSigMgr_PpCommandSignals_Itpms(data)

/**
 * @brief  Call (write) the ParkBrake command state to the PpCommandSignals provide port.
 * @param  data  Boolean command state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Call_PpCommandSignals_ParkBrake(data) \
    Rte_Call_CmdSigMgr_PpCommandSignals_ParkBrake(data)

/**
 * @brief  Call (write) the EscTcs command state to the PpCommandSignals provide port.
 * @param  data  Boolean command state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Call_PpCommandSignals_EscTcs(data) \
    Rte_Call_CmdSigMgr_PpCommandSignals_EscTcs(data)

/**
 * @brief  Call (write) the Wakeup command state to the PpCommandSignals provide port.
 * @param  data  Boolean command state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Call_PpCommandSignals_Wakeup(data) \
    Rte_Call_CmdSigMgr_PpCommandSignals_Wakeup(data)

/**
 * @brief  Call (write) the Vmot command state to the PpCommandSignals provide port.
 * @param  data  Boolean command state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Call_PpCommandSignals_Vmot(data) \
    Rte_Call_CmdSigMgr_PpCommandSignals_Vmot(data)

/*==============================================================================
 *  INTERNAL FUNCTION DECLARATIONS
 *  Called via the macros above — not called directly by the SWC.
 *============================================================================*/

/* RpCommandSignalsPacked */
extern Std_ReturnType Rte_Read_CmdSigMgr_RpCommandSignalsPacked_Packed(Rte_DT_RpCommandSignalsPacked_Packed_t *data);

/* PpCommandSignals */
extern Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Vesc      (Rte_DT_PpCommandSignals_Vesc_t      data);
extern Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Bls       (Rte_DT_PpCommandSignals_Bls_t       data);
extern Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Itpms     (Rte_DT_PpCommandSignals_Itpms_t     data);
extern Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_ParkBrake (Rte_DT_PpCommandSignals_ParkBrake_t data);
extern Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_EscTcs    (Rte_DT_PpCommandSignals_EscTcs_t    data);
extern Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Wakeup    (Rte_DT_PpCommandSignals_Wakeup_t    data);
extern Std_ReturnType Rte_Call_CmdSigMgr_PpCommandSignals_Vmot      (Rte_DT_PpCommandSignals_Vmot_t      data);

/*==============================================================================
 *  OS / SCHEDULER INTERFACE
 *  The scheduler must not include CmdSigMgr.h directly; all entry points
 *  are exposed here.
 *============================================================================*/

/**
 * @brief  Initialize all CmdSigMgr domain software components.
 * @note   Must be called once at system startup before any CmdSigMgr runnables.
 */
void Rte_CmdSigMgr_Init(void);

/**
 * @brief  50 ms cyclic runnable for CmdSigMgr command signal unpacking.
 * @note   Must be called by the scheduler every 50 milliseconds.
 */
void Rte_Runnable_CmdSigMgr_50ms(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_CMDSIGMGR_H */
