/**
 * @file    Eth_HwAb.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Ethernet hardware abstraction layer.
 *
 * @details
 * Provides packet structure definitions, logical channel abstraction for
 * TX/RX fields, endian-aware serialization/deserialization helpers, and
 * checksum computation for UDP Ethernet communication. Supports both
 * big-endian and little-endian byte ordering for multi-byte fields.
 * The module now uses the project's standard types defined in Std_Types.h.
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

#ifndef ETH_HWAB_H
#define ETH_HWAB_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include <stdbool.h>
#include <string.h>

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define ETH_HWAB_VENDOR_ID          0xFFFFu

/** AUTOSAR module identifier for the Ethernet driver */
#define ETH_HWAB_MODULE_ID          88u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define ETH_HWAB_SW_MAJOR_VERSION   0x00u

/** Software minor version number */
#define ETH_HWAB_SW_MINOR_VERSION   0x02u

/** Software patch version number */
#define ETH_HWAB_SW_PATCH_VERSION   0x03u

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief Transmit packet structure.
 */
typedef struct __attribute__((packed))
{
    uint8  header[2];         /**< Packet header: 0xAA, 0xBB              */
    uint32 counter;           /**< Packet counter                          */
    uint32 timestamp;         /**< Timestamp in milliseconds               */
    float    btnValue;          /**< Button value                            */
    float    constF2;           /**< Constant float value                    */
    uint16 checksum;          /**< Packet checksum                         */
} Eth_TxPkt_t;

/**
 * @brief Receive packet structure.
 */
typedef struct __attribute__((packed))
{
    uint8  header[2];         /**< Packet header: 0xCC, 0xDD              */
    uint8  cmdType;           /**< Command type                            */
    uint8  reserved;          /**< Reserved byte                           */
    uint32 cmdValue;          /**< Command value                           */
    float    setpoint;          /**< Setpoint value                          */
    uint16 checksum;          /**< Packet checksum                         */
} Eth_RxPkt_t;

/**
 * @brief TX logical channel enumeration.
 */
typedef enum
{
    HWAB_ETH_TX_HEADER,
    HWAB_ETH_TX_COUNTER,
    HWAB_ETH_TX_TIMESTAMP,
    HWAB_ETH_TX_BTN_VALUE,
    HWAB_ETH_TX_CONST_F2,
    HWAB_ETH_TX_CHECKSUM,
    HWAB_ETH_TX_CHANNEL_COUNT   /**< Total TX channel count */
} Eth_HwAb_TxChannelType;

/**
 * @brief RX logical channel enumeration.
 */
typedef enum
{
    HWAB_ETH_RX_HEADER,
    HWAB_ETH_RX_CMD_TYPE,
    HWAB_ETH_RX_CMD_VALUE,
    HWAB_ETH_RX_SETPOINT,
    HWAB_ETH_RX_CHECKSUM,
    HWAB_ETH_RX_CHANNEL_COUNT   /**< Total RX channel count */
} Eth_HwAb_RxChannelType;

/**
 * @brief Field value union for type-safe channel access.
 */
typedef union
{
    uint8  u8;                /**< 8-bit unsigned integer                  */
    uint16 u16;               /**< 16-bit unsigned integer                 */
    uint32 u32;               /**< 32-bit unsigned integer                 */
    float    f32;               /**< 32-bit floating point                   */
    uint8  header[2];         /**< 2-byte header                           */
} Eth_HwAb_FieldValue_t;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialize the Ethernet hardware abstraction layer.
 */
void Eth_HwAb_Init(void);

/**
 * @brief  Write a value to a TX channel.
 * @param  ch     TX channel identifier.
 * @param  value  Pointer to field value union.
 */
void Eth_HwAb_WriteTx(Eth_HwAb_TxChannelType ch, const Eth_HwAb_FieldValue_t *value);

/**
 * @brief  Read a value from an RX channel.
 * @param  ch     RX channel identifier.
 * @param  value  Pointer to field value union (output).
 */
void Eth_HwAb_ReadRx(Eth_HwAb_RxChannelType ch, Eth_HwAb_FieldValue_t *value);

/**
 * @brief  Get pointer to the TX packet buffer.
 * @return Pointer to TX packet structure.
 */
Eth_TxPkt_t* Eth_HwAb_GetTxPacket(void);

/**
 * @brief  Set the RX packet buffer from received data.
 * @param  pkt  Pointer to received packet data.
 */
void Eth_HwAb_SetRxPacket(const uint8 *pkt);

/**
 * @brief  Update the checksum field in the TX packet.
 */
void Eth_HwAb_UpdateTxChecksum(void);

/**
 * @brief  Verify the checksum of the RX packet.
 * @return true if checksum is valid, false otherwise.
 */
boolean Eth_HwAb_VerifyRxChecksum(void);

/**
 * @brief  Compute 16-bit checksum over a byte array.
 * @param  p  Pointer to data buffer.
 * @param  n  Number of bytes.
 * @return Computed checksum value.
 */
uint16 Eth_HwAb_ChecksumSum16(const uint8 *p, int n);

/* ─── Serialization Helpers ───────────────────────────────────────────────── */

/**
 * @brief  Serialize uint32 in big-endian format.
 * @param  dst  Destination buffer (4 bytes).
 * @param  x    Value to serialize.
 */
void Eth_HwAb_PutU32Be(uint8 *dst, uint32 x);

/**
 * @brief  Serialize uint32 in little-endian format.
 * @param  dst  Destination buffer (4 bytes).
 * @param  x    Value to serialize.
 */
void Eth_HwAb_PutU32Le(uint8 *dst, uint32 x);

/**
 * @brief  Serialize uint32 using configured endianness.
 * @param  dst  Destination buffer (4 bytes).
 * @param  x    Value to serialize.
 */
void Eth_HwAb_PutU32(uint8 *dst, uint32 x);

/**
 * @brief  Serialize float in big-endian format.
 * @param  dst  Destination buffer (4 bytes).
 * @param  x    Value to serialize.
 */
void Eth_HwAb_PutF32Be(uint8 *dst, float x);

/**
 * @brief  Serialize float in little-endian format.
 * @param  dst  Destination buffer (4 bytes).
 * @param  x    Value to serialize.
 */
void Eth_HwAb_PutF32Le(uint8 *dst, float x);

/**
 * @brief  Serialize float using configured endianness.
 * @param  dst  Destination buffer (4 bytes).
 * @param  x    Value to serialize.
 */
void Eth_HwAb_PutF32(uint8 *dst, float x);

/**
 * @brief  Deserialize uint32 from big-endian format.
 * @param  src  Source buffer (4 bytes).
 * @return Deserialized value.
 */
uint32 Eth_HwAb_GetU32Be(const uint8 *src);

/**
 * @brief  Deserialize uint32 from little-endian format.
 * @param  src  Source buffer (4 bytes).
 * @return Deserialized value.
 */
uint32 Eth_HwAb_GetU32Le(const uint8 *src);

/**
 * @brief  Deserialize uint32 using configured endianness.
 * @param  src  Source buffer (4 bytes).
 * @return Deserialized value.
 */
uint32 Eth_HwAb_GetU32(const uint8 *src);

/**
 * @brief  Deserialize float from big-endian format.
 * @param  src  Source buffer (4 bytes).
 * @return Deserialized value.
 */
float Eth_HwAb_GetF32Be(const uint8 *src);

/**
 * @brief  Deserialize float from little-endian format.
 * @param  src  Source buffer (4 bytes).
 * @return Deserialized value.
 */
float Eth_HwAb_GetF32Le(const uint8 *src);

/**
 * @brief  Deserialize float using configured endianness.
 * @param  src  Source buffer (4 bytes).
 * @return Deserialized value.
 */
float Eth_HwAb_GetF32(const uint8 *src);

#endif /* ETH_HWAB_H */
