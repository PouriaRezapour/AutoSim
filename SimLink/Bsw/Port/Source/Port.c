/**
 * @file    Port.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Implementation of the Port / GPIO initialisation module.
 *
 * @details
 * Provides Port_Init() and Port_DeInit() which configure every GPIO pin
 * used by the project.  The file is structured so that adding a new
 * peripheral requires only a new private helper (e.g. Port_InitSpi1_()) and
 * a single call to it inside Port_Init().
 *
 * Currently configured peripherals:
 *   - FDCAN1  :  PD0 (RX), PD1 (TX)  –  AF9
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.2.0   | 2025-05-23 | A.Rezapour | Initial release                      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour | The module now uses the project's    |
 * |         |            |            | standard types defined in            |
 * |         |            |            | Std_Types.h.                         |
 * |---------|------------|------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Port.h"
#include "Std_Types.h"

#include <stdint.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define LOG(fmt, ...)  /* Swap for your project logger if available */

/* ─── Private Types ───────────────────────────────────────────────────────── */

typedef struct {
    int isInitialised;
} PortState_t;

/* ─── Private Variables ───────────────────────────────────────────────────── */

static PortState_t s_portState = { .isInitialised = 0 };

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static void Port_InitFdCan1(void);
static void Port_DeInitFdCan1(void);

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Initialise all GPIO pins required by the project.
 */
PortStatus_t Port_Init(void)
{
    if (s_portState.isInitialised)
    {
        return PORT_OK;
    }

    /* Enable GPIO clocks required across the project */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* Configure FDCAN1 pins */
    Port_InitFdCan1();

    /* TODO: add further peripheral initialisers here, e.g. Port_InitSpi1_() */

    s_portState.isInitialised = 1;
    return PORT_OK;
}

/**
 * @brief  De-initialise all GPIO pins managed by this module.
 */
PortStatus_t Port_DeInit(void)
{
    if (!s_portState.isInitialised)
    {
        return PORT_OK;
    }

    Port_DeInitFdCan1();

    /* TODO: add further peripheral de-initialisers here */

    s_portState.isInitialised = 0;
    return PORT_OK;
}

/* ─── Private Functions ───────────────────────────────────────────────────── */

/**
 * @brief  Configure PD0 and PD1 for FDCAN1 RX and TX (AF9).
 */
static void Port_InitFdCan1(void)
{
    GPIO_InitTypeDef gpioInit = {0};

    /**
     * FDCAN1 GPIO configuration:
     *   PD0  ──►  FDCAN1_RX
     *   PD1  ──►  FDCAN1_TX
     */
    gpioInit.Pin       = PORT_FDCAN1_RX_PIN | PORT_FDCAN1_TX_PIN;
    gpioInit.Mode      = GPIO_MODE_AF_PP;
    gpioInit.Pull      = GPIO_NOPULL;
    gpioInit.Speed     = GPIO_SPEED_FREQ_LOW;
    gpioInit.Alternate = PORT_FDCAN1_AF;
    HAL_GPIO_Init(PORT_FDCAN1_GPIO_PORT, &gpioInit);
}

/**
 * @brief  Release PD0 and PD1 back to reset state.
 */
static void Port_DeInitFdCan1(void)
{
    HAL_GPIO_DeInit(PORT_FDCAN1_GPIO_PORT,
                    PORT_FDCAN1_RX_PIN | PORT_FDCAN1_TX_PIN);
}
