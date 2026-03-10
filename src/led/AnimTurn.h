#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "RuntimeConfig.h"
#include "Types.h"
#include "AnimCommon.h"

// Renders a pulse: all rings blink simultaneously with the configured turn color.
// Returns true when the animation is in the OFF phase (safe to stop).
inline bool isTurnPulseSafeToEnd(uint32_t nowMs) {
  const RuntimeConfig::Data& cfg = RuntimeConfig::get();
  const uint32_t period = cfg.turnPulseOnMs + cfg.turnPulseOffMs;
  if (period == 0) {
    return true;
  }
  const uint32_t phase = nowMs % period;
  return phase >= cfg.turnPulseOnMs; // safe during OFF phase
}

inline void renderTurnPulse(CRGB* leds, uint32_t nowMs) {
  const RuntimeConfig::Data& cfg = RuntimeConfig::get();
  const uint32_t period = cfg.turnPulseOnMs + cfg.turnPulseOffMs;
  if (period == 0) {
    return;
  }

  const uint32_t phase = nowMs % period;
  if (phase >= cfg.turnPulseOnMs) {
    return; // OFF phase — tail layer shows through
  }

  // ON phase — overwrite all rings with turn color
  const CRGB color = colorTurn(cfg.turnRingBright);
  fill_solid(leds, LedLayout::LED_COUNT, color);
}