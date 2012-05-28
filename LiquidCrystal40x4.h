#ifndef LiquidCrystal40x4_h
#define LiquidCrystal40x4_h

#include "LiquidCrystal.h"

#define LCD_DISPLAYCONTROLMASK 0xF8

class LiquidCrystal40x4 : public LiquidCrystal {
public:
  LiquidCrystal40x4(uint8_t rs, uint8_t rw, uint8_t enable_1, uint8_t enable_2,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystal40x4(uint8_t rs, uint8_t enable_1, uint8_t enable_2,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystal40x4(uint8_t rs, uint8_t rw, uint8_t enable_1, uint8_t enable_2,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystal40x4(uint8_t rs, uint8_t enable_1, uint8_t enable_2,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable_1, uint8_t enable_2,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);

  void setCursor(uint8_t col, uint8_t row);
  void createChar(uint8_t location, uint8_t charmap[]);
  virtual void command(uint8_t value);
  virtual size_t write(uint8_t value);

protected:
  uint8_t _enable_1;
  uint8_t _enable_2;
};

#endif
