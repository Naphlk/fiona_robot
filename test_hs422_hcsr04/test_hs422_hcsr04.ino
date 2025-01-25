#include <Servo.h>

Servo lservo; // First servo
Servo rservo; // Second servo

#define TRIG_PIN 5 
#define ECHO_PIN 4 

// Neutral angles for the servos
const int lneutral = 87;  // Neutral position for servo left
const int rneutral = 95;  // Neutral position for servo right
int lservoAngle = 0;
int rservoAngle = 0;
long distance = 0;

void setup() {
  lservo.attach(3);  // Attach lservo to pin 3
  rservo.attach(6); // Attach rservo to pin 6

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600); // Initialize serial communication
}

void loop() {

  // Trigger the sensor
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
    int angle = 8; // robot move forward Desired angle offset from neutral (e.g., 30° forward)
    int lcorrection = 4;  //robot move forward
    int rcorrection = -3; //robot move forward

  
    lservoAngle = lneutral - angle + lcorrection;       //reverse for lservo
    rservoAngle = rneutral + angle + rcorrection;       //forward for rservo

  } else {
  //robot move  backward
    int angle = 3;  //robot move backward
    int lcorrection = 1;  //robot move backward
    int rcorrection = -2; //robot move backward

  // Calculate actual servo positions - robot move backward
    lservoAngle = lneutral + angle + lcorrection;        // Forward for lservo
    rservoAngle = rneutral - angle + rcorrection;        // Reverse for rservo

  }

  

  // Write adjusted angles to servos
  lservo.write(lservoAngle);
  rservo.write(rservoAngle);

  delay(1000); // Wait for 1 second
}



// #include <Servo.h>

// Servo myservo; // Create servo object

// void setup() {
//   myservo.attach(6); // Attach servo to pin 3
// }

// void loop() {
//   myservo.write(96); // Move to 90 degrees (neutral position)-87 or 86 is likely good for left (see from back)
//                                                             //95 or 96 is likely good for right (see from back)
//   delay(2000);       // Wait 2 seconds

//   myservo.write(0);  // Move to 0 degrees
//   delay(2000);       // Wait 2 seconds

//   myservo.write(180); // Move to 180 degrees
//   delay(2000);        // Wait 2 seconds
// }


// #include <Servo.h>

// // Servo objects for left and right motors
// Servo lservo;
// Servo rservo;

// // Pin definitions
// const int rleg = 6;
// const int lleg = 3;

// int pos; // Variable to store servo position

// void setup() {
//   lservo.attach(lleg); // Attach left servo to pin 3
//   rservo.attach(rleg); // Attach right servo to pin 6
  
//   // Set initial positions to neutral (stop the servos)
//   lservo.write(86); 
//   rservo.write(96); 
// }

// void loop() {
//   // Ensure both servos are stopped at the beginning of the loop
//   lservo.write(86);
//   rservo.write(96);
//   delay(2000); // Wait for 3 seconds
  
//   // Move left and right wheels forward
//   //for (pos = 0; pos <= 86; pos++) 
//   {
//     lservo.write(79);
//     rservo.write(103);
//     delay(150); // Pause for a moment to make motion visible
//   }
//   delay(2000); // Wait for 1 second
  
//   // Keep left and right servo at neutral 
//   lservo.write(86);
//   rservo.write(96);

//   // // Move right servo from 0 to 180 degrees
//   // for (pos = 0; pos <= 180; pos++) {
//   //   rservo.write(pos);
//   //   delay(150); // Pause for a moment to make motion visible
//   // }
//   // delay(1000); // Wait for 1 second

//   // Keep right servo at neutral
//   rservo.write(96);
// }
