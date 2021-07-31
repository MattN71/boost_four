#pragma once

#include <cstdint>

enum RCC_index_type {
	CR = 0,
	CFGR = 1,
	CIR = 2,
	APB2RSTR = 3,
	APB1RSTR = 4,
	AHBENR = 5,
	APB2ENR = 6,
	APB1ENR = 7,
	BDCR = 8,
	CSR = 9,
	AHBRSTR = 10,
	CFGR2 = 11,
	CFGR3 = 12,
	CR2 = 13
};

enum RCC_perif_type {
	CLK_DMA,
	CLK_IOA,
	CLK_IOB,
	CLK_IOC,
	CLK_IOD,
	CLK_IOE,
	CLK_IOF,
};


class RCC {

private:
	volatile uint32_t* REG;

public:
	//Constructors
	RCC();

	//Methods
	void init(void);
	
};
