/***************************************************
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks
  ----> http://www.adafruit.com/products/872
  ----> http://www.adafruit.com/products/871
  ----> http://www.adafruit.com/products/870

  These displays use I2C to communicate, 2 pins are required to
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

 #include <Wire.h>
 #include <Adafruit_GFX.h>
 #include "Adafruit_LEDBackpack.h"

 Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

void setup() {
    Serial.begin(9600);
    matrix.begin(0x70);
}

static const uint8_t PROGMEM
    up_bmp[] =
    {
        B00011000,
        B00111100,
        B01111110,
        B11111111,
        B00011000,
        B00011000,
        B00011000,
        B00011000
    },
    left_bmp[] =
    {
        B00001000,
        B00001100,
        B00001110,
        B11111111,
        B11111111,
        B00001110,
        B00001100,
        B00001000
    },
    right_bmp[] =
    {
        B00010000,
        B00110000,
        B01110000,
        B11111111,
        B11111111,
        B01110000,
        B00110000,
        B00010000
    },
    down_bmp[] =
    {
        B00011000,
        B00011000,
        B00011000,
        B00011000,
        B11111111,
        B01111110,
        B00111100,
        B00011000
    };

void loop() {
    matrix.clear();
    matrix.drawBitmap(0, 0, up_bmp, 8, 8, LED_GREEN);
    matrix.writeDisplay();
    delay(500);

    matrix.clear();
    matrix.drawBitmap(0, 0, down_bmp, 8, 8, LED_GREEN);
    matrix.writeDisplay();
    delay(500);

    matrix.clear();
    matrix.drawBitmap(0, 0, left_bmp, 8, 8, LED_GREEN);
    matrix.writeDisplay();
    delay(500);

    matrix.clear();
    matrix.drawBitmap(0, 0, right_bmp, 8, 8, LED_GREEN);
    matrix.writeDisplay();
    delay(500);
}