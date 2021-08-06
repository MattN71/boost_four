#pragma once
#include <cstdint>
#include <cstddef>
#include "device_offsets.h"


class ADC_TYPE {
private:
     //Types
     enum reg_offsets {
          ISR = 0,
          IER = 1,
          CR = 2,
          CFGR1 = 3,
          CFGR2 = 4,
          SMPR = 5,
          TR = 8,
          CHSELR = 10,
          DR = 16,
          CCR = 194
     };

     //Variables
     volatile uint32_t *REG;

     //Methods

public:
     //Types
     enum ADC_TRIGGER_SOURCE_TYPE {
          ADC_TRG0,
          ADC_TRG1,
          ADC_TRG2,
          ADC_TRG3,
          ADC_TRG4
     };

     enum ADC_CONVERSION_MODE {
          ADC_SINGLE_MODE,
          ADC_CONTINUOUS_MODE,
          ADC_DISCONTINUOUS_MODE
     };

     enum ADC_TRIGGER_TYPE {
          ADC_RISING_EDGE,
          ADC_FALLING_EDGE,
          ADC_BOTH_EDGE,
          SOFTWARE
     };

     enum ADC_DMA_MODE_TYPE {
          ADC_DMA_CIRCULAR,
          ADC_DMA_ONE_SHOT,
          ADC_DMA_DISABLED
     };

     enum ADC_CLOCK_TYPE {
          ADC_ASYNC,
          ADC_SYNC_DIV2,
          ADC_SYNC_DIV4
     };

     enum ADC_SAMPLE_TIME_TYPE {
          ADC_14_CYCLE,
          ADC_20_CYCLE
     };

     enum ADC_INTERRUPT_TYPE {
          ADC_END_OF_CONVERSION,
          ADC_END_OF_SEQUENCE,
          ADC_OVERRUN
     };

     //Constructors
     ADC_TYPE(void);

     //Methods
     void enable(void);
     void disable(void);
     void startConversion(void);
     void stopConversion(void);
     void setTrigger(ADC_TRIGGER_SOURCE_TYPE source, ADC_TRIGGER_TYPE type);
     void setConversionMode(ADC_CONVERSION_MODE mode);
     void setDMAMode(ADC_DMA_MODE_TYPE mode);
     void setClockSource(ADC_CLOCK_TYPE clock);
     void setSampleTime(ADC_SAMPLE_TIME_TYPE cycles);
     void selectChannels(uint32_t channels);
     void enableInterrupt(ADC_INTERRUPT_TYPE interrupt, bool setting);

};
