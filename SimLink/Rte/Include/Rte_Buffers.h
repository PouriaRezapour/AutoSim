/**
 * @file    Rte_Buffers.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-17
 * @version 1.0.0
 * @brief   AUTOSAR RTE Shared Signal Buffers
 *
 * @details
 * Declares and manages all inter-SWC signal buffers shared between two or
 * more RTE modules. This is the ONLY file that owns shared signal storage;
 * neither Rte_Hmi.c nor Rte_PcComMgr.c may declare inter-SWC variables
 * directly.
 *
 * Currently managed signals
 * ─────────────────────────
 *   BtnValue  — Written by Rte_Hmi (Hmi PpBtnValue provide port).
 *               Read by Rte_PcComMgr (PcComMgr RpBtnValue require port).
 *
 * Access rules
 * ─────────────
 *   Write: Rte_Buffers_Set_BtnValue()  — called exclusively from Rte_Hmi.c
 *   Read:  Rte_Buffers_Get_BtnValue()  — called exclusively from Rte_PcComMgr.c
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 1.0.0   | 2025-06-17 | A.Rezapour       | Initial release                      |
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
#define RTE_BUFFERS_SW_MINOR_VERSION    0u
#define RTE_BUFFERS_SW_PATCH_VERSION    0u

/*==============================================================================
 *  SHARED SIGNAL TYPE DEFINITIONS
 *============================================================================*/

/**
 * @brief  Type of the BtnValue inter-SWC signal buffer.
 *         Hmi provides this; PcComMgr requires it.
 */
typedef float Rte_BT_BtnValue_t;

/*==============================================================================
 *  PUBLIC API
 *============================================================================*/

/**
 * @brief  Initialise all shared signal buffers to their safe defaults.
 * @note   Must be called once at system startup before any SWC init.
 */
void Rte_Buffers_Init(void);

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

#ifdef __cplusplus
}
#endif

#endif /* RTE_BUFFERS_H */
