/**
 * @file    Dio.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   AUTOSAR DIO Driver – Public Header.
 *
 * @details
 * Defines all types, error codes and API function prototypes for the
 * AUTOSAR DIO Driver module targeting the STM32H753 MCU.
 *
 * AUTOSAR CP R24-11
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour | Template standardization             |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | Module now uses the project's        |
 * |         |            |            | standard types defined in            |
 * |         |            |            | Std_Types.h.                         |
 * | 0.2.6   | 2025-07-08 | A.Rezapour | Migrated to AUTOSAR-compliant        |
 * |         |            |            | implementation.                      |
 * |---------|------------|------------|--------------------------------------|
 */

#ifndef DIO_H
#define DIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Dio_Cfg.h"

/* ─── Module Identification ───────────────────────────────────────────────── */

#define DIO_VENDOR_ID           (0U)     /**< Vendor ID – adjust to your org          */
#define DIO_MODULE_ID           (120U)   /**< AUTOSAR Module ID for DIO Driver        */
#define DIO_INSTANCE_ID         (0U)

#define DIO_AR_RELEASE_MAJOR_VERSION    (4U)
#define DIO_AR_RELEASE_MINOR_VERSION    (7U)
#define DIO_AR_RELEASE_PATCH_VERSION    (0U)

#define DIO_SW_MAJOR_VERSION    (1U)
#define DIO_SW_MINOR_VERSION    (0U)
#define DIO_SW_PATCH_VERSION    (0U)

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief   Numeric ID of a DIO channel (pin).
 * @details [SWS_Dio_00182] Derived from uint.
 *          Encoding for STM32H753: ChannelId = (PortIndex * 16) + PinNumber
 *          Covers all available DIO channels (ports A..K, pins 0..15).
 */
typedef uint16 Dio_ChannelType;

/**
 * @brief   Numeric ID of a DIO port.
 * @details [SWS_Dio_00183] Derived from uint.
 *          0 = GPIOA, 1 = GPIOB, ..., 10 = GPIOK.
 */
typedef uint8  Dio_PortType;

/**
 * @brief   Type for the definition of a DIO channel group.
 * @details [SWS_Dio_00184] Structure containing mask, offset and port.
 *          Used with Dio_ReadChannelGroup / Dio_WriteChannelGroup.
 */
typedef struct
{
    uint16     mask;   /**< Bit mask that defines the channel positions [SWS_Dio_00184] */
    uint8      offset; /**< Position of LSB of the group, counted from port LSB         */
    Dio_PortType port;   /**< Port on which the channel group is defined                  */
} Dio_ChannelGroupType;

/**
 * @brief   Possible levels a DIO channel can have.
 * @details [SWS_Dio_00185] Derived from uint8.
 *          STD_LOW  (0x00) = Physical state 0 V
 *          STD_HIGH (0x01) = Physical state 3.3 V / 5 V
 */
typedef uint8 Dio_LevelType;

/**
 * @brief   Value of a complete DIO port (all channels).
 * @details [SWS_Dio_00186] Size = size of the largest STM32H753 port = 16 bit.
 *          [SWS_Dio_00103] Port width is the largest port on the MCU.
 */
typedef uint16 Dio_PortLevelType;

/* ─── Development Error Codes [SWS_Dio_91001] ────────────────────────────── */

/** Invalid channel ID passed to an API function */
#define DIO_E_PARAM_INVALID_CHANNEL_ID  ((uint8)0x0AU)

/** Invalid port ID passed to an API function */
#define DIO_E_PARAM_INVALID_PORT_ID     ((uint8)0x14U)

/** Invalid channel group pointer passed to an API function */
#define DIO_E_PARAM_INVALID_GROUP       ((uint8)0x1FU)

/** NULL pointer passed where a valid pointer is required */
#define DIO_E_PARAM_POINTER             ((uint8)0x20U)

/* ─── Service IDs (for DET Reporting) ────────────────────────────────────── */

#define DIO_SID_READ_CHANNEL            ((uint8)0x00U)
#define DIO_SID_WRITE_CHANNEL           ((uint8)0x01U)
#define DIO_SID_READ_PORT               ((uint8)0x02U)
#define DIO_SID_WRITE_PORT              ((uint8)0x03U)
#define DIO_SID_READ_CHANNEL_GROUP      ((uint8)0x04U)
#define DIO_SID_WRITE_CHANNEL_GROUP     ((uint8)0x05U)
#define DIO_SID_GET_VERSION_INFO        ((uint8)0x12U)
#define DIO_SID_FLIP_CHANNEL            ((uint8)0x11U)
#define DIO_SID_MASKED_WRITE_PORT       ((uint8)0x13U)

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief   Returns the value of the specified DIO channel.
 * @details [SWS_Dio_00133] Synchronous, Reentrant.
 *          [SWS_Dio_00027] Returns physical pin level (IDR) for output pins
 *          if hardware supports readback [SWS_Dio_00083]; otherwise ODR value
 *          [SWS_Dio_00084]. STM32H753 IDR always reflects the actual pin level.
 *
 * @param[in] ChannelId   ID of the DIO channel (use DioConf_DioChannel_Pxy)
 * @return    Dio_LevelType   STD_HIGH or STD_LOW
 */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/**
 * @brief   Sets the level of a single DIO channel.
 * @details [SWS_Dio_00134] Synchronous, Reentrant.
 *          [SWS_Dio_00006] Sets the level to STD_HIGH or STD_LOW.
 *          [SWS_Dio_00028] Effective only when channel is configured as output.
 *          [SWS_Dio_00029] No effect on physical output if channel is input.
 *          Uses BSRR for atomic write [SWS_Dio_00005].
 *
 * @param[in] ChannelId   ID of the DIO channel
 * @param[in] Level       STD_HIGH or STD_LOW
 */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/**
 * @brief   Returns the level of all channels of a port.
 * @details [SWS_Dio_00135] Synchronous, Reentrant.
 *          [SWS_Dio_00013] Reads the complete port via IDR.
 *          [SWS_Dio_00104] Undefined pins (port < 16-bit) are returned as 0.
 *          STM32H753 all ports are 16-bit so no masking needed.
 *
 * @param[in] PortId   ID of the DIO port (use DioConf_DioPort_PORTx)
 * @return    Dio_PortLevelType   Bit field of all channel levels
 */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/**
 * @brief   Sets the levels of all output channels of a port simultaneously.
 * @details [SWS_Dio_00136] Synchronous, Reentrant.
 *          [SWS_Dio_00034] Sets the port value.
 *          [SWS_Dio_00035][SWS_Dio_00108] Input channels remain unchanged;
 *          the implementation reads MODER and masks out input bits before
 *          writing to ODR.
 *          [SWS_Dio_00105] MSB bits beyond port width are ignored.
 *          Atomic via BSRR [SWS_Dio_00005].
 *
 * @param[in] PortId   ID of the DIO port
 * @param[in] Level    16-bit value to write; bit n → channel n of the port
 */
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/**
 * @brief   Reads a subset of adjoining bits (channel group) from a port.
 * @details [SWS_Dio_00137] Synchronous, Reentrant.
 *          [SWS_Dio_00037] Reads channel group subset.
 *          [SWS_Dio_00092] Applies mask.
 *          [SWS_Dio_00093] Shifts result right so values are LSB-aligned.
 *
 * @param[in] ChannelGroupIdPtr   Pointer to the channel group descriptor
 * @return    Dio_PortLevelType  LSB-aligned levels of the channel group
 */
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr);

/**
 * @brief   Sets an adjoining subset of DIO channels (channel group) to a level.
 * @details [SWS_Dio_00138] Synchronous, Reentrant.
 *          [SWS_Dio_00039] Sets the channel group.
 *          [SWS_Dio_00040] Remaining channels and input channels unchanged.
 *          [SWS_Dio_00090] Applies mask. [SWS_Dio_00091] Shifts value to align.
 *          Atomic via BSRR [SWS_Dio_00005].
 *
 * @param[in] ChannelGroupIdPtr   Pointer to the channel group descriptor
 * @param[in] Level               LSB-aligned value to write to the group
 */
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr,
                           Dio_PortLevelType           Level);

#if (DIO_VERSION_INFO_API == STD_ON)
/**
 * @brief   Returns version information of the DIO module.
 * @details [SWS_Dio_00139] Synchronous, Reentrant.
 *          [SWS_Dio_00189] Reports DIO_E_PARAM_POINTER if VersionInfo is NULL.
 *
 * @param[out] VersionInfo   Pointer to the structure to fill
 */
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo);
#endif

#if (DIO_FLIP_CHANNEL_API == STD_ON)
/**
 * @brief   Flips the level of a DIO channel and returns the new level.
 * @details [SWS_Dio_00190] Synchronous, Reentrant.
 *          [SWS_Dio_00191] For output channels: reads IDR, inverts bit,
 *          writes via BSRR, returns inverted level.
 *          [SWS_Dio_00192][SWS_Dio_00193] For input channels: no effect on
 *          output; returns current pin level.
 *          Atomic via BSRR [SWS_Dio_00005].
 *
 * @param[in] ChannelId   ID of the DIO channel
 * @return    Dio_LevelType   Level of the channel after flip
 */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);
#endif

#if (DIO_MASKED_WRITE_PORT_API == STD_ON)
/**
 * @brief   Sets the levels of selected output channels of a port via a mask.
 * @details [SWS_Dio_00300] Synchronous, Reentrant.
 *          [SWS_Dio_00200] Mask bit '1' → channel selected; bit '0' → unchanged.
 *          [SWS_Dio_00201][SWS_Dio_00203] Input channels remain unchanged.
 *          [SWS_Dio_00204] MSB beyond port width is ignored.
 *          Atomic via BSRR [SWS_Dio_00005].
 *
 * @param[in] PortId   ID of the DIO port
 * @param[in] Level    Levels to write to selected channels
 * @param[in] Mask     Bit mask of channels to affect
 */
void Dio_MaskedWritePort(Dio_PortType     PortId,
                         Dio_PortLevelType Level,
                         Dio_PortLevelType Mask);
#endif

/* ─── Channel Group Symbolic Names [SWS_Dio_00022] ───────────────────────── */
/*
 * Declared here (not in Dio_Cfg.h) because Dio_ChannelGroupType must be
 * fully defined before these externs are visible to consumers.
 * Definitions live in Dio_Cfg.c.
 */
extern const Dio_ChannelGroupType DioConf_DioChannelGroup_Example;

#ifdef __cplusplus
}
#endif

#endif /* DIO_H */