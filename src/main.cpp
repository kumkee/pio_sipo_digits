#include "header.h"

using namespace std;

const uint8_t LATCH_PIN = 33; // to RCLK of the shift register (S/R)
const uint8_t CLOCK_PIN = 26; // to SRCLK of the S/R
const uint8_t DATA_PIN = 14;  // to SER of the S/R
// Common cathode pins for individual digits
const array<uint8_t, NUM_DIGITS> DIGIT_PINS = {23, 18, 19, 21};
// Single-digit multiplexed period in μs
const unsigned MULTIPLEXED_DELAY_US = 1024;
// true for a common anode display; false for common cathode
const bool IS_COMMON_ANODE = false;

const unsigned delay_ms = 100;

const unsigned MAXI = powf(10, NUM_DIGITS);

DigitDisplay dd;

void setup() {
  Serial.begin(115200);  // for debugging output
  dd = {IS_COMMON_ANODE, // false for common cathode display
        {LATCH_PIN, CLOCK_PIN, DATA_PIN},
        DIGIT_PINS,            // array of size NUM_DIGITS
        MULTIPLEXED_DELAY_US}; // duration in μs
  init_digit_display(dd);
}

function<void(int &)> display_funcs[2] = {
    [](int &n) { display_number(dd, n); },
    [](int &n) { display_number(dd, (float)(n / 10.0), 1); }};

void loop() {
  bool is_float = false;
  for (int i = 0; i < MAXI; i++) {
    unsigned long ms = millis();
    if (i % 5 == 0) {
      is_float = !is_float;
    }
    Serial.printf("\r         \r%d", i); // debugging output
    while (millis() < ms + delay_ms) {
      display_funcs[is_float](i);
    }
  }
}
