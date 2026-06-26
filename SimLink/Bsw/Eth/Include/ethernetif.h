/**
 * @file    ethernetif.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Header for ethernetif.c module.
 *
 * @details
 * Declares the LwIP network interface initialisation function and the
 * Ethernet link-status monitoring thread used in the LwIP HTTP Server
 * Netconn RTOS example (originally under
 * LwIP/LwIP_HTTP_Server_Netconn_RTOS/Inc/ethernetif.h).
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.2.3   | 2025-06-07 | A.Rezapour    | Reformatted to project template style|
 * |---------|------------|---------------|--------------------------------------|
 */

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialise the LwIP Ethernet network interface.
 * @param  netif  Pointer to the LwIP netif structure to be populated.
 * @return ERR_OK on success, or a LwIP err_t error code on failure.
 */
err_t ethernetif_init(struct netif *netif);

/**
 * @brief  Ethernet link-status monitoring thread.
 *         Periodically checks the physical link state and notifies LwIP of
 *         any link-up / link-down transitions.
 * @param  argument  CMSIS-RTOS thread argument (unused).
 */
void ethernet_link_thread(void const *argument);

#endif /* __ETHERNETIF_H__ */
