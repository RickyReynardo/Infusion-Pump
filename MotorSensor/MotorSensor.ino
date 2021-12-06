#include "HX711.h"
#define DOUT  A0
#define CLK  A1

int sensor = 4;
unsigned long start_time = 0;
unsigned long end_time = 0;
int steps=0;

HX711 scale;                                                                                                                                                                                  
float calibration_factor = 2230; // this calibration factor is adjusted according to my load cell
float units;

#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 1000
#include <AccelStepper.h>

AccelStepper stepper (1, 9, 8);
int MODE0 = 7;
int MODE1 = 6;
int MODE2 = 5;
int i=0;

void setup() {
 Serial.begin(9600);
 pinMode(sensor,INPUT);
 pinMode(7, OUTPUT);
 pinMode(5, OUTPUT); 
 scale.begin(DOUT,CLK);
 scale.set_scale(calibration_factor); //Adjust to this calibration factor
 scale.tare();
 stepper.setMaxSpeed(15000);
  stepper.setSpeed(12000);
   pinMode(MODE0, OUTPUT); 
   pinMode(MODE1, OUTPUT); 
   pinMode(MODE2, OUTPUT);


   //Motor is now running in Microstepping mode (1/16)

   digitalWrite(MODE0, LOW);
   digitalWrite(MODE1, LOW);
   digitalWrite(MODE2, LOW);
}
void loop() {
  units = scale.get_units(), 5;
  if (units < 0)
  {
   units = 0.00;
  }
  if(units >= 5){
   digitalWrite(7, HIGH);
   digitalWrite(5, LOW);
   motor();
   opto();
  }
  else{
   digitalWrite(7, LOW);
   digitalWrite(5, HIGH);
 }
 delay(200);
}

void motor(){
  if (i=0 || i < stepsPerRevolution) {
    stepper.runSpeed();
    i++;
    }

    if (i > stepsPerRevolution) {
    i = 0;
    }
}

void opto(){
  start_time=millis();
 end_time=start_time+1;
 while(millis()<end_time)
 {
   digitalWrite(5, HIGH);
   if(digitalRead(sensor))
   {
    steps=steps+1;
    digitalWrite(5, HIGH);
    while(digitalRead(sensor));
   }
   Serial.println(steps);
   Serial.print("   ");
  }
}
