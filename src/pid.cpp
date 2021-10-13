#include "pid.h"
#include "helper.h"

PID_Controller::PID_Controller() {
    setpoint = 0;
    p_gain = 0;
    i_gain = 0;
    cum_error = 0;
    cum_error_max = 100;
    cum_error_min = -100;
}


int32_t PID_Controller::execute(int32_t process_variable) {
    int32_t error_value = setpoint - process_variable;
    
    cum_error += error_value;
    cum_error = constrain_num(cum_error, cum_error_min, cum_error_max);
    
    int32_t p_error = error_value * p_gain;
    int32_t i_error = cum_error * i_gain;

    return (p_error + i_error);
}


void PID_Controller::set_setpoint(uint16_t new_setpoint) {
    setpoint = new_setpoint;
}


void PID_Controller::set_p_gain(int32_t new_p_gain) {
    p_gain = new_p_gain;
}


void PID_Controller::set_i_gain(int32_t new_i_gain) {
    i_gain = new_i_gain;
}


void PID_Controller::reset_int_error(void) {
    cum_error = 0;
}