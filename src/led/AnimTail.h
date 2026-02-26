#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "Config.h"
#include "AnimCommon.h"

inline void renderTail(CRGB* leds) {
  // Small ring
  for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
    leds[LedLayout::RING_SMALL_START + i] = colorTail(Config::TAIL_SMALL_BRIGHT);
  }
  // Mid ring
  for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
    leds[LedLayout::RING_MID_START + i] = colorTail(Config::TAIL_MID_BRIGHT);
  }
  // Large ring
  for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
    leds[LedLayout::RING_LARGE_START + i] = colorTail(Config::TAIL_LARGE_BRIGHT);
  }
}