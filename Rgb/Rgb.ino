#include <Arduino.h>

#include <Button.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_NeoPixel.h>
#include <Mimo_Rgb.h>

#define PIN            10
#define BUTTON         11
#define PIXEL_AMOUNT   30

enum {MODE_SWITCH, COLOR_SCAN}; 
uint8_t INPUT_STATE;

Button button = Button(BUTTON, true, true, 25);
const int COLOR_SAMPLE_DELAY = 154;
Adafruit_TCS34725 color_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);
Adafruit_NeoPixel neo_pixel = Adafruit_NeoPixel(PIXEL_AMOUNT, PIN, NEO_GRBW + NEO_KHZ800);
Mimo_Rgb rgb = Mimo_Rgb();

rgbw_t last_rgb;

void setup() {
  INPUT_STATE = MODE_SWITCH;
  last_rgb = rgb.WHITE;
  
  color_sensor.begin();
  color_sensor.setInterrupt(true);
  neo_pixel.begin();
  neo_pixel.show();
}

void loop() {
  button.read();
  input();
}

void input() {
  switch(INPUT_STATE) {
    case MODE_SWITCH:
      changeColor(rgb.BLUE);
      if(button.pressedFor(100)) {
        INPUT_STATE = COLOR_SCAN;
      }
    break;
    case COLOR_SCAN:
      if(button.pressedFor(1000)) {
        INPUT_STATE = MODE_SWITCH;
        break;
      }
      if(button.pressedFor(100)) {
        changeColor(sampleColor());
      }
    break;
  }
}

rgbw_t sampleColor() {
  uint16_t red, green, blue, white;
  color_sensor.setInterrupt(false);
  delay(COLOR_SAMPLE_DELAY);
  color_sensor.getRawData(&red, &green, &blue, &white);
  color_sensor.setInterrupt(true);
  return rgb.convert(red, green, blue, white);
}

void changeColor(rgbw_t color) {
  if (last_rgb.red != color.red || last_rgb.green != color.green || last_rgb.blue != color.blue) {
    fade(last_rgb, color, 256);
    last_rgb = color;
  }
}

void fade(rgbw_t start, rgbw_t end, uint8_t steps) {
  for (int i = 0; i < steps; i++)
  {
      wipePixelColor(
        start.red + (end.red - start.red) * i / steps,
        start.green + (end.green - start.green) * i / steps,
        start.blue + (end.blue - start.blue) * i / steps,
        0
        );
      delay(10);
  }
}

void wipePixelColor(int red, int green, int blue, int white) {
    for(uint16_t i = 0 ;  i < PIXEL_AMOUNT ; i++) {
        neo_pixel.setPixelColor(i, red, green, blue, white);
    }
    neo_pixel.show();  
}
