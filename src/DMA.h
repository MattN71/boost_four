#pragma once

#include <stdint.h>

#define HIGH true
#define LOW false

enum dma_channel_type {
	CH1 = 1, 
	CH2 = 2, 
	CH3 = 3, 
	CH4 = 4, 
	CH5 = 5, 
	CH6 = 6, 
	CH7 = 7
};

enum dma_priority_type {
	PRIORITY_LOW = 0x0,
	PRIORITY_MEDIUM = 0x1,
	PRIORITY_HIGH = 0x2,
	PRIORITY_VERY_HIGH = 0x3
};

enum dma_size_type {
	SIZE_8_BIT = 0x0,
	SIZE_16_BIT = 0x1,
	SIZE_32_BIT = 0x2
};

enum dma_direction_type {
	PERIF_TO_MEM = 0x0,
	MEM_TO_PERIF = 0x1
};

class DMA_CH_TYPE {
private:
	uint32_t TEIF_mask;
	uint32_t HTIF_mask;
	uint32_t TCIF_mask;
	uint32_t GIF_mask;
	
	//Same for all channels
	volatile uint32_t* BASE_ADDR;
	volatile uint32_t* ISR;
	volatile uint32_t* IFCR;

	//Specific to each channel
	volatile uint32_t* CCR;
	volatile uint32_t* CNDTR;
	volatile uint32_t* CPAR;
	volatile uint32_t* CMAR;

public:
	//Constructors
	DMA_CH_TYPE(dma_channel_type channel);

	//Methods
	void resetChannel(void); 
	bool checkErrorIntFlag(void);
	void resetErrorIntFlag(void);
	void setPriority(dma_priority_type prior);
	
	void setMemSize(dma_size_type size);
	void setPerifSize(dma_size_type size);
	
	void setMemIncrement(bool m_inc);
	void setPerifIncrement(bool p_inc);
	
	void setCircularMode(bool circ_mode);
	void setTransferDirection(dma_direction_type dir);
	void setErrorInterrupt(bool err_int);

	void setNumberTransfers(uint16_t num);
	void setMemAddress(uint32_t addr);
	void setPerifAddress(uint32_t addr);

	void enableChannel(void);
	void disableChannel(void);

};
