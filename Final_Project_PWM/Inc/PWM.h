/*
 * PWM.h
 *
 *  Created on: Dec 4, 2025
 *      Author: Elias Asami, Milton Salazar
 */

#ifndef __STM32L476G_PWM_H
#define __STM32L476G_PWM_H

#include "stm32l476xx.h"
#include <stdint.h>

// Global variable to store the most recent PWM duty value (0..1023).
// Useful for monitoring/debugging in the Expressions window.
extern volatile uint16_t pwm_duty;

// Modular function to initialize the PWM output pin.
// In this sample, PA5 (TIM2_CH1) is used to drive the LD2 LED with PWM.
void PWM_Pin_Init(void);

// Modular function to configure the timer used for PWM generation.
// In this sample, TIM2 Channel 1 is used.
void PWM_Timer_Init(void);

// Modular function to initialize PWM: configure both pin and timer.
void PWM_Init(void);

// Modular function to set PWM duty cycle.
// Input 'duty' is a 10-bit value: 0 (0%) to 1023 (≈100%).
void PWM_SetPulse_us(uint16_t us);


#endif /* __STM32L476G_PWM_H */

