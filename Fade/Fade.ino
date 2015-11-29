#include <Adafruit_NeoPixel.h>

#define PIN            1
#define NUMPIXELS      2

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  pixels.begin();
  //pixels.setBrightness(100);
  pixels.show();
}

int red = true;

void loop() {
  if (red) {
    fadeColor(255, 0, 0, 0, 0, 255);
    red = false;
  } else {
    fadeColor(0, 0, 255, 255, 0, 0);
    red = true;
  }
  delay(10);
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
