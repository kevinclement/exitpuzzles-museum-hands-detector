#include <Arduino.h>

int IN_PIN = 14;
int THRESH = 100;
int BOUNCE = 200;

unsigned long lastStateTime = 0;
bool touching = false;
bool lastTouchState = false;

bool isTouching = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Museum Hand Detector by kevinc...\n");
}

void loop() {
    int total1 = analogRead(IN_PIN);

    //Serial.println(total1);
    if ((!lastTouchState && total1 > THRESH) || 
        (lastTouchState && total1 < THRESH)) {
      lastStateTime = millis();
    }

    if ((millis() - lastStateTime) > BOUNCE) {

      if (touching && !lastTouchState) {
        Serial.println("OFF#");
        touching = false;
      }

      if (!touching && lastTouchState) {
        Serial.println("ON#");
        touching = true;
      }
    }

    lastTouchState = total1 > THRESH;
    delay(10);
}