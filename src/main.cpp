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
volatile byte ready = 0;
volatile byte counter = 0;
const byte limit = 1;

void switch_on_LCD();
void switch_off_LCD();

void setup() {
  // setup LCD and start it with light off
  lcd.begin(16, 2);
  pinMode(LCD_PIN, OUTPUT);
  digitalWrite(LCD_PIN, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Starting ...");
  delay(500);

  // set up serial com
  Serial.begin(9600);
  Serial.println("Starting ...");
  delay(500);

  // setup the timer interupt
  Timer1.initialize(3000000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(switch_off_LCD);
  Timer1.stop();
  Serial.println("Timer initialized");
  delay(500);

  // initialize The temeraturee sensor
  myDHT22.readData();
  myDHT22.getTemperatureC();
  myDHT22.getHumidity();
  Serial.println("Temp Sensor initialized");
  delay(500);

  // setup the LCD toggle interupt
  pinMode(IRQ_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), switch_on_LCD, FALLING);
  Serial.println("LCD IRQ initialized");
  delay(500);


  Serial.println("Ready           ");
  lcd.setCursor(0, 0);
  lcd.print("Ready           ");
  counter = limit;

  Timer1.start();
  ready = 1;
}

void loop() {

  delay(1000);
}

void meassure() {
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

void switch_on_LCD() {
  if (state == LOW) {
    meassure();
    state = HIGH;
    Serial.println("On");
    lcd.display();
    digitalWrite(LCD_PIN, LOW);
    Timer1.start();
  } else {
    counter = 0;
  }
}

void switch_off_LCD() {
  if (ready == 1)
  if (counter == limit) {
    Serial.println("Off");
    lcd.noDisplay();
    state = LOW;
    digitalWrite(LCD_PIN, HIGH);
    Timer1.stop();
    counter = 0;
  } else {
    counter = counter + 1;
    Serial.println(counter);
  }
}
