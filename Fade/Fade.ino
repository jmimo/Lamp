#include <Adafruit_NeoPixel.h>

#define PIN            10
#define NUMPIXELS      30

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  pixels.begin();
  setColor(255, 0, 0, 0);
  pixels.show();
}

int state = 0;

void loop() {
  if (state == 0) {
    fadeColor(255, 0, 0, 0, 255, 0);
    state = 1;
  }
  if (state == 1) {
    fadeColor(0, 255, 0, 0, 0, 255);
    state = 2;
  }
  if (state == 2) {
    fadeColor(0, 0, 255, 255, 0, 0);
    state = 0;
  }
  delay(500);
}

void fadeColor(int rStart, int gStart, int bStart, int rEnd, int gEnd, int bEnd) {
  int n = 256;
  for (int i = 0; i < n; i++)
  {
      setColor(
        rStart + (rEnd - rStart) * i / n,
        gStart + (gEnd - gStart) * i / n,
        bStart + (bEnd - bStart) * i / n,
        0
        );
  
      delay(10);
  }
}

void setColor(int red, int green, int blue, int white) {
  for(int i = 0 ; i < NUMPIXELS ; i++){
    pixels.setPixelColor(i, red, green, blue, white);
  }
  pixels.show();
}
