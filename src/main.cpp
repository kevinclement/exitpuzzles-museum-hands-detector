#include <Arduino.h>
#include "version.h"

#define IN_PIN 14
#define THRESH 325
#define BOUNCE 150

// functions
void readAnySerialMessage();
bool currentlyTouching();
void status();
void solved();
void(* resetFunc) (void) = 0;

bool isTouching = false;
unsigned long touch_timestamp = 0;
bool _mock = false;
bool _debug = false;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);

  Serial.println("Museum Hand Detector by kevinc...");
  Serial.println(getFullVersion("museum-hands-detector"));

  status();
}

void loop() {
  readAnySerialMessage();

  if (!currentlyTouching()) {
    if (isTouching) {
      Serial.println("finished holding hands.");
      isTouching = false;
      status();
    }
    touch_timestamp = 0;
  } else if(touch_timestamp <= 0) {
    touch_timestamp = millis();
  } else if (!isTouching && millis() - touch_timestamp > BOUNCE) {
    Serial.println("held hands.");
    isTouching = true;
    status();
  }

  delay(10);
}

void status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"
      "touching:%s,"
      "mock:%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,
      isTouching ? "true" : "false",
      _mock ? "true" : "false");

  Serial.println(cMsg);
}

bool currentlyTouching() {
  int curVal = analogRead(IN_PIN);
  if (_debug) {
    Serial.print("current value: ");
    Serial.println(curVal);
  }

  return curVal > THRESH || _mock;
}

void readAnySerialMessage() {
  if (!Serial.available()) {
    return;
  }

  String msg = Serial.readStringUntil('\n');
  Serial.print("got '");
  Serial.print(msg);
  Serial.println("' command");

  if (msg == "mock" || msg == "m") {
    Serial.println("mocking a touch event...");
    _mock = !_mock;
  }
  else if (msg == "debug" || msg == "x") {
    Serial.println("toggling debug...");
    _debug = !_debug;
  }
  else if (msg == "reset" || msg == "reboot" || msg == "r") {
    resetFunc();
  } else {
    Serial.print("unknown command: ");
    Serial.println(msg);
  }
}