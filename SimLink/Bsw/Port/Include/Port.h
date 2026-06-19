/**
 * @file    Port.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Port / GPIO initialisation interface.
 *
 * @details
 * Centralises all GPIO pin-mux and alternate-function assignments for the
 * project.  Currently covers FDCAN1 (PD0/PD1).  Additional peripherals are
 * added here as the project grows.
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.2.0   | 2025-05-23 | A.Rezapour | Initial release                      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | The module now uses the project's    |
 * |         |            |            | standard types defined in            |
 * |         |            |            | Std_Types.h.                         |
 * |---------|------------|------------|--------------------------------------|
 */

#ifndef PORT_H
#define PORT_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Mcal.h"
#include "Std_Types.h"

#include <stdint.h>

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define PORT_VENDOR_ID          0xFFFFu   /**< Project vendor identifier       */
#define PORT_MODULE_ID          124u      /**< AUTOSAR Port module identifier  */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define PORT_MAJOR_VERSION      0x00u     /**< Software major version          */
#define PORT_MINOR_VERSION      0x02u     /**< Software minor version          */
#define PORT_PATCH_VERSION      0x03u     /**< Software patch version          */

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* FDCAN1 pin definitions ─────────────────────────────── */
#define PORT_FDCAN1_GPIO_PORT       GPIOD
#define PORT_FDCAN1_RX_PIN          GPIO_PIN_0   /**< PD0 → FDCAN1_RX */
#define PORT_FDCAN1_TX_PIN          GPIO_PIN_1   /**< PD1 → FDCAN1_TX */
#define PORT_FDCAN1_AF              GPIO_AF9_FDCAN1

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief  Return status codes for the Port module.
 */
typedef enum {
    PORT_OK    =  0,  /**< Operation succeeded  */
    PORT_ERROR = -1   /**< Generic error         */
} PortStatus_t;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialise all GPIO pins required by the project.
 * @details Enables the necessary GPIO clocks and configures each pin's
 *          mode, speed, pull, and alternate function.
 * @return PORT_OK on success, PORT_ERROR on any HAL failure.
 */
PortStatus_t Port_Init(void);

/**
 * @brief  De-initialise all GPIO pins managed by this module.
 * @return PORT_OK on success.
 */
PortStatus_t Port_DeInit(void);

#endif /* PORT_H */
