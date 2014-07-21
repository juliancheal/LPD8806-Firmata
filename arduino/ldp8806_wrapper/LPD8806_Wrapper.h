/**
 *
 * This the ws2812 library used for the various examples
 *
 * Most of its job is to parse messages correctly and hand off to the 
 * Adafruit library for more serious tasking.
 *
 * You'll need to call ws2812_initialise() to begin with so it sets up
 * the strip appropriately
 *
 */

#ifndef LPD8806_h
#define LPD8806_h

#define DEBUG true

#include "Arduino.h"
#include "LPD8806.h"

#define LED_PIN 6
#define STRIP_LENGTH 44
#define BUFLENGTH 32

void lpd8806_initialise();
void parse_message(String& message, int message_start);


#endif
