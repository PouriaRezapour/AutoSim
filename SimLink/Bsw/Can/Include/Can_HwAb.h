/**
 * @file    Can_HwAb.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   CAN Hardware Abstraction layer – Tx/Rx descriptors, data buffers,
 *          and filter configuration for Classic CAN on STM32H7 FDCAN.
 *
 * @details
 * This header decouples the logical CAN driver (Can.c) from the STM32 HAL
 * structures.  It defines:
 *   - Thin wrapper types over FDCAN_TxHeaderTypeDef / FDCAN_RxHeaderTypeDef
 *     so upper layers never include stm32h7xx_hal_fdcan.h directly.
 *   - A filter descriptor type (CanFilterConfig_t) that maps to
 *     FDCAN_FilterTypeDef.
 *   - Extern declarations for the shared Tx/Rx data buffers that live in
 *     Can_HwAb.c.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * @note   Can_HwAb_BuildTxHeader was removed in v0.2.2.  Tx header
 *         construction is now handled by the private function
 *         Can_BuildTxHeader_() inside Can.c, which is the only caller.
 *
 * @par Revision History:
 * |---------|------------|------------|---------------------------------------|
 * | Version | Date       | Author     | Description                           |
 * |---------|------------|------------|---------------------------------------|
 * | 0.2.0   | 2025-05-23 | A.Rezapour | Initial release                       |
 * | 0.2.2   | 2025-06-01 | A.Rezapour | Remove Can_HwAb_BuildTxHeader;        |
 * |         |            |            | moved to private Can_BuildTxHeader_   |
 * |         |            |            | in Can.c                              |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | Integrate Std_Types.h; add Module     |
 * |         |            |            | Info and Version Info sections        |
 * |---------|------------|------------|---------------------------------------|
 */

#ifndef CAN_HWAB_H
#define CAN_HWAB_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Mcal.h"

#include <stdint.h>
#include <stddef.h>

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define CAN_HWAB_VENDOR_ID          0xFFFFu

/** AUTOSAR module identifier for the CAN driver (shared with Can.h) */
#define CAN_HWAB_MODULE_ID          80u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define CAN_HWAB_SW_MAJOR_VERSION   0x00u

/** Software minor version number */
#define CAN_HWAB_SW_MINOR_VERSION   0x02u

/** Software patch version number */
#define CAN_HWAB_SW_PATCH_VERSION   0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/** Maximum payload bytes for a Classic CAN frame */
#define CAN_HWAB_MAX_DATA_BYTES     8U

/** Number of standard-ID reception filters to allocate in Message RAM */
#define CAN_HWAB_STD_FILTERS_NB     1U

/** Number of dedicated Rx buffer elements */
#define CAN_HWAB_RX_BUFFERS_NB      1U

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief  Tx frame descriptor (thin wrapper over FDCAN_TxHeaderTypeDef).
 */
typedef FDCAN_TxHeaderTypeDef CanTxHeader_t;

/**
 * @brief  Rx frame descriptor (thin wrapper over FDCAN_RxHeaderTypeDef).
 */
typedef FDCAN_RxHeaderTypeDef CanRxHeader_t;

/**
 * @brief  Reception filter configuration.
 *
 * @details
 * Maps directly to FDCAN_FilterTypeDef.  When filterConfig is set to
 * FDCAN_FILTER_TO_RXBUFFER the filterId2 field is ignored by the HAL.
 */
typedef struct {
    uint32 idType;          /**< FDCAN_STANDARD_ID or FDCAN_EXTENDED_ID         */
    uint32 filterIndex;     /**< Filter element index in Message RAM (0-based)   */
    uint32 filterConfig;    /**< e.g. FDCAN_FILTER_TO_RXBUFFER                   */
    uint32 filterId1;       /**< Primary ID to match (or range start)            */
    uint32 filterId2;       /**< Secondary ID / mask (ignored for RxBuffer mode) */
    uint32 rxBufferIndex;   /**< Dedicated Rx buffer index to store matched msg  */
} CanFilterConfig_t;

/* ─── Extern Data Buffers ─────────────────────────────────────────────────── */

/**
 * @brief  Raw byte array written before each transmission.
 *         Populate before calling Can_Transmit().
 */
extern uint8 g_canTxDataBuffer[CAN_HWAB_MAX_DATA_BYTES];

/**
 * @brief  Raw byte array filled by Can_Receive() upon successful reception.
 */
extern uint8 g_canRxDataBuffer[CAN_HWAB_MAX_DATA_BYTES];

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Build and return a default reception filter aimed at Rx buffer 0.
 * @param  peerId       Standard ID of the remote node whose frames to accept.
 * @return Populated CanFilterConfig_t ready to pass to Can_HwAb_ApplyFilter().
 */
CanFilterConfig_t Can_HwAb_BuildRxFilter(uint32 peerId);

/**
 * @brief  Apply a reception filter to the FDCAN peripheral.
 * @param  pHfdcan      Pointer to the FDCAN handle.
 * @param  pFilterCfg   Pointer to a populated CanFilterConfig_t.
 * @return HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef Can_HwAb_ApplyFilter(FDCAN_HandleTypeDef *pHfdcan,
                                        const CanFilterConfig_t *pFilterCfg);

#endif /* CAN_HWAB_H */
