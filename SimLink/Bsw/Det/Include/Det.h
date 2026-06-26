/**
 * @file    Det.h
 * @brief   AUTOSAR Default Error Tracer (DET) interface
 * @details Stub header satisfying [SWS_Dio_00194] – Dio.c shall include
 *          Det.h if detection of development error (DET) is enabled.
 *          AUTOSAR CP R24-11
 */

#ifndef DET_H
#define DET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

/*===========================================================================*
 * MODULE IDENTIFICATION
 *===========================================================================*/
#define DET_VENDOR_ID       (60U)
#define DET_MODULE_ID       (15U)
#define DET_AR_MAJOR_VERSION (4U)
#define DET_AR_MINOR_VERSION (7U)
#define DET_AR_PATCH_VERSION (0U)

/*===========================================================================*
 * API
 *===========================================================================*/

/**
 * @brief   Service to report development errors.
 * @details As required by [SWS_Dio_00140]: Det_ReportError from Det.h.
 *
 * @param[in] ModuleId   Module ID of calling module
 * @param[in] InstanceId Instance ID of calling module
 * @param[in] ApiId      ID of API service in which the error occurred
 * @param[in] ErrorId    ID of the detected error
 * @return    Std_ReturnType  Always E_OK (implementation-defined)
 */
Std_ReturnType Det_ReportError(uint16 ModuleId,
                               uint8  InstanceId,
                               uint8  ApiId,
                               uint8  ErrorId);

#ifdef __cplusplus
}
#endif

#endif /* DET_H */
