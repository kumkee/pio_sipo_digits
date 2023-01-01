#include "header.h"
#include <array>
#include <cstdint>

#define NUM_DIGITS 4

// Pin connected to latch pin (ST_CP) of 74HC595
const uint8_t LATCH_PIN = 33;
// Pin connected to clock pin (SH_CP) of 74HC595
const uint8_t CLOCK_PIN = 26;
////Pin connected to Data in (DS) of 74HC595
const uint8_t DATA_PIN = 14;
const std::array<uint8_t, NUM_DIGITS> DIGIT_PINS = {10, 18, 19, 21};
const unsigned delay_ms = 100;
const unsigned MULTIPLEXED_DELAY_US = 1024;

struct DigitDisplay {
  uint8_t num_digits;
  std::array<uint8_t, 3> latch_clock_data_pins;
  std::array<uint8_t, NUM_DIGITS> digit_pins;
  unsigned multiplexed_delay_us;
};

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

template <std::size_t N> void declare_outputs(std::array<uint8_t, N> pins) {
  for (uint8_t p : pins) {
    pinMode(p, OUTPUT);
  }
}

void init_digit_display(DigitDisplay d) {
  declare_outputs<3>(d.latch_clock_data_pins);
  declare_outputs<NUM_DIGITS>(d.digit_pins);
}

void setup() {
  // set pins to output so you can control the shift register
  Serial.begin(115200);
  DigitDisplay dd = {NUM_DIGITS,
                     {LATCH_PIN, CLOCK_PIN, DATA_PIN},
                     DIGIT_PINS,
                     MULTIPLEXED_DELAY_US};
  init_digit_display(dd);
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
    delayMicroseconds(MULTIPLEXED_DELAY_US);
  }
}

void display_digits(void) {
  // For debugging
  for (uint8_t i = 0; i < NUM_DIGITS; i++) {
    display_char((char)('0' + i), i);
    delayMicroseconds(MULTIPLEXED_DELAY_US);
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
  for (uint8_t b : DIGIT_PINS) {
    digitalWrite(b, HIGH);
  }
  digitalWrite(DIGIT_PINS[digit_index], LOW);
  // take the latchPin low so
  // the LEDs don't change while you're sending in bits:
  digitalWrite(LATCH_PIN, LOW);
  // shift out the bits:
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, bin);
  // take the latch pin high so the LEDs will light up:
  digitalWrite(LATCH_PIN, HIGH);
  // pause before next value:
}
