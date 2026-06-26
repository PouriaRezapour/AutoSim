/**
 * @file    Eth_LwipCfg.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-05-10
 * @version 0.1.0
 * @brief   lwIP stack configuration options for STM32H7xx targets.
 *
 * @details
 * This file overrides the default lwIP compile-time options defined in
 * opts.h. It enables the RTOS-aware (NO_SYS=0) configuration with TCP,
 * UDP, ICMP and HTTP server support. Hardware checksum offload is active
 * by default via the CHECKSUM_BY_HARDWARE define; comment it out to fall
 * back to software checksums. Heap and thread parameters are tuned for an
 * STM32H7xx device with lwIP RAM placed at a fixed address in AXI SRAM.
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.1.0   | 2025-05-10 | A.Rezapour    | Initial release                      |
 * |---------|------------|---------------|--------------------------------------|
 */

#ifndef ETH_LWIPCFG_H
#define ETH_LWIPCFG_H

/* ─── System Options ──────────────────────────────────────────────────────── */

/**
 * @brief NO_SYS: Operating-system integration mode.
 *        Set to 1 for a bare-metal build with very minimal functionality.
 *        Set to 0 (default here) to use the full lwIP RTOS facilities
 *        (mailboxes, semaphores, thread support).
 */
#define NO_SYS                          0

/* ─── Memory Options ──────────────────────────────────────────────────────── */

/**
 * @brief MEM_ALIGNMENT: CPU memory alignment in bytes.
 *        Must match the natural alignment of the target CPU.
 *        Use 4 for 32-bit ARM Cortex-M devices.
 */
#define MEM_ALIGNMENT                   4

/**
 * @brief MEM_SIZE: Total heap size in bytes available to lwIP.
 *        Increase this value when the application sends large amounts
 *        of data that must be copied into the stack's internal buffers.
 */
#define MEM_SIZE                        (14*1024)

/**
 * @brief LWIP_RAM_HEAP_POINTER: Absolute address of the lwIP RAM heap.
 *        Relocates the heap to a specific memory region (here: AXI SRAM
 *        on STM32H7xx) instead of using the default linker-placed area.
 */
#define LWIP_RAM_HEAP_POINTER           (0x30004000)

/**
 * @brief MEMP_NUM_TCP_PCB: Maximum number of simultaneously active TCP
 *        connections (Protocol Control Blocks).
 */
#define MEMP_NUM_TCP_PCB                10

/**
 * @brief MEMP_NUM_TCP_SEG: Maximum number of simultaneously queued TCP
 *        segments across all connections.
 */
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN

/* ─── Pbuf Options ────────────────────────────────────────────────────────── */

/**
 * @brief PBUF_POOL_BUFSIZE: Size of each pbuf in the pool, in bytes.
 *        Set to accommodate a full Ethernet frame payload (1536 bytes).
 * @note  The pool is used to allocate Tx pbufs only.
 */
#define PBUF_POOL_BUFSIZE               1536

/**
 * @brief LWIP_SUPPORT_CUSTOM_PBUF: Allow zero-copy reception.
 *        When set to 1, MAC Rx buffers can be passed directly to the
 *        stack as custom pbufs, avoiding an extra memory copy.
 */
#define LWIP_SUPPORT_CUSTOM_PBUF        1

/* ─── IPv4 Options ────────────────────────────────────────────────────────── */

/**
 * @brief LWIP_IPV4: Enable IPv4 support.
 */
#define LWIP_IPV4                       1

/* ─── TCP Options ─────────────────────────────────────────────────────────── */

/**
 * @brief LWIP_TCP: Enable the TCP protocol.
 */
#define LWIP_TCP                        1

/**
 * @brief TCP_TTL: Default Time-To-Live value for TCP segments.
 */
#define TCP_TTL                         255

/**
 * @brief TCP_MSS: Maximum Segment Size in bytes.
 *        Calculated as: Ethernet MTU (1500) - IP header (20) - TCP header (20).
 */
#define TCP_MSS                         (1500 - 40)

/**
 * @brief TCP_SND_BUF: TCP sender buffer space in bytes.
 *        Controls how much unacknowledged data the stack may hold per connection.
 */
#define TCP_SND_BUF                     (4*TCP_MSS)

/**
 * @brief TCP_WND: TCP receive window size in bytes.
 *        Advertised to the remote peer; governs inbound flow control.
 * @note  Out-of-order segment queuing is controlled separately.
 *        Set TCP_QUEUE_OOSEQ to 0 on memory-constrained devices.
 */
#define TCP_WND                         (4*TCP_MSS)

/* ─── ICMP Options ────────────────────────────────────────────────────────── */

/**
 * @brief LWIP_ICMP: Enable ICMP support (ping, destination-unreachable, etc.).
 */
#define LWIP_ICMP                       1

/* ─── DHCP Options ────────────────────────────────────────────────────────── */

/**
 * @brief LWIP_DHCP: Enable DHCP client.
 *        Set to 0 to use a static IP configuration (current default).
 */
#define LWIP_DHCP                       0

/* ─── UDP Options ─────────────────────────────────────────────────────────── */

/**
 * @brief LWIP_UDP: Enable the UDP protocol.
 */
#define LWIP_UDP                        1

/**
 * @brief UDP_TTL: Default Time-To-Live value for UDP datagrams.
 */
#define UDP_TTL                         255

/* ─── Statistics Options ──────────────────────────────────────────────────── */

/**
 * @brief LWIP_STATS: Enable lwIP internal statistics collection.
 *        Set to 0 to reduce code size and RAM usage in production builds.
 */
#define LWIP_STATS                      0

/* ─── Network Interface Options ───────────────────────────────────────────── */

/**
 * @brief LWIP_NETIF_LINK_CALLBACK: Enable link-state change callbacks.
 *        When set to 1, a registered callback is invoked whenever the
 *        physical link goes up or down on a network interface.
 */
#define LWIP_NETIF_LINK_CALLBACK        1

/* ─── Checksum Options ────────────────────────────────────────────────────── */

/**
 * @brief CHECKSUM_BY_HARDWARE: Use the STM32H7xx Ethernet DMA hardware
 *        to compute and verify IP, UDP, TCP, and ICMP checksums.
 *        Comment out this define to process checksums in software (CPU).
 */
#define CHECKSUM_BY_HARDWARE

#ifdef CHECKSUM_BY_HARDWARE

  /** @brief CHECKSUM_GEN_IP: Generate IP checksums in hardware (0 = hardware). */
  #define CHECKSUM_GEN_IP               0

  /** @brief CHECKSUM_GEN_UDP: Generate UDP checksums in hardware (0 = hardware). */
  #define CHECKSUM_GEN_UDP              0

  /** @brief CHECKSUM_GEN_TCP: Generate TCP checksums in hardware (0 = hardware). */
  #define CHECKSUM_GEN_TCP              0

  /** @brief CHECKSUM_CHECK_IP: Verify incoming IP checksums in hardware. */
  #define CHECKSUM_CHECK_IP             0

  /** @brief CHECKSUM_CHECK_UDP: Verify incoming UDP checksums in hardware. */
  #define CHECKSUM_CHECK_UDP            0

  /** @brief CHECKSUM_CHECK_TCP: Verify incoming TCP checksums in hardware. */
  #define CHECKSUM_CHECK_TCP            0

  /**
   * @brief CHECKSUM_GEN_ICMP: Generate outgoing ICMP checksums in software.
   * @note  Hardware TCP/UDP checksum insertion is not supported when the
   *        packet is an IPv4 fragment; ICMP must always use software here.
   */
  #define CHECKSUM_GEN_ICMP             1

  /** @brief CHECKSUM_CHECK_ICMP: Verify incoming ICMP checksums in hardware. */
  #define CHECKSUM_CHECK_ICMP           0

#else

  /** @brief CHECKSUM_GEN_IP: Generate IP checksums in software (1 = software). */
  #define CHECKSUM_GEN_IP               1

  /** @brief CHECKSUM_GEN_UDP: Generate UDP checksums in software. */
  #define CHECKSUM_GEN_UDP              1

  /** @brief CHECKSUM_GEN_TCP: Generate TCP checksums in software. */
  #define CHECKSUM_GEN_TCP              1

  /** @brief CHECKSUM_CHECK_IP: Verify incoming IP checksums in software. */
  #define CHECKSUM_CHECK_IP             1

  /** @brief CHECKSUM_CHECK_UDP: Verify incoming UDP checksums in software. */
  #define CHECKSUM_CHECK_UDP            1

  /** @brief CHECKSUM_CHECK_TCP: Verify incoming TCP checksums in software. */
  #define CHECKSUM_CHECK_TCP            1

  /** @brief CHECKSUM_GEN_ICMP: Generate outgoing ICMP checksums in software. */
  #define CHECKSUM_GEN_ICMP             1

  /** @brief CHECKSUM_CHECK_ICMP: Verify incoming ICMP checksums in software. */
  #define CHECKSUM_CHECK_ICMP           1

#endif /* CHECKSUM_BY_HARDWARE */

/* ─── Sequential (Netconn) API Options ───────────────────────────────────── */

/**
 * @brief LWIP_NETCONN: Enable the Netconn sequential API.
 *        Requires api_lib.c to be included in the build.
 */
#define LWIP_NETCONN                    1

/**
 * @brief LWIP_SO_RCVTIMEO: Enable receive timeout support for Netconn sockets.
 *        Allows blocking recv calls to time out after a configurable period.
 */
#define LWIP_SO_RCVTIMEO                1

/* ─── Socket API Options ──────────────────────────────────────────────────── */

/**
 * @brief LWIP_SOCKET: Enable the BSD-style Socket API.
 *        Requires sockets.c to be included in the build.
 *        Disabled here in favour of the lighter Netconn API.
 */
#define LWIP_SOCKET                     0

/* ─── Netif API Options ───────────────────────────────────────────────────── */

/**
 * @brief LWIP_NETIF_API: Enable the NETIF management API.
 *        Provides functions to add, remove, and configure network interfaces
 *        from application code.
 */
#define LWIP_NETIF_API                  1

/* ─── HTTP Server Options ─────────────────────────────────────────────────── */

/**
 * @brief HTTPD_USE_CUSTOM_FSDATA: Use "fsdata_custom.c" for the HTTP server
 *        file system instead of the default "fsdata.c".
 *        Prevents unintentional modifications to the CVS-controlled file.
 */
#define HTTPD_USE_CUSTOM_FSDATA         1

/** @brief MEMP_DEBUG: Enable debug output for the memp memory pool module. */
#define MEMP_DEBUG                      LWIP_DBG_ON

/** @brief MEM_DEBUG: Enable debug output for the heap memory module. */
#define MEM_DEBUG                       LWIP_DBG_ON

/* ─── OS / Thread Options ─────────────────────────────────────────────────── */

/** @brief TCPIP_THREAD_NAME: Name string assigned to the TCP/IP thread. */
#define TCPIP_THREAD_NAME               "TCP/IP"

/** @brief TCPIP_THREAD_STACKSIZE: Stack size in words for the TCP/IP thread. */
#define TCPIP_THREAD_STACKSIZE          2048

/** @brief TCPIP_MBOX_SIZE: Depth of the TCP/IP thread input mailbox. */
#define TCPIP_MBOX_SIZE                 6

/** @brief DEFAULT_UDP_RECVMBOX_SIZE: Depth of each UDP receive mailbox. */
#define DEFAULT_UDP_RECVMBOX_SIZE       6

/** @brief DEFAULT_TCP_RECVMBOX_SIZE: Depth of each TCP receive mailbox. */
#define DEFAULT_TCP_RECVMBOX_SIZE       6

/** @brief DEFAULT_ACCEPTMBOX_SIZE: Depth of each TCP accept mailbox. */
#define DEFAULT_ACCEPTMBOX_SIZE         6

/** @brief DEFAULT_THREAD_STACKSIZE: Default stack size in words for lwIP threads. */
#define DEFAULT_THREAD_STACKSIZE        1024

/** @brief TCPIP_THREAD_PRIO: RTOS priority assigned to the TCP/IP thread. */
#define TCPIP_THREAD_PRIO               osPriorityHigh

#endif /* ETH_LWIPCFG_H */
