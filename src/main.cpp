#include <stdint.h>
#include <stddef.h>
#include "gpio.h"
#include "DMA.h"
#include "RCC.h"
#include "device_offsets.h"


//Main function, section attribute makes sure it is at the beginning of .text
__attribute__((section (".entry_point"))) void main(void)  {

	RCC RCC1;

	//MCO on PA8
	initPin(GPIO_A_BASE_ADDR, 8, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
	initPinAlt(GPIO_A_BASE_ADDR, 8, AF0);
	
	//Pause for a couple seconds
	uint32_t j;
	for (uint32_t i = 0; i < 36000000; i++) {
		//Do nothing
		j = i;
	}
	
	//Switch RCC oscillator to HSI48
	RCC1.init();

	//Infinite loop
	while(1) {

	}
}
