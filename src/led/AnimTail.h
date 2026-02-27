#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "RuntimeConfig.h"
#include "AnimCommon.h"

inline void renderTail(CRGB* leds, uint8_t scale = 255) {
  const RuntimeConfig::Data& cfg = RuntimeConfig::get();
  // Small ring
  for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
    leds[LedLayout::RING_SMALL_START + i] = colorTail(scale8(cfg.tailSmallBright, scale));
  }
  // Mid ring
  for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
    leds[LedLayout::RING_MID_START + i] = colorTail(scale8(cfg.tailMidBright, scale));
  }
  // Large ring
  for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
    leds[LedLayout::RING_LARGE_START + i] = colorTail(scale8(cfg.tailLargeBright, scale));
  }
}