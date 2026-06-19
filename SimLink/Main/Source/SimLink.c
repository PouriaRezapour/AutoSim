/**
 * @file    SimLink.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-05-18
 * @version 0.2.0
 * @brief   SimLink application layer implementation.
 *
 * @details
 * Implements the top-level SimLink application entry points. The init function
 * sequences the full hardware and software stack initialisation — MCAL (HAL),
 * BSW (system clock, port/GPIO, DIO hardware abstraction, CAN), and application
 * layer (PC Communication Manager). The run function hands control to the OS
 * scheduler and never returns.
 *
 * @par Revision History:
 * |---------|------------|------------------|-------------------------------------|
 * | Version | Date       | Author           | Description                         |
 * |---------|------------|------------------|-------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                     |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template           |
 * | 0.2.0   | 2025-05-23 | A.Rezapour       | Added Port_Init, Can_Init, Can_Start|
 * |---------|------------|------------------|-------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "SimLink.h"

#include "Mcu.h"
#include "Port.h"
#include "Dio_HwAb.h"
#include "Can.h"
#include "Os.h"

#include "PcComMgr.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

void SimLink_Init(void)
{
    /* __Mcal__ */
    HAL_Init();

    /* __Bsw__ */
    Mpu_Init();        /* Clocks, MPU, cache, BSP LEDs             */
    Port_Init();       /* GPIO pin-mux for all peripherals         */
    Dio_HwAb_Init();   /* Digital I/O hardware abstraction         */
    Can_Init();        /* FDCAN1 peripheral configuration          */
    Can_Start();       /* Apply Rx filters and start the bus       */

    /* __App__ */
    PcComMgr_Init();
}

void SimLink_Run(void)
{
    /* Create application startup tasks */
    Os_Run();

    while (1);
}
