/**
 * @file    DioExt.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6 
 * @brief   DIO extension indirection layer — pin configuration fragment selector.
 *
 * @details
 * DioExt.c is the source-side twin of DioExt.h.  It is included (not
 * compiled independently) by DioIf.c *inside* the pinConfig[] array
 * initialiser body to inject the active extension's designated-initialiser
 * rows.
 *
 * ── Switching extensions ───────────────────────────────────────────────────
 *
 * Mirror whatever change was made in DioExt.h:
 *   Replace the EscDioExt include below with the new extension's .c fragment.
 *
 * ── Fragment protocol ──────────────────────────────────────────────────────
 *
 * This file defines DIO_EXT_CFG_INTERNAL before including the extension
 * fragment, so the extension can verify it is not being compiled standalone.
 * The guard is cleared afterwards so it does not persist in the including TU.
 *
 * The entire active content is wrapped in #ifdef DIO_HwAb_C_INTERNAL so
 * that when the build system compiles this file as a standalone translation
 * unit (which produces no useful object) it becomes an empty TU instead of
 * emitting errors.  DioIf.c defines DIO_HwAb_C_INTERNAL immediately
 * before #including this file.
 *
 * @note  Do NOT add this file to the build system as a standalone translation
 *        unit.  It must be #included by DioIf.c only.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifdef DIO_HwAb_C_INTERNAL

/* ── Active extension pin-config selector ─────────────────────────────────
 *
 * Define the authorisation guard, pull in the pinConfig fragment, then
 * clear the guard so it does not persist in DioIf.c's scope.
 */
#define DIO_EXT_CFG_INTERNAL
#include "EscDioExt.c"   /* ← swap this line to change extension */
#undef  DIO_EXT_CFG_INTERNAL

#endif /* DIO_HwAb_C_INTERNAL */
