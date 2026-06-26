/**
 * @file    Os_CyclicTasks.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Cyclic task function declarations.
 *
 * @details
 * This header declares the individual task thread functions that are
 * executed periodically by the RTOS scheduler. Each task function is
 * conditionally declared based on Os_TasksCfg.h configuration.
 *
 * Available cyclic tasks:
 * - Ethernet task (Os_Task_Eth)
 * - 1ms high-priority task (Os_Task_1ms)
 * - 5ms task (Os_Task_5ms)
 * - 10ms task (Os_Task_10ms)
 * - 20ms task (Os_Task_20ms)
 * - 50ms task (Os_Task_50ms)
 * - 100ms task (Os_Task_100ms)
 * - Background idle task (Os_Task_Background)
 *
 * Each task handle and function prototype is conditionally compiled based
 * on the corresponding OS_TASK_*_ENABLED flag in Os_TasksCfg.h.
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

#ifndef OS_CYCLIC_TASKS_H
#define OS_CYCLIC_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Os_TasksCfg.h"
#include "Std_Types.h"
#include "cmsis_os.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define OS_VENDOR_ID            0xFFFFu   /**< Project vendor identifier       */
#define OS_MODULE_ID            1u        /**< AUTOSAR Os module identifier    */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define OS_MAJOR_VERSION        0x00u     /**< Software major version          */
#define OS_MINOR_VERSION        0x02u     /**< Software minor version          */
#define OS_PATCH_VERSION        0x03u     /**< Software patch version          */

/* ─── Task Handle Declarations ────────────────────────────────────────────── */

#if (OS_TASK_ETH_ENABLED == 1)
extern osThreadId Os_EthThreadHandle;
#endif

#if (OS_TASK_1MS_ENABLED == 1)
extern osThreadId Os_Task1msThreadHandle;
#endif

#if (OS_TASK_5MS_ENABLED == 1)
extern osThreadId Os_Task5msThreadHandle;
#endif

#if (OS_TASK_10MS_ENABLED == 1)
extern osThreadId Os_Task10msThreadHandle;
#endif

#if (OS_TASK_20MS_ENABLED == 1)
extern osThreadId Os_Task20msThreadHandle;
#endif

#if (OS_TASK_50MS_ENABLED == 1)
extern osThreadId Os_Task50msThreadHandle;
#endif

#if (OS_TASK_100MS_ENABLED == 1)
extern osThreadId Os_Task100msThreadHandle;
#endif

#if (OS_TASK_BACKGROUND_ENABLED == 1)
extern osThreadId Os_TaskBackgroundThreadHandle;
#endif

/* ─── Task Function Prototypes ────────────────────────────────────────────── */

#if (OS_TASK_ETH_ENABLED == 1)
/**
 * @brief  Ethernet task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_Eth(void const * argument);
#endif

#if (OS_TASK_BACKGROUND_ENABLED == 1)
/**
 * @brief  Background idle task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_Background(void const * argument);
#endif

#if (OS_TASK_1MS_ENABLED == 1)
/**
 * @brief  1ms cyclic task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_1ms(void const * argument);
#endif

#if (OS_TASK_5MS_ENABLED == 1)
/**
 * @brief  5ms cyclic task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_5ms(void const * argument);
#endif

#if (OS_TASK_10MS_ENABLED == 1)
/**
 * @brief  10ms cyclic task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_10ms(void const * argument);
#endif

#if (OS_TASK_20MS_ENABLED == 1)
/**
 * @brief  20ms cyclic task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_20ms(void const * argument);
#endif

#if (OS_TASK_50MS_ENABLED == 1)
/**
 * @brief  50ms cyclic task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_50ms(void const * argument);
#endif

#if (OS_TASK_100MS_ENABLED == 1)
/**
 * @brief  100ms cyclic task thread function.
 * @param  argument  Thread argument (unused).
 */
void Os_Task_100ms(void const * argument);
#endif

#ifdef __cplusplus
}
#endif

#endif /* OS_CYCLIC_TASKS_H */
