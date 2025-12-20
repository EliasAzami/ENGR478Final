/*
 * button.c
 *
 *  Created on: Dec 4, 2025
 *      Author: Elias Asami, Milton Salazar
 */
#include "button.h"
#include "LED.h"
#include "PWM.h"
#include "Systick_timer.h"
#include "stm32l476xx.h"
#include <stdint.h>

// Flags are defined in main.c
extern volatile uint8_t  system_active;
extern volatile uint8_t  system_arming;
extern volatile uint32_t arming_ms;

// PC13  <--> Blue User Button
#define BUTTON_PIN   13
#define PWM_STOP_TICKS  1000

void button_Init(void) {
    // 1. Enable the clock to GPIO Port C
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    // 2. Configure PC13 as input
    GPIOC->MODER &= ~(3UL << (2 * BUTTON_PIN));    // Input(00)

    // 3. No pull-up or pull-down
    GPIOC->PUPDR &= ~(3UL << (2 * BUTTON_PIN));

    // 4. Configure EXTI line 13 to be triggered by PC13
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;              // Enable SYSCFG clock
    SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13);
    SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC;    // EXTI13 <- PC13

    // 5. Enable falling-edge trigger on line 13
    EXTI->IMR1  |=  EXTI_IMR1_IM13;     // Unmask
    EXTI->FTSR1 |=  EXTI_FTSR1_FT13;    // Falling edge
    EXTI->PR1    =  EXTI_PR1_PIF13;     // Clear any pending flag

    // 6. Enable EXTI15_10 interrupt in NVIC
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void) {
    // Check if EXTI13 triggered
    if (EXTI->PR1 & EXTI_PR1_PIF13) {
        EXTI->PR1 = EXTI_PR1_PIF13;   // Clear pending flag

        if (system_active) {
            // ARMED → DISARMED
            system_active = 0;
            system_arming = 0;
            arming_ms     = 0;

            // Immediately force PWM to STOP pulse (e.g., 1 ms pulse)
            PWM_SetPulse_us(PWM_STOP_TICKS);
            // Stop LED (SysTick_Handler will keep it off while inactive)
            turn_off_LED();
        }
        else if (!system_arming) {
            // DISARMED → start ARMING sequence
            system_arming = 1;
            arming_ms     = 0;

            // Start from LED off; SysTick will fast blink during arming
            turn_off_LED();
        }
    }
}
