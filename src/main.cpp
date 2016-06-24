/**
Author: Martin Weise
**/

#include <Arduino.h>
//#include <vector>

const auto d = 0.0;
const byte ledPin = 4;
const byte interruptPin = 3;
volatile byte state = LOW;

void blink();

void setup() {
  Serial.begin(9600);
    Serial.println("start");
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  Serial.println(state);
  digitalWrite(ledPin, state);
  delay(1000);
}

void blink() {
  Serial.println("IRQ");
  state = !state;
}
/**int main () {
  Serial.println("start");
  setup();

  while(true)
    loop();

}**/
