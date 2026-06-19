/**
 * @file    Eth_HwAb.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Ethernet Hardware Abstraction Layer implementation.
 *
 * @details
 * Manages the internal TX and RX packet buffers for UDP communication.
 * Provides channel-based read/write accessors for all packet fields, along
 * with endian-aware serialisation helpers for 32-bit integers and IEEE-754
 * floats (controlled by the UDP_ENDIAN compile-time switch). Includes a
 * simple 16-bit additive checksum for both TX packet generation and RX
 * packet verification.
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
#include "Eth_HwAb.h"
#include "Eth_Config.h"

/* ─── Private Variables ───────────────────────────────────────────────────── */

static Eth_TxPkt_t s_txPacket;
static Eth_RxPkt_t s_rxPacket;

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Eth_HwAb_Init(void)
{
    /* Initialize TX packet with default header */
    s_txPacket.header[0] = 0xAAU;
    s_txPacket.header[1] = 0xBBU;
    s_txPacket.counter   = 0U;
    s_txPacket.timestamp = 0U;
    s_txPacket.btnValue  = 0.0f;
    s_txPacket.constF2   = 0.0f;
    s_txPacket.checksum  = 0U;

    /* Initialize RX packet */
    memset(&s_rxPacket, 0, sizeof(s_rxPacket));
}

Eth_TxPkt_t* Eth_HwAb_GetTxPacket(void)
{
    return &s_txPacket;
}

void Eth_HwAb_WriteTx(Eth_HwAb_TxChannelType ch, const Eth_HwAb_FieldValue_t *value)
{
    if (value == NULL_PTR) return;

    switch (ch)
    {
        case HWAB_ETH_TX_HEADER:
            s_txPacket.header[0] = value->header[0];
            s_txPacket.header[1] = value->header[1];
            break;

        case HWAB_ETH_TX_COUNTER:
            Eth_HwAb_PutU32((uint8*)&s_txPacket.counter, value->u32);
            break;

        case HWAB_ETH_TX_TIMESTAMP:
            Eth_HwAb_PutU32((uint8*)&s_txPacket.timestamp, value->u32);
            break;

        case HWAB_ETH_TX_BTN_VALUE:
            Eth_HwAb_PutF32((uint8*)&s_txPacket.btnValue, value->f32);
            break;

        case HWAB_ETH_TX_CONST_F2:
            Eth_HwAb_PutF32((uint8*)&s_txPacket.constF2, value->f32);
            break;

        case HWAB_ETH_TX_CHECKSUM:
            /* Checksum is typically calculated, not written directly */
            Eth_HwAb_PutU32Be((uint8*)&s_txPacket.checksum, value->u16);
            break;

        default:
            break;
    }
}

void Eth_HwAb_UpdateTxChecksum(void)
{
    uint16 sum = Eth_HwAb_ChecksumSum16((uint8*)&s_txPacket,
                                           sizeof(Eth_TxPkt_t) - sizeof(uint16));

    /* Store checksum in big-endian */
    s_txPacket.checksum = (sum >> 8U) | (sum << 8U);
}

void Eth_HwAb_SetRxPacket(const uint8 *pkt)
{
    if (pkt != NULL_PTR)
    {
        memcpy(&s_rxPacket, pkt, sizeof(Eth_RxPkt_t));
    }
}

void Eth_HwAb_ReadRx(Eth_HwAb_RxChannelType ch, Eth_HwAb_FieldValue_t *value)
{
    if (value == NULL_PTR) return;

    switch (ch)
    {
        case HWAB_ETH_RX_HEADER:
            value->header[0] = s_rxPacket.header[0];
            value->header[1] = s_rxPacket.header[1];
            break;

        case HWAB_ETH_RX_CMD_TYPE:
            value->u8 = s_rxPacket.cmdType;
            break;

        case HWAB_ETH_RX_CMD_VALUE:
            value->u32 = Eth_HwAb_GetU32((uint8*)&s_rxPacket.cmdValue);
            break;

        case HWAB_ETH_RX_SETPOINT:
            value->f32 = Eth_HwAb_GetF32((uint8*)&s_rxPacket.setpoint);
            break;

        case HWAB_ETH_RX_CHECKSUM:
            value->u16 = (s_rxPacket.checksum >> 8U) | (s_rxPacket.checksum << 8U);
            break;

        default:
            break;
    }
}

boolean Eth_HwAb_VerifyRxChecksum(void)
{
    uint16 calculated = Eth_HwAb_ChecksumSum16((uint8*)&s_rxPacket,
                                                  sizeof(Eth_RxPkt_t) - sizeof(uint16));

    uint16 received = (s_rxPacket.checksum >> 8U) | (s_rxPacket.checksum << 8U);

    return (calculated == received);
}

/* ─── Checksum ────────────────────────────────────────────────────────────── */

uint16 Eth_HwAb_ChecksumSum16(const uint8 *p, int n)
{
    uint16 sum = 0U;
    for (int i = 0; i < n; i++)
    {
        sum += p[i];
    }
    return sum;
}

/* ─── 32-bit Integer Serialisation ───────────────────────────────────────── */

void Eth_HwAb_PutU32Be(uint8 *dst, uint32 x)
{
    dst[0] = (uint8)((x >> 24U) & 0xFFU);
    dst[1] = (uint8)((x >> 16U) & 0xFFU);
    dst[2] = (uint8)((x >>  8U) & 0xFFU);
    dst[3] = (uint8)((x >>  0U) & 0xFFU);
}

void Eth_HwAb_PutU32Le(uint8 *dst, uint32 x)
{
    dst[0] = (uint8)((x >>  0U) & 0xFFU);
    dst[1] = (uint8)((x >>  8U) & 0xFFU);
    dst[2] = (uint8)((x >> 16U) & 0xFFU);
    dst[3] = (uint8)((x >> 24U) & 0xFFU);
}

void Eth_HwAb_PutU32(uint8 *dst, uint32 x)
{
#if UDP_ENDIAN == ENDIAN_BIG
    Eth_HwAb_PutU32Be(dst, x);
#elif UDP_ENDIAN == ENDIAN_LITTLE
    Eth_HwAb_PutU32Le(dst, x);
#else
    #error "UDP_ENDIAN must be ENDIAN_BIG or ENDIAN_LITTLE"
#endif
}

uint32 Eth_HwAb_GetU32Be(const uint8 *src)
{
    return ((uint32)src[0] << 24U) |
           ((uint32)src[1] << 16U) |
           ((uint32)src[2] <<  8U) |
           ((uint32)src[3] <<  0U);
}

uint32 Eth_HwAb_GetU32Le(const uint8 *src)
{
    return ((uint32)src[0] <<  0U) |
           ((uint32)src[1] <<  8U) |
           ((uint32)src[2] << 16U) |
           ((uint32)src[3] << 24U);
}

uint32 Eth_HwAb_GetU32(const uint8 *src)
{
#if UDP_ENDIAN == ENDIAN_BIG
    return Eth_HwAb_GetU32Be(src);
#elif UDP_ENDIAN == ENDIAN_LITTLE
    return Eth_HwAb_GetU32Le(src);
#else
    #error "UDP_ENDIAN must be ENDIAN_BIG or ENDIAN_LITTLE"
#endif
}

/* ─── 32-bit Float (IEEE-754) Serialisation ──────────────────────────────── */

void Eth_HwAb_PutF32Be(uint8 *dst, float x)
{
    uint32 u;
    memcpy(&u, &x, sizeof(u));
    dst[0] = (uint8)((u >> 24U) & 0xFFU);
    dst[1] = (uint8)((u >> 16U) & 0xFFU);
    dst[2] = (uint8)((u >>  8U) & 0xFFU);
    dst[3] = (uint8)((u >>  0U) & 0xFFU);
}

void Eth_HwAb_PutF32Le(uint8 *dst, float x)
{
    uint32 u;
    memcpy(&u, &x, sizeof(u));
    dst[0] = (uint8)((u >>  0U) & 0xFFU);
    dst[1] = (uint8)((u >>  8U) & 0xFFU);
    dst[2] = (uint8)((u >> 16U) & 0xFFU);
    dst[3] = (uint8)((u >> 24U) & 0xFFU);
}

void Eth_HwAb_PutF32(uint8 *dst, float x)
{
#if UDP_ENDIAN == ENDIAN_BIG
    Eth_HwAb_PutF32Be(dst, x);
#elif UDP_ENDIAN == ENDIAN_LITTLE
    Eth_HwAb_PutF32Le(dst, x);
#else
    #error "UDP_ENDIAN must be ENDIAN_BIG or ENDIAN_LITTLE"
#endif
}

float Eth_HwAb_GetF32Be(const uint8 *src)
{
    uint32 u = ((uint32)src[0] << 24U) |
                 ((uint32)src[1] << 16U) |
                 ((uint32)src[2] <<  8U) |
                 ((uint32)src[3] <<  0U);
    float f;
    memcpy(&f, &u, sizeof(f));
    return f;
}

float Eth_HwAb_GetF32Le(const uint8 *src)
{
    uint32 u = ((uint32)src[0] <<  0U) |
                 ((uint32)src[1] <<  8U) |
                 ((uint32)src[2] << 16U) |
                 ((uint32)src[3] << 24U);
    float f;
    memcpy(&f, &u, sizeof(f));
    return f;
}

float Eth_HwAb_GetF32(const uint8 *src)
{
#if UDP_ENDIAN == ENDIAN_BIG
    return Eth_HwAb_GetF32Be(src);
#elif UDP_ENDIAN == ENDIAN_LITTLE
    return Eth_HwAb_GetF32Le(src);
#else
    #error "UDP_ENDIAN must be ENDIAN_BIG or ENDIAN_LITTLE"
#endif
}
