/**
 * @file    Dio.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Generic Digital I/O (DIO) MCAL implementation.
 *
 * @details
 * Provides thin HAL wrappers for GPIO clock enabling, pin write, pin read,
 * and pin toggle operations. This layer is the sole point of contact with
 * STM32 HAL GPIO functions for digital I/O and must not be called directly
 * by application-layer modules — use Dio_HwAb instead.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------|
 * | Version | Date       | Author           | Description                    |
 * |---------|------------|------------------|--------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the        |
 * |         |            |                  | project's standard types       |
 * |         |            |                  | defined in Std_Types.h.        |
 * |---------|------------|------------------|--------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Dio.h"
#include "Std_Types.h"

#include <stdbool.h>

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Dio_Init(void)
{
    /* Enable all necessary GPIO clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
}

void Dio_WritePin(GPIO_TypeDef *port, uint16 pin, boolean state)
{
    HAL_GPIO_WritePin(port, pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

boolean Dio_ReadPin(GPIO_TypeDef *port, uint16 pin)
{
    return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET);
}

void Dio_TogglePin(GPIO_TypeDef *port, uint16 pin)
{
    HAL_GPIO_TogglePin(port, pin);
}
