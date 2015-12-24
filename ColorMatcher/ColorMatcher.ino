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

enum { COLOR_SCAN, RANDOM_FADE, RADICAL, DICE};

uint8_t input_state;
uint32_t last_rgb;

unsigned long rf_wait_time = 2000;
unsigned long rf_wait_start = 0;

unsigned long ra_wait_time = 250;
unsigned long ra_wait_start = 0;
int ra_index = 0;

const uint32_t named_colors[] = {
  rgb.Color(128, 0, 0, 0),
  rgb.Color(255, 0, 0, 0),
  rgb.Color(128, 128, 0, 0),
  rgb.Color(255, 255, 0, 0),
  rgb.Color(0, 128, 0, 0),
  rgb.Color(0, 255, 0, 0),
  rgb.Color(0, 128, 128, 0),
  rgb.Color(0, 255, 255, 0),
  rgb.Color(0, 0, 128, 0),
  rgb.Color(0, 0, 255, 0),
  rgb.Color(128, 0, 128, 0),
  rgb.Color(255, 0, 255, 0)
};

void setup() {
  randomSeed(analogRead(0));
  last_rgb = createRandomColor();
  pixels.begin();
  wipeColor(last_rgb);
  pixels.show();
  color_sensor.begin();
  color_sensor.setInterrupt(true);
}

void loop() {
  button_001.read();
  button_002.read();
  state();
  input();
}

void input() {
  if(button_002.wasPressed()) {
    switch(input_state) {
      case COLOR_SCAN:
      input_state = RANDOM_FADE;
      break;
      case RANDOM_FADE:
      input_state = RADICAL;
      break;
      case RADICAL:
      input_state = DICE;
      break;
      case DICE:
      input_state = COLOR_SCAN;
    }
  }
}

void state() {
    switch(input_state) {
    case RANDOM_FADE:
      randomFade();
      break;
    case COLOR_SCAN:
      scanColor();
      break;
    case RADICAL:
      radical();
      break;
    case DICE:
      dice();
      break;
  }
}

void scanColor() {
  if(button_001.wasPressed()) {
    uint32_t temp = sampleColor();
    fade(last_rgb, temp);
    last_rgb = temp;
  }
}

uint32_t sampleColor() {
  uint16_t red, green, blue, white;
  color_sensor.setInterrupt(false);
  delay(COLOR_SAMPLE_DELAY);
  color_sensor.getRawData(&red, &green, &blue, &white);
  color_sensor.setInterrupt(true);
  return rgb.convert(red, green, blue, white);
}

void randomFade() {
  if(millis() - rf_wait_start < rf_wait_time) {
    return;
  }
  uint32_t next = createRandomColor();
  fade(last_rgb, next);
  last_rgb = next;
  rf_wait_start = millis();
}

void radical() {
  if(millis() - ra_wait_start < ra_wait_time) {
    return;
  }
  if (ra_index >= sizeof(named_colors)) {
    ra_index = 0;
  }
  wipeColor(named_colors[ra_index]);
  ++ra_index;
  ra_wait_start = millis();
}

void dice() {
  if(button_001.wasPressed()) {
    last_rgb = createRandomColor();
    wipeColor(last_rgb);
  }
}

uint32_t createRandomColor() {
  return rgb.Color(random(0, 255), random(0, 255), random(0, 255), 0);
}

void fade(uint32_t start, uint32_t end) {
  int n = 256;
  for (int i = 0; i < n; i++) {
      wipeColor(
          rgb.Color(
            rgb.Red(start) + (rgb.Red(end) - rgb.Red(start)) * i / n,
            rgb.Green(start) + (rgb.Green(end) - rgb.Green(start)) * i / n,
            rgb.Blue(start) + (rgb.Blue(end) - rgb.Blue(start)) * i / n,
          0)
        );
      delay(5);
  }
}

void wipeColor(uint32_t color) {
  for(int i = 0 ; i < NUMPIXELS ; i++){
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}
