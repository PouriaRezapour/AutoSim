/**
 * @file    Port.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   AUTOSAR Port Driver – Public Header.
 *
 * @details
 * Defines all types, error codes, and API function prototypes for the
 * AUTOSAR Port Driver module (AUTOSAR CP R24-11, Document ID 40:
 * AUTOSAR_CP_SWS_PortDriver) targeting the STM32H753 microcontroller.
 *
 * This header constitutes the complete public interface of the Port Driver.
 * It exposes:
 *   - Module identification and version macros
 *   - AUTOSAR-compliant type definitions  (Port_ConfigType, Port_PinType,
 *     Port_PinDirectionType, Port_PinModeType, Port_PinConfigType)
 *   - Development-error codes
 *   - Service ID macros used when reporting errors through DET
 *   - Pre-processor feature switches (via Port_Cfg.h)
 *   - Board/peripheral pin definitions used by Port_Init()
 *   - Symbolic pin names published to the DIO driver
 *   - Public API prototypes: Port_Init, Port_SetPinDirection,
 *     Port_RefreshPortDirection, Port_GetVersionInfo, Port_SetPinMode
 *
 * Include order and circular-dependency avoidance
 * ------------------------------------------------
 * Port_Cfg.h is included FIRST so that its feature-switch macros and
 * version constants are visible to the rest of this file.  Port_Cfg.h
 * deliberately does NOT include Port.h; therefore no circular dependency
 * exists.  All Port-specific types (Port_PinType, Port_PinDirectionType,
 * Port_PinConfigType, Port_ConfigType) are defined HERE, after Port_Cfg.h
 * has been processed.
 *
 * Relationship to other modules
 * ------------------------------
 *   - The DIO driver depends on the Port driver being initialised first
 *     [SWS_Port_00078][SWS_Port_00213].
 *   - Symbolic pin names required by the DIO driver are published through
 *     this header [SWS_Port_00208].
 *   - All HAL/LL resources are accessed through "Mcal.h".
 *
 * AUTOSAR CP R24-11
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.2.0   | 2025-05-23 | A.Rezapour | Initial release                      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | Migrated to Std_Types.h              |
 * | 0.2.6   | 2025-07-08 | A.Rezapour | Migrated to full AUTOSAR-compliant   |
 * |         |            |            | interface (CP R24-11). Added         |
 * |         |            |            | PortConf_PIN_* names for all DIO     |
 * |         |            |            | channels; DioIf_Init GPIO setup      |
 * |         |            |            | moved entirely into Port_Init.       |
 * |---------|------------|------------|--------------------------------------|
 */

#ifndef PORT_H
#define PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*
 * INCLUDES
 *
 * Order is significant:
 *   1. Mcal.h      – HAL types (GPIO_TypeDef, uint32…) needed below.
 *   2. Std_Types.h – AUTOSAR base types (uint8, boolean, STD_ON …).
 *   3. Port_Cfg.h  – Feature switches and version macros only; it does NOT
 *                    include Port.h, so no circular dependency is created.
 *===========================================================================*/

#include "Mcal.h"          /* STM32 HAL/LL headers + MCU-specific definitions */
#include "Std_Types.h"     /* AUTOSAR standard types: uint8, uint16, boolean … */
#include "Port_Cfg.h"      /* Feature switches & version macros (no Port types) */

/*===========================================================================*
 * MODULE IDENTIFICATION  [SWS_Port_00208]
 *===========================================================================*/

/** @brief Vendor identifier (project-specific; adjust to your organisation). */
#define PORT_VENDOR_ID                  (0xFFFFU)

/** @brief AUTOSAR module identifier for the Port Driver [Document ID 40]. */
#define PORT_MODULE_ID                  (124U)

/** @brief Module instance – only one Port Driver instance is supported. */
#define PORT_INSTANCE_ID                (0U)

/*===========================================================================*
 * AUTOSAR RELEASE VERSION
 *===========================================================================*/

#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (7U)
#define PORT_AR_RELEASE_PATCH_VERSION   (0U)

/*===========================================================================*
 * SOFTWARE VERSION
 *===========================================================================*/

#define PORT_SW_MAJOR_VERSION           (0U)
#define PORT_SW_MINOR_VERSION           (2U)
#define PORT_SW_PATCH_VERSION           (6U)

/*===========================================================================*
 * VERSION CONSISTENCY CHECK (against Port_Cfg.h)
 *
 * Safe to perform here: Port_Cfg.h was included above, so
 * PORT_CFG_AR_MAJOR_VERSION and PORT_CFG_AR_MINOR_VERSION are defined.
 *===========================================================================*/

#if (PORT_AR_RELEASE_MAJOR_VERSION != PORT_CFG_AR_MAJOR_VERSION)
  #error "Port.h: AUTOSAR major version mismatch with Port_Cfg.h"
#endif
#if (PORT_AR_RELEASE_MINOR_VERSION != PORT_CFG_AR_MINOR_VERSION)
  #error "Port.h: AUTOSAR minor version mismatch with Port_Cfg.h"
#endif

/*===========================================================================*
 * TYPE DEFINITIONS
 *===========================================================================*/

/**
 * @brief   Symbolic name (numeric ID) of a port pin.
 * @details [SWS_Port_00229][SWS_Port_00013] Port_PinType shall be used for
 *          the symbolic name of a Port Pin.
 *          [SWS_Port_00219] The concrete underlying type is chosen for best
 *          performance on the STM32H753; uint16 covers the full 176-channel
 *          space (11 ports × 16 pins).
 *          Encoding: PinId = (PortIndex * 16) + PinNumber.
 *          Available via Port.h.
 */
typedef uint16 Port_PinType;

/**
 * @brief   Maximum valid Port_PinType value ((NUM_PORTS * PINS_PER_PORT) - 1).
 * @details Defined here rather than in Port_Cfg.h because it requires the
 *          Port_PinType typedef above to be visible.
 */
#define PORT_MAX_PIN_ID                 ((Port_PinType)((PORT_NUM_PORTS * PORT_PINS_PER_PORT) - 1U))

/**
 * @brief   Possible directions of a port pin.
 * @details [SWS_Port_00230][SWS_Port_00046][SWS_Port_00220]
 *          Enumeration with exactly two members: PORT_PIN_IN (0x00) and
 *          PORT_PIN_OUT (0x01).
 *          Available via Port.h.
 */
typedef enum
{
    PORT_PIN_IN  = 0x00U,   /**< Configures the pin as a digital input  */
    PORT_PIN_OUT = 0x01U    /**< Configures the pin as a digital output */
} Port_PinDirectionType;

/**
 * @brief   Different port pin modes (alternate functions + GPIO).
 * @details [SWS_Port_00231][SWS_Port_00124][SWS_Port_00212][SWS_Port_00221]
 *          A port pin may be configurable with a number of modes; the range
 *          is implementation-specific and determined by the STM32H753 AF map.
 *          Port_PinModeType shall be used with Port_SetPinMode().
 *          Available via Port.h.
 */
typedef uint8 Port_PinModeType;

/** @brief GPIO / DIO mode (no alternate function). */
#define PORT_PIN_MODE_GPIO      ((Port_PinModeType)0x00U)

/** @brief Analog mode (e.g. ADC, DAC). */
#define PORT_PIN_MODE_ANALOG    ((Port_PinModeType)0x03U)

/**
 * @brief   Alternate function modes AF0 … AF15.
 * @details STM32H753 supports 16 alternate functions per pin.  Each is
 *          encoded as (0x10 + AF_number) so that GPIO (0x00) and Analog
 *          (0x03) do not conflict with any AF value.
 */
#define PORT_PIN_MODE_AF(n)     ((Port_PinModeType)(0x10U + (uint8)(n)))

/** Convenience aliases for the most common alternate functions */
#define PORT_PIN_MODE_AF0       PORT_PIN_MODE_AF(0U)   /**< System / SWD        */
#define PORT_PIN_MODE_AF1       PORT_PIN_MODE_AF(1U)   /**< TIM1/2              */
#define PORT_PIN_MODE_AF2       PORT_PIN_MODE_AF(2U)   /**< TIM3/4/5            */
#define PORT_PIN_MODE_AF3       PORT_PIN_MODE_AF(3U)   /**< TIM8/9/10/11        */
#define PORT_PIN_MODE_AF4       PORT_PIN_MODE_AF(4U)   /**< I2C1-4              */
#define PORT_PIN_MODE_AF5       PORT_PIN_MODE_AF(5U)   /**< SPI1/2/3            */
#define PORT_PIN_MODE_AF6       PORT_PIN_MODE_AF(6U)   /**< SPI2/3 / SAI1       */
#define PORT_PIN_MODE_AF7       PORT_PIN_MODE_AF(7U)   /**< USART1-3            */
#define PORT_PIN_MODE_AF8       PORT_PIN_MODE_AF(8U)   /**< UART4/5/8 / SPDIFRX */
#define PORT_PIN_MODE_AF9       PORT_PIN_MODE_AF(9U)   /**< FDCAN1/2 / TIM13/14 */
#define PORT_PIN_MODE_AF10      PORT_PIN_MODE_AF(10U)  /**< USB OTG / SAI2      */
#define PORT_PIN_MODE_AF11      PORT_PIN_MODE_AF(11U)  /**< ETH / UART7         */
#define PORT_PIN_MODE_AF12      PORT_PIN_MODE_AF(12U)  /**< FMC / SDMMC1        */
#define PORT_PIN_MODE_AF13      PORT_PIN_MODE_AF(13U)  /**< DCMI / COMP         */
#define PORT_PIN_MODE_AF14      PORT_PIN_MODE_AF(14U)  /**< LTDC                */
#define PORT_PIN_MODE_AF15      PORT_PIN_MODE_AF(15U)  /**< EVENTOUT            */

/*===========================================================================*
 * PER-PIN CONFIGURATION TYPE
 *
 * Port_PinConfigType holds every attribute that Port_Init() must program into
 * the STM32H7 GPIO peripheral for one pin.  It maps directly onto the HAL
 * GPIO_InitTypeDef fields plus extra AUTOSAR metadata.
 * [SWS_Port_00004][SWS_Port_00079][SRS_Port_12001]
 *
 * Defined here (not in Port_Cfg.h) because it depends on Port_PinType and
 * Port_PinDirectionType, both of which are declared above.
 *===========================================================================*/

/**
 * @brief   Configuration record for one port pin.
 * @details Every field follows the AUTOSAR PortPin container parameters
 *          [ECUC_Port_00118 … ECUC_Port_00134].
 */
typedef struct
{
    /**
     * @brief   GPIO port base address (e.g. GPIOD).
     * @details Hardware-specific; derived from PortContainer / PortPin mapping.
     */
    GPIO_TypeDef          *GpioPort;

    /**
     * @brief   HAL pin bit-mask (e.g. GPIO_PIN_0).
     * @details Corresponds to PortPinId converted to the HAL bit-mask encoding.
     */
    uint32              GpioPinMask;

    /**
     * @brief   HAL pin mode (GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_AF_PP …).
     * @details Derived from PortPinMode + PortPinDirection
     *          [ECUC_Port_00128][ECUC_Port_00125].
     */
    uint32              HalMode;

    /**
     * @brief   Pull-up / pull-down configuration (GPIO_NOPULL, GPIO_PULLUP …).
     * @details Optional per [SRS_Port_12001] / [SWS_Port_00081].
     */
    uint32              Pull;

    /**
     * @brief   Output speed (GPIO_SPEED_FREQ_LOW … GPIO_SPEED_FREQ_VERY_HIGH).
     * @details Optional per [SWS_Port_00081].
     */
    uint32              Speed;

    /**
     * @brief   Alternate function index (0 … 15); ignored for GPIO / Analog modes.
     * @details [SWS_Port_00004] – pin functionality (DIO, CAN, SPI …).
     */
    uint32              Alternate;

    /**
     * @brief   Initial output level applied before direction is set to output.
     * @details [SWS_Port_00055] – PortPinLevelValue (PORT_PIN_LEVEL_HIGH /
     *          PORT_PIN_LEVEL_LOW); ignored for input or AF pins.
     */
    uint8                InitialLevel;

    /**
     * @brief   Direction of the pin (PORT_PIN_IN / PORT_PIN_OUT).
     * @details [ECUC_Port_00125] PortPinDirection.
     */
    Port_PinDirectionType  Direction;

    /**
     * @brief   TRUE  → Port_SetPinDirection() may change this pin's direction.
     * @details [ECUC_Port_00126] PortPinDirectionChangeable.
     *          FALSE → direction is fixed; Port_RefreshPortDirection() will
     *          restore it if it has been corrupted.
     */
    boolean                DirectionChangeable;

    /**
     * @brief   TRUE  → Port_SetPinMode() may change this pin's mode.
     * @details [ECUC_Port_00134] PortPinModeChangeable.
     */
    boolean                ModeChangeable;

    /**
     * @brief   AUTOSAR symbolic Port_PinType ID for this pin.
     * @details [ECUC_Port_00127] PortPinId.
     *          Encoding: PinId = (PortIndex * PORT_PINS_PER_PORT) + PinNumber.
     *          Used by Port_SetPinDirection() and Port_SetPinMode() to locate
     *          a pin within the configuration table.
     */
    Port_PinType           PinId;

} Port_PinConfigType;

/*===========================================================================*
 * CONFIGURATION SET TYPE  [SWS_Port_00228]
 *
 * Defined here (not in Port_Cfg.h) because it depends on Port_PinConfigType,
 * which in turn depends on Port_PinType and Port_PinDirectionType above.
 *===========================================================================*/

/**
 * @brief   External data structure containing the initialisation data for
 *          the Port Driver.
 * @details [SWS_Port_00228] Port_ConfigType is a hardware-dependent structure.
 *          The structure is defined in Port_Cfg.c and is passed as a const
 *          pointer to Port_Init().
 *          Available via Port.h [SWS_Port_00228].
 */
typedef struct Port_ConfigType_s Port_ConfigType;

/**
 * @brief   Top-level configuration structure passed to Port_Init().
 * @details Contains a pointer to the pin-configuration array and the number
 *          of entries in that array.  This layout supports both
 *          VARIANT-PRE-COMPILE and VARIANT-POST-BUILD.
 */
struct Port_ConfigType_s
{
    /**
     * @brief   Pointer to the array of per-pin configuration records.
     * @details Defined and initialised in Port_Cfg.c.
     */
    const Port_PinConfigType *PinConfig;

    /**
     * @brief   Number of entries in the PinConfig array.
     */
    uint16                  NumPins;
};

/*===========================================================================*
 * SYMBOLIC PIN NAMES  [SWS_Port_00006][SWS_Port_00207][SWS_Port_00208]
 *
 * These macros provide human-readable identifiers that map to the numeric
 * Port_PinType IDs.  They are used by the application and by DIO driver
 * configuration.  Encoding: PinId = (PortIndex * 16) + PinNumber.
 * PortIndex: A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9, K=10.
 *
 * Defined here (not in Port_Cfg.h) because the cast requires Port_PinType
 * to be visible.
 *===========================================================================*/

/* --- FDCAN1 ---------------------------------------------------------------- */

/** @brief Symbolic name for PD0 (FDCAN1_RX). PinId = (3*16)+0 = 48. */
#define PortConf_PIN_FDCAN1_RX          ((Port_PinType)48U)

/** @brief Symbolic name for PD1 (FDCAN1_TX). PinId = (3*16)+1 = 49. */
#define PortConf_PIN_FDCAN1_TX          ((Port_PinType)49U)

/* --- ESC DIO: Motor status input ----------------------------------------- */

/** @brief PG6 – Motor status input. PinId = (6*16)+6 = 102. */
#define PortConf_PIN_MOTOR_STATUS       ((Port_PinType)102U)

/* --- ESC DIO: Valve status inputs ---------------------------------------- */

/** @brief PF10 – Valve-ON status input. PinId = (5*16)+10 = 90. */
#define PortConf_PIN_SIGNAL_VALVE_ON    ((Port_PinType)90U)

/** @brief PG4 – Valve 1 status input. PinId = (6*16)+4 = 100. */
#define PortConf_PIN_SIGNAL_VALVE1      ((Port_PinType)100U)

/** @brief PG5 – Valve 2 status input. PinId = (6*16)+5 = 101. */
#define PortConf_PIN_SIGNAL_VALVE2      ((Port_PinType)101U)

/** @brief PG8 – Valve 3 status input. PinId = (6*16)+8 = 104. */
#define PortConf_PIN_SIGNAL_VALVE3      ((Port_PinType)104U)

/** @brief PE0 – Valve 4 status input. PinId = (4*16)+0 = 64. */
#define PortConf_PIN_SIGNAL_VALVE4      ((Port_PinType)64U)

/** @brief PF15 – Valve 5 status input. PinId = (5*16)+15 = 95. */
#define PortConf_PIN_SIGNAL_VALVE5      ((Port_PinType)95U)

/** @brief PE15 – Valve 6 status input. PinId = (4*16)+15 = 79. */
#define PortConf_PIN_SIGNAL_VALVE6      ((Port_PinType)79U)

/** @brief PD15 – Valve 7 status input. PinId = (3*16)+15 = 63. */
#define PortConf_PIN_SIGNAL_VALVE7      ((Port_PinType)63U)

/** @brief PE10 – Valve 8 status input. PinId = (4*16)+10 = 74. */
#define PortConf_PIN_SIGNAL_VALVE8      ((Port_PinType)74U)

/** @brief PD14 – Valve 9 status input. PinId = (3*16)+14 = 62. */
#define PortConf_PIN_SIGNAL_VALVE9      ((Port_PinType)62U)

/** @brief PD11 – Valve 10 status input. PinId = (3*16)+11 = 59. */
#define PortConf_PIN_SIGNAL_VALVE10     ((Port_PinType)59U)

/** @brief PE7 – Valve 11 status input. PinId = (4*16)+7 = 71. */
#define PortConf_PIN_SIGNAL_VALVE11     ((Port_PinType)71U)

/** @brief PD12 – Valve 12 status input. PinId = (3*16)+12 = 60. */
#define PortConf_PIN_SIGNAL_VALVE12     ((Port_PinType)60U)

/* --- ESC DIO: Command outputs -------------------------------------------- */

/** @brief PB11 – VESC command output. PinId = (1*16)+11 = 27. */
#define PortConf_PIN_CMD_VESC           ((Port_PinType)27U)

/** @brief PC6 – BLS command output. PinId = (2*16)+6 = 38. */
#define PortConf_PIN_CMD_BLS            ((Port_PinType)38U)

/** @brief PC8 – ITPMS command output. PinId = (2*16)+8 = 40. */
#define PortConf_PIN_CMD_ITPMS          ((Port_PinType)40U)

/** @brief PC9 – Park-brake command output. PinId = (2*16)+9 = 41. */
#define PortConf_PIN_CMD_PARK_BRAKE     ((Port_PinType)41U)

/** @brief PB8 – ESC-TCS command output. PinId = (1*16)+8 = 24. */
#define PortConf_PIN_CMD_ESC_TCS        ((Port_PinType)24U)

/** @brief PB9 – Wake-up command output. PinId = (1*16)+9 = 25. */
#define PortConf_PIN_CMD_WAKEUP         ((Port_PinType)25U)

/** @brief PB12 – VMOT command output. PinId = (1*16)+12 = 28. */
#define PortConf_PIN_CMD_VMOT           ((Port_PinType)28U)

/* --- ESC DIO: Controller LEDs -------------------------------------------- */

/** @brief PB0 – LED1 output. PinId = (1*16)+0 = 16. */
#define PortConf_PIN_LED1               ((Port_PinType)16U)

/** @brief PE1 – LED2 output. PinId = (4*16)+1 = 65. */
#define PortConf_PIN_LED2               ((Port_PinType)65U)

/** @brief PB14 – LED3 output. PinId = (1*16)+14 = 30. */
#define PortConf_PIN_LED3               ((Port_PinType)30U)

/* --- HMI ------------------------------------------------------------------ */

/** @brief PC13 – Blue user button input (with pull-down). PinId = (2*16)+13 = 45. */
#define PortConf_PIN_BLUE_USER_BUTTON   ((Port_PinType)45U)

/*
 * Extend with further symbolic names as the project grows.
 * Example:
 *   #define PortConf_PIN_SPI1_SCK  ((Port_PinType)5U)  // PA5
 */

/*===========================================================================*
 * PERIPHERAL GPIO / AF MACROS
 *
 * All board-level GPIO assignments are gathered here so that every call site
 * in Port_Cfg.c uses symbolic names rather than magic constants.
 * [SWS_Port_00006][SWS_Port_00207][SWS_Port_00208]
 *===========================================================================*/

/* --- FDCAN1 ---------------------------------------------------------------- */
/** @brief GPIO port that hosts the FDCAN1 pins (PD0 / PD1). */
#define PORT_FDCAN1_GPIO_PORT           GPIOD

/** @brief PD0 → FDCAN1_RX */
#define PORT_FDCAN1_RX_PIN              GPIO_PIN_0

/** @brief PD1 → FDCAN1_TX */
#define PORT_FDCAN1_TX_PIN              GPIO_PIN_1

/** @brief Alternate function index for FDCAN1 on STM32H753 (AF9). */
#define PORT_FDCAN1_AF                  GPIO_AF9_FDCAN1

/*===========================================================================*
 * EXTERNALLY VISIBLE CONFIGURATION OBJECT
 *===========================================================================*/

/**
 * @brief   Default (pre-compiled) configuration set.
 * @details Defined in Port_Cfg.c.  Pass its address to Port_Init() from the
 *          application startup code:
 *              Port_Init(&PortConfigSet);
 */
extern const Port_ConfigType PortConfigSet;

/*===========================================================================*
 * DEVELOPMENT ERROR CODES  [SWS_Port_00051]
 *===========================================================================*/

/** @brief Port_SetPinDirection or Port_SetPinMode called with invalid Pin ID. */
#define PORT_E_PARAM_PIN                ((uint8)0x0AU)

/**
 * @brief   Port_SetPinDirection called on a pin not configured as
 *          direction-changeable.
 */
#define PORT_E_DIRECTION_UNCHANGEABLE   ((uint8)0x0BU)

/** @brief Port_Init called with an invalid / NULL ConfigPtr. */
#define PORT_E_INIT_FAILED              ((uint8)0x0CU)

/** @brief Port_SetPinMode called with an invalid mode value. */
#define PORT_E_PARAM_INVALID_MODE       ((uint8)0x0DU)

/** @brief Port_SetPinMode called on a pin whose mode is not changeable. */
#define PORT_E_MODE_UNCHANGEABLE        ((uint8)0x0EU)

/** @brief Any Port API service called without prior Port_Init(). */
#define PORT_E_UNINIT                   ((uint8)0x0FU)

/** @brief NULL pointer passed where a valid pointer is required. */
#define PORT_E_PARAM_POINTER            ((uint8)0x10U)

/*===========================================================================*
 * SERVICE IDs (for DET reporting)
 *===========================================================================*/

#define PORT_SID_INIT                   ((uint8)0x00U)
#define PORT_SID_SET_PIN_DIRECTION      ((uint8)0x01U)
#define PORT_SID_REFRESH_PORT_DIRECTION ((uint8)0x02U)
#define PORT_SID_GET_VERSION_INFO       ((uint8)0x03U)
#define PORT_SID_SET_PIN_MODE           ((uint8)0x04U)

/*===========================================================================*
 * PUBLIC API PROTOTYPES
 *===========================================================================*/

/**
 * @brief   Initialises the Port Driver module.
 * @details [SWS_Port_00140][SWS_Port_00041][SWS_Port_00001]
 *          Port_Init() initialises ALL port pins with the configuration set
 *          pointed to by ConfigPtr.  It must be called before any DIO
 *          function or any other Port API service.
 *
 *          The function:
 *            - Enables all required GPIO clocks.
 *            - Configures every pin's mode, direction, pull, speed and
 *              alternate function according to ConfigPtr.
 *            - Sets the output-latch to the configured initial level BEFORE
 *              switching to output mode to avoid glitches [SWS_Port_00055].
 *            - Sets unused pins to a defined safe state [SWS_Port_00005].
 *            - Is NOT re-entrant and must NOT be called during normal
 *              operation [SWS_Port_00078].
 *
 *          Synchronous / Non-Reentrant.
 *
 * @param[in] ConfigPtr   Pointer to the configuration set.  For the
 *                        VARIANT-PRE-COMPILE variant this may be NULL_PTR;
 *                        the driver then uses its internally linked default
 *                        configuration [SWS_Port_00121].
 * @return    void        [SWS_Port_00140]
 */
void Port_Init(const Port_ConfigType *ConfigPtr);

/**
 * @brief   Sets the direction of a port pin during runtime.
 * @details [SWS_Port_00141][SWS_Port_00063][SWS_Port_00086]
 *          Available only when PORT_SET_PIN_DIRECTION_API == STD_ON.
 *          Only pins configured with PortPinDirectionChangeable = TRUE may
 *          be changed; attempting to change a fixed-direction pin reports
 *          PORT_E_DIRECTION_UNCHANGEABLE via DET.
 *          Does NOT alter the output-latch level [SWS_Port_00138].
 *
 *          Synchronous / Reentrant (different pins on independent ports).
 *
 * @param[in] Pin         Symbolic ID of the port pin (Port_PinType).
 * @param[in] Direction   PORT_PIN_IN or PORT_PIN_OUT.
 * @return    void
 */
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
#endif

/**
 * @brief   Refreshes the direction of all configured port pins.
 * @details [SWS_Port_00142][SWS_Port_00060][SWS_Port_00061]
 *          Restores the configured direction for all pins whose
 *          PortPinDirectionChangeable is FALSE.  Pins marked as
 *          direction-changeable are excluded [SWS_Port_00061].
 *
 *          Purpose: makes the system robust against EMC-induced or software-
 *          bug-caused corruption of GPIO direction registers
 *          [SRS_Port_12406].
 *
 *          Synchronous / Non-Reentrant.
 *
 * @return  void
 */
void Port_RefreshPortDirection(void);

/**
 * @brief   Returns the version information of the Port Driver module.
 * @details [SWS_Port_00143][SWS_Port_00225]
 *          Available only when PORT_VERSION_INFO_API == STD_ON.
 *          Reports PORT_E_PARAM_POINTER via DET when versioninfo is NULL.
 *
 *          Synchronous / Reentrant.
 *
 * @param[out] versioninfo   Pointer to the Std_VersionInfoType structure that
 *                           receives vendor ID, module ID, and SW version.
 * @return     void
 */
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif

/**
 * @brief   Sets the port pin mode during runtime.
 * @details [SWS_Port_00145][SWS_Port_00125]
 *          Available only when PORT_SET_PIN_MODE_API == STD_ON.
 *          Only pins with PortPinModeChangeable = TRUE may be changed.
 *          Attempting to change a fixed-mode pin when DET is enabled reports
 *          PORT_E_MODE_UNCHANGEABLE [SWS_Port_00223].
 *
 *          Synchronous / Reentrant (different pins on independent ports).
 *
 * @param[in] Pin    Symbolic ID of the port pin.
 * @param[in] Mode   New pin mode (use PORT_PIN_MODE_GPIO, PORT_PIN_MODE_AF(n),
 *                   or PORT_PIN_MODE_ANALOG).
 * @return    void
 */
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);
#endif

#ifdef __cplusplus
}
#endif

#endif /* PORT_H */
