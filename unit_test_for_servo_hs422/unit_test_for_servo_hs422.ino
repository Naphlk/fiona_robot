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


#include <Servo.h>

// Servo objects for left and right motors
Servo lservo;
Servo rservo;

// Pin definitions
const int rleg = 6;
const int lleg = 3;

int pos; // Variable to store servo position

void setup() {
  lservo.attach(lleg); // Attach left servo to pin 3
  rservo.attach(rleg); // Attach right servo to pin 6
  
  // Set initial positions to neutral (stop the servos)
  lservo.write(86); 
  rservo.write(96); 
}

void loop() {
  // Ensure both servos are stopped at the beginning of the loop
  lservo.write(86);
  rservo.write(96);
  delay(3000); // Wait for 3 seconds
  
  // Move left servo from 0 to 180 degrees
  for (pos = 0; pos <= 180; pos++) {
    lservo.write(pos);
    delay(150); // Pause for a moment to make motion visible
  }
  delay(1000); // Wait for 1 second
  
  // Keep left servo at neutral 
  lservo.write(86);

  // Move right servo from 0 to 180 degrees
  for (pos = 0; pos <= 180; pos++) {
    rservo.write(pos);
    delay(150); // Pause for a moment to make motion visible
  }
  delay(1000); // Wait for 1 second

  // Keep right servo at neutral
  rservo.write(96);
}
