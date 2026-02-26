#include "LedStrip.h"
#include "Pins.h"
#include "Config.h"

void LedStrip::begin() {
  FastLED.addLeds<WS2812, Pins::LED_DATA, GRB>(_leds, LedLayout::LED_COUNT);
  FastLED.setBrightness(Config::LED_BRIGHTNESS_MAX);
  clear();
  show();
}

void LedStrip::clear() {
  fill_solid(_leds, LedLayout::LED_COUNT, CRGB::Black);
}

void LedStrip::show() {
  FastLED.show();
}