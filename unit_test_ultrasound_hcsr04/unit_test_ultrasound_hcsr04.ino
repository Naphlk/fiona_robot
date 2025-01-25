#define TRIG_PIN 5 
#define ECHO_PIN 4 

void setup() {
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
    long distance = duration / 58; // 58 µs per cm

    // Print the result
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(2000); // Delay for stability
      
  } else {
    Serial.println("out of range");
  }

  
}




// #include <HCSR04.h>
// //these lines of code are showing the distance around 131 , sometimes 0, 70, 100
// HCSR04 hc(7, 4); //initialisation class HCSR04 (trig pin , echo pin)

// void setup()
// {
//     Serial.begin(9600);
// }

// void loop()
// {
//     Serial.println(hc.dist()); // return curent distance in serial
//     delay(600);                 // we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.
// }
/*
these lines of code are not correct and show 133 cm with every distance. Failed
// Initialize sensor that uses digital pins 13 and 12.
const byte triggerPin = 7;
const byte echoPin = 4;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

void setup () {
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
}

void loop () {
    // Every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters.
    float distance = distanceSensor.measureDistanceCm();
    Serial.print("distance is (cm) ");
    Serial.println(distance);
    delay(1000);
}
*/