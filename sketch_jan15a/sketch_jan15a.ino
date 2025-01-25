#include <Servo.h>

Servo lservo;
Servo rservo;

const int rpin = 6;
const int lpin = 3;

const int trig = 11;
const int echo = 12;

const int fanpin = A1;
const int flameled = 8;

int heatcap = 100;
int distmin = 12;
int temppin = A0;

void setup() {
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

void loop() {
  int distance[19];

  int sweepmilli = 3000;
  int maxsweep = 0;
  int heat = 0;
  float temp = 0;

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

  for (int i = 0; i <= 6; i++) {
    temp = analogRead(temppin);
    Serial.print(temp);
    Serial.print(" - ");
    if (temp > heatcap) {
      heat = 1;
      maxsweep = 0;
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
      Serial.println("INVALID");
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

    if ((heat == 1) or (temp > heatcap)) {
      Serial.println("HERE");
      int hot = 0;
      delay(1000);
      lservo.write(100);
      rservo.write(100);
      delay(sweepmilli / 2);

      lservo.write(90);
      rservo.write(90);
      delay(500);

      for (int i = 0; i <= 6; i++) {
        temp = analogRead(temppin);
        Serial.println(temp);

        if (temp > hot) {
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
    Serial.println("FLAME FOUND");
    analogWrite(fanpin, 150);
    digitalWrite(flameled, HIGH);
    delay(10000);
    analogWrite(fanpin, 0);
    digitalWrite(flameled, LOW);
  }
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
