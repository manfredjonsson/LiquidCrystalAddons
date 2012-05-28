#include <Arduino.h>

#include "LiquidCrystalAdafruitI2C.h"

// address
#define MCP23008_ADDRESS (uint8_t)0x20

// registers
#define MCP23008_IODIR (uint8_t)0x00
#define MCP23008_IPOL (uint8_t)0x01
#define MCP23008_GPINTEN (uint8_t)0x02
#define MCP23008_DEFVAL (uint8_t)0x03
#define MCP23008_INTCON (uint8_t)0x04
#define MCP23008_IOCON (uint8_t)0x05
#define MCP23008_GPPU (uint8_t)0x06
#define MCP23008_INTF (uint8_t)0x07
#define MCP23008_INTCAP (uint8_t)0x08
#define MCP23008_GPIO (uint8_t)0x09
#define MCP23008_OLAT (uint8_t)0x0A

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystalAdafruitI2C constructor is called).

LiquidCrystalAdafruitI2C::LiquidCrystalAdafruitI2C(uint8_t i2caddr)
{
  _backlight = 0;
  _i2caddr = i2caddr;
  LiquidCrystal::init(1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255);
}


void LiquidCrystalAdafruitI2C::begin(uint8_t cols, uint8_t lines, uint8_t flags) {
  Wire.begin();

  // set defaults!
  Wire.beginTransmission(MCP23008_ADDRESS | _i2caddr);
  Wire.write(MCP23008_IODIR);
  Wire.write((uint8_t)0x01);  // only bit 0 is input
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);
  Wire.write((uint8_t)0x00);	
  Wire.endTransmission();

  LiquidCrystal::begin(cols, lines, flags);
}

// Allows to set the backlight, if the LCD backpack is used
void LiquidCrystalAdafruitI2C::setBacklight(uint8_t status) {
  _backlight = status;
//  command(LCD_FUNCTIONSET | _displayfunction); // nop command to send backlight data
}

/************ low level data pushing commands **********/
void LiquidCrystalAdafruitI2C::writebits(uint8_t value, uint8_t mode) {
  value = (value >> 1) & 0x78; // select bits
  value = value | (mode ? 0x02 : 0); // register select
//  value |= (_backlight ? 0x80 : 0); // backlight

  Wire.beginTransmission(MCP23008_ADDRESS | _i2caddr);
  Wire.write(MCP23008_GPIO);
  Wire.write(value);
  Wire.endTransmission();

  delayMicroseconds(100);

  Wire.beginTransmission(MCP23008_ADDRESS | _i2caddr);
  Wire.write(MCP23008_GPIO);
  Wire.write(value | 0x04);	
  Wire.endTransmission();

  delayMicroseconds(100);

  Wire.beginTransmission(MCP23008_ADDRESS | _i2caddr);
  Wire.write(MCP23008_GPIO);
  Wire.write(value);	
  Wire.endTransmission();
}
