/**
 * @file    Dio_HwAb.h
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Digital I/O hardware abstraction layer.
 *
 * @details
 * Provides logical channel abstraction for digital I/O pins including
 * user buttons, motor status signals, valve control signals, and command
 * outputs. Maps application-level channel names to physical GPIO pins
 * with optional debouncing support.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                      |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Template standardization             |
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | The three on-board controller LEDs   |
 * |         |            |                  | are now listed                       |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the project's    |
 * |         |            |                  | standard types defined in            |
 * |         |            |                  | Std_Types.h.                         |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef DIO_HWAB_H
#define DIO_HWAB_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Mcal.h"
#include "Std_Types.h"

#include <stdbool.h>
#include <stdint.h>

/* ─── Module Info ─────────────────────────────────────────────────────────── */

#define DIO_HWAB_VENDOR_ID      0xFFFFu   /**< Project vendor identifier       */
#define DIO_HWAB_MODULE_ID      120u      /**< AUTOSAR Dio module identifier   */

/* ─── Version Info ────────────────────────────────────────────────────────── */

#define DIO_HWAB_MAJOR_VERSION  0x00u     /**< Software major version          */
#define DIO_HWAB_MINOR_VERSION  0x02u     /**< Software minor version          */
#define DIO_HWAB_PATCH_VERSION  0x03u     /**< Software patch version          */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/**
 * @brief Pin configuration structure.
 */
typedef struct
{
    GPIO_TypeDef *port;         /**< GPIO port pointer */
    uint16 pin;               /**< GPIO pin number */
    boolean isOutput;              /**< Pin direction (true = output) */
    boolean debounceStatus;        /**< Debounce state for inputs */
} Dio_PinConfig;

/**
 * @brief Logical DIO channel enumeration.
 */
typedef enum
{
    /* ─── Inputs ─── */
    
    /* HMI */
    HWAB_DIO_BLUE_USER_BUTTON,

    /* Motor */
    HWAB_DIO_MOTOR_STATUS,

    /* Valves */
    HWAB_DIO_SIGNAL_VALVE_ON,
    HWAB_DIO_SIGNAL_VALVE1,
    HWAB_DIO_SIGNAL_VALVE2,
    HWAB_DIO_SIGNAL_VALVE3,
    HWAB_DIO_SIGNAL_VALVE4,
    HWAB_DIO_SIGNAL_VALVE5,
    HWAB_DIO_SIGNAL_VALVE6,
    HWAB_DIO_SIGNAL_VALVE7,
    HWAB_DIO_SIGNAL_VALVE8,
    HWAB_DIO_SIGNAL_VALVE9,
    HWAB_DIO_SIGNAL_VALVE10,
    HWAB_DIO_SIGNAL_VALVE11,
    HWAB_DIO_SIGNAL_VALVE12,

    /* ─── Outputs ─── */
    
    /* Command Signals */
    HWAB_DIO_CMD_VESC,
    HWAB_DIO_CMD_BLS,
    HWAB_DIO_CMD_ITPMS,
    HWAB_DIO_CMD_PARK_BRAKE,
    HWAB_DIO_CMD_ESC_TCS,
    HWAB_DIO_CMD_WAKEUP,
    HWAB_DIO_CMD_VMOT,

    /* Controller LEDs */
    HWAB_DIO_LED1,
    HWAB_DIO_LED2,
    HWAB_DIO_LED3,

    HWAB_DIO_CHANNEL_COUNT      /**< Total channel count */
} Dio_HwAb_ChannelType;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialize the DIO hardware abstraction layer.
 */
void Dio_HwAb_Init(void);

/**
 * @brief  Read the state of a logical DIO channel.
 * @param  ch  Logical channel identifier.
 * @return true if channel is HIGH, false if LOW.
 */
boolean Dio_HwAb_Read(Dio_HwAb_ChannelType ch);

/**
 * @brief  Write a state to a logical DIO channel.
 * @param  ch     Logical channel identifier.
 * @param  state  Desired state (true = HIGH, false = LOW).
 */
void Dio_HwAb_Write(Dio_HwAb_ChannelType ch, boolean state);

/**
 * @brief  Toggle the state of a logical DIO channel.
 * @param  ch  Logical channel identifier.
 */
void Dio_HwAb_Toggle(Dio_HwAb_ChannelType ch);

#endif /* DIO_HWAB_H */
