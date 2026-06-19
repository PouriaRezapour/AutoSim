/**
 * @file    Mcal_Config.h
 * @author  Ali Rezapour
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   HAL configuration file.
 *
 * @details
 * This header selects which STM32H7xx HAL peripheral modules are compiled in,
 * sets oscillator and system-level constants, and pulls in each module's
 * corresponding header when its enable guard is defined.
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.2.3   | 2025-06-07 | Ali Rezapour  | Reformatted to project template style|
 * |---------|------------|---------------|--------------------------------------|
 */

#ifndef MCAL_CONF_H
#define MCAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

/* ─── Module Selection ────────────────────────────────────────────────────── */
/**
 * @brief List of modules to be used in the HAL driver.
 *        Uncomment a line to enable the corresponding peripheral module.
 */
#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
/* #define HAL_CEC_MODULE_ENABLED    */
/* #define HAL_COMP_MODULE_ENABLED   */
#define HAL_CORTEX_MODULE_ENABLED
/* #define HAL_CRC_MODULE_ENABLED    */
/* #define HAL_CRYP_MODULE_ENABLED   */
/* #define HAL_DAC_MODULE_ENABLED    */
/* #define HAL_DCMI_MODULE_ENABLED   */
/* #define HAL_DFSDM_MODULE_ENABLED  */
#define HAL_DMA_MODULE_ENABLED
#define HAL_DMA2D_MODULE_ENABLED
#define HAL_ETH_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#define HAL_FDCAN_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
/* #define HAL_HASH_MODULE_ENABLED   */
/* #define HAL_HCD_MODULE_ENABLED    */
/* #define HAL_HRTIM_MODULE_ENABLED  */
/* #define HAL_HSEM_MODULE_ENABLED   */
#define HAL_I2C_MODULE_ENABLED
/* #define HAL_I2S_MODULE_ENABLED    */
/* #define HAL_IRDA_MODULE_ENABLED   */
/* #define HAL_IWDG_MODULE_ENABLED   */
/* #define HAL_JPEG_MODULE_ENABLED   */
/* #define HAL_LPTIM_MODULE_ENABLED  */
#define HAL_LTDC_MODULE_ENABLED
/* #define HAL_MDIOS_MODULE_ENABLED  */
#define HAL_MDMA_MODULE_ENABLED
/* #define HAL_MMC_MODULE_ENABLED    */
/* #define HAL_NAND_MODULE_ENABLED   */
/* #define HAL_NOR_MODULE_ENABLED    */
/* #define HAL_OPAMP_MODULE_ENABLED  */
/* #define HAL_PCD_MODULE_ENABLED    */
#define HAL_PWR_MODULE_ENABLED
/* #define HAL_QSPI_MODULE_ENABLED   */
/* #define HAL_RAMECC_MODULE_ENABLED */
#define HAL_RCC_MODULE_ENABLED
/* #define HAL_RNG_MODULE_ENABLED    */
/* #define HAL_RTC_MODULE_ENABLED    */
/* #define HAL_SAI_MODULE_ENABLED    */
/* #define HAL_SD_MODULE_ENABLED     */
#define HAL_SDRAM_MODULE_ENABLED
/* #define HAL_SMARTCARD_MODULE_ENABLED */
/* #define HAL_SMBUS_MODULE_ENABLED  */
/* #define HAL_SPDIFRX_MODULE_ENABLED */
/* #define HAL_SPI_MODULE_ENABLED    */
/* #define HAL_SRAM_MODULE_ENABLED   */
/* #define HAL_SWPMI_MODULE_ENABLED  */
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
/* #define HAL_USART_MODULE_ENABLED  */
/* #define HAL_WWDG_MODULE_ENABLED   */

/* ─── Oscillator Values ───────────────────────────────────────────────────── */
/**
 * @brief External High Speed oscillator (HSE) frequency in Hz.
 *        Used by RCC HAL to compute the system clock when HSE is the source.
 *        Override by defining HSE_VALUE in the toolchain preprocessor.
 */
#if !defined(HSE_VALUE)
  #define HSE_VALUE  ((uint32)8000000)
#endif

/**
 * @brief Timeout for HSE start-up, in ms.
 */
#if !defined(HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT  ((uint32)100)
#endif

/**
 * @brief Internal CSI oscillator default frequency in Hz (reset value).
 */
#if !defined(CSI_VALUE)
  #define CSI_VALUE  ((uint32)4000000)
#endif

/**
 * @brief Internal High Speed oscillator (HSI) frequency in Hz.
 *        Used by RCC HAL when HSI is the clock source.
 */
#if !defined(HSI_VALUE)
  #define HSI_VALUE  ((uint32)64000000)
#endif

/**
 * @brief External Low Speed oscillator (LSE) frequency in Hz.
 *        Used by UART and RTC HAL modules.
 */
#if !defined(LSE_VALUE)
  #define LSE_VALUE  ((uint32)32768)
#endif

/**
 * @brief Timeout for LSE start-up, in ms.
 */
#if !defined(LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT  ((uint32)5000)
#endif

/**
 * @brief Internal Low Speed oscillator (LSI) typical frequency in Hz.
 *        The real value may vary with voltage and temperature.
 */
#if !defined(LSI_VALUE)
  #define LSI_VALUE  ((uint32)32000)
#endif

/**
 * @brief External clock source for the I2S peripheral (I2S_CKIN pad), in Hz.
 */
#if !defined(EXTERNAL_CLOCK_VALUE)
  #define EXTERNAL_CLOCK_VALUE  12288000U
#endif

/* ─── System Configuration ────────────────────────────────────────────────── */
/**
 * @brief System-level HAL configuration constants.
 */
#define VDD_VALUE          3300UL           /*!< VDD supply voltage in mV          */
#define TICK_INT_PRIORITY  ((uint32)0x0F) /*!< SysTick interrupt priority        */
#define USE_RTOS           0                /*!< Set to 1 when an RTOS is present  */
#define USE_SD_TRANSCEIVER 1U               /*!< Enable uSD transceiver support    */

/* ─── Assert Configuration ────────────────────────────────────────────────── */
/**
 * @brief Uncomment USE_FULL_ASSERT to enable assert_param checks in HAL drivers.
 */
/* #define USE_FULL_ASSERT  1 */

/* ─── SPI Configuration ───────────────────────────────────────────────────── */
/**
 * @brief Controls CRC feature inside the HAL SPI driver.
 *        1U = CRC code compiled in; 0U = CRC code excluded.
 */
#define USE_SPI_CRC  1U

/* ─── Includes ────────────────────────────────────────────────────────────── */
/**
 * @brief Conditionally include each peripheral module's header.
 */

#ifdef HAL_RCC_MODULE_ENABLED
  #include "stm32h7xx_hal_rcc.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
  #include "stm32h7xx_hal_gpio.h"
#endif

#ifdef HAL_DMA_MODULE_ENABLED
  #include "stm32h7xx_hal_dma.h"
#endif

#ifdef HAL_MDMA_MODULE_ENABLED
  #include "stm32h7xx_hal_mdma.h"
#endif

#ifdef HAL_HASH_MODULE_ENABLED
  #include "stm32h7xx_hal_hash.h"
#endif

#ifdef HAL_DCMI_MODULE_ENABLED
  #include "stm32h7xx_hal_dcmi.h"
#endif

#ifdef HAL_DMA2D_MODULE_ENABLED
  #include "stm32h7xx_hal_dma2d.h"
#endif

#ifdef HAL_DFSDM_MODULE_ENABLED
  #include "stm32h7xx_hal_dfsdm.h"
#endif

#ifdef HAL_ETH_MODULE_ENABLED
  #include "stm32h7xx_hal_eth.h"
#endif

#ifdef HAL_EXTI_MODULE_ENABLED
  #include "stm32h7xx_hal_exti.h"
#endif

#ifdef HAL_CORTEX_MODULE_ENABLED
  #include "stm32h7xx_hal_cortex.h"
#endif

#ifdef HAL_ADC_MODULE_ENABLED
  #include "stm32h7xx_hal_adc.h"
#endif

#ifdef HAL_FDCAN_MODULE_ENABLED
  #include "stm32h7xx_hal_fdcan.h"
#endif

#ifdef HAL_CEC_MODULE_ENABLED
  #include "stm32h7xx_hal_cec.h"
#endif

#ifdef HAL_COMP_MODULE_ENABLED
  #include "stm32h7xx_hal_comp.h"
#endif

#ifdef HAL_CRC_MODULE_ENABLED
  #include "stm32h7xx_hal_crc.h"
#endif

#ifdef HAL_CRYP_MODULE_ENABLED
  #include "stm32h7xx_hal_cryp.h"
#endif

#ifdef HAL_DAC_MODULE_ENABLED
  #include "stm32h7xx_hal_dac.h"
#endif

#ifdef HAL_FLASH_MODULE_ENABLED
  #include "stm32h7xx_hal_flash.h"
#endif

#ifdef HAL_HRTIM_MODULE_ENABLED
  #include "stm32h7xx_hal_hrtim.h"
#endif

#ifdef HAL_HSEM_MODULE_ENABLED
  #include "stm32h7xx_hal_hsem.h"
#endif

#ifdef HAL_SRAM_MODULE_ENABLED
  #include "stm32h7xx_hal_sram.h"
#endif

#ifdef HAL_NOR_MODULE_ENABLED
  #include "stm32h7xx_hal_nor.h"
#endif

#ifdef HAL_NAND_MODULE_ENABLED
  #include "stm32h7xx_hal_nand.h"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
  #include "stm32h7xx_hal_i2c.h"
#endif

#ifdef HAL_I2S_MODULE_ENABLED
  #include "stm32h7xx_hal_i2s.h"
#endif

#ifdef HAL_IWDG_MODULE_ENABLED
  #include "stm32h7xx_hal_iwdg.h"
#endif

#ifdef HAL_JPEG_MODULE_ENABLED
  #include "stm32h7xx_hal_jpeg.h"
#endif

#ifdef HAL_MDIOS_MODULE_ENABLED
  #include "stm32h7xx_hal_mdios.h"
#endif

#ifdef HAL_MMC_MODULE_ENABLED
  #include "stm32h7xx_hal_mmc.h"
#endif

#ifdef HAL_LPTIM_MODULE_ENABLED
  #include "stm32h7xx_hal_lptim.h"
#endif

#ifdef HAL_LTDC_MODULE_ENABLED
  #include "stm32h7xx_hal_ltdc.h"
#endif

#ifdef HAL_OPAMP_MODULE_ENABLED
  #include "stm32h7xx_hal_opamp.h"
#endif

#ifdef HAL_PWR_MODULE_ENABLED
  #include "stm32h7xx_hal_pwr.h"
#endif

#ifdef HAL_QSPI_MODULE_ENABLED
  #include "stm32h7xx_hal_qspi.h"
#endif

#ifdef HAL_RAMECC_MODULE_ENABLED
  #include "stm32h7xx_hal_ramecc.h"
#endif

#ifdef HAL_RNG_MODULE_ENABLED
  #include "stm32h7xx_hal_rng.h"
#endif

#ifdef HAL_RTC_MODULE_ENABLED
  #include "stm32h7xx_hal_rtc.h"
#endif

#ifdef HAL_SAI_MODULE_ENABLED
  #include "stm32h7xx_hal_sai.h"
#endif

#ifdef HAL_SD_MODULE_ENABLED
  #include "stm32h7xx_hal_sd.h"
#endif

#ifdef HAL_SDRAM_MODULE_ENABLED
  #include "stm32h7xx_hal_sdram.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
  #include "stm32h7xx_hal_spi.h"
#endif

#ifdef HAL_SPDIFRX_MODULE_ENABLED
  #include "stm32h7xx_hal_spdifrx.h"
#endif

#ifdef HAL_SWPMI_MODULE_ENABLED
  #include "stm32h7xx_hal_swpmi.h"
#endif

#ifdef HAL_TIM_MODULE_ENABLED
  #include "stm32h7xx_hal_tim.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
  #include "stm32h7xx_hal_uart.h"
#endif

#ifdef HAL_USART_MODULE_ENABLED
  #include "stm32h7xx_hal_usart.h"
#endif

#ifdef HAL_IRDA_MODULE_ENABLED
  #include "stm32h7xx_hal_irda.h"
#endif

#ifdef HAL_SMARTCARD_MODULE_ENABLED
  #include "stm32h7xx_hal_smartcard.h"
#endif

#ifdef HAL_SMBUS_MODULE_ENABLED
  #include "stm32h7xx_hal_smbus.h"
#endif

#ifdef HAL_WWDG_MODULE_ENABLED
  #include "stm32h7xx_hal_wwdg.h"
#endif

#ifdef HAL_PCD_MODULE_ENABLED
  #include "stm32h7xx_hal_pcd.h"
#endif

#ifdef HAL_HCD_MODULE_ENABLED
  #include "stm32h7xx_hal_hcd.h"
#endif

/* ─── Assert Macro ────────────────────────────────────────────────────────── */
/**
 * @brief  assert_param checks a function argument expression.
 *         When USE_FULL_ASSERT is defined and @p expr evaluates to false,
 *         assert_failed() is called with the source file name and line number.
 *         When USE_FULL_ASSERT is not defined the macro compiles away to nothing.
 * @param  expr  Expression to evaluate.
 */
#ifdef USE_FULL_ASSERT
  #define assert_param(expr) \
    ((expr) ? (void)0U : assert_failed((uint8 *)__FILE__, __LINE__))
  void assert_failed(uint8 *file, uint32 line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* MCAL_CONF_H*/
