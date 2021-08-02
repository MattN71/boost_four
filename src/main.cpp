#include <stdint.h>
#include <stddef.h>
#include "gpio.h"
#include "DMA.h"
#include "RCC.h"
#include "device_offsets.h"
#include "timer2-3.h"
#include "timer16_17.h"
#include "usart.h"
#include "timer15.h"



void init_io(void) {

    // ******** Analog inputs ********
    //ADC_CH0 - ADC_CH7
    initPin(GPIO_A_BASE_ADDR, 0, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_A_BASE_ADDR, 1, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_A_BASE_ADDR, 2, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_A_BASE_ADDR, 3, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_A_BASE_ADDR, 4, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_A_BASE_ADDR, 5, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_A_BASE_ADDR, 6, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_A_BASE_ADDR, 7, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    //ADC_CH8 - ADC_CH9
    initPin(GPIO_B_BASE_ADDR, 0, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    initPin(GPIO_B_BASE_ADDR, 1, GPIO_MODE_ANALOG, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);


    // ******** PWM outputs ********
    //Phase 1
    initPin(GPIO_B_BASE_ADDR, 3, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    initPinAlt(GPIO_B_BASE_ADDR, 3, AF2);
    //Phase 2
    initPin(GPIO_B_BASE_ADDR, 5, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    initPinAlt(GPIO_B_BASE_ADDR, 5, AF1);
    //Phase 3
    initPin(GPIO_B_BASE_ADDR, 8, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    initPinAlt(GPIO_B_BASE_ADDR, 8, AF2);
    //Phase 4
    initPin(GPIO_B_BASE_ADDR, 9, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    initPinAlt(GPIO_B_BASE_ADDR, 9, AF2);


    // ******** Enable outputs ********
    //Phase 1
    initPin(GPIO_A_BASE_ADDR, 15, GPIO_MODE_OUTPUT, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    drivePin(GPIO_A_BASE_ADDR, 15, LOW);
    //Phase 2
    initPin(GPIO_B_BASE_ADDR, 4, GPIO_MODE_OUTPUT, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    drivePin(GPIO_B_BASE_ADDR, 4, LOW);
    //Phase 3
    initPin(GPIO_B_BASE_ADDR, 6, GPIO_MODE_OUTPUT, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    drivePin(GPIO_B_BASE_ADDR, 6, LOW);
    //Phase 4
    initPin(GPIO_B_BASE_ADDR, 7, GPIO_MODE_OUTPUT, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    drivePin(GPIO_B_BASE_ADDR, 7, LOW);

    // ******** General purpose digital outputs ********
    //Power good output
    initPin(GPIO_B_BASE_ADDR, 15, GPIO_MODE_OUTPUT, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_LOW);
    drivePin(GPIO_B_BASE_ADDR, 15, LOW);


    // ******** Serial port ********
    //TX
    initPin(GPIO_A_BASE_ADDR, 9, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    initPinAlt(GPIO_A_BASE_ADDR, 9, AF1);
    //RX
    initPin(GPIO_A_BASE_ADDR, 10, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
    initPinAlt(GPIO_A_BASE_ADDR, 10, AF1);
}


//Main function, section attribute makes sure it is at the beginning of .text
__attribute__((section (".entry_point"))) void main(void)  {

     RCC RCC1;

     //Setup system clock, peripheral clocks
	RCC1.init();

	//Setup GPIO for I/O and alternate functions
	init_io();

	//Setup timers for 4-phase interleaved PWM
	TIM2_3_TYPE TIM2(TIM2_BASE_ADDR);
     TIM2.setTopValue(120);
     TIM2.setCompValue(TIM2_3_TYPE::CCR_CH2, 30);
     TIM2.setupPWM();
     TIM2.startTimer();

     TIM2_3_TYPE TIM3(TIM3_BASE_ADDR);
     TIM3.setTopValue(120);
     TIM3.setCompValue(TIM2_3_TYPE::CCR_CH2, 30);
     TIM3.setupPWM();
     TIM3.startTimer();

     TIM16_17_TYPE TIM16(TIM16_BASE_ADDR);
     TIM16.setTopValue(120);
     TIM16.setCompValue(TIM16_17_TYPE::CCR_CH1, 30);
     TIM16.setupPWM();
     TIM16.startTimer();

     TIM16_17_TYPE TIM17(TIM17_BASE_ADDR);
     TIM17.setTopValue(120);
     TIM17.setCompValue(TIM16_17_TYPE::CCR_CH1, 30);
     TIM17.setupPWM();
     TIM17.startTimer();

     volatile uint32_t* tim2_cnt = TIM2.getCountReg();
     volatile uint32_t* tim3_cnt = TIM3.getCountReg();
     volatile uint32_t* tim16_cnt = TIM16.getCountReg();
     volatile uint32_t* tim17_cnt = TIM17.getCountReg();


     //Sync counters
     *tim2_cnt = 0;
     *tim3_cnt = (30 + 4);
     *tim16_cnt = (60 + 8);
     *tim17_cnt = (90 + 12);

     USART_TYPE UART1(USART_TYPE::USART1_PERIF, USART_TYPE::BAUD_2M);

     char message[] = "Hello World!";
     UART1.println(message);

     TIM15_TYPE TIM15;
     TIM15.setTopValue(125);
     TIM15.setCompValue(TIM15_TYPE::CCR_CH1, 10);
     TIM15.setupTriggerOut();
     TIM15.startTimer();
     initPin(GPIO_B_BASE_ADDR, 14, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
     initPinAlt(GPIO_B_BASE_ADDR, 14, AF1);
     volatile uint32_t* tim15_cnt = TIM15.getCountReg();

	//Infinite loop
     uint16_t comp_val = 30;
     uint16_t tim15_val = 0;
     char receieved = 'x';
	while(1) {

          tim15_val = *tim15_cnt;

          if (UART1.readAvailable()) {
               receieved = UART1.readChar();
               if (receieved == 'm' && comp_val > 0) {
                    comp_val -= 1;
               } else if (receieved == 'p' && comp_val <= 120) {
                    comp_val += 1;
               }
               TIM2.setCompValue(TIM2_3_TYPE::CCR_CH2, comp_val);
               TIM3.setCompValue(TIM2_3_TYPE::CCR_CH2, comp_val);
               TIM16.setCompValue(TIM16_17_TYPE::CCR_CH1, comp_val);
               TIM17.setCompValue(TIM16_17_TYPE::CCR_CH1, comp_val);
          }

     }
}



//Debugging code
/*

	//MCO on PA8
	initPin(GPIO_A_BASE_ADDR, 8, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
	initPinAlt(GPIO_A_BASE_ADDR, 8, AF0);


	//Drive P1_EN high
	initPin(GPIO_A_BASE_ADDR, 15, GPIO_MODE_OUTPUT, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
	drivePinFast(GPIO_A_BASE_ADDR, 15, true);

	//Pause for a couple seconds
	uint32_t j;
	for (uint32_t i = 0; i < 36000000; i++) {
		//Do nothing
		j = i;
	}



*/
