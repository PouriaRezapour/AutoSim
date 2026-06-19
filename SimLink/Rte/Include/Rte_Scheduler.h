/**
 * @file    Rte_Scheduler.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   RTE scheduler interface for periodic and background task execution.
 *
 * @details
 * This header provides the scheduler API for initializing and executing periodic
 * tasks at various time intervals (1ms, 5ms, 10ms, 20ms, 50ms, 100ms), as well
 * as background and Ethernet tasks. The scheduler orchestrates the execution of
 * all RTE runnables across different domains (HMI, Ethernet, etc.).
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * The scheduler must be initialized once at system startup via Rte_Scheduler_Init(),
 * and periodic functions should be invoked by the OS timer or RTOS at their
 * respective intervals.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h; add Module    |
 * |         |            |                  | Info and Version Info sections       |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_SCHEDULER_H
#define RTE_SCHEDULER_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define RTE_SCHEDULER_VENDOR_ID             0xFFFFu

/** AUTOSAR module identifier for the RTE module */
#define RTE_SCHEDULER_MODULE_ID             2u

/* ─── Version Info ────────────────────────────────────────────────────────── */

/** Software major version number */
#define RTE_SCHEDULER_SW_MAJOR_VERSION      0x00u

/** Software minor version number */
#define RTE_SCHEDULER_SW_MINOR_VERSION      0x02u

/** Software patch version number */
#define RTE_SCHEDULER_SW_PATCH_VERSION      0x03u

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* None */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/* None */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize the RTE scheduler.
 * @note   Must be called once at system startup before any periodic tasks.
 */
void Rte_Scheduler_Init(void);

/**
 * @brief  1ms periodic scheduler task.
 * @note   Should be invoked every 1 millisecond by the OS timer.
 */
void Rte_Scheduler_1ms(void);

/**
 * @brief  5ms periodic scheduler task.
 * @note   Should be invoked every 5 milliseconds by the OS timer.
 */
void Rte_Scheduler_5ms(void);

/**
 * @brief  10ms periodic scheduler task.
 * @note   Should be invoked every 10 milliseconds by the OS timer.
 */
void Rte_Scheduler_10ms(void);

/**
 * @brief  20ms periodic scheduler task.
 * @note   Should be invoked every 20 milliseconds by the OS timer.
 */
void Rte_Scheduler_20ms(void);

/**
 * @brief  50ms periodic scheduler task.
 * @note   Should be invoked every 50 milliseconds by the OS timer.
 */
void Rte_Scheduler_50ms(void);

/**
 * @brief  100ms periodic scheduler task.
 * @note   Should be invoked every 100 milliseconds by the OS timer.
 */
void Rte_Scheduler_100ms(void);

/**
 * @brief  Background scheduler task.
 * @note   Should be invoked in the main loop or lowest-priority task.
 */
void Rte_Scheduler_Background(void);

/**
 * @brief  Ethernet scheduler task.
 * @note   Should be invoked by the Ethernet task or thread.
 */
void Rte_Scheduler_Eth(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_SCHEDULER_H */
