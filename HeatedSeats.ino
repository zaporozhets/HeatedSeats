/*
 * Copyright 2017 Taras Zaporozhets
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include <Adafruit_SleepyDog.h>

#include <Bounce2.h>


const int LEFT_LED1 = 14;
const int LEFT_LED2 = 15;

const int RIGHT_LED1 = 10;
const int RIGHT_LED2 = 16;


const int LEFT_BUTTON = 7;
const int RIGHT_BUTTON = 2;

const int LEFT_HEATER_UP = 3;
const int LEFT_HEATER_DOWN = 5;

const int RIGHT_HEATER_UP = 6;
const int RIGHT_HEATER_DOWN = 9;



Bounce leftDebouncer = Bounce();
Bounce rightDebouncer = Bounce();

int leftSeatState = 0;
int rightSeatState = 0;

// R - L
const int LED_SEQ[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

// UP - DOWN
const int HEATER_SEQ[4][2] = {{0, 0}, {100, 100}, {160, 160}, {255, 255}};


//Timer leftSeatTimeout;

void setup() {

  int countdownMS = Watchdog.enable();

  pinMode(LEFT_HEATER_UP, OUTPUT);
  analogWrite(LEFT_HEATER_UP, 0);

  pinMode(LEFT_HEATER_DOWN, OUTPUT);
  analogWrite(LEFT_HEATER_DOWN, 0);

  pinMode(RIGHT_HEATER_UP, OUTPUT);
  analogWrite(RIGHT_HEATER_UP, 0);

  pinMode(RIGHT_HEATER_DOWN, OUTPUT);
  analogWrite(RIGHT_HEATER_DOWN, 0);

  // Setup LEDs
  pinMode(LEFT_LED1, OUTPUT);
  pinMode(LEFT_LED2, OUTPUT);
  pinMode(RIGHT_LED1, OUTPUT);
  pinMode(RIGHT_LED2, OUTPUT);

  ledPlay();

  // Setup the left button with an internal pull-up
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  
  // After setting up the button, setup the Bounce instance
  leftDebouncer.attach(LEFT_BUTTON);
  leftDebouncer.interval(5); // interval in ms

  // Setup the right button with an internal pull-up
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  
  // After setting up the button, setup the Bounce instance
  rightDebouncer.attach(RIGHT_BUTTON);
  rightDebouncer.interval(5); // interval in ms
}

void loop() {
  Watchdog.reset();

  // Update the Bounce instances
  leftDebouncer.update();
  rightDebouncer.update();

  if ( leftDebouncer.fell() ) {
    leftSeatState++;
    leftSeatState %= 4;
    setLeftSeatState(leftSeatState);
  }

  if ( rightDebouncer.fell() ) {
    rightSeatState++;
    rightSeatState %= 4;
    setRightSeatState(rightSeatState);
  }
}

void ledPlay()
{
  int pattern[6][4] =
  {
    {0, 0, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1},
    {0, 0, 0, 0},
  };

  for (int i = 0; i < (sizeof(pattern)/sizeof(int)/4); i++)
  {
    digitalWrite(LEFT_LED1,  pattern[i][0]);
    digitalWrite(LEFT_LED2,  pattern[i][1]);
    digitalWrite(RIGHT_LED1, pattern[i][2]);
    digitalWrite(RIGHT_LED2, pattern[i][3]);
    delay(100);
  }
  return;
}

void setLeftSeatState(int state)
{
  digitalWrite(LEFT_LED1, LED_SEQ[state][0]);  // Left
  digitalWrite(LEFT_LED2, LED_SEQ[state][1]);  // Right

  analogWrite(LEFT_HEATER_UP,   HEATER_SEQ[state][0]);  // Up
  analogWrite(LEFT_HEATER_DOWN, HEATER_SEQ[state][1]);  // Down
}

void setRightSeatState(int state)
{
  digitalWrite(RIGHT_LED1, LED_SEQ[state][0]);  // Left
  digitalWrite(RIGHT_LED2, LED_SEQ[state][1]);  // Right

  analogWrite(RIGHT_HEATER_UP,   HEATER_SEQ[state][0]); // Up
  analogWrite(RIGHT_HEATER_DOWN, HEATER_SEQ[state][1]); // Down
}

