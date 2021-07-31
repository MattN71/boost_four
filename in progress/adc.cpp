static const uint16_t adc_data_mask = 0xFFF; //12 bit ADC resolution

static const enum {
    cur1 = 0, //Phase 1 current
    future1 = 1, //Reserved for future use
    heatsink_temp = 2, 
    cur2 = 3, // Phase 2 current
    future2 = 4, //Reserved for future use
    v_out = 5, // Output voltage
    cur3 = 6, // Phase 3 current
    future3 = 7, //Reserved for future use
    v_in = 8, //Input voltage
    cur4 = 9, // Phase 4 current
    future4 = 10, //Reserved for future use
    cur_out = 11, //Output current
} adc_data_index;

volatile uint16_t adc_data[12]; //Data copied from from ADC_DR register via DMA after each conversion


const uint16_t *adc_dma_dest = adc_data; //Destination address for DMA transfer
const uint16_t *adc_dma_source = ADC->DR; //Source address for DMA transfer?
const uint16_t adc_dma_len = 12; //Length of DMA transfer




//Need to accumulate for averaging
uint32_t heatsink_temp_sum;
uint32_t v_out_sum;
uint32_t v_in_sum;
uint32_t cur_out_sum;



struct pid_param_type { 
    // All gains are based on division by 32768 at end, i.e.
    // 32768 = gain of 1.0
    // 65536 = gain of 2.0
    // 10000 = gain of 0.305
    uint32_t p_gain; 
    uint32_t i_gain;
    uint32_t d_gain;
};

pid_param_type current_loop_params = {
    .p_gain = 32768,
    .i_gain = 32768,
    .d_gain = 32768
};

pid_param_type voltage_loop_params = {
    .p_gain = 32768,
    .i_gain = 32768,
    .d_gain = 32768
};





/* 

Main cpu loop:


48 Mhz / 48 kHz processing loop = 1000 clock cycles

1000 clock cycles:

    cur1 PID - 150 cycles
    cur2 PID - 150 cycles
    cur3 PID - 150 cycles
    cur4 PID - 150 cycles
    v_out PID - 150 cycles
_____________________________________ +
    total control loops = 750 cycles

    1000 - 750 = 250 cycles for housekeeping
    - Over temp monitoring
    - serial comms?


*/

/*  Current loop steps

    1. Read ADC current value - done automatically by DMA
    2. Calculate error value: Current setpoint - ADC reading. Integer subtraction - 1 cycle
    3. Calculate proportional error: 1 multiply and 1 right shift (division by multiple of 2) - 2 cycles
    4. Calculate integral error: 1 addition and 1 multiply + right shift by multiple of 2 - 3 cycles
    5. Calculate derivitive error: 1 subtraction, 1 multiply + right shift by multiple of 2 - 3 cycles
    6. Calculate P+I+D: 3 additions, 3 cycles
    7. Scale [0,4096] range to [0, 120]: Multiple time - 2 cycles
*/

/* Voltage loop steps




    9. Calculate total current setpoint */

static struct setpoint_type {
    uint32_t p1_current;
    uint32_t p2_current;
    uint32_t p3_current;
    uint32_t p4_current;
    uint32_t total_current;
    uint32_t output_voltage
} setpoints = {
    0,
    0,
    0,
    0,
    0,
    0
};


    uint32_t total_cur_setpoint = 8500;
    uint32_t cur_setpoint = total_cur_setpoint >> 2;

    //Calculate individual current setpoints, slightly unbalance phases to achieve better accuracy
    if (total_cur_setpoint & 0x1) { //i.e. if %4 is 1 or 3
        ch1_cur_setpoint = cur_setpoint + 1;
    } else {
        ch1_cur_setpoint = cur_setpoint;
    }

    if (total_cur_setpoint & 0x2) { //i.e. if %4 is 2 or 3
        ch2_cur_setpoint = cur_setpoint + 1;
        ch3_cur_setpoint = cur_setpoint + 1;
    } else {
        ch2_cur_setpoint = cur_setpoint;
        ch3_cur_setpoint = cur_setpoint;
    }

    ch4_cur_setpoint = cur_setpoint;

    







