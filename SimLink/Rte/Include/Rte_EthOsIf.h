/**
 * @file    Rte_Eth.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   RTE-OS interface for Ethernet domain initialization and runnables.
 *
 * @details
 * This header provides the OS/scheduler interface to the Ethernet domain
 * software components. It exposes initialization and runnable entry points
 * that the scheduler can invoke without directly including Ethernet SWC headers.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * The scheduler must not include lwip, ETH_NetInit.h, Eth_UdpServer.h,
 * PcComMgr.h, or any Ethernet SWC headers directly; all access to Ethernet
 * functionality is routed through this RTE interface layer.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h; add Module    |
 * |         |            |                  | Info and Version Info sections       |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_ETHOSIF_H
#define RTE_ETHOSIF_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define RTE_ETHOSIF_VENDOR_ID           0xFFFFu

/** AUTOSAR module identifier for the RTE module */
#define RTE_ETHOSIF_MODULE_ID           2u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define RTE_ETHOSIF_SW_MAJOR_VERSION    0x00u

/** Software minor version number */
#define RTE_ETHOSIF_SW_MINOR_VERSION    0x02u

/** Software patch version number */
#define RTE_ETHOSIF_SW_PATCH_VERSION    0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* None */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/* None */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize all Ethernet domain software components.
 * @note   Must be called once at system startup for Ethernet SWCs requiring
 *         startup configuration.
 */
void Rte_Eth_Init(void);

/**
 * @brief  Ethernet initialization runnable.
 * @note   Initializes LwIP stack, network interface, UDP server, and PcComMgr.
 *         This runnable terminates the calling task after initialization.
 */
void Rte_Runnable_Eth(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_ETHOSIF_H */
