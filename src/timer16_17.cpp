#include "timer16_17.h"
#include <cstdint>
#include "device_offsets.h"

//********** Constructors **********

//Do not use
TIM16_17_TYPE::TIM16_17_TYPE() {
     REG = TIM15_BASE_ADDR;
}

TIM16_17_TYPE::TIM16_17_TYPE(volatile uint32_t *base_address) {
     REG = base_address;
}


//********** Methods **********

void TIM16_17_TYPE::setTopValue(uint16_t val) {
     REG[ARR] = val;
}

void TIM16_17_TYPE::setCompValue(CCR_CHANNEL_TYPE channel, uint16_t val) {
     switch (channel) {
          case CCR_CH1:
               REG[CCR1] = val;
               break;
     }
}

void TIM16_17_TYPE::setupPWM(void) {
     //Emable CH1
     REG[CCMR1] |= BIT6 | BIT5; //PWM mode 1, CCR1
     REG[CCER] |= BIT0; //Enable CCR1 output
     REG[BDTR] |= BIT15; //Enable outputs
}


uint16_t TIM16_17_TYPE::getCount(void) {
     return REG[CNT];
}

volatile uint32_t* TIM16_17_TYPE::getCountReg(void) {
     return &REG[CNT];
}
