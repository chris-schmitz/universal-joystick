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

#define BUTTON_LED 12
#define BUTTON 13

void setup () {
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

    pinMode(BUTTON, INPUT);
    pinMode(BUTTON_LED, OUTPUT);
}

int cursor [2] = {0, 0};

// note that we're hard coding to the matrix grid here
// if you abstract this to a class at some point, make the
// grid size configurable.
const int maxCoordinates = 64; // number of pixels in grid
int onLEDs[maxCoordinates] = {0};

void loop () {


    int leftState = digitalRead(LEFT);
    int rightState = digitalRead(RIGHT);
    int upState = digitalRead(UP);
    int downState = digitalRead(DOWN);

    int buttonState = digitalRead(BUTTON);


    // X axis
    if (rightState == LOW) {
        cursor[0] = ++cursor[0] % 8;
    }

    if (leftState == LOW) {
        // accounting for negative 1
        cursor[0] = ((--cursor[0] % 8) + 8) % 8;
    }

    // Y axis
    if (downState == LOW) {
        cursor[1] = ++cursor[1] % 8;
    }

    if (upState == LOW && cursor[1] >= 0) {
        // accounting for negative 1
        cursor[1] = ((--cursor[1] % 8) + 8) % 8;
    }

    // Serial.println("Cursor: ");
    // Serial.println(cursor[0]);
    // Serial.println(cursor[1]);

    matrix.clear();
    matrix.drawPixel(cursor[0], cursor[1], LED_GREEN);
    // matrix.writeDisplay();

    if (buttonState == LOW) {
        Serial.println("button pressed");

        int xTranslated = cursor[0] % 8;
        int yTranslated = cursor[1] * 8;
        int onLEDArrayPosition = xTranslated + yTranslated ;

        // int onLEDArrayPosition = cursor[0] + cursor[1];
        Serial.println("++++++++++++++++++++");
        Serial.print("cursor x: "); Serial.println(cursor[0]);
        Serial.print("cursor y: "); Serial.println(cursor[1]);
        Serial.print("position: "); Serial.println(onLEDArrayPosition);
        Serial.println("++++++++++++++++++++");

        Serial.println(onLEDArrayPosition);
        togglePixelState(onLEDArrayPosition);
    }

    updateMatrix();

    delay(100);
}

void togglePixelState (int arrayPosition) {
    onLEDs[arrayPosition] = !onLEDs[arrayPosition];
}

void updateMatrix () {
    for (int i = 0 ; i < maxCoordinates ; i++) {

        if (onLEDs[i]) {
            int row = i % 8;
            int column = i / 8;
            // int row = (64 -  i) % 8; // grid row size
            // int column = i / 8; // grid column size

            // Serial.println("--------------------");
            Serial.print("index: "); Serial.println(i);
            Serial.print("row: "); Serial.println(row);
            Serial.print("column: "); Serial.println(column);
            // Serial.println("--------------------");

            if (row == cursor[0] && column == cursor[1]) {
                matrix.drawPixel(row, column, LED_YELLOW);
            } else {
                matrix.drawPixel(row, column, LED_RED);
            }
        }
    }

    matrix.writeDisplay();
}
