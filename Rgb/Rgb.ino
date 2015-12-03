#include <Arduino.h>

#include <Adafruit_TCS34725.h>
#include <Adafruit_NeoPixel.h>

#define PIN            10
#define NUMPIXELS      1

typedef struct {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
  uint16_t white;
} rgb_t;

const uint8_t PROGMEM gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

unsigned long COLOR_SAMPLE_INTERVAL = 10000;
unsigned long lastColorSampleTime = 0;

//unsigned long CHANGE_COLOR_INTERVAL = 1000;
//unsigned long lastColorChangeTime = 0;

int colorSampleDelay = 154;
Adafruit_TCS34725 color_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

rgb_t last_rgb;
rgb_t last_color_sample;

void setup() {

  last_rgb.red = 188;
  last_rgb.green = 188;
  last_rgb.blue = 188;
  last_rgb.white = 0;
  last_color_sample = last_rgb;
  
  color_sensor.begin();
  color_sensor.setInterrupt(true);
  pixels.begin();

  establishColor(last_rgb);
}

void loop() {
  unsigned long currentMillis = millis();

  if(canProcess(currentMillis, COLOR_SAMPLE_INTERVAL, &lastColorSampleTime)) {
    last_color_sample = sampleColor();
    changeColor(last_rgb, last_color_sample);
    last_rgb = last_color_sample;
  }
  /*
  if(canProcess(currentMillis, CHANGE_COLOR_INTERVAL, &lastColorChangeTime)) {
    changeColor(last_rgb, last_rgb_readout);
    last_rgb = last_rgb_readout;
  }
  */
}

rgb_t sampleColor() {
  uint16_t red, green, blue, white;
  color_sensor.setInterrupt(false);
  delay(colorSampleDelay);
  color_sensor.getRawData(&red, &green, &blue, &white);
  color_sensor.setInterrupt(true);
  
  uint32_t sum = red;
  sum += green;
  sum += blue;
  sum += white;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;

  rgb_t rgb;
  rgb.red = pgm_read_byte(&gamma[(int)r]);
  rgb.green = pgm_read_byte(&gamma[(int)g]);
  rgb.blue = pgm_read_byte(&gamma[(int)b]);
  rgb.white = 0;
  return rgb;
}

void changeColor(rgb_t oldColor, rgb_t newColor) {
  if (oldColor.red != newColor.red || oldColor.green != newColor.green || oldColor.blue != newColor.blue) {
    fadeColor(oldColor, newColor);
  }
}

void fadeColor(rgb_t old, rgb_t nnew) {
  int steps = 256;
  for (int i = 0; i < steps; i++)
  {
      establishColor(
        old.red + (nnew.red - old.red) * i / steps,
        old.green + (nnew.green - old.green) * i / steps,
        old.blue + (nnew.blue - old.blue) * i / steps,
        0
        );
      delay(10);
  }
}

void establishColor(rgb_t color) {
  establishColor(color.red, color.green, color.blue, color.white);
}

void establishColor(int red, int green, int blue, int white) {
  for(int i = 0 ; i < NUMPIXELS ; i++){
    pixels.setPixelColor(i, red, green, blue, white);
  }
  pixels.show();
}

bool canProcess(unsigned long current, unsigned long interval, unsigned long *last) {
  if(current - *last >= interval) {
    *last = current;
    return true;
  }
  return false;
}
