/**
 * @file    Port.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   AUTOSAR Port Driver Implementation – STM32H753.
 *
 * @details
 * Implements the full AUTOSAR Port Driver API (AUTOSAR CP R24-11,
 * Document ID 40: AUTOSAR_CP_SWS_PortDriver) targeting the STM32H753
 * microcontroller.
 *
 * Architecture notes
 * ------------------
 * The Port Driver is the only module that configures GPIO pin multiplexing,
 * direction, speed, pull-resistors and alternate functions.  The DIO Driver
 * relies on the Port Driver having completed Port_Init() before any DIO
 * function is called [SWS_Port_00078][SWS_Port_00213].
 *
 * All GPIO hardware access is performed through STM32 HAL/LL functions
 * provided via "Mcal.h".  The HAL functions satisfy the atomicity
 * requirement for pin initialisation [SWS_Port_00075][SRS_Port_12423].
 *
 * Pin lookup strategy
 * -------------------
 * The configuration table (Port_PinConfigs[], defined in Port_Cfg.c) is
 * indexed linearly.  For Port_SetPinDirection() and Port_SetPinMode() the
 * table is searched by Port_PinType ID.  The table is expected to be small
 * (tens of entries), so linear search is appropriate.
 *
 * Clock enabling
 * --------------
 * Port_Init() enables only the GPIO clocks that appear in the configuration
 * table; it does NOT blindly enable all eleven GPIO clocks.
 * [SWS_Port_00042]
 *
 * Glitch prevention [SWS_Port_00055]
 * -----------------------------------
 * For output pins, the output-data register (ODR) is pre-loaded with the
 * configured initial level through BSRRs BEFORE the MODER field is switched
 * to output mode.  This prevents a spurious LOW or HIGH pulse during
 * initialisation.
 *
 * Implemented requirements
 * ------------------------
 *  [SWS_Port_00001]  Port Driver initialises the whole port structure
 *  [SWS_Port_00002]  Port_Init initialises all variables to initial state
 *  [SWS_Port_00003]  Port_Init may be used for re-initialisation
 *  [SWS_Port_00004]  Configuration of different functionality per pin
 *  [SWS_Port_00005]  Unused pins set to a defined state
 *  [SWS_Port_00041]  Port_Init initialises ALL ports with ConfigPtr set
 *  [SWS_Port_00042]  Port_Init initialises all configured resources
 *  [SWS_Port_00043]  Port_Init avoids glitches on affected pins
 *  [SWS_Port_00051]  Development error codes defined
 *  [SWS_Port_00055]  Output latch set to default level before output mode
 *  [SWS_Port_00060]  Port_RefreshPortDirection refreshes fixed-direction pins
 *  [SWS_Port_00061]  Direction-changeable pins excluded from refresh
 *  [SWS_Port_00063]  Port_SetPinDirection sets direction at runtime
 *  [SWS_Port_00075]  Atomic port access (HAL functions provide atomicity)
 *  [SWS_Port_00077]  DET parameter checking order respected
 *  [SWS_Port_00078]  Port_Init must be called before DIO functions
 *  [SWS_Port_00079]  Additional pin-level / direction / mode config
 *  [SWS_Port_00086]  Port_SetPinDirection available only when switch is ON
 *  [SWS_Port_00087]  On DET error: skip functionality and return
 *  [SWS_Port_00113]  Register initialised by the driver that owns it
 *  [SWS_Port_00121]  Port_Init always has a pointer parameter (may be NULL)
 *  [SWS_Port_00125]  Port_SetPinMode sets pin mode at runtime
 *  [SWS_Port_00138]  Switching direction does NOT alter output latch
 *  [SWS_Port_00140]  Port_Init API definition
 *  [SWS_Port_00141]  Port_SetPinDirection API definition
 *  [SWS_Port_00142]  Port_RefreshPortDirection API definition
 *  [SWS_Port_00143]  Port_GetVersionInfo API definition
 *  [SWS_Port_00145]  Port_SetPinMode API definition
 *  [SWS_Port_00208]  Symbolic names published via Port.h
 *  [SWS_Port_00213]  No operation possible before Port_Init
 *  [SWS_Port_00223]  Port_SetPinMode reports error if mode unchangeable
 *  [SWS_Port_00225]  Port_GetVersionInfo validates versioninfo pointer
 *  [SRS_Port_12001]  Static configuration of pin options
 *  [SRS_Port_12300]  Unused pins set to defined state
 *  [SRS_Port_12405]  Runtime direction-change service
 *  [SRS_Port_12406]  Direction-refresh service
 *  [SRS_Port_12423]  Atomic port register access
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
 * |         |            |            | interface (CP R24-11).               |
 * |---------|------------|------------|--------------------------------------|
 */

/*===========================================================================*
 * INCLUDES
 *===========================================================================*/

#include "Port.h"
#include "Port_Cfg.h"

/* [SWS_Port_00194-equivalent] Include Det.h when dev-error detection is ON */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  #include "Det.h"
#endif

/*===========================================================================*
 * VERSION CONSISTENCY CHECK
 * Ensures that Port.c and Port.h were compiled from the same release.
 * [SWS_Port_00042 / SWS BSW General, section 5.1.8]
 *===========================================================================*/

#if (PORT_AR_RELEASE_MAJOR_VERSION != PORT_CFG_AR_MAJOR_VERSION)
  #error "Port.c: AUTOSAR major version mismatch with Port_Cfg.h"
#endif
#if (PORT_AR_RELEASE_MINOR_VERSION != PORT_CFG_AR_MINOR_VERSION)
  #error "Port.c: AUTOSAR minor version mismatch with Port_Cfg.h"
#endif

/*===========================================================================*
 * PRIVATE CONSTANTS AND MACROS
 *===========================================================================*/

/**
 * @brief   HAL GPIO mode mask for the alternate-function bits in MODER.
 * @details STM32H7 HAL encodes GPIO_MODE_AF_PP as 0x00000002UL in the lower
 *          nibble of the mode word; this mask isolates that field.
 */
#define PORT_PRIVATE_HAL_MODE_AF_MASK   (0x00000002UL)

/**
 * @brief   HAL GPIO mode value that indicates an output pin.
 * @details GPIO_MODE_OUTPUT_PP == 0x00000001UL in STM32 HAL.
 */
#define PORT_PRIVATE_HAL_MODE_OUTPUT    (GPIO_MODE_OUTPUT_PP)

/**
 * @brief   BSRR shift to reach the reset (upper) half of the register.
 */
#define PORT_PRIVATE_BSRR_RESET_SHIFT   (16U)

/**
 * @brief   Macro: report a development error through DET and execute
 *          the given return statement.
 * @details [SWS_Port_00087] When DET is enabled and an error is detected,
 *          the current function shall be aborted (return without action).
 *          The macro is a no-op when PORT_DEV_ERROR_DETECT == STD_OFF.
 *
 * @param[in] ApiId       SERVICE ID of the caller (PORT_SID_xxx).
 * @param[in] ErrorId     Error code    (PORT_E_xxx).
 * @param[in] return_stmt Full return statement to execute after DET call,
 *                        e.g. "return" or "return STD_LOW".
 */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  #define PORT_DET_REPORT_ERROR(ApiId, ErrorId, return_stmt)             \
      do {                                                                \
          (void)Det_ReportError((uint16)PORT_MODULE_ID,                \
                                (uint8) PORT_INSTANCE_ID,              \
                                (uint8)(ApiId),                        \
                                (uint8)(ErrorId));                     \
          return_stmt;                                                    \
      } while (0)
#else
  #define PORT_DET_REPORT_ERROR(ApiId, ErrorId, return_stmt)             \
      do { return_stmt; } while (0)
#endif

/*===========================================================================*
 * PRIVATE TYPES
 *===========================================================================*/

/**
 * @brief   Internal state of the Port Driver module.
 */
typedef struct
{
    /**
     * @brief   Pointer to the active configuration set.
     * @details Set by Port_Init(); used by all other API functions.
     *          NULL_PTR when the module is uninitialised.
     */
    const Port_ConfigType *ConfigPtr;

    /**
     * @brief   Initialisation flag.
     * @details TRUE after a successful Port_Init(); FALSE before.
     *          [SWS_Port_00087] API functions check this flag when
     *          PORT_DEV_ERROR_DETECT == STD_ON.
     */
    boolean IsInitialised;

} Port_StateType;

/*===========================================================================*
 * PRIVATE VARIABLES
 *===========================================================================*/

/**
 * @brief   Module-level state variable.
 * @details [SWS_Port_00002] Initialised to its zero/false state at
 *          programme start by the C runtime; explicitly set by Port_Init().
 */
static Port_StateType Port_State =
{
    .ConfigPtr     = NULL_PTR,
    .IsInitialised = FALSE
};

/*===========================================================================*
 * PRIVATE HELPER PROTOTYPES
 *===========================================================================*/

static void     Port_lEnableGpioClock(const GPIO_TypeDef *GpioPort);
static boolean  Port_lIsPinIdValid   (Port_PinType PinId,
                                      const Port_ConfigType *ConfigPtr,
                                      uint16 *IndexOut);
static void     Port_lApplyHalInit   (const Port_PinConfigType *PinCfg);

/*===========================================================================*
 * PUBLIC API IMPLEMENTATION
 *===========================================================================*/

/*---------------------------------------------------------------------------*
 * Port_Init
 *---------------------------------------------------------------------------*/
/**
 * @brief   Initialises the Port Driver module.
 * @details See Port.h for the full specification.
 *
 * Sequence of operations per pin:
 *   1. Enable the GPIO clock for the pin's port.
 *   2. Pre-load the output-data register with the configured initial level
 *      BEFORE calling HAL_GPIO_Init() [SWS_Port_00055].
 *   3. Call HAL_GPIO_Init() to configure MODER, OTYPER, OSPEEDR, PUPDR and
 *      AFR in one coherent, glitch-free write [SWS_Port_00043].
 *
 * @param[in] ConfigPtr   Pointer to the configuration set.
 */
void Port_Init(const Port_ConfigType *ConfigPtr)
{
    uint16 pinIdx;

    /* ------------------------------------------------------------------ *
     * Parameter validation [SWS_Port_00051][SWS_Port_00077]              *
     * ------------------------------------------------------------------ */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if ((ConfigPtr == NULL_PTR) || (ConfigPtr->PinConfig == NULL_PTR) ||
        (ConfigPtr->NumPins == 0U))
    {
        PORT_DET_REPORT_ERROR(PORT_SID_INIT, PORT_E_INIT_FAILED, return);
    }
#else
    /*
     * Assumption: when DET is disabled the caller guarantees a valid pointer.
     * Use the pre-compiled default set if NULL_PTR is passed
     * [SWS_Port_00121].
     */
    if (ConfigPtr == NULL_PTR)
    {
        ConfigPtr = &PortConfigSet;
    }
#endif

    /* ------------------------------------------------------------------ *
     * Store the active configuration and mark as initialised              *
     * [SWS_Port_00002][SWS_Port_00003]                                   *
     * ------------------------------------------------------------------ */
    Port_State.ConfigPtr     = ConfigPtr;
    Port_State.IsInitialised = FALSE;   /* guard against partial init     */

    /* ------------------------------------------------------------------ *
     * Initialise every pin in the configuration table                     *
     * [SWS_Port_00041][SWS_Port_00042]                                   *
     * ------------------------------------------------------------------ */
    for (pinIdx = 0U; pinIdx < ConfigPtr->NumPins; pinIdx++)
    {
        const Port_PinConfigType *pPin = &ConfigPtr->PinConfig[pinIdx];

        /* Step 1: enable the GPIO clock for this port [SWS_Port_00042]  */
        Port_lEnableGpioClock(pPin->GpioPort);

        /* Step 2: pre-load output latch before switching to output mode  *
         *         [SWS_Port_00055] prevents glitch on output pins        */
        if (pPin->InitialLevel != STD_LOW)
        {
            /* Drive pin HIGH atomically via BSRR set-half */
            pPin->GpioPort->BSRR = pPin->GpioPinMask;
        }
        else
        {
            /* Drive pin LOW atomically via BSRR reset-half */
            pPin->GpioPort->BSRR =
                (pPin->GpioPinMask << PORT_PRIVATE_BSRR_RESET_SHIFT);
        }

        /* Step 3: configure MODER / OTYPER / OSPEEDR / PUPDR / AFR      *
         *         [SWS_Port_00004][SWS_Port_00079][SWS_Port_00043]       */
        Port_lApplyHalInit(pPin);
    }

    /* ------------------------------------------------------------------ *
     * Module successfully initialised [SWS_Port_00001]                   *
     * ------------------------------------------------------------------ */
    Port_State.IsInitialised = TRUE;
}

/*---------------------------------------------------------------------------*
 * Port_SetPinDirection
 *---------------------------------------------------------------------------*/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/**
 * @brief   Sets the direction of a port pin during runtime.
 * @details See Port.h for the full specification.
 *
 * Implementation detail:
 *   The function modifies only the two MODER bits that correspond to the
 *   target pin using a read-modify-write on GPIO->MODER.  The output-data
 *   latch (ODR / BSRR) is never touched, satisfying [SWS_Port_00138].
 *
 *   The read-modify-write is NOT hardware-atomic.  If the same port is
 *   accessed concurrently (e.g. from an ISR), the caller must protect this
 *   call with a critical section.  This is acceptable per the AUTOSAR Port
 *   Driver constraint [Section 4.1] which places that responsibility on the
 *   integrator.
 *
 * @param[in] Pin         Symbolic pin ID.
 * @param[in] Direction   PORT_PIN_IN or PORT_PIN_OUT.
 */
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    uint16 pinIdx = 0U;

    /* ------------------------------------------------------------------ *
     * DET: check module initialisation [SWS_Port_00087]                  *
     * ------------------------------------------------------------------ */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_State.IsInitialised == FALSE)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_SET_PIN_DIRECTION,
                              PORT_E_UNINIT,
                              return);
    }

    /* DET: validate Pin ID [SWS_Port_00077]                              */
    if (Port_lIsPinIdValid(Pin, Port_State.ConfigPtr, &pinIdx) == FALSE)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_SET_PIN_DIRECTION,
                              PORT_E_PARAM_PIN,
                              return);
    }

    /* DET: check DirectionChangeable [SWS_Port_00077]                    */
    if (Port_State.ConfigPtr->PinConfig[pinIdx].DirectionChangeable == FALSE)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_SET_PIN_DIRECTION,
                              PORT_E_DIRECTION_UNCHANGEABLE,
                              return);
    }
#else
    (void)Port_lIsPinIdValid(Pin, Port_State.ConfigPtr, &pinIdx);
#endif

    {
        const Port_PinConfigType *pPin =
            &Port_State.ConfigPtr->PinConfig[pinIdx];
        GPIO_TypeDef *pGpio  = pPin->GpioPort;
        uint32     pinNum = 0U;
        uint32     tmpMask;

        /* Determine pin number from HAL bitmask (log2 of GpioPinMask)  */
        tmpMask = pPin->GpioPinMask;
        while ((tmpMask & 0x1UL) == 0UL)
        {
            pinNum++;
            tmpMask >>= 1U;
        }

        /* Modify only the two MODER bits for this pin                    *
         * [SWS_Port_00063][SWS_Port_00138]                               */
        {
            uint32 moder = pGpio->MODER;

            /* Clear the two bits for this pin */
            moder &= ~(0x3UL << (pinNum * 2U));

            if (Direction == PORT_PIN_OUT)
            {
                /* Set to output mode (01b) */
                moder |= (0x1UL << (pinNum * 2U));
            }
            /* else input mode (00b) – bits already cleared */

            pGpio->MODER = moder;
        }
    }
}
#endif /* PORT_SET_PIN_DIRECTION_API */

/*---------------------------------------------------------------------------*
 * Port_RefreshPortDirection
 *---------------------------------------------------------------------------*/
/**
 * @brief   Refreshes the direction of all configured port pins.
 * @details See Port.h for the full specification.
 *
 * Implementation detail:
 *   Iterates the configuration table and restores the MODER bits only for
 *   pins with DirectionChangeable == FALSE.  Pins marked as direction-
 *   changeable retain whatever direction was set at runtime [SWS_Port_00061].
 *
 *   The same read-modify-write on MODER applies as in Port_SetPinDirection;
 *   the integrator is responsible for ensuring exclusive access if needed.
 */
void Port_RefreshPortDirection(void)
{
    uint16 pinIdx;

    /* ------------------------------------------------------------------ *
     * DET: check module initialisation [SWS_Port_00087]                  *
     * ------------------------------------------------------------------ */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_State.IsInitialised == FALSE)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_REFRESH_PORT_DIRECTION,
                              PORT_E_UNINIT,
                              return);
    }
#endif

    for (pinIdx = 0U; pinIdx < Port_State.ConfigPtr->NumPins; pinIdx++)
    {
        const Port_PinConfigType *pPin =
            &Port_State.ConfigPtr->PinConfig[pinIdx];

        /* [SWS_Port_00061] Skip direction-changeable pins                */
        if (pPin->DirectionChangeable == TRUE)
        {
            continue;
        }

        /* Restore configured direction in MODER [SWS_Port_00060]         */
        {
            GPIO_TypeDef *pGpio  = pPin->GpioPort;
            uint32     pinNum = 0U;
            uint32     tmpMask = pPin->GpioPinMask;
            uint32     moder;

            while ((tmpMask & 0x1UL) == 0UL)
            {
                pinNum++;
                tmpMask >>= 1U;
            }

            moder  = pGpio->MODER;
            moder &= ~(0x3UL << (pinNum * 2U));  /* clear direction bits */

            if (pPin->Direction == PORT_PIN_OUT)
            {
                moder |= (0x1UL << (pinNum * 2U));  /* restore output    */
            }
            /* else input (00b) – bits already cleared                    */

            pGpio->MODER = moder;
        }
    }
}

/*---------------------------------------------------------------------------*
 * Port_GetVersionInfo
 *---------------------------------------------------------------------------*/
#if (PORT_VERSION_INFO_API == STD_ON)
/**
 * @brief   Returns the version information of the Port Driver module.
 * @details See Port.h for the full specification.
 */
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    /* ------------------------------------------------------------------ *
     * DET: NULL pointer check [SWS_Port_00225]                           *
     * ------------------------------------------------------------------ */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_GET_VERSION_INFO,
                              PORT_E_PARAM_POINTER,
                              return);
    }
#endif

    versioninfo->vendorID         = (uint16)PORT_VENDOR_ID;
    versioninfo->moduleID         = (uint16)PORT_MODULE_ID;
    versioninfo->sw_major_version = (uint8) PORT_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = (uint8) PORT_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = (uint8) PORT_SW_PATCH_VERSION;
}
#endif /* PORT_VERSION_INFO_API */

/*---------------------------------------------------------------------------*
 * Port_SetPinMode
 *---------------------------------------------------------------------------*/
#if (PORT_SET_PIN_MODE_API == STD_ON)
/**
 * @brief   Sets the port pin mode during runtime.
 * @details See Port.h for the full specification.
 *
 * Implementation detail:
 *   Reconfigures a single pin by rebuilding a GPIO_InitTypeDef from the
 *   stored configuration entry and substituting the new mode.  Using
 *   HAL_GPIO_Init() for a single pin ensures that all associated registers
 *   (MODER, OTYPER, OSPEEDR, PUPDR, AFR) are updated coherently.
 *
 * @param[in] Pin    Symbolic pin ID.
 * @param[in] Mode   New pin mode (PORT_PIN_MODE_GPIO, PORT_PIN_MODE_AF(n),
 *                   PORT_PIN_MODE_ANALOG).
 */
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    uint16 pinIdx = 0U;

    /* ------------------------------------------------------------------ *
     * DET: check module initialisation [SWS_Port_00087]                  *
     * ------------------------------------------------------------------ */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    if (Port_State.IsInitialised == FALSE)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_SET_PIN_MODE, PORT_E_UNINIT, return);
    }

    /* DET: validate Pin ID [SWS_Port_00077]                              */
    if (Port_lIsPinIdValid(Pin, Port_State.ConfigPtr, &pinIdx) == FALSE)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_SET_PIN_MODE,
                              PORT_E_PARAM_PIN,
                              return);
    }

    /* DET: check ModeChangeable [SWS_Port_00223]                         */
    if (Port_State.ConfigPtr->PinConfig[pinIdx].ModeChangeable == FALSE)
    {
        PORT_DET_REPORT_ERROR(PORT_SID_SET_PIN_MODE,
                              PORT_E_MODE_UNCHANGEABLE,
                              return);
    }

    /* DET: validate Mode value – AF modes are 0x10 … 0x1F, GPIO = 0x00, *
     *      Analog = 0x03; any other value is invalid [SWS_Port_00051]   */
    if ((Mode != PORT_PIN_MODE_GPIO) &&
        (Mode != PORT_PIN_MODE_ANALOG) &&
        (Mode < PORT_PIN_MODE_AF(0U) || Mode > PORT_PIN_MODE_AF(15U)))
    {
        PORT_DET_REPORT_ERROR(PORT_SID_SET_PIN_MODE,
                              PORT_E_PARAM_INVALID_MODE,
                              return);
    }
#else
    (void)Port_lIsPinIdValid(Pin, Port_State.ConfigPtr, &pinIdx);
#endif

    {
        const Port_PinConfigType *pPin =
            &Port_State.ConfigPtr->PinConfig[pinIdx];
        GPIO_InitTypeDef gpioInit = {0};

        gpioInit.Pin   = pPin->GpioPinMask;
        gpioInit.Pull  = pPin->Pull;
        gpioInit.Speed = pPin->Speed;

        if (Mode == PORT_PIN_MODE_GPIO)
        {
            /*
             * Pure GPIO mode: use the pin's configured direction to select
             * push-pull input or output.
             */
            gpioInit.Mode      = (pPin->Direction == PORT_PIN_OUT)
                                 ? GPIO_MODE_OUTPUT_PP
                                 : GPIO_MODE_INPUT;
            gpioInit.Alternate = 0U;
        }
        else if (Mode == PORT_PIN_MODE_ANALOG)
        {
            gpioInit.Mode      = GPIO_MODE_ANALOG;
            gpioInit.Alternate = 0U;
        }
        else
        {
            /* Alternate function: Mode encodes 0x10 + AF index           */
            gpioInit.Mode      = GPIO_MODE_AF_PP;
            gpioInit.Alternate = (uint32_t)(Mode - PORT_PIN_MODE_AF(0U));
        }

        HAL_GPIO_Init(pPin->GpioPort, &gpioInit);
    }
}
#endif /* PORT_SET_PIN_MODE_API */

/*===========================================================================*
 * PRIVATE HELPER IMPLEMENTATIONS
 *===========================================================================*/

/*---------------------------------------------------------------------------*
 * Port_lEnableGpioClock
 *---------------------------------------------------------------------------*/
/**
 * @brief   Enables the AHB4 clock for a given GPIO port peripheral.
 * @details [SWS_Port_00042] Port_Init shall initialise all configured
 *          resources.  Only the clocks that are actually referenced in the
 *          configuration table are enabled; unused GPIO clocks remain off.
 *
 *          Assumption: the HAL __HAL_RCC_GPIOx_CLK_ENABLE() macros are
 *          idempotent – calling them multiple times for the same port is safe.
 *
 * @param[in] GpioPort   Base address of the GPIO peripheral.
 */
static void Port_lEnableGpioClock(const GPIO_TypeDef *GpioPort)
{
    if      (GpioPort == GPIOA) { __HAL_RCC_GPIOA_CLK_ENABLE(); }
    else if (GpioPort == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); }
    else if (GpioPort == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); }
    else if (GpioPort == GPIOD) { __HAL_RCC_GPIOD_CLK_ENABLE(); }
    else if (GpioPort == GPIOE) { __HAL_RCC_GPIOE_CLK_ENABLE(); }
    else if (GpioPort == GPIOF) { __HAL_RCC_GPIOF_CLK_ENABLE(); }
    else if (GpioPort == GPIOG) { __HAL_RCC_GPIOG_CLK_ENABLE(); }
    else if (GpioPort == GPIOH) { __HAL_RCC_GPIOH_CLK_ENABLE(); }
    else if (GpioPort == GPIOI) { __HAL_RCC_GPIOI_CLK_ENABLE(); }
    else if (GpioPort == GPIOJ) { __HAL_RCC_GPIOJ_CLK_ENABLE(); }
    else if (GpioPort == GPIOK) { __HAL_RCC_GPIOK_CLK_ENABLE(); }
    else
    {
        /* Unknown port base address – silently ignored.                  *
         * DET validation in Port_Init would have caught an invalid entry  *
         * in a properly configured table.                                 */
    }
}

/*---------------------------------------------------------------------------*
 * Port_lIsPinIdValid
 *---------------------------------------------------------------------------*/
/**
 * @brief   Searches the configuration table for a given Port_PinType ID.
 * @details Returns TRUE and writes the table index to *IndexOut when found.
 *          Returns FALSE when the ID is not present in the active
 *          configuration set.
 *
 * @param[in]  PinId      Pin identifier to look up.
 * @param[in]  ConfigPtr  Active configuration set.
 * @param[out] IndexOut   Table index of the matching entry (valid only when
 *                        the function returns TRUE).
 * @return     boolean    TRUE if found, FALSE otherwise.
 */
static boolean Port_lIsPinIdValid(Port_PinType            PinId,
                                  const Port_ConfigType  *ConfigPtr,
                                  uint16               *IndexOut)
{
    uint16 idx;

    if ((ConfigPtr == NULL_PTR) || (IndexOut == NULL_PTR))
    {
        return FALSE;
    }

    for (idx = 0U; idx < ConfigPtr->NumPins; idx++)
    {
        if (ConfigPtr->PinConfig[idx].PinId == PinId)
        {
            *IndexOut = idx;
            return TRUE;
        }
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*
 * Port_lApplyHalInit
 *---------------------------------------------------------------------------*/
/**
 * @brief   Translates one Port_PinConfigType entry into a HAL_GPIO_Init call.
 * @details Builds a GPIO_InitTypeDef from the configuration record and
 *          calls HAL_GPIO_Init().  The function handles three cases:
 *
 *          1. GPIO output / input  (HalMode is not an AF mode)
 *          2. Alternate function   (HalMode == GPIO_MODE_AF_PP or _OD)
 *          3. Analog               (HalMode == GPIO_MODE_ANALOG)
 *
 *          The caller is responsible for pre-loading the output latch
 *          (BSRR) before calling this function [SWS_Port_00055].
 *
 * @param[in] PinCfg   Pointer to the pin configuration entry.
 */
static void Port_lApplyHalInit(const Port_PinConfigType *PinCfg)
{
    GPIO_InitTypeDef gpioInit = {0};

    gpioInit.Pin       = PinCfg->GpioPinMask;
    gpioInit.Mode      = PinCfg->HalMode;
    gpioInit.Pull      = PinCfg->Pull;
    gpioInit.Speed     = PinCfg->Speed;
    gpioInit.Alternate = PinCfg->Alternate;

    HAL_GPIO_Init(PinCfg->GpioPort, &gpioInit);
}
