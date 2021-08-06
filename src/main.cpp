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
#include "adc.h"

extern const uint8_t thermistor_map[];




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
     char fault_message[] = "ADC Overrun!";


     TIM15_TYPE TIM15;
     TIM15.setTopValue(124);
     TIM15.setCompValue(TIM15_TYPE::CCR_CH1, 60);
     TIM15.setupTriggerOut();
     TIM15.startTimer();
     initPin(GPIO_B_BASE_ADDR, 14, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
     initPinAlt(GPIO_B_BASE_ADDR, 14, AF1);


     ADC_TYPE ADC;
     ADC.selectChannels(0xFF); //CH0-CH7
     ADC.setConversionMode(ADC_TYPE::ADC_DISCONTINUOUS_MODE);
     ADC.setTrigger(ADC_TYPE::ADC_TRG4, ADC_TYPE::ADC_RISING_EDGE);
     ADC.setClockSource(ADC_TYPE::ADC_SYNC_DIV4);
     ADC.setSampleTime(ADC_TYPE::ADC_20_CYCLE);
     ADC.setDMAMode(ADC_TYPE::ADC_DMA_CIRCULAR);
     //ADC.enableInterrupt(ADC_TYPE::ADC_END_OF_CONVERSION, true);
     //ADC.enableInterrupt(ADC_TYPE::ADC_END_OF_SEQUENCE, true);

     //Enable ADC interrupt in NVIC-ISER register
     //*(NVIC_BASE_ADDR) |= BIT12;

     //Enable DMA-CH1 interrupt in NVIC-ISER register
     *(NVIC_BASE_ADDR) |= BIT9;

     volatile uint16_t adc_data[8]; //Getting optimized out?
     enum adc_data_index {
         iPhase1 = 0,
         thermistor = 1,
         iPhase2 = 2,
         vIn = 3,
         iPhase3 = 4,
         vOut = 5,
         iPhase4 = 6,
         iOut = 7
     };


     //const uint16_t adc_data_mask = 0xFFF; //12-bits


     DMA_CH_TYPE DMA_ADC(CH1);
     DMA_ADC.setPriority(PRIORITY_VERY_HIGH);
     DMA_ADC.setTransferDirection(PERIF_TO_MEM);
     DMA_ADC.setPerifSize(SIZE_16_BIT);
     DMA_ADC.setMemSize(SIZE_16_BIT);
     DMA_ADC.setMemIncrement(true);
     DMA_ADC.setPerifIncrement(false);
     DMA_ADC.setCircularMode(true);
     DMA_ADC.setErrorInterrupt(false);
     DMA_ADC.setMemAddress((uint32_t)adc_data);
     DMA_ADC.setPerifAddress(0x40012440); //ADC base + ADC_DR offset
     DMA_ADC.setNumberTransfers(8);
     DMA_ADC.enableTransferCompleteInterrupt(true);
     DMA_ADC.enableChannel();


     ADC.enable();
     ADC.startConversion();

	//Infinite loop
     //uint16_t comp_val = 30;
     //char receieved = 'x';
     //volatile uint32_t adc_isr_status = 0;

     char start[] = "Temperature is: ";
     char end[] = " degrees C.";


	while(1) {


          //Detect ADC overrun
          if (*(ADC_BASE_ADDR) & BIT4) {
               *(ADC_BASE_ADDR) |= BIT4;
               UART1.println(fault_message);
          }

          //togglePinFast(volatile uint32_t *gpio_base, uint8_t pin);


          //Convert adc reading from 12 bit to 10 bit, then use lookup table to convert to celius
          uint8_t temp_c = thermistor_map[(adc_data[thermistor] >> 2)];


          UART1.print(start);
          UART1.sendNumAsASCII(temp_c);
          UART1.println(end);

          //Delay
          //for (uint32_t i = 0; i < 480000; i++) {}

     }
}



void ADC1_COMP_IRQHandler(void) {
     //End of conversion
     //*(ADC_BASE_ADDR) |= BIT2 | BIT3;
     //togglePinFast(GPIO_A_BASE_ADDR, 15); //CH1 on scope
}


void DMA1_Channel1_IRQHandler(void) {
     //End of transfer
     DMA_BASE_ADDR[1] |= BIT0;
     GPIO_A_BASE_ADDR[5] ^= (1 << 15);
}
