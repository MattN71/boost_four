#include <stddef.h>
#include "usart.h"
#include "device_offsets.h"
#include "gpio.h"

static uint8_t tx_buffer[40];
static size_t tx_add_index = 0;
static size_t tx_send_index = 0;

void uart_init(void) {

	//Initialize TX pin (PA2)
	initPin(GPIO_A_BASE_ADDR, 2, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
	initPinAlt(GPIO_A_BASE_ADDR, 2, AF1);

	//Set baud rate to 9600
	//USART2_BASE_ADDR[3] |= 0x1388;
	//Set baud rate to 2 Mbaud
	//USART2_BASE_ADDR[3] |= 0x18;
	//Set baud rate to 3 Mbaud
	USART2_BASE_ADDR[3] |= BAUD_3M;

	//Power on UART
	USART2_BASE_ADDR[0] |= (1 << 0);

	//Power on transmitter
	USART2_BASE_ADDR[0] |= (1 << 3);

	//Enable TXE interrupt
	USART2_BASE_ADDR[0] |= BIT7;
}

void sendByte(uint8_t byte) {
	//Wait for last byte to be transferred to the TX shift register
	while (!(USART2_BASE_ADDR[7] & BIT7));
	USART2_BASE_ADDR[10] = byte;
}

void sendWord(uint32_t word) {
	sendByte(word & 0xFF);
	sendByte((word >> 8) & 0xFF);
	sendByte((word >> 16) & 0xFF);
	sendByte(word >> 24);
}

void sendHalfWord(uint32_t word) {
	sendByte(word & 0xFF);
	sendByte((word >> 8) & 0xFF);
}


void addByte(uint8_t byte) {
	tx_buffer[tx_add_index] = byte;
	tx_add_index++;
	if (tx_add_index > 39) tx_add_index = 0;
}

void USART2_IRQHandler(void) {
	uint32_t flags = USART2_BASE_ADDR[7];
	
	//TXE bit is set
	if (flags & BIT7) {
		if (tx_send_index != tx_add_index) {
			USART2_BASE_ADDR[10] = tx_buffer[tx_send_index];
			tx_send_index++;
			if (tx_send_index > 39) tx_send_index = 0;
		}
	}
	
}
