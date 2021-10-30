#include <stdint.h>
#include <stddef.h>
#include "pid.h"
#include "digital.h"
#include "analog.h"
#include "../hardware/hardware.h"

#define OVER_CUR_LIMIT 1000
#define OVER_VOL_LIMIT 15000



//Set points for control loops
volatile uint32_t current_setpoint = 500;
volatile uint32_t voltage_setpoint = 30000;


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
     init_gpio();

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
          if (getOutputVoltage() >= OVER_VOL_LIMIT) {
               e_stop();
               UART1.println("Over-voltage fault!");
          }

          //Detect over-currents
          if (getPhase1Current() >= OVER_CUR_LIMIT) {
               e_stop();
               UART1.println("Over-current fault on phase 1!");
          }

          if (getPhase2Current() >= OVER_CUR_LIMIT) {
               e_stop();
               UART1.println("Over-current fault on phase 2!");
          }

          if (getPhase3Current() >= OVER_CUR_LIMIT) {
               e_stop();
               UART1.println("Over-current fault on phase 3!");
          }

          if (getPhase4Current() >= OVER_CUR_LIMIT) {
               e_stop();
               UART1.println("Over-current fault on phase 4!");
          }

          if (UART1.readAvailable()) {
               static uint8_t new_comp = 0;
               char readin = UART1.readChar();
               if (readin == 'p') {
                    if (new_comp < 120) {
                         new_comp += 1;
                         setPhase1Duty(new_comp);
                    }
               } else if (readin == 'm') {
                    if (new_comp > 0) {
                         new_comp -= 1;
                         setPhase1Duty(new_comp);
                    }
               } else if (readin == 'd') {
                    setPhase1Enabled(false);
               } else if (readin == 'e') {
                    setPhase1Enabled(true);
               }
          }
     }
}