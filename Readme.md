# RGB Serial Controller

A simple RGB controller for arduino that utilises the serial port for communication.

## Features

- serial interface for easy mode adjustment
- EEPROM storage to preserve settings even after power down
- 7 static colors
- strobing effect with adjustable rate

## Setup

Adjust the pins numbers in the code. Pins must be pwm to work. Upload code to arduino. After this open serial communication with the arduino, it should reply with instructions.

## Usage

There are two modes static and strobe. Static stays on one color while strobe slowly transitions between them. 

For static mode enter the desired colors letter. The available colors are:
- yellow (y)
- orange (o)
- red (r)
- green (g)
- blue (b)
- purple (p)
- white (w)

For strobe mode enter 's'. The rate of the strobe can then be adjusted up (u) or down (d).

## Additional Configuration

Extra colors can be added using the custom `rgb_t` type using the following syntax.  
`#define <COLOR_NAME> (rgb_t){0, 0, 0}`  
Replace 0, 0, 0 with the desired rgb values to get approximate rgb values use [google color picker](https://g.co/kgs/6BVWJq).

To add the color to the strobing list add it to `possibleColors` and increment `COLORNUM`.  

To add the color as a static color add another case block in the switch state in `adjustRGB()` with the desired color letter. 



