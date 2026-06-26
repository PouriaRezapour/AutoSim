/**
 * @file    DioExt.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   DIO extension indirection layer — enum fragment selector.
 *
 * @details
 * DioExt.h is the single switch-point that selects which application-specific
 * DIO extension is active for the current build.  DioIf.h includes this
 * file (and only this file) to pull in extension-specific channel names.
 *
 * ── Switching extensions ───────────────────────────────────────────────────
 *
 * To activate a different extension (e.g. "FooBarDioExt") instead of the
 * default ESC extension:
 *
 *   1. Create FooBarDioExt/FooBarDioExt.h (enum fragment) and
 *      FooBarDioExt/FooBarDioExt.c (pinConfig fragment) following the
 *      EscDioExt pattern.
 *   2. In this file, replace the EscDioExt include with FooBarDioExt.h.
 *   3. In DioExt.c, replace the EscDioExt include with FooBarDioExt.c.
 *   4. Rebuild — no other file needs to change.
 *
 * ── Include guard / fragment protocol ─────────────────────────────────────
 *
 * This header is included *inside an open enum body* in DioIf.h.
 * It therefore defines DIO_EXT_INTERNAL so the extension header knows it
 * is being included legitimately, then immediately un-defines it so the
 * guard cannot leak to other translation units.
 *
 * @note  Do NOT include this file directly from application code.
 *        Use DioIf.h for the full API.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef DIO_EXT_H
#define DIO_EXT_H

/* ── Active extension selector ────────────────────────────────────────────
 *
 * Set the guard that authorises the extension header to be included, pull
 * in the fragment, then clear the guard.  Only one extension may be active
 * at a time.
 */
#define DIO_EXT_INTERNAL
#include "EscDioExt.h"   /* ← swap this line to change extension */
#undef  DIO_EXT_INTERNAL

#endif /* DIO_EXT_H */
