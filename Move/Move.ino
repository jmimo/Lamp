#include <Adafruit_NeoPixel.h>
#include <Mimo_Rgb.h>

#define PIN            10
#define NUMPIXELS      30

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);
Mimo_Rgb rgb = Mimo_Rgb();

void setup() {
  pixels.begin();
  pixels.show();
  wipeColor(rgb.BLUE);
}

bool done;

void loop() {
  if(done) {
    fadeArray(rgb.RED, rgb.BLUE);
    done = false;
  } else {
    fadeArray(rgb.BLUE, rgb.RED);
    done = true;
  }
  delay(5000);
}

void fadeArray(rgbw_t from, rgbw_t to) {
  int phases[30];
  for(int i = 30 ; i < 0 ; i++) {
    phases[i] = (i + 1) * -1;
  }

  for(int n = 0 ; n < 286 ; n++) {
    for(int m = 0 ; m < 30 ; m++) {
      if(phases[m] >= 0 && phases[m] <= 256) {
        setColor(m, calculateColor(from, to, phases[m], 256));
      }
      phases[m] += 1;
    }
    pixels.show();
    delay(10);
  }
}

rgbw_t calculateColor(rgbw_t start, rgbw_t end, int phase, int steps) {
  rgbw_t result;
  result.red = start.red + (end.red - start.red) * phase / steps;
  result.green = start.green + (end.green - start.green) * phase / steps;
  result.blue = start.blue + (end.blue - start.blue) * phase / steps;
  result.white = 0;
  return result;
}

void setColor(int index, rgbw_t color) {
  pixels.setPixelColor(index, color.red, color.green, color.blue, color.white);
}

void wipeColor(rgbw_t color) {
  for(int i = 0 ; i < NUMPIXELS ; i++){
    pixels.setPixelColor(i, color.red, color.green, color.blue, color.white);
  }
  pixels.show();  
}

