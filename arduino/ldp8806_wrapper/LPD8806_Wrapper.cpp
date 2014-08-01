#include "LPD8806_Wrapper.h"
#include "Arduino.h"

// Number of RGB LEDs in strand:
// int STRIP_LENGTH = 32;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;

LPD8806 strip = LPD8806(STRIP_LENGTH, dataPin, clockPin);

char buf[BUFLENGTH]; // character buffer for json message processing

void lpd8806_initialise() {
    // initialises the strip
    strip.begin();
    strip.show();
}

void parse_message(String& message, int message_start) {
  // processes the message that has come in

  String msg_string = message.substring(message_start);
  msg_string = msg_string.substring(1, msg_string.lastIndexOf("}"));
  msg_string.replace(" ", "");
  msg_string.replace("\"", "");

  msg_string.toCharArray(buf, BUFLENGTH);

  // iterate over the tokens of the message - assumed flat structure.
  char *p = buf;
  char *str;

  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
  int16_t pos = -1;
  bool colour_change = false;
  bool packed_colour = false; // used to pack the colour into one value.
  uint32_t colour = 0;

  while ((str = strtok_r(p, ",", &p)) != NULL) { 
    char *tp = str;
    char *key; char *val;

    // get the key and it's value.
    key = strtok_r(tp, ":", &tp);
    val = strtok_r(NULL, ":", &tp);

    if (*key == 'r' || *key == 'R') red = atoi(val);
    if (*key == 'g' || *key == 'G') green = atoi(val);
    if (*key == 'b' || *key == 'B') blue = atoi(val);
    if (*key == 'p' || *key == 'P') {
      if (*val == 'a' || *val == 'A') {
        pos = -1; // this means light them all up.
      } else {
        pos = atoi(val);
      }
    }

    if (*key == 'c' || *key == 'C') {
      packed_colour = true;
      colour = atol(val);
    }
  }

  // this just changes the value to the provided vals and limits if needed
  if (pos == -1) {
    // set the whole strand the same colour
    for (uint16_t i = 0; i<STRIP_LENGTH; i++) {
      if (packed_colour) {
        // used packed syntax
        strip.setPixelColor(i, colour);
      } else {
        // used long syntax
        strip.setPixelColor(i, red, green, blue);
      }
    }
  } else {
    // set a pixel
    if (packed_colour) {
      // used packed syntax
      strip.setPixelColor(pos, colour);
    } else {
      strip.setPixelColor(pos, red, green, blue);
    }
  }
  strip.show();
}