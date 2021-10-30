#pragma once
#include <cstdint>
#include "device_offsets.h"

class TIM16_17_TYPE {
private:
	//Types
	enum reg_offsets {
		CR1 = 0,
	     CR2 = 1,
	     DIER = 3,
	     SR = 4,
	     EGR = 5,
	     CCMR1 = 6,
	     CCER = 8,
	     CNT = 9,
	     PSC = 10,
	     ARR = 11,
		RCR = 12,
	     CCR1 = 13,
		BDTR = 17,
	     DCR = 18,
	     DMAR = 19
	};

	//Variables
	volatile uint32_t *REG;
public:

	enum CCR_CHANNEL_TYPE {
		CCR_CH1
	};

     TIM16_17_TYPE();
     TIM16_17_TYPE(volatile uint32_t *base_address);


     void setTopValue(uint16_t val);
     void setCompValue(CCR_CHANNEL_TYPE channel, uint16_t val);
     //uint32_t* getCompValuePointer(void);
     void setupPWM(void);

	uint16_t getCount(void);
	volatile uint32_t* getCountReg(void);


	//Inline functions
	__attribute__((always_inline)) inline void startTimer(void) {
		REG[CR1] = BIT0;
	}

	__attribute__((always_inline)) inline void setCountValue(uint16_t val) {
	     REG[CNT] = val;
	}

};
