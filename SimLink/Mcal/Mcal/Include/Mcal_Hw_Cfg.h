/**
 * @file    Mcal_Hw_Config.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   STM32H7xx Nucleo board configuration file.
 *
 * @details
 * Board-level configuration for the STM32H7xx Nucleo family. Selects the
 * target Nucleo variant, enables or disables BSP features, and sets
 * peripheral and interrupt-priority defaults.
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.2.3   | 2025-06-07 | A.Rezapour    | Reformatted to project template style|
 * |---------|------------|---------------|--------------------------------------|
 */

#ifndef MCAL_HW_CONFIG
#define MCAL_HW_CONFIG

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "stm32h7xx_hal.h"

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/**
 * @brief Nucleo board variant selection.
 *        Define the physical form-factor and part number in use.
 */
#define USE_NUCLEO_144
#define USE_NUCLEO_H743ZI2

/**
 * @brief COM / UART logging feature switches.
 *        Set USE_COM_LOG to 1U to route BSP log output over a COM port.
 *        Set USE_BSP_COM_FEATURE to 0U to exclude COM support entirely.
 */
#define USE_COM_LOG          0U
#define USE_BSP_COM_FEATURE  1U

/**
 * @brief IRQ priority for the user button EXTI line.
 */
#define BSP_BUTTON_USER_IT_PRIORITY  15U

/**
 * @brief SPI1 bus clock frequency in Hz.
 */
#define BUS_SPI1_BAUDRATE  18000000

#ifdef __cplusplus
}
#endif

#endif /* MCAL_HW_CONFIG */