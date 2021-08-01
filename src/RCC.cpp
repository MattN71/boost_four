#include "RCC.h"
#include "device_offsets.h"

RCC::RCC() {
	REG = RCC_BASE_ADDR;

	//Enable MCO on PA8 for debugging
	REG[CFGR] |= BIT30 | BIT29 | BIT28; //MCO output prescaler is clock/128
	REG[CFGR] |= BIT26; //System clock to MCO pin
}

void RCC::init(void) {
	REG[CR2] |= BIT16; //Start HSI48 oscillator

	//Change flash wait state to 1 (required for sysclck > 24 Mhz)
	FLASH_BASE_ADDR[0] |= 0x1;

	while (!(REG[CR2] & BIT17)) {
		//Block until oscillator is ready
	}

	//Switch system clock to HSI48
	REG[CFGR] |= 0x3;

	//Enable AHB/APB busses
	REG[AHBENR] |= BIT17 | BIT18 | BIT19 | BIT0; //GPIO A, B, C; DMA
	REG[APB2ENR] |= BIT14 | BIT9 | BIT11 | BIT16 | BIT17 | BIT18; //USART1, ADC and Timers
	REG[APB1ENR] |= BIT8 | BIT5 | BIT4 | BIT1 | BIT0; //Timers

	//Set system clock as clock input to USART1
	REG[CFGR3] |= BIT0;
}
