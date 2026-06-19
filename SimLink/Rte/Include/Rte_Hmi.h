/**
 * @file    Rte_Hmi.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-17
 * @version 1.0.0
 * @brief   AUTOSAR RTE Interface Header for SWC: Hmi
 *
 * @details
 * Single consolidated RTE header for the Hmi software component.
 *
 * Port naming follows the AUTOSAR convention: no underscore between the
 * Pp/Rp prefix and the port name (e.g. RpDio, PpBtnValue).
 *
 * Require Ports:
 *   RpDio     — UsrBtn    (boolean)   Digital input from blue user button
 *
 * Provide Ports:
 *   PpBtnValue — BtnValue (float)     Accumulated button value for PcComMgr
 *
 * Design constraints
 * ──────────────────
 *   • A Provide Port (Pp) must only expose write operations.
 *     Reading back a Pp signal inside the same SWC is forbidden; the Hmi
 *     SWC maintains the accumulated value as an internal SWC variable instead.
 *   • The shared signal storage for PpBtnValue is owned by Rte_Buffers.
 *     Rte_Hmi.c writes it via Rte_Buffers_Set_BtnValue(); Rte_PcComMgr.c
 *     reads it via Rte_Buffers_Get_BtnValue(). Neither includes the other's
 *     RTE header.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 1.0.0   | 2025-06-17 | A.Rezapour       | Initial consolidated release         |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_HMI_H
#define RTE_HMI_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define RTE_HMI_VENDOR_ID               0xFFFFu

/** AUTOSAR module identifier */
#define RTE_HMI_MODULE_ID               2u

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define RTE_HMI_SW_MAJOR_VERSION        1u
#define RTE_HMI_SW_MINOR_VERSION        0u
#define RTE_HMI_SW_PATCH_VERSION        0u

/*==============================================================================
 *  DATA TYPES  (signal-level types used by the Hmi SWC)
 *============================================================================*/

/* --- RpDio signals -------------------------------------------------------- */

/** @brief  Blue user button raw digital state */
typedef boolean Rte_DT_RpDio_UsrBtn_t;

/* --- PpBtnValue signals --------------------------------------------------- */

/** @brief  Accumulated button press value (incremented by 0.5 per press) */
typedef float Rte_DT_PpBtnValue_BtnValue_t;

/*==============================================================================
 *  REQUIRE PORT MACROS  — Rte_Read_<Port>_<Signal>()
 *============================================================================*/

/**
 * @brief  Read UsrBtn digital state from the RpDio require port.
 * @param  data  Pointer to receive the boolean button state.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpDio_UsrBtn(data) \
    Rte_Read_Hmi_RpDio_UsrBtn(data)

/*==============================================================================
 *  PROVIDE PORT MACROS  — Rte_Write_<Port>_<Signal>()
 *
 *  NOTE: A Provide Port exposes WRITE operations only.
 *        There is intentionally no Rte_Read_PpBtnValue_BtnValue macro here.
 *        The Hmi SWC tracks the accumulated value in an internal variable.
 *============================================================================*/

/**
 * @brief  Write accumulated BtnValue to the PpBtnValue provide port.
 * @param  data  Float value to write.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Write_PpBtnValue_BtnValue(data) \
    Rte_Write_Hmi_PpBtnValue_BtnValue(data)

/*==============================================================================
 *  INTERNAL FUNCTION DECLARATIONS
 *  Called via the macros above — not called directly by the SWC.
 *============================================================================*/

/* RpDio */
extern Std_ReturnType Rte_Read_Hmi_RpDio_UsrBtn(Rte_DT_RpDio_UsrBtn_t *data);

/* PpBtnValue */
extern Std_ReturnType Rte_Write_Hmi_PpBtnValue_BtnValue(Rte_DT_PpBtnValue_BtnValue_t data);

/*==============================================================================
 *  OS / SCHEDULER INTERFACE
 *  The scheduler must not include Hmi.h directly; all entry points are
 *  exposed here.
 *============================================================================*/

/**
 * @brief  Initialize all Hmi domain software components.
 * @note   Must be called once at system startup before any Hmi runnables.
 */
void Rte_Hmi_Init(void);

/**
 * @brief  10 ms cyclic runnable for Hmi user-button processing.
 * @note   Must be called by the scheduler every 10 milliseconds.
 */
void Rte_Runnable_Hmi_10ms(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_HMI_H */
