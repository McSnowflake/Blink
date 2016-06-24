/**
Author: Martin Weise
**/

#include <Arduino.h>
#include "TimerOne.h"
#include "Wire.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

volatile byte state = LOW;

void blink();

void setup() {
  lcd.begin(16, 2);
  //Serial.begin(9600);
    //Serial.println("start");
    lcd.setCursor(5, 0);
    lcd.print("init");
    lcd.setCursor(0, 1);

Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
//Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
Timer1.attachInterrupt(blink);  // attaches callback() as a timer overflow interrupt

}

void loop() {
  if (state == LOW) {
     lcd.print("IRQ");
  } else {
     lcd.print("     ");
  }
  lcd.setCursor(5, 1);
  lcd.print(state);
  lcd.setCursor(0, 1);


    delay(100);
}

void blink() {
  //Serial.println("IRQ");


state = !state;
}
