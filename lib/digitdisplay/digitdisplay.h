
#ifndef DIGITDISPLAY_H
#define DIGITDISPLAY_H

#define NUM_DIGITS 4
#include "charmap.h"
#include <Arduino.h>
#include <array>
#include <cstdint>
#include <cstring>

struct DigitDisplay {
  bool is_common_anode;
  std::array<uint8_t, 3> latch_clock_data_pins;
  std::array<uint8_t, NUM_DIGITS> digit_pins;
  unsigned multiplexed_delay_us;
};

void init_digit_display(DigitDisplay d);
uint8_t num_to_str(char *str, int num);
uint8_t num_to_str(char *str, float num, uint8_t num_decimals = NUM_DIGITS);
void display_digits(DigitDisplay dd);
void display_string(DigitDisplay dd, char *str, uint8_t dec_pnts = 0);
void display_number(DigitDisplay dd, int num);
void display_number(DigitDisplay dd, float num,
                    uint8_t num_decimals = NUM_DIGITS);
// TODO: implementation of the two above with memoization of num_to_str

#endif
