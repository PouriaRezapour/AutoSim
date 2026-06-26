/**
 * @file    Rte_PcComMgr.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-23
 * @version 0.2.4
 * @brief   AUTOSAR RTE Interface Header for SWC: PcComMgr
 *
 * @details
 * Single consolidated RTE header for the PcComMgr software component.
 *
 * Port naming follows the AUTOSAR convention: no underscore between the
 * Pp/Rp prefix and the port name. Inter-SWC ports share the same base name
 * differentiated only by the Pp/Rp prefix:
 *   Hmi provides  → PpBtnValue
 *   PcComMgr reads → RpBtnValue   (same base name, Rp prefix)
 *
 * Require Ports:
 *   RpBtnValue — BtnValue   (float)    Accumulated button value from Hmi SWC
 *   RpEth      — RxHeader0  (uint8)    Received packet header byte 0
 *                RxHeader1  (uint8)    Received packet header byte 1
 *                RxCmdType  (uint8)    Received command type
 *                RxCmdValue (uint32)   Received command value
 *                RxSetpoint (float)    Received setpoint
 *                Timestamp  (uint32)   Current system tick from HAL
 *
 * Provide Ports:
 *   PpEth      — TxHeader   (uint8[2]) TX packet header bytes
 *                TxCounter  (uint32)   TX packet counter
 *                TxTimestamp(uint32)   TX packet timestamp
 *                TxBtnValue (float)    TX button value field
 *                TxConstF2  (float)    TX constant float field
 *
 * Cross-SWC access
 * ─────────────────
 *   Rte_PcComMgr.c reads BtnValue via Rte_Buffers_Get_BtnValue() from
 *   Rte_Buffers. It does NOT include Rte_Hmi.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial consolidated release         |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef RTE_PCCOMMGR_H
#define RTE_PCCOMMGR_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Module Info ─────────────────────────────────────────────────────────── */

/** AUTOSAR vendor identifier (project-wide) */
#define RTE_PCCOMMGR_VENDOR_ID              0xFFFFu

/** AUTOSAR module identifier */
#define RTE_PCCOMMGR_MODULE_ID              2u

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define RTE_PCCOMMGR_SW_MAJOR_VERSION       1u
#define RTE_PCCOMMGR_SW_MINOR_VERSION       0u
#define RTE_PCCOMMGR_SW_PATCH_VERSION       0u

/*==============================================================================
 *  TYPE DEFINITIONS  (function pointer for pre-build hook)
 *============================================================================*/

/**
 * @brief  Function pointer type for the TX pre-build hook callback.
 * @note   Registered with the Eth UDP server; invoked before each packet build.
 */
typedef void (*Rte_PcComMgr_PreBuildHook_t)(void);

/*==============================================================================
 *  DATA TYPES  (signal-level types used by the PcComMgr SWC)
 *============================================================================*/

/* --- RpBtnValue signals --------------------------------------------------- */

/** @brief  Accumulated button value supplied by the Hmi SWC.
 *  @note   Port name RpBtnValue matches Hmi's PpBtnValue (same base name). */
typedef float   Rte_DT_RpBtnValue_BtnValue_t;

/* --- RpEth signals -------------------------------------------------------- */

typedef uint8   Rte_DT_RpEth_RxHeader0_t;    /**< RX header byte 0          */
typedef uint8   Rte_DT_RpEth_RxHeader1_t;    /**< RX header byte 1          */
typedef uint8   Rte_DT_RpEth_RxCmdType_t;    /**< RX command type           */
typedef uint32  Rte_DT_RpEth_RxCmdValue_t;   /**< RX command value          */
typedef float   Rte_DT_RpEth_RxSetpoint_t;   /**< RX setpoint               */
typedef uint32  Rte_DT_RpEth_Timestamp_t;    /**< Current system tick (ms)  */

/* --- PpEth signals -------------------------------------------------------- */

typedef uint8   Rte_DT_PpEth_TxHeader0_t;    /**< TX header byte 0          */
typedef uint8   Rte_DT_PpEth_TxHeader1_t;    /**< TX header byte 1          */
typedef uint32  Rte_DT_PpEth_TxCounter_t;    /**< TX packet counter         */
typedef uint32  Rte_DT_PpEth_TxTimestamp_t;  /**< TX timestamp (ms)         */
typedef float   Rte_DT_PpEth_TxBtnValue_t;   /**< TX button value field     */
typedef float   Rte_DT_PpEth_TxConstF2_t;    /**< TX constant float field   */

/*==============================================================================
 *  REQUIRE PORT MACROS  — Rte_Read_<Port>_<Signal>()
 *============================================================================*/

/* ---------- RpBtnValue ---------------------------------------------------- */

/**
 * @brief  Read BtnValue from the RpBtnValue require port (inter-SWC).
 * @param  data  Pointer to receive the float value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpBtnValue_BtnValue(data) \
    Rte_Read_PcComMgr_RpBtnValue_BtnValue(data)

/* ---------- RpEth --------------------------------------------------------- */

/**
 * @brief  Read RX header byte 0 from the RpEth require port.
 * @param  data  Pointer to receive the uint8 value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpEth_RxHeader0(data) \
    Rte_Read_PcComMgr_RpEth_RxHeader0(data)

/**
 * @brief  Read RX header byte 1 from the RpEth require port.
 * @param  data  Pointer to receive the uint8 value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpEth_RxHeader1(data) \
    Rte_Read_PcComMgr_RpEth_RxHeader1(data)

/**
 * @brief  Read RX command type from the RpEth require port.
 * @param  data  Pointer to receive the uint8 value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpEth_RxCmdType(data) \
    Rte_Read_PcComMgr_RpEth_RxCmdType(data)

/**
 * @brief  Read RX command value from the RpEth require port.
 * @param  data  Pointer to receive the uint32 value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpEth_RxCmdValue(data) \
    Rte_Read_PcComMgr_RpEth_RxCmdValue(data)

/**
 * @brief  Read RX setpoint from the RpEth require port.
 * @param  data  Pointer to receive the float value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpEth_RxSetpoint(data) \
    Rte_Read_PcComMgr_RpEth_RxSetpoint(data)

/**
 * @brief  Read the current system timestamp from the RpEth require port.
 * @param  data  Pointer to receive the uint32 tick value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Read_RpEth_Timestamp(data) \
    Rte_Read_PcComMgr_RpEth_Timestamp(data)

/*==============================================================================
 *  PROVIDE PORT MACROS  — Rte_Write_<Port>_<Signal>()
 *============================================================================*/

/* ---------- PpEth --------------------------------------------------------- */

/**
 * @brief  Write TX header bytes to the PpEth provide port.
 * @param  header0  First header byte.
 * @param  header1  Second header byte.
 */
#define Rte_Write_PpEth_TxHeader(header0, header1) \
    Rte_Write_PcComMgr_PpEth_TxHeader((header0), (header1))

/**
 * @brief  Write TX counter to the PpEth provide port.
 * @param  data  Packet counter value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Write_PpEth_TxCounter(data) \
    Rte_Write_PcComMgr_PpEth_TxCounter(data)

/**
 * @brief  Write TX timestamp to the PpEth provide port.
 * @param  data  Timestamp value in milliseconds.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Write_PpEth_TxTimestamp(data) \
    Rte_Write_PcComMgr_PpEth_TxTimestamp(data)

/**
 * @brief  Write TX button value to the PpEth provide port.
 * @param  data  Float button value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Write_PpEth_TxBtnValue(data) \
    Rte_Write_PcComMgr_PpEth_TxBtnValue(data)

/**
 * @brief  Write TX constant float field to the PpEth provide port.
 * @param  data  Float constant value.
 * @return Std_ReturnType  RTE_E_OK on success.
 */
#define Rte_Write_PpEth_TxConstF2(data) \
    Rte_Write_PcComMgr_PpEth_TxConstF2(data)

/*==============================================================================
 *  ETH PACKET MANAGEMENT
 *============================================================================*/

/**
 * @brief  Initialize the Ethernet hardware abstraction layer.
 */
void Rte_PcComMgr_EthInit(void);

/**
 * @brief  Commit the assembled TX signals to the hardware abstraction layer
 *         and mark the packet ready for UDP transmission.
 */
void Rte_PcComMgr_CommitTxPacket(void);

/**
 * @brief  Validate the received RX packet and load its fields into signal store.
 * @return TRUE if a valid packet was loaded, FALSE if checksum or header failed.
 */
boolean Rte_PcComMgr_LoadRxPacket(void);

/*==============================================================================
 *  REGISTRATION
 *============================================================================*/

/**
 * @brief  Register the TX pre-build hook callback with the Ethernet layer.
 * @param  hook  Function pointer to the callback (typically PcComMgr_PrepareTx).
 * @note   Must be called before Rte_PcComMgr_RegisterPktBuilder().
 */
void Rte_PcComMgr_RegisterPreBuildHook(Rte_PcComMgr_PreBuildHook_t hook);

/**
 * @brief  Register the TX packet builder with the Ethernet UDP server.
 * @note   Must be called once after Eth_UdpServerInit().
 */
void Rte_PcComMgr_RegisterPktBuilder(void);

/**
 * @brief  Reset the shared BtnValue signal buffer to zero.
 * @details
 * Writes 0.0 to Rte_Buffers so that the Hmi accumulated value is cleared.
 * Called by PcComMgr_ResetBtnValue(). Does not include Rte_Hmi.h.
 */
void Rte_PcComMgr_ResetBtnValue(void);

/*==============================================================================
 *  INTERNAL FUNCTION DECLARATIONS
 *  Called via the macros above — not called directly by the SWC.
 *============================================================================*/

/* RpBtnValue */
extern Std_ReturnType Rte_Read_PcComMgr_RpBtnValue_BtnValue(Rte_DT_RpBtnValue_BtnValue_t *data);

/* RpEth */
extern Std_ReturnType Rte_Read_PcComMgr_RpEth_RxHeader0 (Rte_DT_RpEth_RxHeader0_t  *data);
extern Std_ReturnType Rte_Read_PcComMgr_RpEth_RxHeader1 (Rte_DT_RpEth_RxHeader1_t  *data);
extern Std_ReturnType Rte_Read_PcComMgr_RpEth_RxCmdType (Rte_DT_RpEth_RxCmdType_t  *data);
extern Std_ReturnType Rte_Read_PcComMgr_RpEth_RxCmdValue(Rte_DT_RpEth_RxCmdValue_t *data);
extern Std_ReturnType Rte_Read_PcComMgr_RpEth_RxSetpoint (Rte_DT_RpEth_RxSetpoint_t *data);
extern Std_ReturnType Rte_Read_PcComMgr_RpEth_Timestamp  (Rte_DT_RpEth_Timestamp_t  *data);

/* PpEth */
extern void           Rte_Write_PcComMgr_PpEth_TxHeader   (Rte_DT_PpEth_TxHeader0_t  header0,
                                                             Rte_DT_PpEth_TxHeader1_t  header1);
extern Std_ReturnType Rte_Write_PcComMgr_PpEth_TxCounter  (Rte_DT_PpEth_TxCounter_t   data);
extern Std_ReturnType Rte_Write_PcComMgr_PpEth_TxTimestamp(Rte_DT_PpEth_TxTimestamp_t data);
extern Std_ReturnType Rte_Write_PcComMgr_PpEth_TxBtnValue (Rte_DT_PpEth_TxBtnValue_t  data);
extern Std_ReturnType Rte_Write_PcComMgr_PpEth_TxConstF2  (Rte_DT_PpEth_TxConstF2_t   data);

#ifdef __cplusplus
}
#endif

#endif /* RTE_PCCOMMGR_H */
