#ifndef usart_h
#define usart_h

#include <stdint.h>
#include <stdbool.h>

#define HIGH true
#define LOW false

//All assuming 48 Mhz clock and oversampling by 16 (OVER8 = 0)
enum BAUD_RATE_TYPE {BAUD_9600 = 0x1388, BAUD_115200 = 0x1A1, BAUD_2M = 0x18, BAUD_3M = 0x10};


void uart_init(void);

void sendByte(uint8_t byte);
void sendWord(uint32_t word);
void sendHalfWord(uint32_t halfWord);

#endif //usart_h
