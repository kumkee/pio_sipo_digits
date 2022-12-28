#include "esp32-hal-gpio.h"
#include <Arduino.h>
#include <cstdint>
#include <cstring>

// Pin connected to latch pin (ST_CP) of 74HC595
const uint8_t latchPin = 33;
// Pin connected to clock pin (SH_CP) of 74HC595
const uint8_t clockPin = 26;
////Pin connected to Data in (DS) of 74HC595
const uint8_t dataPin = 14;
const uint8_t digits[] = {21, 19, 18, 10};
const unsigned delay_ms = 512;
const unsigned multiplexed_delay_us = 1024;

const uint8_t dec_pnt_positions[] = {1, 2, 4, 8};

void run_thr_segments(uint8_t digit_index = 0);
uint8_t char_map(char);
void display_char(uint8_t bin, uint8_t digit_index = 0);
void display_char(char chr, uint8_t digit_index = 0, uint8_t dec_pnts = 0);
void run_thr_digits(uint8_t = 0);
void display_digits(void);
void display_string(char *str, uint8_t dec_pnts = 0);
void num_to_str(char *str, int num);

void setup() {
  // set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (uint8_t d : digits) {
    pinMode(d, OUTPUT);
  }
}

// void loop() { display_digits(); }
void loop() {
  char str[] = "0000";
  for (int i = -1024; i < 10000; i++) {
    unsigned long ms = millis();
    num_to_str(str, i);
    while (millis() < ms + delay_ms) {
      display_string(str, i % 16);
    }
  }
}

void num_to_str(char *str, int num) {
  if (num < -999 || num > 9999) {
    strcpy(str, "....");
  } else {
    sprintf(str, "%d", num);
  }
}

uint8_t num_to_str(char *str, float num) {
  char buf[10];
  sprintf(buf, "%.4f", num);
  char *dp_index = strchr(buf, '.');
  return 0;
}

void display_string(char *str, uint8_t dec_pnts) {
  uint8_t n = strlen(str);
  char buf[5];
  if (n > 4) {
    strcpy(buf, "____");
  } else {
    strcpy(buf, str);
  }
  for (uint8_t i = 0; i < n; i++) {
    display_char(buf[n - 1 - i], i, dec_pnts);
    delayMicroseconds(multiplexed_delay_us);
  }
}

void display_digits(void) {
  // For debugging
  for (uint8_t i = 0; i < 4; i++) {
    display_char((char)('0' + i), i);
    delayMicroseconds(multiplexed_delay_us);
  }
}

void run_thr_digits(uint8_t digit_index) {
  // For debugging
  for (char c = '0'; c <= '9'; c++) {
    display_char(c, digit_index);
    delay(delay_ms);
  }
}

void run_thr_segments(uint8_t digit_index) {
  // For debugging
  // count from 0 to 255 and display the number
  // on the LEDs
  for (uint8_t numberToDisplay = 1; numberToDisplay > 0;
       numberToDisplay = numberToDisplay << 1) {
    display_char(numberToDisplay, digit_index);
    delay(delay_ms);
  }
}

void display_char(char chr, uint8_t digit_index, uint8_t dec_pnts) {
  display_char((uint8_t)(char_map(chr) +
                         (dec_pnts & dec_pnt_positions[digit_index] ? 128 : 0)),
               digit_index);
}

void display_char(uint8_t bin, uint8_t digit_index) {
  for (uint8_t b : digits) {
    digitalWrite(b, HIGH);
  }
  digitalWrite(digits[digit_index], LOW);
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
    return 0b00111111; // 0
  case '1':
    return 0b00000110; // 1
  case '2':
    return 0b01011011; // 2
  case '3':
    return 0b01001111; // 3
  case '4':
    return 0b01100110; // 4
  case '5':
    return 0b01101101; // 5
  case '6':
    return 0b01111101; // 6
  case '7':
    return 0b00000111; // 7
  case '8':
    return 0b01111111; // 8
  case '9':
    return 0b01101111; // 9
  case 'a':
    return 0b01110111; // A/10
  case 'b':
    return 0b01111100; // b/11
  case 'c':
    return 0b00111001; // C/12
  case 'd':
    return 0b01011110; // d/13
  case 'e':
    return 0b01111001; // E/14
  case 'f':
    return 0b01110001; // F/15
  case 'g':
    return 0b00111101; // G/16
  case 'h':
    return 0b01110110; // H/17
  case 'i':
    return 0b00110000; // I/18
  case 'j':
    return 0b00011110; // J/19
  case 'l':
    return 0b00111000; // L/20
  case 'n':
    return 0b01010100; // n/21
  case 'o':
    return 0b01011100; // o/22
  case 'p':
    return 0b01110011; // P/23
  case 'q':
    return 0b01100111; // q/24
  case 'r':
    return 0b01010000; // r/25
  case 's':
    return 0b01101101; // S/26   looks like number 5
  case 't':
    return 0b01111000; // t/27
  case 'u':
    return 0b00111110; // U/28
  case 'y':
    return 0b01101110; // y/29
  case '-':
    return 0b01000000; //-/30
  case '.':
    return 0b10000000; //./31
  case ']':
    return 0b00001111; //]/32
  case '[':
    return 0b00111001; //[/33
  case '_':
    return 0b00001000; //_/34
  default:
    return 0;
  }
}
