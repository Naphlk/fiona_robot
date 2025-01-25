/* Include the library */
#include "HCMotor.h"

/* Set the pin that will control the motor. Note that it doesn't have to be a PWM pin - 
   any digital pin will do! */
#define MOTOR_PIN 9

/* Set the analogue pin the potentiometer will be connected to. */
#define POT_PIN A0

/* Create an instance of the library */
HCMotor HCMotor;


void setup() 
{
  /* Initialise the library */
  HCMotor.Init();

  /* Attach motor 0 to digital pin 7. The first parameter specifies the 
     motor number, the second is the motor type, and the third is the 
     digital pin that will control the motor */
  HCMotor.attach(0, DCMOTOR, MOTOR_PIN);

  /* Set the duty cycle of the PWM signal in 100uS increments. 
     Here 100 x 100uS = 1mS duty cycle. */
  HCMotor.DutyCycle(0, 100);
  Serial.begin(9600);
}



void loop() 
{
  long Speed = 0;
  Serial.println("start scan motor");


  /* Read the analogue pin to determine the position of the pot. The map 
     function takes this value which could be anywhere between 0 - 1024 
     and reduces it down to match the duty cycle range of 0 - 100 */ 
  //Speed = map(analogRead(POT_PIN), 0, 1024, 0, 100);
  for (long m = 0; m < 1024; m += 200){
    Speed = map(m, 0, 1024, 0, 100);
  
  /* Set the on time of the duty cycle to match the position of the pot. */
    HCMotor.OnTime(0, Speed);
    
    Serial.println(Speed);
    delay(3000);
  }
  
}