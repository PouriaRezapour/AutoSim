/**
 * @file    DioIf.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   DIO Hardware Abstraction Layer implementation.
 *
 * @details
 * Provides board-specific GPIO channel mapping and runtime access to all
 * digital input/output channels via the AUTOSAR-compliant DIO driver.
 *
 * Each logical channel (DioIf_ChannelType) is bound to an AUTOSAR
 * Dio_ChannelType ID (defined in Dio_Cfg.h) using the encoding:
 *     ChannelId = (PortIndex * 16) + PinNumber
 * where PortIndex: A=0, B=1, C=2, D=3, E=4, F=5, G=6.
 *
 * Input channels with debouncing enabled use a counter-based algorithm
 * (threshold: BTN_DEBOUNCE_COUNT) to filter glitches before reporting a
 * stable state. Output channels support direct write and toggle operations.
 *
 * AUTOSAR layering
 * ----------------
 * DioIf sits above the DIO driver and below the application.  It must NOT
 * access MCAL (HAL/LL) or hardware registers directly.  All GPIO operations
 * are performed exclusively through the AUTOSAR DIO API:
 *   Dio_ReadChannel(), Dio_WriteChannel(), Dio_FlipChannel().
 *
 * Pin direction and GPIO peripheral configuration are the sole
 * responsibility of the PORT driver (Port_Init()).  DioIf_Init() therefore
 * only resets the software debounce state; it does NOT call HAL_GPIO_Init()
 * or any other HAL/LL function.
 *
 * @par Revision History:
 * |---------|------------|------------------|----------------------------------|
 * | Version | Date       | Author           | Description                      |
 * |---------|------------|------------------|----------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                  |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template        |
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | The three on-board controller    |
 * |         |            |                  | LEDs are now listed              |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the          |
 * |         |            |                  | project's standard types         |
 * |         |            |                  | defined in Std_Types.h.          |
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Migrated to AUTOSAR-compliant    |
 * |         |            |                  | Dio driver.                      |
 * |         |            |                  | Separated generic and            |
 * |         |            |                  | application-specific channels.   |
 * |         |            |                  | ESC signals moved to EscDioExt.  |
 * |         |            |                  | Extension entries injected via   |
 * |         |            |                  | DioExt.c at compile time.        |
 * |         |            |                  | Removed Mcal.h dependency and    |
 * |         |            |                  | HAL_GPIO_Init calls from         |
 * |         |            |                  | DioIf_Init. GPIO configuration   |
 * |         |            |                  | is now exclusively owned by the  |
 * |         |            |                  | PORT driver (Port_Init).         |
 * |---------|------------|------------------|----------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

/*
 * NOTE: Mcal.h is intentionally NOT included here.
 * DioIf must remain hardware-independent and may only access GPIO through
 * the AUTOSAR DIO API (Dio_ReadChannel, Dio_WriteChannel, Dio_FlipChannel).
 * All GPIO pin configuration (direction, pull, speed, mode) is performed
 * exclusively by the PORT driver via Port_Init().
 */

#include "DioIf.h"    /* Also pulls in DioExt.h → EscDioExt.h for the enum */
#include "Std_Types.h"

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define BTN_DEBOUNCE_COUNT      5u

/* ─── Private Variables ───────────────────────────────────────────────────── */

/**
 * @brief Board-specific channel mapping.
 *
 * @details Each entry maps a logical DioIf_ChannelType to its AUTOSAR
 *          Dio_ChannelType ID. The ID is derived from the physical GPIO
 *          assignment using ChannelId = (PortIndex * 16) + PinNumber.
 *
 *          Port indices: A=0, B=1, C=2, D=3, E=4, F=5, G=6.
 *
 *          The table is split into two sections:
 *            Section 1 — generic channels defined directly below.
 *            Section 2 — extension channels contributed by DioExt.c,
 *                        which is #included inside the array initialiser.
 */
static const Dio_PinConfig pinConfig[HWAB_DIO_CHANNEL_COUNT] =
{
    /* ── Section 1: Generic channels ─────────────────────────────────────
     *
     * These entries are always present regardless of which extension is
     * active.  They map to the generic enumerators defined at the top of
     * DioIf_ChannelType in DioIf.h.
     */



    /* ── Section 2: Extension channels ───────────────────────────────────
     *
     * Contributed by the active extension at compile time.
     * DioExt.c defines DIO_EXT_CFG_INTERNAL and includes the appropriate
     * extension .c fragment (e.g. EscDioExt.c).
     *
     * To switch extensions: change the include inside DioExt.c (and the
     * matching include inside DioExt.h).  No other file needs modification.
     */
#define DIO_HwAb_C_INTERNAL
#include "DioExt.c"
#undef  DIO_HwAb_C_INTERNAL
};

/* Internal state for debouncing */
static boolean s_lastStableState[HWAB_DIO_CHANNEL_COUNT];
static uint8   s_debounceCounter[HWAB_DIO_CHANNEL_COUNT];
static boolean s_lastRawReading[HWAB_DIO_CHANNEL_COUNT];

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Initialize the DIO hardware abstraction layer.
 *
 * @details Resets all internal debounce state arrays.
 *
 *          GPIO pin direction, pull-resistors, output type, speed and
 *          alternate functions are configured exclusively by the PORT driver
 *          (Port_Init()) before this function is called.  DioIf_Init()
 *          does NOT access any MCAL / HAL functions or hardware registers.
 *
 *          Call order required at startup:
 *              1. Port_Init(&PortConfigSet);   ← configures all GPIO pins
 *              2. DioIf_Init();                ← resets debounce state only
 *
 * @pre    Port_Init() has been called with a configuration set that covers
 *         all channels listed in pinConfig[].
 */
void DioIf_Init(void)
{
    uint8 i;

    for (i = 0u; i < HWAB_DIO_CHANNEL_COUNT; i++)
    {
        s_lastStableState[i] = FALSE;
        s_debounceCounter[i] = 0u;
        s_lastRawReading[i]  = FALSE;
    }
}

/**
 * @brief  Read the state of a logical DIO channel.
 * @param  ch  Logical channel identifier.
 * @return TRUE if the channel is HIGH, FALSE if LOW.
 *
 * @details For channels without debouncing, delegates directly to
 *          Dio_ReadChannel(). For debounce-enabled inputs, a
 *          counter-based filter (threshold: BTN_DEBOUNCE_COUNT) is
 *          applied before the stable state is returned.
 */
boolean DioIf_Read(DioIf_ChannelType ch)
{
    boolean currentRaw;

    if (ch >= HWAB_DIO_CHANNEL_COUNT)
    {
        return FALSE;
    }

    const Dio_PinConfig *cfg = &pinConfig[ch];

    /* Case 1: Standard direct read */
    if (!cfg->debounceStatus)
    {
        return (Dio_ReadChannel(cfg->channelId) == STD_HIGH) ? TRUE : FALSE;
    }

    /* Case 2: Debounced read */
    currentRaw = (Dio_ReadChannel(cfg->channelId) == STD_HIGH) ? TRUE : FALSE;

    if (currentRaw == s_lastStableState[ch])
    {
        s_debounceCounter[ch] = 0u;
    }
    else
    {
        /* If raw reading is consistent with previous raw reading, increment */
        if (currentRaw == s_lastRawReading[ch])
        {
            s_debounceCounter[ch]++;
        }
        else
        {
            /* Reset counter on glitch/flip */
            s_debounceCounter[ch] = 1u;
        }

        /* Check if stable long enough */
        if (s_debounceCounter[ch] >= BTN_DEBOUNCE_COUNT)
        {
            s_lastStableState[ch] = currentRaw;
            s_debounceCounter[ch] = 0u;
        }
    }

    s_lastRawReading[ch] = currentRaw;
    return s_lastStableState[ch];
}

/**
 * @brief  Write a state to a logical DIO channel.
 * @param  ch     Logical channel identifier.
 * @param  state  Desired output state (TRUE = HIGH, FALSE = LOW).
 *
 * @details Calls Dio_WriteChannel() with STD_HIGH or STD_LOW. Has no effect
 *          if ch is out of range or the channel is not configured as output.
 */
void DioIf_Write(DioIf_ChannelType ch, boolean state)
{
    if (ch < HWAB_DIO_CHANNEL_COUNT && pinConfig[ch].isOutput)
    {
        Dio_WriteChannel(pinConfig[ch].channelId,
                         (state != FALSE) ? STD_HIGH : STD_LOW);
    }
}

/**
 * @brief  Toggle the state of a logical DIO channel.
 * @param  ch  Logical channel identifier.
 *
 * @details Calls Dio_FlipChannel() (AUTOSAR [SWS_Dio_00190]). Has no effect
 *          if ch is out of range or the channel is not configured as output.
 */
void DioIf_Toggle(DioIf_ChannelType ch)
{
    if (ch < HWAB_DIO_CHANNEL_COUNT && pinConfig[ch].isOutput)
    {
        (void)Dio_FlipChannel(pinConfig[ch].channelId);
    }
}
