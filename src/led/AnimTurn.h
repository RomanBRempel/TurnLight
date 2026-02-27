#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "RuntimeConfig.h"
#include "Types.h"
#include "AnimCommon.h"

// Renders wave by rings: small -> mid (or large), with pause each cycle.
inline uint32_t turnWaveCycleMs(uint8_t activeRings) {
  const RuntimeConfig::Data& cfg = RuntimeConfig::get();
  return (cfg.turnRingStepMs * activeRings) + cfg.turnRingCyclePauseMs;
}

inline void renderTurnRingsWave(CRGB* leds, uint32_t nowMs, uint8_t activeRings) {
  const uint32_t step = RuntimeConfig::get().turnRingStepMs;
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
  const uint32_t local = phase - (ringIndex * step);
  const uint8_t mix = static_cast<uint8_t>((local * 255UL) / step);
  const uint8_t brightA = scale8(RuntimeConfig::get().turnRingBright, 255 - mix);
  const uint8_t brightB = scale8(RuntimeConfig::get().turnRingBright, mix);

  const CRGB colorA = colorTurn(brightA);
  const CRGB colorB = colorTurn(brightB);

  if (ringIndex == 0) {
    for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
      leds[LedLayout::RING_SMALL_START + i] = colorA;
    }
    if (activeRings > 1) {
      for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
        leds[LedLayout::RING_MID_START + i] = colorB;
      }
    }
  } else if (ringIndex == 1) {
    for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
      leds[LedLayout::RING_MID_START + i] = colorA;
    }
    if (activeRings > 2) {
      for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
        leds[LedLayout::RING_LARGE_START + i] = colorB;
      }
    }
  } else {
    for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
      leds[LedLayout::RING_LARGE_START + i] = colorA;
    }
  }
}