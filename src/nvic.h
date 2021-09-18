#pragma once
#include <cstdint>
#include <cstddef>
#include "device_offsets.h"


class NVIC_TYPE {
private:
     //Types

     //Variables
     volatile uint32_t * ISER = (volatile uint32_t *)0xE000E100;
     volatile uint32_t * ICER = (volatile uint32_t *)0xE000E180;
     volatile uint32_t * ISPR = (volatile uint32_t *)0xE000E200;
     volatile uint32_t * ICPR = (volatile uint32_t *)0xE000E280;
     //volatile uint32_t * const IPR = 0xE000E400; //IPR[0] - IPR[7]

     //Methods

public:
     //Types
     enum INTERRUPT_TYPE {
          DMA_CH1 = 9,
          ADC_COMP = 12,
          USART1 = 27
     };

     //Constructor


     //Methods
     void enableInterrupt(INTERRUPT_TYPE interrupt, bool setting);

};
