/**
 * @file    Hmi.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-17
 * @version 1.0.0
 * @brief   AUTOSAR SWC Implementation: Hmi
 *
 * @details
 * Consolidated Hmi software component. Follows the standard AUTOSAR SWC
 * template pattern (ReadInputs → CheckFaults → ComputeOutputs → WriteOutputs).
 *
 * On each rising edge of UsrBtn, the BtnValue provide-port signal is
 * incremented by BTN_INCREMENT (0.5). The Hmi SWC never touches hardware
 * directly; all I/O passes through Rte_Hmi.
 *
 * Port naming: no underscore after Pp/Rp prefix (RpDio, PpBtnValue).
 *
 * Accumulated value ownership
 * ────────────────────────────
 *   The running total is kept in the static Hmi_Outputs.BtnValue field and
 *   is never read back from the provide port (AUTOSAR forbids Pp reads).
 *   Hmi_CalcBtnValue() adds BTN_INCREMENT directly to Hmi_Outputs.BtnValue
 *   on a detected rising edge.
 *
 * Permitted includes: Hmi.h, Rte_Hmi.h
 * Forbidden:          Dio_HwAb.h, Eth_*.h, Mcal.h, HAL_*.h
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 1.0.0   | 2025-06-17 | A.Rezapour       | Initial consolidated release         |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Hmi.h"
#include "Rte_Hmi.h"

/* ─── Private Macros ──────────────────────────────────────────────────────── */

/** @brief  Float increment added to BtnValue on each rising edge. */
#define BTN_INCREMENT   0.5f

/* ─── Private Variables ───────────────────────────────────────────────────── */

static Hmi_InputData_t   Hmi_Inputs;
static Hmi_OutputData_t  Hmi_Outputs;   /* Also serves as the internal accumulator */

static boolean           Hmi_Initialized  = FALSE;
static uint16            Hmi_DiagEventCnt = 0u;

/** @brief  Last stable button state — used for rising-edge detection. */
static Rte_DT_RpDio_UsrBtn_t s_lastStableState = FALSE;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static void    Hmi_ReadInputs  (void);
static void    Hmi_WriteOutputs(void);
static boolean Hmi_CheckFaults (const Hmi_InputData_t *inputs);
static float   Hmi_CalcBtnValue(const Hmi_InputData_t *inputs);

/* ─── Private Function Implementations ───────────────────────────────────── */

/**
 * @brief  Read all require-port signals into the local input buffer.
 * @details
 * A read error increments the diagnostic counter and falls back to the
 * safe default (button not pressed).
 */
static void Hmi_ReadInputs(void)
{
    Std_ReturnType ret;

    /* --- RpDio ------------------------------------------------------------ */
    ret = Rte_Read_RpDio_UsrBtn(&Hmi_Inputs.UsrBtn);
    if (ret != RTE_E_OK)
    {
        Hmi_Inputs.UsrBtn = FALSE;
        Hmi_DiagEventCnt++;
    }
}

/**
 * @brief  Write the local output buffer to all provide-port signals.
 */
static void Hmi_WriteOutputs(void)
{
    /* --- PpBtnValue ------------------------------------------------------- */
    (void)Rte_Write_PpBtnValue_BtnValue(Hmi_Outputs.BtnValue);
}

/**
 * @brief  Check input plausibility and detect fault conditions.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return TRUE if a fault is present, FALSE otherwise.
 */
static boolean Hmi_CheckFaults(const Hmi_InputData_t *inputs)
{
    (void)inputs;
    /* No plausibility checks needed for a single boolean input. */
    return FALSE;
}

/**
 * @brief  Compute the new BtnValue based on rising-edge detection.
 * @details
 * Detects a LOW-to-HIGH transition on UsrBtn. When detected, BTN_INCREMENT
 * is added to the current accumulated value held in Hmi_Outputs.BtnValue.
 * The SWC does NOT read back its own provide port — the accumulated total
 * is maintained internally through the static Hmi_Outputs structure.
 * The last stable state is updated unconditionally at the end of each call.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return Updated BtnValue (unchanged if no rising edge was detected).
 */
static float Hmi_CalcBtnValue(const Hmi_InputData_t *inputs)
{
    float newValue = Hmi_Outputs.BtnValue; /* Read from internal SWC state, NOT from Pp */

    /* Detect rising edge (button press) */
    if ((inputs->UsrBtn == TRUE) && (s_lastStableState == FALSE))
    {
        newValue += BTN_INCREMENT;
    }

    /* Update edge-detection state */
    s_lastStableState = inputs->UsrBtn;

    return newValue;
}

/* ─── Runnable Implementations ───────────────────────────────────────────── */

/**
 * @brief  Hmi_Init — called once at ECU startup.
 * @details
 * Reads the initial button state (to avoid a false rising edge on the first
 * cycle) and initialises the BtnValue provide port to 0.0.
 */
void Hmi_Init(void)
{
    /* Zero all local structures */
    Hmi_Inputs  = (Hmi_InputData_t) {0};
    Hmi_Outputs = (Hmi_OutputData_t){0};

    Hmi_DiagEventCnt = 0u;

    /* Read initial button state to seed the edge-detector */
    (void)Rte_Read_RpDio_UsrBtn(&s_lastStableState);

    /* Write safe default output */
    Hmi_Outputs.BtnValue = 0.0f;
    Hmi_WriteOutputs();

    Hmi_Initialized = TRUE;
}

/**
 * @brief  Hmi_Run_10ms — cyclic runnable, called every 10 ms by the scheduler.
 *
 * Execution order:
 *   1. Read all require-port inputs
 *   2. Run fault detection
 *   3. Compute output signals
 *   4. Write all provide-port outputs
 */
void Hmi_Run_10ms(void)
{
    boolean faultPresent;

    if (Hmi_Initialized == FALSE)
    {
        return; /* Safety guard: do not execute before init */
    }

    /* ── Step 1: Read inputs ─────────────────────────────────────────────── */
    Hmi_ReadInputs();

    /* ── Step 2: Fault detection ─────────────────────────────────────────── */
    faultPresent = Hmi_CheckFaults(&Hmi_Inputs);
    if (faultPresent == TRUE)
    {
        Hmi_DiagEventCnt++;
    }

    /* ── Step 3: Compute outputs ─────────────────────────────────────────── */
    Hmi_Outputs.BtnValue = Hmi_CalcBtnValue(&Hmi_Inputs);

    /* ── Step 4: Write outputs ───────────────────────────────────────────── */
    Hmi_WriteOutputs();
}
