/**
 * @file    <SwcName>.c
 * @brief   AUTOSAR SWC Implementation: <SwcName>
 *
 * SWC Name    : <SwcName>
 * Author      : Your Name
 * Date        : 20xx-xx-xx
 * Version     : x.x.x
 *
 * Require Ports : Rp1 (<RpSignal1>, <RpSignal2>, <RpSignal3>)
 *                 Rp2 (<RpSignal4>, <RpSignal5>, <RpSignal6>)
 *                 (add / remove require ports as needed)
 *
 * Provide Ports : Pp1 (<PpSignal1>, <PpSignal2>, <PpSignal3>)
 *                 Pp2 (<PpSignal4>, <PpSignal5>, <PpSignal6>)
 *                 (add / remove provide ports as needed)
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | x.x.x   | 20xx-xx-xx | Your Name  | Initial release                      |
 * |---------|------------|------------|--------------------------------------|
 */

/*==============================================================================
 *  INCLUDES
 *============================================================================*/
#include "<SwcName>.h"
#include "Rte_<SwcName>.h"

/*==============================================================================
 *  LOCAL VARIABLES
 *============================================================================*/
static <SwcName>_InputData_t  <SwcName>_Inputs;
static <SwcName>_OutputData_t <SwcName>_Outputs;

/* Internal state */
static boolean <SwcName>_Initialized   = FALSE;
static uint16  <SwcName>_DiagEventCnt  = 0u;

/*==============================================================================
 *  LOCAL FUNCTION PROTOTYPES
 *============================================================================*/
static void    <SwcName>_ReadInputs  (void);
static void    <SwcName>_WriteOutputs(void);
static boolean <SwcName>_CheckFaults (const <SwcName>_InputData_t *inputs);
static uint8   <SwcName>_Calc<Output1>(const <SwcName>_InputData_t *inputs);
static uint8   <SwcName>_Calc<Output2>(const <SwcName>_InputData_t *inputs);

/*==============================================================================
 *  LOCAL FUNCTION IMPLEMENTATIONS
 *============================================================================*/

/**
 * @brief  Read all require-port signals into the local input buffer.
 *         A read error increments the diagnostic counter and falls back
 *         to a safe default value for that signal.
 */
static void <SwcName>_ReadInputs(void)
{
    Std_ReturnType ret;

    /* --- Rp1 ---------------------------------------------------------------- */
    ret = Rte_Read_Rp1_<RpSignal1>(&<SwcName>_Inputs.<RpSignal1>);
    if (ret != RTE_E_OK)
    {
        <SwcName>_Inputs.<RpSignal1> = 0u;
        <SwcName>_DiagEventCnt++;
    }

    ret = Rte_Read_Rp1_<RpSignal2>(&<SwcName>_Inputs.<RpSignal2>);
    if (ret != RTE_E_OK)
    {
        <SwcName>_Inputs.<RpSignal2> = 0u;
        <SwcName>_DiagEventCnt++;
    }

    ret = Rte_Read_Rp1_<RpSignal3>(&<SwcName>_Inputs.<RpSignal3>);
    if (ret != RTE_E_OK)
    {
        <SwcName>_Inputs.<RpSignal3> = FALSE;
        <SwcName>_DiagEventCnt++;
    }

    /* --- Rp2 ---------------------------------------------------------------- */
    ret = Rte_Read_Rp2_<RpSignal4>(&<SwcName>_Inputs.<RpSignal4>);
    if (ret != RTE_E_OK)
    {
        <SwcName>_Inputs.<RpSignal4> = 0;
        <SwcName>_DiagEventCnt++;
    }

    ret = Rte_Read_Rp2_<RpSignal5>(&<SwcName>_Inputs.<RpSignal5>);
    if (ret != RTE_E_OK)
    {
        <SwcName>_Inputs.<RpSignal5> = 0u;
        <SwcName>_DiagEventCnt++;
    }

    ret = Rte_Read_Rp2_<RpSignal6>(&<SwcName>_Inputs.<RpSignal6>);
    if (ret != RTE_E_OK)
    {
        <SwcName>_Inputs.<RpSignal6> = 0.0f;
        <SwcName>_DiagEventCnt++;
    }

    /* TODO: add Rp3, Rp4, ... blocks following the same pattern */
}

/**
 * @brief  Write the local output buffer to all provide-port signals.
 */
static void <SwcName>_WriteOutputs(void)
{
    /* --- Pp1 ---------------------------------------------------------------- */
    (void)Rte_Write_Pp1_<PpSignal1>(<SwcName>_Outputs.<PpSignal1>);
    (void)Rte_Write_Pp1_<PpSignal2>(<SwcName>_Outputs.<PpSignal2>);
    (void)Rte_Write_Pp1_<PpSignal3>(<SwcName>_Outputs.<PpSignal3>);

    /* --- Pp2 ---------------------------------------------------------------- */
    (void)Rte_Write_Pp2_<PpSignal4>(<SwcName>_Outputs.<PpSignal4>);
    (void)Rte_Write_Pp2_<PpSignal5>(<SwcName>_Outputs.<PpSignal5>);
    (void)Rte_Write_Pp2_<PpSignal6>(<SwcName>_Outputs.<PpSignal6>);

    /* TODO: add Pp3, Pp4, ... blocks following the same pattern */
}

/**
 * @brief  Check input plausibility and detect fault conditions.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return TRUE if a fault is present, FALSE otherwise.
 */
static boolean <SwcName>_CheckFaults(const <SwcName>_InputData_t *inputs)
{
    boolean faultDetected = FALSE;

    /* TODO: replace with real plausibility / range checks, e.g.: */
    if (inputs-><RpSignal1> > <SWCNAME>_<LIMIT_1>_MAX)
    {
        faultDetected = TRUE;
    }

    /* TODO: add further cross-signal plausibility checks here */

    return faultDetected;
}

/**
 * @brief  Compute <Output1> from the current input snapshot.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return Computed <Output1> value.
 */
static uint8 <SwcName>_Calc<Output1>(const <SwcName>_InputData_t *inputs)
{
    uint8 result = 0u;

    /* TODO: implement calculation logic */
    (void)inputs;

    return result;
}

/**
 * @brief  Compute <Output2> from the current input snapshot.
 * @param  inputs  Pointer to the current input data snapshot.
 * @return Computed <Output2> value.
 */
static uint8 <SwcName>_Calc<Output2>(const <SwcName>_InputData_t *inputs)
{
    uint8 result = 0u;

    /* TODO: implement calculation logic */
    (void)inputs;

    return result;
}

/*==============================================================================
 *  RUNNABLE ENTITY IMPLEMENTATIONS
 *============================================================================*/

/**
 * @brief  <SwcName>_Init_Runnable
 *         Called once at ECU startup (InitEvent).
 *         Initialises all internal state and writes safe default outputs.
 */
void <SwcName>_Init_Runnable(void)
{
    /* Zero all local structures */
    <SwcName>_Inputs  = (<SwcName>_InputData_t) {0};
    <SwcName>_Outputs = (<SwcName>_OutputData_t){0};

    <SwcName>_DiagEventCnt = 0u;

    /* TODO: set safe initial output values explicitly, e.g.: */
    <SwcName>_Outputs.<PpSignal1> = 0u;
    <SwcName>_Outputs.<PpSignal2> = FALSE;
    <SwcName>_Outputs.<PpSignal3> = 0u;

    /* Write safe defaults immediately */
    <SwcName>_WriteOutputs();

    <SwcName>_Initialized = TRUE;
}

/**
 * @brief  <SwcName>_<Period>ms_Runnable
 *         Cyclic runnable triggered every <Period> ms.
 *
 *         Execution order:
 *           1. Read all require-port inputs
 *           2. Run fault detection
 *           3. Compute output signals
 *           4. Write all provide-port outputs
 */
void <SwcName>_<Period>ms_Runnable(void)
{
    boolean faultPresent;

    if (<SwcName>_Initialized == FALSE)
    {
        /* Safety guard: do not execute before init */
        return;
    }

    /* ------------------------------------------------------------------ */
    /* Step 1: Read inputs                                                  */
    /* ------------------------------------------------------------------ */
    <SwcName>_ReadInputs();

    /* ------------------------------------------------------------------ */
    /* Step 2: Fault detection                                              */
    /* ------------------------------------------------------------------ */
    faultPresent = <SwcName>_CheckFaults(&<SwcName>_Inputs);

    if (faultPresent == TRUE)
    {
        <SwcName>_DiagEventCnt++;
    }

    /* ------------------------------------------------------------------ */
    /* Step 3: Compute outputs                                              */
    /* ------------------------------------------------------------------ */

    /* TODO: assign computed values into <SwcName>_Outputs, e.g.: */
    <SwcName>_Outputs.<PpSignal1> = <SwcName>_Calc<Output1>(&<SwcName>_Inputs);
    <SwcName>_Outputs.<PpSignal4> = <SwcName>_Calc<Output2>(&<SwcName>_Inputs);

    /* ------------------------------------------------------------------ */
    /* Step 4: Write outputs                                                */
    /* ------------------------------------------------------------------ */
    <SwcName>_WriteOutputs();
}
