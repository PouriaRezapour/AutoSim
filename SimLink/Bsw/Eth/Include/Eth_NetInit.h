/**
 * @file    Eth_NetInit.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Ethernet network interface initialization.
 *
 * @details
 * Provides initialization function for the lwIP network interface and
 * exposes the global netif structure for application-level access to
 * the Ethernet network stack.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h; add Module    |
 * |         |            |                  | Info and Version Info sections;      |
 * |         |            |                  | fix g__netif typo to g_netif         |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef ETH_NETINIT_H
#define ETH_NETINIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "lwip/netif.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define ETH_NETINIT_VENDOR_ID           0xFFFFu

/** AUTOSAR module identifier for the Ethernet driver */
#define ETH_NETINIT_MODULE_ID           88u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define ETH_NETINIT_SW_MAJOR_VERSION    0x00u

/** Software minor version number */
#define ETH_NETINIT_SW_MINOR_VERSION    0x02u

/** Software patch version number */
#define ETH_NETINIT_SW_PATCH_VERSION    0x03u

/* ─── External Variables ──────────────────────────────────────────────────── */

extern struct netif g_netif;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Configure and initialize the Ethernet network interface.
 */
void ETH_NetifConfig(void);

#ifdef __cplusplus
}
#endif

#endif /* ETH_NETINIT_H */
