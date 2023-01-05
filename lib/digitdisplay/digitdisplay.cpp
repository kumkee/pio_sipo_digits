#include "digitdisplay.h"

int max_int, min_int;
float max_f, min_f;

// cached variables for memoization of display_number functions
char str_int_cache[2 * NUM_DIGITS + 1];
char str_float_cache[2 * NUM_DIGITS + 1];
uint8_t decpnt_cache = 0;
int int_cache = 0;
float float_cache = 0.;
uint8_t num_decimals_cache = 0;

uint8_t dec_pnt_positions(uint8_t i) {
  return (i < NUM_DIGITS) ? (1 << i) : 0;
};

void run_thr_segments(uint8_t digit_index = 0);
void display_char(DigitDisplay dd, uint8_t bin, uint8_t digit_index = 0);
void display_char(DigitDisplay dd, char chr, uint8_t digit_index = 0,
                  uint8_t dec_pnts = 0);
void run_thr_digits(uint8_t = 0);
uint8_t separate_str_dots(char *str, uint8_t dec_pnts = 0);

template <std::size_t N> void declare_outputs(std::array<uint8_t, N> pins) {
  for (uint8_t p : pins) {
    pinMode(p, OUTPUT);
  }
}

void init_digit_display(DigitDisplay d) {
  declare_outputs<3>(d.latch_clock_data_pins);
  declare_outputs<NUM_DIGITS>(d.digit_pins);
  max_int = (int)powf(10, NUM_DIGITS) - 1;
  min_int = -(int)powf(10, NUM_DIGITS - 1) + 1;
  max_f = (float)max_int + 0.5;
  min_f = (float)min_int - 0.5;
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
  } else {
    return dec_pnts << (NUM_DIGITS - strlen(str));
  }
}

uint8_t num_to_str(char *str, int num) {
  if (num < min_int || num > max_int) {
    strcpy(str, "....");
  } else {
    sprintf(str, "%d", num);
  }
  return 0;
}

uint8_t num_to_str(char *str, float num, uint8_t num_decimals) {
  if (num < min_f || num > max_f) {
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

void display_string(DigitDisplay dd, char *str, uint8_t dec_pnts) {
  uint8_t n = strlen(str);
  char buf[NUM_DIGITS + 1];
  if (n > NUM_DIGITS) {
    strcpy(buf, "____");
  } else {
    strcpy(buf, str);
  }
  for (uint8_t i = 0; i < n; i++) {
    display_char(dd, buf[i], i + NUM_DIGITS - n, dec_pnts);
    delayMicroseconds(dd.multiplexed_delay_us);
  }
}

void display_number(DigitDisplay dd, int num) {
  if (num == int_cache) {
    display_string(dd, str_int_cache);
  } else {
    int_cache = num;
    num_to_str(str_int_cache, num);
    display_string(dd, str_int_cache);
  }
}

void display_number(DigitDisplay dd, float num, uint8_t num_decimals) {
  if (num == float_cache && num_decimals == num_decimals_cache) {
    display_string(dd, str_float_cache, decpnt_cache);
  } else {
    float_cache = num;
    num_decimals_cache = num_decimals;
    decpnt_cache = num_to_str(str_float_cache, num, num_decimals);
    display_string(dd, str_float_cache, decpnt_cache);
  }
}

void display_digits(DigitDisplay dd) {
  // For debugging
  for (uint8_t i = 0; i < NUM_DIGITS; i++) {
    display_char(dd, (char)('0' + i), i);
    delayMicroseconds(dd.multiplexed_delay_us);
  }
}

void run_thr_digits(DigitDisplay dd, uint8_t digit_index, uint8_t delay_ms) {
  // For debugging
  for (char c = '0'; c <= '9'; c++) {
    display_char(dd, c, digit_index);
    delay(delay_ms);
  }
}

void run_thr_segments(DigitDisplay dd, uint8_t digit_index, uint8_t delay_ms) {
  // For debugging
  // count from 0 to 255 and display the number
  // on the LEDs
  for (uint8_t numberToDisplay = 1; numberToDisplay > 0;
       numberToDisplay = numberToDisplay << 1) {
    display_char(dd, numberToDisplay, digit_index);
    delay(delay_ms);
  }
}

void display_char(DigitDisplay dd, char chr, uint8_t digit_index,
                  uint8_t dec_pnts) {
  display_char(dd,
               (uint8_t)(char_map(chr) +
                         (dec_pnts & dec_pnt_positions(digit_index) ? 128 : 0)),
               digit_index);
}

void display_char(DigitDisplay dd, uint8_t bin, uint8_t digit_index) {
  // turning off all digits
  for (uint8_t d : dd.digit_pins) {
    digitalWrite(d, dd.is_common_anode ? LOW : HIGH);
  }
  // turn on digit_index digit
  digitalWrite(dd.digit_pins[digit_index], dd.is_common_anode ? HIGH : LOW);

  // take the latchPin low so
  // the LEDs don't change while you're sending in bits:
  digitalWrite(dd.latch_clock_data_pins[0], LOW);
  // shift out the bits:
  shiftOut(dd.latch_clock_data_pins[2], dd.latch_clock_data_pins[1], MSBFIRST,
           dd.is_common_anode ? ~bin : bin);
  // take the latch pin high so the LEDs will light up:
  digitalWrite(dd.latch_clock_data_pins[0], HIGH);
}
