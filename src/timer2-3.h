#pragma once
#include <cstdint>
#include "device_offsets.h"

class TIM2_3_TYPE {
private:
	//Types
	enum TIM2_3_index_type {
		CR1 = 0,
	     CR2 = 1,
	     SMCR = 2,
	     DIER = 3,
	     SR = 4,
	     EGR = 5,
	     CCMR1 = 6,
	     CCMR2 = 7,
	     CCER = 8,
	     CNT = 9,
	     PSC = 10,
	     ARR = 11,
	     CCR1 = 13,
	     CCR2 = 14,
	     CCR3 = 15,
	     CCR4 = 16,
	     DCR = 18,
	     DMAR = 19
	};

	//Variables
	volatile uint32_t *REG;
public:

	enum TIM2_3_CHANNEL_TYPE {
		CCR_CH1,
		CCR_CH2,
		CCR_CH3,
		CCR_CH4
	};

     TIM2_3_TYPE();
     TIM2_3_TYPE(volatile uint32_t *base_address);


     void setTopValue(uint16_t val);
     void setCompValue(TIM2_3_CHANNEL_TYPE channel, uint16_t val);
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
