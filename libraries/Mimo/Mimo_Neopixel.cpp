#include "Mimo_Neopixel.h"

Mimo_Neopixel::Mimo_Neopixel(uint16_t amount, uint8_t port)
    : BaseType(amount, port, NEO_GRBW + NEO_KHZ800),
      amount_(amount),
      fadeSteps_(256) {
}

void Mimo_Neopixel::fade(rgbw_t start, rgbw_t end, uint8_t steps) {
  for (int i = 0; i < steps; i++)
  {
      BaseType::setPixelColor(
        start.red + (end.red - start.red) * i / steps,
        start.green + (end.green - start.green) * i / steps,
        start.blue + (end.blue - start.blue) * i / steps,
        0
        );
      delay(10);
  }
}

void Mimo_Neopixel::fade(rgbw_t start, rgbw_t end) {
    fade(start, end, fadeSteps_);
}

void Mimo_Neopixel::wipePixelColor(rgbw_t color) {
    for(uint16_t i = 0 ;  i < amount_ ; i++) {
        setPixelColor(i, color);
    }
}

void Mimo_Neopixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    BaseType::setPixelColor(n, r, g, b, w);
}

void Mimo_Neopixel::setPixelColor(uint16_t number, rgbw_t color) {
    BaseType::setPixelColor(number, color.red, color.green, color.blue, color.white);
}