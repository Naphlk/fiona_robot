#include <Servo.h>
#include <HCMotor.h> //this library implement some logic algorith that create interference with servo motors-2025-01-29

Servo lservo; // left wheel - section view from back of robot
Servo rservo; // right wheel - section view from back of robot

#define TRIG_PIN 5 
#define ECHO_PIN 4 
/* Set the pin that will control the motor. Note that it doesn't have to be a PWM pin - 
   any digital pin will do! */
#define MOTOR_PIN 3

/* Create an instance of the library */
//HCMotor HCMotor;

const int fanpin = 3;//LK added 2025-01-18
const int flameled = 2;

int heatcap = 500; //LK added 2025-01-18
int distmin = 12;
int temppin = A0;

// Neutral angles for the servos
const int lneutral = 86;  // Neutral position for servo left
const int rneutral = 95;  // Neutral position for servo right
int lservoAngle = 0;
int rservoAngle = 0;
long distance = 0;

long smoothTemp = 0;
long average = 5;
int start_and_check = 0;


void setup() {
  lservo.attach(12);  // Attach lservo to pin 12
  rservo.attach(13); // Attach rservo to pin 13

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  /* Initialise the library */
  //HCMotor.Init();

  /* Attach motor 0 to digital pin 9. The first parameter specifies the 
     motor number, the second is the motor type, and the third is the 
     digital pin that will control the motor */
  //HCMotor.attach(0, DCMOTOR, MOTOR_PIN);

  /* Set the duty cycle of the PWM signal in 100uS increments. 
     Here 100 x 100uS = 1mS duty cycle. */
  //HCMotor.DutyCycle(0, 100);

  pinMode(fanpin, OUTPUT);
  pinMode(flameled, OUTPUT);

  // Configure Timer2 for Fast PWM (Inverted Mode)
    TCCR2A = (1 << COM2B1) | (1 << COM2B0) | (1 << WGM20) | (1 << WGM21);
    TCCR2B = (1 << CS21);  // Prescaler = 8 (Adjust for speed)

    OCR2B = 255;  // Start with motor off (full inversion)

  
  Serial.begin(9600); // Initialize serial communication
}

/*
void implement_motor(bool switch_status){
  long speed = 0;

  if (switch_status == 1){
    speed = map(400, 0, 1024, 0, 100);
  } else speed = map(0, 0, 1024, 0, 100);
  HCMotor.OnTime(0, speed);
}
*/

void smoothServoMovement(int targetLAngle, int targetRAngle, int step, int delayBetweenSteps) {
  //int step = 1; // Adjust this value for smoother or faster transitions
  //int delayBetweenSteps = 10; // Delay in milliseconds between angle changes

  // Gradually adjust the left servo
  while (lservoAngle != targetLAngle) {
    if (lservoAngle < targetLAngle) lservoAngle += step;
    else if (lservoAngle > targetLAngle) lservoAngle -= step;

    lservo.write(lservoAngle);
    delay(delayBetweenSteps);
  }

  // Gradually adjust the right servo
  while (rservoAngle != targetRAngle) {
    if (rservoAngle < targetRAngle) rservoAngle += step;
    else if (rservoAngle > targetRAngle) rservoAngle -= step;

    rservo.write(rservoAngle);
    delay(delayBetweenSteps);
  }
}


void loop() {

  if (start_and_check == 0){ //implement checking servos and fan and led at the first loop
    //digitalWrite(flameled, LOW);

    //implement_motor(true);
    //analogWrite(fanpin, 150);

    lservo.write(lneutral-10);
    delay(100);
    rservo.write(rneutral+10);
    //smoothServoMovement(lneutral-5, rneutral+8, 1, 2);
    delay(2000);

    // lservo.write(lneutral+30);
    // rservo.write(rneutral-30);
    //smoothServoMovement(lneutral+30, rneutral-30);
    //delay(3000);

     lservo.write(lneutral);
     delay(100);
     rservo.write(rneutral);
    // delay(2000);
    //smoothServoMovement(lneutral, rneutral, 1, 2);

    //implement_motor(false);
    //analogWrite(fanpin, 0);


    digitalWrite(flameled, LOW);
    //OCR2B = 100; // 100/255 (~40% speed)
    //delay(2000);
    OCR2B = 200; // 200/255 (~80% speed)
    delay(2000);
    OCR2B = 255; // Stop motor (inverted mode)
    //delay(2000);


    digitalWrite(flameled, HIGH);
    start_and_check = 1;//complete the first loop
  } else { //implement the main logic at the second loop
      
      /*
        this section of code is for checking flame and on/off fan
      */
      for (int n = 0; n < average; n++){//this 'for' loop to avoid interference from ambiance
      smoothTemp += analogRead(temppin);
      //Serial.println(analogRead(temppin));
      //delay(500);
      }

      smoothTemp = smoothTemp / average;

      if (smoothTemp > 200){

        digitalWrite(flameled, LOW);
        
        //implement_motor(true);//detect something abnormal with HCmotor library 2025-01-29
        //analogWrite(fanpin, 250);//LK added 2025-01-18
        OCR2B = 50; // 50/255 (~25% speed)
        delay(200);
        

        //we will stop two servos and reducing EMI from servos to the robot
        lservo.write(lneutral+1);
        delay(200);
        rservo.write(rneutral-1);
        delay(200);
        lservo.write(lneutral);
        delay(200);
        rservo.write(rneutral);
      
        //smoothServoMovement(lneutral, rneutral, 1, 1);
        //lservo.detach();  // detach lservo to pin 12
        //rservo.detach(); // detach rservo to pin 13
        
        //lservo.write(lservo.read());  // Hold the current position
        //rservo.write(rservo.read());

        //lservo.writeMicroseconds(1500);  // Stop left servo
        //rservo.writeMicroseconds(1500);  // Stop right servo

        
        //delay(100);

        //implement_motor(false);//detect something abnormal with HCmotor library 2025-01-29
        //analogWrite(fanpin, 0);//LK added 2025-01-18
        //delay(3000);
        OCR2B = 100; // 100/255 (~40% speed)
        delay(2000);
        OCR2B = 200; // 200/255 (~80% speed)
        delay(2000);
        OCR2B = 255; // Stop motor (inverted mode)
        delay(100);

        //lservo.attach(12);  // Attach lservo to pin 12
        //rservo.attach(13); // Attach rservo to pin 13

        // Write adjusted angles to servos
        lservo.write(lservoAngle);
        rservo.write(rservoAngle);
      } else {
        //this section of code for scanning obstructive objects
        digitalWrite(flameled, HIGH);

        // Trigger the ultrasonic sensor
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        // Measure the echo duration
        long duration = pulseIn(ECHO_PIN, HIGH);
        if (duration != 0){
          // Calculate the distance in cm
          distance = duration / 58; // 58 µs per cm
        }

        if (distance > 10) {
        //robot move forward
          int angle = 5; 
          int lcorrection = 2;  //robot move forward
          int rcorrection = 0; //robot move forward
        
          lservoAngle = lneutral - angle + lcorrection;       //reverse for lservo
          rservoAngle = rneutral + angle + rcorrection;       //forward for rservo

        } else {
        //robot move  backward
          int angle = 5;  //robot move backward
          int lcorrection = 1;  //robot move backward
          int rcorrection = -2; //robot move backward

        // Calculate actual servo positions - robot move backward
          lservoAngle = lneutral + angle + lcorrection;        // Forward for lservo
          rservoAngle = rneutral - angle + rcorrection;        // Reverse for rservo

        }

        // Write adjusted angles to servos
        lservo.write(lservoAngle);
        rservo.write(rservoAngle);
        //smoothServoMovement(lservoAngle, rservoAngle);


    ////delay(1000); // Wait for 1 second

      }
  
    //delay(1000);
    start_and_check = 1;
  }
}

