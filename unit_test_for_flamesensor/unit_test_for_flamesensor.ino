/*
     # Product: Flame sensor
     # SKU    : DFR0076
     # Description:
    */

    void setup() {
      Serial.begin(9600);
    }

    void loop() {
      int sensorValue = analogRead(A0);
      Serial.println(sensorValue);
      delay(1000);
    }