#include <Arduino.h>
#include "pins_arduino.h"

#include "LiquidCrystal40x4.h"

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
// LiquidCrystal40x4 constructor is called).

LiquidCrystal40x4::LiquidCrystal40x4(uint8_t rs, uint8_t rw, uint8_t enable_1, uint8_t enable_2,
                                     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(1, rs, rw, enable_1, enable_2, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal40x4::LiquidCrystal40x4(uint8_t rs, uint8_t enable_1, uint8_t enable_2,
                                     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(1, rs, 255, enable_1, enable_2,  d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal40x4::LiquidCrystal40x4(uint8_t rs, uint8_t rw, uint8_t enable_1, uint8_t enable_2,
                                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(1, rs, rw, enable_1, enable_2, 255, 255, 255, 255, d4, d5, d6, d7);
}

LiquidCrystal40x4::LiquidCrystal40x4(uint8_t rs, uint8_t enable_1, uint8_t enable_2,
                                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(1, rs, 255, enable_1, enable_2, 255, 255, 255, 255, d4, d5, d6, d7);
}

void LiquidCrystal40x4::init(uint8_t ifmode, uint8_t rs, uint8_t rw, uint8_t enable_1, uint8_t enable_2,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  _enable_1 = enable_1;
  _enable_2 = enable_2;

  LiquidCrystal::init(ifmode, rs, rw, enable_1, d0, d1, d2, d3, d4, d5, d6, d7);
  pinMode(_enable_2, OUTPUT);
}

void LiquidCrystal40x4::begin(uint8_t cols, uint8_t rows, uint8_t charsize) {
  _enable_pin = _enable_2;
  LiquidCrystal::begin(cols, rows, charsize);
  // initialize first display last. this sets _enable_pin to the first display
  // and write() will automatically start to send characters to the first display.
  _enable_pin = _enable_1;
  LiquidCrystal::begin(cols, rows, charsize);
}

void LiquidCrystal40x4::setCursor(uint8_t col, uint8_t row)
{
  uint8_t row_offsets[] = { 0x00, 0x40};
  if ( row > _numrows ) {
    row = _numrows-1;    // we count rows starting w/0
  }
  if ( row < 2 ) {
    _enable_pin = _enable_1;
  } else {
    _enable_pin = _enable_2;
  }
  command(LCD_SETDDRAMADDR | (col + row_offsets[row & 0x01]));
  // refresh cursor settings. the code in command() will automatically switch
  // the cursor on or off depending on which is the active display the current
  // cursor settings.
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal40x4::createChar(uint8_t location, uint8_t charmap[]) {
  uint8_t enable_save = _enable_pin;
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    _enable_pin = _enable_1;
    send(charmap[i], HIGH);
    _enable_pin = _enable_2;
    send(charmap[i], HIGH);
  }
  _enable_pin = enable_save;
}

/************ mid level data pushing commands **********/
void LiquidCrystal40x4::command(uint8_t value) {
  uint8_t enable_save = _enable_pin;
  uint8_t value_1 = value;
  uint8_t value_2 = value;

  //mask off cursor on the inactive display
  if ((value & LCD_DISPLAYCONTROLMASK) == LCD_DISPLAYCONTROL) {
    if (_enable_pin == _enable_1) {
      value_2 = value & (LCD_DISPLAYCONTROLMASK | LCD_DISPLAYON);
    } else {
      value_1 = value & (LCD_DISPLAYCONTROLMASK | LCD_DISPLAYON);
    }
  }

  _enable_pin = _enable_1;
  send(value_1,LOW);
  _enable_pin = _enable_2;
  send(value_2,LOW);
  _enable_pin = enable_save;
}

size_t LiquidCrystal40x4::write(uint8_t value) {
  uint8_t enable_pin_safe = _enable_pin;

  // write value to one display
  LiquidCrystal::write(value);
  // shift the other display in autoscrollmode
  if(_displaymode & LCD_ENTRYSHIFTENABLE) {
    if(_enable_pin == _enable_1) {
      _enable_pin = _enable_2;
    } else {
      _enable_pin = _enable_1;
    }
    command(LCD_CURSORSHIFT
           | LCD_DISPLAYMOVE
           | ( _displaymode & LCD_ENTRYLEFT) ? LCD_MOVERIGHT : LCD_MOVELEFT );
    _enable_pin = enable_pin_safe;
  }
}

/************ low level data pushing commands **********/
