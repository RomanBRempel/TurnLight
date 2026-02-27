#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "RuntimeConfig.h"
#include "AnimCommon.h"

inline void renderBrakeSmallAndLarge(CRGB* leds) {
  const RuntimeConfig::Data& cfg = RuntimeConfig::get();
  // Small
  for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
    leds[LedLayout::RING_SMALL_START + i] = colorBrake(cfg.brakeBright);
  }
  // Mid
  for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
    leds[LedLayout::RING_MID_START + i] = colorBrake(cfg.brakeBright);
  }
  // Large
  for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
    leds[LedLayout::RING_LARGE_START + i] = colorBrake(cfg.brakeBright);
  }
}