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

  int16_t pos = -1;
  uint32_t colour = 0;
  int16_t wait = 0;
  bool rain = false;
  bool rainCycle = false;
  bool wipe = false;
  bool chase = false;
  bool tchase = false;
  bool tChaseRain = false;
  
  
  while ((str = strtok_r(p, ",", &p)) != NULL) { 
    char *tp = str;
    char *key; char *val;

    // get the key and it's value.
    key = strtok_r(tp, ":", &tp);
    val = strtok_r(NULL, ":", &tp);

    if (*key == 'p' || *key == 'P') {
      if (*val == 'a' || *val == 'A') {
        pos = -1; // this means light them all up.
      } else {
        pos = atoi(val);
      }
    }

    if (*key == 'c' || *key == 'C') colour = atol(val);

    if (*key == 'w' || *key == 'W') wait = atoi(val);
    
    if (*key == 't' || *key == 'T') {
      uint32_t color = colour = true;
      if (*val == '1') rain = true;
      if (*val == '2') rainCycle = true;
      if (*val == '3') wipe = true;
      if (*val == '4') chase = true;
      if (*val == '5') tchase = true;
      if (*val == '6') tChaseRain = true;
    }
  }
  if (rain) rainbow(wait);
  if (rainCycle) rainbowCycle(wait);
  if (wipe) colorWipe(colour, wait);
  if (chase) colorChase(colour, wait);
  if (tchase) theaterChase(colour, wait);
  if (tChaseRain) theaterChaseRainbow(wait);
  
  // sendPixel(0,10,0,"a");
  if (pos == -1) {
    // set the whole strand the same colour
    for (uint16_t i = 0; i<STRIP_LENGTH; i++) {
      strip.setPixelColor(i, colour);
    }
  } else {
    strip.setPixelColor(pos, colour);
  }
  strip.show();
}

void setPixels(uint8_t pos, uint32_t colour) {
  // if it's a colour change then lets change the colour.
  // also just check for the position too.
  // this just changes the value to the provided vals and limits if needed
  if (pos == -1) {
    // set the whole strand the same colour
    for (uint16_t i = 0; i<STRIP_LENGTH; i++) {
      strip.setPixelColor(i, colour);
    }
  } else {
    strip.setPixelColor(pos, colour);
  }
  strip.show();
}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}