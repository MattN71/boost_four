#include "DMA.h"

void dma_init(void) {

    uint16_t adc_data[8];
    
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

    const uint16_t adc_data_mask = 0xFFF; //12-bits

    DMA_CH_TYPE DMA_ADC(CH1);
    
    DMA_ADC.setPriority(PRIORITY_VERY_HIGH);
    DMA_ADC.setTransferDirection(PERIF_TO_MEM);
    DMA_ADC.setPerifSize(SIZE_16_BIT);
    DMA_ADC.setMemSize(SIZE_16_BIT);
    DMA_ADC.setMemIncrement(true);
    DMA_ADC.setPerifIncrement(false);
    DMA_ADC.setCircularMode(true);
    DMA_ADC.setErrorInterrupt(false);
    DMA_ADC.setMemAddress(uint32_t)adc_data);
    DMA_ADC.setPerifAddress((uint32_t)0x40012440); //ADC base + ADC_DR offset
    DMA_ADC.setNumberTransfers(8);   
    DMA_ADC.enableChannel();


    while (1) {
        if (DMA_ADC.checkErrorIntFlag()) {
            DMA_ADC.disableChannel();
            shutdown();
        }
    }

}


void fault_shutdown() {
    digitalwrite(power_good, LOW);
    phase1.disable();
    phase2.disable();
    phase3.disable();
    phase4.disable();
    Serial.log("powering off\n");
}