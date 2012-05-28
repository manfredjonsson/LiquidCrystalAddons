#include <Arduino.h>
#include "pins_arduino.h"

#include "LiquidCrystalAdafruitSPI.h"

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
// LiquidCrystalAdafruitSPI constructor is called).

LiquidCrystalAdafruitSPI::LiquidCrystalAdafruitSPI(uint8_t cs, uint8_t clk, uint8_t si)
{
  init(1, 255, 255, cs, 255, 255, 255, 255, 255, 255, clk, si);
}

LiquidCrystalAdafruitSPI::LiquidCrystalAdafruitSPI(uint8_t cs)
{
  init(1, 255, 255, cs, 255, 255, 255, 255, 255, 255, 255, 255);
}

void LiquidCrystalAdafruitSPI::init(uint8_t ifmode, uint8_t rs, uint8_t rw, uint8_t enable,
                               uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                               uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  _backlight = 0;
  LiquidCrystal::init(1, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

void LiquidCrystalAdafruitSPI::begin(uint8_t cols, uint8_t rows, uint8_t dotsize) {
  if (_data_pins[7] == 0xff) { 
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
    pinMode(_enable_pin, OUTPUT);
    pinMode(_data_pins[6], OUTPUT); //clk
    pinMode(_data_pins[7], OUTPUT); //mosi
  }

  LiquidCrystal::begin(cols, rows, dotsize);
}

// Allows to set the backlight, if the LCD backpack is used
void LiquidCrystalAdafruitSPI::setBacklight(uint8_t status) {
  _backlight = status;
  command(LCD_FUNCTIONSET | _displayfunction); // nop command to send backlight data
}

/************ low level data pushing commands **********/
void LiquidCrystalAdafruitSPI::writebits(uint8_t value, uint8_t mode) {
  /* 
  if(_data_pins[5] != 0xff) {
    LiquidCrystal::writebits(value, mode);
    return;
  }
  */
  value = ((value >> 4) & 0x08)  | ((value >> 2) & 0x10)
        | ((value) & 0x20)  | ((value << 2) & 0x40) ; //exchange bits
  value |= (mode ? 0x02 : 0); // register select
  value |= (_backlight ? 0x08 : 0); // backlight

  digitalWrite(_enable_pin, LOW);
  if(_data_pins[7] == 0xff) {
    SPDR = value;
    while(!(SPSR & _BV(SPIF)));
  } else {
    shiftOut(_data_pins[7], _data_pins[6], MSBFIRST, value);
  }
  digitalWrite(_enable_pin, HIGH);

  delayMicroseconds(1);    

  digitalWrite(_enable_pin, LOW);
  if(_data_pins[7] == 0xff) {
    SPDR = value | 0x04;
    while(!(SPSR & _BV(SPIF)));
  } else {
    shiftOut(_data_pins[7], _data_pins[6], MSBFIRST, value | 0x04);
  }
  digitalWrite(_enable_pin, HIGH);

  delayMicroseconds(1);    

  digitalWrite(_enable_pin, LOW);
  if(_data_pins[7] == 0xff) {
    SPDR = value;
    while(!(SPSR & _BV(SPIF)));
  } else {
    shiftOut(_data_pins[7], _data_pins[6], MSBFIRST, value);
  }
  digitalWrite(_enable_pin, HIGH);

  delayMicroseconds(100);    
}
