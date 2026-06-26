/**
 * @file    PcComMgr.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-23
 * @version 0.2.4
 * @brief   AUTOSAR SWC Implementation: PcComMgr
 *
 * @details
 * Consolidated PC Communication Manager software component. Implements
 * outgoing UDP packet construction and registration with the Ethernet UDP
 * server. Follows the standard AUTOSAR SWC pattern
 * (ReadInputs → CheckFaults → ComputeOutputs → WriteOutputs), with the
 * pre-build hook (PcComMgr_PrepareTx) running this cycle on each UDP
 * transmission event.
 *
 * The SWC never touches hardware or lower-layer Ethernet headers directly;
 * all I/O passes through Rte_PcComMgr.
 *
 * Port naming: no underscore after Pp/Rp prefix (RpBtnValue, RpEth, PpEth).
 *
 * Permitted includes: PcComMgr.h, Rte_PcComMgr.h
 * Forbidden:          Eth_UdpServer.h, Eth_Cfg.h, Eth_HwAb.h, Mcal.h,
 *                     Rte_HmiPcComIf.h, Rte_PcComEthIf.h, Rte_HmiDioIf.h,
 *                     Rte_Hmi.h
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial consolidated release         |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "PcComMgr.h"
#include "Rte_PcComMgr.h"

/* ─── Private Variables ───────────────────────────────────────────────────── */

static PcComMgr_InputData_t   PcComMgr_Inputs;
static PcComMgr_OutputData_t  PcComMgr_Outputs;

static boolean                PcComMgr_Initialized  = FALSE;
static uint16                 PcComMgr_DiagEventCnt = 0u;

/** @brief  Monotonically increasing TX packet counter. */
static uint32                 s_packetCounter       = 0U;

/** @brief  User-configurable constant float sent in every TX packet. */
static float                  s_constF2             = 3.14159f;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static void    PcComMgr_ReadInputs  (void);
static void    PcComMgr_WriteOutputs(void);
static boolean PcComMgr_CheckFaults (const PcComMgr_InputData_t *inputs);
static void    PcComMgr_CalcOutputs (const PcComMgr_InputData_t *inputs,
                                     PcComMgr_OutputData_t       *outputs);

/* ─── Private Function Implementations ───────────────────────────────────── */

/**
 * @brief  Read all require-port signals into the local input buffer.
 * @details
 * A read error increments the diagnostic counter and falls back to a safe
 * default value for that signal.
 */
static void PcComMgr_ReadInputs(void)
{
    Std_ReturnType ret;

    /* --- RpBtnValue ------------------------------------------------------- */
    ret = Rte_Read_RpBtnValue_BtnValue(&PcComMgr_Inputs.BtnValue);
    if (ret != RTE_E_OK)
    {
        PcComMgr_Inputs.BtnValue = 0.0f;
        PcComMgr_DiagEventCnt++;
    }

    /* --- RpEth (timestamp) ------------------------------------------------ */
    ret = Rte_Read_RpEth_Timestamp(&PcComMgr_Inputs.Timestamp);
    if (ret != RTE_E_OK)
    {
        PcComMgr_Inputs.Timestamp = 0U;
        PcComMgr_DiagEventCnt++;
    }

    /* --- RpEth (RX fields — best-effort; caller checks LoadRxPacket) ------ */
    ret = Rte_Read_RpEth_RxHeader0(&PcComMgr_Inputs.RxHeader0);
    if (ret != RTE_E_OK) { PcComMgr_Inputs.RxHeader0 = 0U; PcComMgr_DiagEventCnt++; }

    ret = Rte_Read_RpEth_RxHeader1(&PcComMgr_Inputs.RxHeader1);
    if (ret != RTE_E_OK) { PcComMgr_Inputs.RxHeader1 = 0U; PcComMgr_DiagEventCnt++; }

    ret = Rte_Read_RpEth_RxCmdType(&PcComMgr_Inputs.RxCmdType);
    if (ret != RTE_E_OK) { PcComMgr_Inputs.RxCmdType = 0U; PcComMgr_DiagEventCnt++; }

    ret = Rte_Read_RpEth_RxCmdValue(&PcComMgr_Inputs.RxCmdValue);
    if (ret != RTE_E_OK) { PcComMgr_Inputs.RxCmdValue = 0U; PcComMgr_DiagEventCnt++; }

    ret = Rte_Read_RpEth_RxSetpoint(&PcComMgr_Inputs.RxSetpoint);
    if (ret != RTE_E_OK) { PcComMgr_Inputs.RxSetpoint = 0.0f; PcComMgr_DiagEventCnt++; }
}

/**
 * @brief  Write the local output buffer to all TX provide-port signals.
 */
static void PcComMgr_WriteOutputs(void)
{
    /* --- PpEth ------------------------------------------------------------ */
    Rte_Write_PpEth_TxHeader   (PcComMgr_Outputs.TxHeader0, PcComMgr_Outputs.TxHeader1);
    (void)Rte_Write_PpEth_TxCounter   (PcComMgr_Outputs.TxCounter);
    (void)Rte_Write_PpEth_TxTimestamp (PcComMgr_Outputs.TxTimestamp);
    (void)Rte_Write_PpEth_TxBtnValue  (PcComMgr_Outputs.TxBtnValue);
    (void)Rte_Write_PpEth_TxConstF2   (PcComMgr_Outputs.TxConstF2);
}

/**
 * @brief  Check input plausibility and detect fault conditions.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return TRUE if a fault is present, FALSE otherwise.
 */
static boolean PcComMgr_CheckFaults(const PcComMgr_InputData_t *inputs)
{
    (void)inputs;
    /* No plausibility checks defined at this revision. */
    return FALSE;
}

/**
 * @brief  Compute all TX output fields from the current input snapshot.
 * @details
 * Fills the output structure with the fixed header magic bytes, the current
 * packet counter, the captured timestamp, the Hmi button value, and the
 * user-configured constant float. The packet counter is incremented here so
 * that each PrepareTx call produces a unique counter value.
 * @param  inputs   Pointer to the current input data snapshot.
 * @param  outputs  Pointer to the output structure to populate.
 */
static void PcComMgr_CalcOutputs(const PcComMgr_InputData_t  *inputs,
                                   PcComMgr_OutputData_t        *outputs)
{
    outputs->TxHeader0   = 0xAAU;
    outputs->TxHeader1   = 0xBBU;
    outputs->TxCounter   = s_packetCounter;
    outputs->TxTimestamp = inputs->Timestamp;
    outputs->TxBtnValue  = inputs->BtnValue;
    outputs->TxConstF2   = s_constF2;

    s_packetCounter++;
}

/* ─── Public Function Implementations ────────────────────────────────────── */

/**
 * @brief  PcComMgr_Init — called once at ECU startup.
 * @details
 * Initialises the Ethernet RTE layer, resets internal state, and registers
 * both the pre-build hook and the UDP packet builder.
 */
void PcComMgr_Init(void)
{
    /* Zero all local structures */
    PcComMgr_Inputs  = (PcComMgr_InputData_t) {0};
    PcComMgr_Outputs = (PcComMgr_OutputData_t){0};

    PcComMgr_DiagEventCnt = 0u;
    s_packetCounter       = 0U;

    /* Initialise the Ethernet hardware abstraction via RTE */
    Rte_PcComMgr_EthInit();

    /* Register pre-build hook first, then the packet builder */
    Rte_PcComMgr_RegisterPreBuildHook(PcComMgr_PrepareTx);
    Rte_PcComMgr_RegisterPktBuilder();

    PcComMgr_Initialized = TRUE;
}

/**
 * @brief  PcComMgr_PrepareTx — TX pre-build hook, invoked before each packet.
 * @details
 * Executes the full Read → CheckFaults → CalcOutputs → WriteOutputs cycle
 * so that every TX packet carries up-to-date signal values.
 */
void PcComMgr_PrepareTx(void)
{
    boolean faultPresent;

    if (PcComMgr_Initialized == FALSE)
    {
        return; /* Safety guard: do not execute before init */
    }

    /* ── Step 1: Read inputs ─────────────────────────────────────────────── */
    PcComMgr_ReadInputs();

    /* ── Step 2: Fault detection ─────────────────────────────────────────── */
    faultPresent = PcComMgr_CheckFaults(&PcComMgr_Inputs);
    if (faultPresent == TRUE)
    {
        PcComMgr_DiagEventCnt++;
    }

    /* ── Step 3: Compute outputs ─────────────────────────────────────────── */
    PcComMgr_CalcOutputs(&PcComMgr_Inputs, &PcComMgr_Outputs);

    /* ── Step 4: Write outputs ───────────────────────────────────────────── */
    PcComMgr_WriteOutputs();
}

/**
 * @brief  Set the constant float value transmitted in every TX packet.
 * @param  value  New constant float to store.
 */
void PcComMgr_SetConstFloat(float value)
{
    s_constF2 = value;
}

/**
 * @brief  Get the current constant float value.
 * @return Current s_constF2 value.
 */
float PcComMgr_GetConstFloat(void)
{
    return s_constF2;
}

/**
 * @brief  Get the current accumulated button value from the Hmi SWC.
 * @details
 * Reads the Hmi PpBtnValue signal via the RpBtnValue require port.
 * @return Current BtnValue float.
 */
float PcComMgr_GetBtnValue(void)
{
    Rte_DT_RpBtnValue_BtnValue_t value = 0.0f;
    (void)Rte_Read_RpBtnValue_BtnValue(&value);
    return value;
}

/**
 * @brief  Reset the Hmi accumulated button value to zero.
 * @details
 * Calls the RTE-layer reset helper Rte_PcComMgr_ResetBtnValue(), which
 * writes 0.0 directly to the Rte_Buffers shared signal store. PcComMgr.c
 * does not include Rte_Hmi.h; the reset is performed at the buffer level,
 * keeping each RTE module self-contained.
 */
void PcComMgr_ResetBtnValue(void)
{
    Rte_PcComMgr_ResetBtnValue();
}
