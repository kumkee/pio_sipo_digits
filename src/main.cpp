#include "header.h"

#define NUM_DIGITS 4

// Pin connected to latch pin (ST_CP) of 74HC595
const uint8_t latchPin = 33;
// Pin connected to clock pin (SH_CP) of 74HC595
const uint8_t clockPin = 26;
////Pin connected to Data in (DS) of 74HC595
const uint8_t dataPin = 14;
const uint8_t digitPins[] = {10, 18, 19, 21};
const unsigned delay_ms = 100;
const unsigned multiplexed_delay_us = 1024;

uint8_t dec_pnt_positions(uint8_t i) {
  return (i < NUM_DIGITS) ? (1 << i) : 0;
};

void run_thr_segments(uint8_t digit_index = 0);
void display_char(uint8_t bin, uint8_t digit_index = 0);
void display_char(char chr, uint8_t digit_index = 0, uint8_t dec_pnts = 0);
void run_thr_digits(uint8_t = 0);
void display_digits(void);
void display_string(char *str, uint8_t dec_pnts = 0);
uint8_t num_to_str(char *str, int num);
uint8_t num_to_str(char *str, float num, uint8_t num_decimals = NUM_DIGITS);
uint8_t separate_str_dots(char *str, uint8_t dec_pnts = 0);

void setup() {
  // set pins to output so you can control the shift register
  Serial.begin(115200);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (uint8_t d : digitPins) {
    pinMode(d, OUTPUT);
  }
}

// void loop() { display_digits(); }
void loop() {
  char str[2 * NUM_DIGITS + 1];
  bool flag_float = false;
  for (int i = 0; i < 10000; i++) {
    unsigned long ms = millis();
    uint8_t dp;
    if (i % 5 == 0) {
      flag_float = !flag_float;
    }
    dp =
        flag_float ? num_to_str(str, (float)(i / 10.0), 1) : num_to_str(str, i);
    Serial.printf("\r                  \r%s\t%d", str, dp);
    while (millis() < ms + delay_ms) {
      display_string(str, dp);
    }
  }
}

uint8_t separate_str_dots(char *str, uint8_t dec_pnts) {
  // pure_str is without dots
  char *buf = strchr(str, (int)'.');
  if (buf) {
    char pure_str[2 * NUM_DIGITS + 1];
    uint8_t dot_ind = buf - str;
    memcpy(pure_str, str, dot_ind);
    strcpy(pure_str + dot_ind, buf + 1);
    strcpy(str, pure_str);
    return separate_str_dots(str, dec_pnts | dec_pnt_positions(dot_ind - 1));
    // return separate_str_dots(str, dec_pnts | dec_pnt_pos[dot_ind - 1]);
  } else {
    return dec_pnts << (NUM_DIGITS - strlen(str));
  }
}

uint8_t num_to_str(char *str, int num) {
  if (num < -999 || num > 9999) {
    strcpy(str, "....");
  } else {
    sprintf(str, "%d", num);
  }
  return 0;
}

uint8_t num_to_str(char *str, float num, uint8_t num_decimals) {
  if (num < -999.5 || num > 9999.5) {
    strcpy(str, "....");
    return 0;
  } else {
    sprintf(str, "%.*f", num_decimals, num);
    if (strlen(str) > NUM_DIGITS + 1) {
      str[NUM_DIGITS + 1] = '\0';
    }
    return separate_str_dots(str);
  }
}

void display_string(char *str, uint8_t dec_pnts) {
  uint8_t n = strlen(str);
  char buf[NUM_DIGITS + 1];
  if (n > NUM_DIGITS) {
    strcpy(buf, "____");
  } else {
    strcpy(buf, str);
  }
  for (uint8_t i = 0; i < n; i++) {
    display_char(buf[i], i + NUM_DIGITS - n, dec_pnts);
    delayMicroseconds(multiplexed_delay_us);
  }
}

void display_digits(void) {
  // For debugging
  for (uint8_t i = 0; i < NUM_DIGITS; i++) {
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
                         (dec_pnts & dec_pnt_positions(digit_index) ? 128 : 0)),
               digit_index);
}

void display_char(uint8_t bin, uint8_t digit_index) {
  for (uint8_t b : digitPins) {
    digitalWrite(b, HIGH);
  }
  digitalWrite(digitPins[digit_index], LOW);
  // take the latchPin low so
  // the LEDs don't change while you're sending in bits:
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, bin);
  // take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);
  // pause before next value:
}
