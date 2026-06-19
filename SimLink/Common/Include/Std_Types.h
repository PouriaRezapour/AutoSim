/**
 * @file    Std_Types.h
 * @author  Ali Rezapour
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   AUTOSAR Standard Types definitions.
 *
 * @details
 * Provides the AUTOSAR-mandated platform-independent type definitions,
 * symbol definitions, and the NULL_PTR define required by all BSW modules.
 * Conforms to AUTOSAR SWS_Std_00006 (Std_ReturnType) and
 * SWS_Std_00007 (Std_VersionInfoType).
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.2.3   | 2025-06-07 | Ali Rezapour  | Initial release                      |
 * |---------|------------|---------------|--------------------------------------|
 */

#ifndef STD_TYPES_H
#define STD_TYPES_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Platform_Types.h"
#include <stdint.h>

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/*
 * Std_ReturnType
 * AUTOSAR SWS_Std_00006
 * Range: 0x00 (E_OK) .. 0x3F (implementation-defined upper bound).
 * The standard mandates an 8-bit unsigned type.
 */
typedef uint8 Std_ReturnType;

/*
 * Std_VersionInfoType
 * AUTOSAR SWS_Std_00007
 * Used by Xxx_GetVersionInfo() APIs to report module version data.
 */
typedef struct
{
    uint16 vendorID;           /* AUTOSAR vendor identifier                 */
    uint16 moduleID;           /* AUTOSAR module identifier                 */
    uint8  sw_major_version;   /* Software major version number             */
    uint8  sw_minor_version;   /* Software minor version number             */
    uint8  sw_patch_version;   /* Software patch version number             */
} Std_VersionInfoType;

/* ─── Symbol Definitions ──────────────────────────────────────────────────── */

/*
 * E_OK / E_NOT_OK — OSEK-safe guard block.
 * STATUSTYPEDEFINED is set by the OSEK OS header when it defines E_OK.
 * This guard prevents a duplicate-definition clash at compile time.
 */
#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
typedef unsigned char StatusType;   /* OSEK compliance */
#define E_OK        0x00u           /* Operation succeeded                    */
#endif /* STATUSTYPEDEFINED */
#define E_NOT_OK    0x01u           /* Operation failed                       */

/* Physical / logical level symbols */
#define STD_HIGH    0x01u           /* Physical state: voltage level high     */
#define STD_LOW     0x00u           /* Physical state: voltage level low      */

#define STD_ACTIVE  0x01u           /* Logical state active                   */
#define STD_IDLE    0x00u           /* Logical state idle                     */

#define STD_ON      0x01u           /* Feature / function enabled             */
#define STD_OFF     0x00u           /* Feature / function disabled            */

/* ─── NULL Pointer Constant ───────────────────────────────────────────────── */

#define NULL_PTR    ((void *)0)     /* Null pointer constant (AUTOSAR)        */

#endif /* STD_TYPES_H */
