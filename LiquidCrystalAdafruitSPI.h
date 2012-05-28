#ifndef LiquidCrystalAdafruitSPI_h
#define LiquidCrystalAdafruitSPI_h

#include "LiquidCrystal.h"

class LiquidCrystalAdafruitSPI : public LiquidCrystal {
public:
  LiquidCrystalAdafruitSPI(uint8_t cw, uint8_t clk, uint8_t si);
  LiquidCrystalAdafruitSPI(uint8_t cw);

  void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

  virtual void begin(uint8_t cols, uint8_t rows, uint8_t dotsize = 0);
  void setBacklight(uint8_t status);
protected:
  virtual void writebits(uint8_t, uint8_t);
  uint8_t _backlight;
};

#endif
