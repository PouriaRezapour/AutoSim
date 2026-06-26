/**
 * @file    EscDioExt.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6 
 * @brief   ESC-specific DIO extension — pin configuration fragment.
 *
 * @details
 * This file contributes the ESC-application-specific entries to the
 * DioIf pinConfig table.  It is included (as a .c fragment, not compiled
 * independently) by DioIf.c via DioExt.c.
 *
 * ── How the table is assembled ─────────────────────────────────────────────
 *
 *   DioIf.c builds pinConfig[] with two sections:
 *
 *     static const Dio_PinConfig pinConfig[HWAB_DIO_CHANNEL_COUNT] =
 *     {
 *         // 1. Generic entries (user button) — in DioIf.c
 *         [HWAB_DIO_BLUE_USER_BUTTON] = { … },
 *
 *         // 2. Extension entries — contributed here via DioExt.c
 *         #include "DioExt.c"
 *     };
 *
 *   DioExt.c defines DIO_EXT_CFG_INTERNAL and then includes this file,
 *   which expands to the designated-initialiser rows for every ESC channel.
 *
 * ── Standalone compilation guard ──────────────────────────────────────────
 *
 * The entire pin-config content is wrapped in
 *     #ifdef DIO_EXT_CFG_INTERNAL … #endif
 * so that when the build system compiles this file as a standalone TU
 * (because it is a .c file in the source folder) it produces an empty
 * object instead of errors.  DioExt.c defines DIO_EXT_CFG_INTERNAL
 * immediately before #including this file, so the content is only active
 * in the correct include context.
 *
 * @note  Do NOT compile this file directly.  It is a code fragment included
 *        by DioExt.c which is itself included by DioIf.c.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifdef DIO_EXT_CFG_INTERNAL

/*
 * ── Pin configuration fragment ─────────────────────────────────────────────
 *
 * Each row is a C99 designated initialiser that slots directly into the
 * Dio_PinConfig pinConfig[] array defined in DioIf.c.
 *
 * Column layout: { channelId,                  isOutput, debounceStatus }
 *
 * ChannelId encoding: (PortIndex * 16) + PinNumber
 *   Port indices: A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9, K=10
 */

    /* ── Inputs ─────────────────────────────────────────────────────── */

    /* Motor */
    [HWAB_DIO_MOTOR_STATUS]     = {DioConf_DioChannel_PG6,  FALSE, FALSE},  /* GPIOG PIN_6  -> ChannelId 102 */

    /* Valves */
    [HWAB_DIO_SIGNAL_VALVE_ON]  = {DioConf_DioChannel_PF10, FALSE, FALSE},  /* GPIOF PIN_10 -> ChannelId  90 */
    [HWAB_DIO_SIGNAL_VALVE1]    = {DioConf_DioChannel_PG4,  FALSE, FALSE},  /* GPIOG PIN_4  -> ChannelId 100 */
    [HWAB_DIO_SIGNAL_VALVE2]    = {DioConf_DioChannel_PG5,  FALSE, FALSE},  /* GPIOG PIN_5  -> ChannelId 101 */
    [HWAB_DIO_SIGNAL_VALVE3]    = {DioConf_DioChannel_PG8,  FALSE, FALSE},  /* GPIOG PIN_8  -> ChannelId 104 */
    [HWAB_DIO_SIGNAL_VALVE4]    = {DioConf_DioChannel_PE0,  FALSE, FALSE},  /* GPIOE PIN_0  -> ChannelId  64 */
    [HWAB_DIO_SIGNAL_VALVE5]    = {DioConf_DioChannel_PF15, FALSE, FALSE},  /* GPIOF PIN_15 -> ChannelId  95 */
    [HWAB_DIO_SIGNAL_VALVE6]    = {DioConf_DioChannel_PE15, FALSE, FALSE},  /* GPIOE PIN_15 -> ChannelId  79 */
    [HWAB_DIO_SIGNAL_VALVE7]    = {DioConf_DioChannel_PD15, FALSE, FALSE},  /* GPIOD PIN_15 -> ChannelId  63 */
    [HWAB_DIO_SIGNAL_VALVE8]    = {DioConf_DioChannel_PE10, FALSE, FALSE},  /* GPIOE PIN_10 -> ChannelId  74 */
    [HWAB_DIO_SIGNAL_VALVE9]    = {DioConf_DioChannel_PD14, FALSE, FALSE},  /* GPIOD PIN_14 -> ChannelId  62 */
    [HWAB_DIO_SIGNAL_VALVE10]   = {DioConf_DioChannel_PD11, FALSE, FALSE},  /* GPIOD PIN_11 -> ChannelId  59 */
    [HWAB_DIO_SIGNAL_VALVE11]   = {DioConf_DioChannel_PE7,  FALSE, FALSE},  /* GPIOE PIN_7  -> ChannelId  71 */
    [HWAB_DIO_SIGNAL_VALVE12]   = {DioConf_DioChannel_PD12, FALSE, FALSE},  /* GPIOD PIN_12 -> ChannelId  60 */

    /* ── Outputs ─────────────────────────────────────────────────────── */

    /* Command Signals */
    [HWAB_DIO_CMD_VESC]         = {DioConf_DioChannel_PB11, TRUE,  FALSE},  /* GPIOB PIN_11 -> ChannelId  27 */
    [HWAB_DIO_CMD_BLS]          = {DioConf_DioChannel_PC6,  TRUE,  FALSE},  /* GPIOC PIN_6  -> ChannelId  38 */
    [HWAB_DIO_CMD_ITPMS]        = {DioConf_DioChannel_PC8,  TRUE,  FALSE},  /* GPIOC PIN_8  -> ChannelId  40 */
    [HWAB_DIO_CMD_PARK_BRAKE]   = {DioConf_DioChannel_PC9,  TRUE,  FALSE},  /* GPIOC PIN_9  -> ChannelId  41 */
    [HWAB_DIO_CMD_ESC_TCS]      = {DioConf_DioChannel_PB8,  TRUE,  FALSE},  /* GPIOB PIN_8  -> ChannelId  24 */
    [HWAB_DIO_CMD_WAKEUP]       = {DioConf_DioChannel_PB9,  TRUE,  FALSE},  /* GPIOB PIN_9  -> ChannelId  25 */
    [HWAB_DIO_CMD_VMOT]         = {DioConf_DioChannel_PB12, TRUE,  FALSE},  /* GPIOB PIN_12 -> ChannelId  28 */

    /* Controller LEDs */
    [HWAB_DIO_LED1]             = {DioConf_DioChannel_PB0,  TRUE,  FALSE},  /* GPIOB PIN_0  -> ChannelId  16 */
    [HWAB_DIO_LED2]             = {DioConf_DioChannel_PE1,  TRUE,  FALSE},  /* GPIOE PIN_1  -> ChannelId  65 */
    [HWAB_DIO_LED3]             = {DioConf_DioChannel_PB14, TRUE,  FALSE},  /* GPIOB PIN_14 -> ChannelId  30 */

    /* HMI */
    [HWAB_DIO_BLUE_USER_BUTTON] = {DioConf_DioChannel_PC13, FALSE, TRUE },  /* GPIOC PIN_13 -> ChannelId  45 */

#endif /* DIO_EXT_CFG_INTERNAL */
