#ifndef LiquidCrystal3Wire_h
#define LiquidCrystal3Wire_h

#include "LiquidCrystal.h"

class LiquidCrystal3Wire : public LiquidCrystal {
public:
  LiquidCrystal3Wire(uint8_t cs, uint8_t clk, uint8_t si,
                     uint8_t rs, uint8_t rw, uint8_t enable,
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t bl);
  LiquidCrystal3Wire(uint8_t cs,
                     uint8_t rs, uint8_t rw, uint8_t enable,
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t bl);

  void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
            uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
            uint8_t cs, uint8_t clk, uint8_t si,
            uint8_t bl);
  void setBacklight(uint8_t status);
protected:
  virtual void writebits(uint8_t, uint8_t);
  uint8_t _backlight;
  uint8_t _cs_pin;
  uint8_t _clk_pin;
  uint8_t _si_pin;
  uint8_t _bl_pin;
};

#endif
