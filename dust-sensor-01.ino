// This #include statement was automatically added by the Particle IDE.
#include "DustSensor.h"

#include "math.h"

#define PIN_PM2p5 D2
#define PIN_PM10  D3

DustSensor sensor2p5(PIN_PM2p5);
DustSensor sensor10(PIN_PM10);

//Software timer for publishing results
#define PUBLISH_INTERVAL_MS 20000 //note: might be too frequent for librato free version to accept
Timer publish_timer(PUBLISH_INTERVAL_MS, publish_sensor);

//LED for indication
int led2 = D7;
bool LED_RED = 0;
bool LED_GREEN = 0;
bool LED_BLUE = 0; 

void publish_sensor()
{
    //Serial.println("Starting timed publish ...");
    //Serial.print("millis = "); Serial.println(millis());
    
    //TODO: figure out how to publish composite values? Single publish w/ many data?
    
    //Publishing will carry some sense of time along with it; unclear if device timestamped
    //  or reception (server) timestamped.
    
    //Serial.println("Getting LPO's ...");
    float sensor2p5_lpo = sensor2p5.getLPO(); //slow, sums pulse_times array
    float sensor10_lpo = sensor10.getLPO();   //slow, sums pulse_times array
    //Serial.print("PM2.5 : "); Serial.println(sensor2p5_lpo_last, 4);
    //Serial.print("PM10 : ");  Serial.println(sensor10_lpo_last, 4);
    
    //Serial.println("Getting counts ...");
    uint32_t sensor2p5_count = sensor2p5.getCount();
    uint32_t sensor10_count = sensor10.getCount();
    //Serial.print("Count 2.5 : "); Serial.println(sensor2p5_count);
    //Serial.print("Count 10 : ");  Serial.println(sensor10_count);
    
    //Serial.print("Doing web publish ...");
    //Low Pulse Occupancy
    //Particle.publish("librato_pm25_lpo", String( sensor2p5_lpo_last ), 5, PRIVATE);
    //Particle.publish("librato_pm10_lpo", String( sensor10_lpo_last  ), 5, PRIVATE);
    //Serial.println(" done.");
    
    //Newer style single publish with multiple data
    //Firmware:
    Particle.publish("evt_periodic_sensor_data", 
    	"{ \"val_pm25_LPO\" :"   + String(sensor2p5_lpo) + 
    	", \"val_pm10_LPO\" :"   + String(sensor10_lpo) +
    	", \"val_pm25_count\" :" + String(sensor2p5_count) +
    	", \"val_pm10_count\" :" + String(sensor10_count) +
    	" }" ,
    	60, PRIVATE);
    
    //Total pulse counts
    //Particle.publish("librato_pm25_count", String( sensor2p5.getCount() ), 60, PRIVATE);
    //Particle.publish("librato_pm10_count", String( sensor10.getCount() ), 60, PRIVATE);
    
    //Serial.print("Resetting interval ...");
    sensor2p5.resetInterval(); //slow, zeros pulse_times array
    sensor10.resetInterval();  //slow, zeros pulse_times array
    //Serial.println(" done.");
    //Serial.println("-----------------------------------");
    //Serial.println("");
    
    /*if(LED_GREEN){
        RGB.color(0, 24, 0);
        LED_GREEN = 0;
    } else{
        RGB.color(0, 0, 0);
        LED_GREEN = 1;
    }*/

    //Update onboard LED
    setLED( sensor2p5_lpo , sensor10_lpo);
}


void setup() {
    
    //LED, starts off
    pinMode(led2, OUTPUT);
    digitalWrite(led2, HIGH); //led2 on

    //White LED to indicate pre-serial startup state
    RGB.control(true); //take control of RGB led
    RGB.color(255,255,255);
    RGB.brightness(24);
    
    //Serial.begin(9600);   // open serial over USB
    // On Windows it will be necessary to implement the following line:
    // Make sure your Serial Terminal app is closed before powering your device
    // Now open your Serial Terminal, and hit any key to continue!
    //while(!Serial.available()) Particle.process();
    
    //Serial.println("Starting!"); Serial.println();
    
    RGB.color(0,0,0);
    RGB.brightness(255);
    digitalWrite(led2, LOW); //led2 off
    
    sensor2p5.enable(); //turns on pin interrupt, zeros usec interval timer
    sensor10.enable();  //turns on pin interrupt, zeros usec interval timer
    
    publish_timer.start();
}

void loop() {
   //Count detection is done in ISR
   //Publish is done in "software" timer
   
}

//Set LED color based on "badnes" of the air quality; this is very arbitrary.
//From the datasheet, the highest LPO should be 25%, which is assumed for the scaling.
void setLED(float sense1, float sense2){
    
    int red = ceil(255*(sense1/0.25f));
    int blue= ceil(255*(sense2/0.25f));
    
    if(red > 255)  red  = 255 ;
    if(blue > 255) blue = 255 ;
    
    RGB.color(red,0,blue);
    
    if((sense1 > 0.2) && (sense2 > 0.2)){
        RGB.brightness(128); //high brightness for bad conditions
    } else{
        RGB.brightness(32); //half brightness otherwise
    }
    
}