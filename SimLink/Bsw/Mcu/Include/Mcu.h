/**
 * @file    Mcu.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-05-04
 * @version 0.1.2
 * @brief   System-level initialization interface for hardware and peripherals.
 *
 * @details
 * This header provides the low-level system initialization API for configuring
 * hardware components before application startup. It includes functions for:
 * - MPU (Memory Protection Unit) configuration
 * - CPU cache enablement
 * - System clock configuration
 * - Peripheral clock configuration
 * - Board Support Package (BSP) configuration
 * - Overall system initialization orchestration
 * 
 * These functions should be called in the correct sequence during early boot,
 * typically before any application-layer initialization.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef MCU_H
#define MCU_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

/* None */

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* None */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/* None */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Configure Memory Protection Unit (MPU).
 * @note   Should be called first in the initialization sequence.
 */
void Mcu_MpuCfg(void);

/**
 * @brief  Enable CPU instruction and data caches.
 * @note   Should be called after MPU configuration.
 */
void Mcu_CpuCacheEnable(void);

/**
 * @brief  Configure system clocks (PLL, HSE, etc.).
 * @note   Should be called after cache enablement.
 */
void Mpu_ClkCfg(void);

/**
 * @brief  Configure peripheral clocks.
 * @note   Should be called after system clock configuration.
 */
void Mpu_PeriphClkCfg(void);

/**
 * @brief  Configure Board Support Package (BSP) components.
 * @note   Should be called after peripheral clock configuration.
 */
void Mpu_BspCfg(void);

/**
 * @brief  Overall system initialization orchestrator.
 * @note   Calls all initialization functions in the correct sequence.
 *         This is the recommended entry point for system initialization.
 */
void Mpu_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* MCU_H */
