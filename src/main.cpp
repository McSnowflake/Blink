/**
Author: Martin Weise
**/

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "TimerOne.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const auto ledPin = 4;
const byte interruptPin = 3;
volatile byte state = LOW;

void blink();

void setup() {
  Serial.begin(9600);
    Serial.println("start");
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
//Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
Timer1.attachInterrupt(blink);  // attaches callback() as a timer overflow interrupt

}

void loop() {
  Serial.println(state);
  digitalWrite(ledPin, state);
  delay(100);
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
