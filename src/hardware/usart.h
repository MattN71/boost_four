#pragma once

#include <cstdint>
#include <cstddef>
#include "device_offsets.h"

//All assuming 48 Mhz clock and oversampling by 16 (OVER8 = 0)

class USART_TYPE {
private:
     //Types
     enum REG_INDEX {
          CR1 = 0,
          CR2 = 1,
          CR3 = 2,
          BRR = 3,
          GTPR = 4,
          RTOR = 5,
          RQR = 6,
          ISR = 7,
          ICR = 8,
          RDR = 9,
          TDR = 10
     };

     //Variables
     volatile uint32_t *REG;
     uint8_t sendBuffer[1024];

     //Methods
public:
     //Types
     enum BAUD_RATE_TYPE {BAUD_9600, BAUD_115200, BAUD_2M, BAUD_3M};
     enum USART_PERIF_TYPE {USART1_PERIF, USART2_PERIF, USART3_PERIF, USART4_PERIF};
     enum INTERRUPT_TYPE {TEMP, TEMP2};


     //Constructors
     USART_TYPE(void);
     USART_TYPE(USART_PERIF_TYPE usart_perif, BAUD_RATE_TYPE baud_rate);

     //Methods

     //Blocking send functions
     void print(const char *start);
     void println(const char *start);
     void sendBytes(size_t numBytes, uint8_t* start);
     void sendNumAsASCII(uint8_t num);

     //Read functions
     bool readAvailable(void);
     char readChar(void);

     //Interrupts
     void enableInterrupt(INTERRUPT_TYPE interrupt, bool setting);
};
