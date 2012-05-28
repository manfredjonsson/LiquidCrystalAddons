#include <Arduino.h>
#include "pins_arduino.h"

#include "LiquidCrystalDogC.h"

LiquidCrystalDogC::LiquidCrystalDogC(uint8_t rs, uint8_t rw, uint8_t enable,
                                     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(0, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystalDogC::LiquidCrystalDogC(uint8_t rs, uint8_t enable,
                                     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(0, rs, 255, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystalDogC::LiquidCrystalDogC(uint8_t rs, uint8_t rw, uint8_t enable,
                                     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
  init(1, rs, rw, enable, 0xff, 0xff, 0xff, 0xff, d0, d1, d2, d3);
}

LiquidCrystalDogC::LiquidCrystalDogC(uint8_t rs, uint8_t enable,
                                     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
  init(1, rs, 255, enable, 0xff, 0xff, 0xff, 0xff, d0, d1, d2, d3);
}

LiquidCrystalDogC::LiquidCrystalDogC(uint8_t rs, uint8_t cs,
                                     uint8_t clk, uint8_t si)
{
  init(LCD_IF8BIT | LCD_IFTSWSPI, rs, 255, cs, 255, 255, 255, 255, 255, 255, clk, si);
}

LiquidCrystalDogC::LiquidCrystalDogC(uint8_t rs, uint8_t cs)
{
  init(LCD_IF8BIT | LCD_IFTHWSPI, rs, 255, cs, 255, 255, 255, 255, 255, 255, 255, 255);
}

void LiquidCrystalDogC::init(uint8_t ifmode, uint8_t rs, uint8_t rw, uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  _interfacemode = ifmode;
  LiquidCrystal::init(0, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);

  if (_interfacemode & LCD_IFTHWSPI) { 
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
  } else   if (_interfacemode & LCD_IFTSWSPI) { 
    pinMode(_enable_pin, OUTPUT);
    pinMode(_rs_pin, OUTPUT);
    pinMode(_data_pins[6], OUTPUT); //clk
    pinMode(_data_pins[7], OUTPUT); //mosi
  }
}

void LiquidCrystalDogC::begin(uint8_t cols, uint8_t rows, uint8_t mode) {
  uint8_t bias;

  _numcols = cols;
  _numrows = rows;

  if (rows > 1) {
    _displayfunction |= LCD_2LINE;
  } else {
    _displayfunction &= ~LCD_2LINE;
  }

  if (mode & LCD_DOG33V) {
    _dogpower = LCD_DOGBOOSTON;
    bias = LCD_DOGBIASLOW;
  } else {
    _dogpower = LCD_DOGBOOSTOFF;
    bias = LCD_DOGBIASHIGH;
  }

  // for some 1 line displays you can select a 10 pixel high font
  if ((mode & LCD_5x10DOTS) && (rows == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  } else {
    _displayfunction &= ~LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delayMicroseconds(50000); 

  //we don't know which mode the LCD is in. maybe in the middle of a 4 bit mode transfer. force 8 bit mode.
  for(int i = 0; i < 3; i++) {
    // we start in 8bit mode, try to set 8 bit mode
    writebits(LCD_FUNCTIONSET | LCD_8BITMODE, LOW);
    delayMicroseconds(5000); // wait min 4.1ms
  }

  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // finally, set to 4-bit interface
    writebits(LCD_FUNCTIONSET, LOW);
    delayMicroseconds(100);   // commands and writes need > 37us to settle
  }

  if (mode & LCD_TYPEDOG) {
    _displaytype = LCD_TYPEDOG;
    command(LCD_FUNCTIONSET | _displayfunction | LCD_DOGIS1);
    command(LCD_DOGBIAS | bias | (rows == 3));	  // bias
    command(LCD_DOGFOLLOW | 0x02);  // follower control

    contrast(36, 1);
  } else {
    _displaytype = LCD_TYPESTD;
  }

  // finally, set # rows, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  
  delay(5);
  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDISABLE;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystalDogC::setCursor(uint8_t col, uint8_t row) {
  int pos = col;
  if ((_displaytype == LCD_TYPEDOG) && (_numrows == 3)) {
    pos += row << 4;
  } else {
    pos += ((row & 1) ? 0x40 : 0) + ((row & 2) ? _numcols : 0);
  }
  command(LCD_SETDDRAMADDR | (pos & 0x7f));
}

void LiquidCrystalDogC::contrast (uint8_t value, uint8_t is = 0) {
  if (_displaytype != LCD_TYPEDOG)
    return;
  if (is != 1)
    command(LCD_FUNCTIONSET | _displayfunction | LCD_DOGIS1);

  command(LCD_DOGCONTR | (value & 0x0f));
  command(LCD_DOGPOWER | _dogpower | ((value >> 4) & 0x03));

  if (is != 1)
    command(LCD_FUNCTIONSET | _displayfunction | is);
}

/************ low level data pushing commands **********/
void LiquidCrystalDogC::writebits(uint8_t value, uint8_t mode) {
  switch(_interfacemode & LCD_IFTMASK) {
  case LCD_IFTSWSPI:
    digitalWrite(_rs_pin, mode);
    digitalWrite(_enable_pin, LOW);
    shiftOut(_data_pins[7], _data_pins[6], MSBFIRST, value);
    digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(4);
    break;
  case LCD_IFTHWSPI:
    digitalWrite(_rs_pin, mode);
    digitalWrite(_enable_pin, LOW);
    SPDR = value;
    while(!(SPSR & _BV(SPIF)));
    digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(4);
    break;
  // LCD_IFTPARAL:
  default:
    LiquidCrystal::writebits(value, mode);
    break;
  }
}
