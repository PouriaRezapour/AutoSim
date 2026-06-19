/**
 * @file    Eth_Status.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Ethernet link and DHCP status management module.
 *
 * @details
 * This module manages Ethernet link status monitoring and DHCP client
 * state machine. It provides callback functions for link status updates
 * and a dedicated RTOS thread for DHCP address acquisition and renewal.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * DHCP states:
 * - DHCP_OFF: DHCP disabled
 * - DHCP_START: DHCP initialization
 * - DHCP_WAIT_ADDRESS: Waiting for IP address assignment
 * - DHCP_ADDRESS_ASSIGNED: IP address successfully assigned
 * - DHCP_TIMEOUT: DHCP request timeout
 * - DHCP_LINK_DOWN: Physical link is down
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

#ifndef ETH_STATUS_H
#define ETH_STATUS_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "lwip/netif.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define ETH_STATUS_VENDOR_ID            0xFFFFu

/** AUTOSAR module identifier for the Ethernet driver */
#define ETH_STATUS_MODULE_ID            88u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define ETH_STATUS_SW_MAJOR_VERSION     0x00u

/** Software minor version number */
#define ETH_STATUS_SW_MINOR_VERSION     0x02u

/** Software patch version number */
#define ETH_STATUS_SW_PATCH_VERSION     0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

#define DHCP_OFF                (uint8) 0
#define DHCP_START              (uint8) 1
#define DHCP_WAIT_ADDRESS       (uint8) 2
#define DHCP_ADDRESS_ASSIGNED   (uint8) 3
#define DHCP_TIMEOUT            (uint8) 4
#define DHCP_LINK_DOWN          (uint8) 5

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Callback function for Ethernet link status updates.
 * @param  netif  Pointer to the network interface structure.
 */
void Eth_LinkStatusUpdated(struct netif *netif);

#if LWIP_DHCP
/**
 * @brief  DHCP client thread function.
 * @param  argument  Thread argument (unused).
 * @note   This function runs as an RTOS thread when DHCP is enabled.
 */
void DHCP_Thread(void const *argument);
#endif

#endif /* ETH_STATUS_H */
