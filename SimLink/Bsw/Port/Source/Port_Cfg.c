/**
 * @file    Port_Cfg.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   AUTOSAR Port Driver – Static Configuration Data.
 *
 * @details
 * Defines the pin-configuration table and the top-level Port_ConfigType
 * object that is passed to Port_Init() at startup.  Every row in the table
 * corresponds to one physical GPIO pin that the project uses.
 *
 * To add a new peripheral:
 *   1. Add one or more Port_PinConfigType entries to Port_PinConfigs[].
 *   2. Add the corresponding symbolic-name macro in Port.h.
 *   3. The array size is calculated automatically via the sizeof idiom.
 *
 * Currently configured pins:
 *
 *   Peripheral / Alternate-Function pins:
 *   - PD0 (PortConf_PIN_FDCAN1_RX) – FDCAN1_RX, AF9, no pull, low speed
 *   - PD1 (PortConf_PIN_FDCAN1_TX) – FDCAN1_TX, AF9, no pull, low speed
 *
 *   ESC DIO input pins (GPIO_MODE_INPUT):
 *   - PG6  (PortConf_PIN_MOTOR_STATUS)    – Motor status, pull-down
 *   - PF10 (PortConf_PIN_SIGNAL_VALVE_ON) – Valve-ON status, pull-down
 *   - PG4  (PortConf_PIN_SIGNAL_VALVE1)   – Valve 1, pull-down
 *   - PG5  (PortConf_PIN_SIGNAL_VALVE2)   – Valve 2, pull-down
 *   - PG8  (PortConf_PIN_SIGNAL_VALVE3)   – Valve 3, pull-down
 *   - PE0  (PortConf_PIN_SIGNAL_VALVE4)   – Valve 4, pull-down
 *   - PF15 (PortConf_PIN_SIGNAL_VALVE5)   – Valve 5, pull-down
 *   - PE15 (PortConf_PIN_SIGNAL_VALVE6)   – Valve 6, pull-down
 *   - PD15 (PortConf_PIN_SIGNAL_VALVE7)   – Valve 7, pull-down
 *   - PE10 (PortConf_PIN_SIGNAL_VALVE8)   – Valve 8, pull-down
 *   - PD14 (PortConf_PIN_SIGNAL_VALVE9)   – Valve 9, pull-down
 *   - PD11 (PortConf_PIN_SIGNAL_VALVE10)  – Valve 10, pull-down
 *   - PE7  (PortConf_PIN_SIGNAL_VALVE11)  – Valve 11, pull-down
 *   - PD12 (PortConf_PIN_SIGNAL_VALVE12)  – Valve 12, pull-down
 *   - PC13 (PortConf_PIN_BLUE_USER_BUTTON)– Blue user button, pull-down
 *
 *   ESC DIO output pins (GPIO_MODE_OUTPUT_PP):
 *   - PB11 (PortConf_PIN_CMD_VESC)        – VESC command, initial LOW
 *   - PC6  (PortConf_PIN_CMD_BLS)         – BLS command, initial LOW
 *   - PC8  (PortConf_PIN_CMD_ITPMS)       – ITPMS command, initial LOW
 *   - PC9  (PortConf_PIN_CMD_PARK_BRAKE)  – Park-brake command, initial LOW
 *   - PB8  (PortConf_PIN_CMD_ESC_TCS)     – ESC-TCS command, initial LOW
 *   - PB9  (PortConf_PIN_CMD_WAKEUP)      – Wake-up command, initial LOW
 *   - PB12 (PortConf_PIN_CMD_VMOT)        – VMOT command, initial LOW
 *   - PB0  (PortConf_PIN_LED1)            – LED1, initial LOW
 *   - PE1  (PortConf_PIN_LED2)            – LED2, initial LOW
 *   - PB14 (PortConf_PIN_LED3)            – LED3, initial LOW
 *
 * AUTOSAR CP R24-11
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour | Initial AUTOSAR-compliant release.   |
 * |         |            |            | Added all ESC DIO channel entries    |
 * |         |            |            | (previously initialised ad-hoc in    |
 * |         |            |            | DioIf_Init). Port module now owns    |
 * |         |            |            | all GPIO pin configuration.          |
 * |---------|------------|------------|--------------------------------------|
 */

/*===========================================================================*
 * INCLUDES
 *===========================================================================*/

#include "Port_Cfg.h"
#include "Port.h"

/*===========================================================================*
 * PRIVATE MACROS
 *===========================================================================*/

/**
 * @brief   Compile-time count of entries in an array.
 * @details Used to keep Port_ConfigType.NumPins in sync with the table
 *          automatically whenever rows are added or removed.
 */
#define PORT_ARRAY_SIZE(arr)    ((uint16)(sizeof(arr) / sizeof((arr)[0U])))

/*===========================================================================*
 * PIN CONFIGURATION TABLE  [SWS_Port_00004][SWS_Port_00079][SRS_Port_12001]
 *
 * Each entry must set EVERY field of Port_PinConfigType.
 * Fields:
 *   .GpioPort            – GPIO peripheral base (GPIOA … GPIOK)
 *   .GpioPinMask         – HAL GPIO_PIN_x bitmask
 *   .HalMode             – HAL mode constant (GPIO_MODE_AF_PP, …)
 *   .Pull                – GPIO_NOPULL / GPIO_PULLUP / GPIO_PULLDOWN
 *   .Speed               – GPIO_SPEED_FREQ_LOW … GPIO_SPEED_FREQ_VERY_HIGH
 *   .Alternate           – AF index (0 … 15); 0 for GPIO / Analog
 *   .InitialLevel        – PORT_PIN_LEVEL_HIGH / PORT_PIN_LEVEL_LOW
 *   .Direction           – PORT_PIN_IN / PORT_PIN_OUT
 *   .DirectionChangeable – TRUE / FALSE
 *   .ModeChangeable      – TRUE / FALSE
 *   .PinId               – PortConf_PIN_xxx symbolic constant
 *===========================================================================*/

static const Port_PinConfigType Port_PinConfigs[] =
{
    /* ====================================================================== *
     * Section 1: Peripheral / Alternate-Function pins                        *
     * ====================================================================== */

    /* ---------------------------------------------------------------------- *
     * PD0 – FDCAN1_RX (Alternate Function 9)                                 *
     * ---------------------------------------------------------------------- */
    {
        .GpioPort            = PORT_FDCAN1_GPIO_PORT,
        .GpioPinMask         = PORT_FDCAN1_RX_PIN,
        .HalMode             = GPIO_MODE_AF_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = PORT_FDCAN1_AF,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_FDCAN1_RX
    },

    /* ---------------------------------------------------------------------- *
     * PD1 – FDCAN1_TX (Alternate Function 9)                                 *
     * ---------------------------------------------------------------------- */
    {
        .GpioPort            = PORT_FDCAN1_GPIO_PORT,
        .GpioPinMask         = PORT_FDCAN1_TX_PIN,
        .HalMode             = GPIO_MODE_AF_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = PORT_FDCAN1_AF,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_FDCAN1_TX
    },

    /* ====================================================================== *
     * Section 2: ESC DIO – Input pins                                        *
     *                                                                        *
     * All status inputs are configured as GPIO_MODE_INPUT with GPIO_PULLDOWN *
     * so that undriven lines read LOW (safe state).  No initial level is     *
     * relevant for inputs; the field is set to PORT_PIN_LEVEL_LOW by         *
     * convention.                                                             *
     * ====================================================================== */

    /* Motor ---------------------------------------------------------------- */

    /* PG6 – Motor status */
    {
        .GpioPort            = GPIOG,
        .GpioPinMask         = GPIO_PIN_6,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_MOTOR_STATUS
    },

    /* Valves --------------------------------------------------------------- */

    /* PF10 – Valve-ON status */
    {
        .GpioPort            = GPIOF,
        .GpioPinMask         = GPIO_PIN_10,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE_ON
    },

    /* PG4 – Valve 1 */
    {
        .GpioPort            = GPIOG,
        .GpioPinMask         = GPIO_PIN_4,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE1
    },

    /* PG5 – Valve 2 */
    {
        .GpioPort            = GPIOG,
        .GpioPinMask         = GPIO_PIN_5,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE2
    },

    /* PG8 – Valve 3 */
    {
        .GpioPort            = GPIOG,
        .GpioPinMask         = GPIO_PIN_8,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE3
    },

    /* PE0 – Valve 4 */
    {
        .GpioPort            = GPIOE,
        .GpioPinMask         = GPIO_PIN_0,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE4
    },

    /* PF15 – Valve 5 */
    {
        .GpioPort            = GPIOF,
        .GpioPinMask         = GPIO_PIN_15,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE5
    },

    /* PE15 – Valve 6 */
    {
        .GpioPort            = GPIOE,
        .GpioPinMask         = GPIO_PIN_15,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE6
    },

    /* PD15 – Valve 7 */
    {
        .GpioPort            = GPIOD,
        .GpioPinMask         = GPIO_PIN_15,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE7
    },

    /* PE10 – Valve 8 */
    {
        .GpioPort            = GPIOE,
        .GpioPinMask         = GPIO_PIN_10,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE8
    },

    /* PD14 – Valve 9 */
    {
        .GpioPort            = GPIOD,
        .GpioPinMask         = GPIO_PIN_14,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE9
    },

    /* PD11 – Valve 10 */
    {
        .GpioPort            = GPIOD,
        .GpioPinMask         = GPIO_PIN_11,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE10
    },

    /* PE7 – Valve 11 */
    {
        .GpioPort            = GPIOE,
        .GpioPinMask         = GPIO_PIN_7,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE11
    },

    /* PD12 – Valve 12 */
    {
        .GpioPort            = GPIOD,
        .GpioPinMask         = GPIO_PIN_12,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_SIGNAL_VALVE12
    },

    /* HMI ------------------------------------------------------------------ */

    /* PC13 – Blue user button (active-high via external pull-down) */
    {
        .GpioPort            = GPIOC,
        .GpioPinMask         = GPIO_PIN_13,
        .HalMode             = GPIO_MODE_INPUT,
        .Pull                = GPIO_PULLDOWN,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_IN,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_BLUE_USER_BUTTON
    },

    /* ====================================================================== *
     * Section 3: ESC DIO – Output pins (GPIO_MODE_OUTPUT_PP)                 *
     *                                                                        *
     * All command outputs and LEDs are push-pull outputs initialised LOW.    *
     * The output latch is pre-loaded to LOW before MODER is set to output    *
     * mode, preventing any spurious pulse [SWS_Port_00055].                  *
     * ====================================================================== */

    /* Command outputs ------------------------------------------------------ */

    /* PB11 – VESC command */
    {
        .GpioPort            = GPIOB,
        .GpioPinMask         = GPIO_PIN_11,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_CMD_VESC
    },

    /* PC6 – BLS command */
    {
        .GpioPort            = GPIOC,
        .GpioPinMask         = GPIO_PIN_6,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_CMD_BLS
    },

    /* PC8 – ITPMS command */
    {
        .GpioPort            = GPIOC,
        .GpioPinMask         = GPIO_PIN_8,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_CMD_ITPMS
    },

    /* PC9 – Park-brake command */
    {
        .GpioPort            = GPIOC,
        .GpioPinMask         = GPIO_PIN_9,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_CMD_PARK_BRAKE
    },

    /* PB8 – ESC-TCS command */
    {
        .GpioPort            = GPIOB,
        .GpioPinMask         = GPIO_PIN_8,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_CMD_ESC_TCS
    },

    /* PB9 – Wake-up command */
    {
        .GpioPort            = GPIOB,
        .GpioPinMask         = GPIO_PIN_9,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_CMD_WAKEUP
    },

    /* PB12 – VMOT command */
    {
        .GpioPort            = GPIOB,
        .GpioPinMask         = GPIO_PIN_12,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_CMD_VMOT
    },

    /* Controller LEDs ------------------------------------------------------ */

    /* PB0 – LED1
     * NOTE: PB0 is shared with Ethernet on some Nucleo-144 variants.
     *       Ensure the Ethernet peripheral is not simultaneously active. */
    {
        .GpioPort            = GPIOB,
        .GpioPinMask         = GPIO_PIN_0,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_LED1
    },

    /* PE1 – LED2 */
    {
        .GpioPort            = GPIOE,
        .GpioPinMask         = GPIO_PIN_1,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_LED2
    },

    /* PB14 – LED3 */
    {
        .GpioPort            = GPIOB,
        .GpioPinMask         = GPIO_PIN_14,
        .HalMode             = GPIO_MODE_OUTPUT_PP,
        .Pull                = GPIO_NOPULL,
        .Speed               = GPIO_SPEED_FREQ_LOW,
        .Alternate           = 0U,
        .InitialLevel        = PORT_PIN_LEVEL_LOW,
        .Direction           = PORT_PIN_OUT,
        .DirectionChangeable = FALSE,
        .ModeChangeable      = FALSE,
        .PinId               = PortConf_PIN_LED3
    }

    /*
     * ── Template entry for future peripherals ──────────────────────────────
     * {
     *     .GpioPort            = GPIOx,
     *     .GpioPinMask         = GPIO_PIN_n,
     *     .HalMode             = GPIO_MODE_OUTPUT_PP,
     *     .Pull                = GPIO_NOPULL,
     *     .Speed               = GPIO_SPEED_FREQ_LOW,
     *     .Alternate           = 0U,
     *     .InitialLevel        = PORT_PIN_LEVEL_LOW,
     *     .Direction           = PORT_PIN_OUT,
     *     .DirectionChangeable = FALSE,
     *     .ModeChangeable      = FALSE,
     *     .PinId               = PortConf_PIN_MY_NEW_PIN
     * },
     */
};

/*===========================================================================*
 * TOP-LEVEL CONFIGURATION OBJECT  [SWS_Port_00228]
 *===========================================================================*/

/**
 * @brief   Pre-compiled Port Driver configuration set.
 * @details Pass &PortConfigSet to Port_Init() from the application
 *          startup / EcuM initialisation sequence.
 */
const Port_ConfigType PortConfigSet =
{
    .PinConfig = Port_PinConfigs,
    .NumPins   = PORT_ARRAY_SIZE(Port_PinConfigs)
};
