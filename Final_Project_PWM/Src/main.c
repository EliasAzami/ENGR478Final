
/*
  @file    main.c
  @author  Milton
  @date    Fall-2025
 */

#include "stm32l476xx.h"
#include "LED.h"
#include "button.h"
#include "ADC.h"
#include "PWM.h"
#include "Systick_timer.h"
#include <stdint.h>


volatile uint8_t  system_active = 1;   // start
volatile uint8_t  system_arming = 0;   // 1 during arming delay
volatile uint32_t arming_ms     = 0;   // ms counter for arming state

int main(void){

    // 1. Initialize status LED (PA5, LD2)
    configure_LED_pin();
    turn_on_LED();       // Start with system active

    // 2. Initialize the user pushbutton and EXTI interrupt
    button_Init(); // PC13

    // 3. Initialize SysTick
    SysTick_Init(4000);	// 1 ms ticks (4 MHz / 4000 = 1 kHz)

    // 4. Initialize ADC
    ADC_Init();	//(0–3.3 V)throttle input

    // 5. Initialize PWM
    PWM_Init();	// (TIM2_CH1 on PA0) for ESC pulse output

    // 6. Main control loop:
    //    - If system_active = 1: read throttle (ADC) and update PWM pulse width.
    //    - If system_active = 0: hold ESC at a "stopped" pulse.
    while (1) {

        if (system_active) {
            // System running:
            // 1) Read one 10-bit ADC sample (0..1023)
            uint16_t value = ADC_Read10bit();

            // 2) Map ADC value (0..1023) to pulse width 1000..2000 us
            //
            //    us = 1000 us + (value / 1023) * 1000 us
            //    Integer math: us = 1000 + (value * 1000) / 1023
            uint16_t us = 1000 + (value * 1000) / 1023;

            // 3) Update PWM output for ESC
            PWM_SetPulse_us(us);
        }
        else {
            // System paused/disarmed:
            PWM_SetPulse_us(1000);	// Hold ESC at stopped pulse 1000 us
        }
    }
}
