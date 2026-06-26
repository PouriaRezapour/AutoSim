/**
 * @file    Dio.c
 * @brief   AUTOSAR DIO Driver Implementation – STM32H753
 * @details Implements the full AUTOSAR DIO Driver API (AUTOSAR CP R24-11,
 *          Document ID 20: AUTOSAR_CP_SWS_DIODriver) targeting the
 *          STM32H753 microcontroller.
 *
 *  Architecture notes
 *  ------------------
 *  STM32H753 has 11 GPIO ports (A..K), each with 16 pins, giving a total of
 *  176 channels.
 *
 *  Channel ID encoding (implementation-specific, [SWS_Dio_00180]):
 *      ChannelId = (PortIndex * 16) + PinNumber
 *      PortIndex: A=0 B=1 C=2 D=3 E=4 F=5 G=6 H=7 I=8 J=9 K=10
 *
 *  Port ID encoding ([SWS_Dio_00181]):
 *      PortId directly matches the PortIndex above (0..10).
 *
 *  GPIO register usage
 *  -------------------
 *  Read  → GPIOx->IDR   (always reflects actual pin level; satisfies
 *                         [SWS_Dio_00083] for output pins too on STM32H7)
 *  Write → GPIOx->BSRR  (atomic set/reset; no read-modify-write on IDR/ODR;
 *                         satisfies [SWS_Dio_00005])
 *  Port write that must preserve input channels uses MODER to build the
 *  output-only mask and then uses BSRR for atomicity.
 *
 *  All public functions are re-entrant ([SWS_Dio_00060]) because:
 *    • Reads use only IDR (read-only register, no side effects).
 *    • Writes use BSRR which is a write-only, non-shared, atomic register
 *      (set and reset happen in one 32-bit write; no concurrent race).
 *
 *  Implemented requirements
 *  ------------------------
 *  [SWS_Dio_00051] unbuffered synchronous read/write
 *  [SWS_Dio_00005] atomic / consistent data (BSRR)
 *  [SWS_Dio_00089] STD_HIGH / STD_LOW only
 *  [SWS_Dio_00060] re-entrant functions
 *  [SWS_Dio_00061] no port init API (done by PORT driver)
 *  [SWS_Dio_00001] no hardware init interface
 *  [SWS_Dio_00064] write works on input channels (no physical effect)
 *  [SWS_Dio_00070] write to input has no effect on physical output
 *  [SWS_Dio_00012] read works on input and output channels
 *  [SWS_Dio_00083] read of output pin returns real IDR value
 *  [SWS_Dio_00027] Dio_ReadChannel
 *  [SWS_Dio_00028/29/79] Dio_WriteChannel
 *  [SWS_Dio_00031/104/75] Dio_ReadPort
 *  [SWS_Dio_00034/35/105/108] Dio_WritePort
 *  [SWS_Dio_00037/92/93/114] Dio_ReadChannelGroup
 *  [SWS_Dio_00039/40/90/91/114] Dio_WriteChannelGroup
 *  [SWS_Dio_00139/189] Dio_GetVersionInfo
 *  [SWS_Dio_00191/192/193] Dio_FlipChannel
 *  [SWS_Dio_00200/201/202/203/204] Dio_MaskedWritePort
 *  [SWS_Dio_00074/75/114/119] DET reporting
 *  [SWS_Dio_00194] include Det.h when DET enabled
 *
 *  AUTOSAR CP R24-11
 */

/*===========================================================================*
 * INCLUDES
 *===========================================================================*/
#include "Dio.h"
#include "stm32h7xx.h"   /* Provides GPIO_TypeDef, GPIOA..GPIOK, etc.  */

/* [SWS_Dio_00194] Include Det.h when development error detection is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  #include "Det.h"
#endif

/*===========================================================================*
 * VERSION CONSISTENCY CHECK
 *===========================================================================*/
#if (DIO_AR_RELEASE_MAJOR_VERSION != DIO_CFG_AR_MAJOR_VERSION)
  #error "Dio.c: AUTOSAR major version mismatch with Dio_Cfg.h"
#endif
#if (DIO_AR_RELEASE_MINOR_VERSION != DIO_CFG_AR_MINOR_VERSION)
  #error "Dio.c: AUTOSAR minor version mismatch with Dio_Cfg.h"
#endif

/*===========================================================================*
 * PRIVATE CONSTANTS AND MACROS
 *===========================================================================*/

/** Number of pins in each GPIO port on STM32H753 */
#define DIO_PRIVATE_PINS_PER_PORT   (16U)

/** Number of bit positions to shift pin mask into BSRR reset half */
#define DIO_PRIVATE_BSRR_RESET_SHIFT (16U)

/** GPIO mode field width in MODER register (2 bits per pin) */
#define DIO_PRIVATE_MODER_PIN_SHIFT  (2U)

/** MODER mask for a single pin (2 bits) */
#define DIO_PRIVATE_MODER_PIN_MASK   (0x3U)

/** MODER value for output mode (01b) */
#define DIO_PRIVATE_MODER_OUTPUT     (0x1U)

/**
 * @brief   Report a development error via DET if detection is enabled.
 * @details [SWS_Dio_00119] Write functions shall NOT proceed after DET error.
 *          Macro issues Det_ReportError and makes the caller return (void) or
 *          a default value.  The "return_stmt" parameter is the full return
 *          statement to execute, e.g. "return STD_LOW" or "return".
 */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  #define DIO_DET_REPORT_ERROR(ApiId, ErrorId, return_stmt) \
      do { \
          (void)Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, \
                                (uint8)(ApiId), (uint8)(ErrorId)); \
          return_stmt; \
      } while (0)
#else
  #define DIO_DET_REPORT_ERROR(ApiId, ErrorId, return_stmt) \
      do { return_stmt; } while (0)
#endif

/*===========================================================================*
 * PRIVATE HELPER: port base address lookup
 *===========================================================================*/

/**
 * @brief   Returns the GPIO_TypeDef pointer for a given PortId.
 * @details The STM32H7 CMSIS header defines GPIOA..GPIOK as macros resolving
 *          to distinct GPIO_TypeDef* values.  We map by PortId index.
 *          If an invalid index is supplied (should be caught by DET before
 *          reaching this helper), the function returns NULL_PTR so callers
 *          must not use the result blindly.
 *
 * @param[in] PortId   Valid DIO port identifier (0..DIO_MAX_PORT_ID)
 * @return    GPIO_TypeDef*  Base address of the port peripheral, or NULL_PTR
 */
static GPIO_TypeDef *Dio_lGetPortBase(Dio_PortType PortId)
{
    /* Ordered table matching PortId 0..10 → GPIOA..GPIOK */
    static GPIO_TypeDef * const Dio_PortBaseTable[DIO_NUM_PORTS] =
    {
        GPIOA,  /* 0  */
        GPIOB,  /* 1  */
        GPIOC,  /* 2  */
        GPIOD,  /* 3  */
        GPIOE,  /* 4  */
        GPIOF,  /* 5  */
        GPIOG,  /* 6  */
        GPIOH,  /* 7  */
        GPIOI,  /* 8  */
        GPIOJ,  /* 9  */
        GPIOK   /* 10 */
    };

    if (PortId < DIO_NUM_PORTS)
    {
        return Dio_PortBaseTable[PortId];
    }
    return (GPIO_TypeDef *)NULL_PTR;
}

/*===========================================================================*
 * PRIVATE HELPER: validate ChannelId
 *===========================================================================*/

/**
 * @brief   Returns STD_HIGH (valid) or STD_LOW (invalid) for a ChannelId.
 * @details A ChannelId is valid when its port index < DIO_NUM_PORTS and its
 *          pin number is in 0..15.  Since all STM32H753 ports have 16 pins,
 *          any ChannelId <= DIO_MAX_CHANNEL_ID is structurally valid.
 *
 * @param[in] ChannelId
 * @return    boolean  TRUE if valid, FALSE otherwise
 */
static boolean Dio_lIsChannelValid(Dio_ChannelType ChannelId)
{
    return (ChannelId <= DIO_MAX_CHANNEL_ID) ? TRUE : FALSE;
}

/*===========================================================================*
 * PRIVATE HELPER: validate PortId
 *===========================================================================*/

/**
 * @brief   Returns TRUE if PortId is within range.
 *
 * @param[in] PortId
 * @return    boolean
 */
static boolean Dio_lIsPortValid(Dio_PortType PortId)
{
    return (PortId <= DIO_MAX_PORT_ID) ? TRUE : FALSE;
}

/*===========================================================================*
 * PRIVATE HELPER: validate ChannelGroupType pointer
 *===========================================================================*/

/**
 * @brief   Returns TRUE when the ChannelGroupIdPtr and its contents are valid.
 * @details Checks:
 *   1. Pointer is not NULL.
 *   2. Port index is in range.
 *   3. Mask is not zero.
 *   4. Offset is in 0..15.
 *   5. All set bits in mask fall within a 16-bit port width.
 *
 * @param[in] ChannelGroupIdPtr
 * @return    boolean
 */
static boolean Dio_lIsChannelGroupValid(const Dio_ChannelGroupType *ChannelGroupIdPtr)
{
    if (ChannelGroupIdPtr == NULL_PTR)
    {
        return FALSE;
    }
    if (ChannelGroupIdPtr->port > DIO_MAX_PORT_ID)
    {
        return FALSE;
    }
    if (ChannelGroupIdPtr->mask == 0U)
    {
        return FALSE;
    }
    if (ChannelGroupIdPtr->offset >= DIO_PRIVATE_PINS_PER_PORT)
    {
        return FALSE;
    }
    return TRUE;
}

/*===========================================================================*
 * API IMPLEMENTATION
 *===========================================================================*/

/*---------------------------------------------------------------------------*
 * Dio_ReadChannel
 *---------------------------------------------------------------------------*/
/**
 * @brief   Returns the value of the specified DIO channel.
 *
 * Implementation detail:
 *   STM32H753 IDR (Input Data Register) always reflects the actual pin
 *   state whether the pin is configured as input or output, satisfying
 *   [SWS_Dio_00083] (real pin level for output pins).
 */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
    Dio_LevelType level = STD_LOW;

    /* [SWS_Dio_00074] Validate ChannelId when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsChannelValid(ChannelId) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_READ_CHANNEL,
                             DIO_E_PARAM_INVALID_CHANNEL_ID,
                             return STD_LOW);
    }
#endif

    {
        Dio_PortType  portId  = (Dio_PortType)(ChannelId / DIO_PRIVATE_PINS_PER_PORT);
        uint8       pinNum  = (uint8)      (ChannelId % DIO_PRIVATE_PINS_PER_PORT);
        GPIO_TypeDef *pPort   = Dio_lGetPortBase(portId);
        uint16      pinMask = (uint16)(1U << pinNum);

        /* [SWS_Dio_00051] Unbuffered: read directly from IDR */
        if ((pPort->IDR & pinMask) != 0U)
        {
            level = STD_HIGH;
        }
        /* else level stays STD_LOW */
    }

    return level;
}

/*---------------------------------------------------------------------------*
 * Dio_WriteChannel
 *---------------------------------------------------------------------------*/
/**
 * @brief   Sets the level of a single DIO channel.
 *
 * Implementation detail:
 *   Uses GPIOx->BSRR for atomic set/reset [SWS_Dio_00005]:
 *     • Set:   BSRR = pinMask           (lower 16 bits = set bits)
 *     • Reset: BSRR = pinMask << 16     (upper 16 bits = reset bits)
 *   This prevents any race condition between the read and write phases that
 *   would occur with a read-modify-write on ODR.
 *
 *   [SWS_Dio_00029][SWS_Dio_00079] When the pin is configured as input,
 *   writing to BSRR sets the output data latch (ODR) but the physical pin
 *   level is not affected.  This satisfies [SWS_Dio_00109] as well –
 *   the pre-set value appears on the pin when the PORT driver later
 *   reconfigures it as output.
 */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    /* [SWS_Dio_00074] Validate ChannelId when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsChannelValid(ChannelId) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_WRITE_CHANNEL,
                             DIO_E_PARAM_INVALID_CHANNEL_ID,
                             return);
    }
#endif

    {
        Dio_PortType  portId  = (Dio_PortType)(ChannelId / DIO_PRIVATE_PINS_PER_PORT);
        uint8       pinNum  = (uint8)      (ChannelId % DIO_PRIVATE_PINS_PER_PORT);
        GPIO_TypeDef *pPort   = Dio_lGetPortBase(portId);
        uint32     pinMask = (uint32_t)(1UL << pinNum);

        /* [SWS_Dio_00006] Set to STD_HIGH or STD_LOW atomically via BSRR */
        if (Level != STD_LOW)
        {
            /* Set the pin: write to lower half of BSRR */
            pPort->BSRR = pinMask;
        }
        else
        {
            /* Reset the pin: write to upper half of BSRR */
            pPort->BSRR = (pinMask << DIO_PRIVATE_BSRR_RESET_SHIFT);
        }
    }
}

/*---------------------------------------------------------------------------*
 * Dio_ReadPort
 *---------------------------------------------------------------------------*/
/**
 * @brief   Returns the level of all channels of a port.
 *
 * Implementation detail:
 *   IDR is 32 bits wide but only the lower 16 bits correspond to valid GPIO
 *   pins on STM32H753.  The upper 16 bits of IDR are always 0, so the cast
 *   to Dio_PortLevelType (uint16) naturally masks them – satisfying
 *   [SWS_Dio_00104] (undefined pins returned as 0).
 */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
    Dio_PortLevelType portLevel = 0U;

    /* [SWS_Dio_00075] Validate PortId when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsPortValid(PortId) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_READ_PORT,
                             DIO_E_PARAM_INVALID_PORT_ID,
                             return 0U);
    }
#endif

    {
        GPIO_TypeDef *pPort = Dio_lGetPortBase(PortId);

        /* [SWS_Dio_00031] [SWS_Dio_00051] Direct, unbuffered IDR read */
        portLevel = (Dio_PortLevelType)(pPort->IDR & 0xFFFFU);
    }

    return portLevel;
}

/*---------------------------------------------------------------------------*
 * Dio_WritePort
 *---------------------------------------------------------------------------*/
/**
 * @brief   Sets the levels of all output channels of a port simultaneously.
 *
 * Implementation detail (preserving input channels, [SWS_Dio_00035][SWS_Dio_00108]):
 *
 *   The STM32H753 MODER register holds a 2-bit field per pin:
 *     00 = Input, 01 = Output, 10 = Alternate, 11 = Analog
 *
 *   Strategy using BSRR for atomicity [SWS_Dio_00005]:
 *     1. Read MODER and build a 16-bit mask of pins configured as output
 *        (MODER bits == 01b).
 *     2. Intersect the caller's Level with this output mask to get the bits
 *        to set and the bits to reset.
 *     3. Write a single 32-bit value to BSRR:
 *          upper 16 bits → pins to reset  (output pins where Level bit == 0)
 *          lower 16 bits → pins to set    (output pins where Level bit == 1)
 *     This is atomic and never touches input or alternate-function pins.
 */
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    /* [SWS_Dio_00075] Validate PortId when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsPortValid(PortId) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_WRITE_PORT,
                             DIO_E_PARAM_INVALID_PORT_ID,
                             return);
    }
#endif

    {
        GPIO_TypeDef *pPort = Dio_lGetPortBase(PortId);
        uint32     moder;
        uint16      outputMask;
        uint8       pin;
        uint16      bitsToSet;
        uint16      bitsToReset;

        /* Step 1: identify which pins are configured as output in MODER */
        moder      = pPort->MODER;
        outputMask = 0U;
        for (pin = 0U; pin < DIO_PRIVATE_PINS_PER_PORT; pin++)
        {
            uint32 moderField = (moder >> (pin * DIO_PRIVATE_MODER_PIN_SHIFT))
                                  & DIO_PRIVATE_MODER_PIN_MASK;
            if (moderField == DIO_PRIVATE_MODER_OUTPUT)
            {
                outputMask |= (uint16)(1U << pin);
            }
        }

        /* Step 2: restrict Level to output pins only
         *         [SWS_Dio_00035][SWS_Dio_00108] input pins remain unchanged */
        bitsToSet   = (uint16)( Level & outputMask);
        bitsToReset = (uint16)(~Level & outputMask);

        /* Step 3: atomic write via BSRR [SWS_Dio_00005]
         *   lower 16 bits: pins to set HIGH
         *   upper 16 bits: pins to set LOW  */
        pPort->BSRR = ((uint32_t)bitsToReset << DIO_PRIVATE_BSRR_RESET_SHIFT)
                      | (uint32_t)bitsToSet;
    }
}

/*---------------------------------------------------------------------------*
 * Dio_ReadChannelGroup
 *---------------------------------------------------------------------------*/
/**
 * @brief   Reads a subset of adjoining bits (channel group) from a port.
 *
 * Implementation detail:
 *   Raw = IDR & mask  → isolates the group bits within the port word.
 *   Shifted = Raw >> offset → aligns them to the LSB per [SWS_Dio_00093].
 */
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr)
{
    Dio_PortLevelType groupLevel = 0U;

    /* [SWS_Dio_00114] Validate pointer and content when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsChannelGroupValid(ChannelGroupIdPtr) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_READ_CHANNEL_GROUP,
                             DIO_E_PARAM_INVALID_GROUP,
                             return 0U);
    }
#endif

    {
        GPIO_TypeDef *pPort = Dio_lGetPortBase(ChannelGroupIdPtr->port);

        /* [SWS_Dio_00037] [SWS_Dio_00092] apply mask to IDR */
        uint16 rawBits = (uint16)(pPort->IDR & ChannelGroupIdPtr->mask);

        /* [SWS_Dio_00093] shift right to LSB-align the result */
        groupLevel = (Dio_PortLevelType)(rawBits >> ChannelGroupIdPtr->offset);
    }

    return groupLevel;
}

/*---------------------------------------------------------------------------*
 * Dio_WriteChannelGroup
 *---------------------------------------------------------------------------*/
/**
 * @brief   Sets a channel group to a specified level.
 *
 * Implementation detail (atomic, [SWS_Dio_00005]; preserves other pins,
 * [SWS_Dio_00040]):
 *
 *   1. Shift the caller's LSB-aligned Level left by offset.
 *   2. Apply the channel group mask.
 *   3. Compute bits to set and bits to reset within the mask.
 *   4. Write in one BSRR transaction.
 *
 *   Input channels within the group are handled transparently: writing
 *   to BSRR updates ODR but does not affect the physical state of input
 *   pins [SWS_Dio_00070].
 */
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr,
                           Dio_PortLevelType           Level)
{
    /* [SWS_Dio_00114] Validate pointer and content when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsChannelGroupValid(ChannelGroupIdPtr) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_WRITE_CHANNEL_GROUP,
                             DIO_E_PARAM_INVALID_GROUP,
                             return);
    }
#endif

    {
        GPIO_TypeDef *pPort = Dio_lGetPortBase(ChannelGroupIdPtr->port);

        /* [SWS_Dio_00091] shift level left to align with port bit positions */
        uint16 shiftedLevel = (uint16)
                                ((Level << ChannelGroupIdPtr->offset)
                                  & ChannelGroupIdPtr->mask);

        /* Bits within mask that should be HIGH */
        uint16 bitsToSet    = shiftedLevel;
        /* Bits within mask that should be LOW */
        uint16 bitsToReset  = (uint16)(ChannelGroupIdPtr->mask & ~shiftedLevel);

        /* [SWS_Dio_00039][SWS_Dio_00090] atomic write, only mask bits affected */
        pPort->BSRR = ((uint32_t)bitsToReset << DIO_PRIVATE_BSRR_RESET_SHIFT)
                      | (uint32_t)bitsToSet;
    }
}

/*---------------------------------------------------------------------------*
 * Dio_GetVersionInfo
 *---------------------------------------------------------------------------*/
#if (DIO_VERSION_INFO_API == STD_ON)
/**
 * @brief   Returns version information of the DIO module.
 */
void Dio_GetVersionInfo(Std_VersionInfoType *VersionInfo)
{
    /* [SWS_Dio_00189] Report DIO_E_PARAM_POINTER if VersionInfo is NULL */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (VersionInfo == NULL_PTR)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_GET_VERSION_INFO,
                             DIO_E_PARAM_POINTER,
                             return);
    }
#endif

    VersionInfo->vendorID         = (uint16)DIO_VENDOR_ID;
    VersionInfo->moduleID         = (uint16)DIO_MODULE_ID;
    VersionInfo->sw_major_version = (uint8) DIO_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = (uint8) DIO_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = (uint8) DIO_SW_PATCH_VERSION;
}
#endif /* DIO_VERSION_INFO_API */

/*---------------------------------------------------------------------------*
 * Dio_FlipChannel
 *---------------------------------------------------------------------------*/
#if (DIO_FLIP_CHANNEL_API == STD_ON)
/**
 * @brief   Flips the level of a DIO channel and returns the new level.
 *
 * Implementation detail ([SWS_Dio_00191]):
 *   For output channels:
 *     1. Read the current bit from IDR (real pin level, [SWS_Dio_00083]).
 *     2. If currently HIGH → reset via BSRR; return STD_LOW.
 *        If currently LOW  → set   via BSRR; return STD_HIGH.
 *   The read and write are NOT guaranteed to be a single atomic hardware
 *   transaction; however they are re-entrant because BSRR is the only write
 *   target and operates independently on each pin.  If strict hardware
 *   atomicity is required for a shared pin in an ISR context, callers must
 *   protect the call site with a critical section.
 *
 *   For input channels ([SWS_Dio_00192][SWS_Dio_00193]):
 *     No write is performed.  The current IDR value is returned.
 */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
    Dio_LevelType newLevel = STD_LOW;

    /* [SWS_Dio_00074] Validate ChannelId when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsChannelValid(ChannelId) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_FLIP_CHANNEL,
                             DIO_E_PARAM_INVALID_CHANNEL_ID,
                             return STD_LOW);
    }
#endif

    {
        Dio_PortType  portId  = (Dio_PortType)(ChannelId / DIO_PRIVATE_PINS_PER_PORT);
        uint8       pinNum  = (uint8)      (ChannelId % DIO_PRIVATE_PINS_PER_PORT);
        GPIO_TypeDef *pPort   = Dio_lGetPortBase(portId);
        uint32     pinMask = (uint32_t)(1UL << pinNum);

        /* [SWS_Dio_00191] Read current physical level from IDR */
        boolean pinIsHigh = ((pPort->IDR & (uint16)pinMask) != 0U) ? TRUE : FALSE;

        /* Determine MODER to decide if pin is output [SWS_Dio_00191/192] */
        uint32 moderField = (pPort->MODER >> (pinNum * DIO_PRIVATE_MODER_PIN_SHIFT))
                              & DIO_PRIVATE_MODER_PIN_MASK;

        if (moderField == DIO_PRIVATE_MODER_OUTPUT)
        {
            /* Output channel: invert and write back */
            if (pinIsHigh == TRUE)
            {
                /* Currently HIGH → set LOW (reset) */
                pPort->BSRR = (pinMask << DIO_PRIVATE_BSRR_RESET_SHIFT);
                newLevel    = STD_LOW;
            }
            else
            {
                /* Currently LOW → set HIGH */
                pPort->BSRR = pinMask;
                newLevel    = STD_HIGH;
            }
        }
        else
        {
            /* [SWS_Dio_00192] Input (or AF/analog) channel:
             * no write; return current pin level [SWS_Dio_00193] */
            newLevel = (pinIsHigh == TRUE) ? STD_HIGH : STD_LOW;
        }
    }

    return newLevel;
}
#endif /* DIO_FLIP_CHANNEL_API */

/*---------------------------------------------------------------------------*
 * Dio_MaskedWritePort
 *---------------------------------------------------------------------------*/
#if (DIO_MASKED_WRITE_PORT_API == STD_ON)
/**
 * @brief   Sets levels of selected output channels of a port using a mask.
 *
 * Implementation detail ([SWS_Dio_00200][SWS_Dio_00201][SWS_Dio_00202]):
 *   1. Read MODER to determine which masked pins are actual output channels.
 *   2. The effective mask = Mask & outputMask  (only output pins, [SWS_Dio_00201]).
 *   3. Within the effective mask, split into set and reset groups from Level.
 *   4. Single BSRR write for atomicity.
 *
 *   [SWS_Dio_00204] Bits in Level beyond the port width are ignored because
 *   BSRR only acts on the lower 16 bits for set and upper 16 bits for reset.
 */
void Dio_MaskedWritePort(Dio_PortType     PortId,
                         Dio_PortLevelType Level,
                         Dio_PortLevelType Mask)
{
    /* [SWS_Dio_00075] Validate PortId when DET is enabled */
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    if (Dio_lIsPortValid(PortId) == FALSE)
    {
        DIO_DET_REPORT_ERROR(DIO_SID_MASKED_WRITE_PORT,
                             DIO_E_PARAM_INVALID_PORT_ID,
                             return);
    }
#endif

    {
        GPIO_TypeDef *pPort = Dio_lGetPortBase(PortId);
        uint32     moder;
        uint16      outputMask;
        uint8       pin;
        uint16      effectiveMask;
        uint16      bitsToSet;
        uint16      bitsToReset;

        /* Build output-only mask from MODER */
        moder      = pPort->MODER;
        outputMask = 0U;
        for (pin = 0U; pin < DIO_PRIVATE_PINS_PER_PORT; pin++)
        {
            uint32 moderField = (moder >> (pin * DIO_PRIVATE_MODER_PIN_SHIFT))
                                  & DIO_PRIVATE_MODER_PIN_MASK;
            if (moderField == DIO_PRIVATE_MODER_OUTPUT)
            {
                outputMask |= (uint16)(1U << pin);
            }
        }

        /* [SWS_Dio_00201][SWS_Dio_00203] Restrict to selected output pins */
        effectiveMask = (uint16)(Mask & outputMask);

        /* Pins within effective mask that should be set or reset */
        bitsToSet    = (uint16)( Level & effectiveMask);
        bitsToReset  = (uint16)(~Level & effectiveMask);

        /* Atomic write via BSRR [SWS_Dio_00005] */
        pPort->BSRR = ((uint32_t)bitsToReset << DIO_PRIVATE_BSRR_RESET_SHIFT)
                      | (uint32_t)bitsToSet;
    }
}
#endif /* DIO_MASKED_WRITE_PORT_API */
