#include <cstdint>
#include <cstddef>
#include "nvic.h"
#include "device_offsets.h"

void NVIC_TYPE::enableInterrupt(INTERRUPT_TYPE interrupt, bool setting) {
     uint32_t bitmask = 0x0000;
     switch (interrupt) {
          case DMA_CH1:
               bitmask = BIT9;
               break;

          case ADC_COMP:
               bitmask = BIT12;
               break;

          case USART1:
               bitmask = BIT27;
               break;
     }

     if (setting) {
          *ISER |= bitmask;
     } else {
          *ICER |= bitmask;
          return;
     }

     /* To-Do
     size_t priority_reg_index = (interrupt / 4) - 1;
     size_t priority_reg_position = (interrupt % 4);
     uint32_t priority_mask = 0x0000;

     IPR[priority_reg_index] |=
     */
}
