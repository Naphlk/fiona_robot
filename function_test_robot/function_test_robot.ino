#include <Servo.h>
#include <HCMotor.h>

//LK added this library 2025-01-18
/* Set the pin that will control the motor. Note that it doesn't have to be a PWM pin - 
   any digital pin will do! */
#define MOTOR_PIN 9
/* Create an instance of the library */
HCMotor HCMotor;

Servo lservo;
Servo rservo;

const int rpin = 6;
const int lpin = 3;

const int trig = 11;
const int echo = 12;

//const int fanpin = A1;//original
const int fanpin = 9;//LK added 2025-01-18
const int flameled = 8;

//int heatcap = 100; //original
int heatcap = 500; //LK added 2025-01-18
int distmin = 12;
int temppin = A0;

void setup() {

//LK added these lines of code 2025-01-19
 /* Initialise the library */
  HCMotor.Init();

  /* Attach motor 0 to digital pin 9. The first parameter specifies the 
     motor number, the second is the motor type, and the third is the 
     digital pin that will control the motor */
  HCMotor.attach(0, DCMOTOR, MOTOR_PIN);

  /* Set the duty cycle of the PWM signal in 100uS increments. 
     Here 100 x 100uS = 1mS duty cycle. */
  HCMotor.DutyCycle(0, 100);


  lservo.attach(lpin);
  rservo.attach(rpin);

  lservo.write(90);
  rservo.write(90);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(fanpin, OUTPUT);
  pinMode(flameled, OUTPUT);

  Serial.begin(9600);
}

void j() {
  long duration, cm;
  int distance = 0;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = microsecondsToCentimeters(duration);
  Serial.println(distance);
  delay(25);
}


long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void implement_motor(bool switch_status){
  long speed = 0;

  if (switch_status == 1){
    speed = map(500, 0, 1024, 0, 100);
  } else speed = map(0, 0, 1024, 0, 100);
  HCMotor.OnTime(0, speed);
}


void loop() {

  //LK added this line of code 2025-01-18
  Serial.println("start main loop");

  int distance[19];

  int sweepmilli = 3000;
  int maxsweep = 0;
  int heat = 0;
  //float temp = 0;//original 

  long duration, cm;
  int maxdistance = 0;
  int count = 0;

  delay(500);
  lservo.write(100);
  rservo.write(100);
  delay(sweepmilli / 2);

  lservo.write(90);
  rservo.write(90);
  delay(500);

  float smoothTemp = 0;//LK added 2025-01-18

  for (int i = 0; i <= 6; i++) {
    //temp = analogRead(temppin);//original
    //LK added 2025-01-18

    //Serial.println("start the first for loop \n");

    
    //Serial.print(temp);//original
    long average = 10;
    for (int n = 0; n < average; n++){//this for loop to avoid interference from ambiance
    smoothTemp += analogRead(temppin);
    //Serial.println(analogRead(temppin));
    //delay(500);
    }
    
    Serial.print("first smooth temperature ");
    Serial.println(smoothTemp);
    //Serial.print(" - ");
    //if (temp > heatcap) {//original
    if (smoothTemp > heatcap){//LK added 2025-01-18
      heat = 1;
      maxsweep = 0;
      Serial.println("Flame, flame, flame");//LK added 2025-01-18
      break;
    } else {
      if (i != 0) {
        lservo.write(80);
        rservo.write(80);
        delay(sweepmilli / 7);
      }
    }
    lservo.write(90);
    rservo.write(90);

    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    distance[count] = microsecondsToCentimeters(duration);

    delay(250);

    if ((distance[count] >= 5) && (distance[count] <= 300)) {
      Serial.print(i);
      Serial.print(". ");
      Serial.println(distance[count]);

      if ((distance[count] >= maxdistance)) {
        maxdistance = distance[count];
        maxsweep = sweepmilli - ((count + 1) * (sweepmilli / 7));
      }
    } else {
      Serial.println("INVALID, distance ???");
//LK added 2025-01-19
//      break;
    }

    count += 1;
    delay(500);

    if (i == 6) {
      lservo.write(100);
      rservo.write(100);
      delay(maxsweep + 125);
    }
  }

  lservo.write(90);
  rservo.write(90);

  delay(1000);

  lservo.write(83);
  rservo.write(99);
  int start = millis();

  int fan = 0;

  while (millis() <= (start + 5000)) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    int distancewhile = microsecondsToCentimeters(duration);

    //if ((heat == 1) or (temp > heatcap)) {//original
    if ((heat == 1) or (smoothTemp > heatcap)) {// LK added 2025-01-18
      Serial.println("HERE FAN COME");

      int hot = 0;
      delay(1000);
      lservo.write(100);
      rservo.write(100);
      delay(sweepmilli / 2);

      lservo.write(90);
      rservo.write(90);
      delay(500);
      //LK added 'why we have the second for loop???'
      for (int i = 0; i <= 6; i++) {
        // temp = analogRead(temppin);//original
        // Serial.println(temp);//original
        //LK added 2025-01-18

        Serial.print("the second for loop ");
        Serial.println(i);

        
        long average = 10;
        for (int n = 0; n < average; n++){//this for loop to avoid interference from ambiance
        smoothTemp += analogRead(temppin);
        //Serial.println(analogRead(temppin));
        //delay(500);
        }
        Serial.println(smoothTemp);

        //if (temp > hot) {
        if (smoothTemp > hot) {
          hot = i;
        }
        if (i != 0) {
          lservo.write(80);
          rservo.write(80);
          delay(sweepmilli / 7 + 50);
        }
        lservo.write(90);
        rservo.write(90);
        delay(500);
      }

      maxsweep = sweepmilli - ((hot - 1) * (sweepmilli / 7));

      lservo.write(100);
      rservo.write(100);
      delay(maxsweep);
      lservo.write(90);
      rservo.write(90);

      fan = 1;
      break;
    }

    if ((distancewhile < distmin) && (distancewhile >= 5)) {
      Serial.println("DISTANCE INVALID");
      Serial.println(distancewhile);
      start = -2500;
      delay(100);
    }
  }

  lservo.write(90);
  rservo.write(90);

  if (fan == 1) {
    Serial.println("FLAME GO GO GO");
    //analogWrite(fanpin, 150);//original
    //analogWrite(fanpin, 250);//LK added 2025-01-18
    digitalWrite(flameled, HIGH);

    
    
    //delay(10000);//original
    
    //analogWrite(fanpin, 0);//original
    
    //digitalWrite(flameled, LOW);

    
    

    //if (smoothTemp > heatcap){
      digitalWrite(flameled, HIGH);
      implement_motor(true);
      delay(6000);
      implement_motor(false);
      delay(1000);
      digitalWrite(flameled, LOW);

      fan = 0;
    //}

  }
}
