#include "Mimo_Rgb.h"

rgbw_t Mimo_Rgb::convert(uint16_t red, uint16_t green, uint16_t blue, uint16_t white) {
  uint32_t sum = red;
  sum += green;
  sum += blue;
  sum += white;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  rgbw_t rgbw;
  rgbw.red = correct(r);
  rgbw.green = correct(g);
  rgbw.blue = correct(b);
  rgbw.white = 0;
}