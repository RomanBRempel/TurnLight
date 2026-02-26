#pragma once
#include <FastLED.h>
#include "LedLayout.h"
#include "Config.h"
#include "Types.h"
#include "../utils/Timing.h"
#include "AnimCommon.h"

// Renders moving segment on mid ring only.
// - segment position advances with TURN_STEP_MS
// - whole effect blinks with TURN_BLINK_PERIOD_MS / DUTY
inline void renderTurnMidRing(CRGB* leds, uint32_t nowMs) {
  if (!inDutyWindow(nowMs, Config::TURN_BLINK_PERIOD_MS, Config::TURN_BLINK_DUTY)) {
    // during OFF phase: do nothing (leave tail/brake underneath)
    return;
  }

  const uint8_t count = LedLayout::RING_MID_COUNT;
  const uint16_t base = LedLayout::RING_MID_START;

  // step index
  uint32_t step = (Config::TURN_STEP_MS == 0) ? 0 : (nowMs / Config::TURN_STEP_MS);
  int pos = (int)(Config::TURN_START_INDEX_MID_RING + (step % count));

  if (Config::TURN_DIRECTION == TurnDirection::CCW) {
    pos = (int)(Config::TURN_START_INDEX_MID_RING - (int)(step % count));
  }

  // normalize
  while (pos < 0) pos += count;
  pos %= count;

  // draw segment with a small brightness gradient tail
  for (uint8_t k = 0; k < Config::TURN_SEGMENT_LEN; k++) {
    int idx = pos + k;
    idx %= count;

    // front brighter, tail dimmer
    uint8_t v = 220;
    if (Config::TURN_SEGMENT_LEN > 1) {
      // simple linear falloff
      v = (uint8_t)(220 - (k * (140 / (Config::TURN_SEGMENT_LEN - 1))));
    }
    leds[base + idx] = colorTurn(v);
  }
}