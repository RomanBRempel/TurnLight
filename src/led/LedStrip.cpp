#include "LedStrip.h"
#include "Pins.h"
#include "RuntimeConfig.h"

void LedStrip::begin() {
  FastLED.addLeds<WS2812, Pins::LED_DATA, GRB>(_leds, LedLayout::LED_COUNT);
  FastLED.setBrightness(RuntimeConfig::get().ledBrightnessMax);
  clear();
  show();
}

void LedStrip::clear() {
  fill_solid(_leds, LedLayout::LED_COUNT, CRGB::Black);
}

void LedStrip::show() {
  FastLED.show();
}

void LedStrip::setBrightness(uint8_t value) {
  FastLED.setBrightness(value);
}