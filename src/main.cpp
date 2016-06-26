/**
Author: Martin Weise & Franziskus Wild
**/

#include <Arduino.h>
#include "TimerOne.h"
#include "MsTimer2.h"
#include "Wire.h"
#include <LiquidCrystal.h>
#include <DHT22.h>

#define DHT22_PIN 8
#define IRQ_PIN 2
#define LCD_PIN 3

DHT22 myDHT22(DHT22_PIN);
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

volatile byte state = 0;
volatile byte buttonState = 0;
volatile byte change = 0;
volatile byte counter = 0;
const byte limit = 1;
volatile byte pressCounter = 0;

void turnOnLCD();
void turnOffLCD();
void meassure();
void timerISR();
void buttonISR();
void end();

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
  Serial.println("-------------------");
  Serial.println("Starting up");
  delay(100);

  // setup the timer interupts
  MsTimer2::set(500, end);
  MsTimer2::stop();
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
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), buttonISR, CHANGE);
  MsTimer2::stop();
  state = 0;
  Serial.println("LCD IRQ initialized");
  delay(100);


  Serial.println("Ready");
  Serial.println("-------------------");
  lcd.setCursor(0, 0);
  lcd.print("Ready           ");
  counter = limit;
  Timer1.start();
  change = 0;
  pressCounter = 0 ;
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
      case 1: Timer1.start();
      case 2: turnOnLCD();
      break;
    }

    change = 0;
  }

  else if (buttonState == 0 && pressCounter > 1)
  pressCounter = 0;
}

void turnOffLCD() {
  Serial.println("LCD off");
  lcd.noDisplay();
  digitalWrite(LCD_PIN, HIGH);
  Serial.println("-------------------");
}

void turnOnLCD() {
  Serial.println("LCD on");
  lcd.display();
  digitalWrite(LCD_PIN, LOW);
}

void meassure() {
  //Serial.println("Measuring");
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

void end() {
  Serial.println("Timer2 IRQ");
  MsTimer2::stop();
  if (pressCounter > 1) {
    state = 1;
    change = 1;
  }
  buttonState = 0;
}

void buttonISR() {
  Serial.println("Button IRQ");
  if (pressCounter == 0) {

    switch (state) {
      case 0: state = 2;
      MsTimer2::start();
      buttonState = 1;

      break;
      case 1:
      case 2: state = 0;
      break;
    }

    change = 1;
  }
  pressCounter = pressCounter + 1;
}

void timerISR() {
  Serial.println("Timer1 IRQ");
  if (counter == limit) {
    state = 0;
    change = 1;
    counter = 0;
  } else {
    counter = counter + 1;
    Serial.println(counter);
  }
}
