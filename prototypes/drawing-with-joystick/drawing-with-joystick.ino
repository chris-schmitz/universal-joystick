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
#define BUTTON 2

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

    // pinMode(BUTTON, INPUT);
    pinMode(BUTTON_LED, OUTPUT);

    pinMode(BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON), handleButtonPress , CHANGE);
}


// note that we're hard coding to the matrix grid here
// if you abstract this to a class at some point, make the
// grid size configurable.
const int totalGridRows = 8;
const int totalGridColumns = 8;
const int maxCoordinates = totalGridRows * totalGridColumns;
int onLEDs[maxCoordinates] = {0};

int cursor [2] = {0, 0};
bool buttonActive = false;
bool buttonLEDActive = false;

// Note that this handles two pieces of info: the button logical
// state and the button LED state. The end goal is to fire the logical
// branch only once per push but to handle the button LED on every loop
// so that the button's light stays on the entire time that the user is
// holding it down.
void handleButtonPress () {
    int buttonState = digitalRead(BUTTON);

    // note the branch in the logic
    if (buttonState == HIGH) {
        buttonLEDActive = false;
    }
    else {
        buttonActive = true;
        buttonLEDActive = true;
    }

}

void loop () {
    // start fresh
    matrix.clear();

    // Get the state of each of the joystick directions
    int leftState = digitalRead(LEFT);
    int rightState = digitalRead(RIGHT);
    int upState = digitalRead(UP);
    int downState = digitalRead(DOWN);

    // figure out the cursor position
    // X axis
    if (rightState == LOW) {
        cursor[0] = ++cursor[0] % totalGridRows;
    }

    if (leftState == LOW) {
        // accounting for negative 1
        cursor[0] = (--cursor[0] + totalGridRows) % totalGridRows;
    }

    // Y axis
    if (downState == LOW) {
        cursor[1] = ++cursor[1] % totalGridColumns;
    }

    if (upState == LOW && cursor[1] >= 0) {
        // accounting for negative 1
        cursor[1] = (--cursor[1] + totalGridColumns) % totalGridColumns;
    }

    // if the button is active, handle adding or removing the pixel number
    // currently under the cursor to the array of pixels to draw on the
    // matrix
    if (buttonActive == true) {

        int xTranslated = cursor[0] % totalGridRows;
        int yTranslated = cursor[1] * totalGridColumns;
        int onLEDArrayPosition = xTranslated + yTranslated ;

        togglePixelState(onLEDArrayPosition);

        // once we're done with this, deactiveate the button, otherwise
        // we end up with a blinking cursor pixel and the current pixel
        // number being added and removed from the array with each blink.
        // also, note that this deactivates the button _logic_ but not
        // the button's LED.
        buttonActive = false;
    }

    // now that we've setup all of our info, update the matrix display :)
    updateMatrix();

    // and delay b/c otherwise you get a fast cursor ;P
    delay(100);
}


void togglePixelState (int arrayPosition) {
    onLEDs[arrayPosition] = !onLEDs[arrayPosition];
}

void updateMatrix () {

    // It feels a bit awkward putting this function call here b/c it's
    // not really a matrix update, but really it would prob be awkward
    // anywhere other than it's own function and that seems a bit like
    // overkill (kind of like this comment block, but anyway...).
    // The underlying purpose of this function is really just about UX,
    // what is the feedback for the user based on their actions. That's
    // why I put it here. maybe the function should be renamed but i'm
    // terrible at naming things so ¯\_(ツ)_/¯
    digitalWrite(BUTTON_LED, buttonLEDActive);

    matrix.drawPixel(cursor[0], cursor[1], LED_GREEN);

    // I'm sure the encoding and decoding of corridantes/pixel numbers
    // is a bit heavy handed, but I'm still (re)learning C++ and I was
    // having a hard time figuring out how to handle multi-dimensional
    // arrays. This was cool to figure out anyway so I'm all good with
    // it :)
    for (int i = 0 ; i < maxCoordinates ; i++) {

        if (onLEDs[i]) {
            int row = i % totalGridRows;
            int column = i / totalGridColumns;

            if (row == cursor[0] && column == cursor[1]) {
                matrix.drawPixel(row, column, LED_YELLOW);
            } else {
                matrix.drawPixel(row, column, LED_RED);
            }
        }
    }

    matrix.writeDisplay();
}
