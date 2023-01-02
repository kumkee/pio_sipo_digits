#include "charmap.h"

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
