/**
 * @file    Rte_PcComMgr.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-23
 * @version 0.2.4
 * @brief   AUTOSAR RTE Implementation for SWC: PcComMgr
 *
 * @details
 * Single consolidated RTE implementation for the PcComMgr software component.
 *
 * This is the only file outside the Ethernet layer permitted to include
 * Eth_HwAb.h, Eth_UdpServer.h, Eth_Cfg.h, and Mcal.h.
 *
 * Cross-SWC signal access
 * ────────────────────────
 *   BtnValue is read from Rte_Buffers via Rte_Buffers_Get_BtnValue(). This
 *   is the only cross-SWC connector in the system. Rte_Hmi.h is NOT included
 *   here; each RTE module is self-contained with respect to other SWC headers.
 *
 * Port naming
 * ───────────
 *   Ports follow the convention: no underscore after Pp/Rp prefix.
 *   Inter-SWC ports share the same base name (PpBtnValue / RpBtnValue).
 *
 * TX signal storage is held here; RX signal storage is loaded from hardware
 * via Rte_PcComMgr_LoadRxPacket().
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.4   | 2025-06-23 | A.Rezapour       | Initial consolidated release         |
 * |---------|------------|------------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Rte_Type.h"
#include "Rte_PcComMgr.h"
#include "Rte_Buffers.h"
#include "Eth_HwAb.h"
#include "Eth_UdpServer.h"
#include "Eth_Cfg.h"
#include "Mcal.h"

/* ─── Private Variables (TX signal storage) ──────────────────────────────── */

static Rte_DT_PpEth_TxHeader0_t   s_txHeader[2]  = {0xAAU, 0xBBU};
static Rte_DT_PpEth_TxCounter_t   s_txCounter    = 0U;
static Rte_DT_PpEth_TxTimestamp_t s_txTimestamp  = 0U;
static Rte_DT_PpEth_TxBtnValue_t  s_txBtnValue   = 0.0f;
static Rte_DT_PpEth_TxConstF2_t   s_txConstF2    = 0.0f;

/* ─── Private Variables (RX signal storage) ──────────────────────────────── */

static Rte_DT_RpEth_RxHeader0_t   s_rxHeader[2]  = {0xCCU, 0xDDU};
static Rte_DT_RpEth_RxCmdType_t   s_rxCmdType    = 0U;
static Rte_DT_RpEth_RxCmdValue_t  s_rxCmdValue   = 0U;
static Rte_DT_RpEth_RxSetpoint_t  s_rxSetpoint   = 0.0f;

/* ─── Private Variables (hook storage) ───────────────────────────────────── */

static Rte_PcComMgr_PreBuildHook_t s_preBuildHook = NULL_PTR;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static uint16 Rte_PcComMgr_BuildAndCopyTxPacket(uint8 *data, uint16 maxSize);

/* ─── Private Function Implementations ───────────────────────────────────── */

/**
 * @brief  UDP packet builder callback registered with Eth_UdpServer.
 * @details
 * Matches the UdpPktBldr_t signature. Calls the pre-build hook (which
 * populates TX signals via the SWC), commits those signals to hardware,
 * then copies the assembled packet into the provided UDP buffer.
 * @param  data     Destination buffer supplied by Eth_UdpServer.
 * @param  maxSize  Maximum bytes available in the buffer.
 * @return Number of bytes written.
 */
static uint16 Rte_PcComMgr_BuildAndCopyTxPacket(uint8 *data, uint16 maxSize)
{
    Rte_PcComMgr_CommitTxPacket();

    Eth_TxPkt_t *txPkt = Eth_HwAb_GetTxPacket();
    Eth_TxPkt_t *out   = (Eth_TxPkt_t *)data;
    *out = *txPkt;

    (void)maxSize;
    return sizeof(Eth_TxPkt_t);
}

/* ─── Require Port Implementations (RpBtnValue) ──────────────────────────── */

/**
 * @brief  Read BtnValue from the shared inter-SWC signal buffer.
 * @details
 * Delegates to Rte_Buffers_Get_BtnValue() — the single authoritative
 * accessor for the Hmi→PcComMgr inter-SWC signal. Rte_Hmi.h is NOT
 * included; cross-SWC access is mediated exclusively by Rte_Buffers.
 * @param  data  Pointer to receive the float button value.
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Read_PcComMgr_RpBtnValue_BtnValue(Rte_DT_RpBtnValue_BtnValue_t *data)
{
    *data = Rte_Buffers_Get_BtnValue();
    return RTE_E_OK;
}

/* ─── Require Port Implementations (RpEth — RX signals) ──────────────────── */

Std_ReturnType Rte_Read_PcComMgr_RpEth_RxHeader0(Rte_DT_RpEth_RxHeader0_t *data)
{
    *data = s_rxHeader[0];
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_PcComMgr_RpEth_RxHeader1(Rte_DT_RpEth_RxHeader1_t *data)
{
    *data = s_rxHeader[1];
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_PcComMgr_RpEth_RxCmdType(Rte_DT_RpEth_RxCmdType_t *data)
{
    *data = s_rxCmdType;
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_PcComMgr_RpEth_RxCmdValue(Rte_DT_RpEth_RxCmdValue_t *data)
{
    *data = s_rxCmdValue;
    return RTE_E_OK;
}

Std_ReturnType Rte_Read_PcComMgr_RpEth_RxSetpoint(Rte_DT_RpEth_RxSetpoint_t *data)
{
    *data = s_rxSetpoint;
    return RTE_E_OK;
}

/**
 * @brief  Read current system timestamp via HAL abstraction.
 * @param  data  Pointer to receive the uint32 tick value.
 * @return RTE_E_OK always.
 */
Std_ReturnType Rte_Read_PcComMgr_RpEth_Timestamp(Rte_DT_RpEth_Timestamp_t *data)
{
    *data = HAL_GetTick();
    return RTE_E_OK;
}

/* ─── Provide Port Implementations (PpEth — TX signals) ──────────────────── */

void Rte_Write_PcComMgr_PpEth_TxHeader(Rte_DT_PpEth_TxHeader0_t header0,
                                         Rte_DT_PpEth_TxHeader1_t header1)
{
    s_txHeader[0] = header0;
    s_txHeader[1] = header1;
}

Std_ReturnType Rte_Write_PcComMgr_PpEth_TxCounter(Rte_DT_PpEth_TxCounter_t data)
{
    s_txCounter = data;
    return RTE_E_OK;
}

Std_ReturnType Rte_Write_PcComMgr_PpEth_TxTimestamp(Rte_DT_PpEth_TxTimestamp_t data)
{
    s_txTimestamp = data;
    return RTE_E_OK;
}

Std_ReturnType Rte_Write_PcComMgr_PpEth_TxBtnValue(Rte_DT_PpEth_TxBtnValue_t data)
{
    s_txBtnValue = data;
    return RTE_E_OK;
}

Std_ReturnType Rte_Write_PcComMgr_PpEth_TxConstF2(Rte_DT_PpEth_TxConstF2_t data)
{
    s_txConstF2 = data;
    return RTE_E_OK;
}

/* ─── Packet Management ───────────────────────────────────────────────────── */

/**
 * @brief  Initialize the Ethernet hardware abstraction layer and TX/RX signals.
 */
void Rte_PcComMgr_EthInit(void)
{
    s_txHeader[0] = 0xAAU;
    s_txHeader[1] = 0xBBU;
    s_txCounter   = 0U;
    s_txTimestamp = 0U;
    s_txBtnValue  = 0.0f;
    s_txConstF2   = 0.0f;

    s_rxHeader[0] = 0xCCU;
    s_rxHeader[1] = 0xDDU;
    s_rxCmdType   = 0U;
    s_rxCmdValue  = 0U;
    s_rxSetpoint  = 0.0f;

    Eth_HwAb_Init();
}

/**
 * @brief  Commit TX signal store to the hardware abstraction layer.
 * @details
 * Writes each TX signal field into the hardware packet structure and
 * triggers the checksum update, making the packet ready for DMA transfer.
 */
void Rte_PcComMgr_CommitTxPacket(void)
{
    Eth_HwAb_FieldValue_t value;

    value.header[0] = s_txHeader[0];
    value.header[1] = s_txHeader[1];
    Eth_HwAb_WriteTx(HWAB_ETH_TX_HEADER, &value);

    value.u32 = s_txCounter;
    Eth_HwAb_WriteTx(HWAB_ETH_TX_COUNTER, &value);

    value.u32 = s_txTimestamp;
    Eth_HwAb_WriteTx(HWAB_ETH_TX_TIMESTAMP, &value);

    value.f32 = s_txBtnValue;
    Eth_HwAb_WriteTx(HWAB_ETH_TX_BTN_VALUE, &value);

    value.f32 = s_txConstF2;
    Eth_HwAb_WriteTx(HWAB_ETH_TX_CONST_F2, &value);

    Eth_HwAb_UpdateTxChecksum();
}

/**
 * @brief  Validate and load the received RX packet from the hardware layer.
 * @return TRUE  if the packet passed checksum and header validation.
 * @return FALSE if validation failed (signal store is not updated).
 */
boolean Rte_PcComMgr_LoadRxPacket(void)
{
    Eth_HwAb_FieldValue_t value;

    if (!Eth_HwAb_VerifyRxChecksum())
    {
        return FALSE;
    }

    Eth_HwAb_ReadRx(HWAB_ETH_RX_HEADER, &value);
    s_rxHeader[0] = value.header[0];
    s_rxHeader[1] = value.header[1];

    if ((s_rxHeader[0] != 0xCCU) || (s_rxHeader[1] != 0xDDU))
    {
        return FALSE;
    }

    Eth_HwAb_ReadRx(HWAB_ETH_RX_CMD_TYPE, &value);
    s_rxCmdType = value.u8;

    Eth_HwAb_ReadRx(HWAB_ETH_RX_CMD_VALUE, &value);
    s_rxCmdValue = value.u32;

    Eth_HwAb_ReadRx(HWAB_ETH_RX_SETPOINT, &value);
    s_rxSetpoint = value.f32;

    return TRUE;
}

/* ─── Registration ────────────────────────────────────────────────────────── */

/**
 * @brief  Store the pre-build hook for later registration with Eth_UdpServer.
 * @param  hook  Function pointer to the pre-build callback.
 */
void Rte_PcComMgr_RegisterPreBuildHook(Rte_PcComMgr_PreBuildHook_t hook)
{
    s_preBuildHook = hook;
}

/**
 * @brief  Register both the pre-build hook and the packet builder with Eth_UdpServer.
 * @note   Must be called after Rte_PcComMgr_RegisterPreBuildHook().
 */
void Rte_PcComMgr_RegisterPktBuilder(void)
{
    Eth_SetPreBuild(s_preBuildHook);
    Eth_SetPktBldr(Rte_PcComMgr_BuildAndCopyTxPacket);
}

/**
 * @brief  Reset the shared BtnValue signal buffer to zero.
 * @details
 * Writes 0.0 to the Rte_Buffers store, clearing the accumulated Hmi button
 * value. Does not include or call into Rte_Hmi; Rte_Buffers is the single
 * point of cross-SWC signal ownership.
 */
void Rte_PcComMgr_ResetBtnValue(void)
{
    Rte_Buffers_Set_BtnValue(0.0f);
}
