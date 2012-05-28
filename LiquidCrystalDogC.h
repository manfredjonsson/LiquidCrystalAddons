#ifndef LiquidCrystalDogC_h
#define LiquidCrystalDogC_h

#include <LiquidCrystal.h>

// commands
#define LCD_DOGBIAS 0x14
#define LCD_DOGPOWER 0x50
#define LCD_DOGFOLLOW 0x68
#define LCD_DOGCONTR 0x70

// lcd types
#define LCD_TYPESTD 0x00
#define LCD_TYPEDOG 0x01
#define LCD_DOG50V 0x00
#define LCD_DOG33V 0x02

// flags for function set
#define LCD_DOGIS1 0x01

// flags for dog power control
#define LCD_DOGBIASLOW 0x00
#define LCD_DOGBIASHIGH 0x80
#define LCD_DOGBOOSTON 0x04
#define LCD_DOGBOOSTOFF 0x00

// interface modes
#define LCD_IF8BIT 0x00
#define LCD_IF4BIT 0x01
#define LCD_IFTPARAL 0x00
#define LCD_IFTSWSPI 0x02
#define LCD_IFTHWSPI 0x04
#define LCD_IFTMASK 0x06

class LiquidCrystalDogC : public LiquidCrystal {
public:
  LiquidCrystalDogC(uint8_t rs, uint8_t enable,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystalDogC(uint8_t rs, uint8_t rw, uint8_t enable,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystalDogC(uint8_t rs, uint8_t rw, uint8_t enable,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystalDogC(uint8_t rs, uint8_t enable,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
  LiquidCrystalDogC(uint8_t rs, uint8_t cw,
                    uint8_t clk, uint8_t si);
  LiquidCrystalDogC(uint8_t rs, uint8_t cw);

  void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
                    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    
  virtual void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS | LCD_TYPEDOG);

  void contrast(uint8_t, uint8_t);
  void setCursor(uint8_t, uint8_t); 

protected:
  virtual void writebits(uint8_t, uint8_t);

  // _enable_pin:   this pin is used as CS pin in both SPI modes.
  // _data_pins[6]: this pin is used as CLK pin in software SPI mode.
  // _data_pins[7]: this pin is used as MOSI pin in software SPI mode.

  uint8_t _displaytype;
  uint8_t _interfacemode;
  uint8_t _dogpower;
};

#endif
