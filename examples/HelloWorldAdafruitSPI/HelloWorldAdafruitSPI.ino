/*
  LiquidCrystal Adafruit SPI Libraries - Hello World
 
 Demonstrates the use a 16x2 LCD display. The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "hello adafruit spi!" to the LCD
 and shows the time.
 
  The circuit:
 * Adafruit DAT to digital pin 11 (MOSI)
 * Adafruit CLK to digital pin 13 (SCLK)
 * Adafruit LAT is digital pin 10 (SSP)
 * +5V and Gnd to the respective pins
 
 Library originally added 18 Apr 2008
 by Manfred Jonsson
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <LiquidCrystalAdafruitSPI.h>
#include <Streaming.h>

// initialize the library with the numbers of the interface pins
LiquidCrystalAdafruitSPI lcd(10);

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

