//
//  startup.cpp
//
//
//  Created by Matt Nicklas on 9/14/19.
//

#include <stddef.h>
#include <stdint.h>

#pragma GCC push_options
#pragma GCC optimize ("O0")


//Symbols from linker script
extern volatile uint32_t __data_ROM_start__; //LMA
extern volatile uint32_t __data_start__; //VMA
extern volatile uint32_t __data_end__;

extern volatile uint32_t __bss_start__; //VMA
extern volatile uint32_t __bss_end__;

extern volatile uint32_t __stack; //Initial SP (Top of RAM section)

extern void (*__init_array_start__)(); //Function pointers for global object initialization
extern void (*__init_array_end__)();


//Entry point of application
extern void main(void);



//External interrupt handlers
//extern void TIM3_IRQHandler(void);
//extern void USART2_IRQHandler(void);



void Default_Handler (void) {
	while (1) {
		//Catch in debugger
	}
}

void Reset_Handler (void) {

	//Copy .data from ROM to RAM
	volatile uint32_t *startROM = &__data_ROM_start__;
	volatile uint32_t *startRAM = &__data_start__;
	volatile uint32_t *endRAM = &__data_end__;

	while (startRAM < endRAM) {
		*startRAM++ = *startROM++;
	}


	//Zero fill .bss in RAM
	startRAM = &__bss_start__;
	endRAM = &__bss_end__;

	while (startRAM < endRAM) {
		*startRAM++ = 0x0000;
	}

	//For C++, must call global constructors
    for (void (**p)() = &__init_array_start__; p < &__init_array_end__; p++) {
        (**p)();
    }

	//Application entry point
	main();
}



//Memory Layout
/*
 0x0800 0000: initStackPtr
 0x0800 0004: intVectorTable -> Reset_Handler
 ...
 ...
 0x0800 00BC: intVectorTable -> USB_IRQHandler
 */

//Set initial stack ptr
volatile uint32_t *initStackPtr __attribute__((section (".sp_init"))) = &__stack;

//To enable an interrupt, replace Default_Handler with the new handler function
void (*const intVectorTable[]) (void) __attribute__((section (".vectors"))) = {
  Reset_Handler,
  Default_Handler, //NMI_Handler
  Default_Handler, //HardFault_Handler
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  Default_Handler, //SVC_Handler
  0,
  0,
  Default_Handler, //PendSV_Handler
  Default_Handler, //SysTick_Handler
  Default_Handler, //WWDG_IRQHandler                   /* Window WatchDog              */
  Default_Handler, //PVD_VDDIO2_IRQHandler             /* PVD and VDDIO2 through EXTI Line detect */
  Default_Handler, //RTC_IRQHandler                    /* RTC through the EXTI line    */
  Default_Handler, //FLASH_IRQHandler                  /* FLASH                        */
  Default_Handler, //RCC_CRS_IRQHandler                /* RCC and CRS                  */
  Default_Handler, //EXTI0_1_IRQHandler                /* EXTI Line 0 and 1            */
  Default_Handler, //EXTI2_3_IRQHandler                /* EXTI Line 2 and 3            */
  Default_Handler, //EXTI4_15_IRQHandler               /* EXTI Line 4 to 15            */
  Default_Handler, //TSC_IRQHandler                    /* TSC                          */
  Default_Handler, //DMA1_Channel1_IRQHandler          /* DMA1 Channel 1               */
  Default_Handler, //DMA1_Channel2_3_IRQHandler        /* DMA1 Channel 2 and Channel 3 */
  Default_Handler, //DMA1_Channel4_5_6_7_IRQHandler    /* DMA1 Channel 4, Channel 5, Channel 6 and Channel 7*/
  Default_Handler, //ADC1_COMP_IRQHandler              /* ADC1, COMP1 and COMP2         */
  Default_Handler, //TIM1_BRK_UP_TRG_COM_IRQHandler    /* TIM1 Break, Update, Trigger and Commutation */
  Default_Handler, //TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */
  Default_Handler, //TIM2_IRQHandler                   /* TIM2                         */
  Default_Handler, 		                          	     /* TIM3                         */
  Default_Handler, //TIM6_DAC_IRQHandler               /* TIM6 and DAC                 */
  Default_Handler, //TIM7_IRQHandler                   /* TIM7                         */
  Default_Handler, //TIM14_IRQHandler                  /* TIM14                        */
  Default_Handler, //TIM15_IRQHandler                  /* TIM15                        */
  Default_Handler, //TIM16_IRQHandler                  /* TIM16                        */
  Default_Handler, //TIM17_IRQHandler                  /* TIM17                        */
  Default_Handler, //I2C1_IRQHandler                   /* I2C1                         */
  Default_Handler, //I2C2_IRQHandler                   /* I2C2                         */
  Default_Handler, //SPI1_IRQHandler                   /* SPI1                         */
  Default_Handler, //SPI2_IRQHandler                   /* SPI2                         */
  Default_Handler, //USART1_IRQHandler                 /* USART1                       */
  Default_Handler,                 			               /* USART2                       */
  Default_Handler, //USART3_4_IRQHandler               /* USART3 and USART4            */
  Default_Handler, //CEC_CAN_IRQHandler                /* CEC and CAN                  */
  Default_Handler  //USB_IRQHandler                    /* USB                          */
};


#pragma GCC pop_options
