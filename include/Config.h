#pragma once
#include <Arduino.h>
#include "Types.h"

namespace Config {
  // ---------- FastLED global brightness ceiling ----------
  static constexpr uint8_t LED_BRIGHTNESS_MAX = 255; // 0..255
  static constexpr uint8_t LED_BRIGHTNESS_TAIL = 160; // tail-only

  // ---------- Startup effect ----------
  static constexpr uint16_t STARTUP_EFFECT_MS = 3000;
  static constexpr uint16_t STARTUP_BREATH_PERIOD_MS = 1200;
  static constexpr uint8_t STARTUP_BREATH_MAX_BRIGHT = 200;

  // ---------- Tail (габарит) brightness by ring ----------
  static constexpr uint8_t TAIL_SMALL_BRIGHT = 190;
  static constexpr uint8_t TAIL_MID_BRIGHT   = 95;
  static constexpr uint8_t TAIL_LARGE_BRIGHT = 75;

  // ---------- Brake (стоп) ----------
  static constexpr uint8_t BRAKE_BRIGHT = 255;

  // ---------- Turn (поворот) ----------
  static constexpr uint16_t TURN_RING_STEP_MS = 180;     // time per ring
  static constexpr uint16_t TURN_RING_CYCLE_PAUSE_MS = 220; // pause after large ring
  static constexpr uint8_t TURN_RING_BRIGHT = 220;
  static constexpr uint16_t TURN_OFF_HOLD_MS = 180;      // require stable LOW
  static constexpr uint16_t TURN_RING_BOUNDARY_WINDOW_MS = 15; // safe stop window

  // Colors (RGB tuning)
  static constexpr uint8_t TURN_ORANGE_R = 255;
  static constexpr uint8_t TURN_ORANGE_G = 90;
  static constexpr uint8_t TURN_ORANGE_B = 0;

  // ---------- Inputs filtering ----------
  static constexpr uint16_t INPUT_DEBOUNCE_MS = 30;
}