/**
 * @file    Os_Tick.c
 * @author  Ali Rezapour
 * @date    2025-05-10
 * @version 0.1.0
 * @brief   HAL time base implementation based on hardware TIM6.
 *
 * @details
 * Detailed description (optional):
 * Overrides the native HAL time base functions (defined as weak)
 * using the TIM6 peripheral:
 *     - Initializes TIM6 to generate a Period Elapsed Event every 1ms
 *     - HAL_IncTick is called inside HAL_TIM_PeriodElapsedCallback,
 *       i.e. every 1ms
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.1.0   | 2025-05-10 | Ali Rezapour  | Initial release                      |
 * |---------|------------|---------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Mcal.h"

/* ─── Private Macros ──────────────────────────────────────────────────────── */
/* ─── Private Types ───────────────────────────────────────────────────────── */
/* ─── Private Variables ───────────────────────────────────────────────────── */

static TIM_HandleTypeDef        TimHandle;

/* ─── Private Function Prototypes ────────────────────────────────────────── */

void TIM6_DAC_IRQHandler(void);

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  This function configures the TIM6 as a time base source.
 * @note   The time source is configured to have 1ms time base with a
 *         dedicated Tick interrupt priority. This function is called
 *         automatically at the beginning of the program after reset by
 *         HAL_Init(), or at any time when the clock is configured by
 *         HAL_RCC_ClockConfig().
 * @param  TickPriority  Tick interrupt priority.
 * @return HAL status.
 */
HAL_StatusTypeDef HAL_InitTick (uint32 TickPriority)
{
  RCC_ClkInitTypeDef    clkconfig;
  uint32              uwTimclock, uwAPB1Prescaler;
  uint32              uwPrescalerValue;
  uint32              pFLatency;

  /* Configure the TIM6 IRQ priority */
  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
  {
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority ,0U);

    /* Enable the TIM6 global Interrupt */
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    uwTickPrio = TickPriority;
  }
  else
  {
    return HAL_ERROR;
  }

  /* Enable TIM6 clock */
  __HAL_RCC_TIM6_CLK_ENABLE();

  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

  /* Get APB1 prescaler */
  uwAPB1Prescaler = clkconfig.APB1CLKDivider;

  /* Compute TIM6 clock */
  if (uwAPB1Prescaler == RCC_HCLK_DIV1)
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
  }

  /* Compute the prescaler value to have TIM6 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32) ((uwTimclock / 1000000U) - 1U);

  /* Initialize TIM6 */
  TimHandle.Instance = TIM6;

  /* Initialize TIMx peripheral as follow:
     - Period = [(TIM6CLK/1000) - 1], to have a (1/1000) s time base.
     - Prescaler = (uwTimclock/1000000 - 1), to have a 1MHz counter clock.
     - ClockDivision = 0
     - Counter direction = Up
  */
  TimHandle.Init.Period = (1000000U / 1000U) - 1U;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&TimHandle) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    return HAL_TIM_Base_Start_IT(&TimHandle);
  }

  /* Return function status */
  return HAL_ERROR;
}

/**
 * @brief  Suspend Tick increment.
 * @note   Disables the tick increment by disabling the TIM6 update interrupt.
 * @return None.
 */
void HAL_SuspendTick(void)
{
  /* Disable TIM6 update Interrupt */
  __HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enables the tick increment by enabling the TIM6 update interrupt.
 * @return None.
 */
void HAL_ResumeTick(void)
{
  /* Enable TIM6 Update interrupt */
  __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/**
 * @brief  Period elapsed callback in non-blocking mode.
 * @note   Called when a TIM6 interrupt occurs, inside HAL_TIM_IRQHandler().
 *         Makes a direct call to HAL_IncTick() to increment the global
 *         variable "uwTick" used as the application time base.
 * @param  htim  TIM handle.
 * @return None.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  HAL_IncTick();
}

/* ─── Private Functions ───────────────────────────────────────────────────── */

/**
 * @brief  This function handles the TIM6 interrupt request.
 * @return None.
 */
void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}
