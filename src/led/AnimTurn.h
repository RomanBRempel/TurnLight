#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "Config.h"
#include "Types.h"
#include "AnimCommon.h"

// Renders wave by rings: small -> mid (or large), with pause each cycle.
inline uint32_t turnWaveCycleMs(uint8_t activeRings) {
  return (Config::TURN_RING_STEP_MS * activeRings) + Config::TURN_RING_CYCLE_PAUSE_MS;
}

inline void renderTurnRingsWave(CRGB* leds, uint32_t nowMs, uint8_t activeRings) {
  const uint32_t step = Config::TURN_RING_STEP_MS;
  if (step == 0) {
    return;
  }

  if (activeRings < 1 || activeRings > 3) {
    return;
  }

  const uint32_t cycle = turnWaveCycleMs(activeRings);
  if (cycle == 0) {
    return;
  }

  const uint32_t phase = nowMs % cycle;
  if (phase >= (step * activeRings)) {
    return; // pause region
  }

  const uint8_t ringIndex = static_cast<uint8_t>(phase / step); // 0..(activeRings-1)
  const CRGB color = colorTurn(Config::TURN_RING_BRIGHT);

  if (ringIndex == 0) {
    for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
      leds[LedLayout::RING_SMALL_START + i] = color;
    }
  } else if (ringIndex == 1) {
    for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
      leds[LedLayout::RING_MID_START + i] = color;
    }
  } else {
    for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
      leds[LedLayout::RING_LARGE_START + i] = color;
    }
  }
}