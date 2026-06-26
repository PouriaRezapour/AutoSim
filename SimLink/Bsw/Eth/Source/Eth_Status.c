/**
 * @file    Eth_Status.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Ethernet link status and DHCP state machine implementation.
 *
 * @details
 * Handles network interface link state changes and drives the DHCP client
 * state machine. When the link comes up, DHCP negotiation is started (if
 * enabled) or the static IP LEDs are updated. If DHCP times out after
 * MAX_DHCP_TRIES attempts, a static fallback address is applied. LED2
 * indicates an active/assigned link; LED3 indicates link down or DHCP
 * failure.
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
#include "Mcal.h"

#include "lwip/opt.h"
#include "lwip/netifapi.h"
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif

#include "Eth_Status.h"
#include "ethernetif.h"

#include "DioIf.h"


/* ─── Private Macros ──────────────────────────────────────────────────────── */

#if LWIP_DHCP
#define MAX_DHCP_TRIES  4
#endif

/* ─── Private Variables ───────────────────────────────────────────────────── */

#if LWIP_DHCP
__IO uint8 DHCP_state = DHCP_OFF;
#endif

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Notify the user about the network interface link status change.
 * @param  netif  Pointer to the network interface structure.
 */
void Eth_LinkStatusUpdated(struct netif *netif)
{
    if (netif_is_up(netif))
    {
#if LWIP_DHCP
        /* Update DHCP state machine */
        DHCP_state = DHCP_START;
#else
        DioIf_Write(HWAB_DIO_LED2,STD_ON);
        DioIf_Write(HWAB_DIO_LED3,STD_OFF);
#endif /* LWIP_DHCP */
    }
    else
    {
#if LWIP_DHCP
        /* Update DHCP state machine */
        DHCP_state = DHCP_LINK_DOWN;
#else
        DioIf_Write(HWAB_DIO_LED2,STD_OFF);
        DioIf_Write(HWAB_DIO_LED3,STD_ON);
#endif /* LWIP_DHCP */
    }
}

#if LWIP_DHCP
/**
 * @brief  DHCP client OS thread.
 * @param  argument  Pointer to the network interface structure.
 */
void DHCP_Thread(void const *argument)
{
    struct netif *netif = (struct netif *)argument;
    ip_addr_t ipAddr;
    ip_addr_t netMask;
    ip_addr_t gw;
    struct dhcp *dhcp;

    for (;;)
    {
        switch (DHCP_state)
        {
            case DHCP_START:
            {
                ip_addr_set_zero_ip4(&netif->ip_addr);
                ip_addr_set_zero_ip4(&netif->netMask);
                ip_addr_set_zero_ip4(&netif->gw);
                DHCP_state = DHCP_WAIT_ADDRESS;

                DioIf_Write(HWAB_DIO_LED2,STD_OFF);
                DioIf_Write(HWAB_DIO_LED3,STD_OFF);

                netifapi_dhcp_start(netif);
            }
            break;

            case DHCP_WAIT_ADDRESS:
            {
                if (dhcp_supplied_address(netif))
                {
                    DHCP_state = DHCP_ADDRESS_ASSIGNED;

                    DioIf_Write(HWAB_DIO_LED2,STD_ON);
                    DioIf_Write(HWAB_DIO_LED3,STD_OFF);
                }
                else
                {
                    dhcp = (struct dhcp *)netif_get_client_data(
                               netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

                    /* DHCP timeout — fall back to static address */
                    if (dhcp->tries > MAX_DHCP_TRIES)
                    {
                        DHCP_state = DHCP_TIMEOUT;

                        IP_ADDR4(&ipAddr,  IP_ADDR0,      IP_ADDR1,
                                           IP_ADDR2,      IP_ADDR3);
                        IP_ADDR4(&netMask, NETMASK_ADDR0, NETMASK_ADDR1,
                                           NETMASK_ADDR2, NETMASK_ADDR3);
                        IP_ADDR4(&gw,      GW_ADDR0,      GW_ADDR1,
                                           GW_ADDR2,      GW_ADDR3);
                        netifapi_netif_set_addr(netif, ip_2_ip4(&ipAddr),
                                                ip_2_ip4(&netMask),
                                                ip_2_ip4(&gw));

                        DioIf_Write(HWAB_DIO_LED2,STD_ON);
                        DioIf_Write(HWAB_DIO_LED3,STD_OFF);
                    }
                }
            }
            break;

            case DHCP_LINK_DOWN:
            {
                DHCP_state = DHCP_OFF;

                DioIf_Write(HWAB_DIO_LED2,STD_OFF);
                DioIf_Write(HWAB_DIO_LED3,STD_ON);
            }
            break;

            default:
                break;
        }

        /* Wait 500 ms */
        osDelay(500U);
    }
}
#endif /* LWIP_DHCP */
