/**
 * @file    CmdSigMgr.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-01
 * @version 0.2.5
 * @brief   AUTOSAR SWC Interface Header: CmdSigMgr
 *
 * @details
 * Public interface for the Command Signal Manager software component.
 *
 * The CmdSigMgr SWC is responsible for:
 *   - Reading a single packed uint8 command word from the RpCommandSignalsPacked
 *     require port (sourced from Rte_Buffers)
 *   - Masking off the unused bit 7 before use
 *   - Unpacking the remaining 7 bits into individual boolean command signals
 *   - Writing each individual command signal to the PpCommandSignals
 *     provide port, which drives the corresponding DIO output pin
 *
 * Require Ports:
 *   RpCommandSignalsPacked — Packed (uint8) Packed command bit field. This is
 *                            genuine SWC-to-SWC communication and is
 *                            obtained via Rte_Read.
 *
 * Provide Ports:
 *   PpCommandSignals — Vesc      (boolean) HWAB_DIO_CMD_VESC output state
 *                       Bls       (boolean) HWAB_DIO_CMD_BLS output state
 *                       Itpms     (boolean) HWAB_DIO_CMD_ITPMS output state
 *                       ParkBrake (boolean) HWAB_DIO_CMD_PARK_BRAKE output state
 *                       EscTcs    (boolean) HWAB_DIO_CMD_ESC_TCS output state
 *                       Wakeup    (boolean) HWAB_DIO_CMD_WAKEUP output state
 *                       Vmot      (boolean) HWAB_DIO_CMD_VMOT output state
 *
 *                       Each signal targets a Dio pin through the BSW (not
 *                       another SWC) and is therefore written via Rte_Call,
 *                       not Rte_Write.
 *
 * Bit mapping of the packed Rp command word:
 *   Bit 0 — Vesc
 *   Bit 1 — Bls
 *   Bit 2 — Itpms
 *   Bit 3 — ParkBrake
 *   Bit 4 — EscTcs
 *   Bit 5 — Wakeup
 *   Bit 6 — Vmot
 *   Bit 7 — Reserved, masked to 0 before use
 *
 * Permitted includes: CmdSigMgr.h, Rte_CmdSigMgr.h
 * Forbidden:           DioIf.h, Eth_*.h, Mcal.h, HAL_*.h, Rte_Buffers.h
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.5   | 2025-07-01 | A. Rezapour      | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef CMDSIGMGR_H
#define CMDSIGMGR_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_CmdSigMgr.h"

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define CMDSIGMGR_SW_MAJOR_VERSION   1u
#define CMDSIGMGR_SW_MINOR_VERSION   0u
#define CMDSIGMGR_SW_PATCH_VERSION   0u

/* ─── Status Codes ────────────────────────────────────────────────────────── */

#define CMDSIGMGR_STATUS_OK          (0x00u)
#define CMDSIGMGR_STATUS_DEGRADED    (0x01u)
#define CMDSIGMGR_STATUS_FAULT       (0x02u)

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/** @brief  Input snapshot collected each runnable cycle. */
typedef struct
{
    /* --- RpCommandSignalsPacked --- */
    Rte_DT_RpCommandSignalsPacked_Packed_t Packed;   /**< Packed command bit field (bit 7 reserved) */
} CmdSigMgr_InputData_t;

/** @brief  Output snapshot written to provide ports each runnable cycle. */
typedef struct
{
    /* --- PpCommandSignals --- */
    Rte_DT_PpCommandSignals_Vesc_t      Vesc;       /**< HWAB_DIO_CMD_VESC output state       */
    Rte_DT_PpCommandSignals_Bls_t       Bls;        /**< HWAB_DIO_CMD_BLS output state        */
    Rte_DT_PpCommandSignals_Itpms_t     Itpms;      /**< HWAB_DIO_CMD_ITPMS output state      */
    Rte_DT_PpCommandSignals_ParkBrake_t ParkBrake;  /**< HWAB_DIO_CMD_PARK_BRAKE output state */
    Rte_DT_PpCommandSignals_EscTcs_t    EscTcs;     /**< HWAB_DIO_CMD_ESC_TCS output state    */
    Rte_DT_PpCommandSignals_Wakeup_t    Wakeup;     /**< HWAB_DIO_CMD_WAKEUP output state     */
    Rte_DT_PpCommandSignals_Vmot_t      Vmot;       /**< HWAB_DIO_CMD_VMOT output state       */
} CmdSigMgr_OutputData_t;

/* ─── Public Function Prototypes (Runnables) ─────────────────────────────── */

/**
 * @brief  CmdSigMgr init runnable — called once at ECU startup.
 * @details
 * Zeroes all local structures and writes the safe default (all commands
 * inactive / LOW) to every provide-port signal.
 */
void CmdSigMgr_Init(void);

/**
 * @brief  CmdSigMgr 50 ms cyclic runnable — command signal unpacking.
 * @details
 * Reads the packed command word, masks off bit 7, unpacks the remaining
 * bits, and writes each individual command signal to the PpCommandSignals
 * provide port.
 */
void CmdSigMgr_Run_50ms(void);

#endif /* CMDSIGMGR_H */
