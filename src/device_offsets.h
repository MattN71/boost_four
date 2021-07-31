#pragma once


#define HIGH true
#define LOW false




//Basic bitwise defines
#define BIT0 (1 << 0)
#define BIT1 (1 << 1)
#define BIT2 (1 << 2)
#define BIT3 (1 << 3)
#define BIT4 (1 << 4)
#define BIT5 (1 << 5)
#define BIT6 (1 << 6)
#define BIT7 (1 << 7)
#define BIT8 (1 << 8)
#define BIT9 (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)



//Memory offsets for device peripherals
//STM32F071x8 & STM32F071xB

#define GPIO_A_BASE_ADDR ((volatile uint32_t *)0x48000000)
#define GPIO_B_BASE_ADDR ((volatile uint32_t *)0x48000400)
#define GPIO_C_BASE_ADDR ((volatile uint32_t *)0x48000800)
#define DAC_BASE_ADDR ((volatile uint32_t *)0x40007400)
#define ADC_BASE_ADDR ((volatile uint32_t *)0x40012400)
#define RCC_BASE_ADDR ((volatile uint32_t *)0x40021000)
#define USART1_BASE_ADDR ((volatile uint32_t *)0x40013800)
#define USART2_BASE_ADDR ((volatile uint32_t *)0x40004400)
#define FLASH_BASE_ADDR ((volatile uint32_t *)0x40022000)

#define TIM2_BASE_ADDR ((volatile uint32_t *)0x40000000)
#define TIM3_BASE_ADDR ((volatile uint32_t *)0x40000400)

#define NVIC_BASE_ADDR ((volatile uint32_t *)0xE000E100)
