/**
 * @file    Det.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-22
 * @version 0.1.0
 * @brief   AUTOSAR Default Error Tracer (DET) – stub implementation.
 *
 * @details
 * Provides a linkable body for Det_ReportError as required by
 * [SWS_Dio_00194]. This is a minimal stub: it satisfies the linker and
 * can be extended later to forward errors to a UART logger, a fault
 * register, or a full DET ring-buffer when needed.
 *
 * To hook in real error handling, replace the body of Det_ReportError
 * below (or call a board-specific logging function from inside it).
 *
 * @par Revision History:
 * |---------|------------|------------------|-------------------------------|
 * | Version | Date       | Author           | Description                   |
 * |---------|------------|------------------|-------------------------------|
 * | 0.1.0   | 2025-06-22 | A.Rezapour       | Initial stub release          |
 * |---------|------------|------------------|-------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Det.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief   Report a development error.
 *
 * @details Stub implementation – absorbs the error and returns E_OK so
 *          that callers (e.g. Dio_ReadChannel, Dio_WriteChannel) compile
 *          and link cleanly. Extend this function when a logging or fault
 *          management back-end is available.
 *
 * @param[in] ModuleId    Module ID of the reporting module.
 * @param[in] InstanceId  Instance ID of the reporting module.
 * @param[in] ApiId       Service ID in which the error was detected.
 * @param[in] ErrorId     ID of the detected development error.
 * @return    E_OK        Always (per AUTOSAR stub convention).
 */
Std_ReturnType Det_ReportError(uint16 ModuleId,
                               uint8  InstanceId,
                               uint8  ApiId,
                               uint8  ErrorId)
{
    /* Suppress unused-parameter warnings until a real handler is wired in */
    (void)ModuleId;
    (void)InstanceId;
    (void)ApiId;
    (void)ErrorId;

    return E_OK;
}
