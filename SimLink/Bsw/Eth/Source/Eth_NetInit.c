/**
 * @file    Eth_NetInit.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Ethernet network interface initialisation (lwIP).
 *
 * @details
 * Configures the lwIP network interface with a static or DHCP-assigned IP
 * address, registers the ethernetif driver, and spawns the optional link
 * monitoring and DHCP client OS threads. The global netif handle g_netif
 * is set as the default network interface after initialisation.
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
#include "ETH_NetInit.h"
#include "Eth_Config.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "Eth_Status.h"
#include "lwip/tcpip.h"

/* ─── Public Variables ────────────────────────────────────────────────────── */

struct netif g_netif; /* Network interface structure */

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Configure and register the lwIP network interface.
 */
void ETH_NetifConfig(void)
{
    ip_addr_t ipAddr;
    ip_addr_t netMask;
    ip_addr_t gw;

#if LWIP_DHCP
    ip_addr_set_zero_ip4(&ipAddr);
    ip_addr_set_zero_ip4(&netMask);
    ip_addr_set_zero_ip4(&gw);
#else
    IP_ADDR4(&ipAddr,  IP_ADDR0,      IP_ADDR1,      IP_ADDR2,      IP_ADDR3);
    IP_ADDR4(&netMask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP_ADDR4(&gw,      GW_ADDR0,      GW_ADDR1,      GW_ADDR2,      GW_ADDR3);
#endif /* LWIP_DHCP */

    /* Add the network interface.
     * The NULL_PTR argument passes no user state to ethernetif_init,
     * which is the expected usage for this BSP driver.                      */
    netif_add(&g_netif, &ipAddr, &netMask, &gw, NULL_PTR,
              &ethernetif_init, &tcpip_input);

    /* Register as the default network interface */
    netif_set_default(&g_netif);

    Eth_LinkStatusUpdated(&g_netif);

#if LWIP_NETIF_LINK_CALLBACK
    netif_set_link_callback(&g_netif, Eth_LinkStatusUpdated);

    osThreadDef(EthLink, ethernet_link_thread, osPriorityNormal, 0,
                configMINIMAL_STACK_SIZE * 2);
    osThreadCreate(osThread(EthLink), &g_netif);
#endif

#if LWIP_DHCP
    /* Start DHCP client thread */
    osThreadDef(DHCP, DHCP_Thread, osPriorityBelowNormal, 0,
                configMINIMAL_STACK_SIZE * 2);
    osThreadCreate(osThread(DHCP), &g_netif);
#endif
}
