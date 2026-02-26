#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "LedLayout.h"

class LedStrip {
public:
  void begin();
  void clear();
  void show();

  CRGB* leds() { return _leds; }
  const CRGB* leds() const { return _leds; }

private:
  CRGB _leds[LedLayout::LED_COUNT];
};