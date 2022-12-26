#include "esp32-hal-gpio.h"
#include <Arduino.h>

// Pin connected to latch pin (ST_CP) of 74HC595
const uint8_t latchPin = 33;
// Pin connected to clock pin (SH_CP) of 74HC595
const uint8_t clockPin = 26;
////Pin connected to Data in (DS) of 74HC595
const uint8_t dataPin = 14;

const uint8_t digits[] = {10, 18, 19, 21};

void run_thr_segments(void);

uint8_t char_map(char c) {
  switch (c) {
  case '0':
    return 0b11111100; // 0
  case '1':
    return 0b01100000; // 1
  case '2':
    return 0b11011010; // 2
  case '3':
    return 0b11110010; // 3
  case '4':
    return 0b01100110; // 4
  case '5':
    return 0b10110110; // 5
  case '6':
    return 0b10111110; // 6
  case '7':
    return 0b11100000; // 7
  case '8':
    return 0b11111110; // 8
  case '9':
    return 0b11110110; // 9
  case 'a':
    return 0b11101110; // A/10
  case 'b':
    return 0b00111110; // b/11
  case 'c':
    return 0b10011100; // C/12
  case 'd':
    return 0b01111010; // d/13
  case 'e':
    return 0b10011110; // E/14
  case 'f':
    return 0b10001110; // F/15
  case 'g':
    return 0b10111100; // G/16
  case 'h':
    return 0b01101110; // H/17
  case 'i':
    return 0b00001100; // I/18
  case 'j':
    return 0b01111000; // J/19
  case 'l':
    return 0b00011100; // L/20
  case 'n':
    return 0b00101010; // n/21
  case 'o':
    return 0b00111010; // o/22
  case 'p':
    return 0b11001110; // P/23
  case 'q':
    return 0b11100110; // q/24
  case 'r':
    return 0b00001010; // r/25
  case 's':
    return 0b10110110; // S/26   looks like number 5
  case 't':
    return 0b00011110; // t/27
  case 'u':
    return 0b01111100; // U/28
  case 'y':
    return 0b01110110; // y/29
  case '-':
    return 0b00000010; //-/30
  case '.':
    return 0b00000001; //./31
  case ']':
    return 0b11110000; //]/32
  case '[':
    return 0b10011100; //[/33
  case '_':
    return 0b00010000; //_/34
  default:
    return 0;
  }
}

void setup() {
  // set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (uint8_t d : digits) {
    pinMode(d, OUTPUT);
  }
}

void loop() {
  for (uint8_t d : digits) {
    for (uint8_t b : digits) {
      digitalWrite(b, HIGH);
    }
    digitalWrite(d, LOW);
    run_thr_segments();
  }
}

void run_thr_segments(void) {
  // count from 0 to 255 and display the number
  // on the LEDs
  for (uint8_t numberToDisplay = 1; numberToDisplay > 0;
       numberToDisplay = numberToDisplay << 1) {
    // take the latchPin low so
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);
    // take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(200);
  }
}
