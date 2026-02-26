#pragma once
#include <Arduino.h>

namespace LedLayout {
  static constexpr uint16_t RING_SMALL_COUNT = 7;
  static constexpr uint16_t RING_MID_COUNT   = 8;
  static constexpr uint16_t RING_LARGE_COUNT = 12;

  // Default mapping: [small][mid][large] in one chain
  static constexpr uint16_t RING_SMALL_START = 0;
  static constexpr uint16_t RING_MID_START   = RING_SMALL_START + RING_SMALL_COUNT; // 7
  static constexpr uint16_t RING_LARGE_START = RING_MID_START + RING_MID_COUNT;     // 15

  static constexpr uint16_t LED_COUNT = RING_SMALL_COUNT + RING_MID_COUNT + RING_LARGE_COUNT; // 27
}