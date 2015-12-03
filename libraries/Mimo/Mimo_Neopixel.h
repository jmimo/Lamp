#ifndef MIMO_LAMP_H
#define MIMO_LAMP_H

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <Mimo_Rgb.h>

class Mimo_Neopixel : public Adafruit_NeoPixel {

public:    
    explicit Mimo_Neopixel(uint16_t amount, uint8_t port);
    
    typedef Adafruit_NeoPixel BaseType;
    
    void fade(rgbw_t start, rgbw_t end, uint8_t steps);
    void fade(rgbw_t start, rgbw_t end);

    void wipePixelColor(rgbw_t color);
    
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void setPixelColor(uint16_t number, rgbw_t color);
    
private:
    uint16_t amount_;
    uint8_t fadeSteps_;
};

#endif