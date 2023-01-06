#include "header.h"

// Pin connected to latch pin (RCLK) of the shift register
const uint8_t LATCH_PIN = 33;
// Pin connected to clock pin (SRCLK) of the shift register
const uint8_t CLOCK_PIN = 26;
// Pin connected to Data in (SER) of the shift register
const uint8_t DATA_PIN = 14;
// Common cathode pins for individual digits
const std::array<uint8_t, NUM_DIGITS> DIGIT_PINS = {23, 18, 19, 21};
// Single digit display duration in μs
const unsigned MULTIPLEXED_DELAY_US = 1024;
// true for a common anode display; false for common cathode
const bool IS_COMMON_ANODE = false;

const unsigned delay_ms = 100;

const unsigned MAXI = powf(10, NUM_DIGITS);

DigitDisplay dd;

void setup() {
  Serial.begin(115200);
  dd = {IS_COMMON_ANODE, // false for common cathode display
        {LATCH_PIN, CLOCK_PIN, DATA_PIN},
        DIGIT_PINS,            // array of size NUM_DIGITS
        MULTIPLEXED_DELAY_US}; // duration for diplaying each digit in μs
  init_digit_display(dd);
}

void fn_f(uint8_t n) { display_number(dd, (float)(n / 10.0), 1); }
void fn_i(uint8_t n) { display_number(dd, n); }

void loop() {
  bool flag_float = false;
  void (*fn[2])(uint8_t) = {fn_f, fn_i};
  for (int i = 0; i < MAXI; i++) {
    unsigned long ms = millis();
    if (i % 5 == 0) {
      flag_float = !flag_float;
    }
    Serial.printf("\r         \r%d", i);
    while (millis() < ms + delay_ms) {
      (*fn[flag_float])(i);
    }
  }
}
