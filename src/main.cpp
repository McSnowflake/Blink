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

volatile byte state = 0;
volatile byte change = 0;
volatile byte counter = 0;
const byte limit = 1;

void turnOnLCD();
void turnOffLCD();
void meassure();
void timerISR();
void buttonISR();

void setup() {
  // setup LCD and start it with light off
  lcd.begin(16, 2);
  pinMode(LCD_PIN, OUTPUT);
  digitalWrite(LCD_PIN, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Starting ...");
  delay(100);

  // set up serial com
  Serial.begin(9600);
  Serial.println("Starting ...");
  delay(100);

  // setup the timer interupt
  Timer1.initialize(3000000);
  Timer1.attachInterrupt(timerISR);
  Timer1.stop();
  Serial.println("Timer initialized");
  delay(100);

  // initialize The temeraturee sensor
  myDHT22.readData();
  Serial.println("Temp Sensor initialized");
  delay(100);

  // setup the LCD toggle interupt
  pinMode(IRQ_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), buttonISR, FALLING);
  state = 0;
  Serial.println("LCD IRQ initialized");
  delay(100);


  Serial.println("Ready           ");
  lcd.setCursor(0, 0);
  lcd.print("Ready           ");
  counter = limit;
  Timer1.start();
}

void loop() {
  delay(100);
  if (state != 0)
    meassure();
  if (change == 1) {
    switch (state) {
      case 0: turnOffLCD();
              Timer1.stop();
              break;
      case 1: turnOnLCD();
              delay(500);
              if (digitalRead(IRQ_PIN) == HIGH) {
                counter = 0;
                Timer1.start();
              } else {
                Serial.println("long press");
                state = 2;
              }
              break;
    }
    change = 0;
  }
}

void turnOffLCD() {
  Serial.println("Off");
  lcd.noDisplay();
  digitalWrite(LCD_PIN, HIGH);
}

void turnOnLCD() {
  Serial.println("On");
  lcd.display();
  digitalWrite(LCD_PIN, LOW);
}

void meassure() {
  Serial.println("Measuring");
  myDHT22.readData();
  lcd.setCursor(0, 0);
  lcd.print("Temp.: ");
  lcd.print(myDHT22.getTemperatureC());
  lcd.print((char)223);
  lcd.print("C   ");
  lcd.setCursor(0, 1);
  lcd.print("Humid.: ");
  lcd.print(myDHT22.getHumidity());
  lcd.print("%    ");
}

void buttonISR() {
  Serial.println("Button IRQ");
  change = 1;
  switch (state) {
    case 0: state = 1;
    break;
    case 1: counter = 0;
    break;
    case 2: state = 0;
    break;
  }
}

void timerISR() {
  Serial.println("Timer IRQ");
  if (counter == limit) {
    state = 0;
    change = 1;
  } else {
    counter = counter + 1;
    Serial.println(counter);
  }
}
