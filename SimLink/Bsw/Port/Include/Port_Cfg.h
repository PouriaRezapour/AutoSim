/**
 * @file    Port_Cfg.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   AUTOSAR Port Driver – Static Configuration Header.
 *
 * @details
 * Contains all pre-processor feature switches and the port/pin count macros
 * consumed by Port_Init().  In a tool-generated AUTOSAR environment this
 * file would be produced by a configuration tool; here it is hand-crafted
 * to match the project's STM32H753 hardware.
 *
 * Architecture note
 * -----------------
 * Port_PinConfigType, Port_ConfigType, Port_PinType and
 * Port_PinDirectionType are intentionally defined in Port.h, NOT here.
 * Placing them here would create a circular inclusion:
 *
 *   Port_Cfg.h  ──includes──►  Port.h  ──includes──►  Port_Cfg.h  (cycle!)
 *
 * Port_Cfg.h therefore only contains items the compiler can process
 * before Port.h has been seen:
 *   • Mcal.h / Std_Types.h inclusions (no Port types needed)
 *   • Version macros checked by Port.h
 *   • Feature-switch macros (STD_ON / STD_OFF)
 *   • Pure integer / port-count macros
 *   • Initial-level macros (expand to STD_HIGH / STD_LOW, not Port types)
 *
 * All items that require Port_PinType or Port_PinDirectionType live in
 * Port.h and are available to callers after including Port.h.
 *
 * Pin configuration currently defined:
 *   - FDCAN1 : PD0 (RX), PD1 (TX) – AF9, no pull, low speed
 *
 * Adding a new peripheral
 * -----------------------
 * 1. Add a Port_PinConfigType entry to the array in Port_Cfg.c.
 * 2. Add the corresponding symbolic-name macro in Port.h.
 * 3. Add the peripheral GPIO / AF macros in Port.h.
 *
 * AUTOSAR CP R24-11
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour | Initial AUTOSAR-compliant release    |
 * |---------|------------|------------|--------------------------------------|
 */

#ifndef PORT_CFG_H
#define PORT_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*
 * INCLUDES
 *===========================================================================*/

#include "Mcal.h"       /* STM32 HAL/LL – GPIO_TypeDef, HAL_GPIO_Init, … */
#include "Std_Types.h"  /* AUTOSAR standard types                          */

/*===========================================================================*
 * CFG-FILE VERSION  (checked at compile time by Port.h)
 *===========================================================================*/

#define PORT_CFG_AR_MAJOR_VERSION   (4U)
#define PORT_CFG_AR_MINOR_VERSION   (7U)

/*===========================================================================*
 * FEATURE SWITCHES  [ECUC_Port_00117 / PortGeneral]
 *===========================================================================*/

/**
 * @brief   Enable / disable the Port_SetPinDirection() API.
 * @details [SWS_Port_00086][ECUC_Port_00131] PortSetPinDirectionApi.
 *          Set to STD_ON to compile Port_SetPinDirection() into the build.
 */
#define PORT_SET_PIN_DIRECTION_API      STD_ON

/**
 * @brief   Enable / disable the Port_SetPinMode() API.
 * @details [SWS_Port_00223][ECUC_Port_00132] PortSetPinModeApi.
 */
#define PORT_SET_PIN_MODE_API           STD_ON

/**
 * @brief   Enable / disable the Port_GetVersionInfo() API.
 * @details [ECUC_Port_00133] PortVersionInfoApi.
 */
#define PORT_VERSION_INFO_API           STD_ON

/**
 * @brief   Enable / disable development error detection via DET.
 * @details [ECUC_Port_00123] PortDevErrorDetect.
 *          When STD_ON, invalid parameters are reported to Det_ReportError()
 *          and the offending function returns without effect.
 */
#define PORT_DEV_ERROR_DETECT           STD_ON

/*===========================================================================*
 * PORT / PIN COUNTS
 *===========================================================================*/

/** @brief Total number of GPIO ports on STM32H753 (A…K = 11). */
#define PORT_NUM_PORTS                  (11U)

/** @brief Number of pins in each GPIO port on STM32H753. */
#define PORT_PINS_PER_PORT              (16U)

/*
 * NOTE: PORT_MAX_PIN_ID requires Port_PinType, which is defined in Port.h.
 *       It is therefore defined in Port.h after Port_PinType is declared,
 *       not here, to avoid the circular dependency.
 */

/*===========================================================================*
 * INITIAL LEVEL VALUES  [SWS_Port_00055]
 *===========================================================================*/

/** @brief PortPinLevelValue – pin driven HIGH at startup. */
#define PORT_PIN_LEVEL_HIGH             (STD_HIGH)

/** @brief PortPinLevelValue – pin driven LOW at startup (default / safe). */
#define PORT_PIN_LEVEL_LOW              (STD_LOW)

#ifdef __cplusplus
}
#endif

#endif /* PORT_CFG_H */
