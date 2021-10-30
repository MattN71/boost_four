#include <stdint.h>
#include <stddef.h>

#include "digital.h"
#include "../hardware/hardware.h"


//File-scope global peripheral objects
static TIM2_3_TYPE TIM2(TIM2_BASE_ADDR); //Phase 1
static TIM2_3_TYPE TIM3(TIM3_BASE_ADDR); //Phase 2
static TIM16_17_TYPE TIM16(TIM16_BASE_ADDR); //Phase 3
static TIM16_17_TYPE TIM17(TIM17_BASE_ADDR); //Phase 4


void init_timers(void) {
    //Setup timers for 4-phase interleaved PWM
    //Phase 1
    TIM2.setTopValue(120);
    TIM2.setCompValue(TIM2_3_TYPE::CCR_CH2, 0);
    TIM2.setupPWM();
    TIM2.startTimer();

    //Phase 2
    TIM3.setTopValue(120);
    TIM3.setCompValue(TIM2_3_TYPE::CCR_CH2, 0);
    TIM3.setupPWM();
    TIM3.startTimer();

    //Phase 3
    TIM16.setTopValue(120);
    TIM16.setCompValue(TIM16_17_TYPE::CCR_CH1, 0);
    TIM16.setupPWM();
    TIM16.startTimer();

    //Phase 4
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

     //UART1.println("Shutting down...");
}

void init_gpio(void) {

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



//Application-layer abstractions
void setPhase1Duty(uint8_t duty) {
     TIM2.setCompValue(TIM2_3_TYPE::CCR_CH2, duty);
}


void setPhase2Duty(uint8_t duty) {
     TIM3.setCompValue(TIM2_3_TYPE::CCR_CH2, duty);
}


void setPhase3Duty(uint8_t duty) {
     TIM16.setCompValue(TIM16_17_TYPE::CCR_CH1, duty);
}


void setPhase4Duty(uint8_t duty) {
     TIM17.setCompValue(TIM16_17_TYPE::CCR_CH1, duty);
}


void setPhase1Enabled(bool en) {
     drivePin(GPIO_A_BASE_ADDR, 15, en);
}


void setPhase2Enabled(bool en) {
     drivePin(GPIO_B_BASE_ADDR, 4, en);
}


void setPhase3Enabled(bool en) {
     drivePin(GPIO_B_BASE_ADDR, 6, en);
}


void setPhase4Enabled(bool en) {
     drivePin(GPIO_B_BASE_ADDR, 7, en);
}