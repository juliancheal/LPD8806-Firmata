/**
 *
 * This the LPD8806 library used for the various examples
 *
 * Most of its job is to parse messages correctly and hand off to the 
 * Adafruit library for more serious tasking.
 *
 * You'll need to call lpd8806_initialise() to begin with so it sets up
 * the strip appropriately
 *
 */
 
#ifndef LPD8806_h
#define LPD8806_h

#define DEBUG true

#include "Arduino.h"
#include "LPD8806.h"

#define LED_PIN 6
#define STRIP_LENGTH 32
#define BUFLENGTH 32

void
 lpd8806_initialise();
 parse_message(String& message, int message_start);
 setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
 setPixelColor(uint16_t n, uint32_t c);
 show(void);

uint16_t
  numPixels(void);
uint32_t
  Color(byte, byte, byte),
  getPixelColor(uint16_t n);


#endif