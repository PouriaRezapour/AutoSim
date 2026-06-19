/**
 * @file    <SwcName>.h
 * @brief   AUTOSAR SWC Interface Header: <SwcName>
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

#ifndef <SWCNAME>_H
#define <SWCNAME>_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_<SwcName>.h"

/*==============================================================================
 *  VERSION INFORMATION
 *============================================================================*/
#define <SWCNAME>_SW_MAJOR_VERSION   1u
#define <SWCNAME>_SW_MINOR_VERSION   0u
#define <SWCNAME>_SW_PATCH_VERSION   0u

/*==============================================================================
 *  LOCAL DEFINES  (placeholders — rename / adjust to your SWC's needs)
 *============================================================================*/
#define <SWCNAME>_STATUS_OK         (0x00u)
#define <SWCNAME>_STATUS_DEGRADED   (0x01u)
#define <SWCNAME>_STATUS_FAULT      (0x02u)

#define <SWCNAME>_<LIMIT_1>_MAX     (0u)   /* TODO: replace with real limit */
#define <SWCNAME>_<LIMIT_2>_MAX     (0u)   /* TODO: replace with real limit */

/*==============================================================================
 *  TYPE DEFINITIONS
 *============================================================================*/
typedef struct
{
    /* --- Rp1 --- */
    Rte_DT_Rp1_<RpSignal1>_t   <RpSignal1>;
    Rte_DT_Rp1_<RpSignal2>_t   <RpSignal2>;
    Rte_DT_Rp1_<RpSignal3>_t   <RpSignal3>;

    /* --- Rp2 --- */
    Rte_DT_Rp2_<RpSignal4>_t   <RpSignal4>;
    Rte_DT_Rp2_<RpSignal5>_t   <RpSignal5>;
    Rte_DT_Rp2_<RpSignal6>_t   <RpSignal6>;

    /* TODO: add further require-port signal fields here */

} <SwcName>_InputData_t;

typedef struct
{
    /* --- Pp1 --- */
    Rte_DT_Pp1_<PpSignal1>_t   <PpSignal1>;
    Rte_DT_Pp1_<PpSignal2>_t   <PpSignal2>;
    Rte_DT_Pp1_<PpSignal3>_t   <PpSignal3>;

    /* --- Pp2 --- */
    Rte_DT_Pp2_<PpSignal4>_t   <PpSignal4>;
    Rte_DT_Pp2_<PpSignal5>_t   <PpSignal5>;
    Rte_DT_Pp2_<PpSignal6>_t   <PpSignal6>;

    /* TODO: add further provide-port signal fields here */

} <SwcName>_OutputData_t;

/* ─── Public Function Prototypes (Runnables) ─────────────────────────────── */

/**
 * @brief  <SwcName>_Init_Runnable
 *         Called once at ECU startup (InitEvent).
 *         Initialises all internal state and writes safe default outputs.
 */
void <SwcName>_Init_Runnable(void);

/**
 * @brief  <SwcName>_<Period>ms_Runnable
 *         Cyclic runnable triggered every <Period> ms.
 */
void <SwcName>_<Period>ms_Runnable(void);

#endif /* <SWCNAME>_H */
