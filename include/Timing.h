#pragma once
#include <Arduino.h>

struct SoftTimer {
  uint32_t last = 0;

  bool every(uint32_t periodMs, uint32_t now) {
    if ((uint32_t)(now - last) >= periodMs) {
      last = now;
      return true;
    }
    return false;
  }
};

inline bool inDutyWindow(uint32_t now, uint32_t periodMs, float duty) {
  if (periodMs == 0) return true;
  uint32_t phase = now % periodMs;
  uint32_t onMs = (uint32_t)(periodMs * duty);
  return phase < onMs;
}