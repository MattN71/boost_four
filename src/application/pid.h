#pragma once

#include <cstdint>
#include <cstddef>


class PID_Controller {

private:
    //Types

    //Variables
    uint16_t setpoint;
    int32_t p_gain;
    int32_t i_gain;

    int32_t cum_error;
    int32_t cum_error_min;
    int32_t cum_error_max;

    //Methods

public:
    //Types

    //Variables

    //Constructors
    PID_Controller();

    //Methods
    int32_t execute(int32_t process_variable); 
    
    void set_setpoint(uint16_t new_setpoint);
    void set_p_gain(int32_t new_p_gain);
    void set_i_gain(int32_t new_i_gain);
    void reset_int_error(void);

};