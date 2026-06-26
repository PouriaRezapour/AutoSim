/**
 * @file    CmdSigMgr.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR SWC Implementation: CmdSigMgr
 *
 * @details
 * Consolidated Command Signal Manager software component. Follows the
 * standard AUTOSAR SWC template pattern
 * (ReadInputs → CheckFaults → ComputeOutputs → WriteOutputs).
 *
 * On each 50 ms cycle, the SWC reads the packed uint8 command word from
 * the RpCommandSignalsPacked require port (sourced from Rte_Buffers), masks off
 * the reserved bit 7, unpacks the remaining 7 bits into individual boolean
 * command signals, and writes each individual signal to the PpCommandSignals
 * provide port, which in turn drives the corresponding DIO output pin via
 * Rte_CmdSigMgr. The CmdSigMgr SWC never touches hardware directly; all
 * I/O passes through Rte_CmdSigMgr.
 *
 * Port naming: no underscore after Pp/Rp prefix (RpCommandSignalsPacked,
 * PpCommandSignals).
 *
 * RpCommandSignalsPacked.Packed is genuine SWC-to-SWC communication (sourced
 * from Rte_Buffers) and is obtained via Rte_Read. PpCommandSignals signals
 * are BSW-bound (Dio pin) accesses rather than SWC-to-SWC communication, so
 * they are written via Rte_Call_PpCommandSignals_<Signal>(), not
 * Rte_Write_PpCommandSignals_<Signal>().
 *
 * Bit unpacking layout (matches CmdSigMgr.h documentation):
 *   Bit 0 — Vesc      → HWAB_DIO_CMD_VESC
 *   Bit 1 — Bls       → HWAB_DIO_CMD_BLS
 *   Bit 2 — Itpms     → HWAB_DIO_CMD_ITPMS
 *   Bit 3 — ParkBrake → HWAB_DIO_CMD_PARK_BRAKE
 *   Bit 4 — EscTcs    → HWAB_DIO_CMD_ESC_TCS
 *   Bit 5 — Wakeup    → HWAB_DIO_CMD_WAKEUP
 *   Bit 6 — Vmot      → HWAB_DIO_CMD_VMOT
 *   Bit 7 — Reserved, masked to 0 before any unpacking
 *
 * Permitted includes: CmdSigMgr.h, Rte_CmdSigMgr.h
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

#include "CmdSigMgr.h"
#include "Rte_CmdSigMgr.h"

/* ─── Private Macros ──────────────────────────────────────────────────────── */

/**
 * @brief  Mask applied to the raw command word to clear the reserved bit 7.
 * @details Must be applied before any bit extraction to guarantee that
 *          no reserved state is propagated to a DIO output.
 */
#define CMDSIGMGR_CMD_VALID_MASK    (0x7Fu)

/* ─── Private Variables ───────────────────────────────────────────────────── */

static CmdSigMgr_InputData_t   CmdSigMgr_Inputs;
static CmdSigMgr_OutputData_t  CmdSigMgr_Outputs;

static boolean                 CmdSigMgr_Initialized  = FALSE;
static uint16                  CmdSigMgr_DiagEventCnt = 0u;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static void    CmdSigMgr_ReadInputs  (void);
static void    CmdSigMgr_WriteOutputs(void);
static boolean CmdSigMgr_CheckFaults (const CmdSigMgr_InputData_t *inputs);
static void    CmdSigMgr_UnpackCmds  (const CmdSigMgr_InputData_t *inputs,
                                       CmdSigMgr_OutputData_t       *outputs);

/* ─── Private Function Implementations ───────────────────────────────────── */

/**
 * @brief  Read all require-port signals into the local input buffer.
 * @details
 * A read error increments the diagnostic counter and falls back to the
 * safe default (all commands inactive / 0x00).
 */
static void CmdSigMgr_ReadInputs(void)
{
    Std_ReturnType ret;

    /* --- RpCommandSignalsPacked -------------------------------------------------- */
    ret = Rte_Read_RpCommandSignalsPacked_Packed(&CmdSigMgr_Inputs.Packed);
    if (ret != RTE_E_OK)
    {
        CmdSigMgr_Inputs.Packed = 0x00u;
        CmdSigMgr_DiagEventCnt++;
    }
}

/**
 * @brief  Write the local output buffer to all provide-port signals.
 * @details
 * Each PpCommandSignals signal targets a Dio pin through the BSW
 * (DioIf), not another SWC, so it is written via Rte_Call, not Rte_Write.
 */
static void CmdSigMgr_WriteOutputs(void)
{
    /* --- PpCommandSignals -------------------------------------------------- */
    (void)Rte_Call_PpCommandSignals_Vesc     (CmdSigMgr_Outputs.Vesc);
    (void)Rte_Call_PpCommandSignals_Bls      (CmdSigMgr_Outputs.Bls);
    (void)Rte_Call_PpCommandSignals_Itpms    (CmdSigMgr_Outputs.Itpms);
    (void)Rte_Call_PpCommandSignals_ParkBrake(CmdSigMgr_Outputs.ParkBrake);
    (void)Rte_Call_PpCommandSignals_EscTcs   (CmdSigMgr_Outputs.EscTcs);
    (void)Rte_Call_PpCommandSignals_Wakeup   (CmdSigMgr_Outputs.Wakeup);
    (void)Rte_Call_PpCommandSignals_Vmot     (CmdSigMgr_Outputs.Vmot);
}

/**
 * @brief  Check input plausibility and detect fault conditions.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return TRUE if a fault is present, FALSE otherwise.
 */
static boolean CmdSigMgr_CheckFaults(const CmdSigMgr_InputData_t *inputs)
{
    (void)inputs;
    /* No plausibility constraints defined at this revision beyond bit masking. */
    return FALSE;
}

/**
 * @brief  Unpack 7 individual command booleans from the masked command word.
 * @details
 * First masks bit 7 to 0 via CMDSIGMGR_CMD_VALID_MASK, then extracts
 * each bit to its corresponding boolean output field. Each extraction
 * applies an additional single-bit AND to guard against any non-zero
 * carry from the mask step.
 *
 * Bit assignments (same as CmdSigMgr.h specification):
 *   Bit 0 → Vesc, Bit 1 → Bls, Bit 2 → Itpms, Bit 3 → ParkBrake,
 *   Bit 4 → EscTcs, Bit 5 → Wakeup, Bit 6 → Vmot
 *
 * @param  inputs   Pointer to the current input data snapshot.
 * @param  outputs  Pointer to the output structure to populate.
 */
static void CmdSigMgr_UnpackCmds(const CmdSigMgr_InputData_t *inputs,
                                   CmdSigMgr_OutputData_t       *outputs)
{
    /* Mask reserved bit 7 before any further processing */
    uint8 cmd = (uint8)(inputs->Packed & CMDSIGMGR_CMD_VALID_MASK);

    outputs->Vesc      = (boolean)((cmd >> 0u) & 0x01u);
    outputs->Bls       = (boolean)((cmd >> 1u) & 0x01u);
    outputs->Itpms     = (boolean)((cmd >> 2u) & 0x01u);
    outputs->ParkBrake = (boolean)((cmd >> 3u) & 0x01u);
    outputs->EscTcs    = (boolean)((cmd >> 4u) & 0x01u);
    outputs->Wakeup    = (boolean)((cmd >> 5u) & 0x01u);
    outputs->Vmot      = (boolean)((cmd >> 6u) & 0x01u);
}

/* ─── Runnable Implementations ───────────────────────────────────────────── */

/**
 * @brief  CmdSigMgr_Init — called once at ECU startup.
 * @details
 * Zeroes all local structures and writes the safe default (all commands
 * inactive / LOW) to every PpCommandSignals provide-port signal.
 */
void CmdSigMgr_Init(void)
{
    /* Zero all local structures */
    CmdSigMgr_Inputs  = (CmdSigMgr_InputData_t) {0};
    CmdSigMgr_Outputs = (CmdSigMgr_OutputData_t){0};

    CmdSigMgr_DiagEventCnt = 0u;

    /* Write safe defaults — all command outputs inactive */
    CmdSigMgr_Outputs.Vesc      = FALSE;
    CmdSigMgr_Outputs.Bls       = FALSE;
    CmdSigMgr_Outputs.Itpms     = FALSE;
    CmdSigMgr_Outputs.ParkBrake = FALSE;
    CmdSigMgr_Outputs.EscTcs    = FALSE;
    CmdSigMgr_Outputs.Wakeup    = FALSE;
    CmdSigMgr_Outputs.Vmot      = FALSE;
    CmdSigMgr_WriteOutputs();

    CmdSigMgr_Initialized = TRUE;
}

/**
 * @brief  CmdSigMgr_Run_50ms — cyclic runnable, called every 50 ms by the scheduler.
 *
 * Execution order:
 *   1. Read all require-port inputs
 *   2. Run fault detection
 *   3. Unpack command bits into individual output signals
 *   4. Write all provide-port outputs
 */
void CmdSigMgr_Run_50ms(void)
{
    boolean faultPresent;

    if (CmdSigMgr_Initialized == FALSE)
    {
        return; /* Safety guard: do not execute before init */
    }

    /* ── Step 1: Read inputs ─────────────────────────────────────────────── */
    CmdSigMgr_ReadInputs();

    /* ── Step 2: Fault detection ─────────────────────────────────────────── */
    faultPresent = CmdSigMgr_CheckFaults(&CmdSigMgr_Inputs);
    if (faultPresent == TRUE)
    {
        CmdSigMgr_DiagEventCnt++;
    }

    /* ── Step 3: Compute outputs ─────────────────────────────────────────── */
    CmdSigMgr_UnpackCmds(&CmdSigMgr_Inputs, &CmdSigMgr_Outputs);

    /* ── Step 4: Write outputs ───────────────────────────────────────────── */
    CmdSigMgr_WriteOutputs();
}
