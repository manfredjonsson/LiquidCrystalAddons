#ifndef LiquidCrystalAdafruitI2C_h
#define LiquidCrystalAdafruitI2C_h

#include <Wire.h>
#include "LiquidCrystal.h"

class LiquidCrystalAdafruitI2C : public LiquidCrystal {
public:
  LiquidCrystalAdafruitI2C(uint8_t i2caddr);
  void setBacklight(uint8_t status);
  virtual void begin(uint8_t cols, uint8_t lines, uint8_t flags = LCD_5x8DOTS);
protected:
  virtual void writebits(uint8_t, uint8_t);
  uint8_t _backlight;
  uint8_t _i2caddr;
};

#endif
