/*
 * ADC.h
 *
 *  Created on: Dec 4, 2025
 *      Author: Elias Asami, Milton Salazar
 */

#ifndef __STM32L476G_ADC_H
#define __STM32L476G_ADC_H

#include "stm32l476xx.h"
#include <stdint.h>

extern volatile uint32_t adc_result; // Declaration of global variable to store sampled ADC data

// Modular function to wake up ADC1 from the deep-power-down mode
void ADC1_Wakeup (void);

// Modular function to initialize ADC external input channels
// In this sample, PC0 (ADC123_IN1) is used
void ADC_Pin_Init(void);

// Modular function to configure ADC common registers
void ADC_Common_Configuration(void);

// Modular function to initialize ADC (single-conversion, polling mode)
void ADC_Init(void);

// Modular function to perform one 10-bit ADC conversion on Channel 1 (PC0)
// Returns: 0–1023 for 0–3.3 V input
uint16_t ADC_Read10bit(void);

#endif /* __STM32L476G_ADC_H */


