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
extern const uint16_t current_sense_map[];
extern const uint16_t voltage_sense_map[];


volatile uint16_t adc_data[8] __attribute__ ((aligned));
volatile uint16_t adc_data_buffered[8] __attribute__ ((aligned));

//Flags set during control loop processing 
volatile bool current_loop_running = false;
volatile bool voltage_loop_running = false;

//Number of phases active, bits 1-4 represent phases 1-4 (bit 0 not used)
volatile uint32_t active_phases = 0x0;

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


void init_timers(void) {
     //Setup timers for 4-phase interleaved PWM
     //Phase 1
	TIM2_3_TYPE TIM2(TIM2_BASE_ADDR);
     TIM2.setTopValue(120);
     TIM2.setCompValue(TIM2_3_TYPE::CCR_CH2, 0);
     TIM2.setupPWM();
     TIM2.startTimer();

     //Phase 2
     TIM2_3_TYPE TIM3(TIM3_BASE_ADDR);
     TIM3.setTopValue(120);
     TIM3.setCompValue(TIM2_3_TYPE::CCR_CH2, 0);
     TIM3.setupPWM();
     TIM3.startTimer();

     //Phase 3
     TIM16_17_TYPE TIM16(TIM16_BASE_ADDR);
     TIM16.setTopValue(120);
     TIM16.setCompValue(TIM16_17_TYPE::CCR_CH1, 0);
     TIM16.setupPWM();
     TIM16.startTimer();

     //Phase 4
     TIM16_17_TYPE TIM17(TIM17_BASE_ADDR);
     TIM17.setTopValue(120);
     TIM17.setCompValue(TIM16_17_TYPE::CCR_CH1, 0);
     TIM17.setupPWM();
     TIM17.startTimer();
}

void sync_pwm_timers(void) {
     //Sync timers for interleaved pwm 
     volatile uint32_t* tim2_cnt = TIM2.getCountReg();
     volatile uint32_t* tim3_cnt = TIM3.getCountReg();
     volatile uint32_t* tim16_cnt = TIM16.getCountReg();
     volatile uint32_t* tim17_cnt = TIM17.getCountReg();

     //Sync counters
     *tim2_cnt = 0;
     *tim3_cnt = (30 + 4);
     *tim16_cnt = (60 + 8);
     *tim17_cnt = (90 + 12);
}

void setup_adc(void) {
     
     //Setup ADC timer trigger
     TIM15_TYPE TIM15;
     TIM15.setTopValue(124);
     TIM15.setCompValue(TIM15_TYPE::CCR_CH1, 60);
     TIM15.setupTriggerOut();
     TIM15.startTimer();
     initPin(GPIO_B_BASE_ADDR, 14, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
     initPinAlt(GPIO_B_BASE_ADDR, 14, AF1);

     //Setup ADC peripheral
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

     //Setup DMA transfer
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
     DMA_ADC.enableHalfTransferInterrupt(true);
     DMA_ADC.enableChannel();

     //Enable DMA-CH1 interrupt in NVIC-ISER register
     *(NVIC_BASE_ADDR) |= BIT9;


     ADC.enable();
     ADC.startConversion();

}

void e_stop(void) {
     //Disable all phases
     drivePin(GPIO_A_BASE_ADDR, 15, LOW);
     drivePin(GPIO_B_BASE_ADDR, 4, LOW);
     drivePin(GPIO_B_BASE_ADDR, 6, LOW);
     drivePin(GPIO_B_BASE_ADDR, 7, LOW);

     //Drive power-good output low
     drivePin(GPIO_B_BASE_ADDR, 15, LOW);

     //Reset all PWM channel to 0% duty cycle
     TIM2.setCompValue(TIM2_3_TYPE::CCR_CH2, 0);
     TIM3.setCompValue(TIM2_3_TYPE::CCR_CH2, 0);
     TIM16.setCompValue(TIM16_17_TYPE::CCR_CH1, 0);
     TIM17.setCompValue(TIM16_17_TYPE::CCR_CH1, 0);

     UART1.println("Shutting down...");
}

//Main function, section attribute makes sure it is at the beginning of .text
__attribute__((section (".entry_point"))) void main(void)  {


     //Clock system class
     RCC RCC1;

     //Setup system clock, peripheral clocks
	RCC1.init();

     //Setup UART comms , 2 Mbaud
     USART_TYPE UART1(USART_TYPE::USART1_PERIF, USART_TYPE::BAUD_2M);

	//Setup GPIO for I/O and alternate functions
	UART1.println("Initializing I/O");
     init_io();

     //Initialize timers for PWM outputs
     UART1.println("Initializing timers");
     init_timers();

     //Synchronize timers for interleaved operation
     UART1.println("Synchronizing timers");     
     sync_pwm_timers();

     //Setup ADC 
     UART1.println("Initializing ADC and DMA");   
     setup_adc();
     




     //Main Processing loop
	while(1) {


          //Detect ADC overrun
          if (*(ADC_BASE_ADDR) & BIT4) {
               *(ADC_BASE_ADDR) |= BIT4;
               e_stop();
               UART1.println("ADC Overrun!");
          }

          //Detect over-voltage on output
          if (voltage_sense_map[adc_data[vOut]] > 32000) {
               e_stop();
               UART1.println("Over-voltage fault!");
          }

          //Detect over-currents
          if (current_sense_map[adc_data[iPhase1]] > 8000) {
               e_stop();
               UART1.println("Over-current fault on phase 1!");
          }

          if (current_sense_map[adc_data[iPhase2]] > 8000) {
               e_stop();
               UART1.println("Over-current fault on phase 2!");
          }

          if (current_sense_map[adc_data[iPhase3]] > 8000) {
               e_stop();
               UART1.println("Over-current fault on phase 3!");
          }

          if (current_sense_map[adc_data[iPhase4]] > 8000) {
               e_stop();
               UART1.println("Over-current fault on phase 4!");
          }


          if (UART1.readAvailable()) {
               static uint8_t new_comp = 0;
               char readin = UART1.readChar();
               if (readin == 'p') {
                    if (new_comp < 120) {
                         new_comp += 1;
                         TIM17.setCompValue(TIM16_17_TYPE::CCR_CH1, new_comp);
                    }
               } else if (readin == 'm') {
                    if (new_comp > 0) {
                         new_comp -= 1;
                         TIM17.setCompValue(TIM16_17_TYPE::CCR_CH1, new_comp);
                    }
               } else if (readin == 'd') {
                    drivePin(GPIO_B_BASE_ADDR, 7, LOW);
               } else if (readin == 'e') {
                    drivePin(GPIO_B_BASE_ADDR, 7, HIGH);
               }
          }




          //UART1.print(start);
          //UART1.sendNumAsASCII(temp_c);
          //UART1.println(end);

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
     static uint32_t transfer_counter = 0;

     if (DMA_BASE_ADDR[0] & BIT1) {
          //End of transfer
          DMA_BASE_ADDR[1] |= BIT1; //Clear flag
         
          //Make sure not still processing before we overwrite the buffered_adc_data
          //array used for the control loops
          if (current_loop_running || voltage_loop_running) {
               e_stop();
               UART1.println("Control loop in progress during adc refresh!");
          }

          for (size_t i = 4; i < 8; i++) {
               adc_data_buffered[i] = adc_data[i];
          }

          //Run current loop at 48 kHz, voltage loop at 6 kHz
          transfer_counter += 1;
          current_loop();
          if (transfer_counter & 0x7) {
               voltage_loop();
          }
     }

     if (DMA_BASE_ADDR[0] & BIT2) {
          //Half transfer
          DMA_BASE_ADDR[1] |= BIT2; //Clear flag
          
          //Make sure not still processing before we overwrite the buffered_adc_data
          //array used for the control loops
          if (current_loop_running || voltage_loop_running) {
               e_stop();
               UART1.println("Control loop in progress during adc refresh!");
          }
          
          for (size_t i = 0; i < 4; i++) {
               adc_data_buffered[i] = adc_data[i];
          }

     }
}

void current_loop(void) {
     if (active_phases & BIT1) {

     }

     if (active_phases & BIT2) {
          
     }
     
     if (active_phases & BIT3) {
          
     }
     
     if (active_phases & BIT4) {
          
     }
}

void voltage_loop(void) {
     
}

