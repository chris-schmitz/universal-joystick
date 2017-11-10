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

#define LEFT 8
#define RIGHT 9
#define UP 10
#define DOWN 11

#define LEFT_LED 4
#define RIGHT_LED 5
#define UP_LED 6
#define DOWN_LED 7

#define BUTTON_LED 12
#define BUTTON 13


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
},

down_left_bmp[] =
{
    B00000011,
    B00000111,
    B00001110,
    B10011100,
    B11111000,
    B11110000,
    B11110000,
    B11111000
},
down_right_bmp[] =
{
    B11000000,
    B11100000,
    B01110000,
    B00111001,
    B00011111,
    B00001111,
    B00001111,
    B00011111
},
up_right_bmp[] =
{
    B00011111,
    B00001111,
    B00001111,
    B00011111,
    B00111001,
    B01110000,
    B11100000,
    B11000000
},
up_left_bmp[] =
{
    B11111000,
    B11110000,
    B11110000,
    B11111000,
    B10011100,
    B00001110,
    B00000111,
    B00000011
}
;




void setup() {
    Serial.begin(9600);
    matrix.begin(0x70);

    pinMode(LEFT, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(UP, INPUT);
    pinMode(DOWN, INPUT);
    digitalWrite(LEFT, HIGH);
    digitalWrite(RIGHT, HIGH);
    digitalWrite(UP, HIGH);
    digitalWrite(DOWN, HIGH);

    pinMode(LEFT_LED, OUTPUT);
    pinMode(RIGHT_LED, OUTPUT);
    pinMode(UP_LED, OUTPUT);
    pinMode(DOWN_LED, OUTPUT);

    pinMode(BUTTON, INPUT);
    pinMode(BUTTON_LED, OUTPUT);

}

void loop() {
    // so what do we want?
    // directions to correspond to a light
    // pushing button lights up the button and all direction leds
    // pushing button again turns off all lights again


    int leftState = digitalRead(LEFT);
    int rightState = digitalRead(RIGHT);
    int upState = digitalRead(UP);
    int downState = digitalRead(DOWN);

    // Serial.println(BUTTON_LED);
    // Serial.print("right: "); Serial.println(rightState);
    // Serial.print("up: "); Serial.println(upState);
    // Serial.print("down: "); Serial.println(downState);

    int buttonState = digitalRead(BUTTON);

    if (leftState == HIGH && rightState == HIGH && upState == HIGH && downState == HIGH && buttonState == HIGH) {
        matrix.clear();
        matrix.writeDisplay();
        digitalWrite(LEFT_LED, LOW);
        digitalWrite(RIGHT_LED, LOW);
        digitalWrite(UP_LED, LOW);
        digitalWrite(DOWN_LED, LOW);

        digitalWrite(BUTTON_LED, LOW);
    }


    if (leftState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, left_bmp, 8, 8, LED_RED);
        // matrix.writeDisplay();

        digitalWrite(LEFT_LED, HIGH);
    }

    if (rightState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, right_bmp, 8, 8, LED_RED);
        // matrix.writeDisplay();

        digitalWrite(RIGHT_LED, HIGH);
    }

    if (upState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, up_bmp, 8, 8, LED_RED);
        // matrix.writeDisplay();
        digitalWrite(UP_LED, HIGH);
    }

    if (downState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, down_bmp, 8, 8, LED_RED);
        // matrix.writeDisplay();

        digitalWrite(DOWN_LED, HIGH);
    }

    if (downState == LOW && leftState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, down_left_bmp, 8, 8, LED_RED);
    }

    if (downState == LOW && rightState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, down_right_bmp, 8, 8, LED_RED);
    }

    if (upState == LOW && leftState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, up_left_bmp, 8, 8, LED_RED);
    }

    if (upState == LOW && rightState == LOW) {
        matrix.clear();
        matrix.drawBitmap(0, 0, up_right_bmp, 8, 8, LED_RED);
    }

    if (buttonState == LOW) {
        matrix.clear();
        matrix.drawRect(1, 1, 6, 6, LED_GREEN);
        // matrix.writeDisplay();

        digitalWrite(BUTTON_LED, HIGH);

        digitalWrite(UP_LED, HIGH);
        digitalWrite(DOWN_LED, HIGH);
        digitalWrite(LEFT_LED, HIGH);
        digitalWrite(RIGHT_LED, HIGH);
    }

    matrix.writeDisplay();

    delay(100);
}
