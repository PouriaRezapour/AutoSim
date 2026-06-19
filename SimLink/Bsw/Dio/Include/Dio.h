/**
 * @file    Dio.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Generic digital I/O driver interface.
 *
 * @details
 * Provides a hardware-independent API for digital I/O operations including
 * pin initialization, read, write, and toggle functions. This module wraps
 * the STM32 HAL GPIO functions for cleaner application-level access.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the project's    |
 * |         |            |                  | standard types defined in            |
 * |         |            |                  | Std_Types.h.                         |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef DIO_H
#define DIO_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "stm32h7xx_hal.h"
#include "Std_Types.h"

#include <stdbool.h>

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DIO_VENDOR_ID           0xFFFFu   /**< Project vendor identifier       */
#define DIO_MODULE_ID           120u      /**< AUTOSAR Dio module identifier   */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DIO_MAJOR_VERSION       0x00u     /**< Software major version          */
#define DIO_MINOR_VERSION       0x02u     /**< Software minor version          */
#define DIO_PATCH_VERSION       0x03u     /**< Software patch version          */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialize the digital I/O module.
 */
void Dio_Init(void);

/**
 * @brief  Write a digital state to a GPIO pin.
 * @param  port   Pointer to GPIO port (e.g., GPIOA).
 * @param  pin    GPIO pin number (e.g., GPIO_PIN_0).
 * @param  state  Desired state (true = HIGH, false = LOW).
 */
void Dio_WritePin(GPIO_TypeDef *port, uint16 pin, boolean state);

/**
 * @brief  Read the digital state of a GPIO pin.
 * @param  port  Pointer to GPIO port.
 * @param  pin   GPIO pin number.
 * @return true if pin is HIGH, false if LOW.
 */
boolean Dio_ReadPin(GPIO_TypeDef *port, uint16 pin);

/**
 * @brief  Toggle the state of a GPIO pin.
 * @param  port  Pointer to GPIO port.
 * @param  pin   GPIO pin number.
 */
void Dio_TogglePin(GPIO_TypeDef *port, uint16 pin);

#endif /* DIO_H */
