/*
  LiquidCrystal Adafruit I2C Libraries - Hello World
 
 Demonstrates the use of a 16x2 LCD display with Adafruit I2C
 interface. The LiquidCrystalAdafruitI2C library works with 
 all LCD displays that are compatible with the Hitachi HD44780 
 driver and use the I2C part of the Adafruit adapter.
 
 This sketch prints "hello world!" to the LCD
 and shows the time.
 
  The circuit:
 * Adafruit DAT to A4 (SDA)
 * Adafruit CLK to A5 (SCL)
 * Adafruit LAT is not connected
 * +5V and Gnd to the respective pins
 
 Library originally created 05 Jan 2012
 by Manfred Jonsson
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <LiquidCrystalAdafruitI2C.h>
#include <Streaming.h>
#include <Wire.h>

// initialize the library with the numbers of the interface pins
LiquidCrystalAdafruitI2C lcd(0);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd << "hello, world!";
}

void loop() {
  // print the number of seconds since reset:
  lcd(0,1) << millis()/1000;
}

