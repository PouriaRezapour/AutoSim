/**
 * @file    EscDioExt.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   ESC-specific DIO extension — channel enumeration fragment.
 *
 * @details
 * This file is an *extension fragment* intended to be included exclusively
 * by DioExt.h (never directly by application code).  It contributes the
 * ESC-application-specific DIO channels to the master channel enumeration
 * that lives in DioIf.h.
 *
 * The enum fragment pattern works as follows:
 *
 *   1. DioIf.h opens the DioIf_ChannelType enum and defines all
 *      generic channels.
 *   2. At the end of the generic section DioIf.h includes DioExt.h.
 *   3. DioExt.h includes EscDioExt.h (or any other active extension).
 *   4. EscDioExt.h appends its application-specific enumerators.
 *   5. DioIf.h closes the enum and adds the sentinel HWAB_DIO_CHANNEL_COUNT.
 *
 * This gives client code a single, contiguous DioIf_ChannelType enum
 * whose numeric values are stable and extension-specific channels remain
 * fully addressable through DioIf_Read() / Write() / Toggle().
 *
 * @warning This file has NO include guard by design.  It is a raw C fragment
 *          pasted inside an open enum body — adding #ifndef/#endif around it
 *          would inject preprocessor directives into enum syntax.  The
 *          DIO_EXT_INTERNAL check below is the only guard needed.
 *
 * @note  Do NOT include this file directly.  Include DioExt.h or, for the
 *        full API, DioIf.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ── Guard: only allow inclusion from DioExt.h ─────────────────────────────
 *
 * DioExt.h defines DIO_EXT_INTERNAL before including this file.
 * Any other include path is a configuration error.
 */
#ifndef DIO_EXT_INTERNAL
    #error "EscDioExt.h must only be included via DioExt.h"
#endif

/*
 * ── Enum fragment ──────────────────────────────────────────────────────────
 *
 * These enumerators are injected into the DioIf_ChannelType enum body
 * (see DioIf.h).  There is intentionally no surrounding enum { } here
 * and no #ifndef/#endif include guard — both would break the injection.
 */

    /* ── Inputs ─── */

    /* Motor */
    HWAB_DIO_MOTOR_STATUS,

    /* Valves — status inputs from ESC valve controller */
    HWAB_DIO_SIGNAL_VALVE_ON,
    HWAB_DIO_SIGNAL_VALVE1,
    HWAB_DIO_SIGNAL_VALVE2,
    HWAB_DIO_SIGNAL_VALVE3,
    HWAB_DIO_SIGNAL_VALVE4,
    HWAB_DIO_SIGNAL_VALVE5,
    HWAB_DIO_SIGNAL_VALVE6,
    HWAB_DIO_SIGNAL_VALVE7,
    HWAB_DIO_SIGNAL_VALVE8,
    HWAB_DIO_SIGNAL_VALVE9,
    HWAB_DIO_SIGNAL_VALVE10,
    HWAB_DIO_SIGNAL_VALVE11,
    HWAB_DIO_SIGNAL_VALVE12,

    /* ── Outputs ─── */

    /* Command outputs to ESC sub-systems */
    HWAB_DIO_CMD_VESC,
    HWAB_DIO_CMD_BLS,
    HWAB_DIO_CMD_ITPMS,
    HWAB_DIO_CMD_PARK_BRAKE,
    HWAB_DIO_CMD_ESC_TCS,
    HWAB_DIO_CMD_WAKEUP,
    HWAB_DIO_CMD_VMOT,

    /* Controller LEDs */
    HWAB_DIO_LED1,
    HWAB_DIO_LED2,
    HWAB_DIO_LED3,

    /* HMI */
    HWAB_DIO_BLUE_USER_BUTTON,  /**< On-board blue user button (PC13)         */
