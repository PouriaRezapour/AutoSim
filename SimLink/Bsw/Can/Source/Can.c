/**
 * @file    Can.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Implementation of the CAN driver module.
 *
 * @details
 * Wraps the STM32H7 HAL FDCAN API to provide a simpler, project-level
 * interface for Classic CAN communication.  Peripheral clock configuration
 * is handled externally (RCC_PERIPHCLK_FDCAN sourced from PLL) and GPIO
 * pin assignment is handled by Port.c, consistent with the MSP pattern
 * used in stm32h7xx_hal_msp.c.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * FDCAN1 bit-timing (500 Kbit/s):
 *   NominalPrescaler  = 1
 *   NominalTimeSeg1   = 86  (Prop + Phase1)
 *   NominalTimeSeg2   = 13  (Phase2)
 *   NominalSJW        = 13
 *
 * Data-phase parameters are kept for HAL init completeness but are unused
 * because FrameFormat = FDCAN_FRAME_CLASSIC.
 *
 * NVIC: FDCAN1_IT0_IRQn is enabled here for future interrupt-driven use.
 * The current polling implementation does not depend on it.
 *
 * @par Revision History:
 * |---------|------------|------------|---------------------------------------|
 * | Version | Date       | Author     | Description                           |
 * |---------|------------|------------|---------------------------------------|
 * | 0.2.0   | 2025-05-23 | A.Rezapour | Initial release                       |
 * | 0.2.2   | 2025-06-01 | A.Rezapour | Add Can_TransmitBurst, move           |
 * |         |            |            | Can_HwAb_BuildTxHeader to private     |
 * |         |            |            | Can_BuildTxHeader_, switch Tx path    |
 * |         |            |            | to Tx FIFO queue                      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | Integrate Std_Types.h                 |
 * |---------|------------|------------|---------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

#include "Can.h"
#include "Can_HwAb.h"
#include "Port.h"

#include <string.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define LOG(fmt, ...)  /* Swap for project logger when developed */

/** Number of Tx FIFO queue slots allocated in Message RAM */
#define CAN_TX_FIFO_DEPTH_          3U

/* ─── Private Types ───────────────────────────────────────────────────────── */

typedef struct {
    int isInitialised;
    int isStarted;
} CanState_t;

/* ─── Private Variables ───────────────────────────────────────────────────── */

static FDCAN_HandleTypeDef s_hfdcan1;
static CanState_t          s_canState = { .isInitialised = 0, .isStarted = 0 };

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static CanStatus_t    Can_InitPeriphClk_(void);
static CanStatus_t    Can_ConfigNvic_(void);

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Initialise the FDCAN peripheral in Classic CAN mode.
 */
CanStatus_t Can_Init(void)
{
    if (s_canState.isInitialised)
    {
        LOG("CAN already initialised");
        return CAN_ALREADY_INIT;
    }

    /* Enable FDCAN kernel clock (sourced from PLL – see SystemClock_Config) */
    if (Can_InitPeriphClk_() != CAN_OK)
    {
        return CAN_ERROR;
    }

    /* Enable FDCAN peripheral clock */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    /* Configure FDCAN1 ─────────────────────────────────────────────────── */
    s_hfdcan1.Instance = FDCAN1;

    /* Frame format: Classic CAN (no FD, no BRS) */
    s_hfdcan1.Init.FrameFormat        = FDCAN_FRAME_CLASSIC;
    s_hfdcan1.Init.Mode               = FDCAN_MODE_NORMAL;
    s_hfdcan1.Init.AutoRetransmission = DISABLE;
    s_hfdcan1.Init.TransmitPause      = DISABLE;
    s_hfdcan1.Init.ProtocolException  = DISABLE;

    /* Nominal bit-timing: 1 Mbit/s @ 100 MHz FDCAN kernel clock
     * Bit time = (1 + NominalTimeSeg1 + NominalTimeSeg2) × NominalPrescaler
     *          = (1 + 86 + 13) × 1 = 100 TQ  →  1 Mbit/s               */
    s_hfdcan1.Init.NominalPrescaler      = 1U;
    s_hfdcan1.Init.NominalSyncJumpWidth  = 13U;
    s_hfdcan1.Init.NominalTimeSeg1       = 86U;
    s_hfdcan1.Init.NominalTimeSeg2       = 13U;

    /* Data-phase timing: kept for HAL completeness, unused in Classic mode */
    s_hfdcan1.Init.DataPrescaler         = 2U;
    s_hfdcan1.Init.DataSyncJumpWidth     = 12U;
    s_hfdcan1.Init.DataTimeSeg1          = 12U;
    s_hfdcan1.Init.DataTimeSeg2          = 12U;

    /* Message RAM layout */
    s_hfdcan1.Init.MessageRAMOffset      = 0U;
    s_hfdcan1.Init.StdFiltersNbr         = CAN_HWAB_STD_FILTERS_NB;
    s_hfdcan1.Init.ExtFiltersNbr         = 0U;

    /* Rx: no FIFOs, one dedicated buffer of 8 bytes */
    s_hfdcan1.Init.RxFifo0ElmtsNbr      = 0U;
    s_hfdcan1.Init.RxFifo0ElmtSize      = FDCAN_DATA_BYTES_8;
    s_hfdcan1.Init.RxFifo1ElmtsNbr      = 0U;
    s_hfdcan1.Init.RxFifo1ElmtSize      = FDCAN_DATA_BYTES_8;
    s_hfdcan1.Init.RxBuffersNbr         = CAN_HWAB_RX_BUFFERS_NB;
    s_hfdcan1.Init.RxBufferSize         = FDCAN_DATA_BYTES_8;

    /* Tx: no dedicated buffers; Tx FIFO queue of CAN_TX_FIFO_DEPTH_ slots
     * required by HAL_FDCAN_AddMessageToTxFifoQ.  TxBuffersNbr must be 0
     * when TxFifoQueueElmtsNbr > 0 because the two pools share Message RAM
     * and their combined element count must not exceed 32 per the RM.      */
    s_hfdcan1.Init.TxEventsNbr          = 0U;
    s_hfdcan1.Init.TxBuffersNbr         = 0U;
    s_hfdcan1.Init.TxFifoQueueElmtsNbr  = CAN_TX_FIFO_DEPTH_;
    s_hfdcan1.Init.TxFifoQueueMode      = FDCAN_TX_FIFO_OPERATION;
    s_hfdcan1.Init.TxElmtSize           = FDCAN_DATA_BYTES_8;

    if (HAL_FDCAN_Init(&s_hfdcan1) != HAL_OK)
    {
        LOG("HAL_FDCAN_Init failed");
        return CAN_ERROR;
    }

    /* Configure NVIC for FDCAN1 interrupt line 0 */
    if (Can_ConfigNvic_() != CAN_OK)
    {
        return CAN_ERROR;
    }

    s_canState.isInitialised = 1;
    LOG("CAN initialised");
    return CAN_OK;
}

/**
 * @brief  Configure reception filters and start the FDCAN module.
 */
CanStatus_t Can_Start(void)
{
    if (!s_canState.isInitialised)
    {
        LOG("CAN not initialised");
        return CAN_NOT_INIT;
    }

    if (HAL_FDCAN_Start(&s_hfdcan1) != HAL_OK)
    {
        LOG("HAL_FDCAN_Start failed");
        return CAN_ERROR;
    }

    s_canState.isStarted = 1;
    LOG("CAN started");
    return CAN_OK;
}

/**
 * @brief  Transmit a single Classic CAN frame via the Tx FIFO queue.
 *
 * @details
 * Enqueues one frame into the FDCAN Tx FIFO queue.  The HAL schedules
 * transmission autonomously once the frame is queued; no explicit
 * EnableTxBufferRequest call is required when using the FIFO queue path.
 * Returns CAN_TX_PENDING if the FIFO is currently full so the caller can
 * decide whether to retry or discard.
 */
CanStatus_t Can_Transmit(const CanTxHeader_t *pTxHeader, const uint8 *pData)
{
    if (!s_canState.isInitialised || !s_canState.isStarted)
    {
        return CAN_NOT_INIT;
    }

    if ((pTxHeader == NULL_PTR) || (pData == NULL_PTR))
    {
        return CAN_ERROR;
    }

    /* Reject the request when the Tx FIFO has no free slot */
    if (HAL_FDCAN_GetTxFifoFreeLevel(&s_hfdcan1) == 0U)
    {
        LOG("Tx FIFO full");
        return CAN_TX_PENDING;
    }

    if (HAL_FDCAN_AddMessageToTxFifoQ(&s_hfdcan1, pTxHeader, pData) != HAL_OK)
    {
        LOG("AddMessageToTxFifoQ failed");
        return CAN_ERROR;
    }

    return CAN_OK;
}

/**
 * @brief  Poll for a received frame and copy it out of Rx buffer 0.
 */
CanStatus_t Can_Receive(CanRxHeader_t *pRxHeader, uint8 *pData)
{
    if (!s_canState.isInitialised || !s_canState.isStarted)
    {
        return CAN_NOT_INIT;
    }

    if ((pRxHeader == NULL_PTR) || (pData == NULL_PTR))
    {
        return CAN_ERROR;
    }

    if (HAL_FDCAN_IsRxBufferMessageAvailable(&s_hfdcan1, FDCAN_RX_BUFFER0) == 0U)
    {
        return CAN_RX_EMPTY;
    }

    if (HAL_FDCAN_GetRxMessage(&s_hfdcan1, FDCAN_RX_BUFFER0,
                                pRxHeader, pData) != HAL_OK)
    {
        LOG("GetRxMessage failed");
        return CAN_ERROR;
    }

    return CAN_OK;
}

/**
 * @brief  Build and return a Tx header for a Classic CAN data frame.
 *
 * @details
 * Populates all HAL-required fields with Classic CAN defaults:
 *   - Standard 11-bit identifier
 *   - Data frame (not remote)
 *   - Configurable DLC (caller-specified)
 *   - No Bit-Rate Switch, Classic CAN format
 *   - No Tx event FIFO entry
 *   - Error State Indicator = Active
 *
 * @param  identifier   11-bit standard CAN identifier.
 * @param  dataLength   Data Length Code (e.g., FDCAN_DLC_BYTES_8).
 * @return Populated CanTxHeader_t ready to pass to Can_Transmit().
 */
CanTxHeader_t Can_BuildTxHeader(uint32 identifier, uint32 dataLength)
{
    CanTxHeader_t txHeader;
    memset(&txHeader, 0, sizeof(txHeader));

    txHeader.Identifier          = identifier;
    txHeader.IdType              = FDCAN_STANDARD_ID;
    txHeader.TxFrameType         = FDCAN_DATA_FRAME;
    txHeader.DataLength          = dataLength;
    txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txHeader.BitRateSwitch       = FDCAN_BRS_OFF;
    txHeader.FDFormat            = FDCAN_CLASSIC_CAN;
    txHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    txHeader.MessageMarker       = 0x0U;

    return txHeader;
}

/**
 * @brief  Stop the FDCAN module and de-initialise the peripheral.
 */
CanStatus_t Can_DeInit(void)
{
    if (!s_canState.isInitialised)
    {
        return CAN_NOT_INIT;
    }

    if (s_canState.isStarted)
    {
        if (HAL_FDCAN_Stop(&s_hfdcan1) != HAL_OK)
        {
            LOG("HAL_FDCAN_Stop failed");
            return CAN_ERROR;
        }
        s_canState.isStarted = 0;
    }

    if (HAL_FDCAN_DeInit(&s_hfdcan1) != HAL_OK)
    {
        LOG("HAL_FDCAN_DeInit failed");
        return CAN_ERROR;
    }

    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
    __HAL_RCC_FDCAN_CLK_DISABLE();

    s_canState.isInitialised = 0;
    LOG("CAN de-initialised");
    return CAN_OK;
}

/* ─── Private Functions ───────────────────────────────────────────────────── */

/**
 * @brief  Configure the FDCAN peripheral clock to be sourced from PLL.
 */
static CanStatus_t Can_InitPeriphClk_(void)
{
    RCC_PeriphCLKInitTypeDef periphClkInit = {0};

    periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    periphClkInit.FdcanClockSelection  = RCC_FDCANCLKSOURCE_PLL;

    if (HAL_RCCEx_PeriphCLKConfig(&periphClkInit) != HAL_OK)
    {
        LOG("Peripheral clock config failed");
        return CAN_ERROR;
    }

    return CAN_OK;
}

/**
 * @brief  Set FDCAN1 interrupt priority and enable it in the NVIC.
 */
static CanStatus_t Can_ConfigNvic_(void)
{
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0U, 0U);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    return CAN_OK;
}
