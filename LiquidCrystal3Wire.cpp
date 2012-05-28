#include <Arduino.h>
#include "pins_arduino.h"

#include "LiquidCrystal3Wire.h"

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
// LiquidCrystal3Wire constructor is called).

LiquidCrystal3Wire::LiquidCrystal3Wire(uint8_t cs, uint8_t clk, uint8_t si,
                                       uint8_t rs, uint8_t rw, uint8_t enable,
                                       uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                                       uint8_t bl)
{
  init(1, rs, rw, enable, d4, d5, d6, d7, cs, clk, si, 0);
}

LiquidCrystal3Wire::LiquidCrystal3Wire(uint8_t cs,
                                       uint8_t rs, uint8_t rw, uint8_t enable,
                                       uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                                       uint8_t bl)
{
  init(1, rs, rw, enable, d4, d5, d6, d7, cs, 255, 255, 0);
}

void LiquidCrystal3Wire::init(uint8_t ifmode, uint8_t rs, uint8_t rw, uint8_t enable,
                              uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                              uint8_t cs, uint8_t clk, uint8_t si,
                              uint8_t bl)
{
  _backlight = 0;
  _cs_pin = cs;
  _clk_pin = clk;
  _si_pin = si;
  _bl_pin = bl;

  LiquidCrystal::init(1, rs, rw, enable, 255, 255, 255, 255, d4, d5, d6, d7);

  if (_clk_pin == 0xff) { 
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(SS, OUTPUT);
  
    digitalWrite(SCK, LOW);
    digitalWrite(MOSI, LOW);
    digitalWrite(SS, HIGH);

    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);
    
    //SPCR = (SPCR & 0xf3) | 0x00; // SPI_MODE_MASK
    //SPCR = (SPCR & 0xfc) | 0x03; // SPI_CLOCK_MASK
    //SPSR = (SPSR & 0xfe); // SPI_2XCLOCK_MASK
  } else { 
    pinMode(_cs_pin, OUTPUT);
    pinMode(_clk_pin, OUTPUT); //clk
    pinMode(_si_pin, OUTPUT); //mosi
  }
}

// Allows to set the backlight, if the LCD backpack is used
void LiquidCrystal3Wire::setBacklight(uint8_t status) {
  _backlight = status;
  command(LCD_FUNCTIONSET | _displayfunction); // nop command to send backlight data
}

/************ low level data pushing commands **********/
void LiquidCrystal3Wire::writebits(uint8_t value, uint8_t mode) {
  uint8_t regvalue = 0;

  regvalue |= ((value & 0x10) ? 1 : 0) << _data_pins[4];
  regvalue |= ((value & 0x20) ? 1 : 0) << _data_pins[5];
  regvalue |= ((value & 0x40) ? 1 : 0) << _data_pins[6];
  regvalue |= ((value & 0x80) ? 1 : 0) << _data_pins[7];

  regvalue |= (mode ? 1 << _rs_pin : 0); // register select
  regvalue |= (_backlight ? 1 << _bl_pin : 0); // backlight

  digitalWrite(_cs_pin, LOW);
  if(_clk_pin == 0xff) {
    SPDR = regvalue;
    while(!(SPSR & _BV(SPIF)));
  } else {
    shiftOut(_clk_pin, _si_pin, MSBFIRST, regvalue);
  }
  digitalWrite(_cs_pin, HIGH);
  delayMicroseconds(1);    
  digitalWrite(_cs_pin, LOW);
  if(_clk_pin == 0xff) {
    SPDR = regvalue | 1 << _enable_pin;
    while(!(SPSR & _BV(SPIF)));
  } else {
    shiftOut(_clk_pin, _si_pin, MSBFIRST, regvalue | 1 << _enable_pin);
  }
  digitalWrite(_cs_pin, HIGH);
  delayMicroseconds(1);    
  digitalWrite(_cs_pin, LOW);
  if(_clk_pin == 0xff) {
    SPDR = regvalue;
    while(!(SPSR & _BV(SPIF)));
  } else {
    shiftOut(_clk_pin, _si_pin, MSBFIRST, regvalue);
  }
  digitalWrite(_cs_pin, HIGH);
  delayMicroseconds(100);    
}
