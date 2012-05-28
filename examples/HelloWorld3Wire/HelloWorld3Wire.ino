/*
  LiquidCrystal 3Wire Library - Hello World
 
 Demonstrates the use a 16x2 LCD display with a 3 Wire interface. 
 The LiquidCrystal 3 Wire library works with all LCD displays 
 that are compatible with the Hitachi HD44780 driver and use a 
 3 Wire interface. There are many of them out there, some 
 interfaces are found in the Arduino playground. This example 
 uses the Adafruit I2C/SPI adapter with the SPI jumper closed.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * Adafruit DAT to digital pin 11 (MOSI)
 * Adafruit CLK to digital pin 13 (SCLK)
 * Adafruit LAT is digital pin 10 (SSP)
 * +5V and Gnd to the respective pins

 You need the internal wiring of the adapter (74hc585) for the lcd 
 constructor. The following values are for the Adafruit I2C/SPI
 adapter, change them if you have a different wiring. Don't forget 
 to set the adapter to SPI.
 * LCD RS pin to output 1
 * LCD Enable pin to output 2
 * LCD D4 pin to output 3
 * LCD D5 pin to output 4
 * LCD D6 pin to output 5
 * LCD D7 pin to output 6
 * LCD R/W pin to ground -> pin = 255
 * backlight switch to output 7

 Library originally created 18 Jan 2012
 by Manfred Jonsson
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include "LiquidCrystal.h"
#include "LiquidCrystal3Wire.h"


// initialize the library with the interface pins
LiquidCrystal3Wire lcd(10, 1, 255, 2, 6, 5, 4, 3, 7);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, wire!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  delay(250);
}

