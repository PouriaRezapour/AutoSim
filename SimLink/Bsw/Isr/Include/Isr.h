/**
 * @file    Isr.h
 * @author  Your Name
 * @date    2025-05-10
 * @version 0.1.0
 * @brief   Interrupt Service Routines header for STM32H7xx.
 *
 * @details
 * Detailed description (optional):
 * Declares the Cortex-M7 exception handlers and STM32H7xx
 * peripheral interrupt handlers used by the application.
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | 0.1.0   | 2025-05-10 | Your Name  | Initial release                      |
 * |---------|------------|------------|--------------------------------------|
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H7xx_IT_H
#define __STM32H7xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void ETH_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H7xx_IT_H */
