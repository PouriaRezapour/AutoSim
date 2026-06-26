/**
 * @file    Eth_Config.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Ethernet UDP communication configuration.
 *
 * @details
 * This header defines all configuration parameters for Ethernet UDP
 * communication including IP addresses, ports, packet timing, endianness,
 * and application-specific constants for LED control and button handling.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * | 0.1.3   | 2025-05-18 | A.Rezapour       | Removed UDP_SEND_INTERVAL definition |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h; add Module    |
 * |         |            |                  | Info and Version Info sections       |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef ETH_CONFIG_H
#define ETH_CONFIG_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "cmsis_os.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define ETH_CONFIG_VENDOR_ID            0xFFFFu

/** AUTOSAR module identifier for the Ethernet driver */
#define ETH_CONFIG_MODULE_ID            88u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define ETH_CONFIG_SW_MAJOR_VERSION     0x00u

/** Software minor version number */
#define ETH_CONFIG_SW_MINOR_VERSION     0x02u

/** Software patch version number */
#define ETH_CONFIG_SW_PATCH_VERSION     0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* MAC address */
/**
 * @brief Ethernet DMA descriptor counts and default MAC address.
 */
#define ETH_TX_DESC_CNT  4U  /*!< Number of Ethernet Tx DMA descriptors */
#define ETH_RX_DESC_CNT  4U  /*!< Number of Ethernet Rx DMA descriptors */

#define ETH_MAC_ADDR0    ((uint8)0x02)
#define ETH_MAC_ADDR1    ((uint8)0x00)
#define ETH_MAC_ADDR2    ((uint8)0x00)
#define ETH_MAC_ADDR3    ((uint8)0x00)
#define ETH_MAC_ADDR4    ((uint8)0x00)
#define ETH_MAC_ADDR5    ((uint8)0x00)

/* Remote PC IP Address */
#define PC_IP_0                 10
#define PC_IP_1                 42
#define PC_IP_2                 0
#define PC_IP_3                 1

/* Static IP Configuration */
#define IP_ADDR0                10U
#define IP_ADDR1                42U
#define IP_ADDR2                0U
#define IP_ADDR3                177U

/* Netmask Configuration */
#define NETMASK_ADDR0           255U
#define NETMASK_ADDR1           255U
#define NETMASK_ADDR2           255U
#define NETMASK_ADDR3           0U

/* Gateway Configuration */
#define GW_ADDR0                10U
#define GW_ADDR1                42U
#define GW_ADDR2                0U
#define GW_ADDR3                1U

/* UDP Port Configuration */
#define UDP_SEND_PORT           5001
#define Eth_UdpServer_PORT      5000

/* Thread Priority */
#define Eth_UdpServer_PRIO      (osPriorityAboveNormal)

/* Protocol Configuration */
#define UDP_PACKET_SIZE         20

/* Endianness Configuration */
#define ENDIAN_BIG              0
#define ENDIAN_LITTLE           1
#define UDP_ENDIAN              ENDIAN_BIG

/* Application Constants */
#define LED_PULSE_MS            5
#define BUTTON_DEBOUNCE_MS      50
#define INITIAL_BTN_VALUE       10.0f
#define BTN_VALUE_INCREMENT     0.5f
#define BTN_VALUE_MAX           100.0f
#define CONST_FLOAT_VALUE       3.1415926f

#endif /* ETH_CONFIG_H */
