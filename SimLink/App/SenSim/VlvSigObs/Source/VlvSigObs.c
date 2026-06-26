/**
 * @file    VlvSigObs.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR SWC Implementation: VlvSigObs
 *
 * @details
 * Consolidated Valve Signal Observer software component. 
 * Follows the standard AUTOSAR SWC template pattern
 * (ReadInputs → CheckFaults → ComputeOutputs → WriteOutputs).
 *
 * On each 50 ms cycle, the SWC reads 14 DIO input signals through the
 * RpValveStatus require port, packs them into a single uint16 word
 * according to the bit layout defined in VlvSigObs.h, explicitly clears
 * reserved bits 14-15, and writes the result to the PpValveStatusPacked
 * provide port. The VlvSigObs SWC never touches hardware directly; all
 * I/O passes through Rte_VlvSigObs.
 *
 * Port naming: no underscore after Pp/Rp prefix (RpValveStatus, PpValveStatusPacked).
 *
 * RpValveStatus signals are BSW-bound (Dio pin) accesses rather than
 * SWC-to-SWC communication, so they are obtained via
 * Rte_Call_RpValveStatus_<Signal>(), not Rte_Read_RpValveStatus_<Signal>().
 * PpValveStatusPacked.Packed is genuine SWC-to-SWC communication and
 * continues to use Rte_Write.
 *
 * Bit packing layout (matches Rte_Buffers.h / VlvSigObs.h documentation):
 *   Bit  0     — ValveOn  (HWAB_DIO_SIGNAL_VALVE_ON)
 *   Bits 1-12  — Valve1 … Valve12
 *   Bit  13    — MotorStatus (HWAB_DIO_MOTOR_STATUS)
 *   Bits 14-15 — Reserved, explicitly cleared to 0
 *
 * Permitted includes: VlvSigObs.h, Rte_VlvSigObs.h
 * Forbidden:          DioIf.h, Eth_*.h, Mcal.h, HAL_*.h, Rte_Buffers.h
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "VlvSigObs.h"
#include "Rte_VlvSigObs.h"

/* ─── Private Macros ──────────────────────────────────────────────────────── */

/**
 * @brief  Mask to clear reserved bits 14 and 15 of the packed status word.
 * @details Applied after packing to guarantee the two MSBs are always 0.
 */
#define VLVSIGOBS_VALID_BITS_MASK   ((uint16)0x3FFFu)

/* ─── Private Variables ───────────────────────────────────────────────────── */

static VlvSigObs_InputData_t   VlvSigObs_Inputs;
static VlvSigObs_OutputData_t  VlvSigObs_Outputs;

static boolean                 VlvSigObs_Initialized  = FALSE;
static uint16                  VlvSigObs_DiagEventCnt = 0u;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static void    VlvSigObs_ReadInputs  (void);
static void    VlvSigObs_WriteOutputs(void);
static boolean VlvSigObs_CheckFaults (const VlvSigObs_InputData_t *inputs);
static uint16  VlvSigObs_PackStatus  (const VlvSigObs_InputData_t *inputs);

/* ─── Private Function Implementations ───────────────────────────────────── */

/**
 * @brief  Read all require-port signals into the local input buffer.
 * @details
 * A read error increments the diagnostic counter and falls back to the
 * safe default (pin not active / FALSE) for the affected signal. All
 * RpValveStatus signals target Dio pins through the BSW, so they are
 * obtained via Rte_Call, not Rte_Read.
 */
static void VlvSigObs_ReadInputs(void)
{
    Std_ReturnType ret;

    /* --- RpValveStatus — motor and valve-on -------------------------------- */
    ret = Rte_Call_RpValveStatus_MotorStatus(&VlvSigObs_Inputs.MotorStatus);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.MotorStatus = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_ValveOn(&VlvSigObs_Inputs.ValveOn);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.ValveOn = FALSE; VlvSigObs_DiagEventCnt++; }

    /* --- RpValveStatus — individual valves 1-12 ---------------------------- */
    ret = Rte_Call_RpValveStatus_Valve1(&VlvSigObs_Inputs.Valve1);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve1 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve2(&VlvSigObs_Inputs.Valve2);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve2 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve3(&VlvSigObs_Inputs.Valve3);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve3 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve4(&VlvSigObs_Inputs.Valve4);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve4 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve5(&VlvSigObs_Inputs.Valve5);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve5 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve6(&VlvSigObs_Inputs.Valve6);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve6 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve7(&VlvSigObs_Inputs.Valve7);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve7 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve8(&VlvSigObs_Inputs.Valve8);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve8 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve9(&VlvSigObs_Inputs.Valve9);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve9 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve10(&VlvSigObs_Inputs.Valve10);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve10 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve11(&VlvSigObs_Inputs.Valve11);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve11 = FALSE; VlvSigObs_DiagEventCnt++; }

    ret = Rte_Call_RpValveStatus_Valve12(&VlvSigObs_Inputs.Valve12);
    if (ret != RTE_E_OK) { VlvSigObs_Inputs.Valve12 = FALSE; VlvSigObs_DiagEventCnt++; }
}

/**
 * @brief  Write the local output buffer to all provide-port signals.
 */
static void VlvSigObs_WriteOutputs(void)
{
    /* --- PpValveStatusPacked ----------------------------------------------------- */
    (void)Rte_Write_PpValveStatusPacked_Packed(VlvSigObs_Outputs.Packed);
}

/**
 * @brief  Check input plausibility and detect fault conditions.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return TRUE if a fault is present, FALSE otherwise.
 */
static boolean VlvSigObs_CheckFaults(const VlvSigObs_InputData_t *inputs)
{
    (void)inputs;
    /* No cross-signal plausibility constraints defined at this revision. */
    return FALSE;
}

/**
 * @brief  Pack 14 individual boolean input states into a uint16 status word.
 * @details
 * Bit placement follows the specification in VlvSigObs.h:
 *   Bit  0     — ValveOn
 *   Bits 1-12  — Valve1 … Valve12
 *   Bit  13    — MotorStatus
 *   Bits 14-15 — Cleared to 0 via VLVSIGOBS_VALID_BITS_MASK
 *
 * Each boolean is cast to uint16 and shifted to its target position before
 * OR-ing into the accumulator. The final mask ensures reserved bits stay
 * at 0 even if a boolean value was non-standard (i.e. not exactly 0 or 1).
 *
 * @param  inputs  Pointer to the current input data snapshot.
 * @return Packed uint16 status word with bits 14-15 cleared.
 */
static uint16 VlvSigObs_PackStatus(const VlvSigObs_InputData_t *inputs)
{
    uint16 packed = 0u;

    packed |= ((uint16)inputs->ValveOn      & 0x0001u) << 0u;
    packed |= ((uint16)inputs->Valve1       & 0x0001u) << 1u;
    packed |= ((uint16)inputs->Valve2       & 0x0001u) << 2u;
    packed |= ((uint16)inputs->Valve3       & 0x0001u) << 3u;
    packed |= ((uint16)inputs->Valve4       & 0x0001u) << 4u;
    packed |= ((uint16)inputs->Valve5       & 0x0001u) << 5u;
    packed |= ((uint16)inputs->Valve6       & 0x0001u) << 6u;
    packed |= ((uint16)inputs->Valve7       & 0x0001u) << 7u;
    packed |= ((uint16)inputs->Valve8       & 0x0001u) << 8u;
    packed |= ((uint16)inputs->Valve9       & 0x0001u) << 9u;
    packed |= ((uint16)inputs->Valve10      & 0x0001u) << 10u;
    packed |= ((uint16)inputs->Valve11      & 0x0001u) << 11u;
    packed |= ((uint16)inputs->Valve12      & 0x0001u) << 12u;
    packed |= ((uint16)inputs->MotorStatus  & 0x0001u) << 13u;

    /* Explicitly clear reserved bits 14-15 */
    packed &= VLVSIGOBS_VALID_BITS_MASK;

    return packed;
}

/* ─── Runnable Implementations ───────────────────────────────────────────── */

/**
 * @brief  VlvSigObs_Init — called once at ECU startup.
 * @details
 * Zeroes all local structures and writes the safe default (all bits 0)
 * to the PpValveStatusPacked provide port.
 */
void VlvSigObs_Init(void)
{
    /* Zero all local structures */
    VlvSigObs_Inputs  = (VlvSigObs_InputData_t) {0};
    VlvSigObs_Outputs = (VlvSigObs_OutputData_t){0};

    VlvSigObs_DiagEventCnt = 0u;

    /* Write safe default output — all status bits inactive */
    VlvSigObs_Outputs.Packed = 0x0000u;
    VlvSigObs_WriteOutputs();

    VlvSigObs_Initialized = TRUE;
}

/**
 * @brief  VlvSigObs_Run_50ms — cyclic runnable, called every 50 ms by the scheduler.
 *
 * Execution order:
 *   1. Read all require-port inputs
 *   2. Run fault detection
 *   3. Compute packed output word
 *   4. Write all provide-port outputs
 */
void VlvSigObs_Run_50ms(void)
{
    boolean faultPresent;

    if (VlvSigObs_Initialized == FALSE)
    {
        return; /* Safety guard: do not execute before init */
    }

    /* ── Step 1: Read inputs ─────────────────────────────────────────────── */
    VlvSigObs_ReadInputs();

    /* ── Step 2: Fault detection ─────────────────────────────────────────── */
    faultPresent = VlvSigObs_CheckFaults(&VlvSigObs_Inputs);
    if (faultPresent == TRUE)
    {
        VlvSigObs_DiagEventCnt++;
    }

    /* ── Step 3: Compute outputs ─────────────────────────────────────────── */
    VlvSigObs_Outputs.Packed = VlvSigObs_PackStatus(&VlvSigObs_Inputs);

    /* ── Step 4: Write outputs ───────────────────────────────────────────── */
    VlvSigObs_WriteOutputs();
}
