#include "DustSensor.h"

#include "application.h" //Provides Particle middleware support

DustSensor::DustSensor(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
    //if( digitalRead(pin) == HIGH){
    //    this.state = NO_DETECT;
    //}
    //else{
    //    this.state = DETECT;
    //}

    //State at construction    
    this->state = (pinReadFast(this->pin) == HIGH ? NO_DETECT : DETECT);
    this->count = 0;
    
    //attachInterrupt(D2, &DustSensor::handler, this, CHANGE, 13); //now done through a separate enable() function
}

void DustSensor::enable(){
    this->interval_start = micros();

    attachInterrupt(this->pin, &DustSensor::handler, this, CHANGE); 
}

void DustSensor::handler(){
    //uint8_t newstate = digitalRead(this->pin);
    auto newstate = (pinReadFast(this->pin) == HIGH ? NO_DETECT : DETECT);
    
    if(this->state == NO_DETECT){
        if( newstate == DETECT) { //leading edge of particle detection
            this->pulse_start = micros(); //to measure pulse width
        } else{ //Previous state was NO_DETECT, current state still NO_DETECT after a CHANGE interrupt
            //State error
            //TODO: figure out how to handle a state error
            //This could be caused by a very fast pulse
        }
    }
            
    if(this->state == DETECT) {
        if(newstate == NO_DETECT) {
            unsigned long pulse_time = micros() - pulse_start; //math should work even when micros timer wraps
            this->pulse_times[t_ind] = pulse_time;
            this->t_ind++;
            this->count++;
        } else{ //Previous state was DETECT, current state still DETECT after a CHANGE interrupt
            //State error
            //TODO: figure out how to handle a state error
            //This could be caused by a very fast pulse
        } 
    }
    
    this->state = newstate;
    
    digitalWrite(D7, newstate); //led2 off
    
}


uint32_t DustSensor::getCount(){
    return this->count;
}


void DustSensor::resetInterval(){ //note: slow function, since a long loop is done; maybe split into 2 functions?
    for(int i=0; i < t_ind; i++){
        pulse_times[i] = 0;
    }
    
    t_ind = 0;
    count = 0;
    interval_start = micros();
}

float DustSensor::getLPO(){
    
    //Sum all elements of pulse_times
    unsigned long acc = 0;
    for(int i=0; i < t_ind; i++){
        acc += pulse_times[i];
    }
    
    float interval_f = (float)(micros() - interval_start);
    //Serial.print("Working interval = ");
    //Serial.print(interval_f, 0);
    //Serial.println(" us");
    
    //Serial.print("Total pulse time = ");
    //Serial.print(acc);
    //Serial.println(" usec");
    
    //float value = (float)acc / (float)(micros() - this->interval_start);
    float value = (float)acc / interval_f;
    
    return value;
    
}