/**
 * @file    Os_TasksCfg.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   RTOS task configuration header.
 *
 * @details
 * This header provides compile-time configuration for all RTOS cyclic tasks.
 * Each task can be individually enabled/disabled and configured with specific
 * priority, stack size, and execution period.
 *
 * Task configuration parameters:
 * - OS_TASK_*_ENABLED: Enable (1) or disable (0) the task
 * - OS_TASK_*_PRIORITY: CMSIS-RTOS priority level
 * - OS_TASK_*_STACK: Stack size in words
 * - OS_TASK_*_DELAY: Execution period in milliseconds
 *
 * Current task configuration:
 * - ETH: Normal priority, 2× minimal stack
 * - 1MS: Realtime priority, 1ms period
 * - 5MS: High priority, 5ms period
 * - 10MS: Above normal priority, 10ms period
 * - 20MS: Normal priority, 20ms period
 * - 50MS: Below normal priority, 50ms period
 * - 100MS: Low priority, 100ms period
 * - BACKGROUND: Idle priority, 100ms period
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

#ifndef OS_TASKSCONFIG_H
#define OS_TASKSCONFIG_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "Std_Types.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define OS_VENDOR_ID            0xFFFFu   /**< Project vendor identifier       */
#define OS_MODULE_ID            1u        /**< AUTOSAR Os module identifier    */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define OS_MAJOR_VERSION        0x00u     /**< Software major version          */
#define OS_MINOR_VERSION        0x02u     /**< Software minor version          */
#define OS_PATCH_VERSION        0x03u     /**< Software patch version          */

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/**
 * @brief  Default stack size calculation macro.
 * @param  config  Stack size configuration value.
 * @return Calculated stack size in words.
 */
#define OS_TASK_DEFAULT_STACK(config) (config)

/* ─── Ethernet Task Configuration ─────────────────────────────────────────── */

#define OS_TASK_ETH_ENABLED         1
#define OS_TASK_ETH_PRIORITY        osPriorityNormal
#define OS_TASK_ETH_STACK           OS_TASK_DEFAULT_STACK(2 * configMINIMAL_STACK_SIZE)

/* ─── 1ms Task Configuration ──────────────────────────────────────────────── */

#define OS_TASK_1MS_ENABLED         1
#define OS_TASK_1MS_PRIORITY        osPriorityRealtime
#define OS_TASK_1MS_STACK           OS_TASK_DEFAULT_STACK(configMINIMAL_STACK_SIZE)
#define OS_TASK_1MS_DELAY           1

/* ─── 5ms Task Configuration ──────────────────────────────────────────────── */

#define OS_TASK_5MS_ENABLED         1
#define OS_TASK_5MS_PRIORITY        osPriorityHigh
#define OS_TASK_5MS_STACK           OS_TASK_DEFAULT_STACK(configMINIMAL_STACK_SIZE)
#define OS_TASK_5MS_DELAY           5

/* ─── 10ms Task Configuration ─────────────────────────────────────────────── */

#define OS_TASK_10MS_ENABLED        1
#define OS_TASK_10MS_PRIORITY       osPriorityAboveNormal
#define OS_TASK_10MS_STACK          OS_TASK_DEFAULT_STACK(configMINIMAL_STACK_SIZE)
#define OS_TASK_10MS_DELAY          10

/* ─── 20ms Task Configuration ─────────────────────────────────────────────── */

#define OS_TASK_20MS_ENABLED        1
#define OS_TASK_20MS_PRIORITY       osPriorityNormal
#define OS_TASK_20MS_STACK          OS_TASK_DEFAULT_STACK(configMINIMAL_STACK_SIZE)
#define OS_TASK_20MS_DELAY          20

/* ─── 50ms Task Configuration ─────────────────────────────────────────────── */

#define OS_TASK_50MS_ENABLED        1
#define OS_TASK_50MS_PRIORITY       osPriorityBelowNormal
#define OS_TASK_50MS_STACK          OS_TASK_DEFAULT_STACK(configMINIMAL_STACK_SIZE)
#define OS_TASK_50MS_DELAY          50

/* ─── 100ms Task Configuration ────────────────────────────────────────────── */

#define OS_TASK_100MS_ENABLED       1
#define OS_TASK_100MS_PRIORITY      osPriorityLow
#define OS_TASK_100MS_STACK         OS_TASK_DEFAULT_STACK(configMINIMAL_STACK_SIZE)
#define OS_TASK_100MS_DELAY         100

/* ─── Background Task Configuration ───────────────────────────────────────── */

#define OS_TASK_BACKGROUND_ENABLED  1
#define OS_TASK_BACKGROUND_PRIORITY osPriorityIdle
#define OS_TASK_BACKGROUND_STACK    OS_TASK_DEFAULT_STACK(configMINIMAL_STACK_SIZE)
#define OS_TASK_BACKGROUND_DELAY    100

#endif /* OS_TASKSCONFIG_H */
