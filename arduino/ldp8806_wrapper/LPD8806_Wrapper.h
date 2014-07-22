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

void lpd8806_initialise();
void parse_message(String& message, int message_start);

void setPixels(uint8_t pos, uint32_t colour);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void colorChase(uint32_t c, uint8_t wait);
void theaterChase(uint32_t c, uint8_t wait);
void theaterChaseRainbow(uint8_t wait);
uint32_t Wheel(uint16_t WheelPos);

#endif