/**
 * @file    Can_HwAb.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Implementation of the CAN Hardware Abstraction layer.
 *
 * @details
 * Owns the global Tx/Rx data buffers and provides helper functions that
 * build default HAL descriptor structs for reception filters and apply them
 * to the FDCAN peripheral.  The Can.c driver calls these helpers so that
 * HAL-specific field assignments are not scattered through higher-level code.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * @note   Can_HwAb_BuildTxHeader was removed in v0.2.2.  Tx header
 *         construction is now handled by the private function
 *         Can_BuildTxHeader_() inside Can.c, which is the only caller.
 *
 * Default frame format assumptions used when building headers (all
 * overridable by the caller):
 *   - Standard 11-bit identifier
 *   - Classic CAN frame (no Bit-Rate Switch, no FD format)
 *   - 8-byte data length
 *   - No Tx event FIFO entry
 *   - Error State Indicator = Active
 *
 * @par Revision History:
 * |---------|------------|------------|---------------------------------------|
 * | Version | Date       | Author     | Description                           |
 * |---------|------------|------------|---------------------------------------|
 * | 0.2.0   | 2025-05-23 | A.Rezapour | Initial release                       |
 * | 0.2.2   | 2025-06-01 | A.Rezapour | Remove Can_HwAb_BuildTxHeader;        |
 * |         |            |            | moved to private Can_BuildTxHeader_   |
 * |         |            |            | in Can.c                              |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | Integrate Std_Types.h                 |
 * |---------|------------|------------|---------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Can_HwAb.h"
#include "Std_Types.h"

#include <string.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define LOG(fmt, ...)  /* Swap for your project logger if available */

/* ─── Global Variables (declared extern in Can_HwAb.h) ───────────────────── */

uint8 g_canTxDataBuffer[CAN_HWAB_MAX_DATA_BYTES] = {0};
uint8 g_canRxDataBuffer[CAN_HWAB_MAX_DATA_BYTES] = {0};

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Build and return a default reception filter aimed at Rx buffer 0.
 */
CanFilterConfig_t Can_HwAb_BuildRxFilter(uint32 peerId)
{
    CanFilterConfig_t filterCfg;
    memset(&filterCfg, 0, sizeof(filterCfg));

    filterCfg.idType        = FDCAN_STANDARD_ID;
    filterCfg.filterIndex   = 0U;
    filterCfg.filterConfig  = FDCAN_FILTER_TO_RXBUFFER;
    filterCfg.filterId1     = peerId;    /* Accept frames whose ID == peerId  */
    filterCfg.filterId2     = 0x0U;     /* Ignored in FDCAN_FILTER_TO_RXBUFFER mode */
    filterCfg.rxBufferIndex = 0U;       /* Store matched frames in Rx buffer 0 */

    return filterCfg;
}

/**
 * @brief  Apply a reception filter to the FDCAN peripheral.
 */
HAL_StatusTypeDef Can_HwAb_ApplyFilter(FDCAN_HandleTypeDef *pHfdcan,
                                        const CanFilterConfig_t *pFilterCfg)
{
    if ((pHfdcan == NULL_PTR) || (pFilterCfg == NULL_PTR))
    {
        return HAL_ERROR;
    }

    FDCAN_FilterTypeDef halFilter;
    memset(&halFilter, 0, sizeof(halFilter));

    halFilter.IdType        = pFilterCfg->idType;
    halFilter.FilterIndex   = pFilterCfg->filterIndex;
    halFilter.FilterConfig  = pFilterCfg->filterConfig;
    halFilter.FilterID1     = pFilterCfg->filterId1;
    halFilter.FilterID2     = pFilterCfg->filterId2;
    halFilter.RxBufferIndex = pFilterCfg->rxBufferIndex;

    return HAL_FDCAN_ConfigFilter(pHfdcan, &halFilter);
}
