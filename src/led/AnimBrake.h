#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "Config.h"
#include "AnimCommon.h"

inline void renderBrakeSmallAndLarge(CRGB* leds) {
  // Small
  for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
    leds[LedLayout::RING_SMALL_START + i] = colorBrake(Config::BRAKE_BRIGHT);
  }
  // Mid
  for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
    leds[LedLayout::RING_MID_START + i] = colorBrake(Config::BRAKE_BRIGHT);
  }
  // Large
  for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
    leds[LedLayout::RING_LARGE_START + i] = colorBrake(Config::BRAKE_BRIGHT);
  }
}