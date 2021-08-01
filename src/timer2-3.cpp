#include "timer2-3.h"
#include <cstdint>
#include "device_offsets.h"

//********** Constructors **********

//Do not use
TIM2_3_TYPE::TIM2_3_TYPE() {
     REG = TIM2_BASE_ADDR;
}

TIM2_3_TYPE::TIM2_3_TYPE(volatile uint32_t *base_address) {
     REG = base_address;
}


//********** Methods **********

void TIM2_3_TYPE::setTopValue(uint16_t val) {
     REG[ARR] = val;
}

void TIM2_3_TYPE::setCompValue(TIM2_3_CHANNEL_TYPE channel, uint16_t val) {
     switch (channel) {
          case CCR_CH1:
               REG[CCR1] = val;
               break;
          case CCR_CH2:
               REG[CCR2] = val;
               break;
          case CCR_CH3:
               REG[CCR3] = val;
               break;
          case CCR_CH4:
               REG[CCR4] = val;
               break;
     }
}

void TIM2_3_TYPE::setupPWM(void) {
     //Emable CH2
     REG[CCMR1] |= BIT14 | BIT13; //PWM mode 1, CCR2
     REG[CCER] |= BIT4; //Enable CCR2 output
}


uint16_t TIM2_3_TYPE::getCount(void) {
     return REG[CNT];
}

volatile uint32_t* TIM2_3_TYPE::getCountReg(void) {
     return &REG[CNT];
}
