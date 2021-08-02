#include "timer15.h"
#include <cstdint>
#include "device_offsets.h"

//********** Constructors **********


TIM15_TYPE::TIM15_TYPE() {
     REG = TIM15_BASE_ADDR;
}


//********** Methods **********

void TIM15_TYPE::setTopValue(uint16_t val) {
     REG[ARR] = val;
}

void TIM15_TYPE::setCompValue(CCR_CHANNEL_TYPE channel, uint16_t val) {
     switch (channel) {
          case CCR_CH1:
               REG[CCR1] = val;
               break;
          case CCR_CH2:
               REG[CCR2] = val;
               break;
     }
}

void TIM15_TYPE::setupTriggerOut() {
     //Enable CH1
     REG[CCMR1] |= BIT6 | BIT5; //Pulse mode (high on match), CCR1
     REG[CCER] |= BIT0; //Enable CCR1 output
     REG[BDTR] |= BIT15; //Master enable outputs
     REG[CR2] |= BIT6; //Use CCR1 signal for TRGO
}


uint16_t TIM15_TYPE::getCount(void) {
     return REG[CNT];
}

volatile uint32_t* TIM15_TYPE::getCountReg(void) {
     return &REG[CNT];
}
