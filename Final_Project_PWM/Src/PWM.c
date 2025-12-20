/*
 * PWM.c
 *
 *  Created on: Dec 4, 2025
 *      Author: Elias Asami, Milton Salazar
 */
#include "PWM.h"
#include "stm32l476xx.h"
#include <stdint.h>

// Global variable to store the most recent PWM duty/pulse value
volatile uint16_t pwm_duty = 0;

//-------------------------------------------------------------------------------------------
//  PWM_Pin_Init
//  Initialize PA0 as alternate function TIM2_CH1 to output a PWM signal.
//  PA0 will go to the ESC / propulsion system.
//  PA5 is reserved for the on-board LED heartbeat.
//-------------------------------------------------------------------------------------------
void PWM_Pin_Init(void) {

    const uint32_t PWM_PIN = 0;   // PA0

    // 1. Enable the clock of GPIO Port A
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // 2. Configure PA0 as Alternate Function mode
    GPIOA->MODER &= ~(0b11UL << (2 * PWM_PIN));      // Clear mode bits for PA0
    GPIOA->MODER |=  (0b10UL << (2 * PWM_PIN));      // Set mode to Alternate Function

    // 3. Select Alternate Function AF1 (TIM2_CH1) for PA0
    GPIOA->AFR[0] &= ~(0xFUL << (4 * PWM_PIN));      // Clear AF bits for PA0
    GPIOA->AFR[0] |=  (0x1UL << (4 * PWM_PIN));      // AF1 = TIM2_CH1

    // Default: push-pull, low speed, no pull-up/pull-down
}

//-------------------------------------------------------------------------------------------
//  PWM_Timer_Init
//  Configure TIM2 Channel 1 to generate a PWM signal on PA0.
//-------------------------------------------------------------------------------------------
void PWM_Timer_Init(void) {

    // 1. Enable clock for TIM2 on APB1 bus
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    // 2. Configure TIM2 prescaler and auto-reload register
    //
    // Assume system clock = 4 MHz.
    // For a standard ESC-style pulse, we'll use a 20 ms period (50 Hz).
    //
    // Choose:
    //   PSC = 79  => timer clock = 4 MHz / (79 + 1) = 50 kHz
    //   ARR = 999 => period = (999 + 1) / 50 kHz = 20 ms
    //
    TIM2->PSC = 79;
    TIM2->ARR = 999;

    // 3. Configure TIM2 Channel 1 as PWM mode 1, with preload
    TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M);
    TIM2->CCMR1 |=  (6U << TIM_CCMR1_OC1M_Pos);   // OC1M = 110: PWM mode 1
    TIM2->CCMR1 |=  TIM_CCMR1_OC1PE;              // Enable preload for CCR1

    // 4. Enable output for Channel 1
    TIM2->CCER |= TIM_CCER_CC1E;

    // 5. Initialize duty cycle to neutral / low
    TIM2->CCR1 = 0;

    // 6. Enable auto-reload preload
    TIM2->CR1 |= TIM_CR1_ARPE;

    // 7. Generate an update event
    TIM2->EGR |= TIM_EGR_UG;

    // 8. Enable the counter
    TIM2->CR1 |= TIM_CR1_CEN;
}

//-------------------------------------------------------------------------------------------
//  PWM_Init
//-------------------------------------------------------------------------------------------
void PWM_Init(void) {

    PWM_Pin_Init();     // PA0 as TIM2_CH1
    PWM_Timer_Init();   // TIM2 CH1 config
}

//-------------------------------------------------------------------------------------------
//  PWM_SetPulse_us
//  Set the PWM pulse width in microseconds for TIM2 CH1.
//  Here, 1000..2000 us maps into the 20 ms frame.
//-------------------------------------------------------------------------------------------
void PWM_SetPulse_us(uint16_t us)
{
    // Clamp to [1000, 2000] us
    if (us < 1000) us = 1000;
    if (us > 2000) us = 2000;

    // With 50 kHz timer clock and ARR = 999 (20 ms period):
    // Period = 20 ms -> 1000 counts = 20 ms => 1 count = 20 us
    //
    // So counts = us / 20
    uint16_t counts = us / 20;

    pwm_duty    = counts;
    TIM2->CCR1  = counts;
}
