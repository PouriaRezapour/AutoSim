/**
 * @file    Hmi.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR SWC Interface Header: Hmi
 *
 * @details
 * Public interface for the Hmi software component.
 *
 * The Hmi SWC is responsible for:
 *   - Debounce-free rising-edge detection on the blue user button (10 ms task)
 *   - Accumulating a float counter (step = BTN_INCREMENT) on each press
 *   - Writing the accumulated value to the PpBtnValue provide port via Rte_Hmi
 *
 * Require Ports:
 *   RpDio      — UsrBtn    (boolean)  Raw button state from DIO hardware,
 *                                     obtained via Rte_Call (BSW-bound pin
 *                                     access, not SWC-to-SWC communication)
 *
 * Provide Ports:
 *   PpBtnValue — BtnValue  (float)    Accumulated value for PcComMgr,
 *                                     written via Rte_Write (genuine
 *                                     SWC-to-SWC communication)
 *
 * Design note — accumulated value ownership
 * ──────────────────────────────────────────
 *   The Hmi SWC owns the accumulated BtnValue as an internal SWC variable
 *   (Hmi_Outputs.BtnValue). Reading back a Pp signal inside the same SWC is
 *   an AUTOSAR violation; the SWC therefore never calls Rte_Read on its own
 *   provide port. The value persists across cycles through the static
 *   Hmi_Outputs structure.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial consolidated release         |
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | RpDio.UsrBtn is BSW-bound; switched  |
 * |         |            |                  | from Rte_Read_RpDio_UsrBtn to        |
 * |         |            |                  | Rte_Call_RpDio_UsrBtn.               |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef HMI_H
#define HMI_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_Hmi.h"

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define HMI_SW_MAJOR_VERSION    1u
#define HMI_SW_MINOR_VERSION    0u
#define HMI_SW_PATCH_VERSION    0u

/* ─── Status Codes ────────────────────────────────────────────────────────── */

#define HMI_STATUS_OK           (0x00u)
#define HMI_STATUS_DEGRADED     (0x01u)
#define HMI_STATUS_FAULT        (0x02u)

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/** @brief  Input snapshot collected each runnable cycle. */
typedef struct
{
    Rte_DT_RpDio_UsrBtn_t       UsrBtn;     /**< Current button pin state       */
} Hmi_InputData_t;

/** @brief  Output snapshot written to provide ports each runnable cycle.
 *  @note   BtnValue is also the SWC's internal accumulator; it is never
 *          read back from the provide port (that would violate AUTOSAR Pp rules). */
typedef struct
{
    Rte_DT_PpBtnValue_BtnValue_t BtnValue;  /**< Accumulated press counter      */
} Hmi_OutputData_t;

/* ─── Public Function Prototypes (Runnables) ─────────────────────────────── */

/**
 * @brief  Hmi init runnable — called once at ECU startup.
 * @details
 * Reads initial button state and writes 0.0 to the BtnValue provide port.
 */
void Hmi_Init(void);

/**
 * @brief  Hmi 10 ms cyclic runnable — user button processing.
 * @details
 * Detects rising edges on UsrBtn and increments the BtnValue provide-port
 * signal by BTN_INCREMENT (0.5) on each detected press.
 */
void Hmi_Run_10ms(void);

#endif /* HMI_H */
