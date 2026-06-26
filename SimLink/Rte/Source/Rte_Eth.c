/**
 * @file    Rte_Eth.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   RTE OS Interface for the Ethernet domain.
 *
 * @details
 * This is the only file permitted to include lwip/tcpip.h, ETH_NetInit.h,
 * Eth_UdpServer.h, and PcComMgr.h. The init function sequences the lwIP
 * TCP/IP stack initialisation, network interface configuration, and PC
 * Communication Manager startup. The Eth runnable starts the UDP server
 * thread and terminates the host OS task after handing off to lwIP.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------|
 * | Version | Date       | Author           | Description                    |
 * |---------|------------|------------------|--------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h          |
 * |---------|------------|------------------|--------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_Eth.h"
#include "lwip/tcpip.h"
#include "ETH_NetInit.h"
#include "Eth_UdpServer.h"
#include "PcComMgr.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Rte_Eth_Init(void)
{
    /* Initialize lwIP TCP/IP stack; no callback or argument required */
    tcpip_init(NULL_PTR, NULL_PTR);

    /* Configure lwIP network interface */
    ETH_NetifConfig();

    /* Initialize PC communication manager and register packet builder */
    PcComMgr_Init();
}

void Rte_Runnable_Eth(void)
{
    /* Start UDP server thread */
    Eth_UdpServerInit();

    /* Terminate the OS task that hosts this runnable */
    osThreadTerminate(NULL_PTR);
}
