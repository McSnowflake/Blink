/**
Author: Martin Weise
**/
#include "Wire.h"
#include <Arduino.h>
//#include <vector>
const int aNumbers[]= {1,2,3,4,5};
const char* pcPlainOldString ="Hello World";
const auto dVar = 0.0;
const byte bLedPin = 4;
const byte bInterruptPin = 3;
volatile byte bState = LOW;

void blink();

void setup() {
  Serial.begin(9600);
    Serial.println("start");
  pinMode(bLedPin, OUTPUT);
  pinMode(bInterruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(bInterruptPin), blink, CHANGE);
}

void loop() {
  Serial.println(bState);
  for(const auto& item : aNumbers)
  {
    Serial.println(pcPlainOldString);
    Serial.println(item);
  }
  digitalWrite(bLedPin, bState);
  delay(1000);
}

void blink() {
  Serial.println("IRQ");
  bState = !bState;
}
/**int main () {
  Serial.println("start");
  setup();

  while(true)
    loop();

}**/
