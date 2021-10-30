#include <stdint.h>
#include <stddef.h>

#include "control.h"
#include "pid.h"
#include "analog.h"
#include "digital.h"

//PID controller objects
static PID_Controller phase_1_pid;
//static PID_Controller phase_2_pid;
//static PID_Controller phase_3_pid;
//static PID_Controller phase_4_pid;
//static PID_Controller voltage_pid;

/*
//Global flags
volatile uint32_t active_controllers = 0;

static const uint32_t PHASE1 = BIT1;
static const uint32_t PHASE2 = BIT2;
static const uint32_t PHASE3 = BIT3;
static const uint32_t PHASE4 = BIT4;
static const uint32_t VOLTAGE_CNTRL = BIT5;
*/

void main_control_loop(void) {
    static uint32_t loop_counter = 0;



    loop_counter += 1;
}


