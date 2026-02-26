#pragma once
#include <FastLED.h>
#include "Config.h"

// Tail/brake are red; turn is orange.
inline CRGB colorTail(uint8_t bright) {
  // red with chosen brightness
  return CHSV(0, 255, bright);
}

inline CRGB colorBrake(uint8_t bright) {
  return CHSV(0, 255, bright);
}

inline CRGB colorTurn(uint8_t bright) {
  return CRGB(
    scale8(Config::TURN_ORANGE_R, bright),
    scale8(Config::TURN_ORANGE_G, bright),
    scale8(Config::TURN_ORANGE_B, bright)
  );
}