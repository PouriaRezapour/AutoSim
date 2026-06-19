/**
 * @file    Mcu.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-05-18
 * @version 0.1.2
 * @brief   System initialisation implementation (clocks, MPU, cache, BSP).
 *
 * @details
 * Implements the full hardware bring-up sequence for the STM32H7 Nucleo board:
 * MPU region configuration, L1 I/D-cache enable, system clock setup
 * (PLL1 @ 400 MHz from 8 MHz HSE bypass), peripheral clock setup (PLL2 for
 * SPI1/2/3, PLL3 for SPI4/5), and BSP LED initialisation. All steps are
 * orchestrated by the top-level Mpu_Init() function, which must be called
 * before any other BSW or application module.
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

#include "Mcu.h"
#include "Mcal.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Configure the system clock.
 *
 * @details
 * Clock tree summary:
 * |-----------------------|---------------------------|
 * | Parameter             | Value                     |
 * |-----------------------|---------------------------|
 * | Source                | PLL1 (HSE bypass, 8 MHz)  |
 * | SYSCLK                | 400 MHz                   |
 * | HCLK (AXI/AHB)        | 200 MHz  (AHB div 2)      |
 * | D1/D2/D3 APB clocks   | 100 MHz  (APBx div 2)     |
 * | PLL1 M/N/P/Q/R        | 1 / 100 / 2 / 16 / 2      |
 * | Flash latency         | 4 WS                      |
 * | VDD                   | 3.3 V                     |
 * |-----------------------|---------------------------|
 */
void Mpu_ClkCfg(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    HAL_StatusTypeDef  ret               = HAL_OK;

    /* Configure power supply — LDO mode for Nucleo board */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /* Voltage scaling for 400 MHz operation */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /* Enable D2 SRAM3 clock */
    __HAL_RCC_D2SRAM3_CLK_ENABLE();

    /* Enable HSE and wait for stabilisation */
    __HAL_RCC_HSE_CONFIG(RCC_HSE_BYPASS);

    uint32 hse_timeout = 0x5000;
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
    {
        if (--hse_timeout == 0)
        {
            /* HSE failed — hang for debugging */
            while (1) {}
        }
    }

    /* Configure PLL1 with HSE as source */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState            = RCC_HSE_BYPASS;
    RCC_OscInitStruct.HSIState            = RCC_HSI_OFF;
    RCC_OscInitStruct.CSIState            = RCC_CSI_OFF;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM            = 1;
    RCC_OscInitStruct.PLL.PLLN            = 100;
    RCC_OscInitStruct.PLL.PLLP            = 2;
    RCC_OscInitStruct.PLL.PLLQ            = 16;
    RCC_OscInitStruct.PLL.PLLR            = 2;
    RCC_OscInitStruct.PLL.PLLRGE         = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL      = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN        = 0;

    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK) { while (1); }

    /* Bus clocks: SYSCLK=400 MHz, AHB=200 MHz, APBx=100 MHz */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK    | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1   | RCC_CLOCKTYPE_PCLK2
                                     | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
    if (ret != HAL_OK) { while (1); }
}

/**
 * @brief  Configure peripheral clocks (PLL2 for SPI1/2/3, PLL3 for SPI4/5).
 *
 * @details
 * PLL2 outputs (source: HSE 8 MHz, M=1, N=100):
 *   P=4 → 200 MHz,  Q=16 → 50 MHz,  R=2 → 400 MHz
 *
 * PLL3 outputs (source: HSE 8 MHz, M=1, N=60):
 *   P=2 → 240 MHz,  Q=4  → 120 MHz, R=2 → 240 MHz
 */
void Mpu_PeriphClkCfg(void)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    HAL_StatusTypeDef        ret                 = HAL_OK;

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI1 | RCC_PERIPHCLK_SPI2
                                             | RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_SPI4
                                             | RCC_PERIPHCLK_SPI5;

    PeriphClkInitStruct.PLL2.PLL2M      = 1;
    PeriphClkInitStruct.PLL2.PLL2N      = 100;
    PeriphClkInitStruct.PLL2.PLL2P      = 4;
    PeriphClkInitStruct.PLL2.PLL2Q      = 16;
    PeriphClkInitStruct.PLL2.PLL2R      = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE    = RCC_PLL2VCIRANGE_3;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN  = 0;

    PeriphClkInitStruct.PLL3.PLL3M      = 1;
    PeriphClkInitStruct.PLL3.PLL3N      = 60;
    PeriphClkInitStruct.PLL3.PLL3P      = 2;
    PeriphClkInitStruct.PLL3.PLL3Q      = 4;
    PeriphClkInitStruct.PLL3.PLL3R      = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE    = RCC_PLL3VCIRANGE_3;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN  = 0;

    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
    PeriphClkInitStruct.Spi45ClockSelection  = RCC_SPI45CLKSOURCE_PLL3;

    ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if (ret != HAL_OK) { while (1); }
}

/**
 * @brief  Initialise BSP peripherals (LEDs).
 */
void Mpu_BspCfg(void)
{
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
}

/**
 * @brief  Configure MPU regions for the application memory map.
 *
 * @details
 * Region 0 (0x00000000, 4 GB): strongly-ordered, no access — catches
 * unintended accesses to unmapped memory. Region 1 (0x30000000, 1 KB):
 * device, non-cacheable, bufferable — Ethernet DMA descriptors. Region 2
 * (0x30004000, 16 KB): normal, non-cacheable, shareable — lwIP RAM heap.
 */
void Mcu_MpuCfg(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    HAL_MPU_Disable();

    /* Region 0: catch-all — strongly ordered, no access */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x00;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Region 1: ETH DMA descriptors — device, non-cacheable, bufferable */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x30000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_1KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Region 2: lwIP RAM heap — normal, non-cacheable, shareable */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress      = 0x30004000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_16KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  Enable CPU L1 instruction and data caches.
 */
void Mcu_CpuCacheEnable(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
}

/**
 * @brief  Top-level system initialisation sequence.
 */
void Mpu_Init(void)
{
    Mcu_MpuCfg();
    Mcu_CpuCacheEnable();
    Mpu_ClkCfg();
    Mpu_PeriphClkCfg();
    Mpu_BspCfg();
}
