#pragma once


void init_timers(void);
void sync_pwm_timers(void);
void e_stop(void);
void init_gpio(void);

//Application-layer abstractions
void setPhase1Duty(uint8_t duty);
void setPhase2Duty(uint8_t duty);
void setPhase3Duty(uint8_t duty);
void setPhase4Duty(uint8_t duty);

void setPhase1Enabled(bool en);
void setPhase2Enabled(bool en);
void setPhase3Enabled(bool en);
void setPhase4Enabled(bool en);