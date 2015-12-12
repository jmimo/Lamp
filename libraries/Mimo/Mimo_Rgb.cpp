#include "Mimo_Rgb.h"

uint32_t Mimo_Rgb::convert(uint16_t red, uint16_t green, uint16_t blue, uint16_t white) {
  uint32_t sum = red;
  sum += green;
  sum += blue;
  sum += white;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  return Color(gammaCorrect(r), gammaCorrect(g), gammaCorrect(b), 0);
}
