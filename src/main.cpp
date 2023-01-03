#include "header.h"

// Pin connected to latch pin (ST_CP/RCLK) of 74HC595
const uint8_t LATCH_PIN = 33;
// Pin connected to clock pin (SH_CP/SRCLK) of 74HC595
const uint8_t CLOCK_PIN = 26;
////Pin connected to Data in (DS/SER) of 74HC595
const uint8_t DATA_PIN = 14;
const std::array<uint8_t, NUM_DIGITS> DIGIT_PINS = {23, 13, 9, 12};
const unsigned delay_ms = 100;
const unsigned MULTIPLEXED_DELAY_US = 1024;

DigitDisplay dd;

void setup() {
  // set pins to output so you can control the shift register
  Serial.begin(115200);
  dd = {false, // is_common_anode, false for common cathode display
        {LATCH_PIN, CLOCK_PIN, DATA_PIN},
        DIGIT_PINS,            // array<unit
        MULTIPLEXED_DELAY_US}; // duration for diplaying each digit in
                               // microseconds
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
      display_string(dd, str, dp);
    }
  }
}
