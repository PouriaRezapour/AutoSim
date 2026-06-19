/**
 * @file    Dio_HwAb.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   DIO Hardware Abstraction Layer implementation.
 *
 * @details
 * Provides board-specific GPIO pin mapping and runtime access to all digital
 * input/output channels. Each channel is described by a static pin
 * configuration table that binds a logical channel ID to a GPIO port, pin,
 * direction, and debounce flag. Input channels with debouncing enabled use a
 * counter-based algorithm (threshold: BTN_DEBOUNCE_COUNT) to filter glitches
 * before reporting a stable state. Output channels support direct write and
 * toggle operations.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------|
 * | Version | Date       | Author           | Description                    |
 * |---------|------------|------------------|--------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template      |
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | The three on-board controller  |
 * |         |            |                  | LEDs are now listed            |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the        |
 * |         |            |                  | project's standard types       |
 * |         |            |                  | defined in Std_Types.h.        |
 * |---------|------------|------------------|--------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Dio_HwAb.h"
#include "Dio.h"
#include "Mcal.h"
#include "Std_Types.h"

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define BTN_DEBOUNCE_COUNT  5

/* ─── Private Variables ───────────────────────────────────────────────────── */

/* Board-specific pin mapping */
static const Dio_PinConfig pinConfig[HWAB_DIO_CHANNEL_COUNT] =
{
    [HWAB_DIO_BLUE_USER_BUTTON] = {GPIOC, GPIO_PIN_13, false, true},

    [HWAB_DIO_MOTOR_STATUS]     = {GPIOG, GPIO_PIN_6,  false, false},
    [HWAB_DIO_SIGNAL_VALVE_ON]  = {GPIOF, GPIO_PIN_10, false, false},
    [HWAB_DIO_SIGNAL_VALVE1]    = {GPIOG, GPIO_PIN_4,  false, false},
    [HWAB_DIO_SIGNAL_VALVE2]    = {GPIOG, GPIO_PIN_5,  false, false},
    [HWAB_DIO_SIGNAL_VALVE3]    = {GPIOG, GPIO_PIN_8,  false, false},
    [HWAB_DIO_SIGNAL_VALVE4]    = {GPIOE, GPIO_PIN_0,  false, false},
    [HWAB_DIO_SIGNAL_VALVE5]    = {GPIOF, GPIO_PIN_15, false, false},
    [HWAB_DIO_SIGNAL_VALVE6]    = {GPIOE, GPIO_PIN_15, false, false},
    [HWAB_DIO_SIGNAL_VALVE7]    = {GPIOD, GPIO_PIN_15, false, false},
    [HWAB_DIO_SIGNAL_VALVE8]    = {GPIOE, GPIO_PIN_10, false, false},
    [HWAB_DIO_SIGNAL_VALVE9]    = {GPIOD, GPIO_PIN_14, false, false},
    [HWAB_DIO_SIGNAL_VALVE10]   = {GPIOD, GPIO_PIN_11, false, false},
    [HWAB_DIO_SIGNAL_VALVE11]   = {GPIOE, GPIO_PIN_7,  false, false},
    [HWAB_DIO_SIGNAL_VALVE12]   = {GPIOD, GPIO_PIN_12, false, false},

    [HWAB_DIO_CMD_VESC]         = {GPIOB, GPIO_PIN_11, true,  false},
    [HWAB_DIO_CMD_BLS]          = {GPIOC, GPIO_PIN_6,  true,  false},
    [HWAB_DIO_CMD_ITPMS]        = {GPIOC, GPIO_PIN_8,  true,  false},
    [HWAB_DIO_CMD_PARK_BRAKE]   = {GPIOC, GPIO_PIN_9,  true,  false},
    [HWAB_DIO_CMD_ESC_TCS]      = {GPIOB, GPIO_PIN_8,  true,  false},
    [HWAB_DIO_CMD_WAKEUP]       = {GPIOB, GPIO_PIN_9,  true,  false},
    [HWAB_DIO_CMD_VMOT]         = {GPIOB, GPIO_PIN_12, true,  false},

    [HWAB_DIO_LED1]             = {GPIOB, GPIO_PIN_0,  true,  false},
    [HWAB_DIO_LED2]             = {GPIOE, GPIO_PIN_1,  true,  false},
    [HWAB_DIO_LED3]             = {GPIOB, GPIO_PIN_14, true,  false}
};

/* Internal state for debouncing */
static boolean    s_lastStableState[HWAB_DIO_CHANNEL_COUNT];
static uint8 s_debounceCounter[HWAB_DIO_CHANNEL_COUNT];
static boolean    s_lastRawReading[HWAB_DIO_CHANNEL_COUNT];

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Dio_HwAb_Init(void)
{
    Dio_Init(); /* Init generic Dio (clocks) */

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    for (int i = 0; i < HWAB_DIO_CHANNEL_COUNT; i++)
    {
        /* Init debounce states */
        s_lastStableState[i] = false;
        s_debounceCounter[i] = 0;
        s_lastRawReading[i]  = false;

        /* Hardware peripheral init */
        GPIO_InitStruct.Pin   = pinConfig[i].pin;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

        if (pinConfig[i].isOutput)
        {
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
        }
        else
        {
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        }
        HAL_GPIO_Init(pinConfig[i].port, &GPIO_InitStruct);
    }
}

boolean Dio_HwAb_Read(Dio_HwAb_ChannelType ch)
{
    if (ch >= HWAB_DIO_CHANNEL_COUNT) return false;

    const Dio_PinConfig *cfg = &pinConfig[ch];

    /* Case 1: Standard direct read */
    if (!cfg->debounceStatus)
    {
        return Dio_ReadPin(cfg->port, cfg->pin);
    }

    /* Case 2: Debounced read */
    boolean currentRaw = Dio_ReadPin(cfg->port, cfg->pin);

    if (currentRaw == s_lastStableState[ch])
    {
        s_debounceCounter[ch] = 0;
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
            s_debounceCounter[ch] = 1;
        }

        /* Check if stable long enough */
        if (s_debounceCounter[ch] >= BTN_DEBOUNCE_COUNT)
        {
            s_lastStableState[ch] = currentRaw;
            s_debounceCounter[ch] = 0;
        }
    }

    s_lastRawReading[ch] = currentRaw;
    return s_lastStableState[ch];
}

void Dio_HwAb_Write(Dio_HwAb_ChannelType ch, boolean state)
{
    if (ch < HWAB_DIO_CHANNEL_COUNT && pinConfig[ch].isOutput)
    {
        Dio_WritePin(pinConfig[ch].port, pinConfig[ch].pin, state);
    }
}

void Dio_HwAb_Toggle(Dio_HwAb_ChannelType ch)
{
    if (ch < HWAB_DIO_CHANNEL_COUNT && pinConfig[ch].isOutput)
    {
        Dio_TogglePin(pinConfig[ch].port, pinConfig[ch].pin);
    }
}
