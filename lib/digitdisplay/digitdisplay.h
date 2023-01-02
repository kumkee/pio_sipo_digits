
#ifndef DIGITDISPLAY
#define DIGITDISPLAY

#define NUM_DIGITS 4
#include <array>
#include <cstdint>

struct DigitDisplay {
  bool is_common_anode;
  std::array<uint8_t, 3> latch_clock_data_pins;
  std::array<uint8_t, NUM_DIGITS> digit_pins;
  unsigned multiplexed_delay_us;
};

#endif
