#include <cstdint>
#include <cstddef>
#include "usart.h"
#include "device_offsets.h"

//Constructors
USART_TYPE::USART_TYPE(void) {
	//Using USART1
	REG = USART1_BASE_ADDR;
	//Set baud rate to 9600
	REG[BRR] |= 0x1388;

	//Power on UART
	USART2_BASE_ADDR[0] |= (1 << 0);

	//Power on transmitter
	USART2_BASE_ADDR[0] |= (1 << 3);

	//Enable TXE interrupt
	USART2_BASE_ADDR[0] |= BIT7;
}

USART_TYPE::USART_TYPE(USART_PERIF_TYPE usart_perif, BAUD_RATE_TYPE baud_rate) {
	switch (usart_perif) {
		case USART1_PERIF:
	 		REG = USART1_BASE_ADDR;
			break;
		case USART2_PERIF:
			REG = USART2_BASE_ADDR;
			break;
		case USART3_PERIF:
			REG = USART3_BASE_ADDR;
			break;
		case USART4_PERIF:
			REG = USART4_BASE_ADDR;
			break;
	}

	//All assuming 48 Mhz clock and oversampling by 16
	REG[BRR] = 0x0;
	switch (baud_rate) {
		case BAUD_9600:
			REG[BRR] |= 0x1388;
		 	break;
		case BAUD_115200:
			REG[BRR] |= 0x1A1;
		 	break;
		case BAUD_2M:
			REG[BRR] |= 0x18;
			break;
		case	BAUD_3M:
			REG[BRR] |= 0x10;
			break;
	}

	//Power on UART
	REG[CR1] |= BIT0;

	//Power on transmitter/receiver
	REG[CR1] |= BIT3 | BIT2;
}



//Methods

void USART_TYPE::print(char *start) {
	while (*start != '\0') {
		while (!(REG[ISR] & BIT7));
		REG[TDR] = *start++;
	}
}


void USART_TYPE::println(char *start) {
	while (*start != '\0') {
		while (!(REG[ISR] & BIT7));
		REG[TDR] = *start++;
	}
	while (!(REG[ISR] & BIT7));
	REG[TDR] = '\r';
	while (!(REG[ISR] & BIT7));
	REG[TDR] = '\n';
}


void USART_TYPE::sendBytes(size_t numBytes, uint8_t* start) {
	for (size_t i = 0; i < numBytes; i++) {
		while (!(REG[ISR] & BIT7));
		REG[TDR] = *start++;
	}
}
