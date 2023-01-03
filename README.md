# Platformio Example Project
This is an example project ultilizing my [PlatformIO template](https://github.com/kumkee/pio_template).

## Example Description
It is a multiplexed 4-digit 7-segment display controlled by an ESP32 Development Board and an SIPO (serial-in, parallel-out) shift register. Connections are illustrated in the circuit diagram below. 

## Circuit Schematic Diagram
<img width="983" alt="s2p" src="https://user-images.githubusercontent.com/391431/210318676-17233847-d29c-450c-a14d-52e4d1e5d0b7.png">

## Circuit Real Objects

### Real Time

![20230104_011805](https://user-images.githubusercontent.com/391431/210443056-53ebb146-4eeb-4af0-b772-c7b8b8a1d62b.gif)

### Slow-mo

![20230104_101405](https://user-images.githubusercontent.com/391431/210443074-670dcd72-a5eb-4e5f-bc30-590491913d77.gif)

## Project Structure
```
.
├── compile_commands.json      --> Compilation database generated by the compiledb target of the Makefile. Language servers need this to work.
├── include
│   ├── header.h
│   └── README
├── lib
│   ├── charmap                 --> A library of a char-to-binary map
│   │   ├── charmap.cpp
│   │   └── charmap.h
│   ├── digitdisplay            --> A library of multiplexed digit display
│   │   ├── digitdisplay.cpp
│   │   └── digitdisplay.h          --> NUM_DIGITS: to set number of digits
│   └── README
├── LICENSE
├── Makefile
├── platformio.ini              --> Project configurations: platform, board, framework, ...
├── README.md                   --> This file
├── src
│   └── main.cpp                --> The main programme
└── test                        --> Unit test. Not implemented yet.
    └── README
```
