/**
 * @file    Rte_Buffers.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-17
 * @version 1.0.0
 * @brief   AUTOSAR RTE Shared Signal Buffers — Implementation
 *
 * @details
 * Single point of truth for all inter-SWC signal storage. This module owns
 * the static variables that cross SWC boundaries and exposes them only
 * through typed accessor functions declared in Rte_Buffers.h.
 *
 * No other module may declare a variable that holds an inter-SWC signal.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 1.0.0   | 2025-06-17 | A.Rezapour       | Initial release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Buffers.h"

/* ─── Private Variables (shared signal storage) ───────────────────────────── */

/**
 * @brief  BtnValue inter-SWC signal buffer.
 *         Written by Rte_Hmi; read by Rte_PcComMgr.
 */
static Rte_BT_BtnValue_t s_btnValue = 0.0f;

/* ─── Public Function Implementations ────────────────────────────────────── */

/**
 * @brief  Initialise all shared signal buffers to their safe defaults.
 */
void Rte_Buffers_Init(void)
{
    s_btnValue = 0.0f;
}

/**
 * @brief  Write the BtnValue shared signal buffer.
 * @param  value  New accumulated button value from the Hmi SWC.
 */
void Rte_Buffers_Set_BtnValue(Rte_BT_BtnValue_t value)
{
    s_btnValue = value;
}

/**
 * @brief  Read the BtnValue shared signal buffer.
 * @return Current accumulated button value.
 */
Rte_BT_BtnValue_t Rte_Buffers_Get_BtnValue(void)
{
    return s_btnValue;
}
