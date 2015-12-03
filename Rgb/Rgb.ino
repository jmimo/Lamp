#include <Arduino.h>

#include <Button.h>
#include <Adafruit_TCS34725.h>
#include <Mimo_Neopixel.h>
#include <Mimo_Rgb.h>

#define PIN            10
#define BUTTON         11
#define PIXEL_AMOUNT   1

enum {MODE_SWITCH, COLOR_SCAN}; 
uint8_t INPUT_STATE;

Button button = Button(BUTTON, true, true, 25);
const int COLOR_SAMPLE_DELAY = 154;
Adafruit_TCS34725 color_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);
Mimo_Neopixel mimo = Mimo_Neopixel(PIXEL_AMOUNT, PIN);
Mimo_Rgb rgb = Mimo_Rgb();

rgbw_t last_rgb;

void setup() {
  INPUT_STATE = MODE_SWITCH;
  last_rgb = rgb.WHITE;
  
  color_sensor.begin();
  color_sensor.setInterrupt(true);
  mimo.begin();
  mimo.show();
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

void changeColor(rgbw_t newColor) {
  if (last_rgb.red != newColor.red || last_rgb.green != newColor.green || last_rgb.blue != newColor.blue) {
    mimo.fade(last_rgb, newColor);
    last_rgb = newColor;
  }
}
