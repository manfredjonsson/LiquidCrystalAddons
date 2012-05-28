This repository contains some examples for the oop (inheritance) 
extensions of my version of the Liquid Crystal library. In detail:

40x4: Support for 40x4 Displays with two controllers / enable pins
3Wire: Uses some sort of SPI interface e.g. a 74x585 to connect the LCD 
  to the Arduino. The example uses the Adafruit I2c/SPI adapter but any
  adapter described in the playground should work with this code.
AdafruitI2C: Uses the Adafruit I2C/SPI adapter in I2C mode.
AdafruitSPI: Uses the Adafruit I2C/SPI adapter in SPI mode. Hardware and
  software SPI are supported.
DogC: Support for the Dog Charater Display series from Electronic Assembly.
  Privides parallel, hardware SPI and software SPI interface modes.

The files can find their home in the Liquid Crystal library path or in a 
separate firectory because the example sketches include the needed header 
file and the generic LiquidCrystal header file.