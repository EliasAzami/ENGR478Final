
#include "Systick_timer.h"
#include "LED.h"

//-------------------------------------------------------------------------------------------
// Initialize SysTick	
//-------------------------------------------------------------------------------------------	
void SysTick_Init(uint32_t Reload){
	
		
	// 1. Disable SysTick Counter before all configurations are done
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		
	
	// 2. Program reload value by configuring SysTick Reload Value Register. 
	SysTick->LOAD = Reload - 1;    //Note: Time Interval = (Content of the register + 1) � Source_Clock_Period
	
	// 3. Clear reload value by configuring SysTick Current Value Register. 
	SysTick->VAL = 0;
								
	// 4. Enable SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// 5. Select processor clock
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	
	// 6. Enable SysTick Timer once all configurations are done
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  
}

//-------------------------------------------------------------------------------------------
// SysTick Exception Handler
//-------------------------------------------------------------------------------------------
void SysTick_Handler(void){
	// Toggle PA5 each time SysTick interrupt fires
	GPIOA->ODR ^= (1 << 5);
}
	
