/*
  @file    main.c
  @author  Milton Salazar
  @date    Fall-2025
 */

#include "stm32l476xx.h"
#include "ADC.h"
#include "PWM.h"
#include "LED.h"
#include "button.h"
#include "Systick_timer.h"
#include <stdint.h>

// Global flag to indicate whether the PWM system is active or paused.
// 1: System active (PWM follows throttle)
// 0: System paused (PWM off)
volatile uint8_t system_active = 1;

int main(void){

	// 1. Initialize status LED (e.g., LD2) to indicate system active/paused.
	//    In this sample, LED_On means system is running, LED_Off means paused.
	//configure_LED_pin();
	//turn_on_LED();       // Start with system active

	// 2. Initialize the user pushbutton and configure EXTI interrupt.
	button_init(); 	// The button interrupt will toggle 'system_active'

	// 3. Initialize SysTick timer to generate a periodic heartbeat interrupt.
	//    The default processor clock is 4MHz provided by MSI.
	//    For example, SysTick_Init(400000) generates an interrupt every 0.1 s.
	//SysTick_Init(400000);

	// 4. Initialize ADC to read 0–3.3 V on the analog input (e.g., PC0 → ADC1_IN1).
	//    ADC is configured in single-conversion, polling mode with 10-bit resolution.
	ADC_Init();

	// 5. Initialize PWM output (e.g., TIM2_CH1 on PA5) to generate 0–100% duty cycle.
	//    The PWM duty will be updated based on the ADC reading.
	PWM_Init();

	// 6. Main control loop:
	//    - If system_active = 1: read throttle (ADC) and update PWM duty cycle.
	//    - If system_active = 0: force PWM duty to 0% (output off).
	while (1) {

		if (system_active) {
			// System running:
			// 1) Read one 10-bit ADC sample (0..1023) from the throttle input
			uint16_t value = ADC_Read10bit();

			// 2) Directly map ADC value to PWM duty (0..1023 → 0..100% duty)
			PWM_SetDuty(value);
		} else {
			// System paused:
			// Force PWM output off (0% duty cycle)
			PWM_SetDuty(0);
		}

	}
}
