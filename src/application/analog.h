#pragma once

void setup_adc(void);


//Application-layer abstractions
uint16_t getPhase1Current(void);
uint16_t getPhase2Current(void);
uint16_t getPhase3Current(void);
uint16_t getPhase4Current(void);

uint16_t getOutputVoltage(void);
uint16_t getInputVoltage(void);

uint8_t getTemp(void);
