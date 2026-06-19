/**
 * @file    SimLink.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-05-04
 * @version 0.1.2
 * @brief   Main application interface for SimLink HIL platform.
 *
 * @details
 * This header provides the top-level public API for the SimLink (Sensor Simulator)
 * Hardware-in-the-Loop platform. It encapsulates initialization, periodic execution,
 * and event handling for the entire application core.
 * 
 * Responsibilities include:
 * - Initialize all subsystems (RTE, scheduler, communication layers)
 * - Execute periodic processing via the main run loop
 * - Handle PC-to-DUT communication over CAN, SPI, and Ethernet
 * - Provide hooks for data input/output between PC and embedded target
 * 
 * This file is part of the application layer and is user-editable for
 * project-specific customization.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef SIMLINK_H_
#define SIMLINK_H_

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include <stdint.h>
#include <stdbool.h>

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* None */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/* None */

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Initialize the SimLink HIL platform.
 * @note   Must be called once at system startup before SimLink_Run().
 *         Initializes all subsystems including RTE, scheduler, and communication.
 */
void SimLink_Init(void);

/**
 * @brief  Main run loop for SimLink HIL platform.
 * @note   Should be called continuously in the main loop or RTOS task.
 *         Executes periodic processing and handles communication events.
 */
void SimLink_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* SimLink_H_ */
