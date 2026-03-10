#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "RuntimeConfig.h"
#include "AnimCommon.h"

inline void renderBrakeSmallAndLarge(CRGB* leds, uint8_t brightness) {
  // Small
  for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
    leds[LedLayout::RING_SMALL_START + i] = colorBrake(brightness);
  }
  // Mid
  for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
    leds[LedLayout::RING_MID_START + i] = colorBrake(brightness);
  }
  // Large
  for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
    leds[LedLayout::RING_LARGE_START + i] = colorBrake(brightness);
  }
}

inline void renderBrakeSmallAndLarge(CRGB* leds) {
  renderBrakeSmallAndLarge(leds, RuntimeConfig::get().brakeBright);
}