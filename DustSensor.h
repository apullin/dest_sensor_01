#ifndef DUST_SENSOR_H
#define DUST_SENSOR_H

#include "application.h"

typedef enum states {NO_DETECT, DETECT} sensor_state_t;

#define PULSE_BUFFER_LEN 1000 //TODO: figure out actual max/min size, and appropriate size

class DustSensor {
  public:
    //Constructor
    DustSensor(int pin);
    void handler(); //ISR handler, REQUIRES safe code
    void enable();
    
    uint32_t getCount();
    float getLPO();
    void resetInterval();
    
    private:
        int pin;
        uint32_t count;
        sensor_state_t state;
        unsigned long pulse_times[PULSE_BUFFER_LEN] = {0}; //Caution: could be optimized away
        unsigned int t_ind = 0;
        unsigned long pulse_start = 0;
        unsigned long interval_start;
};

#endif