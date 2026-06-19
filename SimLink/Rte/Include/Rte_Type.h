/**
 * @file    Rte_Type.h
 * @brief   Minimal RTE base type definitions.
 *
 * @details
 * This file provides the small set of RTE-level types and constants that
 * SWC code and Rte_<SwcName>.h headers rely on, but which are not part of
 * Std_Types.h / Platform_Types.h.
 *
 * It assumes Std_Types.h already defines (or includes Platform_Types.h
 * which defines) the base types used elsewhere in this project:
 *   boolean, uint8, uint16, uint32, sint8, sint16, sint32, float32,
 *   TRUE, FALSE
 *
 * If your existing Std_Types.h does NOT yet define Std_ReturnType or
 * RTE_E_OK / RTE_E_NOT_OK, this file fills that gap as well (guarded,
 * so it is safe even if they already exist elsewhere).
 *
 * Author      : Your Name
 * Date        : 20xx-xx-xx
 * Version     : x.x.x
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | x.x.x   | 20xx-xx-xx | Your Name  | Initial minimal stub                |
 * |---------|------------|------------|--------------------------------------|
 */

#ifndef RTE_TYPE_H
#define RTE_TYPE_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
 *  STANDARD RETURN TYPE  (guarded — skip if already defined in Std_Types.h)
 *============================================================================*/
#ifndef RTE_TYPE_STD_RETURNTYPE_DEFINED
#ifndef STD_RETURNTYPE_TYPEDEFD
typedef uint8 Std_ReturnType;
#endif
#endif

/*==============================================================================
 *  RTE RETURN / STATUS CODES  (guarded — skip if already defined elsewhere)
 *============================================================================*/
#ifndef RTE_E_OK
#define RTE_E_OK        ((Std_ReturnType)0x00u)   /* Request succeeded        */
#endif

#ifndef RTE_E_NOT_OK
#define RTE_E_NOT_OK    ((Std_ReturnType)0x01u)   /* Request failed (generic) */
#endif

#ifndef RTE_E_INVALID
#define RTE_E_INVALID   ((Std_ReturnType)0x02u)   /* Invalid data / parameter */
#endif

#ifndef RTE_E_LIMIT
#define RTE_E_LIMIT     ((Std_ReturnType)0x03u)   /* Value out of range      */
#endif

#ifndef RTE_E_NO_DATA
#define RTE_E_NO_DATA   ((Std_ReturnType)0x04u)   /* No data available yet   */
#endif

/*==============================================================================
 *  RTE INSTANCE HANDLE
 *============================================================================*/
typedef void* Rte_Instance;

#ifdef __cplusplus
}
#endif

#endif /* RTE_TYPE_H */
