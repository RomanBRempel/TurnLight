#pragma once
#include <Arduino.h>
#include "Types.h"

namespace Config {
  // ---------- FastLED global brightness ceiling ----------
  static constexpr uint8_t LED_BRIGHTNESS_MAX = 180; // 0..255

  // ---------- Tail (габарит) brightness by ring ----------
  static constexpr uint8_t TAIL_SMALL_BRIGHT = 90;
  static constexpr uint8_t TAIL_MID_BRIGHT   = 55;
  static constexpr uint8_t TAIL_LARGE_BRIGHT = 35;

  // ---------- Brake (стоп) ----------
  static constexpr uint8_t BRAKE_BRIGHT = 220;

  // ---------- Turn (поворот) ----------
  static constexpr TurnDirection TURN_DIRECTION = TurnDirection::CW;

  // On mid ring only: 0..(RING_MID_COUNT-1)
  static constexpr uint8_t TURN_START_INDEX_MID_RING = 0;

  static constexpr uint8_t TURN_SEGMENT_LEN = 3;     // 1..8
  static constexpr uint16_t TURN_STEP_MS = 60;       // speed of moving segment

  // blink envelope for turn
  static constexpr uint16_t TURN_BLINK_PERIOD_MS = 650; // ~1.5 Hz
  static constexpr float TURN_BLINK_DUTY = 0.50f;       // 0..1

  // Colors (HSV tuning)
  static constexpr uint8_t TURN_ORANGE_HUE = 24; // FastLED HSV hue for orange-ish

  // ---------- Inputs filtering ----------
  static constexpr uint16_t INPUT_DEBOUNCE_MS = 30;
}