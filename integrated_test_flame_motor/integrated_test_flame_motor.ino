#include <Servo.h>
#include <HCMotor.h>

//LK added this library 2025-01-19
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
int heatcap = 150; //LK added 2025-01-18
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

//original
  // lservo.attach(lpin);
  // rservo.attach(rpin);


//original
  // lservo.write(90);
  // rservo.write(90);

  //LK added 2025-01-19 -> left and right wheels are turning in reverse directions
  // lservo.write(0);
  // rservo.write(180);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(fanpin, OUTPUT);
  pinMode(flameled, OUTPUT);

  Serial.begin(9600);
}

void implement_motor(bool switch_status){
  long speed = 0;

  if (switch_status == 1){
    speed = map(500, 0, 1024, 0, 100);
  } else speed = map(0, 0, 1024, 0, 100);
  HCMotor.OnTime(0, speed);
}

/*
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
*/
void loop() {

  //bool logic = 0;
  //LK added this line of code 2025-01-18
  int start = 0;
  digitalWrite(flameled, LOW);
  Serial.println("start new main loop");
  // Serial.println(("scan motor and flame sensor"));
  // implement_motor(true);
  // delay(3000);
  // implement_motor(false);
  // //delay(3000);

  //int Speed = 0;

  /* The map function takes the value which could be anywhere between 0 - 1024 
     and reduces it down to match the duty cycle range of 0 - 100 */ 
  //for (int m = 0; m < 1024; m += 200){
  //  Speed = map(m, 0, 1024, 0, 100);
  
  /* Set the on time of the duty cycle to match the position of the pot. */
  //  HCMotor.OnTime(0, Speed);
  //  Serial.println(Speed);
  //  delay(2000);
  //}
  
  long smoothTemp = 0;
  long average = 10;
  for (int n = 0; n < average; n++){//this for loop to avoid interference from ambiance
    smoothTemp += analogRead(temppin);
    //Serial.println(analogRead(temppin));
    //delay(500);
  }

  smoothTemp = smoothTemp / average;
  Serial.println("value of smooth temperature ");
  Serial.println(smoothTemp);

  if (smoothTemp > 500){
    digitalWrite(flameled, HIGH);
    implement_motor(true);
    delay(6000);
    implement_motor(false);
    delay(3000);
  } 

  delay(2000);




  
/*
  if (start == 1) {
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

        Serial.println("start the first for loop \n");

        smoothTemp = (smoothTemp * 0.9) + (analogRead(temppin) * 0.1);//do you think smooth enough?
        //Serial.print(temp);//original
        Serial.print(smoothTemp);
        Serial.print(" - ");
        //if (temp > heatcap) {//original
        if (smoothTemp > heatcap){//LK added 2025-01-18
          heat = 1;
          maxsweep = 0;
          Serial.print("Flame, are you sure?");//LK added 2025-01-18
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
          Serial.println("INVALID, distance ??? \n");
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
          Serial.println("HERE-what is it?");
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

            smoothTemp = ((smoothTemp * 0.9) + (analogRead(temppin) * 0.1));
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
          Serial.println("DISTANCEINVALID");
          Serial.println(distancewhile);
          start = -2500;
          delay(100);
        }
      }

      lservo.write(90);
      rservo.write(90);

      if (fan == 1) {
        Serial.println("FLAME FOUND-are you sure?");
        //analogWrite(fanpin, 150);//original
        analogWrite(fanpin, 250);//LK added 2025-01-18
        digitalWrite(flameled, HIGH);

        Serial.println("value of pwm");
        Serial.println(fanpin);
        //delay(10000);//original
        delay(5000);//LK added 2025-01-18
        //analogWrite(fanpin, 0);//original
        analogWrite(fanpin, 150);//LK added 2025-01-28
        digitalWrite(flameled, LOW);

        Serial.println("value of pwm");
        Serial.println(fanpin);
      }
  }
*/
}
