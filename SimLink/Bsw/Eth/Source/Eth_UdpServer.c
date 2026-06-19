/**
 * @file    Eth_UdpServer.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   UDP server implementation using the lwIP Netconn API.
 *
 * @details
 * Implements a high-performance event-driven UDP thread. Unlike previous
 * versions that used non-blocking polling and RTOS delays (limiting throughput
 * to 1000 pps), this implementation uses a blocking model with a 1ms timeout.
 * The thread sleeps when idle, allowing the CPU to process other tasks, but
 * wakes up instantly via interrupt upon packet arrival or timer expiry.
 * This architecture maximizes Ethernet throughput by bypassing the RTOS
 * tick-rate bottleneck.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|-------------------------------------------|
 * | Version | Date       | Author           | Description                               |
 * |---------|------------|------------------|-------------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                           |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template                 |
 * | 0.1.3   | 2025-05-18 | A.Rezapour       | Optimized throughput; removed osDelay(1)  |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h                     |
 * |---------|------------|------------------|-------------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Eth_UdpServer.h"
#include "Eth_Config.h"
#include "Eth_HwAb.h"

/* ─── Private Variables ───────────────────────────────────────────────────── */

static struct netconn *s_udpConn        = NULL_PTR;
static UdpPktBldr_t    s_pktBuilder     = NULL_PTR;
static UdpPreBuild_t   s_preBuild       = NULL_PTR;
static uint32        s_ledOffDeadline = 0U;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static void Eth_UdpServerThread(void *arg);
static void Eth_UdpSendPacket(struct netconn *conn);

/* ─── Private Functions ───────────────────────────────────────────────────── */

static void Eth_UdpSendPacket(struct netconn *conn)
{
    struct netbuf *buf = netbuf_new();
    if (buf == NULL_PTR) return;

    uint8 *data = (uint8 *)netbuf_alloc(buf, UDP_PACKET_SIZE);
    if (data == NULL_PTR)
    {
        netbuf_delete(buf);
        return;
    }

    /* Notify application layer to refresh TX signals into RTE */
    if (s_preBuild != NULL_PTR)
    {
        s_preBuild();
    }

    /* Serialise RTE signals into UDP buffer */
    if (s_pktBuilder != NULL_PTR)
    {
        s_pktBuilder(data, UDP_PACKET_SIZE);
    }
    else
    {
        for (uint16 i = 0U; i < UDP_PACKET_SIZE; i++) data[i] = 0U;
    }

    ip_addr_t remote_addr;
    IP4_ADDR(&remote_addr, PC_IP_0, PC_IP_1, PC_IP_2, PC_IP_3);
    (void)netconn_sendto(conn, buf, &remote_addr, UDP_SEND_PORT);

    netbuf_delete(buf);
}

static void Eth_UdpServerThread(void *arg)
{
    (void)arg;

    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
    BSP_LED_Off(LED2);
    s_ledOffDeadline = 0U;

    s_udpConn = netconn_new(NETCONN_UDP);
    if (s_udpConn == NULL_PTR) return;

    if (netconn_bind(s_udpConn, IP_ADDR_ANY, Eth_UdpServer_PORT) != ERR_OK)
    {
        netconn_delete(s_udpConn);
        s_udpConn = NULL_PTR;
        return;
    }

    /* FIX: Set a receive timeout instead of non-blocking mode.
     * This allows netconn_recv to block, but wake up periodically.
     */
    netconn_set_recvtimeout(s_udpConn, 1);

    while (1)
    {
        /* 1. Receive loop (blocking with timeout) */
        struct netbuf *buf = NULL_PTR;
        err_t err = netconn_recv(s_udpConn, &buf);

        if (err == ERR_OK && buf != NULL_PTR)
        {
            /* Packet received */
            BSP_LED_On(LED2);
            s_ledOffDeadline = HAL_GetTick() + LED_PULSE_MS;
            netbuf_delete(buf);
        }
        /* Note: If err == ERR_TIMEOUT, we just proceed to the send check. */

        /* 2. Send check */
        Eth_UdpSendPacket(s_udpConn);

        /* 3. LED check */
        if (s_ledOffDeadline != 0U && (int32_t)(HAL_GetTick() - s_ledOffDeadline) >= 0)
        {
            BSP_LED_Off(LED2);
            s_ledOffDeadline = 0U;
        }

        /* REMOVED in v0.1.3: osDelay(1); */
    }
}

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Eth_UdpServerInit(void)
{
    sys_thread_new("UDP", Eth_UdpServerThread, NULL_PTR,
                   DEFAULT_THREAD_STACKSIZE, Eth_UdpServer_PRIO);
}

void Eth_SetPktBldr(UdpPktBldr_t builder)
{
    s_pktBuilder = builder;
}

void Eth_SetPreBuild(UdpPreBuild_t hook)
{
    s_preBuild = hook;
}

err_t Eth_UdpSendData(const uint8 *data)
{
    if (s_udpConn == NULL_PTR) return ERR_CONN;

    struct netbuf *buf = netbuf_new();
    if (buf == NULL_PTR) return ERR_MEM;

    uint8 *p = (uint8 *)netbuf_alloc(buf, UDP_PACKET_SIZE);
    if (p == NULL_PTR)
    {
        netbuf_delete(buf);
        return ERR_MEM;
    }

    memcpy(p, data, UDP_PACKET_SIZE);

    ip_addr_t addr;
    IP4_ADDR(&addr, PC_IP_0, PC_IP_1, PC_IP_2, PC_IP_3);

    err_t result = netconn_sendto(s_udpConn, buf, &addr, UDP_SEND_PORT);
    netbuf_delete(buf);
    return result;
}

err_t Eth_UdpSendCustomPacket(const char *data, u16_t len,
                               const char *remote_ip, u16_t remote_port)
{
    (void)data; (void)len; (void)remote_ip; (void)remote_port;
    return ERR_VAL;
}
