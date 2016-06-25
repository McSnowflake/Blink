/**
Author: Martin Weise & Franziskus Wild
**/

#include <Arduino.h>
#include "TimerOne.h"
#include "Wire.h"
#include <LiquidCrystal.h>
#include <DHT22.h>

#define DHT22_PIN 8
#define IRQ_PIN 2
#define LCD_PIN 3

DHT22 myDHT22(DHT22_PIN);
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

volatile byte state = LOW;

void switch_on_LCD();
void switch_off_LCD();

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");

  // setup LCD and start it with light off
  lcd.begin(16, 2);
  pinMode(LCD_PIN, OUTPUT);
  digitalWrite(LCD_PIN, HIGH);
  lcd.noDisplay();

  delay(1000);

  // initialize The temeraturee sensor
  myDHT22.readData();
  myDHT22.getTemperatureC();
  myDHT22.getHumidity();

  delay(1000);

  // setup the timer interupt
  Timer1.initialize(5000000);         // initialize timer1, and set a 1/2 second period

  // setup the LCD toggle interupt
  pinMode(IRQ_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), switch_on_LCD, FALLING);
}

void loop() {

  if (state == HIGH) {
    myDHT22.readData();
    lcd.setCursor(0, 0);
    lcd.print(myDHT22.getTemperatureC());
    lcd.print("C   ");
    lcd.setCursor(0, 1);
    lcd.print(myDHT22.getHumidity());
    lcd.print("%    ");
  }

    delay(1000);
}

void switch_on_LCD() {
  if (state == LOW) {
    state = HIGH;
    Serial.println("On");
    lcd.display();
    digitalWrite(LCD_PIN, LOW);
    Timer1.attachInterrupt(switch_off_LCD, 10000000);
  } else {

  }
}

void switch_off_LCD() {
  Serial.println("Off");
  lcd.noDisplay();
  state = LOW;
  digitalWrite(LCD_PIN, HIGH);
  Timer1.detachInterrupt();
}
