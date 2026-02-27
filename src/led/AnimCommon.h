#pragma once
#include <FastLED.h>
#include "RuntimeConfig.h"

// Tail/brake are red; turn is orange.
inline CRGB colorTail(uint8_t bright) {
  // red with chosen brightness
  return CHSV(0, 255, bright);
}

inline CRGB colorBrake(uint8_t bright) {
  return CHSV(0, 255, bright);
}

inline CRGB colorTurn(uint8_t bright) {
  const RuntimeConfig::Data& cfg = RuntimeConfig::get();
  return CRGB(
    scale8(cfg.turnOrangeR, bright),
    scale8(cfg.turnOrangeG, bright),
    scale8(cfg.turnOrangeB, bright)
  );
}