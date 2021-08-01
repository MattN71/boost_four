#include "DMA.h"

//DMA channel assignments:
// 1. ADC
// 2. USART1 - TX


//Constructor
DMA_CH_TYPE::DMA_CH_TYPE(dma_channel_type channel) {
	//Same for all channels (set by device)
	BASE_ADDR = (volatile uint32_t*)0x40020000;
	ISR = BASE_ADDR;
	IFCR = BASE_ADDR + (0x04/sizeof(BASE_ADDR));

	//Depend on channel selection
	switch (channel) {
		case CH1:
			CCR = BASE_ADDR + (0x08/sizeof(BASE_ADDR));
			break;
		case CH2:
			CCR = BASE_ADDR + (0x1C/sizeof(BASE_ADDR));
			break;
		case CH3:
			CCR = BASE_ADDR + (0x30/sizeof(BASE_ADDR));
			break;
		case CH4:
			CCR = BASE_ADDR + (0x44/sizeof(BASE_ADDR));
			break;
		case CH5:
			CCR = BASE_ADDR + (0x58/sizeof(BASE_ADDR));
			break;
		case CH6:
			CCR = BASE_ADDR + (0x6C/sizeof(BASE_ADDR));
			break;
		case CH7:
			CCR = BASE_ADDR + (0x80/sizeof(BASE_ADDR));
			break;
	}

	CNDTR = CCR + 1;
	CPAR = CNDTR + 1;
	CMAR = CPAR + 1;

	TEIF_mask = 0x8 << (channel - 1);
	HTIF_mask = 0x4 << (channel - 1);
	TCIF_mask = 0x2 << (channel - 1);
	GIF_mask  = 0x1 << (channel - 1);
}


void DMA_CH_TYPE::resetChannel(void) {
	*CCR &= ~(0x7FFF);
}

bool DMA_CH_TYPE::checkErrorIntFlag(void) {
	return (*ISR & TEIF_mask);
}

void DMA_CH_TYPE::resetErrorIntFlag(void) {
	*IFCR |= TEIF_mask;
}

void DMA_CH_TYPE::setPriority(dma_priority_type prior) {
	*CCR &= ~(0x3 << 12); //Reset bits
	switch (prior) {
		case PRIORITY_MEDIUM:
			*CCR |= 0x1 << 12;
			break;
		case PRIORITY_HIGH:
			*CCR |= 0x2 << 12;
			break;
		case PRIORITY_VERY_HIGH:
			*CCR |= 0x3 << 12;
			break;
		case PRIORITY_LOW:
			break;
	}
}

void DMA_CH_TYPE::setMemSize(dma_size_type size) {
	*CCR &= ~(0x3 << 10); //Reset bits
	switch (size) {
		case SIZE_16_BIT:
			*CCR |= 0x1 << 10;
			break;
		case SIZE_32_BIT:
			*CCR |= 0x2 << 10;
			break;
		case SIZE_8_BIT:
			break;
	}
}

void DMA_CH_TYPE::setPerifSize(dma_size_type size) {
	*CCR &= ~(0x3 << 8);
	switch (size) {
		case SIZE_16_BIT:
			*CCR |= 0x1 << 8;
			break;
		case SIZE_32_BIT:
			*CCR |= 0x2 << 8;
			break;
		case SIZE_8_BIT:
			break;
	}
}

void DMA_CH_TYPE::setMemIncrement(bool m_inc) {
	if (m_inc) {
		*CCR |= (1 << 7);
	} else {
		*CCR &= ~(1 << 7);
	}
}

void DMA_CH_TYPE::setPerifIncrement(bool p_inc) {
	if (p_inc) {
		*CCR |= (1 << 6);
	} else {
		*CCR &= ~(1 << 6);
	}
}

void DMA_CH_TYPE::setCircularMode(bool circ_mode) {
	if (circ_mode) {
		*CCR |= (1 << 5);
	} else {
		*CCR &= ~(1 << 5);
	}
}

void DMA_CH_TYPE::setTransferDirection(dma_direction_type dir) {
	*CCR &= ~(1 << 4);
	if (dir == MEM_TO_PERIF) {
		*CCR |= (0x1 << 4);
	}
}

void DMA_CH_TYPE::setErrorInterrupt(bool err_int) {
	if (err_int) {
		*CCR |= (1 << 3);
	} else {
		*CCR &= ~(1 << 3);
	}
}

void DMA_CH_TYPE::setNumberTransfers(uint16_t num) {
	*CNDTR = num;
}

void DMA_CH_TYPE::setMemAddress(uint32_t addr) {
	*CMAR = addr;
}

void DMA_CH_TYPE::setPerifAddress(uint32_t addr) {
	*CPAR = addr;
}

void DMA_CH_TYPE::enableChannel(void) {
	*CCR |= 0x1;
}

void DMA_CH_TYPE::disableChannel(void) {
	*CCR &= ~(0x1);
}
