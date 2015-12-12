#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>
#include <Button.h>
#include <Mimo_Rgb.h>

#define PIN            10
#define NUMPIXELS      30
#define BUTTON_001     11
#define BUTTON_002     12

const int COLOR_SAMPLE_DELAY = 154;
Adafruit_TCS34725 color_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);
Mimo_Rgb rgb = Mimo_Rgb();
Button button_001 = Button(BUTTON_001, true, true, 25);
Button button_002 = Button(BUTTON_002, true, true, 25);

enum {COLOR_SCAN, FADE};

uint8_t input_state;
rgbw_t last_rgb = rgb.WHITE;
int fade_state = 0;

void setup() {
  pixels.begin();
  wipeColor(last_rgb);
  pixels.show();
  color_sensor.begin();
  color_sensor.setInterrupt(true);
}

void loop() {
  button_001.read();
  button_002.read();
  input();
}

void input() {
  if(button_001.wasPressed()) {
    switch(input_state) {
      case COLOR_SCAN:
      rgbw_t temp = sampleColor();
      fade(last_rgb, temp);
      last_rgb = temp;
      break;
    }
  }
  if(button_002.wasPressed()) {
    switch(input_state) {
      case COLOR_SCAN:
      input_state = FADE;
      break;
      case FADE:
      input_state = COLOR_SCAN;
      break;
    }
  }
  if(input_state == FADE) {
    pulse();
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

void pulse() {
  if(fade_state == 0) {
    fade(rgb.BLUE, rgb.GREEN);
    fade_state = 1;
  } else if(fade_state == 1) {
    fade(rgb.GREEN, rgb.RED);
    fade_state = 2;
  } else if(fade_state == 2) {
    fade(rgb.RED, rgb.BLUE);
    fade_state = 0;
  }
  delay(500);
}

void fade(rgbw_t start, rgbw_t end) {
  int n = 256;
  for (int i = 0; i < n; i++) {
      rgbw_t next;
      next.red = start.red + (end.red - start.red) * i / n;
      next.green = start.green + (end.green - start.green) * i / n;
      next.blue = start.blue + (end.blue - start.blue) * i / n;
      next.white = 0;
      wipeColor(next);
  }
  delay(10);
}

void wipeColor(rgbw_t color) {
  for(int i = 0 ; i < NUMPIXELS ; i++){
    pixels.setPixelColor(i, color.red, color.green, color.blue, color.white);
  }
  pixels.show();
}
