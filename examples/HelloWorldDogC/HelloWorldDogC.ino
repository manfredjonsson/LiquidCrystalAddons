/*
  LiquidCrystalDogC Library - Hello World
 
 Demonstrates the use of a Dog Character display. The 
 LiquidCrystalDogC library works with all one, two or three 
 line LCD displays. You can provide a flag to allow operation 
 from 3.3 Volt power supplies. This example uses the SPI 
 interface of the Dog Character displays but 4 and 8 bit 
 modes are supported too, all examples from the original 
 LiquidCrystal library are supposed to work too. 
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD CLK pin to digital pin 13
 * LCD CS pin to digital pin 10
 * LCD RS pin to digital pin 9
 * LCD D7 (MOSI) pin to digital pin 11
 * LCD D0 to D6 pin to ground
 * LCD R/W pin to ground
 
 Library originally added 04 Jan 2012
 by Manfred Jonsson
 
 This example code is released under LGPL 2.0.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <LiquidCrystalDogC.h>

// initialize the library with the numbers of the interface pins
LiquidCrystalDogC lcd(9, 10);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, World!");
}

void loop() {
  // print the number of seconds since reset:
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}
