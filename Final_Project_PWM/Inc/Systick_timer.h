#ifndef __STM32L476G_SYSTICK_H
#define __STM32L476G_SYSTICK_H

#include "stm32l476xx.h"

// Modular function to configure SysTick	module
void SysTick_Init(uint32_t Reload);

// SysTick Exception Handler
void SysTick_Handler(void);

#endif /* __STM32L476G_SYSTICK_H */