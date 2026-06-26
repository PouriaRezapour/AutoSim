/**
 * @file    Os.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Operating System Abstraction Layer interface.
 *
 * @details
 * This header provides the main OS lifecycle functions for the SenSim HIL
 * platform. It abstracts the RTOS initialization and scheduler startup,
 * providing a clean interface for the application entry point.
 *
 * The module exposes two primary functions:
 * - Os_TasksCreate(): Creates all enabled RTOS tasks based on configuration
 * - Os_Run(): Initializes RTE schedulers and starts the RTOS kernel
 *
 * Os_Run() is the "Point of No Return" — under normal operation, it never
 * returns to the caller as the scheduler takes control of the CPU and begins
 * executing the highest priority ready task.
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

#ifndef OS_H
#define OS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define OS_VENDOR_ID            0xFFFFu   /**< Project vendor identifier       */
#define OS_MODULE_ID            1u        /**< AUTOSAR Os module identifier    */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define OS_MAJOR_VERSION        0x00u     /**< Software major version          */
#define OS_MINOR_VERSION        0x02u     /**< Software minor version          */
#define OS_PATCH_VERSION        0x03u     /**< Software patch version          */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initializes and creates all enabled RTOS tasks.
 *
 * @details
 * This function iterates through the task configuration and uses the RTOS
 * API to allocate memory (stack/TCB) and instantiate the threads. This
 * should be called after hardware initialization but before starting the
 * scheduler.
 *
 * @note   If a task fails to create, the system behavior depends on the
 *         error handling implementation (e.g., local assert or error log).
 */
void Os_TasksCreate(void);

/**
 * @brief  Initializes and starts the RTOS scheduler.
 *
 * @details
 * This function performs the following operations:
 * 1. Initializes all RTE schedulers (Rte_Scheduler_Init)
 * 2. Creates all enabled RTOS tasks (Os_TasksCreate)
 * 3. Starts the RTOS kernel scheduler (osKernelStart)
 *
 * @warning This function is the "Point of No Return". Under normal operation,
 *          it will never return to the caller (main). The scheduler takes
 *          control of the CPU and begins executing the highest priority
 *          ready task.
 *
 * @note    This function should be called after all hardware and peripheral
 *          initialization is complete.
 */
void Os_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* OS_H */
