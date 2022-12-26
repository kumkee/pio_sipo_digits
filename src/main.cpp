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
uint8_t char_map(char);
void display_bin(uint8_t);
void run_thr_digits(void);

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
    run_thr_digits();
  }
}

void run_thr_digits(void) {
  for (char c = '0'; c <= '9'; c++) {
    display_bin(char_map(c));
    delay(200);
  }
}

void run_thr_segments(void) {
  // count from 0 to 255 and display the number
  // on the LEDs
  for (uint8_t numberToDisplay = 1; numberToDisplay > 0;
       numberToDisplay = numberToDisplay << 1) {
    display_bin(numberToDisplay);
    delay(200);
  }
}

void display_bin(uint8_t bin) {
  // take the latchPin low so
  // the LEDs don't change while you're sending in bits:
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, bin);
  // take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
  // pause before next value:
}

uint8_t char_map(char c) {
  switch (c) {
  case '0':
    return 0b1111110; // 0
  case '1':
    return 0b0110000; // 1
  case '2':
    return 0b1101101; // 2
  case '3':
    return 0b1111001; // 3
  case '4':
    return 0b0110011; // 4
  case '5':
    return 0b1011011; // 5
  case '6':
    return 0b1011111; // 6
  case '7':
    return 0b1110000; // 7
  case '8':
    return 0b1111111; // 8
  case '9':
    return 0b1111011; // 9
  case 'a':
    return 0b1110111; // A/10
  case 'b':
    return 0b0011111; // b/11
  case 'c':
    return 0b1001110; // C/12
  case 'd':
    return 0b0111101; // d/13
  case 'e':
    return 0b1001111; // E/14
  case 'f':
    return 0b1000111; // F/15
  case 'g':
    return 0b1011110; // G/16
  case 'h':
    return 0b0110111; // H/17
  case 'i':
    return 0b0000110; // I/18
  case 'j':
    return 0b0111100; // J/19
  case 'l':
    return 0b0001110; // L/20
  case 'n':
    return 0b0010101; // n/21
  case 'o':
    return 0b0011101; // o/22
  case 'p':
    return 0b1100111; // P/23
  case 'q':
    return 0b1110011; // q/24
  case 'r':
    return 0b0000101; // r/25
  case 's':
    return 0b1011011; // S/26   looks like number 5
  case 't':
    return 0b0001111; // t/27
  case 'u':
    return 0b0111110; // U/28
  case 'y':
    return 0b0111011; // y/29
  case '-':
    return 0b0000001; //-/30
  case '.':
    return 0b10000000; //./31
  case ']':
    return 0b1111000; //]/32
  case '[':
    return 0b1001110; //[/33
  case '_':
    return 0b0001000; //_/34
  default:
    return 0;
  }
}
