/**
 * @file    Can.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Public interface for the CAN driver module.
 *
 * @details
 * Exposes initialisation, start, single-frame transmit, and receive
 * functions for the FDCAN peripheral operating in Classic CAN mode on
 * the STM32H7.  Clock configuration is intentionally excluded; it is
 * handled by the project-level SystemClock_Config().
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * Typical usage:
 *   1. Call Can_Init()            – configures the FDCAN peripheral.
 *   2. Call Can_Start()           – starts the FDCAN module.
 *   3. Call Can_Transmit()        – enqueue a single frame via Tx FIFO.
 *      Call Can_Receive()         – poll for a received frame.
 *   4. Call Can_DeInit()          – releases resources when done.
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.2.0   | 2025-05-23 | A.Rezapour | Initial release                      |
 * | 0.2.2   | 2025-06-01 | A.Rezapour | Add Can_TransmitBurst prototype      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | Integrate Std_Types.h; add Module    |
 * |         |            |            | Info and Version Info sections       |
 * |---------|------------|------------|--------------------------------------|
 */

#ifndef CAN_H
#define CAN_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Mcal.h"
#include "Can_HwAb.h"

#include <stdint.h>
#include <stddef.h>

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define CAN_VENDOR_ID           0xFFFFu

/** AUTOSAR module identifier for the CAN driver */
#define CAN_MODULE_ID           80u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define CAN_SW_MAJOR_VERSION    0x00u

/** Software minor version number */
#define CAN_SW_MINOR_VERSION    0x02u

/** Software patch version number */
#define CAN_SW_PATCH_VERSION    0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/** Maximum data bytes in a Classic CAN frame */
#define CAN_MAX_DATA_BYTES      8U

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief Return status codes used across the CAN module.
 */
typedef enum {
    CAN_OK           =  0,   /**< Operation succeeded                  */
    CAN_ERROR        = -1,   /**< Generic error                        */
    CAN_NOT_INIT     = -2,   /**< Module not yet initialised           */
    CAN_ALREADY_INIT = -3,   /**< Module already initialised           */
    CAN_TX_PENDING   = -4,   /**< Previous transmission still pending  */
    CAN_RX_EMPTY     = -5    /**< No message available in Rx buffer    */
} CanStatus_t;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialise the FDCAN peripheral in Classic CAN mode.
 * @note   Does not configure clocks; assumes SystemClock_Config() already ran.
 * @return CAN_OK on success, CAN_ERROR on HAL failure,
 *         CAN_ALREADY_INIT if called more than once without DeInit.
 */
CanStatus_t Can_Init(void);

/**
 * @brief  Configure reception filters and start the FDCAN module.
 * @note   Must be called after Can_Init().
 * @return CAN_OK on success, CAN_NOT_INIT if module not initialised,
 *         CAN_ERROR on HAL failure.
 */
CanStatus_t Can_Start(void);

/**
 * @brief  Transmit a single Classic CAN frame via the Tx FIFO queue.
 * @param  pTxHeader  Pointer to a populated CanTxHeader_t descriptor.
 * @param  pData      Pointer to the payload byte array (max CAN_MAX_DATA_BYTES).
 * @return CAN_OK on success, CAN_NOT_INIT, CAN_TX_PENDING (FIFO full), or CAN_ERROR.
 */
CanStatus_t Can_Transmit(const CanTxHeader_t *pTxHeader, const uint8 *pData);

/**
 * @brief  Poll for a received frame and copy it out of Rx buffer 0.
 * @param  pRxHeader  Pointer to a CanRxHeader_t to be filled on success.
 * @param  pData      Pointer to a byte array of at least CAN_MAX_DATA_BYTES.
 * @return CAN_OK when a frame is available and retrieved,
 *         CAN_RX_EMPTY when no frame is ready, CAN_NOT_INIT or CAN_ERROR otherwise.
 */
CanStatus_t Can_Receive(CanRxHeader_t *pRxHeader, uint8 *pData);

/**
 * @brief  Build and return a Tx header for a Classic CAN data frame.
 * @param  identifier   11-bit standard CAN identifier.
 * @param  dataLength   Data Length Code (e.g., FDCAN_DLC_BYTES_8).
 * @return Populated CanTxHeader_t ready to pass to Can_Transmit().
 */
CanTxHeader_t Can_BuildTxHeader(uint32 identifier, uint32 dataLength);

/**
 * @brief  Stop the FDCAN module and de-initialise the peripheral.
 * @return CAN_OK on success, CAN_NOT_INIT if module was not running.
 */
CanStatus_t Can_DeInit(void);

#endif /* CAN_H */
