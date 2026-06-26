/**
 * @file    DioIf.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   Digital I/O hardware abstraction layer — generic interface.
 *
 * @details
 * Provides a logical channel abstraction for digital I/O pins that are
 * common to all product variants (on-board controller LEDs, user button).
 *
 * Application-specific channels (valves, motor status, command signals, …)
 * are defined in the active DIO extension module and injected into the
 * DioIf_ChannelType enum via the DioExt indirection layer:
 *
 *     DioIf.h
 *       └── DioExt.h              (indirection — swap extension here)
 *             └── EscDioExt.h     (or any other active extension)
 *
 * Client code continues to use DioIf_Read() / DioIf_Write() /
 * DioIf_Toggle() with the same channel names (e.g. HWAB_DIO_SIGNAL_VALVE11)
 * as before.  No call sites need to change when the extension is switched.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | The three on-board controller LEDs   |
 * |         |            |                  | are now listed                       |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the project's    |
 * |         |            |                  | standard types defined in            |
 * |         |            |                  | Std_Types.h.                         |
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Migrated to AUTOSAR-compliant Dio    |
 * |         |            |                  | driver. PinConfig now uses           |
 * |         |            |                  | Dio_ChannelType instead of raw       |
 * |         |            |                  | GPIO_TypeDef port/pin pairs.         |
 * |         |            |                  | Dio_ReadPin/WritePin/TogglePin       |
 * |         |            |                  | replaced with Dio_ReadChannel,       |
 * |         |            |                  | Dio_WriteChannel, Dio_FlipChannel.   |
 * |         |            |                  | Separated generic and                |
 * |         |            |                  | application-specific channels.       |
 * |         |            |                  | ESC signals (valves, commands,       |
 * |         |            |                  | LEDs) moved to EscDioExt.            |
 * |         |            |                  | Extension injected via DioExt.h.     |
 * |         |            |                  | DioIf is now extension-agnostic.     |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef DIOIF_H
#define DIOIF_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Dio.h"
#include "Dio_Cfg.h"
#include "Std_Types.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DIOIF_VENDOR_ID      0xFFFFu   /**< Project vendor identifier       */
#define DIOIF_MODULE_ID      120u      /**< AUTOSAR Dio module identifier   */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DIOIF_MAJOR_VERSION  0x00u     /**< Software major version          */
#define DIOIF_MINOR_VERSION  0x04u     /**< Software minor version          */
#define DIOIF_PATCH_VERSION  0x00u     /**< Software patch version          */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief Pin configuration structure.
 *
 * @details Uses the AUTOSAR Dio_ChannelType (PortIndex * 16 + PinNumber) to
 *          identify the physical pin, removing the dependency on raw
 *          GPIO_TypeDef pointers and HAL pin bitmasks.
 */
typedef struct
{
    Dio_ChannelType channelId;      /**< AUTOSAR DIO channel ID (from Dio_Cfg.h) */
    boolean         isOutput;       /**< Pin direction (TRUE = output)            */
    boolean         debounceStatus; /**< Debounce enabled for this input          */
} Dio_PinConfig;

/**
 * @brief Logical DIO channel enumeration.
 *
 * @details The enum is assembled in two parts:
 *
 *   Part 1 — Generic channels (this file):
 *     Hardware present on every board variant: user button and controller LEDs.
 *
 *   Part 2 — Extension channels (injected via DioExt.h → EscDioExt.h):
 *     Application-specific channels such as valves, motor status, and command
 *     outputs.  Changing the active extension in DioExt.h replaces this section
 *     without any modification to DioIf.h or client call sites.
 *
 *   The sentinel HWAB_DIO_CHANNEL_COUNT always appears last, giving the total
 *   number of channels regardless of which extension is active.
 */
typedef enum
{
    /* ── Part 1: Generic channels ─────────────────────────────────────────
     *
     * Channels present on every board variant, independent of which
     * application extension is active.
     */


    /* ── Part 2: Extension channels ──────────────────────────────────────
     *
     * Injected by the active extension via DioExt.h.  Do not add
     * application-specific channels here; add them to the appropriate
     * extension module instead.
     */
#include "DioExt.h"

    HWAB_DIO_CHANNEL_COUNT      /**< Total channel count — always last        */
} DioIf_ChannelType;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialize the DIO hardware abstraction layer.
 */
void DioIf_Init(void);

/**
 * @brief  Read the state of a logical DIO channel.
 * @param  ch  Logical channel identifier.
 * @return TRUE if channel is HIGH, FALSE if LOW.
 */
boolean DioIf_Read(DioIf_ChannelType ch);

/**
 * @brief  Write a state to a logical DIO channel.
 * @param  ch     Logical channel identifier.
 * @param  state  Desired state (TRUE = HIGH, FALSE = LOW).
 */
void DioIf_Write(DioIf_ChannelType ch, boolean state);

/**
 * @brief  Toggle the state of a logical DIO channel.
 * @param  ch  Logical channel identifier.
 */
void DioIf_Toggle(DioIf_ChannelType ch);

#endif /* DIOIF_H */
