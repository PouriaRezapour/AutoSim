/**
 * @file    EthTrcv.c
 * @author  Ali Rezapour
 * @date    2025-05-10
 * @version 0.1.0
 * @brief   LAN8742 10/100 Ethernet PHY transceiver driver implementation.
 *
 * @details
 * Provides a hardware-independent driver for the SMSC/Microchip LAN8742
 * PHY. All bus access is performed through a caller-supplied I/O context
 * (lan8742_IOCtx_t), allowing the same driver code to work with any MDIO
 * implementation on any STM32 (or other) platform.
 *
 * Typical usage:
 *  1. Populate a lan8742_IOCtx_t with platform-specific Read/Write/Tick
 *     function pointers.
 *  2. Call LAN8742_RegisterBusIO() to bind the context to a device object.
 *  3. Call LAN8742_Init() to scan the MDIO bus and latch the PHY address.
 *  4. Use the remaining API functions to control link state, power mode,
 *     loopback, and interrupts.
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.1.0   | 2025-05-10 | Ali Rezapour  | Initial release                      |
 * |---------|------------|---------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "EthTrcv.h"

/* ─── Private Macros ──────────────────────────────────────────────────────── */

/** @brief Maximum valid MDIO device address (5-bit field, 0–31). */
#define LAN8742_MAX_DEV_ADDR  ((uint32)31U)

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Bind I/O functions to a device object.
 * @param  pObj   Pointer to the LAN8742 device object.
 * @param  ioctx  Pointer to the I/O context holding bus functions.
 * @return LAN8742_STATUS_OK if all mandatory pointers are present,
 *         LAN8742_STATUS_ERROR if pObj or any mandatory pointer is NULL.
 */
int32_t LAN8742_RegisterBusIO(lan8742_Object_t *pObj, lan8742_IOCtx_t *ioctx)
{
  if(!pObj || !ioctx->ReadReg || !ioctx->WriteReg || !ioctx->GetTick)
  {
    return LAN8742_STATUS_ERROR;
  }

  pObj->IO.Init     = ioctx->Init;
  pObj->IO.DeInit   = ioctx->DeInit;
  pObj->IO.ReadReg  = ioctx->ReadReg;
  pObj->IO.WriteReg = ioctx->WriteReg;
  pObj->IO.GetTick  = ioctx->GetTick;

  return LAN8742_STATUS_OK;
}

/**
 * @brief  Initialize the LAN8742 and discover its MDIO address.
 * @details
 *         Calls the optional IO.Init callback for GPIO/clock setup, then
 *         scans all 32 possible MDIO addresses by reading the Special Modes
 *         Register (SMR) and comparing the embedded PHY address field. The
 *         first matching address is stored in pObj->DevAddr.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_ADDRESS_ERROR if no device responds,
 *         LAN8742_STATUS_READ_ERROR if every SMR read fails.
 */
int32_t LAN8742_Init(lan8742_Object_t *pObj)
{
  uint32 regvalue = 0, addr = 0;
  int32_t  status   = LAN8742_STATUS_OK;

  if(pObj->Is_Initialized == 0)
  {
    if(pObj->IO.Init != 0)
    {
      /* GPIO and clocks initialisation */
      pObj->IO.Init();
    }

    /* Sentinel value; updated below if a valid address is found */
    pObj->DevAddr = LAN8742_MAX_DEV_ADDR + 1;

    /* Scan all MDIO addresses and match via the SMR PHY address field */
    for(addr = 0; addr <= LAN8742_MAX_DEV_ADDR; addr++)
    {
      if(pObj->IO.ReadReg(addr, LAN8742_SMR, &regvalue) < 0)
      {
        status = LAN8742_STATUS_READ_ERROR;
        /* Cannot read from this address — try the next one */
        continue;
      }

      if((regvalue & LAN8742_SMR_PHY_ADDR) == addr)
      {
        pObj->DevAddr = addr;
        status        = LAN8742_STATUS_OK;
        break;
      }
    }

    if(pObj->DevAddr > LAN8742_MAX_DEV_ADDR)
    {
      status = LAN8742_STATUS_ADDRESS_ERROR;
    }

    if(status == LAN8742_STATUS_OK)
    {
      pObj->Is_Initialized = 1;
    }
  }

  return status;
}

/**
 * @brief  De-initialize the LAN8742 and release hardware resources.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success, LAN8742_STATUS_ERROR if the
 *         IO.DeInit callback returns a negative value.
 */
int32_t LAN8742_DeInit(lan8742_Object_t *pObj)
{
  if(pObj->Is_Initialized)
  {
    if(pObj->IO.DeInit != 0)
    {
      if(pObj->IO.DeInit() < 0)
      {
        return LAN8742_STATUS_ERROR;
      }
    }

    pObj->Is_Initialized = 0;
  }

  return LAN8742_STATUS_OK;
}

/**
 * @brief  Clear the power-down bit in the BCR to wake the PHY.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the BCR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the BCR cannot be written.
 */
int32_t LAN8742_DisablePowerDownMode(lan8742_Object_t *pObj)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BCR, &readval) >= 0)
  {
    readval &= ~LAN8742_BCR_POWER_DOWN;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_BCR, readval) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Set the power-down bit in the BCR to put the PHY to sleep.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the BCR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the BCR cannot be written.
 */
int32_t LAN8742_EnablePowerDownMode(lan8742_Object_t *pObj)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BCR, &readval) >= 0)
  {
    readval |= LAN8742_BCR_POWER_DOWN;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_BCR, readval) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Enable and restart the auto-negotiation process.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the BCR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the BCR cannot be written.
 */
int32_t LAN8742_StartAutoNego(lan8742_Object_t *pObj)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BCR, &readval) >= 0)
  {
    readval |= LAN8742_BCR_AUTONEGO_EN;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_BCR, readval) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Read the current link state from the PHY registers.
 * @details
 *         The BSR is read twice to clear any latched fault bits, then the
 *         link status bit is checked. If a link is up and auto-negotiation
 *         is enabled, the PHYSCSR speed indication field is decoded;
 *         otherwise the BCR speed/duplex bits are used directly.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return One of the LAN8742_STATUS_xxx link or error codes.
 */
int32_t LAN8742_GetLinkState(lan8742_Object_t *pObj)
{
  uint32 readval = 0;

  /* Read BSR once (clears latched bits) then read again for current state */
  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BSR, &readval) < 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BSR, &readval) < 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  if((readval & LAN8742_BSR_LINK_STATUS) == 0)
  {
    return LAN8742_STATUS_LINK_DOWN;
  }

  /* Check whether auto-negotiation is active */
  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BCR, &readval) < 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  if((readval & LAN8742_BCR_AUTONEGO_EN) != LAN8742_BCR_AUTONEGO_EN)
  {
    /* Auto-negotiation disabled — decode BCR speed/duplex bits directly */
    if(((readval & LAN8742_BCR_SPEED_SELECT) == LAN8742_BCR_SPEED_SELECT) &&
       ((readval & LAN8742_BCR_DUPLEX_MODE)  == LAN8742_BCR_DUPLEX_MODE))
    {
      return LAN8742_STATUS_100MBITS_FULLDUPLEX;
    }
    else if((readval & LAN8742_BCR_SPEED_SELECT) == LAN8742_BCR_SPEED_SELECT)
    {
      return LAN8742_STATUS_100MBITS_HALFDUPLEX;
    }
    else if((readval & LAN8742_BCR_DUPLEX_MODE) == LAN8742_BCR_DUPLEX_MODE)
    {
      return LAN8742_STATUS_10MBITS_FULLDUPLEX;
    }
    else
    {
      return LAN8742_STATUS_10MBITS_HALFDUPLEX;
    }
  }
  else
  {
    /* Auto-negotiation enabled — decode result from PHYSCSR */
    if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_PHYSCSR, &readval) < 0)
    {
      return LAN8742_STATUS_READ_ERROR;
    }

    if((readval & LAN8742_PHYSCSR_AUTONEGO_DONE) == 0)
    {
      return LAN8742_STATUS_AUTONEGO_NOTDONE;
    }

    if((readval & LAN8742_PHYSCSR_HCDSPEEDMASK) == LAN8742_PHYSCSR_100BTX_FD)
    {
      return LAN8742_STATUS_100MBITS_FULLDUPLEX;
    }
    else if((readval & LAN8742_PHYSCSR_HCDSPEEDMASK) == LAN8742_PHYSCSR_100BTX_HD)
    {
      return LAN8742_STATUS_100MBITS_HALFDUPLEX;
    }
    else if((readval & LAN8742_PHYSCSR_HCDSPEEDMASK) == LAN8742_PHYSCSR_10BT_FD)
    {
      return LAN8742_STATUS_10MBITS_FULLDUPLEX;
    }
    else
    {
      return LAN8742_STATUS_10MBITS_HALFDUPLEX;
    }
  }
}

/**
 * @brief  Force a specific link speed and duplex mode via the BCR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  LinkState  Desired state; one of LAN8742_STATUS_100MBITS_FULLDUPLEX,
 *                    LAN8742_STATUS_100MBITS_HALFDUPLEX,
 *                    LAN8742_STATUS_10MBITS_FULLDUPLEX, or
 *                    LAN8742_STATUS_10MBITS_HALFDUPLEX.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_ERROR if LinkState is not a recognised value,
 *         LAN8742_STATUS_READ_ERROR if the BCR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the BCR cannot be written.
 */
int32_t LAN8742_SetLinkState(lan8742_Object_t *pObj, uint32 LinkState)
{
  uint32 bcrvalue = 0;
  int32_t  status   = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BCR, &bcrvalue) >= 0)
  {
    /* Disable auto-negotiation and clear current speed/duplex selection */
    bcrvalue &= ~(LAN8742_BCR_AUTONEGO_EN | LAN8742_BCR_SPEED_SELECT | LAN8742_BCR_DUPLEX_MODE);

    if(LinkState == LAN8742_STATUS_100MBITS_FULLDUPLEX)
    {
      bcrvalue |= (LAN8742_BCR_SPEED_SELECT | LAN8742_BCR_DUPLEX_MODE);
    }
    else if(LinkState == LAN8742_STATUS_100MBITS_HALFDUPLEX)
    {
      bcrvalue |= LAN8742_BCR_SPEED_SELECT;
    }
    else if(LinkState == LAN8742_STATUS_10MBITS_FULLDUPLEX)
    {
      bcrvalue |= LAN8742_BCR_DUPLEX_MODE;
    }
    else
    {
      /* LinkState value not recognised */
      status = LAN8742_STATUS_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  if(status == LAN8742_STATUS_OK)
  {
    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_BCR, bcrvalue) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }

  return status;
}

/**
 * @brief  Enable MAC-level loopback mode in the BCR.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the BCR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the BCR cannot be written.
 */
int32_t LAN8742_EnableLoopbackMode(lan8742_Object_t *pObj)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BCR, &readval) >= 0)
  {
    readval |= LAN8742_BCR_LOOPBACK;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_BCR, readval) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Disable MAC-level loopback mode in the BCR.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the BCR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the BCR cannot be written.
 */
int32_t LAN8742_DisableLoopbackMode(lan8742_Object_t *pObj)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_BCR, &readval) >= 0)
  {
    readval &= ~LAN8742_BCR_LOOPBACK;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_BCR, readval) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Unmask one or more interrupt sources in the IMR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags to enable. May be a
 *                    single flag or a bitwise OR of multiple flags.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the IMR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the IMR cannot be written.
 */
int32_t LAN8742_EnableIT(lan8742_Object_t *pObj, uint32 Interrupt)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_IMR, &readval) >= 0)
  {
    readval |= Interrupt;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_IMR, readval) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Mask one or more interrupt sources in the IMR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags to disable. May be a
 *                    single flag or a bitwise OR of multiple flags.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the IMR cannot be read,
 *         LAN8742_STATUS_WRITE_ERROR if the IMR cannot be written.
 */
int32_t LAN8742_DisableIT(lan8742_Object_t *pObj, uint32 Interrupt)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_IMR, &readval) >= 0)
  {
    readval &= ~Interrupt;

    /* Apply configuration */
    if(pObj->IO.WriteReg(pObj->DevAddr, LAN8742_IMR, readval) < 0)
    {
      status = LAN8742_STATUS_WRITE_ERROR;
    }
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Clear pending interrupt flags by reading the ISFR.
 * @details
 *         The ISFR is a read-to-clear register; the Interrupt parameter is
 *         accepted for API consistency but the hardware clears all flags on
 *         any read.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags (informational only).
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_READ_ERROR if the ISFR cannot be read.
 */
int32_t LAN8742_ClearIT(lan8742_Object_t *pObj, uint32 Interrupt)
{
  uint32 readval = 0;
  int32_t  status  = LAN8742_STATUS_OK;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_ISFR, &readval) < 0)
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}

/**
 * @brief  Check whether a specific interrupt flag is set in the ISFR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags to test.
 * @return 1 if all specified flags are set,
 *         0 if any specified flag is clear,
 *         LAN8742_STATUS_READ_ERROR if the ISFR cannot be read.
 */
int32_t LAN8742_GetITStatus(lan8742_Object_t *pObj, uint32 Interrupt)
{
  uint32 readval = 0;
  int32_t  status  = 0;

  if(pObj->IO.ReadReg(pObj->DevAddr, LAN8742_ISFR, &readval) >= 0)
  {
    status = ((readval & Interrupt) == Interrupt);
  }
  else
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  return status;
}
