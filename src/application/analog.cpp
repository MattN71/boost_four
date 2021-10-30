#include <stdint.h>
#include <stddef.h>

#include "analog.h"
#include "control.h"
#include "../hardware/hardware.h"

static volatile uint16_t adc_data[8] __attribute__ ((aligned));
static volatile uint16_t adc_data_buffered[8] __attribute__ ((aligned));

#include "lookup_tables.h"

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

//File-scope global peripheral objects
static ADC_TYPE ADC;
DMA_CH_TYPE DMA_ADC(CH1);
TIM15_TYPE TIM15;

void setup_adc(void) {
     
     //Setup ADC timer trigger

     TIM15.setTopValue(124);
     TIM15.setCompValue(TIM15_TYPE::CCR_CH1, 60);
     //TIM15.setupTriggerOut();
     TIM15.startTimer();
     //initPin(GPIO_B_BASE_ADDR, 14, GPIO_MODE_ALTERNATE, GPIO_DRIVE_PP, GPIO_RESISTOR_NONE, GPIO_SPEED_HIGH);
     //initPinAlt(GPIO_B_BASE_ADDR, 14, AF1);

     //Setup ADC peripheral

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


void ADC1_COMP_IRQHandler(void) {
     //End of conversion
     //*(ADC_BASE_ADDR) |= BIT2 | BIT3;
     //togglePinFast(GPIO_A_BASE_ADDR, 15); //CH1 on scope
}


void DMA1_Channel1_IRQHandler(void) {
    //End of transfer
    if (DMA_BASE_ADDR[0] & BIT1) {
        DMA_BASE_ADDR[1] |= BIT1; //Clear flag

        //Copy second half of DMA data to buffered memory
        for (size_t i = 4; i < 8; i++) {
            adc_data_buffered[i] = adc_data[i];
        }

        //Execute at 48kHz
        main_control_loop();
    }
    
    //Half transfer
    if (DMA_BASE_ADDR[0] & BIT2) {
        DMA_BASE_ADDR[1] |= BIT2; //Clear flag
                
        //Copy first half of DMA data to buffered memory
        for (size_t i = 0; i < 4; i++) {
            adc_data_buffered[i] = adc_data[i];
        }

    }
}


//Application-layer abstractions
uint16_t getPhase1Current(void) {
    return current_sense_map[adc_data_buffered[iPhase1]];
}


uint16_t getPhase2Current(void) {
    return current_sense_map[adc_data_buffered[iPhase2]];
}


uint16_t getPhase3Current(void) {
    return current_sense_map[adc_data_buffered[iPhase3]];
}


uint16_t getPhase4Current(void) {
    return current_sense_map[adc_data_buffered[iPhase4]];
}


uint16_t getOutputVoltage(void) {
    return voltage_sense_map[adc_data_buffered[vOut]];
}


uint16_t getInputVoltage(void) {
    return voltage_sense_map[adc_data_buffered[vIn]];
}


uint8_t getTemp(void) {
    return thermistor_map[(adc_data_buffered[thermistor] & 0x3FF)];
}