/**
 * @file    PcComMgr.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-23
 * @version 0.2.4
 * @brief   AUTOSAR SWC Interface Header: PcComMgr
 *
 * @details
 * Public interface for the PC Communication Manager software component.
 * Uses the standard AUTOSAR SWC pattern referencing only the single
 * Rte_PcComMgr RTE module.
 *
 * Port naming: no underscore after Pp/Rp prefix (RpBtnValue, RpEth, PpEth).
 * Inter-SWC port names share the same base name:
 *   Hmi provides  → PpBtnValue
 *   PcComMgr reads → RpBtnValue
 *
 * The PcComMgr SWC is responsible for:
 *   - Constructing and transmitting UDP packets to the PC over Ethernet
 *   - Registering pre-build and packet-builder hooks with the UDP server
 *   - Populating all TX packet fields (header, counter, timestamp, button
 *     value, constant float) before each transmission cycle
 *   - Exposing management accessors for the constant float and button value
 *
 * Require Ports:
 *   RpBtnValue — BtnValue    (float)    Accumulated button value from Hmi SWC
 *   RpEth      — RxHeader0   (uint8)    Received packet header byte 0
 *                RxHeader1   (uint8)    Received packet header byte 1
 *                RxCmdType   (uint8)    Received command type
 *                RxCmdValue  (uint32)   Received command value
 *                RxSetpoint  (float)    Received setpoint
 *                Timestamp   (uint32)   Current system tick
 *
 * Provide Ports:
 *   PpEth      — TxHeader    (uint8[2]) TX packet header bytes
 *                TxCounter   (uint32)   TX packet counter
 *                TxTimestamp (uint32)   TX packet timestamp
 *                TxBtnValue  (float)    TX button value field
 *                TxConstF2   (float)    TX constant float field
 *
 * Permitted includes (in PcComMgr.c): PcComMgr.h, Rte_PcComMgr.h
 * Forbidden:    Eth_UdpServer.h, Eth_Cfg.h, Eth_HwAb.h, Mcal.h,
 *               Rte_HmiPcComIf.h, Rte_PcComEthIf.h, Rte_HmiDioIf.h
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial consolidated release         |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef PCCOMMGR_H
#define PCCOMMGR_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_PcComMgr.h"

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define PCCOMMGR_SW_MAJOR_VERSION   1u
#define PCCOMMGR_SW_MINOR_VERSION   0u
#define PCCOMMGR_SW_PATCH_VERSION   0u

/* ─── Status Codes ────────────────────────────────────────────────────────── */

#define PCCOMMGR_STATUS_OK          (0x00u)
#define PCCOMMGR_STATUS_DEGRADED    (0x01u)
#define PCCOMMGR_STATUS_FAULT       (0x02u)

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/** @brief  Input snapshot collected by PcComMgr_PrepareTx(). */
typedef struct
{
    /* --- RpBtnValue --- */
    Rte_DT_RpBtnValue_BtnValue_t BtnValue;     /**< Accumulated button value  */

    /* --- RpEth (RX fields) --- */
    Rte_DT_RpEth_RxHeader0_t     RxHeader0;    /**< RX header byte 0          */
    Rte_DT_RpEth_RxHeader1_t     RxHeader1;    /**< RX header byte 1          */
    Rte_DT_RpEth_RxCmdType_t     RxCmdType;    /**< RX command type           */
    Rte_DT_RpEth_RxCmdValue_t    RxCmdValue;   /**< RX command value          */
    Rte_DT_RpEth_RxSetpoint_t    RxSetpoint;   /**< RX setpoint               */
    Rte_DT_RpEth_Timestamp_t     Timestamp;    /**< Current system tick (ms)  */
} PcComMgr_InputData_t;

/** @brief  Output snapshot written to Eth TX provide ports. */
typedef struct
{
    /* --- PpEth (TX fields) --- */
    Rte_DT_PpEth_TxHeader0_t     TxHeader0;    /**< TX header byte 0          */
    Rte_DT_PpEth_TxHeader1_t     TxHeader1;    /**< TX header byte 1          */
    Rte_DT_PpEth_TxCounter_t     TxCounter;    /**< TX packet counter         */
    Rte_DT_PpEth_TxTimestamp_t   TxTimestamp;  /**< TX timestamp (ms)         */
    Rte_DT_PpEth_TxBtnValue_t    TxBtnValue;   /**< TX button value field     */
    Rte_DT_PpEth_TxConstF2_t     TxConstF2;    /**< TX constant float field   */
} PcComMgr_OutputData_t;

/* ─── Public Function Prototypes (Runnables & API) ───────────────────────── */

/**
 * @brief  PcComMgr init runnable — called once at ECU startup.
 * @details
 * Initialises the Ethernet RTE layer, resets the packet counter, and
 * registers both the pre-build hook and the packet builder.
 */
void PcComMgr_Init(void);

/**
 * @brief  Refresh all TX signals into the RTE provide ports.
 * @details
 * Registered as the pre-build hook and called automatically by Eth_UdpServer
 * before each UDP packet is built and sent. Also callable manually.
 */
void PcComMgr_PrepareTx(void);

/**
 * @brief  Set the constant float value transmitted in every packet.
 * @param  value  New float value to store and transmit.
 */
void PcComMgr_SetConstFloat(float value);

/**
 * @brief  Get the current constant float value.
 * @return Current s_constF2 value.
 */
float PcComMgr_GetConstFloat(void);

/**
 * @brief  Get the current accumulated button value from the Hmi SWC.
 * @return Float value read from the Hmi PpBtnValue provide port via RpBtnValue.
 */
float PcComMgr_GetBtnValue(void);

/**
 * @brief  Reset the Hmi accumulated button value to zero.
 * @note   Writes 0.0 back to the shared Rte_Buffers BtnValue signal store,
 *         which is the same buffer written by Rte_Hmi's PpBtnValue provide port.
 */
void PcComMgr_ResetBtnValue(void);

#endif /* PCCOMMGR_H */
