/**
 * @file    Mcu_Sys.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-05-10
 * @version 0.1.0
 * @brief   CMSIS Cortex-M Device Peripheral Access Layer System Source File.
 *
 * @details
 * Detailed description (optional):
 * This file provides two functions and one global variable to be called
 * from the user application:
 *     - SystemInit(): Called at startup just after reset and before the
 *       branch to the main program. This call is made inside the
 *       startup assembly file.
 *     - SystemCoreClock variable: Contains the core clock (HCLK), it can
 *       be used by the user application to set up the SysTick timer or
 *       configure other parameters.
 *     - SystemCoreClockUpdate(): Updates the SystemCoreClock variable and
 *       must be called whenever the core clock is changed during program
 *       execution.
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.1.0   | 2025-05-10 | A.Rezapour   | Initial release                    |
 * |---------|------------|------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "stm32h7xx.h"
#include <math.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32)8000000) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (CSI_VALUE)
  #define CSI_VALUE    ((uint32)4000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* CSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32)64000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use initialized data in D2 domain SRAM  */
/* #define DATA_IN_D2_SRAM */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00000000UL        /*!< Vector Table base offset field.
                                      This value must be a multiple of 0x200. */
/******************************************************************************/

/* ─── Private Types ───────────────────────────────────────────────────────── */
/* ─── Private Variables ───────────────────────────────────────────────────── */

/* This variable is updated in three ways:
   1) by calling CMSIS function SystemCoreClockUpdate()
   2) by calling HAL API function HAL_RCC_GetHCLKFreq()
   3) each time HAL_RCC_ClockConfig() is called to configure the system
      clock frequency
      Note: If you use this function to configure the system clock; then
            there is no need to call the 2 first functions listed above,
            since the SystemCoreClock variable is updated automatically.
*/
uint32 SystemCoreClock = 64000000;
uint32 SystemD2Clock = 64000000;
const  uint8 D1CorePrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/* ─── Private Function Prototypes ────────────────────────────────────────── */
/* ─── Public Functions ────────────────────────────────────────────────────── */

/**
 * @brief  Set up the microcontroller system.
 * @note   Initializes the FPU setting and the vector table location.
 * @return None.
 */
void SystemInit (void)
{
#if defined (DATA_IN_D2_SRAM)
 __IO uint32 tmpreg;
#endif /* DATA_IN_D2_SRAM */

  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= RCC_CR_HSION;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON , CSION,RC48ON, CSIKERON PLL1ON, PLL2ON and PLL3ON bits */
  RCC->CR &= 0xEAF6ED7FU;

  /* Reset D1CFGR register */
  RCC->D1CFGR = 0x00000000;

  /* Reset D2CFGR register */
  RCC->D2CFGR = 0x00000000;

  /* Reset D3CFGR register */
  RCC->D3CFGR = 0x00000000;

  /* Reset PLLCKSELR register */
  RCC->PLLCKSELR = 0x00000000;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00000000;
  /* Reset PLL1DIVR register */
  RCC->PLL1DIVR = 0x00000000;
  /* Reset PLL1FRACR register */
  RCC->PLL1FRACR = 0x00000000;

  /* Reset PLL2DIVR register */
  RCC->PLL2DIVR = 0x00000000;

  /* Reset PLL2FRACR register */

  RCC->PLL2FRACR = 0x00000000;
  /* Reset PLL3DIVR register */
  RCC->PLL3DIVR = 0x00000000;

  /* Reset PLL3FRACR register */
  RCC->PLL3FRACR = 0x00000000;

  /* Reset HSEBYP bit */
  RCC->CR &= 0xFFFBFFFFU;

  /* Disable all interrupts */
  RCC->CIER = 0x00000000;

  /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
  if((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
  {
    /* if stm32h7 revY*/
    /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
    *((__IO uint32*)0x51008108) = 0x000000001U;
  }

#if defined (DATA_IN_D2_SRAM)
  /* in case of initialized data in D2 SRAM , enable the D2 SRAM clock */
  RCC->AHB2ENR |= (RCC_AHB2ENR_D2SRAM1EN | RCC_AHB2ENR_D2SRAM2EN | RCC_AHB2ENR_D2SRAM3EN);
  tmpreg = RCC->AHB2ENR;
  (void) tmpreg;
#endif /* DATA_IN_D2_SRAM */


/*
   * Disable the FMC bank1 (enabled after reset).
   * This, prevents CPU speculation access on this bank which blocks the use of FMC during
   * 24us. During this time the others FMC master (such as LTDC) cannot use it!
   */
  FMC_Bank1_R->BTCR[0] = 0x000030D2;

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  SCB->VTOR = D1_AXISRAM_BASE  | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal SRAM */
#else
  SCB->VTOR = FLASH_BANK1_BASE | VECT_TAB_OFFSET;       /* Vector Table Relocation in Internal FLASH */
#endif


}

/**
 * @brief  Update the SystemCoreClock variable according to the clock
 *         register values.
 * @note   The SystemCoreClock variable contains the core clock and can be
 *         used by the user application to set up the SysTick timer or
 *         configure other parameters. Each time the core clock changes,
 *         this function must be called to update the SystemCoreClock
 *         variable value; otherwise, any configuration based on this
 *         variable will be incorrect.
 *
 *         The system frequency computed by this function is not the real
 *         frequency in the chip. It is calculated based on the predefined
 *         constant and the selected clock source:
 *             - If SYSCLK source is CSI, SystemCoreClock will contain the
 *               CSI_VALUE(*)
 *             - If SYSCLK source is HSI, SystemCoreClock will contain the
 *               HSI_VALUE(**)
 *             - If SYSCLK source is HSE, SystemCoreClock will contain the
 *               HSE_VALUE(***)
 *             - If SYSCLK source is PLL, SystemCoreClock will contain the
 *               CSI_VALUE(*), HSI_VALUE(**) or HSE_VALUE(***) multiplied or
 *               divided by the PLL factors.
 *
 *         (*)   CSI_VALUE is a constant defined in stm32h7xx_hal.h (default
 *               value 4 MHz), but the real value may vary depending on
 *               variations in voltage and temperature.
 *         (**)  HSI_VALUE is a constant defined in stm32h7xx_hal.h (default
 *               value 64 MHz), but the real value may vary depending on
 *               variations in voltage and temperature.
 *         (***) HSE_VALUE is a constant defined in stm32h7xx_hal.h (default
 *               value 25 MHz); the user must ensure that HSE_VALUE matches
 *               the real frequency of the crystal used, otherwise this
 *               function may produce an incorrect result.
 *
 *         The result of this function may not be correct when using a
 *         fractional value for the HSE crystal.
 * @return None.
 */
void SystemCoreClockUpdate (void)
{
  uint32 pllp, pllsource, pllm, pllfracen, hsivalue, tmp;
  float_t fracn1, pllvco;

  /* Get SYSCLK source -------------------------------------------------------*/

  switch (RCC->CFGR & RCC_CFGR_SWS)
  {
  case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
   SystemCoreClock = (uint32) (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3));

    break;

  case RCC_CFGR_SWS_CSI:  /* CSI used as system clock  source */
    SystemCoreClock = CSI_VALUE;
    break;

  case RCC_CFGR_SWS_HSE:  /* HSE used as system clock  source */
    SystemCoreClock = HSE_VALUE;
    break;

  case RCC_CFGR_SWS_PLL1:  /* PLL1 used as system clock  source */

    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN
    SYSCLK = PLL_VCO / PLLR
    */
    pllsource = (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC);
    pllm = ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1)>> 4)  ;
    pllfracen = ((RCC->PLLCFGR & RCC_PLLCFGR_PLL1FRACEN)>>RCC_PLLCFGR_PLL1FRACEN_Pos);
    fracn1 = (float_t)(uint32)(pllfracen* ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACN1)>> 3));

    if (pllm != 0U)
    {
      switch (pllsource)
      {
        case RCC_PLLCKSELR_PLLSRC_HSI:  /* HSI used as PLL clock source */

        hsivalue = (HSI_VALUE >> ((RCC->CR & RCC_CR_HSIDIV)>> 3)) ;
        pllvco = ( (float_t)hsivalue / (float_t)pllm) * ((float_t)(uint32)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );

        break;

        case RCC_PLLCKSELR_PLLSRC_CSI:  /* CSI used as PLL clock source */
          pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
        break;

        case RCC_PLLCKSELR_PLLSRC_HSE:  /* HSE used as PLL clock source */
          pllvco = ((float_t)HSE_VALUE / (float_t)pllm) * ((float_t)(uint32)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
        break;

      default:
          pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1/(float_t)0x2000) +(float_t)1 );
        break;
      }
      pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_P1) >>9) + 1U ) ;
      SystemCoreClock =  (uint32)(float_t)(pllvco/(float_t)pllp);
    }
    else
    {
      SystemCoreClock = 0U;
    }
    break;

  default:
    SystemCoreClock = CSI_VALUE;
    break;
  }

  /* Compute SystemClock frequency --------------------------------------------------*/

  tmp = D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_D1CPRE)>> RCC_D1CFGR_D1CPRE_Pos];
  /* SystemCoreClock frequency : CM7 CPU frequency  */
  SystemCoreClock >>= tmp;

  /* SystemD2Clock frequency : AXI and AHBs Clock frequency  */
  SystemD2Clock = (SystemCoreClock >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE)>> RCC_D1CFGR_HPRE_Pos]) & 0x1FU));
}

/**
 * @brief  Exit Run* mode and configure the system power supply.
 * @note   This function exits Run* mode and configures the system power
 *         supply according to the definition used at compilation
 *         preprocessing level. The application shall set one of the
 *         following configuration options:
 *             - PWR_LDO_SUPPLY
 *             - PWR_DIRECT_SMPS_SUPPLY
 *             - PWR_EXTERNAL_SOURCE_SUPPLY
 *             - PWR_SMPS_1V8_SUPPLIES_LDO
 *             - PWR_SMPS_2V5_SUPPLIES_LDO
 *             - PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO
 *             - PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO
 *             - PWR_SMPS_1V8_SUPPLIES_EXT
 *             - PWR_SMPS_2V5_SUPPLIES_EXT
 *
 *         This function modifies the PWR->CR3 register to enable or disable
 *         specific power supply modes and waits until the voltage level
 *         flag is set, indicating that the power supply configuration is
 *         stable.
 * @return None.
 */
void ExitRun0Mode(void)
{
#if defined(USE_PWR_LDO_SUPPLY)
  #if defined(SMPS)
    /* Exit Run* mode by disabling SMPS and enabling LDO */
    PWR->CR3 = (PWR->CR3 & ~PWR_CR3_SMPSEN) | PWR_CR3_LDOEN;
  #else
    /* Enable LDO mode */
    PWR->CR3 |= PWR_CR3_LDOEN;
  #endif /* SMPS */
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_EXTERNAL_SOURCE_SUPPLY)
  #if defined(SMPS)
    /* Exit Run* mode */
    PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_SMPSEN | PWR_CR3_LDOEN)) | PWR_CR3_BYPASS;
  #else
    PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_LDOEN)) | PWR_CR3_BYPASS;
  #endif /* SMPS */
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_DIRECT_SMPS_SUPPLY) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 &= ~(PWR_CR3_LDOEN);
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_1V8_SUPPLIES_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_2V5_SUPPLIES_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 |= PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_1V8_SUPPLIES_EXT) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_LDOEN)) | PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#elif defined(USE_PWR_SMPS_2V5_SUPPLIES_EXT) && defined(SMPS)
  /* Exit Run* mode */
  PWR->CR3 = (PWR->CR3 & ~(PWR_CR3_LDOEN)) | PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS;
  /* Wait till voltage level flag is set */
  while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) == 0U)
  {}
#else
  /* No system power supply configuration is selected at exit Run* mode */
#endif /* USE_PWR_LDO_SUPPLY */
}
