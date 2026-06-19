/**
 * @file    Eth_UdpServer.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   UDP server module for Ethernet communication.
 *
 * @details
 * This module provides a UDP server implementation using the LwIP stack.
 * It supports custom packet building via callback functions, allowing
 * flexible data transmission over Ethernet. The module handles both
 * standard server responses and custom packet transmission to arbitrary
 * remote endpoints.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * Key features:
 * - UDP server initialization and management
 * - Configurable packet builder callbacks
 * - Pre-build hook for data preparation
 * - Custom packet transmission to specific IP/port
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

#ifndef ETH_UDPSERVER_H
#define ETH_UDPSERVER_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Mcal.h"
#include "lwip/api.h"
#include "lwip/err.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/ip_addr.h"
#include "string.h"
#include "cmsis_os.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define ETH_UDPSERVER_VENDOR_ID         0xFFFFu

/** AUTOSAR module identifier for the Ethernet driver */
#define ETH_UDPSERVER_MODULE_ID         88u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define ETH_UDPSERVER_SW_MAJOR_VERSION  0x00u

/** Software minor version number */
#define ETH_UDPSERVER_SW_MINOR_VERSION  0x02u

/** Software patch version number */
#define ETH_UDPSERVER_SW_PATCH_VERSION  0x03u

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief Packet builder callback type.
 * @param data     Pointer to buffer where packet data should be written.
 * @param maxSize  Maximum size of the buffer.
 * @return Number of bytes written to the buffer.
 */
typedef uint16 (*UdpPktBldr_t)(uint8 *data, uint16 maxSize);

/**
 * @brief Pre-build hook callback type.
 *        Called before packet building to prepare data.
 */
typedef void (*UdpPreBuild_t)(void);

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialize the UDP server module.
 */
void Eth_UdpServerInit(void);

/**
 * @brief  Set the packet builder callback.
 * @param  builder  Function pointer to packet builder callback.
 */
void Eth_SetPktBldr(UdpPktBldr_t builder);

/**
 * @brief  Set the pre-build hook callback.
 * @param  hook  Function pointer to pre-build hook.
 */
void Eth_SetPreBuild(UdpPreBuild_t hook);

/**
 * @brief  Send data via UDP using the configured packet builder.
 * @param  data  Pointer to data buffer to send.
 * @return LwIP error code (ERR_OK on success).
 */
err_t Eth_UdpSendData(const uint8 *data);

/**
 * @brief  Send a custom UDP packet to a specific remote endpoint.
 * @param  data        Pointer to data buffer to send.
 * @param  len         Length of data in bytes.
 * @param  remote_ip   Remote IP address as string (e.g., "192.168.1.100").
 * @param  remote_port Remote UDP port number.
 * @return LwIP error code (ERR_OK on success).
 */
err_t Eth_UdpSendCustomPacket(const char *data, u16_t len,
                               const char *remote_ip, u16_t remote_port);

#endif /* ETH_UDPSERVER_H */
