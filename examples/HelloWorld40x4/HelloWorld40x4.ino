/*
  LiquidCrystal40x4 Library - Hello World
 
 Demonstrates the use a 40x4 LCD display. The LiquidCrystal 40x4
 library works with all 4 line LCD displays that are wider than 
 20 characters and are compatible with the Hitachi HD44780 driver.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD First Enable pin to digital pin 11
 * LCD Second Enable pin to digital pin 10
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally created 18 Jan 2012
 by Manfred Jonsson
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include "LiquidCrystal40x4.h"


// initialize the library with the numbers of the interface pins
LiquidCrystal40x4 lcd(12, 11, 10, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(40, 4);
  // Display cursor (show switch between display driver).
  lcd.cursor();
  lcd.blink();
  // Print a message to the LCD.
  lcd.print("hello, world!");
  // show cursor on first display driver
  delay(2000);
}

void loop() {
  // set the cursor to column 0, line 2 (aka line 0 of the second display driver)
  lcd.setCursor(0, 2);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  delay(250);
}

