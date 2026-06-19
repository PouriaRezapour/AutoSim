/**
 * @file    Rte_<SwcName>.h
 * @brief   AUTOSAR RTE Interface Header for SWC: <SwcName>
 *
 * SWC Name    : <SwcName>
 * Author      : Your Name
 * Date        : 20xx-xx-xx
 *
 * Require Ports : Rp1 (<RpSignal1>, <RpSignal2>, <RpSignal3>)
 *                 Rp2 (<RpSignal4>, <RpSignal5>, <RpSignal6>)
 *                 (add / remove require ports as needed)
 *
 * Provide Ports : Pp1 (<PpSignal1>, <PpSignal2>, <PpSignal3>)
 *                 Pp2 (<PpSignal4>, <PpSignal5>, <PpSignal6>)
 *                 (add / remove provide ports as needed)
 */

#ifndef RTE_<SWCNAME>_H
#define RTE_<SWCNAME>_H

/*==============================================================================
 *  INCLUDES
 *============================================================================*/
#include "Rte_Type.h"     /* AUTOSAR RTE base types                          */
#include "Std_Types.h"    /* AUTOSAR standard types (uint8, uint16, etc.)    */

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
 *  DATA TYPES  (signal-level types used by this SWC)
 *============================================================================*/

/* --- Rp1 signals ---------------------------------------------------------- */
typedef uint16  Rte_DT_Rp1_<RpSignal1>_t;      /* <RpSignal1> [<unit>]      */
typedef uint8   Rte_DT_Rp1_<RpSignal2>_t;      /* <RpSignal2> [<unit>]      */
typedef boolean Rte_DT_Rp1_<RpSignal3>_t;      /* <RpSignal3> flag          */

/* --- Rp2 signals ---------------------------------------------------------- */
typedef sint16  Rte_DT_Rp2_<RpSignal4>_t;      /* <RpSignal4> [<unit>]      */
typedef uint8   Rte_DT_Rp2_<RpSignal5>_t;      /* <RpSignal5> validity flag */
typedef float32 Rte_DT_Rp2_<RpSignal6>_t;      /* <RpSignal6> [<unit>]      */

/* TODO: add Rp3, Rp4, ... signal typedefs here, following the same pattern */

/* --- Pp1 signals ---------------------------------------------------------- */
typedef uint8   Rte_DT_Pp1_<PpSignal1>_t;      /* <PpSignal1> [<unit>]      */
typedef boolean Rte_DT_Pp1_<PpSignal2>_t;      /* <PpSignal2> validity flag */
typedef uint8   Rte_DT_Pp1_<PpSignal3>_t;      /* <PpSignal3> selection     */

/* --- Pp2 signals ---------------------------------------------------------- */
typedef uint8   Rte_DT_Pp2_<PpSignal4>_t;      /* <PpSignal4> [<unit>]      */
typedef boolean Rte_DT_Pp2_<PpSignal5>_t;      /* <PpSignal5> validity flag */
typedef sint8   Rte_DT_Pp2_<PpSignal6>_t;      /* <PpSignal6> offset        */

/* TODO: add Pp3, Pp4, ... signal typedefs here, following the same pattern */

/*==============================================================================
 *  RTE INSTANCE HANDLE
 *============================================================================*/
typedef void* Rte_Instance;

/*==============================================================================
 *  REQUIRE PORT MACROS  — Rte_Read_<Port>_<Signal>()
 *
 *  Prototype:  Std_ReturnType Rte_Read_<Rp>_<Signal>(<type> *data)
 *  Returns:    RTE_E_OK (0) on success, error code otherwise
 *============================================================================*/

/* ---------- Rp1 ----------------------------------------------------------- */
/**
 * @brief  Read <RpSignal1> from Rp1
 * @param  data  Pointer to receive the signal value
 * @return Std_ReturnType
 */
#define Rte_Read_Rp1_<RpSignal1>(data) \
    Rte_Read_<SwcName>_Rp1_<RpSignal1>(data)

/**
 * @brief  Read <RpSignal2> from Rp1
 * @param  data  Pointer to receive the signal value
 * @return Std_ReturnType
 */
#define Rte_Read_Rp1_<RpSignal2>(data) \
    Rte_Read_<SwcName>_Rp1_<RpSignal2>(data)

/**
 * @brief  Read <RpSignal3> flag from Rp1
 * @param  data  Pointer to receive the signal value
 * @return Std_ReturnType
 */
#define Rte_Read_Rp1_<RpSignal3>(data) \
    Rte_Read_<SwcName>_Rp1_<RpSignal3>(data)

/* ---------- Rp2 ----------------------------------------------------------- */
/**
 * @brief  Read <RpSignal4> from Rp2
 * @param  data  Pointer to receive the signal value
 * @return Std_ReturnType
 */
#define Rte_Read_Rp2_<RpSignal4>(data) \
    Rte_Read_<SwcName>_Rp2_<RpSignal4>(data)

/**
 * @brief  Read <RpSignal5> flag from Rp2
 * @param  data  Pointer to receive the signal value
 * @return Std_ReturnType
 */
#define Rte_Read_Rp2_<RpSignal5>(data) \
    Rte_Read_<SwcName>_Rp2_<RpSignal5>(data)

/**
 * @brief  Read <RpSignal6> from Rp2
 * @param  data  Pointer to receive the signal value
 * @return Std_ReturnType
 */
#define Rte_Read_Rp2_<RpSignal6>(data) \
    Rte_Read_<SwcName>_Rp2_<RpSignal6>(data)

/* TODO: add Rp3, Rp4, ... read macros here, following the same pattern */

/*==============================================================================
 *  PROVIDE PORT MACROS  — Rte_Write_<Port>_<Signal>()
 *
 *  Prototype:  Std_ReturnType Rte_Write_<Pp>_<Signal>(<type> data)
 *  Returns:    RTE_E_OK (0) on success, error code otherwise
 *============================================================================*/

/* ---------- Pp1 ----------------------------------------------------------- */
/**
 * @brief  Write <PpSignal1> to Pp1
 * @param  data  Signal value to write
 * @return Std_ReturnType
 */
#define Rte_Write_Pp1_<PpSignal1>(data) \
    Rte_Write_<SwcName>_Pp1_<PpSignal1>(data)

/**
 * @brief  Write <PpSignal2> flag to Pp1
 * @param  data  Signal value to write
 * @return Std_ReturnType
 */
#define Rte_Write_Pp1_<PpSignal2>(data) \
    Rte_Write_<SwcName>_Pp1_<PpSignal2>(data)

/**
 * @brief  Write <PpSignal3> to Pp1
 * @param  data  Signal value to write
 * @return Std_ReturnType
 */
#define Rte_Write_Pp1_<PpSignal3>(data) \
    Rte_Write_<SwcName>_Pp1_<PpSignal3>(data)

/* ---------- Pp2 ----------------------------------------------------------- */
/**
 * @brief  Write <PpSignal4> to Pp2
 * @param  data  Signal value to write
 * @return Std_ReturnType
 */
#define Rte_Write_Pp2_<PpSignal4>(data) \
    Rte_Write_<SwcName>_Pp2_<PpSignal4>(data)

/**
 * @brief  Write <PpSignal5> flag to Pp2
 * @param  data  Signal value to write
 * @return Std_ReturnType
 */
#define Rte_Write_Pp2_<PpSignal5>(data) \
    Rte_Write_<SwcName>_Pp2_<PpSignal5>(data)

/**
 * @brief  Write <PpSignal6> to Pp2
 * @param  data  Signal value to write
 * @return Std_ReturnType
 */
#define Rte_Write_Pp2_<PpSignal6>(data) \
    Rte_Write_<SwcName>_Pp2_<PpSignal6>(data)

/* TODO: add Pp3, Pp4, ... write macros here, following the same pattern */

/*==============================================================================
 *  INTERNAL FUNCTION DECLARATIONS
 *  These are called via the macros above — not called directly by the SWC.
 *============================================================================*/

/* Rp1 */
extern Std_ReturnType Rte_Read_<SwcName>_Rp1_<RpSignal1>(Rte_DT_Rp1_<RpSignal1>_t *data);
extern Std_ReturnType Rte_Read_<SwcName>_Rp1_<RpSignal2>(Rte_DT_Rp1_<RpSignal2>_t *data);
extern Std_ReturnType Rte_Read_<SwcName>_Rp1_<RpSignal3>(Rte_DT_Rp1_<RpSignal3>_t *data);

/* Rp2 */
extern Std_ReturnType Rte_Read_<SwcName>_Rp2_<RpSignal4>(Rte_DT_Rp2_<RpSignal4>_t *data);
extern Std_ReturnType Rte_Read_<SwcName>_Rp2_<RpSignal5>(Rte_DT_Rp2_<RpSignal5>_t *data);
extern Std_ReturnType Rte_Read_<SwcName>_Rp2_<RpSignal6>(Rte_DT_Rp2_<RpSignal6>_t *data);

/* TODO: add extern read declarations for Rp3, Rp4, ... here */

/* Pp1 */
extern Std_ReturnType Rte_Write_<SwcName>_Pp1_<PpSignal1>(Rte_DT_Pp1_<PpSignal1>_t data);
extern Std_ReturnType Rte_Write_<SwcName>_Pp1_<PpSignal2>(Rte_DT_Pp1_<PpSignal2>_t data);
extern Std_ReturnType Rte_Write_<SwcName>_Pp1_<PpSignal3>(Rte_DT_Pp1_<PpSignal3>_t data);

/* Pp2 */
extern Std_ReturnType Rte_Write_<SwcName>_Pp2_<PpSignal4>(Rte_DT_Pp2_<PpSignal4>_t data);
extern Std_ReturnType Rte_Write_<SwcName>_Pp2_<PpSignal5>(Rte_DT_Pp2_<PpSignal5>_t data);
extern Std_ReturnType Rte_Write_<SwcName>_Pp2_<PpSignal6>(Rte_DT_Pp2_<PpSignal6>_t data);

/* TODO: add extern write declarations for Pp3, Pp4, ... here */

/*==============================================================================
 *  RUNNABLE ENTITY DECLARATIONS
 *============================================================================*/
/**
 * @brief  <SwcName> <Period> ms cyclic runnable — main processing step
 */
extern void <SwcName>_<Period>ms_Runnable(void);

/**
 * @brief  <SwcName> init runnable — called once at startup
 */
extern void <SwcName>_Init_Runnable(void);

#ifdef __cplusplus
}
#endif

#endif /* RTE_<SWCNAME>_H */
