/*
 * button.c
 *
 *  Created on: Nov 2025
 *      Author: Milton Salazar
 */
#include "button.h"
#include "LED.h"
#include "Systick_timer.h"

// system_active is defined in main.c
extern volatile uint8_t system_active;
extern volatile unit8_t system_arming;
extern volatile unit8_t arming_ms;

// PC13  <--> Blue User Button
#define BUTTON_PIN   13


void button_init() {
  // 1. Enable the clock to GPIO Port C
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

  // 2. Configure GPIO Mode to Input
  GPIOC->MODER &= ~(3UL << (2 * BUTTON_PIN));    // Input(00)

  // 3. Configure GPIO Pull-Up/Pull-Down
  GPIOC->PUPDR &= ~(3UL << (2 * BUTTON_PIN)); // No Pull-up or Pull-down

  // 4. Configure EXTI line 13 to be triggered by PC13
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;           // Enable SYSCFG clock
  SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13);  // Clear existing setting
  SYSCFG->EXTICR[3] |=  (SYSCFG_EXTICR4_EXTI13_PC); // Set EXTI13 to PC13

  // 5. Configure EXTI Trigger for falling edge (button press pulls PC13 low)
  EXTI->IMR1  |=  (EXTI_IMR1_IM13);    // Unmask EXTI13 line
  EXTI->FTSR1 |=  (EXTI_FTSR1_FT13);   // Enable falling-edge trigger
  EXTI->PR1    =  (EXTI_PR1_PIF13);    // Clear any pending flag

  // 6. Enable EXTI15_10 interrupt in NVIC
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void) {
  // Check if EXTI13 triggered
  if (EXTI->PR1 & EXTI_PR1_PIF13) {
    EXTI->PR1 = EXTI_PR1_PIF13;
    if (syste_active){
    	system_active = 0;
    	system_arming = 0;
    	arming_ms = 0;
    	turn_off_LED();	//Disarmed = LED OFF
    }
    else if (!system_arming) {
    	system_arming = 1;
    	arming_ms = 0;
    	turn_off_LED();
    }
  }
}

