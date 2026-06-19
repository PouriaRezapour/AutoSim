/**
 * @file    main.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-05-18
 * @version 0.1.2
 * @brief   Application entry point.
 *
 * @details
 * Top-level entry point for the SimLink firmware. Delegates hardware
 * bring-up and scheduler startup entirely to the SimLink application layer.
 * SimLink_Run() hands control to the RTOS kernel and does not return.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------|
 * | Version | Date       | Author           | Description                    |
 * |---------|------------|------------------|--------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template      |
 * |---------|------------|------------------|--------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "main.h"
#include "SimLink.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Main program entry point.
 * @return This function does not return.
 */
int main(void)
{
    /* Initialize the project */
    SimLink_Init();

    /* Run the project — does not return */
    SimLink_Run();
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8 *file, uint32 line)
{
    (void)file;
    (void)line;
    while (1) {}
}
#endif
