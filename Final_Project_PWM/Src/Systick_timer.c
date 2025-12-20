#include "Systick_timer.h"
#include "LED.h"
#include "stm32l476xx.h"
#include <stdint.h>

extern volatile uint8_t  system_active;
extern volatile uint8_t  system_arming;
extern volatile uint32_t arming_ms;

//-------------------------------------------------------------------------------------------
// Initialize SysTick
//  Reload is set so that:
//    tick_period = Reload / CPU_clock
//  With 4 MHz CPU clock and Reload=4000 → 1 ms per tick.
//-------------------------------------------------------------------------------------------
void SysTick_Init(uint32_t Reload){
    // 1. Disable SysTick
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    // 2. Program reload value
    SysTick->LOAD = Reload - 1;

    // 3. Clear current value
    SysTick->VAL = 0;

    // 4. Enable SysTick interrupt
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    // 5. Select processor clock
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    // 6. Enable SysTick
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

//-------------------------------------------------------------------------------------------
// SysTick Exception Handler
//  States:
//    DISARMED  : system_active = 0, system_arming = 0 → LED OFF
//    ARMING    : system_arming = 1                 → fast blink (100 ms) for 3 s
//    ARMED     : system_active = 1, !system_arming → heartbeat (500 ms toggle)
//-------------------------------------------------------------------------------------------
void SysTick_Handler(void) {

    // 1. ARMING state: fast blink + 3s delay
    if (system_arming) {
        arming_ms++;

        // Fast blink while arming: toggle every 100 ms
        if ((arming_ms % 100) == 0) {
            toggle_LED();    // LED on PA5
        }

        // After 3 seconds of arming → fully ARMED
        if (arming_ms >= 3000) {
            system_arming = 0;
            system_active = 1;
            arming_ms     = 0;
            turn_on_LED();  // Start ARMED from LED ON
        }
        return;  // Do not run heartbeat in this branch
    }

    // 2. DISARMED: LED off, no heartbeat, PWM neutral in main loop
    if (!system_active) {
        turn_off_LED();
        return;
    }

    // 3. ARMED: slow heartbeat blink
    static uint32_t hb_ms = 0;
    hb_ms++;

    // Heartbeat: toggle every 500 ms (1 Hz blink)
    if (hb_ms >= 500) {
        hb_ms = 0;
        toggle_LED();
    }
}
