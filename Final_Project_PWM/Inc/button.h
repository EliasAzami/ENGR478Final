/*
 * button.h
 *
 *     Created: Nov 2025
 *      Author: Milton Salazar
 */

#ifndef __STM32L476G_BUTTON_H
#define __STM32L476G_BUTTON_H

#include "stm32l476xx.h"

// Modular function to initialize PC13 as an input pin with EXTI interrupt.
void button_init(void);

#endif /* __STM32L476G_BUTTON_H */
