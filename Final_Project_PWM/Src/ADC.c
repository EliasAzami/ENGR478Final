/*
 * ADC.c
 *
 *  Created on: Dec 4, 2025
 *      Author: Elias Asami, Milton Salazar
 */
#include "ADC.h"
#include "stm32l476xx.h"
#include <stdint.h>

volatile uint32_t adc_result = 0; // Definition of global variable 'adc_result' declared in "ADC.h"

//-------------------------------------------------------------------------------------------
//  ADC1_Wakeup
//  Wake up ADC1 from deep-power-down mode and enable the internal voltage regulator.
//-------------------------------------------------------------------------------------------
void ADC1_Wakeup (void) {

	int wait_time;

	// 1. Exit deep power down mode
	ADC1->CR &= ~ADC_CR_DEEPPWD;

	// 2. Enable the ADC internal voltage regulator
	ADC1->CR |= ADC_CR_ADVREGEN;

	// 3. Wait for ADC voltage regulator start-up time (T_ADCVREG_STUP)
	//    T_ADCVREG_STUP ≈ 20 µs at 4 MHz
	wait_time = 20 * (4000000 / 1000000);
	while(wait_time != 0) {
		wait_time--;
	}
}

//-------------------------------------------------------------------------------------------
//  ADC_Common_Configuration
//  Configure ADC common control register: clock mode, prescaler, and dual mode.
//-------------------------------------------------------------------------------------------
void ADC_Common_Configuration() {

	// 1. Select ADC input clock through ADC_CCR, field CKMODE[1:0]
	//    CKMODE = 01: HCLK/1 (synchronous clock mode)
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;   // Clear CKMODE bits
	ADC123_COMMON->CCR |=  ADC_CCR_CKMODE_0; // HCLK/1

	// 2. Independent mode (no dual mode)
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;

	// 3. No additional prescaler on ADC clock (PRESC = 0000)
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
}

//-------------------------------------------------------------------------------------------
//  ADC_Pin_Init
//  Initialize PC0 as analog input for ADC1 channel 1 (ADC123_IN1).
//-------------------------------------------------------------------------------------------
void ADC_Pin_Init(void){

	// 1. Enable the clock of GPIO Port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	// 2. Configure PC0 in analog mode: MODER0[1:0] = 11
	GPIOC->MODER |= 0b11UL << (2 * 0);

	// 3. Connect analog switch to the ADC input pin by configuring GPIOC_ASCR
	GPIOC->ASCR |= 1UL << 0;
}

//--------------------------------------------------------------------------------------------------
//  ADC_Init
//  Initialize ADC1 in single-conversion, polling mode.
//  - 10-bit resolution, right-aligned data
//  - Channel 1 on PC0
//  - Software-triggered conversions (no continuous mode, no interrupts)
//--------------------------------------------------------------------------------------------------
void ADC_Init(void){

	// 1. Disable ADC1 before further configurations
	ADC1->CR &= ~ADC_CR_ADEN;

	// 2. Enable the clock of ADC1
	RCC->AHB2ENR  |= RCC_AHB2ENR_ADCEN;

	// 3. Configure ADC common parameters (clock, dual mode)
	ADC_Common_Configuration();

	// 4. Wake up ADC1 from deep power down mode
	ADC1_Wakeup();

	// 5. Configure single-ended mode for channel 1 (PC0)
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_1; 	// 0: single-ended on channel 1

	// 6. Start ADC1 calibration to remove offset error
	ADC1->CR |=  ADC_CR_ADCAL;                     // Start calibration
	while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); // Wait until calibration is done

	// 7. Enable ADC1 module
	ADC1->CR |= ADC_CR_ADEN;

	// 8. Initialize ADC input pin PC0 as analog
	ADC_Pin_Init();

	// 9. Configure ADC data resolution and alignment
	//    RES[1:0] = 01 → 10-bit; ALIGN = 0 → right alignment
	ADC1->CFGR &= ~ADC_CFGR_RES;        // Clear resolution bits
	ADC1->CFGR |=  ADC_CFGR_RES_0;      // 10-bit resolution
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;      // Right alignment

	// 10. Set up the ADC regular sequence length and channel
	ADC1->SQR1 &= ~ADC_SQR1_L;          // Sequence length = 1 conversion
	ADC1->SQR1 &= ~ADC_SQR1_SQ1;        // Clear first conversion channel
	ADC1->SQR1 |=  (1U << 6);           // Channel 1 as the 1st conversion

	// 11. Select single-conversion mode (CONT = 0)
	ADC1->CFGR &= ~ADC_CFGR_CONT;

	// 12. Disable hardware triggers; use software trigger only (EXTEN = 00)
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;

	// 13. Disable ADC interrupts (use polling for EOC)
	ADC1->IER &= ~ADC_IER_EOC;          // No EOC interrupt

	// 14. Wait until ADC1 is ready to accept conversions (ADRDY = 1)
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
}

//-------------------------------------------------------------------------------------------
//  ADC_Read10bit
//  Perform a single ADC conversion on Channel 1 (PC0) and return a 10-bit result.
//
//  Returns:
//    0–1023: 10-bit conversion result corresponding to 0–3.3 V input.
//-------------------------------------------------------------------------------------------
uint16_t ADC_Read10bit(void) {

	// 1. Start a single ADC conversion by setting ADSTART
	ADC1->CR |= ADC_CR_ADSTART;

	// 2. Wait until End of Conversion (EOC) flag is set
	while ((ADC1->ISR & ADC_ISR_EOC) == 0);

	// 3. Clear EOC flag by writing 1 to it
	ADC1->ISR |= ADC_ISR_EOC;

	// 4. Read the conversion result from ADC1_DR
	adc_result = ADC1->DR;   // Store in global variable for monitoring/debug

	// 5. Return the 10-bit result
	return (uint16_t)adc_result;  // 0..1023
}
