/**
 * @file    Rte_<SwcName>.c
 * @brief   AUTOSAR RTE Stub Implementation for SWC: <SwcName>
 *
 * @details
 * This file provides PLACEHOLDER / STUB implementations of the signal
 * read/write functions declared in Rte_<SwcName>.h, so that <SwcName>.c
 * can be compiled and unit-tested in isolation (e.g. on a PC, outside
 * the target ECU build).
 *
 * Replace the body of each Rte_Read_/Rte_Write_ function with calls into
 * your test harness, signal database, or simulation environment as
 * needed.
 *
 * SWC Name    : <SwcName>
 * Author      : Your Name
 * Date        : 20xx-xx-xx
 * Version     : x.x.x
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | x.x.x   | 20xx-xx-xx | Your Name  | Initial stub                        |
 * |---------|------------|------------|--------------------------------------|
 */

/*==============================================================================
 *  INCLUDES
 *============================================================================*/
#include "Rte_<SwcName>.h"

/*==============================================================================
 *  LOCAL VARIABLES  (stub signal storage)
 *============================================================================*/
static Rte_DT_Rp1_<RpSignal1>_t   Stub_Rp1_<RpSignal1>   = 0u;
static Rte_DT_Rp1_<RpSignal2>_t   Stub_Rp1_<RpSignal2>   = 0u;
static Rte_DT_Rp1_<RpSignal3>_t   Stub_Rp1_<RpSignal3>   = FALSE;

static Rte_DT_Rp2_<RpSignal4>_t   Stub_Rp2_<RpSignal4>   = 0;
static Rte_DT_Rp2_<RpSignal5>_t   Stub_Rp2_<RpSignal5>   = 0u;
static Rte_DT_Rp2_<RpSignal6>_t   Stub_Rp2_<RpSignal6>   = 0.0f;

static Rte_DT_Pp1_<PpSignal1>_t   Stub_Pp1_<PpSignal1>   = 0u;
static Rte_DT_Pp1_<PpSignal2>_t   Stub_Pp1_<PpSignal2>   = FALSE;
static Rte_DT_Pp1_<PpSignal3>_t   Stub_Pp1_<PpSignal3>   = 0u;

static Rte_DT_Pp2_<PpSignal4>_t   Stub_Pp2_<PpSignal4>   = 0u;
static Rte_DT_Pp2_<PpSignal5>_t   Stub_Pp2_<PpSignal5>   = FALSE;
static Rte_DT_Pp2_<PpSignal6>_t   Stub_Pp2_<PpSignal6>   = 0;

/* TODO: add storage for Rp3, Rp4, Pp3, Pp4, ... as needed */

/*==============================================================================
 *  REQUIRE PORT STUB IMPLEMENTATIONS  — Rte_Read_<SwcName>_<Port>_<Signal>()
 *============================================================================*/

/* ---------- Rp1 ----------------------------------------------------------- */
Std_ReturnType Rte_Read_<SwcName>_Rp1_<RpSignal1>(Rte_DT_Rp1_<RpSignal1>_t *data)
{
    *data = Stub_Rp1_<RpSignal1>;
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_<SwcName>_Rp1_<RpSignal2>(Rte_DT_Rp1_<RpSignal2>_t *data)
{
    *data = Stub_Rp1_<RpSignal2>;
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_<SwcName>_Rp1_<RpSignal3>(Rte_DT_Rp1_<RpSignal3>_t *data)
{
    *data = Stub_Rp1_<RpSignal3>;
    return RTE_E_OK;
}

/* ---------- Rp2 ----------------------------------------------------------- */
Std_ReturnType Rte_Read_<SwcName>_Rp2_<RpSignal4>(Rte_DT_Rp2_<RpSignal4>_t *data)
{
    *data = Stub_Rp2_<RpSignal4>;
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_<SwcName>_Rp2_<RpSignal5>(Rte_DT_Rp2_<RpSignal5>_t *data)
{
    *data = Stub_Rp2_<RpSignal5>;
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_<SwcName>_Rp2_<RpSignal6>(Rte_DT_Rp2_<RpSignal6>_t *data)
{
    *data = Stub_Rp2_<RpSignal6>;
    return RTE_E_OK;
}

/* TODO: add stub read implementations for Rp3, Rp4, ... here */

/*==============================================================================
 *  PROVIDE PORT STUB IMPLEMENTATIONS  — Rte_Write_<SwcName>_<Port>_<Signal>()
 *============================================================================*/

/* ---------- Pp1 ----------------------------------------------------------- */
Std_ReturnType Rte_Write_<SwcName>_Pp1_<PpSignal1>(Rte_DT_Pp1_<PpSignal1>_t data)
{
    Stub_Pp1_<PpSignal1> = data;
    return RTE_E_OK;
}

Std_ReturnType Rte_Write_<SwcName>_Pp1_<PpSignal2>(Rte_DT_Pp1_<PpSignal2>_t data)
{
    Stub_Pp1_<PpSignal2> = data;
    return RTE_E_OK;
}

Std_ReturnType Rte_Write_<SwcName>_Pp1_<PpSignal3>(Rte_DT_Pp1_<PpSignal3>_t data)
{
    Stub_Pp1_<PpSignal3> = data;
    return RTE_E_OK;
}

/* ---------- Pp2 ----------------------------------------------------------- */
Std_ReturnType Rte_Write_<SwcName>_Pp2_<PpSignal4>(Rte_DT_Pp2_<PpSignal4>_t data)
{
    Stub_Pp2_<PpSignal4> = data;
    return RTE_E_OK;
}

Std_ReturnType Rte_Write_<SwcName>_Pp2_<PpSignal5>(Rte_DT_Pp2_<PpSignal5>_t data)
{
    Stub_Pp2_<PpSignal5> = data;
    return RTE_E_OK;
}

Std_ReturnType Rte_Write_<SwcName>_Pp2_<PpSignal6>(Rte_DT_Pp2_<PpSignal6>_t data)
{
    Stub_Pp2_<PpSignal6> = data;
    return RTE_E_OK;
}

/* TODO: add stub write implementations for Pp3, Pp4, ... here */
